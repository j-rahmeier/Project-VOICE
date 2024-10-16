////////////////////////////////////////////////////////////////////////////////
// $Id: MainWindow.cpp 8341 2024-09-09 15:19:13Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: The main window of the BCI2000Launcher tool.
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
////////////////////////////////////////////////////////////////////////////////
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "AboutBox.h"
#include "EncodedString.h"
#include "ExecutableHelp.h"
#include "Settings.h"
#include "SysError.h"
#include "Resources.h"
#include "ProcessUtils.h"

#include <QtWidgets>
#include <signal.h>
#include <sstream>
#include <string>
#include <regex>

// The following arrays contain current or former modules from
// the BCI2000 core/contrib directories as of r3545 (Sept 2011).
// To catch modules located in private/custom, and future core/contrib modules,
// we rely on the mechanism available via src/shared/config/ModuleInventory.
static const char *sKnownSourceModules[] = {
    "SignalGenerator", "gUSBampSource",   "gMOBIlab",        "gMOBIlabPlus",
    "TDTclient",       "ModularEEG",      "DT2000",          "RDAClient",
    "DASSource",       "NISource",        "AmpServerPro",    "BioRadio",
    "Biosemi2",        "Emotiv",          "ENOBIO",          "FieldTripBufferSource",
    "FilePlayback",    "Micromed",        "NeuroscanClient", "NeuroscanAccess",
    "NeuroSky",        "NIDAQ_mx_Source", "PythonSource",    "TMSi",
    "TuckerDavis",     "vAmpSource",      "Grapevine",
};

static const char *sKnownSigprocModules[] = {
    "DummySignalProcessing",
    "ARSignalProcessing",
    "P3SignalProcessing",
    "FIRProcessing",
    "PeakDetectSignalProcessing",
    "SWSignalProcessing",
    "CoherenceFFTSignalProcessing",
    "CoherenceSignalProcessing",
    "FFTSignalProcessing",
    "FieldTripBuffer",
    "MatlabSignalProcessing",
    "PythonSignalProcessing",
    "SigfriedSigProc",
};

static const char *sKnownApplicationModules[] = {
    "Screen",
    "D3Box",
    "D2Box",
    "P3AVTask",
    "P3Speller",
    "RJB",
    "Oddball",
    "Speller",
    "SWApp",
    "SWSpeller",
    "StimulusPresentation",
    "FeedbackDemo",
    "CursorTask",
    "DummyApplication",
    "PythonApplication",
    "MFCDemo",
};

static const char* sSuppressOtherModules[] = {
    "BCI2000Command",
    "BCI2000RemoteDemo",
    "BCI2000Shell",
    "Install.*",
    "Setup.*",
    "MessageBox",
};

static std::string EncodeParamValue(const char* value)
{
    std::ostringstream oss;
    EncodedString(value).InsertInto(oss, ";");
    return oss.str();
}

static std::string SubstituteVars(const std::map<std::string, std::string>& vars, const std::string& text)
{
    std::string result = text;
    for (const auto& var : vars) {
        std::string ref = "%" + var.first + "%";
        size_t pos = 0;
        while ((pos = result.find(ref)) != std::string::npos)
            result = result.substr(0, pos) + var.second + result.substr(pos + ref.length());
    }
    return result;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle(QApplication::applicationName());
    SetupProgramsGroup();
    SetupExtensionsGroup();
    SetupParametersGroup();
    SetupScriptingCommandsGroup();
    SetupStatusGroup();
    SetupGlobalLayout();
    SetupActions();

    mDefaultStateVertical = mpVerticalSplitter->saveState();
    mDefaultStateHorizontal = mpHorizontalSplitter->saveState();

    ReadSettings();

    SetupInventory();
    QStringList programs = ScanForPrograms();
    DistributePrograms(programs);
    onProgramSelectionChanged();
}

MainWindow::~MainWindow()
{
    SaveSettings();
    delete ui;
}

