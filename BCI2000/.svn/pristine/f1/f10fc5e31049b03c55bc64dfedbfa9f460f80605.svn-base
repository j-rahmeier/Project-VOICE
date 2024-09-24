//////////////////////////////////////////////////////////////////////
// $Id$
// Authors: mellinger@neurotechcenter.org
// Description: The main window of BCI2000Watchdog.
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
#include "MainWindow.h"

#include "QtMain.h"
#include "LogWidget.h"
#include "AboutBox.h"
#include "ExecutableHelp.h"
#include "Settings.h"
#include "PhysicalUnit.h"
#include "ProcessUtils.h"

#include <QFileDialog>
#include <QtWidgets>

#include <fstream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    bool autoStart = false;
    QString batchFile;

    QStringList args = QApplication::arguments();
    if (!args.isEmpty())
        args.pop_front();
    for (const auto& arg : args) {
        if (arg.startsWith("--IniFile=", Qt::CaseInsensitive))
            mIniFile = arg.split('=').back();
        else if(arg.startsWith("--UILevel=", Qt::CaseInsensitive))
            mUiLevel = arg.split('=').back().toInt();
        else if(arg.compare("--Autostart", Qt::CaseInsensitive) == 0)
            autoStart = true;
        else
            batchFile = arg;
    }

    QVBoxLayout* pMainLayout = new QVBoxLayout;

    QGroupBox* pGroup = new QGroupBox("Settings");
    QBoxLayout* pSubLayout = new QVBoxLayout;
    QBoxLayout* pSubSubLayout = new QHBoxLayout;

    mpScriptFile = new QLineEdit;
    mpScriptFile->setReadOnly(false);
    mpScriptFile->setPlaceholderText("Choose or drop a BCI2000 batch file");
    connect(mpScriptFile, &QLineEdit::textChanged, this, &MainWindow::OnScriptFileChanged);
    pSubSubLayout->addWidget(mpScriptFile);

    QPushButton* pButton = new QPushButton("...");
    connect(pButton, &QPushButton::pressed, this, &MainWindow::OnActionOpen);
    pSubSubLayout->addWidget(pButton);

    mpEditButton = new QPushButton("Edit...");
    connect(mpEditButton, &QPushButton::pressed, this, &MainWindow::OnEditScriptFile);
    pSubSubLayout->addWidget(mpEditButton);

    pSubLayout->addLayout(pSubSubLayout);
    pSubSubLayout = new QHBoxLayout;

    QLabel* pLabel = new QLabel("Timeout for reaching \"Running\" state:");
    pSubSubLayout->addWidget(pLabel);

    mpRunningTimeout = new QLineEdit;
    mpRunningTimeout->setMaximumWidth(mpRunningTimeout->fontMetrics().horizontalAdvance("0000s"));
    pSubSubLayout->addWidget(mpRunningTimeout);
    pSubSubLayout->addStretch();
    pSubLayout->addLayout(pSubSubLayout);

    pSubSubLayout = new QHBoxLayout;
    pLabel = new QLabel("Timeout for terminating processes at shutdown:");
    pSubSubLayout->addWidget(pLabel);

    mpProcessesTimeout = new QLineEdit;
    mpProcessesTimeout->setMaximumWidth(mpProcessesTimeout->fontMetrics().horizontalAdvance("0000s"));
    pSubSubLayout->addWidget(mpProcessesTimeout);
    pSubSubLayout->addStretch();

    pSubLayout->addLayout(pSubSubLayout);
    pGroup->setLayout(pSubLayout);
    pMainLayout->addWidget(pGroup);

    if (mUiLevel < 3)
        pGroup->hide();

    pGroup = new QGroupBox("Canary Expressions (Watches)");
    pSubLayout = new QVBoxLayout;
    pSubSubLayout = new QHBoxLayout;

    mpWatches = new QTableWidget(0, 3);
    mpWatches->setHorizontalHeaderItem(0, new QTableWidgetItem("Expression"));
    mpWatches->setHorizontalHeaderItem(1, new QTableWidgetItem("Initial timeout"));
    mpWatches->setHorizontalHeaderItem(2, new QTableWidgetItem("Timeout"));

    auto pHeader = mpWatches->verticalHeader();
    pHeader->setSectionResizeMode(QHeaderView::Fixed);
    pHeader->setDefaultSectionSize(mpWatches->fontMetrics().height() * 1.2);

    pHeader = mpWatches->horizontalHeader();
    pHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
    pHeader->setStretchLastSection(true);

    mpWatches->setFixedHeight(mpWatches->fontMetrics().height() * 6);

    pSubLayout->addWidget(mpWatches);
    mpPlusButton = new QPushButton("+");
    connect(mpPlusButton, &QPushButton::pressed, this, &MainWindow::OnPlusButton);
    pSubSubLayout->addWidget(mpPlusButton);
    mpMinusButton = new QPushButton("-");
    connect(mpMinusButton, &QPushButton::pressed, this, &MainWindow::OnMinusButton);
    pSubSubLayout->addWidget(mpMinusButton);
    pSubSubLayout->addStretch();
    pSubLayout->addLayout(pSubSubLayout);

    pGroup->setLayout(pSubLayout);

    pMainLayout->addWidget(pGroup);
    if (mUiLevel < 3)
        pGroup->hide();

    pGroup = new QGroupBox("Log");
    pSubLayout = new QHBoxLayout;
    mpLog = new LogWidget;
    pSubLayout->addWidget(mpLog);
    pGroup->setLayout(pSubLayout);
    pMainLayout->addWidget(pGroup, 1);
    if (mUiLevel < 2)
        pGroup->hide();

    pSubLayout = new QHBoxLayout;

    mpResetButton = new QPushButton("Reset");
    mpResetButton->setEnabled(false);
    connect(mpResetButton, &QPushButton::clicked, this, &MainWindow::OnReset);
    pSubLayout->addWidget(mpResetButton);
    if (mUiLevel < 2)
        mpResetButton->hide();

    pSubLayout->addStretch();

    mpTerminateButton = new QPushButton("Terminate BCI2000");
    mpTerminateButton->setEnabled(false);
    connect(mpTerminateButton, &QPushButton::clicked, this, &MainWindow::OnTerminate);
    pSubLayout->addWidget(mpTerminateButton);

    mpStartButton = new QPushButton("Start BCI2000");
    mpStartButton->setEnabled(false);
    connect(mpStartButton, &QPushButton::clicked, this, &MainWindow::OnStart);
    pSubLayout->addWidget(mpStartButton);

    pMainLayout->addLayout(pSubLayout);

    setCentralWidget(new QWidget);
    centralWidget()->setLayout(pMainLayout);

    BuildFileMenu();
    BuildEditMenu();
    BuildActionsMenu();
    BuildHelpMenu();

    if (mUiLevel < 3) {
        menuBar()->hide();
        menuBar()->setDisabled(true);
    }

    connect(QtApplication::instance(), &QtApplication::FileOpen, this, &MainWindow::OnFileOpen);
    setAcceptDrops(true);
    if (mUiLevel < 2)
        setMaximumSize(minimumSizeHint());
    else
        resize(512, 768);
    ReadSettings(mIniFile);

    if (mpRunningTimeout->text().trimmed().isEmpty())
        mpRunningTimeout->setText("10s");

    if (mpProcessesTimeout->text().trimmed().isEmpty())
        mpProcessesTimeout->setText("2s");

    if (mpWatches->rowCount() == 0) {
        mpWatches->setRowCount(1);
        mpWatches->setItem(0, 0, new QTableWidgetItem("SourceTime"));
        mpWatches->setItem(0, 1, new QTableWidgetItem("2s"));
        mpWatches->setItem(0, 2, new QTableWidgetItem("2s"));
    }
    OnWatchesTableSizeChanged();

    if (!batchFile.isEmpty())
        DoOpenFile(batchFile);

    if (mUiLevel < 3 && !mpStartButton->isEnabled()) {
        QMessageBox::critical(this, "BCI2000Watchdog: Invalid configuration", 
            "The configuration is invalid.\nStart BCI2000Watchdog with an UI level of 3 to select a valid configuration."
        );
        mDontRun = true;
    }

    mWatchdog.SetListener(this);

    if (autoStart)
        mAutoStartTimerId = startTimer(0);
}

