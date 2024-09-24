#ifndef PAGES_H
#define PAGES_H

#include <QTextStream>
#include <QThread>
#include <QtWidgets>
#include <string>
#include <vector>

#include "BCI2000FileReader.h"
#include "BCIStream.h"
#include "ReadIniParameters.h"

#include "CARfilter.h"
#include "CDataFile.h"
#include "GetP3Responses.h"
#include "GetScore.h"
#include "P300_Classify.h"
#include "SWLDA.h"
#include "StimulusPresentation_Classify.h"
#include "ap.h"
#include "load_bcidat.h"
#include "WritePrm.h"

class WeightsWorkerThread : public QThread
{
    Q_OBJECT

  public:
    explicit WeightsWorkerThread(bool generate);
    void run() override;
    void GenerateOrApplyFeatureWeights(bool generate);
    void ProgressTextCallBack(QString message);

  signals:
    void signalProgressBar(int done, int total, int stage);
    void signalProgressText(QString message);
    void errorMessage(QString errorMessage);
    void done();

  private:
    bool mGenerate;
};

class ParametersPage;

class DataPage : public QWidget
{
    Q_OBJECT

  public:
    DataPage(QWidget *parent = 0);

    QLineEdit *mpInicfgFileLineEdit;
    QLineEdit *mpWritePRMLineEdit;

    QPushButton *mpLoadTrainingDataFilesButton;
    QPushButton *mpLoadTestingDataFilesButton;
    QPushButton *mpGenerateFeatureWeights;
    QPushButton *mpApplyFeatureWeights;
    QPushButton *mpLoadIniFileButton;
    QPushButton *mpFileWritePRMButton;

    QProgressBar *mpProgressBarOverall;

    QListWidget *mpTrainingDataFilesList;
    QListWidget *mpTestingDataFilesList;

    WeightsWorkerThread mGenerateFeatureWeightsThread,
                        mApplyFeatureWeightsThread;

    QString mPathTrainingData;
    QString mPathTestingData;
    QString mPathIniData;

    std::vector<std::string> mTrainingDataFiles;
    std::vector<std::string> mTestingDataFiles;
    int mNumSamplesTrainingData = 0;
    int mNumChannelsTrainingData = 0;
    AnalysisMode mModeTrainingData = TestExpression;
    int mNumSamplesTestingData = 0;
    int mNumChannelsTestingData = 0;
    AnalysisMode mModeTestingData = TestExpression;
    bool mWeightsValid = false;
    bool mTrainingDataFilesValid = false;
    bool mTestingDataFilesValid = false;
    bool mFeatureWeightsGenerated = false;
    bool mMultipleMenusTrainingData = false;
    bool mMultipleMenusTestingData = false;
    bool mAutoWrite = false;

    // structures
    UserParameters mUserParams;
    ClassificationParameters mClassParams;

    void setParametersPage(ParametersPage *);

  private:
    ParametersPage *mpParametersPage;

public slots:
    void slotProgressText(QString message);
    void slotProgressBar(int done, int total, int stage);
    void slotErrorMessageDialog(QString errorMessage);
    void LoadTrainingButton();
    void LoadTestingButton();
    void LoadIniButton();
    void GenerateFeatures();
    void ApplyFeatures();
    bool WriteParameterFragment(bool showDialog = true);
    void OnFeatureWeightsDone();

public:
    void LoadSettings();
    void ValidateSettings();
    std::string ValidateOnsetExpression(const std::string& expression);

    void SetTrainingFiles(const QStringList&);
    void SetTestingFiles(const QStringList&);
    void SetIniFile(const QString&);
    void SetClassifierFile(const QString&);

    bool ValidateDataFiles(bool training, std::vector<std::string>& fPathArr, int& numSamples, int& numChannels, AnalysisMode& mode);

private:
    bool ValidateStimulusFrequencies(BCI2000FileReader&, const std::string& inFileName, std::ostream& ioErrors);
};

class ParametersPage : public QWidget
{
    Q_OBJECT

  public:
    ParametersPage(QWidget *parent = 0);

    QLineEdit *mpStimulusOnsetExpressionLineEdit;
    QLineEdit *mpRespWindLineEdit;
    QLineEdit *mpDecFreLineEdit;
    QLineEdit *mpMaxModFeaLineEdit;
    QLineEdit *mpChSetLineEdit;
    QLineEdit *mpPenterLineEdit;
    QLineEdit *mpPremoveLineEdit;

    QComboBox* mpSpatFilterComboBox;

    void setDataPage(DataPage *);

  public slots:
    void TextHasChanged();
    void ChSetTextHasChanged();
    void OnsetExpressionHasChanged();

  private:
    DataPage *mpDataPage;
};

class DetailsPage : public QWidget
{
    Q_OBJECT

  public:
    DetailsPage(QWidget *parent = 0);
    void Clear();

    QLineEdit *mpSamplingRateLineEdit;
    QLineEdit *mpSourceChListLineEdit;
    QLineEdit *mpClassifierLineEdit;
    QLineEdit *mpApplicationFilterChainLineEdit;
    QLineEdit *mpInterpretModeLineEdit;
    QLineEdit *mpTotalNumberofSamplesLineEdit;

    QTextEdit *mpProgressTextEdit;

    QProgressBar *mpProgressBarOverall;
};

#endif // PAGES_H