void MainWindow::SetupProgramsGroup()
{
    // Setup layouts in the programs group
    QHBoxLayout *pHBoxProg = new QHBoxLayout;
    QVBoxLayout *pVBoxProg = new QVBoxLayout;

    QVBoxLayout *pVBoxSrc = new QVBoxLayout;
    pVBoxSrc->addWidget(new QLabel(tr("Signal Source")));
    pVBoxSrc->addWidget(ui->sourceList);
    mpSourceSelected = new QLabel;
    pVBoxSrc->addWidget(mpSourceSelected);
    pHBoxProg->addLayout(pVBoxSrc);
    connect(ui->sourceList, &QListWidget::itemSelectionChanged, this, &MainWindow::onProgramSelectionChanged);

    QVBoxLayout *pVBoxSigproc = new QVBoxLayout;
    pVBoxSigproc->addWidget(new QLabel(tr("Signal Processing")));
    pVBoxSigproc->addWidget(ui->sigprocList);
    mpSigProcSelected = new QLabel;
    pVBoxSigproc->addWidget(mpSigProcSelected);
    pHBoxProg->addLayout(pVBoxSigproc);
    connect(ui->sigprocList, &QListWidget::itemSelectionChanged, this, &MainWindow::onProgramSelectionChanged);

    QVBoxLayout *pVBoxApp = new QVBoxLayout;
    pVBoxApp->addWidget(new QLabel(tr("Application")));
    pVBoxApp->addWidget(ui->applicationList);
    mpAppSelected = new QLabel;
    pVBoxApp->addWidget(mpAppSelected);
    pHBoxProg->addLayout(pVBoxApp);
    connect(ui->applicationList, &QListWidget::itemSelectionChanged, this, &MainWindow::onProgramSelectionChanged);

    pHBoxProg->addWidget(ui->otherGroup);
    QHBoxLayout *pHBoxOther = new QHBoxLayout;
    pHBoxOther->addWidget(ui->otherList);
    ui->otherGroup->setLayout(pHBoxOther);

    pVBoxProg->addLayout(pHBoxProg);
    pVBoxProg->addWidget(ui->clearSelectionBtn);

    ui->programsGroup->setLayout(pVBoxProg);
    ui->programsGroup->setTitle(tr("BCI2000 Programs"));
}

void MainWindow::SetupExtensionsGroup()
{
    QLayout* pLayout = new QHBoxLayout;
    pLayout->addWidget(ui->extensionsList);
    ui->extensionsGroup->setLayout(pLayout);
    ui->extensionsList->setHeaderHidden(true);
    connect(ui->extensionsList, &QTreeWidget::itemChanged, this, &MainWindow::onExtensionItemChanged);
    connect(ui->extensionsList, &QTreeWidget::itemCollapsed, this, &MainWindow::onExtensionCollapsedChanged);
    connect(ui->extensionsList, &QTreeWidget::itemExpanded, this, &MainWindow::onExtensionCollapsedChanged);
}

void MainWindow::SetupScriptingCommandsGroup()
{
    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->addWidget(ui->scriptingCommandsEdit);
    ui->scriptingCommandsGroup->setLayout(pLayout);
}

void MainWindow::SetupParametersGroup()
{
    // Setup layouts in the parameters group
    QVBoxLayout *pVBoxParm = new QVBoxLayout, *pVBox1 = new QVBoxLayout, *pVBox2 = new QVBoxLayout;
    QHBoxLayout *pHBox1 = new QHBoxLayout, *pHBox2 = new QHBoxLayout, *pHBox3 = new QHBoxLayout,
                *pHBox11 = new QHBoxLayout;

    pVBox2->addWidget(ui->addParmfileBtn);
    pVBox2->addWidget(ui->removeParmfileBtn);
    pVBox2->addStretch();

    pHBox1->addWidget(ui->paramfilesList);
    pHBox1->addLayout(pVBox2);

    pVBox1->addWidget(new QLabel(tr("Parameter Files")));
    pVBox1->addLayout(pHBox1);

    pHBox11->addWidget(new QLabel(tr("Data File Format")));
    ui->fileFormatComboBox->addItem(tr("BCI2000 native"), "BCI2000");
    ui->fileFormatComboBox->addItem("EDF", "EDF");
    ui->fileFormatComboBox->addItem("GDF", "GDF");
    ui->fileFormatComboBox->addItem(tr("no recording"), "Null");
    pHBox11->addWidget(ui->fileFormatComboBox);
    pHBox11->addStretch();

    pHBox2->addWidget(new QLabel(tr("Save Directory")));
    pHBox2->addWidget(ui->saveDirEdit);
    pHBox2->addWidget(ui->chooseDirBtn);

    pHBox3->addWidget(new QLabel(tr("Subject Name")));
    pHBox3->addWidget(ui->subjectNameEdit);
    pHBox3->addWidget(new QLabel(tr("Session #")));
    pHBox3->addWidget(ui->subjectSessionEdit);

    pVBoxParm->addLayout(pVBox1);
    pVBoxParm->addLayout(pHBox2);
    pVBoxParm->addLayout(pHBox3);
    pVBoxParm->addLayout(pHBox11);
    ui->parametersGroup->setLayout(pVBoxParm);
}

void MainWindow::SetupStatusGroup()
{
    // Setup layouts in the status group
    QVBoxLayout *pVBoxStatus = new QVBoxLayout;
    pVBoxStatus->addWidget(ui->statusEdit);
    pVBoxStatus->addWidget(ui->launchButton);
    ui->statusGroup->setLayout(pVBoxStatus);
}