MainWindow::~MainWindow()
{
    mWatchdog.SetListener(nullptr);
    WriteSettings(mIniFile);
}

bool MainWindow::DontRun() const
{
    return mDontRun;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    bool doClose = false;
    switch (mWatchdog.State()) {
        case Watchdog::Idle:
        case Watchdog::Error:
            doClose = true;
            break;
        default:
            doClose = false;
    }
    if (doClose)
        event->accept();
    else
        event->ignore();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    QList<QUrl> urlList = event->mimeData()->urls();
    if (urlList.size() == 1)
    {
        QString file = urlList.first().toLocalFile();
        if (QFileInfo(file).isFile())
            event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urlList = event->mimeData()->urls();
    if (urlList.size() == 1)
        DoOpenFile(urlList.first().toLocalFile());
}

void MainWindow::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == mAutoStartTimerId) {
        killTimer(mAutoStartTimerId);
        mpStartButton->click();
    }
}

void MainWindow::OnFileOpen(QString filename)
{
    if (!filename.isEmpty())
        DoOpenFile(filename);
}

void MainWindow::OnActionOpen()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose a Batch File"), QDir::currentPath(), 
        tr("BCI2000 Batch Files (*.bat *.bciscript)")
    );
    if (!filename.isEmpty())
        DoOpenFile(filename);
}

