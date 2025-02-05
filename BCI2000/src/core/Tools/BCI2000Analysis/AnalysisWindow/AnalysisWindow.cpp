////////////////////////////////////////////////////////////////////////////////
// $Id: AnalysisWindow.cpp 8270 2024-07-23 19:34:53Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Main window class for AnalysisWindow.
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
#include "AnalysisWindow.h"

#include "AnalysisData.h"
#include "AnalysisParamWidget.h"
#include "AnalysisTargetWidget.h"
#include "AnalysisLogWidget.h"
#include "BCI2000FileReader.h"
#include "ConfigWindow.h"
#include "CppTranslator.h"
#include "CreateFilterGraph.h"
#include "ApplicationFilters.h"
#include "EditMenu.h"
#include "Environment.h"
#include "FileDialog.h"
#include "FilesWidget.h"
#include "FilterGraphNode.h"
#include "FiltersWidget.h"
#include "GenericVisualization.h"
#include "HelpMenu.h"
#include "ObserverWrapper.h"
#include "LogWindow.h"
#include "NullMessageChannel.h"
#include "Preferences.h"
#include "ProfilerWindow.h"
#include "WindowSettings.h"
#include "WindowMenu.h"
#include "WindowFactory.h"
#include "QtMain.h"

#include <QApplication>
#include <QDockWidget>
#include <QLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QProgressDialog>
#include <QToolBar>
#include <QActionGroup>
#include <QStatusBar>

static int sWindowCount = 0;

struct AnalysisWindow::Private : QObject
{
    NullMessageChannel mNullChannel;
    FilesWidget *mpTrainingFilesWidget = nullptr, *mpTestFilesWidget = nullptr;
    FiltersWidget *mpFiltersWidget = nullptr;
    AnalysisParamWidget *mpAnalysisParamWidget = nullptr;
    AnalysisTargetWidget *mpAnalysisTargetWidget = nullptr;
    AnalysisLogWidget *mpAnalysisLogWidget = nullptr;
    AnalysisWindow *mpSelf = nullptr;
    QString mFile;
    QAction *mpFilterGraphAutoItem = nullptr, *mpFilterGraphSMRItem = nullptr, 
            *mpFilterGraphSMR2Item = nullptr, *mpFilterGraphERPItem = nullptr, 
            *mpFilterGraphUserItem = nullptr, *mpAnalysisParamsAutoItem = nullptr,
            *mpAnalysisParamsUserItem = nullptr;
    QAction *mpRunAnalysis1 = nullptr, *mpRunAnalysis2 = nullptr;
    FilesWidget::FileClassification mCurrentClassification;
    struct
    {
        ParamList user, // user changes to default graph parameters
                  graph, // default graph parameters plus parameters from first file
                  verbosity; // verbosity parameters
        bool graphValid = false; // set to false when user parameters or graph changed
    } mParameters;

    Private(AnalysisWindow *);
    ~Private();

    void setWindowTitleFromFile(const QString&);

    void onFileOpen(const QString&);

    void onOpenAnalysisData();
    bool openAnalysisDataFile(const QString&);
    void onSaveAnalysisData();

    void onOpenFilterGraph();
    void openFilterGraph(const QString&);
    void onSaveFilterGraph();
    void onSavePipeDefinition();

    void onLoadFilterParameters();
    void loadFilterParameters(const QString&);
    void onSaveFilterParameters();
    void onClearFilterParameters();

    void onTrainingFilesWidgetDataChanged();
    void onTestFilesWidgetDataChanged();
    void updateRunAnalysisEnabled();


    void onFilterGraphAutoItemToggled(bool);
    void onFilterGraphSMRItemToggled(bool);
    void onFilterGraphSMR2ItemToggled(bool);
    void onFilterGraphERPItemToggled(bool);
    void onFilterGraphUserItemToggled(bool);
    void onFilterGraphComplexExampleItemTriggered();
    void onFilterGraphEdited();
    void onFilterGraphReplaced();

    void onAnalysisParamsAutoItemToggled(bool);
    void onAnalysisParamsUserItemToggled(bool);
    void onAnalysisParamsEdited();

    void onEditFilterParametersTriggered();
    void onResetFilterParametersTriggered();
    void updateGraphParameters();
    GenericFilter *instantiateGraph(ParamList &, StateList &);

    void analyze();
};

AnalysisWindow::Private::Private(AnalysisWindow *p)
    : QObject(p), mpSelf(p), mCurrentClassification(FilesWidget::Undefined)
{
    GenericVisualization::SetOutputChannel(&mNullChannel);
    Preferences::Instance()->mUserLevel = Preferences::None; // suppress parameter level slider
    connect(QtApplication::instance(), &QtApplication::FileOpen, this, &Private::onFileOpen);
}

AnalysisWindow::Private::~Private()
{
}

void AnalysisWindow::Private::setWindowTitleFromFile(const QString& file)
{
    QString title = QApplication::applicationName();
    mpSelf->setWindowTitle(WindowMenu::windowTitleFromFile(title, file));
    WindowMenu::updateWindow(mpSelf);
    mFile = file;
}

