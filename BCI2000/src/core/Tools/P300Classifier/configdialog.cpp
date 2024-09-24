#include "configdialog.h"
#include "pages.h"
#include <QtGui>

ConfigDialog::ConfigDialog()
{
    // Define pointer for all the pages
    mpDataPage = new DataPage;
    mpParametersPage = new ParametersPage;
    mpDetailsPage = new DetailsPage;

    // Pass pointers between pages to share information
    mpParametersPage->setDataPage(mpDataPage);
    mpDataPage->setParametersPage(mpParametersPage);

    mpContentsWidget = new QListWidget;
    mpContentsWidget->setViewMode(QListView::IconMode);
    mpContentsWidget->setIconSize(QSize(96, 84));
    mpContentsWidget->setMovement(QListView::Static);
    mpContentsWidget->setMaximumWidth(128);
    mpContentsWidget->setSpacing(12);

    mpPagesWidget = new QStackedWidget;
    mpPagesWidget->addWidget(mpDataPage);
    mpPagesWidget->addWidget(mpParametersPage);
    mpPagesWidget->addWidget(mpDetailsPage);

    mpPagesWidget->setMinimumWidth(400);

    QPushButton *closeButton = new QPushButton(tr("Close"));

    createIcons();
    mpContentsWidget->setCurrentRow(0);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(mpContentsWidget);
    horizontalLayout->addWidget(mpPagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("P300 Classifier GUI"));
}

void ConfigDialog::createIcons()
{
    QListWidgetItem *configButton = new QListWidgetItem(mpContentsWidget);
    configButton->setIcon(QIcon(":/images/config.png"));
    configButton->setText(tr("Data"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *updateButton = new QListWidgetItem(mpContentsWidget);
    updateButton->setIcon(QIcon(":/images/update.png"));
    updateButton->setText(tr("Parameters"));
    updateButton->setTextAlignment(Qt::AlignHCenter);
    updateButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *queryButton = new QListWidgetItem(mpContentsWidget);
    queryButton->setIcon(QIcon(":/images/query.png"));
    queryButton->setText(tr("Details"));
    queryButton->setTextAlignment(Qt::AlignHCenter);
    queryButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(mpContentsWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this,
            SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}

void ConfigDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    mpPagesWidget->setCurrentIndex(mpContentsWidget->row(current));
}

void ConfigDialog::SetFiles(const QStringList &trainingDataFiles, const QStringList &testingDataFiles, 
                            const QString &inicfgFile, const QString &classifierOutputFile)
{
    mpDataPage->SetTrainingFiles(trainingDataFiles);
    mpDataPage->SetTestingFiles(testingDataFiles);
    mpDataPage->SetIniFile(inicfgFile);

    mpDataPage->mpWritePRMLineEdit->setText(classifierOutputFile);

    // Modification Cristhian Potes
    if (mpDataPage->mTrainingDataFilesValid && trainingDataFiles.count() > 0)
    {
        mpDataPage->mAutoWrite = (classifierOutputFile.length() > 0);
        mpDataPage->mGenerateFeatureWeightsThread.start();
    }
}