void MainWindow::OnActionAbout()
{
    AboutBox().SetApplicationName(QApplication::applicationName().toLocal8Bit().constData()).Display();
}

void MainWindow::OnBCI2000Help()
{
    ExecutableHelp().Display();
}

void MainWindow::OnScriptFileChanged(QString text)
{
    mpScriptFile->setToolTip(text);
    EnableStart(true);
}

void MainWindow::OnEditScriptFile()
{
    std::string text;
    std::ifstream input(mpScriptFile->text().toLocal8Bit(), std::ios::in | std::ios::binary);
    std::getline(input, text, '\0');
    input.close();

    QDialog dialog(this);
    dialog.setWindowTitle(mpScriptFile->text());
    QBoxLayout* pLayout = new QVBoxLayout;
    QPlainTextEdit* pTextEdit = new QPlainTextEdit;
    pLayout->addWidget(pTextEdit);

    pTextEdit->setFont(QFont("Courier", 10));
    int width = 100 * pTextEdit->fontMetrics().averageCharWidth();
    int height = 40 * pTextEdit->fontMetrics().height();
    pTextEdit->setPlainText(QString::fromUtf8(text.c_str()));

    QDialogButtonBox* pButtonBox = new QDialogButtonBox;
    pButtonBox->setStandardButtons(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
    connect(pButtonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(pButtonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    pLayout->addWidget(pButtonBox);

    dialog.setLayout(pLayout);
    dialog.resize(width, height);
    int result = dialog.exec();

    if (result == QDialog::Accepted)
    {
        text = pTextEdit->toPlainText().toUtf8();
        std::ofstream output(mpScriptFile->text().toLocal8Bit(), std::ios::out | std::ios::binary);
        output.write(text.data(), text.size());
    }
}

void MainWindow::OnPlusButton()
{
    mpWatches->setRowCount(mpWatches->rowCount() + 1);
    OnWatchesTableSizeChanged();
}

void MainWindow::OnMinusButton()
{
    int count = mpWatches->rowCount();
    if (count > 1) {
        mpWatches->setRowCount(count - 1);
        OnWatchesTableSizeChanged();
    }
}

void MainWindow::OnWatchesTableSizeChanged()
{
    mpPlusButton->setEnabled(true);
    int count = mpWatches->rowCount();
    mpMinusButton->setEnabled(count > 1);
}

void MainWindow::OnReset()
{
    mWatchdog.Reset();
    mpLog->Clear();
}

void MainWindow::OnStart()
{
    mWatchdog.SetBatchfile(mpScriptFile->text().toUtf8().constData());

    PhysicalUnit secondsUnit;
    secondsUnit.SetGainWithSymbol("1s");

    std::string runningTimeout = mpRunningTimeout->text().toUtf8().constData();
    if (secondsUnit.IsPhysical(runningTimeout))
        mWatchdog.SetRunningTimeout(secondsUnit.PhysicalToRaw(runningTimeout));
    else
        mWatchdog.SetRunningTimeout(mpRunningTimeout->text().toDouble());
    if (std::isnan(mWatchdog.RunningTimeout())) {
        QMessageBox::critical(this, "BCI2000Watchdog Error",
            "Invalid value in Running Timeout field"
        );
        return;
    }

    std::string processesTimeout = mpProcessesTimeout->text().toUtf8().constData();
    if (secondsUnit.IsPhysical(processesTimeout))
        mWatchdog.SetProcessesTimeout(secondsUnit.PhysicalToRaw(processesTimeout));
    else
        mWatchdog.SetProcessesTimeout(mpProcessesTimeout->text().toDouble());
    if (std::isnan(mWatchdog.ProcessesTimeout())) {
        QMessageBox::critical(this, "BCI2000Watchdog Error",
            "Invalid value in Processes Timeout field"
        );
        return;
    }

    std::vector<Watchdog::Watch> watches;
    for (int row = 0; row < mpWatches->rowCount(); ++row)
    {
        Watchdog::Watch watch;
        watch.timeoutSeconds = std::numeric_limits<double>::quiet_NaN();
        auto pItem = mpWatches->item(row, 0);
        if (pItem)
            watch.expression = pItem->text().toUtf8();
        pItem = mpWatches->item(row, 1);
        if (pItem)
        {
            std::string text = pItem->text().toUtf8().constData();
            if (secondsUnit.IsPhysical(text))
                watch.initialTimeoutSeconds = secondsUnit.PhysicalToRaw(text);
            else
                watch.initialTimeoutSeconds = pItem->text().toDouble();
        }
        pItem = mpWatches->item(row, 2);
        if (pItem)
        {
            std::string text = pItem->text().toUtf8().constData();
            if (secondsUnit.IsPhysical(text))
                watch.timeoutSeconds = secondsUnit.PhysicalToRaw(text);
            else
                watch.timeoutSeconds = pItem->text().toDouble();
        }
        if (!watch.expression.empty() && !std::isnan(watch.timeoutSeconds))
            watches.push_back(watch);
    }
    mWatchdog.SetWatches(watches);
    mWatchdog.StartBCI2000();
}

void MainWindow::OnTerminate()
{
    mWatchdog.TerminateBCI2000();
}

void MainWindow::EnableStart(bool enable)
{
    if (enable) {
        QFileInfo info(mpScriptFile->text());
        enable = info.isFile();
        enable = enable && (info.suffix() == "bat" || info.suffix() == "bciscript");
    }

    QMetaObject::invokeMethod(mpStartAction, "setEnabled", Q_ARG(bool, enable));
    QMetaObject::invokeMethod(mpStartButton, "setEnabled", Q_ARG(bool, enable));
    QMetaObject::invokeMethod(mpEditButton, "setEnabled", Q_ARG(bool, enable));
}

void MainWindow::EnableTerminate(bool enable)
{
    QMetaObject::invokeMethod(mpTerminateAction, "setEnabled", Q_ARG(bool, enable));
    QMetaObject::invokeMethod(mpTerminateButton, "setEnabled", Q_ARG(bool, enable));
}

void MainWindow::EnableReset(bool enable)
{
    QMetaObject::invokeMethod(mpResetAction, "setEnabled", Q_ARG(bool, enable));
    QMetaObject::invokeMethod(mpResetButton, "setEnabled", Q_ARG(bool, enable));
}

void MainWindow::BuildFileMenu()
{
    QMenu* pMenu = menuBar()->addMenu("File");
    QAction* pAction = pMenu->addAction("Choose batch file...");
    pAction->setShortcut(Qt::CTRL | Qt::Key_O);
    connect(pAction, &QAction::triggered, this, &MainWindow::OnActionOpen);
    pMenu->addSeparator();
    pAction = pMenu->addAction("Quit");
    pAction->setShortcut(Qt::CTRL | Qt::Key_Q);
    connect(pAction, &QAction::triggered, this, &MainWindow::close);
}

void MainWindow::BuildEditMenu()
{
    QMenu* pMenu = menuBar()->addMenu("Edit");
    QAction* pAction = pMenu->addAction("Cut");
    pAction->setShortcut(Qt::CTRL | Qt::Key_X);
    pAction->setEnabled(false);
    pAction = pMenu->addAction("Copy");
    pAction->setShortcut(Qt::CTRL | Qt::Key_C);
    pAction->setEnabled(false);
    pAction = pMenu->addAction("Paste");
    pAction->setShortcut(Qt::CTRL | Qt::Key_V);
    pAction->setEnabled(false);
    pMenu->addSeparator();
    pAction = pMenu->addAction("Clear");
    connect(pAction, &QAction::triggered, mpLog, &LogWidget::Clear);
}

void MainWindow::BuildActionsMenu()
{
    QMenu* pMenu = menuBar()->addMenu("Actions");
    mpStartAction = pMenu->addAction("Start BCI2000");
    mpStartAction->setShortcut(Qt::CTRL | Qt::Key_R);
    mpStartAction->setEnabled(false);
    connect(mpStartAction, &QAction::triggered, this, &MainWindow::OnStart);
    mpTerminateAction = pMenu->addAction("Terminate BCI2000");
    mpTerminateAction->setShortcut(Qt::CTRL | Qt::Key_T);
    mpTerminateAction->setEnabled(false);
    connect(mpTerminateAction, &QAction::triggered, this, &MainWindow::OnTerminate);
    pMenu->addSeparator();
    mpResetAction = pMenu->addAction("Reset");
    mpResetAction->setShortcut(Qt::CTRL | Qt::ALT | Qt::Key_R);
    mpResetAction->setEnabled(false);
    connect(mpResetAction, &QAction::triggered, this, &MainWindow::OnReset);
}

void MainWindow::BuildHelpMenu()
{
    QMenu* pMenu = menuBar()->addMenu("Help");
    QAction* pAction = pMenu->addAction("BCI2000 Help");
    connect(pAction, &QAction::triggered, this, &MainWindow::OnBCI2000Help);
    pAction = pMenu->addAction("About...");
    connect(pAction, &QAction::triggered, this, &MainWindow::OnActionAbout);
}

void MainWindow::DoOpenFile(const QString& filename)
{
    QFileInfo info(filename);
    QDir::setCurrent(info.canonicalPath());
    mpScriptFile->setText(filename);
}

void MainWindow::WriteSettings(const QString& filename) const
{
    QString file = filename.isEmpty() ? Settings().fileName() : filename;
    QSettings settings(file, QSettings::IniFormat);
    settings.beginGroup("Config");
    settings.setValue("WorkingDirectory", QDir::currentPath());
    settings.setValue("BatchFile", mpScriptFile->text());
    settings.setValue("RunningTimeout", mpRunningTimeout->text());
    settings.setValue("ProcessesTimeout", mpProcessesTimeout->text());
    settings.beginWriteArray("Watches");
    int idx = 0;
    for (int row = 0; row < mpWatches->rowCount(); ++row) {
        auto pItem0 = mpWatches->item(row, 0),
             pItem1 = mpWatches->item(row, 1),
             pItem2 = mpWatches->item(row, 2);
        if (pItem0 && pItem1) {
            settings.setArrayIndex(idx++);
            settings.setValue("Expression", pItem0->text());
            settings.setValue("Initial", pItem1->text());
            settings.setValue("Timeout", pItem2->text());
        }
    }
    settings.endArray();

    settings.beginWriteArray("UI");
    settings.setArrayIndex(mUiLevel - 1);
    settings.setValue("Geometry", this->saveGeometry());
    settings.endArray();

    settings.endGroup();
}

void MainWindow::ReadSettings(const QString& filename)
{
    QString file = filename.isEmpty() ? Settings().fileName() : filename;
    QSettings settings(file, QSettings::IniFormat);
    settings.beginGroup("Config");
    QString wd = settings.value("WorkingDirectory").toString();
    if (!wd.isEmpty())
        QDir::setCurrent(wd);
    QString batchFile = settings.value("BatchFile").toString();
    mpScriptFile->setText(batchFile);
    QString runningTimeout = settings.value("RunningTimeout").toString();
    mpRunningTimeout->setText(runningTimeout);
    QString processesTimeout = settings.value("ProcessesTimeout").toString();
    mpProcessesTimeout->setText(processesTimeout);

    int count = settings.beginReadArray("Watches");
    mpWatches->setRowCount(count);
    for (int row = 0; row < count; ++row) {
        settings.setArrayIndex(row);
        QString expression = settings.value("Expression").toString();
        mpWatches->setItem(row, 0, new QTableWidgetItem(expression));
        QString minimum;
        if (settings.value("Initial").isValid()) {
           minimum = settings.value("Initial").toString();
        }
        else {
           minimum = "0s";
        }
        mpWatches->setItem(row, 1, new QTableWidgetItem(minimum));
        QString timeout = settings.value("Timeout").toString();
        mpWatches->setItem(row, 2, new QTableWidgetItem(timeout));
    }
    settings.endArray();

    settings.beginReadArray("UI");
    settings.setArrayIndex(mUiLevel - 1);
    this->restoreGeometry(settings.value("Geometry").toByteArray());
    settings.endArray();

    settings.endGroup();
}

void MainWindow::OnWatchdogState(Watchdog::StateEnum state)
{
    switch (state) {
        case Watchdog::Idle:
            EnableStart(true);
            EnableTerminate(false);
            EnableReset(true);
            break;
        case Watchdog::UserStartup:
        case Watchdog::TriggeredStartup:
            EnableStart(false);
            EnableTerminate(true);
            EnableReset(false);
            break;
        case Watchdog::Watching:
            EnableStart(false);
            EnableTerminate(true);
            EnableReset(false);
            break;
        case Watchdog::Triggered:
        case Watchdog::TriggeredTerminating:
            EnableStart(false);
            EnableTerminate(false);
            EnableReset(false);
            break;
        case Watchdog::Error:
            EnableStart(false);
            EnableTerminate(false);
            EnableReset(true);
            break;
        default:
            ;
    }
    ProcessUtils::SendProcessToForeground();
    QMetaObject::invokeMethod(this, "repaint");
}

void MainWindow::OnLogMessage(Watchdog::MessageType type, const std::string& inMsg)
{
    OnWatchdogState(mWatchdog.State());

    QColor color = Qt::black;
    switch (type) {
        case Watchdog::LogMessage:
            color = Qt::black;
            break;
        case Watchdog::Log2Message:
            color = Qt::cyan;
            break;
        case Watchdog::ErrorMessage:
            color = Qt::red;
            break;
        case Watchdog::TriggeredMessage:
            color = Qt::magenta;
            break;
    }

    QString msg = QString::fromUtf8(inMsg.c_str());
    QMetaObject::invokeMethod(mpLog, "AddEntry", Q_ARG(QString, msg), Q_ARG(QColor, color));
}
