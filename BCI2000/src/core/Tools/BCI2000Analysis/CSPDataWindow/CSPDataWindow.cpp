////////////////////////////////////////////////////////////////////////////////
// $Id: CSPDataWindow.cpp 8268 2024-07-23 16:38:37Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Data window for Common Spatial Patterns.
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
#include "CSPDataWindow.h"

#include "AnalysisWindow.h"
#include "AnalysisData.h"
#include "SignalWidget.h"
#include "SignalDisplay.h"
#include "TopoplotWidget.h"
#include "FileDialog.h"
#include "EditMenu.h"
#include "WindowMenu.h"
#include "HelpMenu.h"
#include "WindowSettings.h"
#include "WindowFactory.h"

#include <QBoxLayout>
#include <QGroupBox>
#include <QSlider>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QFormLayout>
#include <QLineEdit>
#include <QValidator>
#include <QProgressDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QKeyEvent>

#include <Eigen/Dense>

#include <fstream>

using Eigen::MatrixXf;
using Eigen::VectorXf;

struct CSPDataWindow::Private : QObject
{
    Private(QMainWindow* pParent) : QObject(pParent), mpSelf(pParent) {}

    void setWindowTitleFromFile(const QString&);

    void onApplySpatialFilterParameters();
    void onSaveSpatialFilterParameters();
    void onSaveAllFilterParameters();
    
    void onOpenAnalysisData();
    void onSaveAnalysisData();

    void onNextComponent();
    void onPreviousComponent();

    void onSliderChanged(int);
    void onRetainCheckBoxToggled(bool);
    void onPatternClicked();
    void onFieldClicked();
    void onComponentEdited(const QString&);

    void setAnalysisData(const AnalysisData&);
    void progress();
    void setSpatialFilterParameters(const GenericSignal& patterns);

    void computeCSP(const AnalysisData&);
    MatrixXf computeCovariance(const std::vector<GenericSignal>&);

    AnalysisData mAnalysisData;
    GenericSignal mLogValues, mPatterns, mFields;
    enum { pattern, field } mMode = pattern;
    int mNumComponents = 0, mComponentIndex = 0;
    std::vector<bool> mRetain;
    float mLogMax = 0;

    TopoplotWidget* mpTopoplotWidget = nullptr;
    SignalWidget* mpSignalWidget = nullptr;
    QGroupBox* mpTopoplotGroupBox = nullptr;
    QLineEdit* mpComponentLineEdit = nullptr;
    QSlider* mpComponentSlider = nullptr;
    QCheckBox* mpRetainCheckBox = nullptr;
    QProgressDialog* mpProgress = nullptr;
    AnalysisWindow* mpAnalysisWindow = nullptr;
    QMainWindow* mpSelf;
};

void CSPDataWindow::Private::progress()
{
    if (mpProgress)
        mpProgress->setValue(mpProgress->value() + 1);

}

void CSPDataWindow::Private::setWindowTitleFromFile(const QString& file)
{
    mpSelf->setWindowTitle(WindowMenu::windowTitleFromFile("CSP Analysis Data", file));
}

void CSPDataWindow::Private::setSpatialFilterParameters(const GenericSignal& patterns)
{
    ParamList& params = mAnalysisData.filterParams;

    std::string spatialFilterTypeName = mAnalysisData.absoluteParameterPath("SpatialFilterType");
    if (!params.Exists(spatialFilterTypeName))
        params.Add("Filtering int " + spatialFilterTypeName + "= 0");
    MutableParamRef SpatialFilterType(&params.ByPath(spatialFilterTypeName));

    std::string spatialFilterName = mAnalysisData.absoluteParameterPath("SpatialFilter");
    if (!params.Exists(spatialFilterName))
        params.Add("Filtering matrix " + spatialFilterName + "= 0 0");
    MutableParamRef SpatialFilter(&params.ByPath(spatialFilterName));

    SpatialFilterType = 1; // full matrix
    int numRows = 0;
    for (const auto& retain : mRetain)
        if (retain)
            ++numRows;

    SpatialFilter->SetDimensions(numRows, patterns.Channels());
    for (int col = 0; col < SpatialFilter->NumColumns(); ++col)
    {
        int row = 0;
        for (int el = 0; el < patterns.Channels(); ++el)
        {
            if (mRetain[el])
                SpatialFilter(row++, col) = patterns(col, el);
        }
    }
}

