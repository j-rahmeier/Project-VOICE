////////////////////////////////////////////////////////////////////////////////
// $Id: ApplicationDataWindow.cpp 8268 2024-07-23 16:38:37Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Data window for generic application data.
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
#include "ApplicationDataWindow.h"

#include "AnalysisWindow.h"
#include "AnalysisData.h"
#include "HistogramWidget.h"
#include "FileDialog.h"
#include "EditMenu.h"
#include "WindowMenu.h"
#include "HelpMenu.h"
#include "WindowSettings.h"
#include "Statistics.h"
#include "WindowFactory.h"

#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QSlider>
#include <QLineEdit>
#include <QTextBrowser>
#include <QValidator>
#include <QCheckBox>

#include <fstream>

static const int sNumBins = 60;

namespace {
    int DetermineDimensions(const AnalysisData& data)
    {
        int result = 0;
        for (const auto& block : data.trainingData1)
            result = std::max(result, block.Channels());
        for (const auto& block : data.trainingData2)
            result = std::max(result, block.Channels());
        for (const auto& block : data.testData1)
            result = std::max(result, block.Channels());
        for (const auto& block : data.testData2)
            result = std::max(result, block.Channels());
        return result;
    }
}

struct ApplicationDataWindow::Private : QObject
{
    Private(QMainWindow* pParent) : QObject(pParent), mpSelf(pParent) {}

    void setWindowTitleFromFile(const QString&);

    void onOpenAnalysisData();
    void onSaveAnalysisData();
    void onSaveAllFilterParameters();

    void setAnalysisData(const AnalysisData&);
    double computeRsq(const AnalysisData&, int flags, int& n1, int& n2) const;

    void onNextDimension();
    void onPreviousDimension();
    void onSliderChanged(int);
    void onDimensionEdited(const QString&);
    void onTrainingDataToggled(bool);
    void onTestDataToggled(bool);
    void onIntegratedToggled(bool);

    AnalysisData mAnalysisData;
    int mNumDimensions = 1, mDimensionIndex = 0;
    int mFlags = 0;

    HistogramWidget* mpHistogramWidget = nullptr;
    AnalysisWindow* mpAnalysisWindow = nullptr;
    QGroupBox* mpGroupBox = nullptr;
    QSlider* mpSlider = nullptr;
    QCheckBox* mpIntegrated = nullptr, *mpTraining = nullptr, *mpTesting = nullptr;
    QLineEdit* mpDimensionLineEdit = nullptr, *mpNumBinsLineEdit = nullptr;
    QTextBrowser* mpTextBrowser = nullptr;
    QMainWindow* mpSelf;
};

void ApplicationDataWindow::Private::setWindowTitleFromFile(const QString& file)
{
    mpSelf->setWindowTitle(WindowMenu::windowTitleFromFile("Application Data", file));
}

void ApplicationDataWindow::Private::onOpenAnalysisData()
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
        if (data.kind.find("Application"))
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

void ApplicationDataWindow::Private::onSaveAnalysisData()
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

void ApplicationDataWindow::Private::onSaveAllFilterParameters()
{
    QString file = FileDialog::getSaveFileName(mpSelf, "Save Filter Parameters",
        "BCI2000 Filter Parameters (*.prm)", "output");
    if (!file.isEmpty())
    {
        if (!mAnalysisData.filterParams.Save(file.toLocal8Bit().constData()))
            QMessageBox::critical(mpSelf, "Error writing file",
                QString("Error opening file\n%1\nfor writing").arg(file));
    }
}

void ApplicationDataWindow::Private::setAnalysisData(const AnalysisData& data)
{
    mAnalysisData = data;

    mpTextBrowser->setText("");
    if (data.report.empty()) {
        mpTextBrowser->hide();
    }
    else {
        mpTextBrowser->setText(QString::fromUtf8(data.report.c_str()));
        mpTextBrowser->show();
    }

    mNumDimensions = DetermineDimensions(data);
    mDimensionIndex = 0;

    if (!data.canIntegrateTrials())
        mpIntegrated->setChecked(false);
    mpIntegrated->setEnabled(data.canIntegrateTrials());
    if (!data.haveTrainingData())
        mpTraining->setChecked(false);
    mpTraining->setEnabled(data.haveTrainingData());
    if (!data.haveTestData())
        mpTesting->setChecked(false);
    mpTesting->setEnabled(data.haveTestData());

    mFlags = 0;
    if (mpIntegrated->isChecked())
        mFlags |= HistogramWidget::integrated;
    if (mpTraining->isChecked())
        mFlags |= HistogramWidget::includeTrainingData;
    if (mpTesting->isChecked())
        mFlags |= HistogramWidget::includeTestData;

    mpDimensionLineEdit->setValidator(new QIntValidator(1, mNumDimensions));
    mpSlider->setMinimum(0);
    mpSlider->setMaximum(mNumDimensions - 1);
    mpSlider->setValue(mDimensionIndex);
    onSliderChanged(mDimensionIndex);
}

