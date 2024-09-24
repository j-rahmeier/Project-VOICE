////////////////////////////////////////////////////////////////////////////////
// $Id: Interpolation.cpp 7463 2023-06-30 14:59:17Z mellinger $
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
#include "Interpolation.h"
#include "Debugging.h"

Interpolation::Interpolation() : mNumPoints(0), mHaveValues(false)
{
}

Interpolation::~Interpolation()
{
}

void Interpolation::setPoints(const std::vector<GUI::Point> &points)
{
    mHaveValues = false;
    mNumPoints = points.size();
    onSetPoints(points);
}

void Interpolation::setValues(const std::vector<float> &values)
{
    if (values.size() != mNumPoints)
        throw std_runtime_error << "Number of values is " << values.size() << ", must match number of points ("
                                << mNumPoints << ")";
    mHaveValues = true;
    onSetValues(values);
}

void Interpolation::interpolatePlane(float *pData, int nx, int ny) const
{
    if (mNumPoints == 0)
        throw std_runtime_error << "No points have been specified prior to interpolation";
    if (!mHaveValues)
        throw std_runtime_error << "No values have been specified before interpolation";

    onInterpolatePlane(pData, nx, ny);
}

float Interpolation::interpolateAt(const GUI::Point &point) const
{
    if (mNumPoints == 0)
        throw std_runtime_error << "No points have been specified prior to interpolation";
    if (!mHaveValues)
        throw std_runtime_error << "No values have been specified before interpolation";

    return onInterpolateAt(point);
}
