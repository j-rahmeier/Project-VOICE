////////////////////////////////////////////////////////////////////////////////
// $Id: VideoMediaType.cpp 7882 2024-03-01 20:34:21Z mellinger $
// Author: mellinger@neurotechcenter.com
// Description: A wrapper for Windows Media Foundation video media types.
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
#include "Errors.h"
#include "Exception.h"

#include <mfapi.h>

DWORD VideoMediaType::pref[] = {
  22, // RGB32
  21, // ARGB32
};

VideoMediaType::VideoMediaType(com::Ptr<IMFMediaType> pType)
  : mpType(pType)
{
  if (mpType) {
    if (!IsVideo(mpType))
      throw std_runtime_error << "Not a video media type";

    GUID guid;
    HRSucceedOrThrow hr = pType->GetGUID(MF_MT_SUBTYPE, &guid);
    codec = guid.Data1;
    hr = ::MFGetAttributeSize(pType, MF_MT_FRAME_SIZE, &width, &height);
    hr = ::MFGetAttributeRatio(pType, MF_MT_FRAME_RATE, &framerateN, &framerateD);
    if (S_OK != ::MFGetAttributeRatio(pType, MF_MT_PIXEL_ASPECT_RATIO, &pxN, &pxD))
    {
      pxN = 1;
      pxD = 1;
      hr = ::MFSetAttributeRatio(pType, MF_MT_PIXEL_ASPECT_RATIO, pxN, pxD);
    }
    if (S_OK != pType->GetUINT32(MF_MT_INTERLACE_MODE, &interlaceType))
    {
      interlaceType = MFVideoInterlace_Progressive;
      hr = pType->SetUINT32(MF_MT_INTERLACE_MODE, interlaceType);
    }
    avgbitrate = ::MFGetAttributeUINT32(pType, MF_MT_AVG_BITRATE, 0);
    if (avgbitrate == 0)
    {
      avgbitrate = width * height * framerateN / (1.0 * framerateD) * 32;
      hr = pType->SetUINT32(MF_MT_AVG_BITRATE, avgbitrate);
    }
  }
}

std::string VideoMediaType::Describe() const
{
  union { DWORD i; char c[5]; } fourcc = { codec };
  fourcc.c[4] = 0;
  std::ostringstream oss;
  double framerate = 1.0 * framerateN / framerateD;
  oss << fourcc.c << " " << width << "x" << height << "px " << framerate << "Hz";
  return oss.str();
}

com::Ptr<IMFMediaType> VideoMediaType::Strip() const
{
  com::Ptr<IMFMediaType> pOut;
  if (!mpType)
    return pOut;

  HRSucceedOrThrow hr = ::MFCreateMediaType(pOut.Assignee());

  static GUID guidkeys[] = { MF_MT_MAJOR_TYPE, MF_MT_SUBTYPE };
  for (const auto& key : guidkeys) {
    GUID value;
    HRESULT r = mpType->GetGUID(key, &value);
    if (SUCCEEDED(r))
      hr = pOut->SetGUID(key, value);
  }

  static GUID int32keys[] = {
    MF_MT_INTERLACE_MODE, MF_MT_AVG_BITRATE,
  };
  for (const auto& key : int32keys) {
    UINT32 value;
    HRESULT r = mpType->GetUINT32(key, &value);
    if (SUCCEEDED(r))
      hr = pOut->SetUINT32(key, value);
  }

  static GUID int64keys[] = {
    MF_MT_FRAME_SIZE, MF_MT_FRAME_RATE, MF_MT_PIXEL_ASPECT_RATIO
  };
  for (const auto& key : int64keys) {
    UINT64 value;
    HRESULT r = mpType->GetUINT64(key, &value);
    if (SUCCEEDED(r))
      hr = pOut->SetUINT64(key, value);
  }

  return pOut;
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

bool VideoMediaType::IsVideo(com::Ptr<IMFMediaType> pType)
{
  GUID guid;
  HRSucceedOrThrow hr = pType->GetGUID(MF_MT_MAJOR_TYPE, &guid);
  return guid == MFMediaType_Video;
}

double VideoMediaType::sqDist(const VideoMediaType& other) const
{
  double total = 0;
  int numEntries = sizeof(pref) / sizeof(*pref);
  DWORD* p1 = std::find(pref, pref + numEntries, this->codec);
  DWORD* p2 = std::find(pref, pref + numEntries, other.codec);
  total += 1e10 * (p2 - p1) * (p2 - p1) / numEntries / numEntries;
  total += 1.0 * (this->width - other.width) * (this->width - other.width);
  total += 1.0 * (this->height - other.height) * (this->height - other.height);
  double f1 = this->framerateN * 1.0 / this->framerateD;
  double f2 = other.framerateN * 1.0 / other.framerateD;
  total += 1e2 * (f1 - f2) * (f1 - f2) * 50;
  return total;
}
