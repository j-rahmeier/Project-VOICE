////////////////////////////////////////////////////////////////////////////////
// $Id: CSPDataWindow.h 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Data window for Common Spatial Patterns.
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
#ifndef CSP_DATA_WINDOW_H
#define CSP_DATA_WINDOW_H

#include <QMainWindow>

struct AnalysisData;
class AnalysisWindow;

class CSPDataWindow : public QMainWindow
{
public:
  CSPDataWindow(AnalysisWindow* pAnalysisWindow, const AnalysisData&, const QString& file = "");
  ~CSPDataWindow();

private:
  struct Private;
  Private* p;
};

#endif // CSP_DATA_WINDOW_H