void CSPDataWindow::Private::onApplySpatialFilterParameters()
{
    setSpatialFilterParameters(mPatterns);
    auto pWindow = WindowMenu::validateWindow(mpAnalysisWindow);
    if (pWindow)
        pWindow->setFilterParameters(mAnalysisData.filterParams, mpSelf);
}

void CSPDataWindow::Private::onSaveSpatialFilterParameters()
{
    setSpatialFilterParameters(mPatterns);
    std::string spatialFilterTypeName = mAnalysisData.absoluteParameterPath("SpatialFilter/SpatialFilterType"),
                spatialFilterName = mAnalysisData.absoluteParameterPath("SpatialFilter/SpatialFilter");
    Assert(mAnalysisData.filterParams.Exists(spatialFilterTypeName));
    Assert(mAnalysisData.filterParams.Exists(spatialFilterName));
    ParamList sfParams;
    sfParams.Add(mAnalysisData.filterParams.ByPath(spatialFilterTypeName));
    sfParams.Add(mAnalysisData.filterParams.ByPath(spatialFilterName));
    auto filename =
        FileDialog::getSaveFileName(mpSelf, "Save Spatial Filter Parameters", "BCI2000 Parameter File (*.prm)");
    if (!filename.isEmpty())
    {
        sfParams.Save(filename.toLocal8Bit().constData());
    }
}

void CSPDataWindow::Private::onSaveAllFilterParameters()
{
    auto filename =
        FileDialog::getSaveFileName(mpSelf, "Save All Filter Parameters", "BCI2000 Parameter File (*.prm)");
    if (!filename.isEmpty())
    {
        mAnalysisData.filterParams.Save(filename.toLocal8Bit().constData());
    }
}

void CSPDataWindow::Private::onOpenAnalysisData()
{
    QString file = FileDialog::getOpenFileName(mpSelf, "Open Analysis Data",
        "BCI2000 Analysis Data File (*.bcianalysis)", "output");
    if (!file.isEmpty())
    {
        std::ifstream stream(file.toLocal8Bit().constData(), std::ios::binary);
        AnalysisData data;
        try
        {
            data = AnalysisData::unserialize(stream);
        }
        catch (const std::exception& e)
        {
            QMessageBox::critical(
                mpSelf, "Error reading file",
                QString("An error occurred when reading from file\n\"%1\":\n%2").arg(file).arg(e.what()));
            return;
        }
        if (data.kind.find("SpatialFilter"))
        {
            setAnalysisData(data);
            setWindowTitleFromFile(file);
            mpAnalysisWindow = nullptr;
        }
        else
        {
            auto pWindow = WindowFactory::create(nullptr, data, file);
            if (pWindow)
                pWindow->show();
        }
    }
}

void CSPDataWindow::Private::onSaveAnalysisData()
{
    QString file = FileDialog::getSaveFileName(mpSelf, "Save Analysis Data",
        "BCI2000 Analysis Data File (*.bcianalysis)", "output");
    if (!file.isEmpty())
    {
        std::ofstream stream(file.toLocal8Bit().constData(), std::ios::binary);
        if (!stream)
        {
            QMessageBox::critical(mpSelf, "Error writing file",
                QString("Error opening file\n%1\nfor writing").arg(file));
            return;
        }
        try
        {
            mAnalysisData.serialize(stream);
            setWindowTitleFromFile(file);
        }
        catch (const std::exception& e)
        {
            QMessageBox::critical(
                mpSelf, "Error writing file",
                QString("An error occurred when writing to file\n\"%1\":\n%2").arg(file).arg(e.what()));
        }
    }
}

