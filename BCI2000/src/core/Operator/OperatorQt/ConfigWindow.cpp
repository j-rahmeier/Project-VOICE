//////////////////////////////////////////////////////////////////////
// $Id: ConfigWindow.cpp 8334 2024-09-02 16:36:44Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: The Operator module's parameter configuration dialog.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2023: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
///////////////////////////////////////////////////////////////////////
#include "ConfigWindow.h"
#include "ui_ConfigWindow.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QMessageBox>
#include <QScrollArea>
#include <QScrollBar>
#include <QTemporaryFile>
#include <QWhatsThis>
#include <QBoxLayout>
#include "CollapsibleGroup.h"

#include <algorithm>
#include <string>
#include <vector>

#include "BCI2000FileReader.h"
#include "ExecutableHelp.h"
#include "FileUtils.h"
#include "OperatorUtils.h"
#include "ParamList.h"
#include "Preferences.h"
#include "ShowParameters.h"

#ifdef TODO
# error Remove global gpConfig pointer, use ConfigWindow::Instance()
#endif
ConfigWindow *gpConfig = NULL;

static ConfigWindow* spInstance = nullptr;

ConfigWindow* ConfigWindow::Instance()
{
    return spInstance;
}

ConfigWindow::ConfigWindow(QWidget *parent)
    : QDialog(parent), m_ui(new Ui::ConfigWindow),
    mUserSwitchedTabs(false), mpScrollArea(NULL), mScrollPos(-1),
    mpParameters(nullptr)
{
    spInstance = this;
    m_ui->setupUi(this);
    connect(this, SIGNAL(finished(int)), this, SLOT(OnClose()));
    connect(m_ui->bConfigureLoad, SIGNAL(clicked()), this, SLOT(OnConfigureLoadFilterClick()));
    connect(m_ui->bConfigureSave, SIGNAL(clicked()), this, SLOT(OnConfigureSaveFilterClick()));
    connect(m_ui->bHelp, SIGNAL(clicked()), this, SLOT(OnHelpClick()));
    connect(m_ui->bLoadParameters, SIGNAL(clicked()), this, SLOT(OnLoadParametersClick()));
    connect(m_ui->bSaveParameters, SIGNAL(clicked()), this, SLOT(OnSaveParametersClick()));
    connect(m_ui->bClose, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_ui->cfgTabControl, SIGNAL(currentChanged(int)), this, SLOT(OnCfgTabControlChange()));
    OperatorUtils::RestoreWidget(this);
    if (ExecutableHelp().ParamHelp().Empty())
        m_ui->bHelp->setVisible(false);
    mOriginalTitle = Preferences::Instance()->mInstancePrefix + this->windowTitle();
    this->setWindowTitle(mOriginalTitle);
}

ConfigWindow::~ConfigWindow()
{
    OperatorUtils::SaveWidget(this);
    DisposeWidgets();
    delete m_ui;
    spInstance = nullptr;
}

void ConfigWindow::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

static bool CompareTabNames(const std::string &s1, const std::string &s2)
{ // This comparison function makes sure that standard tabs have standard positions.
    static const char *leftTabs_[] = {
        "Visualize",
        "SignalSharing",
        "System",
        "Source",
        "Storage",
    };
    static std::vector<std::string> leftTabs(leftTabs_, leftTabs_ + sizeof(leftTabs_) / sizeof(*leftTabs_));
    std::vector<std::string>::const_iterator i1 = leftTabs.begin();
    for (; i1 != leftTabs.end(); ++i1)
        if (s1.find(*i1) == 0)
            break;
    std::vector<std::string>::const_iterator i2 = leftTabs.begin();
    for (; i2 != leftTabs.end(); ++i2)
        if (s2.find(*i2) == 0)
            break;
    if (i1 == i2)
        return s1 < s2;
    return i1 < i2;
}

