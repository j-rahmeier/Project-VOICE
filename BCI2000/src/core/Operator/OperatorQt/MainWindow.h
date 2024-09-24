//////////////////////////////////////////////////////////////////////
// $Id: MainWindow.h 8085 2024-04-29 16:58:09Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: The Operator module's main window.
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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTime>
#include <QMainWindow>
class QPushButton;
class QLabel;

#include "OperatorModule.hpp"

#include "ParamList.h"
#include "Preferences.h"
class SysLog;
class ConnectionInfo;

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow, private bci::OperatorModule
{
    Q_OBJECT

  public:
    MainWindow();
    ~MainWindow();
    static MainWindow* Instance();

  private:
    Ui::MainWindow *ui;

  public:
    bool Terminating()
    {
        return mState & terminating;
    }
    QMenu *GetWindowMenu();

  private slots:
    void on_actionStates_triggered();
    void on_actionAbout_triggered();
    void on_actionBCI2000_Help_triggered();
    void on_actionOperator_Log_toggled(bool);
    void on_actionConnection_Info_toggled(bool);
    void on_actionPreferences_triggered();
    void on_pushButton_Config_clicked();
    void on_pushButton_SetConfig_clicked();
    void on_pushButton_RunSystem_clicked();
    void on_actionQuit_triggered();
    void on_pushButton_Quit_clicked();
    void on_pushButton_Btn1_clicked();
    void on_pushButton_Btn2_clicked();
    void on_pushButton_Btn3_clicked();
    void on_pushButton_Btn4_clicked();
    void CloseTopmostWindow();

  protected:
    void closeEvent(QCloseEvent *) override;

  private:
    bool ConfirmQuit();
    void Terminate();

    void ReadCommandLine();
    void UpdateDisplay();
    void SetFunctionButtons();
    void SetupScripts();
    std::string FormatScript(const char *eventName, const QString &script);

    void GetParameters();

  public slots:
    void SetWindowTitle(const QString &title);
    void SetFunctionButton(int idx, const QString &title, const QString &script);
    void PutParameters();
    void ForceQuit();
    void Move(int x, int y) { QWidget::move(x, y); }
    void Resize(int w, int h) { QWidget::resize(w, h); }

  private:
    void SetStartTime();

  protected:
    void timerEvent(QTimerEvent *) override;

    void OnShutdown() override;

    void OnSetConfig() override
    {
        SetStartTime();
    }
    void OnStartup() override;
    void OnStart() override;
    void OnResume() override;
    void OnStartRun(const char*) override;
    void OnNextFilePart(int) override;
    void OnSuspend() override;
    void OnStopRun() override;
    void OnCoreInput() override;
    const char *OnQuitRequest() override;
    void OnDebugMessage(const char *s) override
    {
        OnLogMessage(s);
    }
    void OnLogMessage(const char *) override;
    void OnWarningMessage(const char *) override;
    void OnErrorMessage(const char *) override;
    void OnNoteMessage(const char *) override;
    void OnScriptError(const char *) override;
    int OnUnknownCommand(const char *, const char**) override;
    const char *OnScriptHelp() override;
    void OnParameter(const char *) override;
    void OnVisSignal(const char *, int, int, float *) override;
    void OnVisMemo(const char *, const char *) override;
    void OnVisBitmap(const char *, int, int, unsigned short *) override;
    void OnVisVideoFrame(const char *, int, int, unsigned int *) override;
    void OnVisPropertyMessage(const char *, int, const char *) override;
    void OnVisProperty(const char *, int, const char *) override;
    void OnInitializeVis(const char *, const char *) override;

  private:
    QLabel *mpStatusLabels[4];
    ConnectionInfo *mpConnectionInfo;
    SysLog *mpSyslog;
    ParamList mParameters;
    QDateTime mStarttime;
    QString mTitle, mStartup;
    QStringList mTelnet, mWebsocket;
    bool mHide, mStartupIdle;
    int mUpdateTimerID;
    enum
    {
        terminating = 1,
        userQuit = 2,
        forceQuit = 4,
    };
    int mState;
    std::string mExitScript;
    std::string mButtonScripts[Preferences::numButtons];
    QPushButton *mButtons[Preferences::numButtons];
    std::map<std::string, std::string> mVisWindowTitles;
};

#endif // MAINWINDOW_H