void AnalysisWindow::Private::onFileOpen(const QString& file)
{
    QString extension = QFileInfo(file).suffix();
    if (extension == "bcianalysis")
    {
        if (openAnalysisDataFile(file))
        {
            mpAnalysisLogWidget->append(
                QString("Read analysis data from file \"%1\"").arg(file)
            );
        }
    }
    else if (extension == "bcigraph")
    {
        openFilterGraph(file);
    }
    else if (extension == "bcidat" || extension == "dat")
    {
        QStringList files;
        files.push_back(file);
        mpSelf->openDataFiles(files);
    }
    else if (extension == "bciprm" || extension == "prm")
    {
        loadFilterParameters(file);
    }
}

void AnalysisWindow::Private::onOpenAnalysisData()
{
    QString file = FileDialog::getOpenFileName(mpSelf, "Open Analysis Data",
                                               "BCI2000 Analysis Data File (*.bcianalysis)", "output");
    if (openAnalysisDataFile(file))
    {
        mpAnalysisLogWidget->append(
            QString("Read analysis data from file \"%1\"").arg(file)
        );
    }
}

bool AnalysisWindow::Private::openAnalysisDataFile(const QString& file)
{
    if (!file.isEmpty())
    {
        std::ifstream stream(file.toLocal8Bit().constData(), std::ios::binary);
        AnalysisData data;
        try
        {
            data = AnalysisData::unserialize(stream);
        }
        catch (std::exception &e)
        {
            QMessageBox::critical(
                mpSelf, "Error reading file",
                QString("An error occurred when trying to read file\n\"%1\":\n%2").arg(file).arg(e.what()));
            return false;
        }
        mpTrainingFilesWidget->setFiles(data.trainingFiles);
        mpTestFilesWidget->setFiles(data.testFiles);
        mpFilterGraphUserItem->setChecked(true);
        mpFiltersWidget->setGraph(data.filterGraph);
        mpAnalysisParamsUserItem->setChecked(true);
        mpAnalysisParamWidget->setParams(data.analysisParams);
        mParameters.user = data.filterParams;
        mParameters.graphValid = false;
        if (!data.trainingData1.empty() || !data.trainingData2.empty())
        {
            QWidget* pWindow = WindowFactory::create(mpSelf, data, file);
            if (pWindow)
                pWindow->show();
        }
    }
    setWindowTitleFromFile(file);
    return !file.isEmpty();
}

void AnalysisWindow::Private::onSaveAnalysisData()
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
        AnalysisData data;
        data.trainingFiles = mpTrainingFilesWidget->files();
        data.testFiles = mpTestFilesWidget->files();
        data.filterGraph = mpFiltersWidget->graph();
        data.analysisParams = mpAnalysisParamWidget->params();
        updateGraphParameters();
        data.filterParams = mParameters.graph;
        try
        {
            data.serialize(stream);
            setWindowTitleFromFile(file);
            mpAnalysisLogWidget->append(
                QString("Saved analysis data to file \"%1\"").arg(file)
            );
        }
        catch (std::exception &e)
        {
            QMessageBox::critical(
                mpSelf, "Error writing file",
                QString("An error occurred when trying to write file\n\"%1\":\n%2").arg(file).arg(e.what()));
        }
    }
}

void AnalysisWindow::Private::onOpenFilterGraph()
{
    QString file =
        FileDialog::getOpenFileName(mpSelf, "Open Graph File", "BCI2000 Graph File (*.bcigraph)", "output");
    if (!file.isEmpty())
    {
        openFilterGraph(file);
    }
}

void AnalysisWindow::Private::openFilterGraph(const QString& file)
{
    std::ifstream stream(file.toLocal8Bit().constData(), std::ios::binary);
    FilterGraph::Graph graph;
    try
    {
        graph = FilterGraph::Graph::unserialize(stream);
    }
    catch (std::exception &e)
    {
        QMessageBox::critical(
            mpSelf, "Error reading file",
            QString("An error occurred when trying to read file\n\"%1\":\n%2").arg(file).arg(e.what()));
        return;
    }
    mpFilterGraphUserItem->setChecked(true);
    mpFiltersWidget->setGraph(graph);
    mpAnalysisLogWidget->append(
        QString("Read filter graph from file \"%1\"").arg(file)
    );
}

void AnalysisWindow::Private::onSaveFilterGraph()
{
    QString file = FileDialog::getSaveFileName(mpSelf, "Save Graph", "BCI2000 Graph File (*.bcigraph)", "output");
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
            mpFiltersWidget->graph().serialize(stream);
            mpAnalysisLogWidget->append(
                QString("Saved filter graph to file \"%1\"").arg(file)
            );
        }
        catch (std::exception &e)
        {
            QMessageBox::critical(
                mpSelf, "Error writing file",
                QString("An error occurred when writing to file\n\"%1\":\n%2").arg(file).arg(e.what()));
        }
    }
}

void AnalysisWindow::Private::onSavePipeDefinition()
{
    QString file = FileDialog::getSaveFileName(mpSelf, "Save PipeDefinition File", "C++ source file (*.cpp)", "",
                                               "PipeDefinition.cpp");
    if (!file.isEmpty())
    {
        std::ofstream ofs(file.toLocal8Bit().constData());
        FilterGraph::CppTranslator(ofs).translate(mpFiltersWidget->graph());
        mpAnalysisLogWidget->append(
            QString("Saved pipe definition to \"%1\"").arg(file)
        );
    }
}

