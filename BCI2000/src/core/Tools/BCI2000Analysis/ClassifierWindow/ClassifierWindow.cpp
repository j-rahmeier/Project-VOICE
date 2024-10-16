////////////////////////////////////////////////////////////////////////////////
// $Id: ClassifierWindow.cpp 8268 2024-07-23 16:38:37Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Classifier options window for BCI2000Analysis.
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
#include "ClassifierWindow.h"

#include "AnalysisData.h"
#include "AnalysisWindow.h"
#include "Classifier.h"
#include "ClassifierPropsWidget.h"
#include "ClassifierChannelPlotWindow.h"
#include "TopoplotWindow.h"
#include "EditMenu.h"
#include "FileDialog.h"
#include "HelpMenu.h"
#include "SignalWidget.h"
#include "WindowMenu.h"
#include "WindowSettings.h"

#include <QApplication>
#include <QBoxLayout>
#include <QDockWidget>
#include <QGroupBox>
#include <QMenuBar>

struct ClassifierWindow::Private : QObject
{
    QMainWindow *mpSelf;
    AnalysisWindow *mpAnalysisWindow = nullptr;
    AnalysisData mAnalysisData;
    GenericSignal mWeights;
    double mOffset = 0, mVariance = 0;
    std::string mClassifierKind;

    Private(QMainWindow *);

    void setWindowTitleFromFile(const QString&);

    void setClassifierParameters(const Classifier &);

    void onApplyClassifierParameters();
    void onSaveClassifierParameters();
    void onSaveAllFilterParameters();

    void onShowCurvesAtChannel();
    void onShowDistributionAtElement();
    void onShowDistributionAt(int element);
    void onShowCurvesAt(int channel);
};

ClassifierWindow::Private::Private(QMainWindow *p) : QObject(p), mpSelf(p)
{
}

void ClassifierWindow::Private::setWindowTitleFromFile(const QString& file)
{
    QString title = QString("%1 Classifier").arg(mClassifierKind.c_str());
    mpSelf->setWindowTitle(WindowMenu::windowTitleFromFile(title, file));
}

void ClassifierWindow::Private::setClassifierParameters(const Classifier &trainedClassifier)
{
    ParamList &params = mAnalysisData.filterParams;

    std::string classifierName = mAnalysisData.absoluteParameterPath("Classifier");
    if (!params.Exists(classifierName))
        params.Add("Filtering matrix " + classifierName + "= 0 0");
    MutableParamRef Classifier(&params.ByPath(classifierName));

    std::string normalizerOffsetsName = mAnalysisData.absoluteParameterPath("../Normalizer/NormalizerOffsets");
    if (!params.Exists(normalizerOffsetsName))
        params.Add("Filtering list " + normalizerOffsetsName + "= 0");
    MutableParamRef NormalizerOffsets(&params.ByPath(normalizerOffsetsName));

    std::string normalizerGainsName = mAnalysisData.absoluteParameterPath("../Normalizer/NormalizerGains");
    if (!params.Exists(normalizerGainsName))
        params.Add("Filtering list " + normalizerGainsName + "= 0");
    MutableParamRef NormalizerGains(&params.ByPath(normalizerGainsName));

    std::string bufferConditionsName = mAnalysisData.absoluteParameterPath("../Normalizer/bufferConditions");
    if (!params.Exists(bufferConditionsName))
        params.Add("Filtering list " + bufferConditionsName + "= 0 0");
    MutableParamRef BufferConditions(&params.ByPath(bufferConditionsName));

    int dim = trainedClassifier.outputDimension();
    if (NormalizerOffsets->NumValues() < dim)
        NormalizerOffsets->SetNumValues(dim);
    if (NormalizerGains->NumValues() < dim)
        NormalizerGains->SetNumValues(dim);

    // Read existing classifier matrix, make sure to leave dimensions other than
    // selected classifier dimension intact.
    struct Entry { int inputChannel, inputElement; double weight; };
    std::map<int, std::vector<Entry>> classifierMatrix;
    for (int row = 0; row < Classifier->NumRows(); ++row)
    {
        int outputDimension = Classifier(row, 2);
        if (outputDimension != dim)
        {
            Entry entry = { Classifier(row, 0), Classifier(row, 1), Classifier(row, 3) };
            classifierMatrix[outputDimension].push_back(entry);
        }
    }

    const GenericSignal& weights = trainedClassifier.weightsAsSignal();
    for (int ch = 0; ch < weights.Channels(); ++ch)
    {
        for (int el = 0; el < weights.Elements(); ++el)
        {
            if (weights(ch, el) != 0)
            {
                Entry entry = { ch + 1, el + 1, weights(ch, el) };
                classifierMatrix[dim].push_back(entry);
            }
        }
    }

    int rows = 0;
    for (const auto& dimentry : classifierMatrix)
        rows += dimentry.second.size();

    Classifier->SetDimensions(0, 0);
    Classifier->SetNumColumns(4);
    Classifier->ColumnLabels()[0] = "input channel";
    Classifier->ColumnLabels()[1] = "input element";
    Classifier->ColumnLabels()[2] = "output channel";
    Classifier->ColumnLabels()[3] = "weight";
    Classifier->SetNumRows(rows);
    int row = 0;
    for (const auto& dimentry : classifierMatrix)
    {
        for (const auto& entry : dimentry.second)
        {
            Classifier(row, 0) = entry.inputChannel;
            Classifier(row, 1) = entry.inputElement;
            Classifier(row, 2) = dimentry.first;
            Classifier(row, 3) = entry.weight;
            ++row;
        }
    }
    NormalizerOffsets(dim - 1) = trainedClassifier.offset();
    double totalVariance = trainedClassifier.meanDiff() * trainedClassifier.meanDiff();
    totalVariance += trainedClassifier.variance();
    NormalizerGains(dim - 1) = 1.0 / ::sqrt(totalVariance);
    if (BufferConditions->NumColumns() < dim)
        BufferConditions->SetNumColumns(dim);
    if (BufferConditions->NumRows() < 2)
        BufferConditions->SetNumRows(2);
    BufferConditions(0, dim - 1) = mAnalysisData.analysisParams.condition1;
    BufferConditions(1, dim - 1) = mAnalysisData.analysisParams.condition2;
}

