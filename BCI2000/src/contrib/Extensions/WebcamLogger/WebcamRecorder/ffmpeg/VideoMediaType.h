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
#ifndef VIDEO_MEDIA_TYPE_H
#define VIDEO_MEDIA_TYPE_H

#include <cstdint>

struct VideoMediaType
{
  VideoMediaType();

  uint32_t codec = 0;
  uint32_t width = 0, height = 0;
  uint32_t framerateN = 0, framerateD = 0;
  uint32_t pxN = 0, pxD = 0;
  uint32_t interlaceType = 0;
  uint32_t avgbitrate = 0; // not usable for codec comparation

  double sqDist(const VideoMediaType&) const;
  bool IsCompatible(const VideoMediaType&) const;
};

#endif // VIDEO_MEDIA_TYPE_H