void AnalysisWindow::Private::onLoadFilterParameters()
{
    QString file = FileDialog::getOpenFileName(mpSelf, "Open Parameter File", "BCI2000 Parameter File (*.prm *.bciprm)");
    if (!file.isEmpty())
    {
        loadFilterParameters(file);
    }
}

void AnalysisWindow::Private::loadFilterParameters(const QString& file)
{
    bool ok = false;
    QString error;
    try {
        ok = mParameters.user.Load(file.toLocal8Bit().constData());
    }
    catch (const std::exception& e) {
        error = QString("An error occurred when trying to read file\n\"%1\": %2").arg(file).arg(e.what());
    }
    if (ok)
    {
        mParameters.graphValid = false;
        mpAnalysisLogWidget->append(
            QString("Read filter parameters from \"%1\"").arg(file)
        );
    }
    else
    {
        QMessageBox::critical(mpSelf, "Error reading parameter file", error);
        mpAnalysisLogWidget->append(
            QString("Failed to read filter parameters from \"%1\"").arg(file)
        );
    }
}

void AnalysisWindow::Private::onSaveFilterParameters()
{
    QString file = FileDialog::getSaveFileName(mpSelf, "Save Filter Parameters", "BCI2000 Parameter File (*.prm *.bciprm)");
    if (!file.isEmpty())
    {
        updateGraphParameters();
        if (mParameters.graph.Save(file.toLocal8Bit().constData()))
        {
            mpAnalysisLogWidget->append(
                QString("Saved filter parameters to \"%1\"").arg(file)
            );
        }
        else
        {
            QMessageBox::critical(mpSelf, "Error writing parameter file",
                QString("An error occurred when writing to file\n\"%1\"").arg(file));
        }
    }
}

void AnalysisWindow::Private::onClearFilterParameters()
{
    mParameters.user.Clear();
    mParameters.graphValid = false;
}

void AnalysisWindow::Private::updateRunAnalysisEnabled()
{
    bool haveFiles = !mpTrainingFilesWidget->data().empty();
    bool haveRunnableGraph = mpFiltersWidget->graph().isRunnable();
    mpRunAnalysis1->setEnabled(haveFiles && haveRunnableGraph);
    mpRunAnalysis2->setEnabled(haveFiles && haveRunnableGraph);
    QString tooltip = "Run Analysis";
    if (!haveFiles)
        tooltip = "Disabled because there are no files to analyze.";
    else if (!haveRunnableGraph)
        tooltip = "Disabled because the graph contains placeholder nodes.";
    mpRunAnalysis2->setToolTip(tooltip);
}

void AnalysisWindow::Private::onTrainingFilesWidgetDataChanged()
{
    mpAnalysisLogWidget->append("Changed training file set");
    mParameters.graphValid = false;
    bool haveFiles = !mpTrainingFilesWidget->data().empty();
    mpTestFilesWidget->setReferenceFile(haveFiles ? mpTrainingFilesWidget->data().front() : nullptr);

    if (mpFilterGraphAutoItem->isChecked())
    {
        auto files = mpTrainingFilesWidget->data();
        FilesWidget::FileClassification fileClass = FilesWidget::Undefined;
        if (!files.empty())
            fileClass = FilesWidget::classifyFile(files.front());
        for (auto pFile : files)
            if (FilesWidget::classifyFile(pFile) != fileClass)
                fileClass = FilesWidget::Unknown;
        if (fileClass != mCurrentClassification)
        {
            mCurrentClassification = fileClass;
            switch (fileClass)
            {
            case FilesWidget::Undefined:
                mpFiltersWidget->clearGraph();
                break;
            case FilesWidget::Unknown:
                mpFiltersWidget->setGraph(FilterGraph::createMinimalGraph());
                break;
            case FilesWidget::SMR:
                mpFiltersWidget->setGraph(FilterGraph::createSMRAnalysisGraph());
                break;
            case FilesWidget::ERP:
                mpFiltersWidget->setGraph(FilterGraph::createERPAnalysisGraph());
                break;
            default:
                ;
            }
        }
    }
    if (mpAnalysisParamsAutoItem->isChecked())
    {
        auto files = mpTrainingFilesWidget->data();
        if (files.empty())
            mpAnalysisParamWidget->setParams(AnalysisParamWidget::AnalysisParams());
        else
        {
            auto params = AnalysisParamWidget::AnalysisParams::createFromFile(files.front());
            mpAnalysisParamWidget->setParams(params);
        }
    }

    updateRunAnalysisEnabled();
}

void AnalysisWindow::Private::onTestFilesWidgetDataChanged()
{
    mpAnalysisLogWidget->append("Changed test file set");
}

void AnalysisWindow::Private::onFilterGraphAutoItemToggled(bool checked)
{
    if (checked && mpTrainingFilesWidget)
        onTrainingFilesWidgetDataChanged();
}

void AnalysisWindow::Private::onFilterGraphSMRItemToggled(bool checked)
{
    if (checked && mCurrentClassification != FilesWidget::SMR)
    {
        mCurrentClassification = FilesWidget::SMR;
        mpFiltersWidget->setGraph(FilterGraph::createSMRAnalysisGraph());
    }
}

void AnalysisWindow::Private::onFilterGraphSMR2ItemToggled(bool checked)
{
    if (checked && mCurrentClassification != FilesWidget::SMR2)
    {
        mCurrentClassification = FilesWidget::SMR2;
        mpFiltersWidget->setGraph(FilterGraph::createAlternativeSMRAnalysisGraph());
    }
}

