////////////////////////////////////////////////////////////////////////////////
// $Id: AnalysisWindow.h 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Main window class for BCI2000Analysis.
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
#ifndef ANALYSIS_WINDOW_H
#define ANALYSIS_WINDOW_H
#include <QMainWindow>

class ParamList;

class AnalysisWindow : public QMainWindow
{
  public:
    AnalysisWindow();
    ~AnalysisWindow();
    AnalysisWindow(const AnalysisWindow &) = delete;
    AnalysisWindow &operator=(const AnalysisWindow &) = delete;

    void openDataFiles(const QStringList&);
    void openAnalysisFile(const QString&);

    void setFilterParameters(const ParamList &, const QWidget*);
    const ParamList &filterParameters() const;

  protected:
    void closeEvent(QCloseEvent *);

  private:
    struct Private;
    Private *p;
};

#endif // ANALYSIS_WINDOW_H