void ClassifierWindow::Private::onApplyClassifierParameters()
{
    auto pWindow = WindowMenu::validateWindow(mpAnalysisWindow);
    if (pWindow)
        pWindow->setFilterParameters(mAnalysisData.filterParams, mpSelf);
}

void ClassifierWindow::Private::onSaveClassifierParameters()
{
    std::string classifierName = mAnalysisData.absoluteParameterPath("Classifier"),
                normalizerOffsetsName = mAnalysisData.absoluteParameterPath("../Normalizer/NormalizerOffsets"),
                normalizerGainsName = mAnalysisData.absoluteParameterPath("../Normalizer/NormalizerGains");
    Assert(mAnalysisData.filterParams.Exists(classifierName));
    Assert(mAnalysisData.filterParams.Exists(normalizerOffsetsName));
    Assert(mAnalysisData.filterParams.Exists(normalizerGainsName));
    ParamList classifierParams;
    classifierParams.Add(mAnalysisData.filterParams.ByPath(classifierName));
    classifierParams.Add(mAnalysisData.filterParams.ByPath(normalizerOffsetsName));
    classifierParams.Add(mAnalysisData.filterParams.ByPath(normalizerGainsName));
    auto filename =
        FileDialog::getSaveFileName(mpSelf, "Save Classifier Parameters", "BCI2000 Parameter File (*.prm)");
    if (!filename.isEmpty())
    {
        classifierParams.Save(filename.toLocal8Bit().constData());
    }
}

void ClassifierWindow::Private::onSaveAllFilterParameters()
{
    auto filename =
        FileDialog::getSaveFileName(mpSelf, "Save All Filter Parameters", "BCI2000 Parameter File (*.prm)");
    if (!filename.isEmpty())
    {
        mAnalysisData.filterParams.Save(filename.toLocal8Bit().constData());
    }
}

void ClassifierWindow::Private::onShowCurvesAtChannel()
{
    int ch = ClassifierChannelPlotWindow::askForChannel(&mWeights.Properties());
    if (ch >= 0)
        onShowCurvesAt(ch);
}

void ClassifierWindow::Private::onShowDistributionAtElement()
{
    int el = TopoplotWindow::askForElement(&mWeights.Properties());
    if (el >= 0)
        onShowDistributionAt(el);
}