void AnalysisWindow::Private::onFilterGraphERPItemToggled(bool checked)
{
    if (checked && mCurrentClassification != FilesWidget::ERP)
    {
        mCurrentClassification = FilesWidget::ERP;
        mpFiltersWidget->setGraph(FilterGraph::createERPAnalysisGraph());
    }
}

void AnalysisWindow::Private::onFilterGraphUserItemToggled(bool checked)
{
    if (checked && mCurrentClassification != FilesWidget::Unknown)
    {
        mCurrentClassification = FilesWidget::Unknown;
        mpFiltersWidget->setGraph(FilterGraph::createMinimalGraph());
    }
}

void AnalysisWindow::Private::onFilterGraphComplexExampleItemTriggered()
{
    mpFilterGraphUserItem->setChecked(true);
    mpFiltersWidget->setGraph(FilterGraph::createComplexExampleGraph());
}

void AnalysisWindow::Private::onAnalysisParamsAutoItemToggled(bool checked)
{
    if (checked && mpTrainingFilesWidget)
        onTrainingFilesWidgetDataChanged();
}

void AnalysisWindow::Private::onAnalysisParamsUserItemToggled(bool checked)
{
    if (checked && mpTrainingFilesWidget)
        onTrainingFilesWidgetDataChanged();
}

void AnalysisWindow::Private::onFilterGraphEdited()
{
    mCurrentClassification = FilesWidget::Unknown;
    mpFilterGraphUserItem->setChecked(true);
    mParameters.graphValid = false;
    updateRunAnalysisEnabled();
    mpAnalysisLogWidget->append("Edited filter graph");
}

void AnalysisWindow::Private::onFilterGraphReplaced()
{
    mParameters.graphValid = false;
    updateRunAnalysisEnabled();
    mpAnalysisLogWidget->append("Replaced filter graph");
}

void AnalysisWindow::Private::onAnalysisParamsEdited()
{
    mpAnalysisParamsUserItem->setChecked(true);
}

void AnalysisWindow::Private::onEditFilterParametersTriggered()
{
    bcierr__.Clear();
    updateGraphParameters();
    if (bcierr__.Flushes() > 0)
    {
        QMessageBox::critical(mpSelf, "Error",
            "An error occurred while obtaining parameters from the filter graph.\n"
            "See the filter log window for details.");
        mParameters.graphValid = false;
        return;
    }
    ParamList parameters;
    float hint = 0;
    auto hiddenSections = { "Visualize", "Application" };
    for (const auto& p : mParameters.graph)
    {
        bool hidden = false;
        for (const auto& s : hiddenSections)
            if (p.Section().find(s) == 0)
                hidden = true;
        if (!hidden)
            parameters.Add(p, hint++);
    }
    parameters.Unchanged();

    ConfigWindow configWindow(mpSelf);
    configWindow.Initialize(&parameters);
    configWindow.exec();

    for (const auto &p : parameters)
    {
        if (p.Changed())
        {
            if (mParameters.user.Exists(p.Path()))
                mParameters.user.ByPath(p.Path()).AssignValues(p);
            else
                mParameters.user.Add(p);
            mParameters.graphValid = false;
        }
    }
    mParameters.user.Unchanged();
    mParameters.graph.Unchanged();
    mpAnalysisLogWidget->append("Edited filter parameters");
}

void AnalysisWindow::Private::onResetFilterParametersTriggered()
{
    int result = QMessageBox::question(mpSelf, "Really reset parameters?", 
        "Do you really want to reset parameters?\nIf you click \"yes\", all user changes will be lost.",
        QMessageBox::Yes | QMessageBox::No);
    if (result == 0)
    {
        mParameters.user.Clear();
        mpAnalysisLogWidget->append("Reset filter parameters to default values, and values from first file");
    }
}

void AnalysisWindow::Private::updateGraphParameters()
{
    if (mParameters.graphValid)
        return;

    // create a list of parameters corresponding to the current graph,
    // including parameters from the first file, and user parameters
    mParameters.graph.Clear();
    StateList states;
    auto pFilter = instantiateGraph(mParameters.graph, states);
    delete pFilter;
    mParameters.graphValid = true;
    mParameters.graph.Unchanged();
}

GenericFilter *AnalysisWindow::Private::instantiateGraph(ParamList &parameters, StateList &states)
{
    LogWindow::instance()->clear();
    bcierr__.Clear();
    GenericFilter::RootChain().ClearRegistrars();
    GenericFilter::Registrar::Clear();
    parameters.Add(Param("Source:Source int /SampleBlockSize= 1"), INT_MIN);
    auto ctx = Environment::Context::GlobalInstance();
    ctx->EnterPhase(Environment::construction, &parameters, &states);
    GenericFilter *pFilter = mpFiltersWidget->graph().instantiateAsFilter();
    ctx->EnterPhase(Environment::nonaccess);
    ctx->EnterPhase(Environment::publication, &parameters, &states);
    pFilter->CallPublish();
    ctx->EnterPhase(Environment::nonaccess);
    parameters.Sort();
    parameters.Prune();
    auto files = mpTrainingFilesWidget->data();
    if (!files.empty())
        for (const auto &p : *files.front()->Parameters())
            if (parameters.Exists(p.Path()))
                parameters.ByPath(p.Path()).AssignValues(p);
    for (const auto &p : mParameters.user)
        if (parameters.Exists(p.Path()))
            parameters.ByPath(p.Path()).AssignValues(p);
    return pFilter;
}