void CSPDataWindow::Private::setAnalysisData(const AnalysisData& data)
{
    mAnalysisData = data;

    int progressCount = 2 * (data.trainingData1.size() + data.trainingData2.size()) + 4;
    QProgressDialog dialog("Computing CSP Components...", "", 0, progressCount, mpAnalysisWindow);
    mpProgress = &dialog;
    dialog.setCancelButton(nullptr);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setWindowFlags(dialog.windowFlags() & ~Qt::WindowCloseButtonHint & ~Qt::WindowContextHelpButtonHint);
    dialog.setAutoClose(false);
    dialog.setValue(0);
    dialog.setMinimumDuration(0);
    dialog.show();

    computeCSP(mAnalysisData);

    dialog.close();
    mpProgress = nullptr;

    setSpatialFilterParameters(mPatterns);

    mpComponentLineEdit->setValidator(new QIntValidator(1, mLogValues.Elements() - 1));
    mpComponentSlider->setMinimum(0);
    mpComponentSlider->setMaximum(mLogValues.Elements() - 1);
    mpSignalWidget->setSignal(mLogValues);
    mpSignalWidget->display().SetDisplayMode(SignalDisplay::polyline);
    mpSignalWidget->display().SetChannelLabelsVisible(true);
    mpSignalWidget->display().SetBaselinesVisible(true);
    mpSignalWidget->display().SetMinValue(-mLogMax).SetMaxValue(mLogMax);
    onSliderChanged(0);
}

void CSPDataWindow::Private::computeCSP(const AnalysisData& data)
{
    auto cov1 = computeCovariance(data.trainingData1),
         cov2 = computeCovariance(data.trainingData2);
    Eigen::GeneralizedEigenSolver<MatrixXf> solver;
    solver.compute(cov1, cov2, true);
    progress();
    MatrixXf patterns;
    patterns = solver.eigenvectors().real();
    progress();
    MatrixXf fields;
    fields = patterns.inverse();
    VectorXf eigenvalues = solver.eigenvalues().real();
    progress();

    std::vector<int> index;
    index.reserve(eigenvalues.count());
    for (int i = 0; i < eigenvalues.count(); ++i)
    {
        index.push_back(i);
        eigenvalues[i] = ::log(eigenvalues[i]);
    }
    std::sort(index.begin(), index.end(),
        [eigenvalues](int a, int b) { return ::fabs(eigenvalues[a]) > ::fabs(eigenvalues[b]); }
    );
    mLogMax = ::fabs(eigenvalues[index[0]]);
    mNumComponents = eigenvalues.count();
    mRetain.resize(index.size(), false);
    int retainCount = mRetain.size() / 16; // arbitrary value
    retainCount = std::max(retainCount, 2);
    retainCount = std::min<int>(retainCount, mRetain.size());
    for (int i = 0; i < retainCount; ++i)
        mRetain[i] = true;

    SignalProperties prop(1, eigenvalues.count());
    prop.ChannelLabels()[0] = "log ratio";
    mLogValues.SetProperties(prop);
    for (int el = 0; el < index.size(); ++el)
    {
        float value = eigenvalues[index[el]];
        mLogValues(0, el) = value;
    }

    prop = data.trainingData1.front().Properties();
    prop.ValueUnit().Clear();
    prop.SetElements(prop.Channels());
    mPatterns.SetProperties(prop);
    mFields.SetProperties(prop);
    for (int ch = 0; ch < mPatterns.Channels(); ++ch)
    {
        for (int el = 0; el < mPatterns.Elements(); ++el)
        {
            mPatterns(ch, el) = patterns(ch, index[el]);
            mFields(ch, el) = fields(index[el], ch);
        }
    }
    progress();
}

