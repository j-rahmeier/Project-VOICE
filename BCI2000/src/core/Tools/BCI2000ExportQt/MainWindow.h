//////////////////////////////////////////////////////////////////////////////
// $Id: MainWindow.h 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: BCI2000Export main window
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
//////////////////////////////////////////////////////////////////////////////
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
class BCI2000FileReader;

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

  private:
    void dragEnterEvent(QDragEnterEvent *);
    void dropEvent(QDropEvent *);
    void ChannelNames_handleFiles(const QStringList &);
    void ImportStates_handleFiles(const QStringList &);
    void DropArea_handleFiles(const QStringList &);

  private slots:
    void on_FileOpen();
    void on_FileQuit();
    void on_HelpBCI2000Help();
    void on_HelpAbout();

  private:
    void EnableAll();
    void DisableAll();
    void SaveSettings() const;
    void ReadSettings();

  private:
    struct KeyPressListWidget : public QListWidget
    {
        void keyPressEvent(QKeyEvent *);
    };
    KeyPressListWidget *mpImportStates;
    QTextEdit *mpChannelNames;
    QLabel *mpDropArea;
    QComboBox *mpOutputFormat;
    QLabel *mpStatusLabel;
};

#endif // MAINWINDOW_H