void AnalysisWindow::Private::analyze()
{
    auto files1 = mpTrainingFilesWidget->data(), files2 = mpTestFilesWidget->data();
    if (files1.empty())
    {
        QMessageBox::critical(mpSelf, "Error",
                              "You must specify at least one training file in order to analyze data");
        return;
    }

    size_t numTrainingFiles = files1.size();
    auto files = files1;
    files.insert(files.end(), files2.begin(), files2.end());

    auto pFile = files[0];

    AnalysisData analysisData;
    analysisData.trainingFiles = mpTrainingFilesWidget->files();
    analysisData.testFiles = mpTestFilesWidget->files();
    analysisData.analysisParams = mpAnalysisParamWidget->params();
    analysisData.filterGraph = mpFiltersWidget->graph();

    updateGraphParameters();

    auto ctx = Environment::Context::GlobalInstance();
    GenericFilter *pFilter = nullptr;
    try
    {
        ParamList parameters;
        parameters.Add("Source float /SamplingRate= 1Hz");
        for (const auto& p : mParameters.verbosity)
            parameters.Add(p);
        StateList states = *pFile->States();
        pFilter = instantiateGraph(parameters, states);

        FilterGraph::Node* pAnalysisTarget = mpAnalysisTargetWidget->target();
        bool doProfile = !pAnalysisTarget;
        pFilter->EnableProfiling(doProfile);
        struct F 
        {
          const bool doProfile;
          bool operator()(Directory::Node* pNode) {
            auto pFilterNode = dynamic_cast<GenericFilter*>(pNode);
            if (pFilterNode)
              pFilterNode->EnableProfiling(doProfile);
            return true;
          }
        } f = { doProfile };
        Directory::Traverse(pFilter, f);

        ObserverBase* pStatistics = nullptr;
        ApplicationFilter* pApplication = nullptr;
        if (pAnalysisTarget)
        {
            auto pFilter = pAnalysisTarget->filterInstance();
            Assert(pFilter);
            analysisData.target = pFilter->Path();
            analysisData.kind = ClassName(typeid(*pFilter));
            pStatistics = dynamic_cast<ObserverBase*>(pFilter);
            Assert(pStatistics);
            auto params = mpAnalysisParamWidget->params();
            pStatistics->setCondition1(params.condition1);
            pStatistics->setCondition2(params.condition2);
            pStatistics->setObserve(true);
            pApplication = dynamic_cast<ApplicationFilter*>(pFilter);
            if (pApplication)
                pApplication->setEnabled(true);
        }

        bcierr__.MessageContext("Basic configuration");
        ParamRef SampleBlockSize(&parameters.ByPath("SampleBlockSize"));
        if (SampleBlockSize < 1)
        {
            bcierr__ << "Parameter SampleBlockSize must be greater 0";
        }
        else if (SampleBlockSize > pFile->NumSamples())
        {
            bcierr__ << "Parameter SampleBlockSize exceeds number of samples in input file";
        }
        else
        {
            try
            {
                MeasurementUnits::Initialize(parameters);
            }
            catch (const std::exception& e)
            {
                bcierr__ << "When parsing parameter SamplingRate: " << e.what();
            }
        }
        bcierr__.MessageContext("");
        if (!bcierr__.Empty())
            throw std::runtime_error("Configuration errors occurred.\nSee the filter log window for details.");

        SignalProperties InputProperties = pFile->SignalProperties();
        InputProperties.SetElements(SampleBlockSize);
        InputProperties.ValueUnit().SetRawMin(-100).SetRawMax(100).SetGainWithSymbol("1muV");
        states.AssignPositions();
        StateVector statevector(states, InputProperties.Elements());
        ctx->EnterPhase(Environment::preflight, &parameters, &states, &statevector);
        SignalProperties OutputProperties;
        pFilter->CallPreflight(InputProperties, OutputProperties);
        ctx->EnterPhase(Environment::nonaccess);

        if (!bcierr__.Empty())
            throw std::runtime_error(
                "Errors occurred when configuring filters.\nSee the filter log window for details.");

        ctx->EnterPhase(Environment::initialization, &parameters, &states, &statevector);
        pFilter->CallInitialize(InputProperties, OutputProperties);
        ctx->EnterPhase(Environment::nonaccess);

        GenericSignal Input(InputProperties), Output(OutputProperties);
        int sampleBlockSize = InputProperties.Elements();
        int channels = InputProperties.Channels();

        QProgressDialog dialog("Running analysis...", "Cancel", 0, 1000 * files.size(), mpSelf);
        dialog.setWindowModality(Qt::WindowModal);
        dialog.setWindowFlags(dialog.windowFlags() & ~Qt::WindowCloseButtonHint & ~Qt::WindowContextHelpButtonHint);
        dialog.setAutoClose(false);
        dialog.setValue(0);
        dialog.setMinimumDuration(0);
        dialog.show();
        int fileIndex = 0;
        for (auto pData : files)
        {
            if (dialog.wasCanceled())
                break;

            ctx->EnterPhase(Environment::startRun, &parameters, &states, &statevector);
            pFilter->CallStartRun();
            ctx->EnterPhase(Environment::nonaccess);

            for (uint64_t sample = 0; sample < pData->NumSamples(); sample += sampleBlockSize)
            {
                if (dialog.wasCanceled())
                    break;
                for (int s = 0; s < sampleBlockSize; ++s)
                {
                    for (int ch = 0; ch < channels; ++ch)
                        Input(ch, s) = pData->CalibratedValue(ch, sample + s);
                    pData->ReadStateVector(sample + s);
                    for (auto &state : *pData->States())
                    {
                        auto value = pData->StateVector()->StateValue(state.Name(), 0);
                        statevector.SetStateValue(state.Name(), s, value);
                    }
                }
                ctx->EnterPhase(Environment::processing, &parameters, &states, &statevector);
                pFilter->CallProcess(Input, Output);
                ctx->EnterPhase(Environment::nonaccess);
                if (!bcierr__.Empty())
                    throw std::runtime_error(
                        "Errors occurred during processing.\nSee the filter log window for details.");
                dialog.setValue(1000 * fileIndex + (sample * 1000) / pData->NumSamples());
            }
            ctx->EnterPhase(Environment::stopRun, &parameters, &states, &statevector);
            pFilter->CallStopRun();
            ctx->EnterPhase(Environment::nonaccess);
            if (!bcierr__.Empty())
                throw std::runtime_error("Errors occurred after processing.\nSee the filter log window for details.");

            ++fileIndex;
            if (fileIndex == numTrainingFiles && pStatistics)
            {
                analysisData.trainingData1 = pStatistics->data1();
                analysisData.trainingData2 = pStatistics->data2();
                analysisData.trainingTrials1 = pStatistics->trials1();
                analysisData.trainingTrials2 = pStatistics->trials2();
                pStatistics->resetStatistics();
            }
        }
        analysisData.filterParams = mParameters.graph;
        analysisData.filterParams.Join(mParameters.user);
        bool wasCanceled = dialog.wasCanceled();
        dialog.close();

        QWidget *pWindow = nullptr;
        if (doProfile)
        {
            auto graph = mpFiltersWidget->graph();
            graph.gatherPerformanceData();
            graph.clearFilterInstances();
            mpAnalysisLogWidget->append("Ran analysis with \"Profiler\" target");
            pWindow = new ProfilerWindow(graph);
        }
        else if (!wasCanceled && pStatistics)
        {
            analysisData.testData1 = pStatistics->data1();
            analysisData.testData2 = pStatistics->data2();
            analysisData.testTrials1 = pStatistics->trials1();
            analysisData.testTrials2 = pStatistics->trials2();

            if (pApplication) {
                std::ostringstream oss;
                pApplication->Report(oss);
                analysisData.report = oss.str();
            }

            if (pAnalysisTarget)
            {
                mpAnalysisLogWidget->append(
                    QString("Ran analysis with \"%1\" target").arg(pAnalysisTarget->name().c_str())
                );
            }
            else
            {
                mpAnalysisLogWidget->append("Ran analysis with unknown target");
            }
            pWindow = WindowFactory::create(mpSelf, analysisData, mFile);
        }
        if (pWindow)
            pWindow->show();
    }
    catch (const std::runtime_error &e)
    {
        if (e.what() && *e.what())
            QMessageBox::critical(mpSelf, "Error", e.what());
    }

    ctx->EnterPhase(Environment::destruction);
    pFilter->CallHalt();
    delete pFilter;
    ctx->EnterPhase(Environment::nonaccess);
}

