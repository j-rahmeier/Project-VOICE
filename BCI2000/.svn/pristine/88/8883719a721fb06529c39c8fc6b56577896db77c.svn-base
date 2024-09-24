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
#ifndef NOTES_WINDOW_H
#define NOTES_WINDOW_H

#include <QWidget>

class NotesWindow : public QWidget
{
    Q_OBJECT

  public:
    static void Initialize(class MainWindow *);
    static void Deinitialize();
    static NotesWindow *Instance();

  public:
    void OnStartup();
    void OnStartRun(const char*);
    void OnNextFilePart(int);
    void OnStopRun();

  public slots:
    void AddNote(const QString&);
    void Move(int x, int y) { QWidget::move(x, y); }
    void Resize(int w, int h) { QWidget::resize(w, h); }

  private:
    NotesWindow(class MainWindow *);
    ~NotesWindow();

    struct Private;
    Private* p;
};

#endif // WATCHES_WINDOW_H
