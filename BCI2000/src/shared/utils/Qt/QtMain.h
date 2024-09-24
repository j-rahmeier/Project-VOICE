////////////////////////////////////////////////////////////////////////////////
// $Id: QtMain.h 7678 2023-10-26 14:21:43Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A wrapper for a QApplication object.
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
#ifndef QT_MAIN_H
#define QT_MAIN_H

#include <QObject>

class QtApplication : public QObject
{
    Q_OBJECT
    
  public:
    static QtApplication* instance();
    
    QtApplication(int &, char **);
    ~QtApplication();
    int Run();
    
  signals:
    void FileOpen(const QString&);

  private:
    struct Private;
    Private *p;
};

#endif // QT_MAIN_H
