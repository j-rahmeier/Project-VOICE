////////////////////////////////////////////////////////////////////////////////
// $Id: AudioMediaType.h 7882 2024-03-01 20:34:21Z mellinger $
// Author: mellinger@neurotechcenter.com
// Description: A wrapper for Windows Media Foundation audio media types.
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
#ifndef AUDIO_MEDIA_TYPE_H
#define AUDIO_MEDIA_TYPE_H

#include <mfidl.h>
#include "ComPtr.h"

struct AudioMediaType
{
  AudioMediaType(com::Ptr<IMFMediaType> = nullptr);

  mutable com::Ptr<IMFMediaType> mpType;

  DWORD codec = 0;
  UINT32 channels = 0;
  UINT32 samplerate = 0, bitspersample = 0;
  UINT32 avgbitrate = 0; // not usable for codec comparation

  double sqDist(const AudioMediaType&) const;
  std::string Describe() const;
  com::Ptr<IMFMediaType> Strip() const;
  bool IsCompatible(const AudioMediaType&) const;

  static bool IsAudio(com::Ptr<IMFMediaType> type);
};

#endif // AUDIO_MEDIA_TYPE_H