double ApplicationDataWindow::Private::computeRsq(const AnalysisData& analysisData, int flags, int& n1, int& n2) const
{
    if (flags & HistogramWidget::integrated)
    {
        AnalysisData integratedData = analysisData;
        integratedData.integrateTrials();
        flags &= ~HistogramWidget::integrated;
        return computeRsq(integratedData, flags, n1, n2);
    }
    std::vector<GenericSignal> data1, data2;
    if (flags & HistogramWidget::includeTrainingData)
    {
        for (const auto& point : analysisData.trainingData1)
            data1.push_back(point);
        for (const auto& point : analysisData.trainingData2)
            data2.push_back(point);
    }
    if (flags & HistogramWidget::includeTestData)
    {
        for (const auto& point : analysisData.testData1)
            data1.push_back(point);
        for (const auto& point : analysisData.testData2)
            data2.push_back(point);
    }
    n1 = data1.size();
    n2 = data2.size();
    GenericSignal rsq = Statistics::rsq(data1, data2);
    if (rsq.Channels() <= mDimensionIndex)
        return NaN<double>();
    if (rsq.Elements() > 1)
        return NaN<double>();
    return rsq(mDimensionIndex, 0);
}

void ApplicationDataWindow::Private::onNextDimension()
{
    mpSlider->setValue(mpSlider->value() + mpSlider->singleStep());
}

void ApplicationDataWindow::Private::onPreviousDimension()
{
    mpSlider->setValue(mpSlider->value() - mpSlider->singleStep());
}

void ApplicationDataWindow::Private::onSliderChanged(int i)
{
    mDimensionIndex = i;
    int n1 = 0, n2 = 0;
    double rsq = computeRsq(mAnalysisData, mFlags, n1, n2);
    double snr = ::log(rsq / (1-rsq)) / ::log(2);
    mpGroupBox->setTitle(QString(
        "Histogram for dimension %1 (N\xE2\x82\x81=%4, N\xE2\x82\x82=%5, r\xc2\xb2=%2, SNR=%3 bit)")
                        .arg(i + 1).arg(rsq, 0, 'f', 3).arg(snr, 0, 'f', 3).arg(n1).arg(n2));
    mpDimensionLineEdit->setText(QString("%1").arg(i + 1));
    mpHistogramWidget->setData(mAnalysisData, mDimensionIndex, mFlags, sNumBins);
}

void ApplicationDataWindow::Private::onDimensionEdited(const QString& text)
{
    int i = text.toInt() - 1;
    mpSlider->setValue(i);
    mpDimensionLineEdit->setText(QString::number(mpSlider->value() + 1));
}

void ApplicationDataWindow::Private::onTrainingDataToggled(bool b)
{
    mFlags &= ~HistogramWidget::includeTrainingData;
    if (b)
        mFlags |= HistogramWidget::includeTrainingData;
    onSliderChanged(mDimensionIndex);
}

void ApplicationDataWindow::Private::onTestDataToggled(bool b)
{
    mFlags &= ~HistogramWidget::includeTestData;
    if (b)
        mFlags |= HistogramWidget::includeTestData;
    onSliderChanged(mDimensionIndex);
}

void ApplicationDataWindow::Private::onIntegratedToggled(bool b)
{
    mFlags &= ~HistogramWidget::integrated;
    if (b)
        mFlags |= HistogramWidget::integrated;
    onSliderChanged(mDimensionIndex);
}

