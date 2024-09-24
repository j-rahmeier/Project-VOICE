////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.com
// Description: A class that represents a video media type.
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
#include "VideoMediaType.h"

#include <cmath>

VideoMediaType::VideoMediaType()
{
}

bool VideoMediaType::IsCompatible(const VideoMediaType& other) const
{
  if (codec != other.codec)
    return false;
  if (width != other.width)
    return false;
  if (height != other.height)
    return false;
  double pxr = 1.0 * pxN / pxD,
    other_pxr = 1.0 * other.pxN / other.pxD;
  if (::fabs(1.0 - pxr / other_pxr) > 0.05)
    return false;
  double fps = 1.0 * framerateN / framerateD,
    other_fps = 1.0 * other.framerateN / other.framerateD;
  if (::fabs(1.0 - fps / other_fps) > 0.05)
    return false;
  return true;
}

double VideoMediaType::sqDist(const VideoMediaType& other) const
{
  double total = 0;
  total += 1.0 * (this->width - other.width) * (this->width - other.width);
  total += 1.0 * (this->height - other.height) * (this->height - other.height);
  double f1 = this->framerateN * 1.0 / this->framerateD;
  double f2 = other.framerateN * 1.0 / other.framerateD;
  total += 1e2 * (f1 - f2) * (f1 - f2) * 50;
  return total;
}