void ClassifierWindow::Private::onShowDistributionAt(int element)
{
    auto pWindow = new TopoplotWindow(mWeights, element, mpSelf);
    pWindow->resize(400, 400);
    pWindow->show();
}

void ClassifierWindow::Private::onShowCurvesAt(int channel)
{
    auto pWindow = new ClassifierChannelPlotWindow(mWeights, channel, mpSelf);
    pWindow->resize(400, 100);
    pWindow->show();
}


ClassifierWindow::ClassifierWindow(
    AnalysisWindow* pAnalysisWindow, const AnalysisData &data, 
    const Classifier &classifier, const QString& file)
    : QMainWindow(nullptr), p(new Private(this))
{
    setAttribute(Qt::WA_DeleteOnClose, true);

    p->mpAnalysisWindow = pAnalysisWindow;
    p->mAnalysisData = data;
    p->mClassifierKind = classifier.method();
    p->mWeights = classifier.weightsAsSignal();
    p->mOffset = classifier.offset();
    p->mVariance = classifier.variance();
    p->setClassifierParameters(classifier);

    p->setWindowTitleFromFile(file);

    //  QMainWindow::move(10, 10);
    QMenu *pMenu = QMainWindow::menuBar()->addMenu("&File");
    QAction *pAction = pMenu->addAction("&Apply Classifier Parameters...");
    connect(pAction, &QAction::triggered, p, &Private::onApplyClassifierParameters);
    pAction->setShortcut(Qt::CTRL | Qt::Key_Y);
    pAction = pMenu->addAction("&Save Classifier Parameters...");
    connect(pAction, &QAction::triggered, p, &Private::onSaveClassifierParameters);
    pAction->setShortcut(Qt::CTRL | Qt::Key_S);
    pAction = pMenu->addAction("Save All Filter &Parameters...");
    connect(pAction, &QAction::triggered, p, &Private::onSaveAllFilterParameters);
    pMenu->addSeparator();
    pAction = pMenu->addAction("&Close Window");
    pAction->setShortcut(Qt::CTRL | Qt::Key_W);
    connect(pAction, &QAction::triggered, this, &QMainWindow::close);

    EditMenu::addToMenuBar(menuBar());

    pMenu = QMainWindow::menuBar()->addMenu("&View");
    pMenu->addAction("Show &Weights for Channel...", p, &Private::onShowCurvesAtChannel);
    pMenu->addAction("Show &Distribution at Time/Frequency...", p, &Private::onShowDistributionAtElement);

    WindowMenu::addWindow(this);
    HelpMenu::addToMenuBar(menuBar());

    QDockWidget *pDockWidget = new QDockWidget("Classifier Information", this);
    pDockWidget->setFeatures(pDockWidget->features() & ~QDockWidget::DockWidgetClosable);
    auto pProps = new ClassifierPropsWidget(data, classifier, this);
    pDockWidget->setWidget(pProps);
    addDockWidget(Qt::LeftDockWidgetArea, pDockWidget);

    GenericSignal s = classifier.weightsAsSignal();
    for (int ch = 0; ch < s.Channels(); ++ch)
        for (int el = 0; el < s.Elements(); ++el)
            s(ch, el) = ::fabs(s(ch, el));
    auto pWidget = new EditMenu::CopyWidget(this);
    auto pLayout = new QHBoxLayout;
    pLayout->setContentsMargins(0, 0, 0, 0);
    pWidget->setLayout(pLayout);
    QGroupBox *pGroupBox = new QGroupBox("Classifier weights (absolute values)");
    pWidget->layout()->addWidget(pGroupBox);
    pLayout = new QHBoxLayout;
    pLayout->setContentsMargins(0, 0, 0, 0);
    pGroupBox->setLayout(pLayout);
    auto pSignalWidget = new SignalWidget(s);
    pSignalWidget->setContextMenu(true);
    connect(pSignalWidget, &SignalWidget::showCurvesAt, p, &Private::onShowCurvesAt);
    connect(pSignalWidget, &SignalWidget::showDistributionAt, p, &Private::onShowDistributionAt);
    pGroupBox->layout()->addWidget(pSignalWidget);
    setCentralWidget(pWidget);

    setMinimumHeight(240);
    setMinimumWidth(320);
    WindowSettings::read(this);
}

ClassifierWindow::~ClassifierWindow()
{
    WindowSettings::write(this);
    WindowMenu::removeWindow(this);
}
