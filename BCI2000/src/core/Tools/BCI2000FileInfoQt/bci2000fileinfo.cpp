//////////////////////////////////////////////////////////////////////
// $Id: bci2000fileinfo.cpp 8243 2024-07-09 17:56:32Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: The main window of BCI2000FileInfo.
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
#include "bci2000fileinfo.h"
#include "ui_bci2000fileinfo.h"

#include <QFileDialog>
#include <QtWidgets>

#include "QtMain.h"
#include "AboutBox.h"
#include "BitmapImage.h"
#include "ConfigWindow.h"
#include "ExecutableHelp.h"
#include "GenericSignal.h"
#include "Preferences.h"
#include "Preferences.h"
#include "Settings.h"
#include "ShowStates.h"

#include <sstream>

#define EMPTY_FILENAME "<p align=center><i>&lt;drop a parameter or data file&gt;</i>" // using a macro allows translation

BCI2000FileInfo::BCI2000FileInfo(QWidget *parent) : QMainWindow(parent), ui(new Ui::BCI2000FileInfo)
{
    ui->setupUi(this);
    for (QLabel* pLabel : {
        ui->filename,
        ui->blockSize,
        ui->feedback,
        ui->samplingRate,
        ui->formatVer,
        ui->dataFormat,
    }) {
        QFont font = pLabel->font();
        font.setBold(true);
        pLabel->setFont(font);
    }
    ui->filename->setText(tr(EMPTY_FILENAME));
    connect(QtApplication::instance(), &QtApplication::FileOpen, this, &BCI2000FileInfo::onFileOpen);
    setAcceptDrops(true);
    Preferences::Instance()->mUserLevel = Preferences::None; // suppress parameter level slider
    ReadSettings();
    if (qApp->arguments().size() > 1)
        DoOpenFile(qApp->arguments().at(1));
}

BCI2000FileInfo::~BCI2000FileInfo()
{
    WriteSettings();
    delete ui;
}

void BCI2000FileInfo::dragEnterEvent(QDragEnterEvent *event)
{
    // Only accept if the dragged object is a single BCI2000 file
    QList<QUrl> urlList = event->mimeData()->urls();
    if (urlList.size() == 1)
    {
        QString file = urlList.first().toLocalFile();
        if (QFileInfo(file).isFile())
        {
            std::string filename = file.toLocal8Bit().constData();
            if (BCI2000FileReader(filename).IsOpen() || ParamList().Load(filename))
                event->acceptProposedAction();
        }
    }
}

void BCI2000FileInfo::dropEvent(QDropEvent *event)
{
    QList<QUrl> urlList = event->mimeData()->urls();
    if (urlList.size() == 1)
        DoOpenFile(urlList.first().toLocalFile());
}

void BCI2000FileInfo::onFileOpen(const QString& filename)
{
    if (!filename.isEmpty())
    {
        QDir::setCurrent(QFileInfo(filename).canonicalPath());
        DoOpenFile(filename);
    }
}

void BCI2000FileInfo::on_parmButton_clicked()
{
    gpConfig->Initialize(&mParameters);
    gpConfig->show();
}

void BCI2000FileInfo::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Data File"), QDir::currentPath(), 
        tr("BCI2000 Data Files (*.dat *.bcidat);;BCI2000 Parameter Files (*.prm *.bciprm)")
    );
    if (!filename.isEmpty())
    {
        QDir::setCurrent(QFileInfo(filename).canonicalPath());
        DoOpenFile(filename);
    }
}

void BCI2000FileInfo::on_actionAbout_triggered()
{
    // Do stuff for About
    AboutBox().SetApplicationName(QApplication::applicationName().toLocal8Bit().constData()).Display();
}

void BCI2000FileInfo::on_actionBCI2000_Help_triggered()
{
    // Do stuff for help
    ExecutableHelp().Display();
}

void BCI2000FileInfo::DoOpenFile(QString filename)
{
    enum kind { none, dat, prm };
    kind mode = none;

    mParameters.Clear();
    std::string file = filename.toLocal8Bit().constData();
    mFile.Open(file);
    if (mFile.IsOpen())
    {
        mParameters = *mFile.Parameters();
        mode = dat;
    }
    else if (mParameters.Load(file, true))
    {
        mode = prm;
    }
    else
    {
        mode = none;
    }

    if (mode == none)
    {
        if (gpConfig)
            gpConfig->accept();
        QMessageBox msgBox;
        std::ostringstream oss;
        oss << "Could not open \"" << filename.toLocal8Bit().constData() << "\" as a BCI2000 data or parameter file.";
        msgBox.setText(QString::fromLocal8Bit(oss.str().c_str()));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        ui->parmButton->setEnabled(false);
        ui->blockSize->setText("");
        ui->filename->setText(tr(EMPTY_FILENAME));
        ui->feedback->setText("");
        ui->samplingRate->setText("");
        ui->formatVer->setText("");
        ui->dataFormat->setText("");
    }
    else if (mode == dat)
    {
        ui->parmButton->setEnabled(true);
        ui->filename->setText(filename);
        float blockSize = mFile.SignalProperties().Elements();
        if (blockSize > 0)
        {
            ui->blockSize->setText(mFile.Parameter("SampleBlockSize").c_str());
            ui->feedback->setText(QString::number(mFile.SamplingRate() / blockSize).append(QString(" Hz")));
            ui->samplingRate->setText(QString::number(mFile.SamplingRate()).append(QString(" Hz")));
            ui->formatVer->setText(mFile.FileFormatVersion().c_str());
            ui->dataFormat->setText(mFile.SignalProperties().Type().Name());
        }
    }
    else if (mode == prm)
    {
        ui->parmButton->setEnabled(true);
        ui->filename->setText(filename);

        ui->blockSize->setText(tr("N/A"));
        ui->feedback->setText(tr("N/A"));
        ui->samplingRate->setText(tr("N/A"));
        ui->formatVer->setText(tr("N/A"));
        ui->dataFormat->setText(tr("N/A"));
    }

    if (mode != none)
    {
        // Populate Config Window
        if (gpConfig == nullptr)
            gpConfig = new ConfigWindow(this);
        gpConfig->Initialize(&mParameters);
    }
}

void BCI2000FileInfo::WriteSettings() const
{
    Settings settings;
    settings.beginGroup("Config");
    settings.setValue("Geometry", this->saveGeometry());
    settings.setValue("WorkingDirectory", QDir::currentPath());
}

void BCI2000FileInfo::ReadSettings()
{
    Settings settings;
    settings.beginGroup("Config");
    this->restoreGeometry(settings.value("Geometry").toByteArray());
    QString wd = settings.value("WorkingDirectory").toString();
    if (!wd.isEmpty())
        QDir::setCurrent(wd);
}