AnalysisWindow::AnalysisWindow()
: QMainWindow(nullptr), p(new Private(this))
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    ++sWindowCount;
    setWindowTitle(QApplication::applicationName());

    QMainWindow::move(10, 10);
    QMenu *pMenu = QMainWindow::menuBar()->addMenu("&File");
    QAction *pAddTrainingFileAction = pMenu->addAction("Add &Analysis Files...");
    pAddTrainingFileAction->setShortcut(Qt::CTRL | Qt::Key_D);
    QAction *pAddTestFileAction = pMenu->addAction("Add Classifier T&est Files...");
    pAddTestFileAction->setShortcut(Qt::CTRL | Qt::Key_T);
    pMenu->addSeparator();
    QAction *pAction = pMenu->addAction("Open Filter &Graph...");
    connect(pAction, &QAction::triggered, p, &Private::onOpenFilterGraph);
    pAction = pMenu->addAction("Sa&ve Filter Graph...");
    connect(pAction, &QAction::triggered, p, &Private::onSaveFilterGraph);
    pMenu->addSeparator();
    pAction = pMenu->addAction("Save &PipeDefinition.cpp...");
    connect(pAction, &QAction::triggered, p, &Private::onSavePipeDefinition);
    pMenu->addSeparator();
    pAction = pMenu->addAction("&Load Filter Parameters...");
    connect(pAction, &QAction::triggered, p, &Private::onLoadFilterParameters);
    pAction->setShortcut(Qt::CTRL | Qt::Key_L);
    pAction = pMenu->addAction("Save &Filter Parameters...");
    connect(pAction, &QAction::triggered, p, &Private::onSaveFilterParameters);
    pAction->setShortcut(Qt::CTRL | Qt::Key_F);
    pAction = pMenu->addAction("&Clear Filter Parameters");
    connect(pAction, &QAction::triggered, p, &Private::onClearFilterParameters);
    pMenu->addSeparator();
    pAction = pMenu->addAction("&Open Analysis Data...");
    pAction->setShortcut(Qt::CTRL | Qt::Key_O);
    connect(pAction, &QAction::triggered, p, &Private::onOpenAnalysisData);
    pAction = pMenu->addAction("&Save Analysis Data...");
    pAction->setShortcut(Qt::CTRL | Qt::Key_S);
    connect(pAction, &QAction::triggered, p, &Private::onSaveAnalysisData);
    pMenu->addSeparator();
    pAction = pMenu->addAction("&Quit");
    pAction->setShortcut(Qt::CTRL | Qt::Key_Q);
    connect(pAction, &QAction::triggered, this, &QMainWindow::close);

    pMenu = EditMenu::addToMenuBar(menuBar());
    pMenu->addSeparator();
    pAction = pMenu->addAction("Edit &Filter Parameters...");
    pAction->setShortcut(Qt::CTRL | Qt::Key_P);
    connect(pAction, &QAction::triggered, p, &Private::onEditFilterParametersTriggered);
    pAction = pMenu->addAction("&Reset Filter Parameters");
    connect(pAction, &QAction::triggered, p, &Private::onResetFilterParametersTriggered);

    pMenu = QMainWindow::menuBar()->addMenu("&Analysis");
    QMenu *pMenu2 = pMenu->addMenu("&Filter Graph");
    p->mpFilterGraphAutoItem = pMenu2->addAction("&Auto");
    connect(p->mpFilterGraphAutoItem, &QAction::toggled, p, &Private::onFilterGraphAutoItemToggled);
    p->mpFilterGraphSMRItem = pMenu2->addAction("&SMR/ERD");
    connect(p->mpFilterGraphSMRItem, &QAction::toggled, p, &Private::onFilterGraphSMRItemToggled);
    p->mpFilterGraphSMR2Item = pMenu2->addAction("&SMR/ERD (alt)");
    connect(p->mpFilterGraphSMR2Item, &QAction::toggled, p, &Private::onFilterGraphSMR2ItemToggled);
    p->mpFilterGraphERPItem = pMenu2->addAction("&P300/ERP");
    connect(p->mpFilterGraphERPItem, &QAction::toggled, p, &Private::onFilterGraphERPItemToggled);
    p->mpFilterGraphUserItem = pMenu2->addAction("&User Defined");
    connect(p->mpFilterGraphUserItem, &QAction::toggled, p, &Private::onFilterGraphUserItemToggled);
    pMenu2->addSeparator();
    pMenu2->addAction("Complex E&xample", p, &Private::onFilterGraphComplexExampleItemTriggered);
    auto pGroup = new QActionGroup(this);
    pGroup->addAction(p->mpFilterGraphAutoItem);
    p->mpFilterGraphAutoItem->setCheckable(true);
    p->mpFilterGraphAutoItem->setChecked(true);
    pGroup->addAction(p->mpFilterGraphSMRItem);
    p->mpFilterGraphSMRItem->setCheckable(true);
    pGroup->addAction(p->mpFilterGraphSMR2Item);
    p->mpFilterGraphSMR2Item->setCheckable(true);
    pGroup->addAction(p->mpFilterGraphERPItem);
    p->mpFilterGraphERPItem->setCheckable(true);
    pGroup->addAction(p->mpFilterGraphUserItem);
    p->mpFilterGraphUserItem->setCheckable(true);

    pMenu2 = pMenu->addMenu("Analysis &Conditions");
    p->mpAnalysisParamsAutoItem = pMenu2->addAction("&Auto");
    connect(p->mpAnalysisParamsAutoItem, &QAction::toggled, p, &Private::onAnalysisParamsAutoItemToggled);
    p->mpAnalysisParamsUserItem = pMenu2->addAction("&User Defined");
    connect(p->mpAnalysisParamsUserItem, &QAction::toggled, p, &Private::onAnalysisParamsUserItemToggled);
    pGroup = new QActionGroup(this);
    pGroup->addAction(p->mpAnalysisParamsAutoItem);
    p->mpAnalysisParamsAutoItem->setCheckable(true);
    p->mpAnalysisParamsAutoItem->setChecked(true);
    pGroup->addAction(p->mpAnalysisParamsUserItem);
    p->mpAnalysisParamsUserItem->setCheckable(true);

    pMenu->addSeparator();
    p->mpRunAnalysis1 = pMenu->addAction("&Run Analysis...", p, &Private::analyze);
    p->mpRunAnalysis1->setShortcut(Qt::CTRL | Qt::Key_R);
    p->mpRunAnalysis1->setEnabled(false);

    WindowMenu::addWindow(this);
    HelpMenu::addToMenuBar(menuBar());

    auto pToolBar = QMainWindow::addToolBar("Actions");
    QAction *pAddTrainingFileAction2 = pToolBar->addAction("Add Analysis files...");
    QAction *pAddTestFileAction2 = pToolBar->addAction("Add Classifier test files...");
    p->mpRunAnalysis2 = pToolBar->addAction("Run Analysis", p, &Private::analyze);
    p->mpRunAnalysis2->setEnabled(false);

    QDockWidget *pDockWidget = new QDockWidget("Analysis Files", this);
    pDockWidget->setFeatures(pDockWidget->features() & ~QDockWidget::DockWidgetClosable);
    p->mpTrainingFilesWidget = new FilesWidget(this);
    pDockWidget->setWidget(p->mpTrainingFilesWidget);
    addDockWidget(Qt::LeftDockWidgetArea, pDockWidget);
    connect(p->mpTrainingFilesWidget, &FilesWidget::dataChanged, p, &Private::onTrainingFilesWidgetDataChanged);
    connect(pAddTrainingFileAction, &QAction::triggered, p->mpTrainingFilesWidget, &FilesWidget::addFile);
    connect(pAddTrainingFileAction2, &QAction::triggered, p->mpTrainingFilesWidget, &FilesWidget::addFile);

    pDockWidget = new QDockWidget("Classifier Test Files", this);
    pDockWidget->setFeatures(pDockWidget->features() & ~QDockWidget::DockWidgetClosable);
    p->mpTestFilesWidget = new FilesWidget(this);
    pDockWidget->setWidget(p->mpTestFilesWidget);
    addDockWidget(Qt::LeftDockWidgetArea, pDockWidget);
    connect(p->mpTestFilesWidget, &FilesWidget::dataChanged, p, &Private::onTestFilesWidgetDataChanged);
    connect(pAddTestFileAction, &QAction::triggered, p->mpTestFilesWidget, &FilesWidget::addFile);
    connect(pAddTestFileAction2, &QAction::triggered, p->mpTestFilesWidget, &FilesWidget::addFile);

    pDockWidget = new QDockWidget("Analysis Parameters", this);
    pDockWidget->setFeatures(pDockWidget->features() & ~QDockWidget::DockWidgetClosable);
    p->mpAnalysisParamWidget = new AnalysisParamWidget(this);
    pDockWidget->setWidget(p->mpAnalysisParamWidget);
    addDockWidget(Qt::LeftDockWidgetArea, pDockWidget);
    connect(p->mpAnalysisParamWidget, &AnalysisParamWidget::conditionsEdited, p, &Private::onAnalysisParamsEdited);
    connect(p->mpAnalysisParamWidget, &AnalysisParamWidget::editFilterParamsClicked, p,
            &Private::onEditFilterParametersTriggered);
    connect(p->mpAnalysisParamWidget, &AnalysisParamWidget::resetFilterParamsClicked, p,
        &Private::onResetFilterParametersTriggered);

    pDockWidget = new QDockWidget("Filter Log", this);
    pDockWidget->setFeatures(pDockWidget->features() & ~QDockWidget::DockWidgetClosable);
    pDockWidget->setWidget(new LogWindow(&p->mParameters.verbosity));
    addDockWidget(Qt::RightDockWidgetArea, pDockWidget);

    FiltersWidget *pFiltersWidget = new FiltersWidget(this);
    pFiltersWidget->setEditable(true);
    connect(pFiltersWidget, &FiltersWidget::edited, p, &Private::onFilterGraphEdited);
    connect(pFiltersWidget, &FiltersWidget::graphReplaced, p, &Private::onFilterGraphReplaced);
    p->mpFiltersWidget = pFiltersWidget;
    layout()->setContentsMargins(0, 0, 0, 0);
    setCentralWidget(pFiltersWidget);

    pDockWidget = new QDockWidget("Analysis Target", this);
    pDockWidget->setFeatures(pDockWidget->features() & ~QDockWidget::DockWidgetClosable);
    p->mpAnalysisTargetWidget = new AnalysisTargetWidget(pFiltersWidget);
    pDockWidget->setWidget(p->mpAnalysisTargetWidget);
    addDockWidget(Qt::RightDockWidgetArea, pDockWidget);

    pDockWidget = new QDockWidget("Analysis Log", this);
    pDockWidget->setFeatures(pDockWidget->features() & ~QDockWidget::DockWidgetClosable);
    p->mpAnalysisLogWidget = new AnalysisLogWidget;
    pDockWidget->setWidget(p->mpAnalysisLogWidget);
    addDockWidget(Qt::BottomDockWidgetArea, pDockWidget);

    setMinimumHeight(620);
    setMinimumWidth(940);

    p->updateRunAnalysisEnabled();

    if (!WindowSettings::read(this))
        showMaximized();
}

AnalysisWindow::~AnalysisWindow()
{
    WindowSettings::write(this);
    WindowMenu::removeWindow(this);
}

void AnalysisWindow::setFilterParameters(const ParamList &params, const QWidget* pWindow)
{
    p->mParameters.user.Join(params);
    p->mParameters.graphValid = false;
    p->mpAnalysisLogWidget->append(QString("Applied filter parameters from %1").arg(pWindow->windowTitle()));
}

const ParamList &AnalysisWindow::filterParameters() const
{
    p->updateGraphParameters();
    return p->mParameters.graph;
}

void AnalysisWindow::closeEvent(QCloseEvent *ev)
{
    QMainWindow::closeEvent(ev);
    if (--sWindowCount == 0)
        QApplication::quit();
}

void AnalysisWindow::openAnalysisFile(const QString& file)
{
    if (p->openAnalysisDataFile(file))
        FileDialog::setDefaultDirectory(QFileInfo(file).path());
}

void AnalysisWindow::openDataFiles(const QStringList& files)
{
    p->mpTrainingFilesWidget->addFiles(files);
    if (!files.empty())
        FileDialog::setDefaultDirectory(QFileInfo(files.front()).path());
}
