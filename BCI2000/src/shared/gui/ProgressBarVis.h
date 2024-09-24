////////////////////////////////////////////////////////////////////////////////
// $Id $
// Author: mellinger@neurotechcenter.org
// Description: A Visualization object representing a progress bar.
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
#ifndef PROGRESS_BAR_VIS_H
#define PROGRESS_BAR_VIS_H

#include "GenericVisualization.h"

class ProgressBarVis : public BitmapVisualization
{
  public:
    ProgressBarVis();
    ~ProgressBarVis();
    // Properties
    ProgressBarVis &SetTotal(int);
    int Total() const;
    ProgressBarVis &SetCurrent(int);
    int Current() const;
    ProgressBarVis &SetBackgroundColor(RGBColor);
    RGBColor BackgroundColor() const;
    ProgressBarVis &SetForegroundColor(RGBColor);
    RGBColor ForegroundColor() const;
    ProgressBarVis &SetWidth(int);
    int Width() const;
    ProgressBarVis &SetHeight(int);
    int Height() const;
    // Actions
    ProgressBarVis& SendReferenceFrame();
    ProgressBarVis& SendDifferenceFrame();

  private:
    struct Private;
    Private *p;
};

#endif // PROGRESS_BAR_VIS_H