void MainWindow::SetupGlobalLayout()
{
    mpVerticalSplitter = new QSplitter(Qt::Vertical);
    mpVerticalSplitter->setChildrenCollapsible(false);
    mpVerticalSplitter->addWidget(ui->programsGroup);
    mpHorizontalSplitter = new QSplitter(Qt::Horizontal);
    mpHorizontalSplitter->setChildrenCollapsible(false);
    mpHorizontalSplitter->addWidget(ui->extensionsGroup);
    mpHorizontalSplitter->addWidget(ui->scriptingCommandsGroup);
    mpHorizontalSplitter->addWidget(ui->parametersGroup);
    mpVerticalSplitter->addWidget(mpHorizontalSplitter);
    mpVerticalSplitter->addWidget(ui->statusGroup);
    QLayout* pLayout = new QHBoxLayout;
    pLayout->addWidget(mpVerticalSplitter);
    centralWidget()->setLayout(pLayout);
}

void MainWindow::SetupActions()
{
    // Setup context menus in the programs group
    ui->sourceList->addAction(ui->actionTo_Others);
    ui->sourceList->addAction(ui->actionTo_Signal_Source);
    ui->sourceList->addAction(ui->actionTo_Signal_Processing);
    ui->sourceList->addAction(ui->actionTo_Applications);
    ui->sourceList->addAction(ui->actionUp);
    ui->sourceList->addAction(ui->actionDown);

    ui->sigprocList->addAction(ui->actionTo_Others);
    ui->sigprocList->addAction(ui->actionTo_Signal_Source);
    ui->sigprocList->addAction(ui->actionTo_Signal_Processing);
    ui->sigprocList->addAction(ui->actionTo_Applications);
    ui->sigprocList->addAction(ui->actionUp);
    ui->sigprocList->addAction(ui->actionDown);

    ui->applicationList->addAction(ui->actionTo_Others);
    ui->applicationList->addAction(ui->actionTo_Signal_Source);
    ui->applicationList->addAction(ui->actionTo_Signal_Processing);
    ui->applicationList->addAction(ui->actionTo_Applications);
    ui->applicationList->addAction(ui->actionUp);
    ui->applicationList->addAction(ui->actionDown);

    ui->otherList->addAction(ui->actionTo_Others);
    ui->otherList->addAction(ui->actionTo_Signal_Source);
    ui->otherList->addAction(ui->actionTo_Signal_Processing);
    ui->otherList->addAction(ui->actionTo_Applications);
    ui->otherList->addAction(ui->actionUp);
    ui->otherList->addAction(ui->actionDown);

    ui->paramfilesList->addAction(ui->actionUp);
    ui->paramfilesList->addAction(ui->actionDown);
}

void MainWindow::SetupInventory()
{
#define RANGE(x) (x), (x) + sizeof(x) / sizeof(*x)
    mInventory[bci::Inventory::SignalSource].insert(RANGE(sKnownSourceModules));
    mInventory[bci::Inventory::SignalProcessing].insert(RANGE(sKnownSigprocModules));
    mInventory[bci::Inventory::Application].insert(RANGE(sKnownApplicationModules));
}

QStringList MainWindow::ScanForPrograms()
{
    QDir dir(FileUtils::InstallationDirectory().c_str());
    dir.setFilter(QDir::Files | QDir::Executable);
    dir.setSorting(QDir::Name | QDir::IgnoreCase);
    QStringList entries = dir.entryList(), programs;
    for (const auto& entry : entries)
        programs.append(QFileInfo(entry).completeBaseName());
    // Remove the program itself.
    programs.removeOne(QFileInfo(QApplication::applicationFilePath()).completeBaseName());
    // Remove all operator modules.
    for (const auto& prog : mInventory[bci::Inventory::Operator])
        programs.removeOne(QString::fromLocal8Bit(prog.c_str()));
    // Remove all tools.
    for (const auto& prog : mInventory[bci::Inventory::Tool])
        programs.removeOne(QString::fromLocal8Bit(prog.c_str()));
    // Remove all helpers.
    for (const auto& prog : mInventory[bci::Inventory::Helper])
      programs.removeOne(QString::fromLocal8Bit(prog.c_str()));
    return programs;
}

void MainWindow::DistributePrograms(const QStringList &inList)
{
    // Build a list of all programs known by the application.
    QStringList knownPrograms;
    for (int i = 0; i < ui->sourceList->count(); ++i)
        knownPrograms.append(ui->sourceList->item(i)->text());
    for (int i = 0; i < ui->sigprocList->count(); ++i)
        knownPrograms.append(ui->sigprocList->item(i)->text());
    for (int i = 0; i < ui->applicationList->count(); ++i)
        knownPrograms.append(ui->applicationList->item(i)->text());
    for (int i = 0; i < ui->otherList->count(); ++i)
        knownPrograms.append(ui->otherList->item(i)->text());
    QStringList unknownPrograms;
    for (QString program : inList)
    {
        if (knownPrograms.contains(program, Qt::CaseSensitive))
            knownPrograms.removeOne(program);
        else
            unknownPrograms.append(program);
    }
    for (QString program : knownPrograms)
        ClearProgramEntries(program);
    for (QString program : unknownPrograms)
        IntroduceNewProgram(program);
}

