//////////////////////////////////////////////////////////////////////
// $Id$
// Authors: juergen.mellinger@neurotechcenter.org
// Description: A window to enter and display notes associated with 
//   signal positions.
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
#include "NotesWindow.h"

#include "MainWindow.h"
#include "OperatorUtils.h"
#include "Debugging.h"
#include "FileUtils.h"
#include "Files.h"

#include <mutex>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <QMenu>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QKeyEvent>

namespace {

    // This flag determines whether time stamps are saved with milliseconds.
    // Given that entering text takes a relatively long time, second resolution seems appropriate.
    const bool sSaveWithMs = false;
    // The name of the event that represents line numbers in data files.
    const char* spEventName = "Notes";
    // Increase the event width constant if 64k lines per file is too little.
    const char* spEventWidth = "16";
    // A tag appended to the base file name, indicating the type of file.
    const char* spFileTag = "notes";

    const char* spInactiveText = "Not running<br/><font color='red'>Notes will not be saved</font>";
    const char* spActiveText = "Running<br/><font color='green'>Notes will be saved</font>";

    std::string FormatMs(int64_t ms, bool withMs)
    {
        static const int minutesPerHour = 60;
        static const int secondsPerMinute = 60;
        static const int millisecondsPerSecond = 1000;
        static const int millisecondsPerMinute = millisecondsPerSecond * secondsPerMinute;
        static const int millisecondsPerHour = millisecondsPerMinute * minutesPerHour;
        std::ostringstream oss;
        if (ms < 0)
        {
            oss << "-";
            ms *= -1;
        }
        oss << std::setfill('0');
        oss << ms / millisecondsPerHour << ":";
        ms %= millisecondsPerHour;
        oss << std::setw(2) << ms / millisecondsPerMinute << ":";
        ms %= millisecondsPerMinute;
        oss << std::setw(2) << ms / millisecondsPerSecond;
        if (withMs)
        {
            oss << ".";
            ms %= millisecondsPerSecond;
            oss << std::setw(4) << ms;
        }
        return oss.str();
    }


    NotesWindow* spInstance = nullptr;
    std::mutex sInstanceMutex;
}

struct NotesWindow::Private : QObject
{
    QAction* mpMenuEntry = nullptr;
    QLineEdit* mpLineEdit = nullptr;
    QTextEdit* mpTextEdit = nullptr;
    QLabel* mpLabel = nullptr;

    File mFile;
    std::string mFileBase;
    int mFilePart = 0, mLineCount = 0;
    QDateTime mStartTime;

    bool mActive = false;

    Private(QObject* parent) : QObject(parent) {}

    void RegisterEvent()
    {
        // Add an event variable to store line numbers (one-based).
        std::string command = "SET LogLevel 0; INSERT EVENT " 
                            + std::string(spEventName) + " " + spEventWidth + " 0";
        bci::BCI_ExecuteScript(command.c_str());
    }

    void OnReturnPressed()
    {
        QString text = mpLineEdit->text();
        mpLineEdit->clear();
        AddNote(text);
    }

    void AddNote(const QString& note)
    {
        QDateTime now = QDateTime().currentDateTime();
        WriteLineToFile(now, note);

        QString text = "<i>" + now.toString(Qt::ISODate) + "</i><br/>" + note;
        if (!mFile.IsOpen()) {
            text = "<font color='grey'>" + text + "</font>";
        }
        mpTextEdit->append(text);
    }

    void SetStartTime()
    {
        mStartTime = QDateTime::currentDateTime();
    }

    void SetState(bool active)
    {
        mActive = active;
        UpdateState(mActive);
    }

    void UpdateState(bool active)
    {
        mpLabel->setText(active ? spActiveText : spInactiveText);
    }

    void OpenFile()
    {
        bool success = mFile.Open(ConstructCurrentFileName(), File::out | File::exclusive);
        UpdateState(success);
    }

    void CloseFile()
    {
        mFile.Close();
        mLineCount = 0;

        std::ostringstream oss;
        oss << "SET LogLevel 0; SET AbortOnError false; ";
        oss << "SET EVENT " << spEventName << " 0";
        bci::BCI_ExecuteScript(oss.str().c_str());
    }