int ConfigWindow::Initialize(ParamList *inParameters)
{
    mpParameters = inParameters;

    mpParameters->Sort();
    DeleteAllTabs();

    std::vector<std::string> tabNames;
    int globalUserLevel = Preferences::Instance()->mUserLevel;
    for (const auto& param : *mpParameters)
    {
        if (OperatorUtils::GetUserLevel(param.Path().c_str()) <= globalUserLevel)
        {
            std::string tabName = param.Section();
            if (std::find(tabNames.begin(), tabNames.end(), tabName) == tabNames.end())
                tabNames.push_back(tabName);
        }
    }
    stable_sort(tabNames.begin(), tabNames.end(), CompareTabNames);

    mUserSwitchedTabs = false;
    for (const auto& name : tabNames)
    {
        QScrollArea *pScrollArea = new QScrollArea;
        pScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_ui->cfgTabControl->addTab(pScrollArea, name.c_str());
        pScrollArea->setWidget(new QLabel);
    }
    mUserSwitchedTabs = true;

    if (m_ui->cfgTabControl->count() == 0)
    {
        if (mpParameters->Empty())
        {
            m_ui->cfgTabControl->insertTab(0, new QWidget, tr("No parameters available"));
        }
        else
        {
            m_ui->cfgTabControl->insertTab(0, new QWidget, tr("No parameter visible"));
            QMessageBox::information(this, tr("Message"), tr("No parameter visible! Increase user level"),
                                     QMessageBox::Yes | QMessageBox::Default);
        }
    }
    if (std::find(tabNames.begin(), tabNames.end(), mCurTab) == tabNames.end())
        mCurTab = "Storage";
    if (std::find(tabNames.begin(), tabNames.end(), mCurTab) == tabNames.end())
        mCurTab = "Filtering";
    if (std::find(tabNames.begin(), tabNames.end(), mCurTab) == tabNames.end())
        if (!tabNames.empty())
            mCurTab = tabNames.front();
    mUserSwitchedTabs = false;
    int curTabIdx = std::find(tabNames.begin(), tabNames.end(), mCurTab) - tabNames.begin();
    if (curTabIdx != tabNames.size())
        m_ui->cfgTabControl->setCurrentIndex(curTabIdx);
    mUserSwitchedTabs = true;
    mCurTab = m_ui->cfgTabControl->tabText(curTabIdx).toLocal8Bit().constData();
    RenderParameters(mCurTab);
    if (mScrollPos >= 0 && mpScrollArea)
        mpScrollArea->verticalScrollBar()->setValue(mScrollPos);
    return 0;
}

void ConfigWindow::DeleteAllTabs()
{
    mUserSwitchedTabs = false;
    // delete old Tabs, if present
    while (m_ui->cfgTabControl->count() > 0)
        m_ui->cfgTabControl->removeTab(0);
    mUserSwitchedTabs = true;
}

