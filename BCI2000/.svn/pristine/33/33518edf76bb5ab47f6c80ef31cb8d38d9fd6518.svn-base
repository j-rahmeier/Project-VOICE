#include "pages.h"
#include "FileUtils.h"
#include "ProcessUtils.h"
#include "configdialog.h"
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include <iomanip>
#include <set>

static WeightsWorkerThread *spGenerateFeatureWeightsThread = nullptr;
static DataPage *spDataPage = nullptr;
static ParametersPage *spParametersPage = nullptr;
static DetailsPage *spDetailsPage = nullptr;
static QErrorMessage *spErrorMessageDialog = nullptr;

static bool ReadVector(const QString& inText, QVector<int>& outVector)
{
    if (inText.length() > 0)
    {
        QStringList elements = inText.simplified().split(QString(" "));
        outVector.resize(elements.size());

        for (int idx = 0; idx < elements.size(); idx++)
        {
            bool success = false;
            outVector[idx] = elements.value(idx).toInt(&success);
            if (!success)
                return false;
        }
        return true;
    }
    return false;
}

static void ShowToolTip(QLineEdit* lineEdit, QString message)
{
    QPoint pos(lineEdit->x(), lineEdit->y());
    pos.setX(lineEdit->parentWidget()->mapToGlobal(pos).x());
    pos.setY(lineEdit->parentWidget()->mapToGlobal(pos).y() + 10);

    QRect rect(lineEdit->pos().x(), lineEdit->pos().y(), 100, 100);
    QToolTip::showText(pos, message, lineEdit, rect);
}

static void MarkEditColor(QLineEdit* lineEdit, bool result)
{
    if (result)
    {
        QPalette palette(lineEdit->palette());
        palette.setColor(QPalette::Base, QColor("lightgreen"));
        palette.setColor(QPalette::Text, QColor("black"));
        lineEdit->setPalette(palette);
        QToolTip::hideText();
    }
    else
    {
        QPalette palette(lineEdit->palette());
        palette.setColor(QPalette::Base, QColor("lightpink"));
        palette.setColor(QPalette::Text, QColor("black"));
        lineEdit->setPalette(palette);
    }
}

// jm, Oct 24, 2012
static int FixupStimulusCodes(ap::template_1d_array<unsigned short int, true> &ioCodes)
{
    std::set<int> codes;
    for (int i = 0; i <= ioCodes.gethighbound(0); ++i)
        codes.insert(ioCodes(i));
    codes.erase(0);
    if (!codes.empty())
    {
        int minCode = *codes.begin();
        if (minCode > 1)
            for (int i = 0; i <= ioCodes.gethighbound(0); ++i)
                ioCodes(i) -= (minCode - 1);
    }
    return static_cast<int>(codes.size());
}

static void Reset()
{
    spDetailsPage->Clear();
    spDataPage->findChild<QProgressBar *>()->setValue(0);
}

static std::ostream &PrintClassificationResults(std::ostream &oss, const std::vector<std::string> &predicted,
                                                const std::vector<double> &vresult)
{
    if (vresult.empty())
        return oss << "No classification results available (unknown target specification).\n\n";

    // Display classification results
    std::ios_base::fmtflags savedFlags = oss.flags();
    oss.precision(0);
    oss.setf(std::ios::fixed, std::ios::floatfield);
    oss.setf(std::ios::left, std::ios::adjustfield);
    int width = static_cast<int>(predicted.size()) / static_cast<int>(vresult.size());
    oss << "\nClassifying Responses...\n\n";
    oss << std::setw(12) << "Flashes";
    oss << std::setw(12) << "% Correct";
    oss << std::setw(width) << "Predicted Symbols";
    oss << "\n";

    for (size_t k = 0; k < vresult.size(); k++)
    {
        oss << std::setw(12) << k + 1;
        oss << std::setw(12) << vresult[k];
        for (size_t j = k * width; j < (k + 1) * width; j++)
            oss << predicted[j];
        oss << "\n";
    }
    oss.flags(savedFlags);
    return oss << "\n";
}

// end jm

static void callback_status(const std::string &message)
{
    if (spGenerateFeatureWeightsThread)
        spGenerateFeatureWeightsThread->ProgressTextCallBack(message.c_str());
}