MatrixXf CSPDataWindow::Private::computeCovariance(const std::vector<GenericSignal>& data)
{
    if (data.empty())
        throw std_runtime_error << "Cannot compute covariance from empty data set";

    int p0 = 0;
    VectorXf p1(data.front().Channels());
    p1.fill(0);
    MatrixXf p2(data.front().Channels(), data.front().Channels());
    p2.fill(0);

    for (const auto& signal : data)
    {
        progress();
        for (int sample = 0; sample < signal.Elements(); ++sample)
        {
            ++p0;
            VectorXf v(signal.Channels());
            for (int channel = 0; channel < signal.Channels(); ++channel)
                v[channel] = signal(channel, sample);
            p1 += v;
        }
    }

    if (p0 < 2)
        throw std_runtime_error << "Cannot compute covariance from " << p0 << " samples";

    p1 /= p0;

    for (const auto& signal : data)
    {
        progress();
        for (int sample = 0; sample < signal.Elements(); ++sample)
        {
            VectorXf v(signal.Channels());
            for (int channel = 0; channel < signal.Channels(); ++channel)
                v[channel] = signal(channel, sample);
            v -= p1;
            p2 += v * v.adjoint();
        }
    }

    p2 /= (p0 - 1);

    return p2;
}

void CSPDataWindow::Private::onNextComponent()
{
    auto pSlider = mpComponentSlider;
    pSlider->setValue(pSlider->value() + pSlider->singleStep());
}

void CSPDataWindow::Private::onPreviousComponent()
{
    auto pSlider = mpComponentSlider;
    pSlider->setValue(pSlider->value() - pSlider->singleStep());
}

void CSPDataWindow::Private::onSliderChanged(int i)
{
    QString s;
    if (mMode == pattern)
    {
        mpTopoplotWidget->setData(mPatterns, i);
        s = "Topoplot of pattern %1 (log ratio %2 = %3 bit)";
    }
    else if (mMode == field)
    {
        mpTopoplotWidget->setData(mFields, i);
        s = "Topoplot of field %1 (log ratio %2 = %3 bit)";
    }
    mComponentIndex = i;
    mpRetainCheckBox->setChecked(mRetain[i]);
    mpTopoplotGroupBox->setTitle(s.arg(i + 1).arg(mLogValues(0, i)).arg(mLogValues(0, i) / ::log(2.0)));
    mpComponentLineEdit->setText(QString("%1").arg(i + 1));
}

void CSPDataWindow::Private::onRetainCheckBoxToggled(bool value)
{
    mRetain[mComponentIndex] = value;
}

void CSPDataWindow::Private::onPatternClicked()
{
    mMode = pattern;
    onSliderChanged(mComponentIndex);
}

void CSPDataWindow::Private::onFieldClicked()
{
    mMode = field;
    onSliderChanged(mComponentIndex);
}

void CSPDataWindow::Private::onComponentEdited(const QString& text)
{
    int i = text.toInt() - 1;
    mpComponentSlider->setValue(i);
}

