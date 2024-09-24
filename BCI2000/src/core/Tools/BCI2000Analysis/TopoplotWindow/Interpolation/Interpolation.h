////////////////////////////////////////////////////////////////////////////////
// $Id: Interpolation.h 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Given a non-uniformly spaced set of data points, and values
//   corresponding to these data points, computes interpolated data on a
//   uniformly spaced grid.
//   Coordinates are supposed to be between 0 and 1.
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
#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include "GUI.h"
#include <vector>

class Interpolation
{
  public:
    Interpolation();
    virtual ~Interpolation();
    Interpolation(const Interpolation &) = delete;
    Interpolation &operator=(const Interpolation &) = delete;

    void setPoints(const std::vector<GUI::Point> &);
    void setValues(const std::vector<float> &);
    void interpolatePlane(float *, int nx, int ny) const;
    float interpolateAt(const GUI::Point &) const;

  protected:
    virtual void onSetPoints(const std::vector<GUI::Point> &) = 0;
    virtual void onSetValues(const std::vector<float> &) = 0;
    virtual void onInterpolatePlane(float *, int nx, int ny) const = 0;
    virtual float onInterpolateAt(const GUI::Point &) const = 0;

  private:
    int mNumPoints;
    bool mHaveValues;
};

#endif // INTERPOLATION_H