void MainWindow::ClearProgramEntries(const QString &inProgram)
{ // Remove a program's entries from all program lists.
    QList<QListWidgetItem *> items = ui->sourceList->findItems(inProgram, Qt::MatchExactly);
    items.append(ui->sigprocList->findItems(inProgram, Qt::MatchExactly));
    items.append(ui->applicationList->findItems(inProgram, Qt::MatchExactly));
    items.append(ui->otherList->findItems(inProgram, Qt::MatchExactly));
    for (QListWidgetItem *item : items)
        delete item;
}

void MainWindow::IntroduceNewProgram(const QString &inProgram)
{ // Introduce a new program into the programs group.
    // Check against the lists of known modules first.
    // If not found there, add to the "others" list.
    const bci::Inventory::mapped_type &sources = mInventory[bci::Inventory::SignalSource];
    if (sources.find(inProgram.toLocal8Bit().constData()) != sources.end())
    {
        ui->sourceList->addItem(inProgram);
        return;
    }

    const bci::Inventory::mapped_type &sigprocs = mInventory[bci::Inventory::SignalProcessing];
    if (sigprocs.find(inProgram.toLocal8Bit().constData()) != sigprocs.end())
    {
        ui->sigprocList->addItem(inProgram);
        return;
    }

    const bci::Inventory::mapped_type &apps = mInventory[bci::Inventory::Application];
    if (apps.find(inProgram.toLocal8Bit().constData()) != apps.end())
    {
        ui->applicationList->addItem(inProgram);
        return;
    }

    for (const char* pattern : sSuppressOtherModules)
    {
        std::regex regex(pattern, std::regex_constants::icase);
        if (std::regex_match(inProgram.toLocal8Bit().constData(), regex))
            return;
    }

    ui->otherList->addItem(inProgram);
}

QStringList MainWindow::GetExtensions(const QString& program) const
{
    QProcess process;
    process.setProgram(FileUtils::InstallationDirectory().c_str() + program);
    process.setWorkingDirectory(FileUtils::InstallationDirectory().c_str());
    process.setArguments({"--Extensions"});
    process.start();
    process.waitForFinished();
    QByteArray data = process.readAllStandardOutput();
    QString stringdata = QString::fromUtf8(data.constData());
    stringdata.replace("\r\n", "\n");
    QStringList list = stringdata.split("\n", Qt::SkipEmptyParts);
    if (!list.isEmpty())
        list.pop_front();
    return list;
}

QString MainWindow::ExtensionArgs(const QString& category) const
{
    QString args;
    for (const auto& enabler : mExtensionEnablerState)
        if (enabler.second.checked && enabler.second.category == category)
            args += " --" + enabler.first + "=1";
    return args;
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}

void MainWindow::onProgramSelectionChanged()
{
    ui->extensionsList->clear();
    auto pSender = dynamic_cast<QListWidget*>(sender());
    if (!pSender)
        pSender = ui->sourceList;

    QString category = "other";
    QLabel* pLabel = nullptr;
    if (pSender == ui->sourceList) {
        category = "source";
        pLabel = mpSourceSelected;
    }
    else if (pSender == ui->sigprocList) {
        category = "sigproc";
        pLabel = mpSigProcSelected;
    }
    else if (pSender == ui->applicationList) {
        category = "app";
        pLabel = mpAppSelected;
    }
    auto selectedItems = pSender->selectedItems();
    QString text;
    if (!selectedItems.empty())
        text = "<b>" + selectedItems.first()->text() + "</b>";
    if (pLabel)
        pLabel->setText(text);
    if (selectedItems.empty())
        return;

    QListWidgetItem* pSelectedItem = selectedItems.front();
    QStringList extensions = GetExtensions(pSelectedItem->text());
    extensions.sort();
    for (const auto& extension : extensions) {
        QStringList enablers = extension.split("\t");
        QTreeWidgetItem* pTopItem = new QTreeWidgetItem({enablers[0]});
        for (int i = 1; i < enablers.size(); ++i) {
            auto pItem = new QTreeWidgetItem(pTopItem, {enablers[i]});
            pItem->setFlags(pItem->flags() | Qt::ItemIsUserCheckable);
            bool checked = mExtensionEnablerState[enablers[i]].checked;
            mExtensionEnablerState[enablers[i]].category = category;
            pItem->setData(0, Qt::CheckStateRole, checked ? Qt::Checked : Qt::Unchecked);
        }
        ui->extensionsList->addTopLevelItem(pTopItem);
        pTopItem->setExpanded(!mExtensionCollapsedState[pTopItem->text(0)]);
    }
}