    void SetRunFile(const std::string& file)
    {
        mFileBase = FileUtils::StripExtension(file);
        mFileBase += "_";
        mFileBase += spFileTag;
    }

    void SetFilePart(int part)
    {
        mFilePart = part;
    }

    std::string ConstructCurrentFileName() const
    {
        std::string file = mFileBase;
        if (mFilePart > 1)
            file += ".part" + std::to_string(mFilePart);
        file += ".txt";
        return file;
    }

    void WriteLineToFile(const QDateTime& now, const QString& userText)
    {
        if (mLineCount == 0 && mActive)
            OpenFile();

        if (mFile.IsOpen()) {
            auto deltaTime = mStartTime.msecsTo(now);
            QString line = now.toString(sSaveWithMs ? Qt::ISODateWithMs : Qt::ISODate)
                            + "\t" + QString(FormatMs(deltaTime, sSaveWithMs).c_str())
                            + "\t" + userText;
            std::string lineUtf8 = line.toUtf8().constData() + std::string("\r\n");
            mFile.Write(lineUtf8.data(), lineUtf8.size());
            ++mLineCount;

            std::ostringstream oss;
            // If the event cannot be set (because we are not in Running state any more) that's not an error.
            oss << "SET LogLevel 0; SET AbortOnError false; ";
            oss << "SET EVENT " << spEventName << " " << mLineCount;
            bci::BCI_ExecuteScript(oss.str().c_str());
        }
    }

};

void NotesWindow::Initialize(MainWindow* parent)
{
    std::unique_lock<std::mutex> lock(sInstanceMutex);
    Assert(!spInstance);
    spInstance = new NotesWindow(parent);
}

void NotesWindow::Deinitialize()
{
    std::unique_lock<std::mutex> lock(sInstanceMutex);
    NotesWindow* pInstance = spInstance;
    spInstance = nullptr;
    lock.unlock();
    delete pInstance;
}

NotesWindow* NotesWindow::Instance()
{
    return spInstance;
}

NotesWindow::NotesWindow(MainWindow* parent)
: p(new Private(this))
{
    setWindowTitle("BCI2000/Operator Notes");
    setMinimumWidth(160);
    setMinimumHeight(240);
    p->mpMenuEntry = parent->GetWindowMenu()->addAction("Notes");
    p->mpMenuEntry->setShortcut(QKeySequence("Ctrl+N"));
    p->mpMenuEntry->setCheckable(true);
    p->mpMenuEntry->setChecked(this->isVisible());
    connect(p->mpMenuEntry, &QAction::triggered, this, &QWidget::setVisible);

    QVBoxLayout* pLayout = new QVBoxLayout;
    setLayout(pLayout);

    p->mpLabel = new QLabel;
    p->mpLabel->setText(spInactiveText);
    p->mpLabel->setWordWrap(true);
    pLayout->addWidget(p->mpLabel);

    p->mpTextEdit = new QTextEdit;
    p->mpTextEdit->setReadOnly(true);
    pLayout->addWidget(p->mpTextEdit);

    p->mpLineEdit = new QLineEdit;
    p->mpLineEdit->setPlaceholderText("Enter a note here");
    pLayout->addWidget(p->mpLineEdit);
    connect(p->mpLineEdit, &QLineEdit::returnPressed, p, &Private::OnReturnPressed);
    p->mpLineEdit->setFocus();

    OperatorUtils::RestoreWidget(this);
}

NotesWindow::~NotesWindow()
{
    // p will be auto-deleted by Qt
    OperatorUtils::SaveWidget(this);
}

void NotesWindow::OnStartup()
{
    p->RegisterEvent();
}

void NotesWindow::OnStartRun(const char* inpRunFile)
{
    p->SetRunFile(inpRunFile);
    p->SetFilePart(1);
    p->CloseFile();
    p->SetStartTime();
    p->SetState(true);
}

void NotesWindow::OnNextFilePart(int inPart)
{
    p->SetFilePart(inPart);
    p->CloseFile();
    p->SetStartTime();
    p->SetState(true);
}

void NotesWindow::OnStopRun()
{
    p->CloseFile();
    p->SetState(false);
}

void NotesWindow::AddNote(const QString& note)
{
    p->AddNote(note);
}