DataPage::DataPage(QWidget *parent) : QWidget(parent), mAutoWrite(false),
  mGenerateFeatureWeightsThread(true), mApplyFeatureWeightsThread(false)
{
    spErrorMessageDialog = new QErrorMessage(this);

    // Groups
    auto pTrainingDataGroup = new QGroupBox(tr("[-TrainingDataFiles] Training Data Files"));
    auto pTestingDataGroup = new QGroupBox(tr("[-TestingDataFiles] Testing Data Files"));
    auto pInicfgGroup = new QGroupBox(tr("[-inicfg] Feature Extraction INI File"));
    auto pWritePRMGroup = new QGroupBox(tr("[-ClassifierOutputFile] Write *.prm File"));
    auto pOverAllProgressGroup = new QGroupBox(tr("Overall Progress"));

    // objects
    mpInicfgFileLineEdit = new QLineEdit;
    mpWritePRMLineEdit = new QLineEdit;
    mpInicfgFileLineEdit->setReadOnly(true);
    mpWritePRMLineEdit->setReadOnly(false);

    mpProgressBarOverall = new QProgressBar();
    mpProgressBarOverall->setRange(0, 100);
    mpProgressBarOverall->setTextVisible(false);
    mpProgressBarOverall->setValue(0);

    mpLoadTrainingDataFilesButton = new QPushButton(tr("Load Training Data Files"));
    mpLoadTestingDataFilesButton = new QPushButton(tr("Load Testing Data Files"));
    mpLoadIniFileButton = new QPushButton(tr("Load Ini File"));
    mpGenerateFeatureWeights = new QPushButton(tr("Generate Feature Weights"));
    mpApplyFeatureWeights = new QPushButton(tr("Apply Feature Weights"));
    mpFileWritePRMButton = new QPushButton(tr("Write *.prm File"));
    mpGenerateFeatureWeights->setEnabled(false);
    mpApplyFeatureWeights->setEnabled(false);
    mpFileWritePRMButton->setEnabled(false);

    mpTrainingDataFilesList = new QListWidget;

    mpTestingDataFilesList = new QListWidget;

    // Conect the signals
    connect(mpLoadTrainingDataFilesButton, SIGNAL(clicked()), this, SLOT(LoadTrainingButton()));
    connect(mpLoadTestingDataFilesButton, SIGNAL(clicked()), this, SLOT(LoadTestingButton()));
    connect(mpLoadIniFileButton, SIGNAL(clicked()), this, SLOT(LoadIniButton()));
    connect(mpGenerateFeatureWeights, SIGNAL(clicked()), this, SLOT(GenerateFeatures()));
    connect(mpApplyFeatureWeights, SIGNAL(clicked()), this, SLOT(ApplyFeatures()));
    connect(mpFileWritePRMButton, SIGNAL(clicked()), this, SLOT(WriteParameterFragment()));

    connect(&mGenerateFeatureWeightsThread, SIGNAL(signalProgressText(QString)), this, SLOT(slotProgressText(QString)));
    connect(&mGenerateFeatureWeightsThread, SIGNAL(signalProgressBar(int,int,int)), this,
            SLOT(slotProgressBar(int,int,int)));
    connect(&mGenerateFeatureWeightsThread, SIGNAL(errorMessage(QString)), this, SLOT(slotErrorMessageDialog(QString)));
    connect(&mGenerateFeatureWeightsThread, SIGNAL(done()), this, SLOT(OnFeatureWeightsDone()));

    connect(&mApplyFeatureWeightsThread, SIGNAL(signalProgressText(QString)), this, SLOT(slotProgressText(QString)));
    connect(&mApplyFeatureWeightsThread, SIGNAL(signalProgressBar(int,int,int)), this,
            SLOT(slotProgressBar(int,int,int)));
    connect(&mApplyFeatureWeightsThread, SIGNAL(errorMessage(QString)), this, SLOT(slotErrorMessageDialog(QString)));

    // layout manager
    auto pTrainingDataLayout = new QVBoxLayout;
    auto pTestingDataLayout = new QVBoxLayout;
    auto pInicfgLayout = new QVBoxLayout;
    auto pWritePRMLayout = new QVBoxLayout;
    auto pOverAllProgressLayout = new QVBoxLayout;

    // wire them together
    pTrainingDataLayout->addWidget(mpTrainingDataFilesList);
    pTrainingDataLayout->addSpacing(12);
    pTrainingDataLayout->addWidget(mpLoadTrainingDataFilesButton);
    pTestingDataLayout->addWidget(mpTestingDataFilesList);
    pTestingDataLayout->addSpacing(12);
    pTestingDataLayout->addWidget(mpLoadTestingDataFilesButton);
    pInicfgLayout->addWidget(mpInicfgFileLineEdit);
    pInicfgLayout->addSpacing(12);
    pInicfgLayout->addWidget(mpLoadIniFileButton);
    pWritePRMLayout->addWidget(mpWritePRMLineEdit);
    pWritePRMLayout->addWidget(mpFileWritePRMButton);
    pOverAllProgressLayout->addWidget(mpProgressBarOverall);
    pOverAllProgressLayout->addWidget(mpGenerateFeatureWeights);
    pOverAllProgressLayout->addWidget(mpApplyFeatureWeights);

    pTrainingDataGroup->setLayout(pTrainingDataLayout);
    pTestingDataGroup->setLayout(pTestingDataLayout);
    pInicfgGroup->setLayout(pInicfgLayout);
    pWritePRMGroup->setLayout(pWritePRMLayout);
    pOverAllProgressGroup->setLayout(pOverAllProgressLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(pTrainingDataGroup);
    mainLayout->addWidget(pTestingDataGroup);
    mainLayout->addWidget(pInicfgGroup);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(pOverAllProgressGroup);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(pWritePRMGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);

    spDataPage = this;
    spGenerateFeatureWeightsThread = &mGenerateFeatureWeightsThread;

    mPathIniData = QString::fromLocal8Bit(ProcessUtils::InitialProcessWD().c_str());
    std::string path = FileUtils::InstallationDirectory() + "../../data";
    if (!FileUtils::IsDirectory(path))
        path = FileUtils::InstallationDirectory();
    mPathTrainingData = QString::fromLocal8Bit(path.c_str());
    mPathTestingData = mPathTrainingData;
}

ParametersPage::ParametersPage(QWidget *parent) : QWidget(parent)
{
    // groups
    auto pClassifierSettingsGroup = new QGroupBox(tr("Classifier Settings"));
    auto pFilterSettingsGroup = new QGroupBox(tr("Filter Settings"));
    auto pSpatialSettingsGroup = new QGroupBox(tr("Spatial Settings"));
    auto pTemporalSettingsGroup = new QGroupBox(tr("Temporal Settings"));

    // objects
    mpStimulusOnsetExpressionLineEdit = new QLineEdit;
    mpRespWindLineEdit = new QLineEdit;
    mpDecFreLineEdit = new QLineEdit;
    mpMaxModFeaLineEdit = new QLineEdit;
    mpChSetLineEdit = new QLineEdit;
    mpPenterLineEdit = new QLineEdit;
    mpPremoveLineEdit = new QLineEdit;

    mpSpatFilterComboBox = new QComboBox;
    mpSpatFilterComboBox->addItem(tr("Raw"));
    mpSpatFilterComboBox->addItem(tr("CAR"));

    auto pStimulusOnsetExpressionLabel = new QLabel(tr("Stimulus Onset Expression:"));
    auto pRespWindLabel = new QLabel(tr("Response Window [begin end] (ms):"));
    auto pDecFreLabel = new QLabel(tr("Decimation Frequency (Hz):"));
    auto pMaxModFeaLabel = new QLabel(tr("Max Model Features:"));
    auto pChSetLabel = new QLabel(tr("Channel Set:"));
    auto pPenterLabel = new QLabel(tr("Penter:"));
    auto pPremoveLabel = new QLabel(tr("Premove:"));
    auto pSpatFilterLabel = new QLabel(tr("Spatial Filter:"));

    // regular expressions
    QRegularExpression regExpFloat("([0]+)\\.([0-9]*)");
    QRegularExpression regExpInt("(([0-9]|[1-9][0-9]|[1-9][0-9][0-9])\\s)*");
    QRegularExpression regExpIntMF("^([1-9]|[1-9][0-9]|[1-9][0-9][0-9])$");
    QRegularExpression regExpIntDF("^([1-9]|[1-9][0-9])$");
    QRegularExpression regExpIntRW("((([0-9])|([1-9][0-9])|([1-9][0-9][0-9])|([1-9][0-9][0-9][0-9]))\\s(([1-9])|([1-9][0-9])|([1-"
                        "9][0-9][0-9])|([1-9][0-9][0-9][0-9])))");

    // validators
    auto pValidatorMaxModelFeatures = new QRegularExpressionValidator(regExpIntMF, mpMaxModFeaLineEdit);
    auto pValidatorDecFreq = new QRegularExpressionValidator(regExpIntDF, mpDecFreLineEdit);
    auto pValidatorChSet = new QRegularExpressionValidator(regExpInt, mpChSetLineEdit);
    auto pValidatorPenter = new QRegularExpressionValidator(regExpFloat, mpPenterLineEdit);
    auto pValidatorPremove = new QRegularExpressionValidator(regExpFloat, mpPremoveLineEdit);
    auto pValidatorRespWind = new QRegularExpressionValidator(regExpIntRW, mpRespWindLineEdit);

    mpMaxModFeaLineEdit->setValidator(pValidatorMaxModelFeatures);
    mpDecFreLineEdit->setValidator(pValidatorDecFreq);
    mpChSetLineEdit->setValidator(pValidatorChSet);
    mpPenterLineEdit->setValidator(pValidatorPenter);
    mpPremoveLineEdit->setValidator(pValidatorPremove);
    mpRespWindLineEdit->setValidator(pValidatorRespWind);
    mpStimulusOnsetExpressionLineEdit->setText("StimulusBegin>0");
    MarkEditColor(mpStimulusOnsetExpressionLineEdit, true);

    // track changes
    connect(mpMaxModFeaLineEdit, SIGNAL(textEdited(QString)), this, SLOT(TextHasChanged()));
    connect(mpDecFreLineEdit, SIGNAL(textEdited(QString)), this, SLOT(TextHasChanged()));
    connect(mpPenterLineEdit, SIGNAL(textEdited(QString)), this, SLOT(TextHasChanged()));
    connect(mpPremoveLineEdit, SIGNAL(textEdited(QString)), this, SLOT(TextHasChanged()));
    connect(mpRespWindLineEdit, SIGNAL(textEdited(QString)), this, SLOT(TextHasChanged()));
    connect(mpStimulusOnsetExpressionLineEdit, SIGNAL(textEdited(QString)), this, SLOT(OnsetExpressionHasChanged()));
    connect(mpSpatFilterComboBox, SIGNAL(activated(int)), this, SLOT(TextHasChanged()));
    connect(mpChSetLineEdit, SIGNAL(textEdited(QString)), this, SLOT(ChSetTextHasChanged()));

    // layout manager
    auto pClassifierLayout = new QGridLayout;
    auto pFilterLayout = new QGridLayout;
    auto pSpatialLayout = new QGridLayout;
    auto pTemporalLayout = new QGridLayout;

    // wire them together
    pClassifierLayout->addWidget(pMaxModFeaLabel, 0, 0);
    pClassifierLayout->addWidget(mpMaxModFeaLineEdit, 0, 1);
    pClassifierLayout->addWidget(pPenterLabel, 2, 0);
    pClassifierLayout->addWidget(mpPenterLineEdit, 2, 1);
    pClassifierLayout->addWidget(pPremoveLabel, 3, 0);
    pClassifierLayout->addWidget(mpPremoveLineEdit, 3, 1);

    pFilterLayout->addWidget(pSpatFilterLabel, 0, 0);
    pFilterLayout->addWidget(mpSpatFilterComboBox, 0, 1);
    pFilterLayout->addWidget(pDecFreLabel, 1, 0);
    pFilterLayout->addWidget(mpDecFreLineEdit, 1, 1);

    pSpatialLayout->addWidget(pChSetLabel, 0, 0);
    pSpatialLayout->addWidget(mpChSetLineEdit, 0, 1);

    pTemporalLayout->addWidget(pStimulusOnsetExpressionLabel, 0, 0);
    pTemporalLayout->addWidget(mpStimulusOnsetExpressionLineEdit, 0, 1);
    pTemporalLayout->addWidget(pRespWindLabel, 1, 0);
    pTemporalLayout->addWidget(mpRespWindLineEdit, 1, 1);

    pClassifierSettingsGroup->setLayout(pClassifierLayout);
    pFilterSettingsGroup->setLayout(pFilterLayout);
    pSpatialSettingsGroup->setLayout(pSpatialLayout);
    pTemporalSettingsGroup->setLayout(pTemporalLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(pClassifierSettingsGroup);
    mainLayout->addWidget(pFilterSettingsGroup);
    mainLayout->addWidget(pSpatialSettingsGroup);
    mainLayout->addWidget(pTemporalSettingsGroup);

    setLayout(mainLayout);

    spParametersPage = this;
}

DetailsPage::DetailsPage(QWidget *parent) : QWidget(parent)
{
    // Groups
    auto pProgressGroup = new QGroupBox(tr("Overall Progress"));
    auto pFileInfoGroup = new QGroupBox(tr("BCI2000 FileInfo"));

    // Progress Bar
    mpProgressBarOverall = new QProgressBar();
    mpProgressBarOverall->setRange(0, 100);
    mpProgressBarOverall->setTextVisible(false);
    mpProgressBarOverall->setValue(0);

    // Text Edit
    mpProgressTextEdit = new QTextEdit;
    mpProgressTextEdit->setReadOnly(true);
    mpProgressTextEdit->setCurrentFont(QFont("Courier", 10));

    // Labels
    auto pSamplingRateLabel = new QLabel(tr("Sampling Rate (Hz):"));
    auto pSourceChListLabel = new QLabel(tr("Number of Channels:"));
    auto pClassifierLabel = new QLabel(tr("Classifier:"));
    auto pApplicationFilterChainLabel = new QLabel(tr("Application:"));
    auto pInterpretModeLabel = new QLabel(tr("Interpret Mode:"));
    auto pTotalNumberOfSamplesLabel = new QLabel(tr("Duration (s):"));

    // Line Edit
    mpSamplingRateLineEdit = new QLineEdit;
    mpSourceChListLineEdit = new QLineEdit;
    mpClassifierLineEdit = new QLineEdit;
    mpApplicationFilterChainLineEdit = new QLineEdit;
    mpInterpretModeLineEdit = new QLineEdit;
    mpTotalNumberofSamplesLineEdit = new QLineEdit;

    mpSamplingRateLineEdit->setReadOnly(true);
    mpSourceChListLineEdit->setReadOnly(true);
    mpClassifierLineEdit->setReadOnly(true);
    mpApplicationFilterChainLineEdit->setReadOnly(true);
    mpInterpretModeLineEdit->setReadOnly(true);
    mpTotalNumberofSamplesLineEdit->setReadOnly(true);

    // Layouts
    auto pOverallProgressLayout = new QVBoxLayout;
    auto pFileInfoLayout = new QGridLayout;

    // wire them together
    pOverallProgressLayout->addWidget(mpProgressBarOverall);
    pOverallProgressLayout->addWidget(mpProgressTextEdit);
    pFileInfoLayout->addWidget(pSamplingRateLabel, 0, 0);
    pFileInfoLayout->addWidget(mpSamplingRateLineEdit, 0, 1);
    pFileInfoLayout->addWidget(pSourceChListLabel, 1, 0);
    pFileInfoLayout->addWidget(mpSourceChListLineEdit, 1, 1);
    pFileInfoLayout->addWidget(pClassifierLabel, 2, 0);
    pFileInfoLayout->addWidget(mpClassifierLineEdit, 2, 1);
    pFileInfoLayout->addWidget(pApplicationFilterChainLabel, 3, 0);
    pFileInfoLayout->addWidget(mpApplicationFilterChainLineEdit, 3, 1);
    pFileInfoLayout->addWidget(pInterpretModeLabel, 4, 0);
    pFileInfoLayout->addWidget(mpInterpretModeLineEdit, 4, 1);
    pFileInfoLayout->addWidget(pTotalNumberOfSamplesLabel, 5, 0);
    pFileInfoLayout->addWidget(mpTotalNumberofSamplesLineEdit, 5, 1);

    pProgressGroup->setLayout(pOverallProgressLayout);
    pFileInfoGroup->setLayout(pFileInfoLayout);

    auto pMainLayout = new QVBoxLayout;
    pMainLayout->addWidget(pFileInfoGroup);
    pMainLayout->addWidget(pProgressGroup);

    setLayout(pMainLayout);

    spDetailsPage = this;
}

void DetailsPage::Clear()
{
    QList<QLineEdit *> fields = findChildren<QLineEdit *>();
    for (int i = 0; i < fields.size(); ++i)
        fields[i]->setText("");
    QList<QTextEdit *> fields2 = findChildren<QTextEdit *>();
    for (int i = 0; i < fields2.size(); ++i)
        fields2[i]->setText("");
    findChild<QProgressBar *>()->setValue(0);
}

void ParametersPage::setDataPage(DataPage *dataPage)
{
    mpDataPage = dataPage;
}

void DataPage::setParametersPage(ParametersPage *parametersPage)
{
    mpParametersPage = parametersPage;
}

void ParametersPage::TextHasChanged()
{
    mpDataPage->mpApplyFeatureWeights->setEnabled(false);
    mpDataPage->ValidateSettings();
}

void ParametersPage::ChSetTextHasChanged()
{
    mpDataPage->mpApplyFeatureWeights->setEnabled(false);

    QString ch = mpChSetLineEdit->text();
    QVector<int> ChSet;
    ReadVector(ch, ChSet);
    mpDataPage->mUserParams.channel_set.clear();

    if (ChSet.size() > 0)
    {
        for (int i = 0; i < ChSet.size(); i++)
            if ((ChSet[i] > mpDataPage->mNumChannelsTrainingData) || (ChSet[i] <= 0))
            {
                std::ostringstream oss;
                oss << "The Channel Set provided is not a subset of the channels contained in the input file";
                ShowToolTip(mpChSetLineEdit, oss.str().c_str());
                MarkEditColor(mpChSetLineEdit, false);
                mpDataPage->mpGenerateFeatureWeights->setEnabled(false);
                return;
            }
            else
            {
                MarkEditColor(mpChSetLineEdit, true);
                mpDataPage->mUserParams.channel_set.push_back(ChSet[i]);
                mpDataPage->mpGenerateFeatureWeights->setEnabled(true);
            }
    }
    else
    {
        std::ostringstream oss;
        oss << "Channel Set must have at least one channel.";
        ShowToolTip(mpChSetLineEdit, oss.str().c_str());
        MarkEditColor(mpChSetLineEdit, false);
        mpDataPage->mpGenerateFeatureWeights->setEnabled(false);
    }

    if (mpDataPage->mTrainingDataFiles.size() == 0)
        mpDataPage->mpGenerateFeatureWeights->setEnabled(false);

    if (mpDataPage->mTrainingDataFiles.size() > 0)
        if (!mpDataPage->mTrainingDataFilesValid)
        {
            std::ostringstream oss;
            oss << "Load the Training Data Files again";
            QMessageBox msgBox(QMessageBox::Warning, tr("Warning"), oss.str().c_str(), QMessageBox::NoButton, this);
            msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
            msgBox.exec();
            mpDataPage->mpGenerateFeatureWeights->setEnabled(false);
        }
}

void ParametersPage::OnsetExpressionHasChanged()
{
    mpDataPage->mpApplyFeatureWeights->setEnabled(false);
    std::string expression = mpStimulusOnsetExpressionLineEdit->text().toUtf8().constData();
    std::string err = mpDataPage->ValidateOnsetExpression(expression);
    if (err.empty())
    {
        MarkEditColor(mpStimulusOnsetExpressionLineEdit, true);
        mpDataPage->ValidateSettings();
    }
    else
    {
        ShowToolTip(mpStimulusOnsetExpressionLineEdit, QString::fromUtf8(err.c_str()));
        MarkEditColor(mpStimulusOnsetExpressionLineEdit, false);
        mpDataPage->mpGenerateFeatureWeights->setEnabled(false);
    }
}

void DataPage::LoadSettings()
{
    Reset();

    std::string szFile = mpInicfgFileLineEdit->text().toLocal8Bit().constData();
    char format = 'f';
    int precision = 4;

    bool IsFile = ReadIniParameters(szFile, mUserParams);

    if (!IsFile)
    {
        mUserParams.maxiter = 60;
        mUserParams.penter = 0.1000f;
        mUserParams.premove = 0.1500f;
        mUserParams.Decimation_Frequency = 20;
        for (int i = 0; i < 8; i++)
            mUserParams.channel_set.push_back(i + 1);

        QPalette palette(mpParametersPage->mpChSetLineEdit->palette());
        palette.setColor(QPalette::Base, QColor("lightgreen"));
        palette.setColor(QPalette::Text, QColor("black"));
        mpParametersPage->mpChSetLineEdit->setPalette(palette);

        mUserParams.windlen.push_back(0);
        mUserParams.windlen.push_back(800);
        mUserParams.SF = UserParameters::Raw;
    }

    mpParametersPage->mpMaxModFeaLineEdit->setText(QString::number(mUserParams.maxiter));
    mpParametersPage->mpPenterLineEdit->setText(QString::number(mUserParams.penter, format, precision));
    mpParametersPage->mpPremoveLineEdit->setText(QString::number(mUserParams.premove, format, precision));
    mpParametersPage->mpDecFreLineEdit->setText(QString::number(mUserParams.Decimation_Frequency));

    QString chset;
    QString respwind;

    for (size_t i = 0; i < static_cast<int>(mUserParams.channel_set.size()); i++)
        if (i != static_cast<int>(mUserParams.channel_set.size()) - 1)
            QTextStream(&chset) << mUserParams.channel_set[i] << " ";
        else
            QTextStream(&chset) << mUserParams.channel_set[i];

    for (size_t i = 0; i < static_cast<int>(mUserParams.windlen.size()); i++)
        QTextStream(&respwind) << mUserParams.windlen[i] << " ";

    mpParametersPage->mpChSetLineEdit->setText(chset);
    mpParametersPage->mpRespWindLineEdit->setText(respwind);

    if (mUserParams.SF == UserParameters::Raw)
        mpParametersPage->mpSpatFilterComboBox->setCurrentIndex(0);

    if (mUserParams.SF == UserParameters::CAR)
        mpParametersPage->mpSpatFilterComboBox->setCurrentIndex(1);
}

void DataPage::SetTrainingFiles(const QStringList& files)
{
    Reset();
    mpTrainingDataFilesList->clear();
    mTrainingDataFiles.clear();

    QString openFilesPath;
    for (int i = 0; i < files.count(); i++)
    {
        openFilesPath = files[i];
        mTrainingDataFiles.push_back(openFilesPath.toLocal8Bit().constData());
        mpTrainingDataFilesList->addItem(openFilesPath);
    }
    mTrainingDataFilesValid = false;
    if (!mTrainingDataFiles.empty())
        mTrainingDataFilesValid = ValidateDataFiles(true, mTrainingDataFiles, mNumSamplesTrainingData,
            mNumChannelsTrainingData, mModeTrainingData);
    mpGenerateFeatureWeights->setEnabled(mTrainingDataFilesValid);

    mPathTrainingData = openFilesPath;

    if (mTrainingDataFilesValid)
    {
        mpParametersPage->ChSetTextHasChanged();
        mpParametersPage->TextHasChanged();
        mpParametersPage->OnsetExpressionHasChanged();
    }
}

void DataPage::LoadTrainingButton()
{
    QStringList files =
        QFileDialog::getOpenFileNames(this, tr("Load Training Data Files"), mPathTrainingData, tr("Data Files (*.dat)"));

    if (files.count() > 0)
        SetTrainingFiles(files);
}

void DataPage::SetTestingFiles(const QStringList& files)
{
    mpTestingDataFilesList->clear();
    mTestingDataFiles.clear();

    QString openFilesPath;

    for (int i = 0; i < files.count(); i++)
    {
        openFilesPath = files[i];
        mTestingDataFiles.push_back(openFilesPath.toLocal8Bit().constData());
        mpTestingDataFilesList->addItem(openFilesPath);
    }

    mTestingDataFilesValid = false;
    if (!mTestingDataFiles.empty())
        mTestingDataFilesValid = ValidateDataFiles(false, mTestingDataFiles, mNumSamplesTestingData,
            mNumChannelsTestingData, mModeTestingData);

    if (!mTestingDataFilesValid)
        mpApplyFeatureWeights->setEnabled(false);
    else if (mWeightsValid)
        mpApplyFeatureWeights->setEnabled(true);

    mPathTestingData = openFilesPath;

    mpParametersPage->ChSetTextHasChanged();
    mpParametersPage->TextHasChanged();
    mpParametersPage->OnsetExpressionHasChanged();
}

void DataPage::LoadTestingButton()
{
    QStringList files =
        QFileDialog::getOpenFileNames(this, tr("Load Testing Data Files"), mPathTestingData, tr("Data Files (*.dat)"));

    if (files.count() > 0)
        SetTestingFiles(files);
}

void DataPage::SetIniFile(const QString& file)
{
    mpInicfgFileLineEdit->setText(file);
    LoadSettings();
    ValidateSettings();
    mPathIniData = file;
    if (mWeightsValid)
        mpApplyFeatureWeights->setEnabled(false);
}

void DataPage::LoadIniButton()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Load Ini File"), mPathIniData, tr("Ini Files (*.ini)"));
    if (file.length() > 0)
        SetIniFile(file);
}

void DataPage::ValidateSettings()
{
    bool result = true;

    // result &= ValidateLineEdit(mpParametersPage->MaxModFeaLineEdit);

    QString MaxModFeat = mpParametersPage->mpMaxModFeaLineEdit->text();
    QString penter = mpParametersPage->mpPenterLineEdit->text();
    QString premove = mpParametersPage->mpPremoveLineEdit->text();
    QString DecFre = mpParametersPage->mpDecFreLineEdit->text();
    QString RespWind = mpParametersPage->mpRespWindLineEdit->text();

    mUserParams.SF = mpParametersPage->mpSpatFilterComboBox->currentIndex() + 1;

    bool ok;
    bool validMaxModFeat, validPenterPremove, validDecFre, validRespWind;

    // Validate Max Model Features
    if (MaxModFeat.toInt(&ok, 10) <= 0)
    {
        ShowToolTip(mpParametersPage->mpMaxModFeaLineEdit, tr("Max Model Features must be > 0"));
        MarkEditColor(mpParametersPage->mpMaxModFeaLineEdit, false);
    }
    if (MaxModFeat == "")
    {
        ShowToolTip(mpParametersPage->mpMaxModFeaLineEdit, tr("Max Model Features must have a value"));
        MarkEditColor(mpParametersPage->mpMaxModFeaLineEdit, false);
    }
    if ((MaxModFeat.toInt(&ok, 10) <= 0) | (MaxModFeat == ""))
        validMaxModFeat = false;
    else
    {
        validMaxModFeat = true;
        mUserParams.maxiter = MaxModFeat.toInt(&ok, 10);
        MarkEditColor(mpParametersPage->mpMaxModFeaLineEdit, true);
    }

    // Validate penter and premove
    if ((penter.toFloat(&ok) > premove.toFloat(&ok)))
    {
        ShowToolTip(mpParametersPage->mpPenterLineEdit, tr("Penter must be less than Premove"));
        MarkEditColor(mpParametersPage->mpPenterLineEdit, false);
        MarkEditColor(mpParametersPage->mpPremoveLineEdit, false);
    }
    if (penter == "")
    {
        ShowToolTip(mpParametersPage->mpPenterLineEdit, tr("Penter must have a value"));
        MarkEditColor(mpParametersPage->mpPenterLineEdit, false);
    }
    if (premove == "")
    {
        ShowToolTip(mpParametersPage->mpPenterLineEdit, tr("Premove must have a value"));
        MarkEditColor(mpParametersPage->mpPremoveLineEdit, false);
    }

    if ((penter.toFloat(&ok) > premove.toFloat(&ok)) | (penter == "") | (premove == ""))
        validPenterPremove = false;
    else
    {
        validPenterPremove = true;
        mUserParams.penter = penter.toFloat(&ok);
        mUserParams.premove = premove.toFloat(&ok);
        MarkEditColor(mpParametersPage->mpPenterLineEdit, true);
        MarkEditColor(mpParametersPage->mpPremoveLineEdit, true);
    }

    // Validate Decimation Frequency
    if (DecFre.toInt(&ok, 10) <= 0)
    {
        ShowToolTip(mpParametersPage->mpDecFreLineEdit, tr("Decimation Frequency must be > 0"));
        MarkEditColor(mpParametersPage->mpDecFreLineEdit, false);
    }
    if (DecFre == "")
    {
        ShowToolTip(mpParametersPage->mpDecFreLineEdit, tr("Decimation Frequency must have a value"));
        MarkEditColor(mpParametersPage->mpDecFreLineEdit, false);
    }

    if ((DecFre.toInt(&ok, 10) <= 0) | (DecFre == ""))
        validDecFre = false;
    else
    {
        validDecFre = true;
        mUserParams.Decimation_Frequency = DecFre.toInt(&ok, 10);
        MarkEditColor(mpParametersPage->mpDecFreLineEdit, true);
    }

    QVector<int> wind;
    ReadVector(RespWind, wind);
    // Validate Response window
    mUserParams.windlen.clear();
    if (wind.size() == 2)
    {
        if (wind[0] > wind[1])
        {
            ShowToolTip(mpParametersPage->mpRespWindLineEdit,
                        tr("[Begin] must be less than [End] in the Response Window"));
            MarkEditColor(mpParametersPage->mpRespWindLineEdit, false);
            validRespWind = false;
        }
        else
        {
            validRespWind = true;
            mUserParams.windlen.push_back(wind[0]);
            mUserParams.windlen.push_back(wind[1]);
            MarkEditColor(mpParametersPage->mpRespWindLineEdit, true);
        }
    }
    else
    {
        validRespWind = false;
        ShowToolTip(mpParametersPage->mpRespWindLineEdit, tr("Response Window must have [Begin] and [End]"));
        MarkEditColor(mpParametersPage->mpRespWindLineEdit, false);
    }

    if (validMaxModFeat && validPenterPremove && validDecFre && validRespWind)
        mpGenerateFeatureWeights->setEnabled(mTrainingDataFilesValid);

    if (mTrainingDataFiles.size() == 0)
        mpGenerateFeatureWeights->setEnabled(false);
}

std::string DataPage::ValidateOnsetExpression(const std::string& expression)
{
    std::vector<std::string> files;
    for (const auto& file : mTrainingDataFiles)
        files.push_back(file);
    for (const auto& file : mTestingDataFiles)
        files.push_back(file);
    for (const auto& file : files)
    {
        ap::template_2d_array<float, true> ignoredSignal;
        prm ignoredPrm;
        ste ignoredSte;
        std::string err = load_bcidat(file, TestExpression, expression, ignoredSignal, ignoredPrm, ignoredSte);
        if (!err.empty())
            return err;
    }
    return "";
}

bool DataPage::ValidateDataFiles(bool training, std::vector<std::string>& fPathArr, int& numSamples, int& numChannels,
    AnalysisMode& mode)
{
    numSamples = 0;
    numChannels = 0;
    fPathArr.clear();

    ///////////////////////////////////////////////////////////////////////////////
    // Section: Define variables
    std::vector<int> channelsInFile;
    std::vector<int> samplesInFile;
    std::vector<int> SamplingRate;
    std::vector<int> NumberOfSequences;
    std::vector<int> NumMatrixRows;
    std::vector<int> NumMatrixColumns;
    std::vector<int> NumStimuli;
    std::vector<int> InterpretMode;
    std::vector<std::string> ApplicationFilterChain;
    std::vector<std::string> Experiment;

    ///////////////////////////////////////////////////////////////////////////////
    // Section: Open BCI2000 File, check if the file is open, and check compatibility
    // among the files
    BCI2000FileReader CurrentFile;
    Experiment.push_back("P3SpellerTask");
    Experiment.push_back("StimulusPresentationTask");

    // Check if the BCI2000 file exists
    fPathArr.clear();
    QListWidgetItem* item = nullptr;//new QListWidgetItem;
    QListWidget* pFilesList = training ? mpTrainingDataFilesList : mpTestingDataFilesList;
    for (int file = 0; file < pFilesList->count(); file++)
    {
        item = pFilesList->item(file);
        item->setForeground(Qt::black);
        fPathArr.push_back(item->text().toLocal8Bit().constData());
        CurrentFile.Open(fPathArr[file].c_str());
        if (!CurrentFile.IsOpen())
        {
            fPathArr[file] = fPathArr[file] + ".dat";
            CurrentFile.Open(fPathArr[file].c_str());
        }
        if (!CurrentFile.IsOpen())
        {
            std::ostringstream oss;
            oss << "Could not open: \n" << fPathArr[file] << "\" as a BCI2000 data file.";
            QMessageBox msgBox(QMessageBox::Warning, tr("Warning"), oss.str().c_str(), QMessageBox::NoButton, this);
            msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
            if (msgBox.exec() == QMessageBox::AcceptRole)
            {
                pFilesList->item(file)->setBackground(QColor("lightpink"));
                return false;
            }
        }
        channelsInFile.push_back(CurrentFile.SignalProperties().Channels());
        samplesInFile.push_back(CurrentFile.NumSamples());

        // Check the Interpret Mode
        if (CurrentFile.Parameters()->Exists("InterpretMode"))
        {
            InterpretMode.push_back(CurrentFile.Parameter("InterpretMode"));
            if (InterpretMode[0] == 0)
            {
                std::ostringstream oss;
                oss << "InterpretMode of data file " << fPathArr[file] << " is not supported";
                QMessageBox msgBox(QMessageBox::Warning, tr("Warning"), oss.str().c_str(), QMessageBox::NoButton, this);
                msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
                if (msgBox.exec() == QMessageBox::AcceptRole)
                {
                    pFilesList->item(file)->setBackground(QColor("lightpink"));
                    return false;
                }
            }
        }
        else
        {
            std::ostringstream oss;
            oss << "Parameter InterpretMode not found in file " << fPathArr[file];
            QMessageBox msgBox(QMessageBox::Warning, tr("Warning"), oss.str().c_str(), QMessageBox::NoButton, this);
            msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
            if (msgBox.exec() == QMessageBox::AcceptRole)
            {
                pFilesList->item(file)->setBackground(QColor("lightpink"));
                return false;
            }
        }

        // ********************************************************************
        // Modification Cristhian, Sep 11, 2009

        if (CurrentFile.States()->Exists("SelectedTarget") && CurrentFile.States()->Exists("SelectedRow") &&
            CurrentFile.States()->Exists("SelectedColumn"))
        {
            ApplicationFilterChain.push_back("P3SpellerTask");
        }
        else if (CurrentFile.States()->Exists("SelectedStimulus"))
        {
            ApplicationFilterChain.push_back("StimulusPresentationTask");
        }
        else
        {
            std::ostringstream oss;
            oss << "The Application type in file " << fPathArr[file] << " is not supported";
            QMessageBox msgBox(QMessageBox::Warning, tr("Warning"), oss.str().c_str(), QMessageBox::NoButton, this);
            msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
            if (msgBox.exec() == QMessageBox::AcceptRole)
            {
                pFilesList->item(file)->setBackground(QColor("lightpink"));
                return false;
            }
        }
        // Check the Sampling Rate
        if (CurrentFile.Parameters()->Exists("SamplingRate"))
            SamplingRate.push_back(CurrentFile.SamplingRate());
        else
        {
            std::ostringstream oss;
            oss << "Parameter SamplingRate does not exist in file " << fPathArr[file];
            QMessageBox msgBox(QMessageBox::Warning, tr("Warning"), oss.str().c_str(), QMessageBox::NoButton, this);
            msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
            if (msgBox.exec() == QMessageBox::AcceptRole)
            {
                pFilesList->item(file)->setBackground(QColor("lightpink"));
                return false;
            }
        }

        if (training)
        {
            // Check the Parameter Number of Sequences - Modifiction Cristhian Potes - January 27, 2013
            if (!CurrentFile.Parameters()->Exists("NumberOfSequences"))
            {
                std::ostringstream oss;
                oss << "Parameter NumberOfSequences does not exist in file " << fPathArr[file];
                QMessageBox msgBox(QMessageBox::Critical, tr("Error"), oss.str().c_str(), QMessageBox::NoButton, this);
                msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
                if (msgBox.exec() == QMessageBox::AcceptRole)
                {
                    mpTrainingDataFilesList->item(file)->setBackground(QColor("lightpink"));
                    return false;
                }
            }
        }

        bool& multipleMenus = training ? mMultipleMenusTrainingData : mMultipleMenusTestingData;
        multipleMenus = false; // make sure it is set correctly for StimulusPresentation files, jm
        // Check the NumMatrixRows. Cristhian Modification Oct 9, 2009
        if (CurrentFile.Parameters()->Exists("NumMatrixRows"))
        {
            if (CurrentFile.Parameter("NumMatrixRows")->NumValues() != 1)
            {
                NumMatrixRows.push_back(0);
                multipleMenus = true;
            }
            else
            {
                NumMatrixRows.push_back(CurrentFile.Parameter("NumMatrixRows"));
                multipleMenus = false;
            }
        }
        else
        {
            if (ApplicationFilterChain[file] == Experiment[0])
            {
                std::ostringstream oss;
                oss << "Parameter NumMatrixRows does not exist in file " << fPathArr[file];
                QMessageBox msgBox(QMessageBox::Critical, tr("Error"), oss.str().c_str(), QMessageBox::NoButton, this);
                msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
                if (msgBox.exec() == QMessageBox::AcceptRole)
                {
                    pFilesList->item(file)->setBackground(QColor("lightpink"));
                    return false;
                }
            }
        }

        // Check the NumMatrixColumns
        if (CurrentFile.Parameters()->Exists("NumMatrixColumns"))
        {
            if (CurrentFile.Parameter("NumMatrixColumns")->NumValues() != 1)
            {
                NumMatrixColumns.push_back(0);
                multipleMenus = true;
            }
            else
            {
                NumMatrixColumns.push_back(CurrentFile.Parameter("NumMatrixColumns"));
                multipleMenus = false;
            }
        }
        else
        {
            if (ApplicationFilterChain[file] == Experiment[0])
            {
                std::ostringstream oss;
                oss << "Parameter NumMatrixColumns does not exist in file " << fPathArr[file];
                QMessageBox msgBox(QMessageBox::Critical, tr("Error"), oss.str().c_str(), QMessageBox::NoButton, this);
                msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
                if (msgBox.exec() == QMessageBox::AcceptRole)
                {
                    pFilesList->item(file)->setBackground(QColor("lightpink"));
                    return false;
                }
            }
        }

        // Check the number of stimulus in the Stimulus Presentation Task
        if (CurrentFile.Parameters()->Exists("Stimuli"))
        {
            ParamRef parameter = CurrentFile.Parameter("Stimuli");
            NumStimuli.push_back(parameter->NumColumns());
        }
        else
        {
            if (ApplicationFilterChain[file] == Experiment[1])
            {
                std::ostringstream oss;
                oss << "Parameter Stimuli does not exist in file " << fPathArr[file];
                QMessageBox msgBox(QMessageBox::Warning, tr("Warning"), oss.str().c_str(), QMessageBox::NoButton, this);
                msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
                if (msgBox.exec() == QMessageBox::AcceptRole)
                {
                    pFilesList->item(file)->setBackground(QColor("lightpink"));
                    return false;
                }
            }
        }

        // Validate stimulus frequencies, jm Mar 21, 2011
        if (ApplicationFilterChain[file] == Experiment[1])
        {
            std::ostringstream errors;
            if (!ValidateStimulusFrequencies(CurrentFile, fPathArr[file], errors))
            {
                QMessageBox msgBox(QMessageBox::Critical, tr("Error"), errors.str().c_str(), QMessageBox::NoButton, this);
                msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
                if (msgBox.exec() == QMessageBox::AcceptRole)
                {
                    pFilesList->item(file)->setBackground(QColor("lightpink"));
                    return false;
                }
            }
        }

        if (file > 0)
        {
            if (channelsInFile[file - 1] != channelsInFile[file])
            {
                std::ostringstream oss;
                oss << "All input files must have the same numbers of Channels";
                QMessageBox msgBox(QMessageBox::Critical, tr("Error"), oss.str().c_str(), QMessageBox::NoButton, this);
                msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
                if (msgBox.exec() == QMessageBox::AcceptRole)
                {
                    pFilesList->item(file - 1)->setBackground(QColor("yellow"));
                    pFilesList->item(file)->setBackground(QColor("yellow"));
                    return false;
                }
            }
            if (SamplingRate[file - 1] != SamplingRate[file])
            {
                std::ostringstream oss;
                oss << "All input files must have the same SamplingRate";
                QMessageBox msgBox(QMessageBox::Critical, tr("Error"), oss.str().c_str(), QMessageBox::NoButton, this);
                msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
                if (msgBox.exec() == QMessageBox::AcceptRole)
                {
                    pFilesList->item(file - 1)->setBackground(QColor("yellow"));
                    pFilesList->item(file)->setBackground(QColor("yellow"));
                    return false;
                }
            }
            if (InterpretMode[file - 1] != InterpretMode[file])
            {
                std::ostringstream oss;
                oss << "All input files must have the same InterpretMode";
                QMessageBox msgBox(QMessageBox::Critical, tr("Error"), oss.str().c_str(), QMessageBox::NoButton, this);
                msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
                if (msgBox.exec() == QMessageBox::AcceptRole)
                {
                    pFilesList->item(file - 1)->setBackground(QColor("yellow"));
                    pFilesList->item(file)->setBackground(QColor("yellow"));
                    return false;
                }
            }
            if (ApplicationFilterChain[file - 1] != ApplicationFilterChain[file])
            {
                std::ostringstream oss;
                oss << "All input files must be recorded using the same experiment";
                QMessageBox msgBox(QMessageBox::Critical, tr("Error"), oss.str().c_str(), QMessageBox::NoButton, this);
                msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
                if (msgBox.exec() == QMessageBox::AcceptRole)
                {
                    pFilesList->item(file - 1)->setBackground(QColor("yellow"));
                    pFilesList->item(file)->setBackground(QColor("yellow"));
                    return false;
                }
            }
            if (ApplicationFilterChain[file] == Experiment[0])
                if (NumMatrixRows[file - 1] != NumMatrixRows[file])
                {
                    std::ostringstream oss;
                    oss << "All input files must be recorded using the same experiment";
                    QMessageBox msgBox(QMessageBox::Critical, tr("Error"), oss.str().c_str(), QMessageBox::NoButton, this);
                    msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
                    if (msgBox.exec() == QMessageBox::AcceptRole)
                    {
                        pFilesList->item(file - 1)->setBackground(QColor("yellow"));
                        pFilesList->item(file)->setBackground(QColor("yellow"));
                        return false;
                    }
                }
            if (ApplicationFilterChain[file] == Experiment[0])
                if (NumMatrixColumns[file - 1] != NumMatrixColumns[file])
                {
                    std::ostringstream oss;
                    oss << "All input files must be recorded using the same experiment";
                    QMessageBox msgBox(QMessageBox::Critical, tr("Error"), oss.str().c_str(), QMessageBox::NoButton, this);
                    msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
                    if (msgBox.exec() == QMessageBox::AcceptRole)
                    {
                        pFilesList->item(file - 1)->setBackground(QColor("yellow"));
                        pFilesList->item(file)->setBackground(QColor("yellow"));
                        return false;
                    }
                }
            if (ApplicationFilterChain[file] == Experiment[1])
                if (NumStimuli[file - 1] != NumStimuli[file])
                {
                    std::ostringstream oss;
                    oss << "All input files must be recorded using the same experiment";
                    QMessageBox msgBox(QMessageBox::Critical, tr("Error"), oss.str().c_str(), QMessageBox::NoButton, this);
                    msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
                    if (msgBox.exec() == QMessageBox::AcceptRole)
                    {
                        pFilesList->item(file - 1)->setBackground(QColor("yellow"));
                        pFilesList->item(file)->setBackground(QColor("yellow"));
                        return false;
                    }
                }
        }
        numSamples += samplesInFile[file];
        pFilesList->item(file)->setBackground(QColor("lightgreen"));
    }

    numChannels = channelsInFile[0];
    if (training)
    {
        // Check that the channel set provided is a subset of the channels contained in the input files
        QVector<int> ChannelSet;
        QString Ch = mpParametersPage->mpChSetLineEdit->text();
        ReadVector(Ch, ChannelSet);

        for (int i = 0; i < ChannelSet.size(); i++)
        {
            if (ChannelSet[i] > numChannels)
            {
                std::ostringstream oss;
                oss << "The Channel Set provided is not a subset of the channels contained in the input file(s).";
                QMessageBox msgBox(QMessageBox::Warning, tr("Warning"), oss.str().c_str(), QMessageBox::NoButton, this);
                msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
                QPalette palette(mpParametersPage->mpChSetLineEdit->palette());

                if (msgBox.exec() == QMessageBox::AcceptRole)
                {
                    palette.setColor(QPalette::Base, QColor("lightpink"));
                    mpParametersPage->mpChSetLineEdit->setPalette(palette);
                    return false;
                }
            }
        }
        QPalette palette(mpParametersPage->mpChSetLineEdit->palette());
        palette.setColor(QPalette::Base, QColor("lightgreen"));
        mpParametersPage->mpChSetLineEdit->setPalette(palette);
    }

    if ((Experiment[0] == ApplicationFilterChain[0]) && InterpretMode[0] == 1)
        mode = P3SpellerFree;
    if ((Experiment[0] == ApplicationFilterChain[0]) && InterpretMode[0] == 2)
        mode = P3SpellerCopy;
    if ((Experiment[1] == ApplicationFilterChain[0]) && InterpretMode[0] == 1)
        mode = StimulusPresentationFree;
    if ((Experiment[1] == ApplicationFilterChain[0]) && InterpretMode[0] == 2)
        mode = StimulusPresentationCopy;

    if (training)
    {
        // Fill the Detail Page with the BCI2000 FileInfo
        spDetailsPage->mpSamplingRateLineEdit->setText(QString::number(SamplingRate[0]));
        spDetailsPage->mpSourceChListLineEdit->setText(QString::number(channelsInFile[0]));
        spDetailsPage->mpClassifierLineEdit->setText(tr("Stepwise Linear Regression"));

        if (ApplicationFilterChain[0] == Experiment[0])
            spDetailsPage->mpApplicationFilterChainLineEdit->setText(Experiment[0].c_str());
        else if (ApplicationFilterChain[0] == Experiment[1])
            spDetailsPage->mpApplicationFilterChainLineEdit->setText(Experiment[1].c_str());

        if (InterpretMode[0] == 1)
            spDetailsPage->mpInterpretModeLineEdit->setText(tr("Online Free Mode"));
        else if (InterpretMode[0] == 2)
            spDetailsPage->mpInterpretModeLineEdit->setText(tr("Copy Mode"));

        spDetailsPage->mpTotalNumberofSamplesLineEdit->setText(
            QString::number(1.0 * numSamples / SamplingRate[0], 'f', 2));
    }
    return true;
}


void DataPage::GenerateFeatures()
{
    if (mMultipleMenusTrainingData)
    {
        std::ostringstream oss;
        oss << "This program does not support nested matrices. Therefore, it won't display the classification "
               "accuracies in the details panel.";
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"), oss.str().c_str(), QMessageBox::NoButton, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        msgBox.exec();
    }

    spDetailsPage->mpProgressBarOverall->setValue(0);
    spDataPage->mpProgressBarOverall->setValue(0);
    mpGenerateFeatureWeights->setEnabled(false);
    mGenerateFeatureWeightsThread.start();

    if (!mFeatureWeightsGenerated)
    {
        spDetailsPage->mpProgressBarOverall->setValue(0);
        spDataPage->mpProgressBarOverall->setValue(0);
    }
}

void DataPage::ApplyFeatures()
{
    if (mMultipleMenusTestingData)
    {
        std::ostringstream oss;
        oss << "This program does not support nested matrices. Therefore, it won't display the classification "
               "accuracies in the details panel.";
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"), oss.str().c_str(), QMessageBox::NoButton, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::AcceptRole);
        if (msgBox.exec() == QMessageBox::AcceptRole)
        {
        }
    }
    spDataPage->mpProgressBarOverall->setValue(0);
    spDetailsPage->mpProgressBarOverall->setValue(0);
    mpApplyFeatureWeights->setEnabled(false);
    mApplyFeatureWeightsThread.start();
}

void DataPage::slotProgressBar(int done, int total, int stage)
{
    if (stage == 1 || stage == 2)
    {
        spDetailsPage->mpProgressBarOverall->setValue(30 * done / total);
        spDataPage->mpProgressBarOverall->setValue(30 * done / total);
    }
    else
    {
        spDetailsPage->mpProgressBarOverall->setValue(100 * done / total);
        spDataPage->mpProgressBarOverall->setValue(100 * done / total);
    }

    if (stage == 7)
    {
        mpGenerateFeatureWeights->setEnabled(true);
        mpFileWritePRMButton->setEnabled(true);
        mWeightsValid = true;

        if ((static_cast<int>(mTestingDataFiles.size()) > 0) & (mTestingDataFilesValid))
            mpApplyFeatureWeights->setEnabled(true);
    }
}

void DataPage::slotProgressText(QString message)
{
    spDetailsPage->mpProgressTextEdit->append(message);
}

// Work on this
void DataPage::slotErrorMessageDialog(QString errorMessage)
{
    spErrorMessageDialog->showMessage(errorMessage);
    spErrorMessageDialog->exec();
    spDetailsPage->mpProgressBarOverall->setValue(0);
    spDataPage->mpProgressBarOverall->setValue(0);
}

bool DataPage::WriteParameterFragment(bool showDialog)
{
    QString fileName = mpWritePRMLineEdit->text();
    if (showDialog)
        fileName = QFileDialog::getSaveFileName(this, tr("Write *.prm File"), fileName, tr("Parameter Files (*.prm)"));
    if (!fileName.isEmpty())
    {
        mpWritePRMLineEdit->setText(fileName);
        // Write PRM
        if (!WritePrm(fileName.toLocal8Bit().constData(), mClassParams, mUserParams.SF))
        {
            QString msg = "Could not write to file: " + fileName;
            QMetaObject::invokeMethod(this, "slotErrorMessageDialog", Q_ARG(QString, msg));
            return false;
        }
    }
    return true;
}

void DataPage::OnFeatureWeightsDone()
{
    // Write the suggested name for the *.prm file
    if (mpWritePRMLineEdit->text().length() == 0)
    {
        std::string filedata = mTrainingDataFiles[0];
        size_t found = filedata.find_last_of("/\\");
        std::string fileSug =
            (found == std::string::npos ? std::string() : filedata.substr(0, found + 1)) + "P3Classifier_ChS1_";
        if (mUserParams.SF == UserParameters::Raw)
            fileSug += "RAW_";
        else if (mUserParams.SF == UserParameters::CAR)
            fileSug += "CAR_";
        fileSug += "SW.prm";

        QString text = QString::fromUtf8(fileSug.c_str());
        mpWritePRMLineEdit->setText(text);
    }
    mFeatureWeightsGenerated = true;
    if (mAutoWrite)
    {
        if (WriteParameterFragment(false))
        {
            QWidget* topLevelWidget = nullptr;
            for (QWidget* pWidget = spDataPage; pWidget; pWidget = pWidget->parentWidget())
                topLevelWidget = pWidget;
            topLevelWidget->close();
        }
    }
}

bool DataPage::ValidateStimulusFrequencies(BCI2000FileReader &ioFile, const std::string &inFileName,
                                           std::ostream &ioErrors)
{
    bool errorOccurred = false;
    const char *parameters[] = {
        "Stimuli",
        "SequenceType",
        "Sequence",
    };
    for (int i = 0; i < sizeof(parameters) / sizeof(*parameters); ++i)
        if (!ioFile.Parameters()->Exists(parameters[i]))
        {
            ioErrors << "Parameter \"" << parameters[i] << "\" does not exist in file " << inFileName << ".\n";
            errorOccurred = true;
        }
    if (errorOccurred)
        return false;

    int numStimuli = ioFile.Parameter("Stimuli")->NumColumns();
    std::vector<int> stimulusFrequencies(numStimuli, 0);
    ParamRef Sequence = ioFile.Parameter("Sequence");
    if (ioFile.Parameter("SequenceType") == 0) // deterministic sequence
    {
        for (int i = 0; i < Sequence->NumValues(); ++i)
        {
            if (Sequence(i) >= 1 && Sequence(i) <= numStimuli)
            {
                ++stimulusFrequencies[static_cast<unsigned int>(Sequence(i)) - 1];
            }
            else
            {
                ioErrors << "Invalid stimulus code in the \"Sequence\" parameter of file " << inFileName << ".\n";
                return false;
            }
        }
    }
    else if (ioFile.Parameter("SequenceType") == 1) // random sequence
    {
        for (int i = 0; i < std::min(numStimuli, Sequence->NumValues()); ++i)
            stimulusFrequencies[i] = Sequence(i);
    }
    else if (ioFile.Parameter("SequenceType") == 2) // P3Speller compatible
    {
        for (int i = 0; i < numStimuli; ++i)
            stimulusFrequencies[i] = 1;
    }
    else // unknown sequence type
    {
        ioErrors << "Unknown sequence type in file " << inFileName << ".\n";
        return false;
    }

    // Error if stimulus frequencies are not 1
    for (int i = 0; i < numStimuli; ++i)
        if (stimulusFrequencies[i] != 1)
        {
            ioErrors << "All stimuli defined in the \"Stimuli\" parameter "
                     << "must appear exactly once per sequence "
                     << "(check the \"Sequence\" parameter in file " << inFileName << ").\n";
            return false;
        }

    return true;
}

// Worker thread

WeightsWorkerThread::WeightsWorkerThread(bool generate)
    : mGenerate(generate)
{
}

void WeightsWorkerThread::run()
{
    GenerateOrApplyFeatureWeights(mGenerate);
}

void WeightsWorkerThread::GenerateOrApplyFeatureWeights(bool generate)
{
    ap::template_2d_array<float, true> signal;
    ap::template_2d_array<float, true> signal_tmp;
    ap::template_2d_array<float, true> signal_all_files;
    ap::template_1d_array<double, true> windowlen;
    ap::template_1d_array<double, true> chset;
    ap::real_2d_array pscore;
    std::vector<float> signal_vector;
    std::vector<short int> trialnr;
    std::vector<unsigned short int> Type;
    std::vector<unsigned short int> Code;
    std::vector<unsigned char> StimulusOnset;
    prm parms;
    ste state;
    ste state_tmp;

    std::ostringstream oss;

    clock_t clockBegin = clock(); // start timer

    windowlen.setbounds(0, static_cast<int>(spDataPage->mUserParams.windlen.size()) - 1);
    chset.setbounds(0, static_cast<int>(spDataPage->mUserParams.channel_set.size()) - 1);

    for (int i = 0; i < static_cast<int>(spDataPage->mUserParams.channel_set.size()); i++)
        chset(i) = spDataPage->mUserParams.channel_set[i];

    int row_chset = chset.gethighbound(1) + 1;

    std::string op = generate ? "Generating" : "Applying";
    oss << "***** "
        << op << " Feature Weights "
        << "*****" << std::endl;

    if (spDataPage->mUserParams.SF == UserParameters::Raw)
        oss << "        (Spatial Filter = Raw)" << std::endl;
    else if (spDataPage->mUserParams.SF == UserParameters::CAR)
        oss << "        (Spatial Filter = CAR)" << std::endl;
    emit signalProgressText(oss.str().c_str());
    oss.str("");

    const auto& files = (generate ? spDataPage->mTrainingDataFiles : spDataPage->mTestingDataFiles);

    int TotalFiles = static_cast<int>(files.size());
    int numSequences = INT_MAX, maxValue = 0;
    int DF = 1, MA = 1;
    for (int file = 0; file < TotalFiles; file++)
    {
        oss << "Loading file "
            << "[" << file + 1 << "/" << TotalFiles << "]"
            << "...";
        emit signalProgressText(oss.str().c_str());
        oss.str("");
        // Load signal, parameters, and states from the BCI2000 data file
        auto& mode = generate ? spDataPage->mModeTrainingData : spDataPage->mModeTestingData;
        std::string expr = spParametersPage->mpStimulusOnsetExpressionLineEdit->text().toUtf8().constData();
        std::string err = load_bcidat(files[file], mode, expr, signal, parms, state_tmp);

        if (!err.empty())
        {
            err = "An error occurred while reading \"" + files[file] + "\": " + err;
            emit errorMessage(QString::fromLocal8Bit(err.c_str()));
            spDataPage->mFeatureWeightsGenerated = false;
            return;
        }

        emit signalProgressBar(file + 1, TotalFiles, 1);

        // Take the minimum number of sequences that a file has
        if (parms.NumberOfSequences < numSequences)
            numSequences = parms.NumberOfSequences;

        windowlen(0) = ap::round(spDataPage->mUserParams.windlen[0] * parms.SamplingRate / 1000);
        windowlen(1) = ap::round(spDataPage->mUserParams.windlen[1] * parms.SamplingRate / 1000);
        DF = ap::iceil((double)parms.SamplingRate / (spDataPage->mUserParams.Decimation_Frequency + 0.000001));
        MA = DF;

        if (spDataPage->mUserParams.SF == UserParameters::CAR)
            // Apply common average reference to the channels
            CARfilter(signal);

        int row_signal = signal.gethighbound(1) + 1;

        // Extract from signal only the channels specified in the initial parameter channel set
        signal_tmp.setbounds(0, row_signal - 1, 0, row_chset - 1);
        for (int i = 0; i < row_chset; i++)
            ap::vmove(signal_tmp.getcolumn(i, 0, row_signal - 1), signal.getcolumn(chset(i) - 1, 0, row_signal - 1));

        oss << "Collecting Responses from file "
            << "[" << file + 1 << "/" << TotalFiles << "]"
            << "...\n";
        oss << "Filtering and Decimating file "
            << "[" << file + 1 << "/" << TotalFiles << "]"
            << "...\n";
        emit signalProgressText(oss.str().c_str());
        oss.str("");

        // The signal is filtered and decimated before the SWLDA is applied */
        GetP3Responses(signal_tmp, state_tmp.trialnr, windowlen, state_tmp.StimulusCode, state_tmp.StimulusType,
            state_tmp.StimulusOnset, chset, MA, DF);
        emit signalProgressBar(file + 1, TotalFiles, 2);

        // Concatenate signal and states
        // Cristhian modification, Sep 11, 2009

        for (int i = 0; i < signal_tmp.gethighbound(1) + 1; i++)
        {
            for (int j = 0; j < signal_tmp.gethighbound(0) + 1; j++)
                signal_vector.push_back(signal_tmp(i, j));

            Code.push_back(state_tmp.StimulusCode(i));
            Type.push_back(state_tmp.StimulusType(i));
            StimulusOnset.push_back(state_tmp.StimulusOnset(i));
            trialnr.push_back(state_tmp.trialnr(i) + maxValue);
        }
        if (!trialnr.empty())
            maxValue = *max_element(trialnr.begin(), trialnr.end());
    }
    parms.NumberOfSequences = numSequences;
    signal_all_files.setbounds(0, trialnr.size() - 1, 0, signal_tmp.gethighbound(0));
    state.trialnr.setbounds(0, trialnr.size() - 1);
    state.StimulusCode.setbounds(0, trialnr.size() - 1);
    state.StimulusType.setbounds(0, trialnr.size() - 1);
    state.StimulusOnset.setbounds(0, trialnr.size() - 1);
    state.TargetDefinitions = state_tmp.TargetDefinitions;

    int Target = 0;
    int nonTarget = 0;
    int l = 0;
    for (int i = 0; i < trialnr.size(); i++)
    {
        for (int j = 0; j < signal_tmp.gethighbound(0) + 1; j++)
        {
            signal_all_files(i, j) = signal_vector[l];
            l++;
        }
        state.trialnr(i) = trialnr[i];
        state.StimulusCode(i) = Code[i];
        state.StimulusType(i) = Type[i];
        state.StimulusOnset(i) = StimulusOnset[i];

        if (Type[i] == 0)
            nonTarget++;

        if (Type[i] == 1)
            Target++;
    }

    oss << "Targets: " << Target << std::endl;
    oss << "Non Targets: " << nonTarget << std::endl;
    emit signalProgressText(oss.str().c_str());
    oss.str("");

    if (generate)
    {
        oss << std::endl << "Applying Stepwisefit classifier...\n";
        emit signalProgressText(oss.str().c_str());
        oss.str("");
        // Check that the signal has more rows than columns (overdetermined system)
        if (signal_all_files.gethighbound(1) < signal_all_files.gethighbound(0))
        {
            oss << "Signal must have rows > cols. Linear classifier is valid only for overdetermined systems." << std::endl;
            emit errorMessage(oss.str().c_str());
            spDataPage->mFeatureWeightsGenerated = false;
            return;
        }

        // Perform the Stepwise Linear Discriminant Analysis
        if (!SWLDA(signal_all_files, state.StimulusType, state.trialnr, windowlen, chset, MA, DF, spDataPage->mUserParams.SF,
            parms.SamplingRate, spDataPage->mUserParams.penter, spDataPage->mUserParams.premove,
            spDataPage->mUserParams.maxiter, parms.SoftwareCh, spDataPage->mClassParams, &callback_status))
        {
            oss << "Unable to generate usable weights, try different parameters or dataset." << std::endl;
            emit errorMessage(oss.str().c_str());
            spDataPage->mFeatureWeightsGenerated = false;
            return;
        }
        emit signalProgressBar(1, 5, 3);
    }
    // jm Oct 24, 2012
    ap::template_1d_array<unsigned short int, true> stimCode_GetScore = state.StimulusCode;
    int NumberOfChoices = FixupStimulusCodes(stimCode_GetScore);

    bool ok = GetScore(signal_all_files, stimCode_GetScore, spDataPage->mClassParams, state.trialnr, windowlen, row_chset,
        parms.NumberOfSequences, NumberOfChoices, spDataPage->mModeTrainingData, pscore);
    if (generate && !ok)
    {
        // Cristhian modification April 17, 2010
        oss << "Dataset is corrupted. Try another dataset." << std::endl;
        emit errorMessage(oss.str().c_str());
        spDataPage->mFeatureWeightsGenerated = false;
        return;
    }

    emit signalProgressBar(2, 5, 4);

    bool multiple = generate ? spDataPage->mMultipleMenusTrainingData : spDataPage->mMultipleMenusTestingData;
    if (!multiple)
    {
        std::vector<std::string> predicted;
        std::vector<double> vresult;

        if (spDataPage->mModeTrainingData & P3Speller)
            P300_Classify(pscore, state.StimulusCode, state.StimulusType, state.trialnr, parms.NumberOfSequences,
                parms.NumMatrixRows, parms.NumMatrixColumns, state.TargetDefinitions, vresult, predicted);

        else if (spDataPage->mModeTrainingData & StimulusPresentation)
            StimulusPresentation_Classify(pscore, state.StimulusCode, state.StimulusType, state.trialnr,
                parms.NumberOfSequences, NumberOfChoices, state.TargetDefinitions, vresult,
                predicted);
        emit signalProgressBar(3, 5, 5);
        signal_vector.clear();
        trialnr.clear();
        Code.clear();
        Type.clear();
        StimulusOnset.clear();
        state.TargetDefinitions.clear();
        state_tmp.TargetDefinitions.clear();

        PrintClassificationResults(oss, predicted, vresult);
    }
    oss << "Done!\n\n";
    oss << std::setprecision(10);
    oss << "Time elapsed [s]: " << 1.0 * (clock() - clockBegin) / CLOCKS_PER_SEC << std::endl << std::endl;

    // Cristhian Modification, May 04, 2010
    // Modification to display classification results in unicode
    QString unicodeString = QString::fromLocal8Bit(oss.str().c_str());
    emit signalProgressText(unicodeString);
    oss.str("");
    emit signalProgressBar(5, 5, 7);

    if (generate)
        emit done();
}

void WeightsWorkerThread::ProgressTextCallBack(QString message)
{
    emit signalProgressText(message);
}