void MainWindow::onExtensionItemChanged(QTreeWidgetItem* pItem)
{
    int value = pItem->data(0, Qt::CheckStateRole).toInt();
    mExtensionEnablerState[pItem->text(0)].checked = value > 0;
}

void MainWindow::onExtensionCollapsedChanged(QTreeWidgetItem* pItem)
{
    bool expanded = pItem->isExpanded();
    mExtensionCollapsedState[pItem->text(0)] = !expanded;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutBox().SetApplicationName(QApplication::applicationName().toLocal8Bit().constData()).Display();
}

void MainWindow::on_actionReset_View_triggered()
{
    mpVerticalSplitter->restoreState(mDefaultStateVertical);
    mpHorizontalSplitter->restoreState(mDefaultStateHorizontal);
}

void MainWindow::on_actionCreate_Batch_File_triggered()
{
    QString dir = (FileUtils::InstallationDirectoryS() + "/../batch").c_str();
    QString file = QFileDialog::getSaveFileName(this, tr("Choose a name for the batch file"), dir, tr("BCI2000 Batch file (*.bat)"));
    if (file.isEmpty())
        return;

    std::string signalSource = "# no signal source module selected",
                signalProcessing = "# no signal processing module selected",
                application = "# no application module selected";
    QList<QListWidgetItem*> list = ui->sourceList->selectedItems();
    if (!list.isEmpty()) {
        signalSource = "Start executable ";
        signalSource += list.first()->text().toUtf8().constData();
        signalSource += " --local ";
        signalSource += " --FileFormat=";
        signalSource += ui->fileFormatComboBox->currentData().toString().toLocal8Bit().constData();
        signalSource += ExtensionArgs("source").toUtf8().constData();
    }
    list = ui->sigprocList->selectedItems();
    if (!list.isEmpty()) {
        signalProcessing = "Start executable ";
        signalProcessing += list.first()->text().toUtf8().constData();
        signalProcessing += " --local ";
        signalProcessing += ExtensionArgs("sigproc").toUtf8().constData();
    }
    list = ui->applicationList->selectedItems();
    if (!list.isEmpty()) {
        application = "Start executable ";
        application += list.first()->text().toUtf8().constData();
        application += " --local ";
        application += ExtensionArgs("app").toUtf8().constData();
    }
    std::ostringstream scriptingCommands;
    WriteCommands("\r\n", scriptingCommands);

    std::map<std::string, std::string> vars;
    vars["SignalSourceCommand"] = signalSource;
    vars["SignalProcessingCommand"] = signalProcessing;
    vars["ApplicationCommand"] = application;
    vars["ScriptingCommands"] = scriptingCommands.str();

    std::string contents(bci::Resources::BatchFile_bat_in.data, bci::Resources::BatchFile_bat_in.length);
    contents = SubstituteVars(vars, contents);

    std::ofstream out(file.toLocal8Bit().constData(), std::ios::binary);
    out.write(contents.data(), contents.length());
}

void MainWindow::on_actionBCI2000_Help_triggered()
{
    ExecutableHelp().Display();
}

void MainWindow::MoveListItem(QListWidget *pTarget)
{
    QListWidget *pListWidget = qobject_cast<QListWidget *>(this->focusWidget());
    if (pListWidget != NULL && pListWidget != pTarget)
    {
        for (QListWidgetItem *pItem : pListWidget->selectedItems())
        {
            QString text = pItem->text();
            delete pItem;
            pTarget->addItem(text);
        }
    }
}

void MainWindow::on_actionTo_Others_triggered()
{
    MoveListItem(ui->otherList);
}

void MainWindow::on_actionTo_Applications_triggered()
{
    MoveListItem(ui->applicationList);
}

void MainWindow::on_actionTo_Signal_Processing_triggered()
{
    MoveListItem(ui->sigprocList);
}

void MainWindow::on_actionTo_Signal_Source_triggered()
{
    MoveListItem(ui->sourceList);
}