// render all parameters in a particular section on the screen
void ConfigWindow::RenderParameters(const std::string &inSection)
{
    if (inSection.empty())
        return;

    DisposeWidgets();
    mParamDisplays.clear();

    std::string lastSubsection = "";
    CollapsibleGroup* pLastGroup = nullptr;
    QWidget *pTabWidget = m_ui->cfgTabControl->currentWidget();
    mpScrollArea = dynamic_cast<QScrollArea *>(pTabWidget);
    if (mpScrollArea == NULL)
        return;
    QWidget *pScrollingPane = new QWidget(mpScrollArea);
    delete mpScrollArea->widget();
    mpScrollArea->setWidget(nullptr);
    QVBoxLayout* pScrollingPaneLayout = new QVBoxLayout;

    std::map<std::string, int> subsectionIndex;
    std::vector<std::string> subsectionTable;
    std::vector<std::vector<int>> subsectionGroups;

    int globalUserLevel = Preferences::Instance()->mUserLevel;
    for (int i = 0; i < mpParameters->Size(); ++i)
    {
        const Param &p = mpParameters->ByIndex(i);
        if (inSection == p.Section() && OperatorUtils::GetUserLevel(p.Path().c_str()) <= globalUserLevel)
        {
            std::string subsection;
            if (p.Sections().size() > 1)
                subsection = p.Sections()[1];
            if (subsectionIndex.find(subsection) == subsectionIndex.end())
            {
                subsectionIndex[subsection] = static_cast<int>(subsectionTable.size());
                subsectionTable.push_back(subsection);
                subsectionGroups.resize(subsectionGroups.size() + 1);
            }
            subsectionGroups[subsectionIndex[subsection]].push_back(i);
        }
    }

    for (const auto& subsection : subsectionTable)
    {
        if (globalUserLevel > Preferences::Beginner)
        { // A group box for each subsection.
            if (pLastGroup != nullptr)
            {
                pScrollingPaneLayout->addWidget(pLastGroup);
                pLastGroup = nullptr;
            }
            if (subsection != "")
            {
                QString title = QString::fromUtf8(subsection.c_str());
                CollapsibleGroup* pGroup = new CollapsibleGroup(title, this);
                QFont font = pGroup->titleFont();
                font.setBold(true);
                pGroup->setTitleFont(font);
                pGroup->setContentLayout(new QVBoxLayout);
                pGroup->setCollapsed(OperatorUtils::GetSubsectionCollapsed(title.toUtf8()));
                connect(pGroup, &CollapsibleGroup::toggled, this, &ConfigWindow::OnSubsectionCollapsed);
                pLastGroup = pGroup;
            }
        }
        int groupIndex = subsectionIndex[subsection];
        for (auto paramIndex : subsectionGroups[groupIndex])
        {
            const Param &p = mpParameters->ByIndex(paramIndex);
            ParamDisplay* pParamDisplay = new ParamDisplay(p);
            pParamDisplay->ReadValuesFrom(p);
            mParamDisplays[p.Path()] = pParamDisplay;
            if (pLastGroup)
                pLastGroup->contentLayout()->addWidget(pParamDisplay->GetWidget());
            else
                pScrollingPaneLayout->addWidget(pParamDisplay->GetWidget());
        }
    }
    if (pLastGroup != nullptr)
        pScrollingPaneLayout->addWidget(pLastGroup);
    pScrollingPaneLayout->addStretch();
    pScrollingPane->setLayout(pScrollingPaneLayout);
    mpScrollArea->setWidget(pScrollingPane);
    mpScrollArea->setWidgetResizable(true);
    int margins = layout()->contentsMargins().left()
                + layout()->contentsMargins().right();
    m_ui->cfgTabControl->setMinimumWidth(pScrollingPane->minimumSizeHint().width() + margins);
}

// update one particular parameter on the screen
// useful, for example, if parameters change while stuff on screen
void ConfigWindow::RenderParameter(Param *inParam)
{
    if (mParamDisplays.find(inParam->Path()) != mParamDisplays.end())
        mParamDisplays[inParam->Path()]->ReadValuesFrom(*inParam);
}

// go through the parameters on the screen and update the parameters using the data on the screen
void ConfigWindow::UpdateParameters()
{
    for (const auto& entry : mParamDisplays)
        if (entry.second->Modified())
            entry.second->WriteValuesTo(mpParameters->ByPath(entry.first));
}

void ConfigWindow::OnSubsectionCollapsed(bool)
{
    auto pGroup = dynamic_cast<CollapsibleGroup*>(sender());
    if (pGroup)
        OperatorUtils::SetSubsectionCollapsed(pGroup->title().toUtf8(), pGroup->isCollapsed());
}

void ConfigWindow::OnCfgTabControlChange()
{
    if (!mUserSwitchedTabs)
        return;

    UpdateParameters();
    int curTabIdx = m_ui->cfgTabControl->currentIndex();
    mCurTab = m_ui->cfgTabControl->tabText(curTabIdx).toLocal8Bit().constData();
    RenderParameters(mCurTab);
}

void ConfigWindow::OnClose()
{
    if (mpScrollArea)
        mScrollPos = mpScrollArea->verticalScrollBar()->value();
    UpdateParameters();
    mParamDisplays.clear();
    DisposeWidgets();
    DeleteAllTabs();
}

