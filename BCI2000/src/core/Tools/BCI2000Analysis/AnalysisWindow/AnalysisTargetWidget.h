////////////////////////////////////////////////////////////////////////////////
// $Id: AnalysisTargetWidget.h 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A widget to select an analysis target.
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
#ifndef ANALYSIS_TARGET_WIDGET_H
#define ANALYSIS_TARGET_WIDGET_H

#include <QWidget>

class FiltersWidget;

namespace FilterGraph { class Node; }

class AnalysisTargetWidget : public QWidget
{
  public:
    AnalysisTargetWidget(FiltersWidget*);
    ~AnalysisTargetWidget();
    AnalysisTargetWidget(AnalysisTargetWidget &) = delete;
    AnalysisTargetWidget &operator=(AnalysisTargetWidget &) = delete;

    FilterGraph::Node* target() const;

  signals:
    void targetChanged(FilterGraph::Node*);

  private:
    struct Private;
    Private *p;
};

#endif // ANALYSIS_TARGET_WIDGET_H