CSPDataWindow::CSPDataWindow(AnalysisWindow* pAnalysisWindow, const AnalysisData& data, const QString& file)
: p(new Private(this))
{
    setAttribute(Qt::WA_DeleteOnClose, true);

    p->setWindowTitleFromFile(file);
    p->mpAnalysisWindow = pAnalysisWindow;

    QMenu* pMenu = QMainWindow::menuBar()->addMenu("&File");
    QAction* pAction = pMenu->addAction("&Open Analysis Data...");
    connect(pAction, &QAction::triggered, p, &Private::onOpenAnalysisData);
    pAction->setShortcut(Qt::CTRL | Qt::Key_O);
    pAction = pMenu->addAction("&Save Analysis Data...");
    connect(pAction, &QAction::triggered, p, &Private::onSaveAnalysisData);
    pAction->setShortcut(Qt::CTRL | Qt::Key_S);
    pMenu->addSeparator();
    pAction = pMenu->addAction("Apply Spatial Filter &Parameters...");
    connect(pAction, &QAction::triggered, p, &Private::onApplySpatialFilterParameters);
    pAction = pMenu->addAction("Save Spatial &Filter Parameters...");
    connect(pAction, &QAction::triggered, p, &Private::onSaveSpatialFilterParameters);
    pAction = pMenu->addAction("Save All Filter &Parameters...");
    connect(pAction, &QAction::triggered, p, &Private::onSaveAllFilterParameters);
    pMenu->addSeparator();
    pAction = pMenu->addAction("&Close Window");
    pAction->setShortcut(Qt::CTRL | Qt::Key_W);
    connect(pAction, &QAction::triggered, this, &QMainWindow::close);

    EditMenu::addToMenuBar(menuBar());

    pMenu = QMainWindow::menuBar()->addMenu("&View");
    pAction = pMenu->addAction("&Next component");
    pAction->setShortcut(Qt::SHIFT | Qt::Key_Right);
    connect(pAction, &QAction::triggered, p, &Private::onNextComponent);
    pAction = pMenu->addAction("&Previous component");
    pAction->setShortcut(Qt::SHIFT | Qt::Key_Left);
    connect(pAction, &QAction::triggered, p, &Private::onPreviousComponent);

    WindowMenu::addWindow(this);
    HelpMenu::addToMenuBar(menuBar());

    auto pWindow = new EditMenu::CopyWidget(this);

    auto pLayout = new QVBoxLayout;
    p->mpTopoplotGroupBox = new QGroupBox("Topography");
    auto pVBoxLayout = new QVBoxLayout;
    p->mpTopoplotGroupBox->setLayout(pVBoxLayout);
    p->mpTopoplotWidget = new TopoplotWidget;
    p->mpTopoplotGroupBox->layout()->addWidget(p->mpTopoplotWidget);
    auto pSlider = new QSlider;
    pSlider->setOrientation(Qt::Horizontal);
    p->mpTopoplotGroupBox->layout()->addWidget(pSlider);
    connect(pSlider, &QSlider::valueChanged, p, &Private::onSliderChanged);
    p->mpComponentSlider = pSlider;

    auto pHBoxLayout = new QHBoxLayout;
    auto pFormLayout = new QFormLayout;
    pHBoxLayout->addLayout(pFormLayout);
    auto pLineEdit = new QLineEdit;
    pLineEdit->setFixedWidth(fontMetrics().averageCharWidth() * 4);
    pLineEdit->setValidator(new QIntValidator(1, p->mNumComponents));
    pFormLayout->addRow("Component index:", pLineEdit);
    p->mpComponentLineEdit = pLineEdit;
    connect(p->mpComponentLineEdit, &QLineEdit::textEdited, p, &Private::onComponentEdited);

    auto pCheckBox = new QCheckBox("Retain");
    pHBoxLayout->addWidget(pCheckBox);
    p->mpRetainCheckBox = pCheckBox;
    connect(pCheckBox, &QCheckBox::toggled, p, &Private::onRetainCheckBoxToggled);

    auto pRadioButton1 = new QRadioButton("Display Pattern");
    auto pRadioButton2 = new QRadioButton("Display Field");
    auto pButtonGroup = new QButtonGroup;
    pRadioButton1->setChecked(true);
    connect(pRadioButton1, &QRadioButton::clicked, p, &Private::onPatternClicked);
    connect(pRadioButton2, &QRadioButton::clicked, p, &Private::onFieldClicked);

    pButtonGroup->addButton(pRadioButton1);
    pButtonGroup->addButton(pRadioButton2);
    pHBoxLayout->addWidget(pRadioButton1);
    pHBoxLayout->addWidget(pRadioButton2);
    pVBoxLayout->addLayout(pHBoxLayout);
    pLayout->addWidget(p->mpTopoplotGroupBox);

    auto pGroupBox = new QGroupBox("Components (log ratio)");
    pGroupBox->setLayout(new QHBoxLayout);
    auto pWidget = new SignalWidget;
    pGroupBox->layout()->addWidget(pWidget);
    p->mpSignalWidget = pWidget;
    pLayout->addWidget(pGroupBox);
    pWindow->setLayout(pLayout);
    setCentralWidget(pWindow);

    p->setAnalysisData(data);

    WindowSettings::read(this);
}

CSPDataWindow::~CSPDataWindow()
{
    WindowSettings::write(this);
    WindowMenu::removeWindow(this);
}