void ConfigWindow::OnSaveParametersClick()
{
    // Avoid Qt warning about empty path when calling canonicalPath().
    QString path = Preferences::Instance()->mCurrentDir.path();
    if (!path.isEmpty())
        path = Preferences::Instance()->mCurrentDir.canonicalPath();
    QString fileName =
        QFileDialog::getSaveFileName(this, tr("Save Parameter File"), path,
                                     tr("BCI2000 parameter files (*.prm)"));
    if (!fileName.isEmpty())
    {
        Preferences::Instance()->mCurrentDir.setPath(QFileInfo(fileName).canonicalPath());
        UpdateParameters();

        ParamList paramsToSave;
        for (const auto& param : *mpParameters)
            if (0 == OperatorUtils::GetFilterStatus(param.Name().c_str(), OperatorUtils::saveFilter))
                paramsToSave.Add(param);

        bool result = paramsToSave.Save(fileName.toLocal8Bit().data());
        if (result)
            this->setWindowTitle(mOriginalTitle + tr(" - wrote %1").arg(fileName));
        else
            QMessageBox::critical(this, tr("Error"), tr("Error writing parameter file"));
    }
}

bool ConfigWindow::LoadParameters(const QString &inName)
{
    ParamList paramsFromFile = *mpParameters;
    paramsFromFile.Unchanged();
    bool result = false;
    BCI2000FileReader reader(inName.toLocal8Bit().constData());
    if (reader.IsOpen())
    {
        result = paramsFromFile.CopyFrom(*reader.Parameters(), false);
        reader.Close();
    }
    else
    {
        result = paramsFromFile.Load(inName.toLocal8Bit().constData(), false);
    }

    for (const auto& p : paramsFromFile)
    {
        Param &q = mpParameters->ByPath(p.Path());
        if (p.Changed() && 0 == OperatorUtils::GetFilterStatus(q.Path().c_str(), OperatorUtils::loadFilter))
            q.AssignValues(p, true);
    }
    return result;
}

void ConfigWindow::DisposeWidgets()
{
  // Widgets are automatically disposed by Qt
}

void ConfigWindow::OnLoadParametersClick()
{
    // Avoid Qt warning about empty path when calling canonicalPath().
    QString path = Preferences::Instance()->mCurrentDir.path();
    if (!path.isEmpty())
        path = Preferences::Instance()->mCurrentDir.canonicalPath();
    QString fileName =
        QFileDialog::getOpenFileName(NULL, tr("Open Parameter File"), path,
            tr("BCI2000 parameter file (*.prm);;BCI2000 data file (*.dat *.bcidat);;Any file (*.*)"));
    if (!fileName.isEmpty())
    {
        Preferences::Instance()->mCurrentDir.setPath(QFileInfo(fileName).canonicalPath());
        UpdateParameters();
        bool result = false;
        QString error;
        try {
            result = LoadParameters(fileName);
        }
        catch (const std::exception& e) {
            error = e.what();
        }
        if (result)
        {
            this->setWindowTitle(mOriginalTitle + tr(" - read %1").arg(fileName));
            RenderParameters(mCurTab);
        }
        else
        {
            if (!error.isEmpty())
                error = ": " + error;
            error = tr("Error reading parameter file ") + fileName + error;
            if (!error.endsWith("."))
                error += ".";
            QMessageBox::critical(this, tr("Error"), error);
        }
    }
}

void ConfigWindow::OnConfigureSaveFilterClick()
{
    ShowParameters dialog(this, *mpParameters, OperatorUtils::saveFilter);
    dialog.exec();
}

void ConfigWindow::OnConfigureLoadFilterClick()
{
    ShowParameters dialog(this, *mpParameters, OperatorUtils::loadFilter);
    dialog.exec();
}

void ConfigWindow::OnHelpClick()
{
    QWhatsThis::enterWhatsThisMode();
}