void MainWindow::SaveSettings() const
{
    Settings settings;
    settings.beginGroup("Config");
    settings.setValue("Geometry", this->saveGeometry());
    settings.setValue("VerticalSplitter", mpVerticalSplitter->saveState());
    settings.setValue("HorizontalSplitter", mpHorizontalSplitter->saveState());
    settings.setValue("WorkingDirectory", QDir::currentPath());

    settings.beginWriteArray("Source Modules");
    for (int i = 0; i < ui->sourceList->count(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("Name", ui->sourceList->item(i)->text());
        settings.setValue("Selected", ui->sourceList->item(i)->isSelected());
    }
    settings.endArray();

    settings.beginWriteArray("Signal Processing Modules");
    for (int i = 0; i < ui->sigprocList->count(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("Name", ui->sigprocList->item(i)->text());
        settings.setValue("Selected", ui->sigprocList->item(i)->isSelected());
    }
    settings.endArray();

    settings.beginWriteArray("Application Modules");
    for (int i = 0; i < ui->applicationList->count(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("Name", ui->applicationList->item(i)->text());
        settings.setValue("Selected", ui->applicationList->item(i)->isSelected());
    }
    settings.endArray();

    settings.beginWriteArray("Other Modules");
    for (int i = 0; i < ui->otherList->count(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("Name", ui->otherList->item(i)->text());
        settings.setValue("Selected", ui->otherList->item(i)->isSelected());
    }
    settings.endArray();

    settings.beginWriteArray("ExtensionEnablers");
    int i = 0;
    for (const auto& entry : mExtensionEnablerState)
    {
        settings.setArrayIndex(i);
        settings.setValue("Name", entry.first);
        settings.setValue("Checked", entry.second.checked);
        settings.setValue("Category", entry.second.category);
        ++i;
    }
    settings.endArray();

    settings.beginWriteArray("ExtensionsCollapsed");
    i = 0;
    for (const auto& entry : mExtensionCollapsedState)
    {
        settings.setArrayIndex(i);
        settings.setValue("Name", entry.first);
        settings.setValue("Collapsed", entry.second);
        ++i;
    }
    settings.endArray();

    settings.setValue("ScriptingCommands", ui->scriptingCommandsEdit->toPlainText());

    settings.beginWriteArray("Parameter Files");
    for (int i = 0; i < mParameterFiles.count(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("Name", mParameterFiles.at(i));
    }
    settings.endArray();

    settings.setValue("Save Directory", ui->saveDirEdit->text());
    settings.setValue("Subject Name", ui->subjectNameEdit->text());
    settings.setValue("Subject Session", ui->subjectSessionEdit->text());
    settings.setValue("Data File Format", ui->fileFormatComboBox->currentData().toString());

    settings.endGroup();
}

void MainWindow::ReadSettings()
{
    Settings settings;
    settings.beginGroup("Config");
    this->restoreGeometry(settings.value("Geometry").toByteArray());
    mpVerticalSplitter->restoreState(settings.value("VerticalSplitter").toByteArray());
    mpHorizontalSplitter->restoreState(settings.value("HorizontalSplitter").toByteArray());
    QString wd = settings.value("WorkingDirectory").toString();
    if (wd.isEmpty())
        wd = FileUtils::InstallationDirectory().c_str();
    QDir::setCurrent(wd);

    ui->sourceList->clear();
    int size = settings.beginReadArray("Source Modules");
    for (int i = 0; i < size; ++i)
    {
        settings.setArrayIndex(i);
        ui->sourceList->addItem(settings.value("Name").toString());
        ui->sourceList->item(i)->setSelected(settings.value("Selected").toBool());
    }
    settings.endArray();

    auto selectedItems = ui->sourceList->selectedItems();
    if (!selectedItems.isEmpty())
        ui->sourceList->scrollToItem(selectedItems.front(), QAbstractItemView::PositionAtCenter);

    ui->sigprocList->clear();
    size = settings.beginReadArray("Signal Processing Modules");
    for (int i = 0; i < size; ++i)
    {
        settings.setArrayIndex(i);
        ui->sigprocList->addItem(settings.value("Name").toString());
        ui->sigprocList->item(i)->setSelected(settings.value("Selected").toBool());
    }
    settings.endArray();
    selectedItems = ui->sigprocList->selectedItems();
    if (!selectedItems.isEmpty())
        ui->sigprocList->scrollToItem(selectedItems.front(), QAbstractItemView::PositionAtCenter);

    ui->applicationList->clear();
    size = settings.beginReadArray("Application Modules");
    for (int i = 0; i < size; ++i)
    {
        settings.setArrayIndex(i);
        ui->applicationList->addItem(settings.value("Name").toString());
        ui->applicationList->item(i)->setSelected(settings.value("Selected").toBool());
    }
    settings.endArray();
    selectedItems = ui->applicationList->selectedItems();
    if (!selectedItems.isEmpty())
        ui->applicationList->scrollToItem(selectedItems.front(), QAbstractItemView::PositionAtCenter);

    ui->otherList->clear();
    size = settings.beginReadArray("Other Modules");
    for (int i = 0; i < size; ++i)
    {
        settings.setArrayIndex(i);
        ui->otherList->addItem(settings.value("Name").toString());
        ui->otherList->item(i)->setSelected(settings.value("Selected").toBool());
    }
    settings.endArray();
    selectedItems = ui->otherList->selectedItems();
    if (!selectedItems.isEmpty())
        ui->otherList->scrollToItem(selectedItems.front(), QAbstractItemView::PositionAtCenter);

    size = settings.beginReadArray("ExtensionEnablers");
    for (int i = 0; i < size; ++i)
    {
        settings.setArrayIndex(i);
        QString name = settings.value("Name").toString();
        mExtensionEnablerState[name].checked = settings.value("Checked").toBool();
        mExtensionEnablerState[name].category = settings.value("Category").toString();
    }
    settings.endArray();

    size = settings.beginReadArray("ExtensionsCollapsed");
    for (int i = 0; i < size; ++i)
    {
        settings.setArrayIndex(i);
        QString name = settings.value("Name").toString();
        mExtensionCollapsedState[name] = settings.value("Collapsed").toBool();
    }
    settings.endArray();

    ui->scriptingCommandsEdit->setPlainText(settings.value("ScriptingCommands").toString());

    mParameterFiles.clear();
    ui->paramfilesList->clear();
    size = settings.beginReadArray("Parameter Files");
    for (int i = 0; i < size; ++i)
    {
        settings.setArrayIndex(i);
        mParameterFiles.append(settings.value("Name").toString());
        ui->paramfilesList->addItem(QFileInfo(mParameterFiles.last()).completeBaseName());
    }
    settings.endArray();

    ui->saveDirEdit->setText(settings.value("Save Directory").toString());
    ui->subjectNameEdit->setText(settings.value("Subject Name").toString());
    ui->subjectSessionEdit->setText(settings.value("Subject Session").toString());
    int idx = ui->fileFormatComboBox->findData(settings.value("Data File Format").toString());
    if (idx < 0) {
        idx = 0;
    }
    ui->fileFormatComboBox->setCurrentIndex(idx);

    settings.endGroup();
}

void MainWindow::on_clearSelectionBtn_clicked()
{
    ui->sourceList->clearSelection();
    ui->sigprocList->clearSelection();
    ui->applicationList->clearSelection();
    ui->otherList->clearSelection();
}

void MainWindow::on_actionUp_triggered()
{
    QListWidget *pListWidget = qobject_cast<QListWidget *>(this->focusWidget());
    if (pListWidget != NULL)
    {
        if (pListWidget->currentRow() > 0)
        {
            int targetRow = pListWidget->currentRow() - 1;
            QListWidgetItem *pItem = pListWidget->takeItem(pListWidget->currentRow());
            pListWidget->insertItem(targetRow, pItem);
            pListWidget->setCurrentRow(targetRow);
        }
    }
}

void MainWindow::on_actionDown_triggered()
{
    QListWidget *pListWidget = qobject_cast<QListWidget *>(this->focusWidget());
    if (pListWidget != NULL)
    {
        if (pListWidget->currentRow() >= 0 && pListWidget->currentRow() < pListWidget->count() - 1)
        {
            int targetRow = pListWidget->currentRow() + 1;
            QListWidgetItem *pItem = pListWidget->takeItem(pListWidget->currentRow());
            pListWidget->insertItem(targetRow, pItem);
            pListWidget->setCurrentRow(targetRow);
        }
    }
}

void MainWindow::on_addParmfileBtn_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select parameter file(s) to apply"),
                                                          QDir::currentPath(), tr("BCI2000 Parameter Files (*.prm)"));
    if (!fileNames.empty())
    {
        QDir::setCurrent(QFileInfo(fileNames.first()).absolutePath());
        for (QString file : fileNames)
        {
            ui->paramfilesList->addItem(QFileInfo(file).completeBaseName());
            mParameterFiles.append(file);
        }
    }
}

void MainWindow::on_removeParmfileBtn_clicked()
{
    int idx = ui->paramfilesList->currentRow();
    if (idx >= 0)
    {
        QStringList::iterator i = mParameterFiles.begin() + idx;
        mParameterFiles.erase(i);
        delete ui->paramfilesList->takeItem(idx);
    }
}

void MainWindow::on_chooseDirBtn_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select top level directory to save sessions into"),
                                                    QDir::currentPath());
    if (!dir.isEmpty())
    {
        QDir::setCurrent(dir);
        ui->saveDirEdit->setText(dir);
        ui->saveDirEdit->setToolTip(dir);
    }
}

void MainWindow::on_subjectSessionEdit_editingFinished()
{
    QLineEdit *pSender = qobject_cast<QLineEdit *>(sender());
    if (pSender != NULL)
    {
        QString text = pSender->text();
        while (text.length() < 3)
            text = QString("0") + text;
        pSender->setText(text);
    }
}

void MainWindow::SyncToolTip()
{
    QLineEdit *pSender = qobject_cast<QLineEdit *>(sender());
    if (pSender != NULL)
        pSender->setToolTip(pSender->text());
}

void MainWindow::WriteCommands(const std::string& separator, std::ostream& os) const
{
    for (int i = 0; i < mParameterFiles.size(); ++i)
        os << "Load parameterfile " << EncodedString(mParameterFiles.at(i).toLocal8Bit().constData()) << separator;
    if (!ui->saveDirEdit->text().isEmpty())
        os << "Set parameter DataDirectory " << EncodeParamValue(ui->saveDirEdit->text().toLocal8Bit().constData()) << separator;
    if (!ui->subjectNameEdit->text().isEmpty())
        os << "Set parameter SubjectName " << EncodeParamValue(ui->subjectNameEdit->text().toLocal8Bit().constData()) << separator;
    if (!ui->subjectSessionEdit->text().isEmpty())
        os << "Set parameter SubjectSession " << EncodeParamValue(ui->subjectSessionEdit->text().toLocal8Bit().constData()) << separator;
    QStringList lines = ui->scriptingCommandsEdit->toPlainText().split('\n');
    for (const auto& line : lines)
        os << line.toUtf8().constData() << separator;
    if (mParameterFiles.size() > 0)
        os << "SetConfig" << separator;
}

void MainWindow::on_subjectNameEdit_textChanged(QString)
{
    SyncToolTip();
}

void MainWindow::on_saveDirEdit_textChanged(QString)
{
    SyncToolTip();
}

void MainWindow::on_subjectSessionEdit_textChanged(QString)
{
    SyncToolTip();
}

void MainWindow::on_launchButton_clicked()
{
    std::string progdir = FileUtils::InstallationDirectory();

    ui->statusEdit->clear();
    ui->statusEdit->append(tr("Launching %1...").arg("Operator"));
    ui->statusEdit->repaint();

    std::ostringstream args, prog;
    std::string operatorProg = progdir + "Operator";
#if __APPLE__
    if (!FileUtils::IsFile(operatorProg))
        operatorProg += ".app/Contents/macOS/Operator";
#endif
    args << " --Startup localhost";

    args << " --OnConnect \"-";
    WriteCommands("; ", args);
    args << "\"";

    if (!StartExecutable(operatorProg, args.str(), progdir))
        return;

    args.str("");
    prog.str("");
    if (ui->sourceList->selectedItems().empty())
    {
        ui->statusEdit->append(tr("No Source Module selected..."));
        ui->statusEdit->repaint();
    }
    else
    {
        QListWidgetItem *item = ui->sourceList->selectedItems().first();
        ui->statusEdit->append(tr("Launching %1...").arg(item->text()));
        ui->statusEdit->repaint();
        prog << progdir << item->text().toLocal8Bit().constData();
        args << " --local";
        args << ExtensionArgs("source").toLocal8Bit().constData();
        args << " --FileFormat=" << ui->fileFormatComboBox->currentData().toString().toLocal8Bit().constData();
        if (!StartExecutable(prog.str(), args.str(), progdir))
            return;
    }

    args.str("");
    prog.str("");
    if (ui->sigprocList->selectedItems().empty())
    {
        ui->statusEdit->append(tr("No Signal Processing Module selected..."));
        ui->statusEdit->repaint();
    }
    else
    {
        QListWidgetItem *item = ui->sigprocList->selectedItems().first();
        ui->statusEdit->append(tr("Launching %1...").arg(item->text()));
        ui->statusEdit->repaint();
        prog << progdir << item->text().toLocal8Bit().constData();
        args << " --local";
        args << ExtensionArgs("sigproc").toLocal8Bit().constData();
        if (!StartExecutable(prog.str(), args.str(), progdir))
            return;
    }

    prog.str("");
    args.str("");
    if (ui->applicationList->selectedItems().empty())
    {
        ui->statusEdit->append(tr("No Application Module selected..."));
        ui->statusEdit->repaint();
    }
    else
    {
        QListWidgetItem *item = ui->applicationList->selectedItems().first();
        ui->statusEdit->append(tr("Launching %1...").arg(item->text()));
        ui->statusEdit->repaint();
        prog << progdir << item->text().toLocal8Bit().constData();
        args << " --local";
        args << ExtensionArgs("app").toLocal8Bit().constData();
        if (!StartExecutable(prog.str(), args.str(), progdir))
            return;
    }

    for (QListWidgetItem *item : ui->otherList->selectedItems())
    {
        ui->statusEdit->append(tr("Launching %1...").arg(item->text()));
        ui->statusEdit->repaint();
        prog.str("");
        prog << "\"" << progdir << item->text().toLocal8Bit().constData();
        if (!StartExecutable(prog.str(), "", progdir))
            return;
    }

    ui->statusEdit->append(tr("Finished."));
}

bool MainWindow::StartExecutable(const std::string& inExecutable, const std::string& inArgs, const std::string& inWorkingDir)
{
    std::string oldWd = FileUtils::WorkingDirectory();
    FileUtils::ChangeDirectory(inWorkingDir);
    int exitCode = 0;
    bool result = ProcessUtils::ExecuteAsynchronously(inExecutable, inArgs, exitCode);
    FileUtils::ChangeDirectory(oldWd);
    if (!result)
    {
        ui->statusEdit->append(SysError().Message());
        ui->statusEdit->repaint();
    }
    return result;
}

