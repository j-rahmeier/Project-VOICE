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
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "Watchdog.h"

class MainWindow : public QMainWindow, Watchdog::Listener
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool DontRun() const;

  protected:
    void closeEvent(QCloseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    
  public slots:
    void OnFileOpen(QString);

  private slots:
    void OnActionOpen();
    void OnReset();

    void OnStart();
    void OnTerminate();

    void OnActionAbout();
    void OnBCI2000Help();

    void OnScriptFileChanged(QString);
    void OnEditScriptFile();

    void OnPlusButton();
    void OnMinusButton();
    void OnWatchesTableSizeChanged();

  private:
    void BuildFileMenu();
    void BuildEditMenu();
    void BuildActionsMenu();
    void BuildHelpMenu();

    void EnableStart(bool);
    void EnableTerminate(bool);
    void EnableReset(bool);
      
    void DoOpenFile(const QString& filename);
    void WriteSettings(const QString& filename = "") const;
    void ReadSettings(const QString& filename = "");

    void OnWatchdogState(Watchdog::StateEnum);
    void OnLogMessage(Watchdog::MessageType, const std::string&) override;

  private:
    int mUiLevel = 3;
    int mAutoStartTimerId = 0;
    QString mIniFile;
    bool mDontRun = false;

    class QLineEdit* mpScriptFile, *mpRunningTimeout, *mpProcessesTimeout;
    class QTableWidget* mpWatches;
    class LogWidget* mpLog;
    class QPushButton* mpStartButton, *mpTerminateButton, *mpResetButton,
                      *mpPlusButton, *mpMinusButton, *mpEditButton;
    class QAction* mpStartAction, *mpTerminateAction, *mpResetAction;
    Watchdog mWatchdog;
};

#endif // MAIN_WINDOW_H