ApplicationDataWindow::ApplicationDataWindow(AnalysisWindow* pAnalysisWindow, const AnalysisData& data, const QString& file)
: p(new Private(this))
{
    setAttribute(Qt::WA_DeleteOnClose, true);

    p->setWindowTitleFromFile(file);
    p->mpAnalysisWindow = pAnalysisWindow;

    //  QMainWindow::move(10, 10);
    QMenu* pMenu = QMainWindow::menuBar()->addMenu("&File");
    QAction* pAction = pMenu->addAction("&Open Analysis Data...");
    connect(pAction, &QAction::triggered, p, &Private::onOpenAnalysisData);
    pAction->setShortcut(Qt::CTRL | Qt::Key_O);
    pAction = pMenu->addAction("&Save Analysis Data...");
    connect(pAction, &QAction::triggered, p, &Private::onSaveAnalysisData);
    pAction->setShortcut(Qt::CTRL | Qt::Key_S);
    pMenu->addSeparator();
    pAction = pMenu->addAction("Save all Filter &Parameters...");
    connect(pAction, &QAction::triggered, p, &Private::onSaveAllFilterParameters);
    pMenu->addSeparator();
    pAction = pMenu->addAction("&Close Window");
    pAction->setShortcut(Qt::CTRL | Qt::Key_W);
    connect(pAction, &QAction::triggered, this, &QMainWindow::close);

    EditMenu::addToMenuBar(menuBar());

    pMenu = QMainWindow::menuBar()->addMenu("&View");
    pAction = pMenu->addAction("&Next dimension");
    pAction->setShortcut(Qt::SHIFT | Qt::Key_Right);
    connect(pAction, &QAction::triggered, p, &Private::onNextDimension);
    pAction = pMenu->addAction("&Previous dimension");
    pAction->setShortcut(Qt::SHIFT | Qt::Key_Left);
    connect(pAction, &QAction::triggered, p, &Private::onPreviousDimension);

    WindowMenu::addWindow(this);
    HelpMenu::addToMenuBar(menuBar());

    auto pWindow = new EditMenu::CopyWidget(this);

    auto pLayout = new QVBoxLayout;
    auto pGroupBox = new QGroupBox("Histogram");
    auto pVBoxLayout = new QVBoxLayout;
    pGroupBox->setLayout(pVBoxLayout);
    p->mpGroupBox = pGroupBox;
    p->mpHistogramWidget = new HistogramWidget;
    p->mpHistogramWidget->setColors(RGBColor::White, RGBColor::Red);
    pGroupBox->layout()->addWidget(p->mpHistogramWidget);
    auto pSlider = new QSlider;
    pSlider->setOrientation(Qt::Horizontal);
    pGroupBox->layout()->addWidget(pSlider);
    connect(pSlider, &QSlider::valueChanged, p, &Private::onSliderChanged);
    p->mpSlider = pSlider;

    auto pHBoxLayout = new QHBoxLayout;
    auto pFormLayout = new QFormLayout;
    pHBoxLayout->addLayout(pFormLayout);
    auto pLineEdit = new QLineEdit;
    pLineEdit->setFixedWidth(fontMetrics().averageCharWidth() * 4);
    pLineEdit->setValidator(new QIntValidator(1, p->mNumDimensions));
    pFormLayout->addRow("Dimension:", pLineEdit);
    p->mpDimensionLineEdit = pLineEdit;
    connect(p->mpDimensionLineEdit, &QLineEdit::textEdited, p, &Private::onDimensionEdited);

    auto pCheckBox = new QCheckBox("Training");
    connect(pCheckBox, &QCheckBox::toggled, p, &Private::onTrainingDataToggled);
    pCheckBox->setChecked(true);
    pHBoxLayout->addWidget(pCheckBox);
    p->mpTraining = pCheckBox;

    pCheckBox = new QCheckBox("Testing");
    connect(pCheckBox, &QCheckBox::toggled, p, &Private::onTestDataToggled);
    pCheckBox->setChecked(false);
    pHBoxLayout->addWidget(pCheckBox);
    p->mpTesting = pCheckBox;

    pCheckBox = new QCheckBox("Integrate Trials");
    connect(pCheckBox, &QCheckBox::toggled, p, &Private::onIntegratedToggled);
    pCheckBox->setChecked(false);
    pHBoxLayout->addWidget(pCheckBox);
    p->mpIntegrated = pCheckBox;

    pVBoxLayout->addLayout(pHBoxLayout);
    p->mpTextBrowser = new QTextBrowser;
    pVBoxLayout->addWidget(p->mpTextBrowser);
    pLayout->addWidget(pGroupBox);
    pWindow->setLayout(pLayout);

    setCentralWidget(pWindow);

    p->setAnalysisData(data);

    WindowSettings::read(this);
}

ApplicationDataWindow::~ApplicationDataWindow()
{
    WindowSettings::write(this);
    WindowMenu::removeWindow(this);
}
