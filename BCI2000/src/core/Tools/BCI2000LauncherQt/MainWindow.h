////////////////////////////////////////////////////////////////////////////////
// $Id: MainWindow.h 8341 2024-09-09 15:19:13Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: The main window of the BCI2000Launcher tool.
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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QSplitter>
#include <QLabel>
#include <QMainWindow>

#include <map>

#include "ModuleInventory.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

  private:
    Ui::MainWindow *ui;

  private slots:
    void on_launchButton_clicked();
    void on_subjectSessionEdit_textChanged(QString);
    void on_saveDirEdit_textChanged(QString);
    void on_subjectNameEdit_textChanged(QString);
    void on_subjectSessionEdit_editingFinished();
    void on_chooseDirBtn_clicked();
    void on_removeParmfileBtn_clicked();
    void on_addParmfileBtn_clicked();
    void on_actionDown_triggered();
    void on_actionUp_triggered();
    void on_clearSelectionBtn_clicked();
    void on_actionTo_Signal_Source_triggered();
    void on_actionTo_Signal_Processing_triggered();
    void on_actionTo_Applications_triggered();
    void on_actionTo_Others_triggered();
    void on_actionBCI2000_Help_triggered();
    void on_actionAbout_triggered();
    void on_actionReset_View_triggered();
    void on_actionCreate_Batch_File_triggered();
    void on_actionQuit_triggered();

    void onProgramSelectionChanged();
    void onExtensionItemChanged(class QTreeWidgetItem*);
    void onExtensionCollapsedChanged(class QTreeWidgetItem*);

  private:
    void SetupProgramsGroup();
    void SetupExtensionsGroup();
    void SetupScriptingCommandsGroup();
    void SetupParametersGroup();
    void SetupStatusGroup();
    void SetupGlobalLayout();
    void SetupActions();

    void SetupInventory();
    QStringList ScanForPrograms();
    void DistributePrograms(const QStringList &);
    void ClearProgramEntries(const QString &);
    void IntroduceNewProgram(const QString &);
    QStringList GetExtensions(const QString& program) const;
    QString ExtensionArgs(const QString& category) const;

    void MoveListItem(class QListWidget *pTarget);
    void SaveSettings() const;
    void ReadSettings();
    void SyncToolTip();

    void WriteCommands(const std::string& separator, std::ostream&) const;

    bool StartExecutable(const std::string& executable, const std::string& args, const std::string& wd);

  private:
    QStringList mParameterFiles;
    struct Enabler { bool checked = false; QString category; };
    std::map<QString, Enabler> mExtensionEnablerState;
    std::map<QString, bool> mExtensionCollapsedState;
    bci::Inventory mInventory;
    QSplitter* mpVerticalSplitter = nullptr, *mpHorizontalSplitter = nullptr;
    QByteArray mDefaultStateVertical, mDefaultStateHorizontal;
    QLabel* mpSourceSelected, *mpSigProcSelected, *mpAppSelected;
};

#endif // MAINWINDOW_H
