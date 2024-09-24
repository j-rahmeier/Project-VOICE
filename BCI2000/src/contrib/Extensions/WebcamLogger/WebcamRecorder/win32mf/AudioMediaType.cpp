////////////////////////////////////////////////////////////////////////////////
// $Id: AudioMediaType.cpp 7882 2024-03-01 20:34:21Z mellinger $
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
#include "AudioMediaType.h"
#include "Errors.h"
#include "Exception.h"

#include <mfapi.h>

AudioMediaType::AudioMediaType(com::Ptr<IMFMediaType> pType)
  : mpType(pType)
{
  if (mpType) {
    if (!IsAudio(mpType))
      throw std_runtime_error << "Not an audio media type";

    GUID guid;
    HRSucceedOrThrow hr = pType->GetGUID(MF_MT_SUBTYPE, &guid);
    if (guid == MFAudioFormat_PCM)
        codec = FCC('sowt');
    else if (guid == MFAudioFormat_AAC)
        codec = FCC('aac ');
    channels = ::MFGetAttributeUINT32(pType, MF_MT_AUDIO_NUM_CHANNELS, 0);
    samplerate = ::MFGetAttributeUINT32(pType, MF_MT_AUDIO_SAMPLES_PER_SECOND, 0);
    bitspersample = ::MFGetAttributeUINT32(pType, MF_MT_AUDIO_BITS_PER_SAMPLE, 16);
  }
}

std::string AudioMediaType::Describe() const
{
  union { DWORD i; char c[5]; } fourcc = { codec };
  fourcc.c[4] = 0;
  std::ostringstream oss;
  oss << fourcc.c << " " << bitspersample << " x " << samplerate << "Hz";
  return oss.str();
}

com::Ptr<IMFMediaType> AudioMediaType::Strip() const
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

  static GUID uint32keys[] = {
    MF_MT_AUDIO_NUM_CHANNELS, MF_MT_AUDIO_SAMPLES_PER_SECOND,
    MF_MT_AUDIO_BITS_PER_SAMPLE,
  };
  for (const auto& key : uint32keys) {
    UINT32 value;
    HRESULT r = mpType->GetUINT32(key, &value);
    if (SUCCEEDED(r))
      hr = pOut->SetUINT32(key, value);
  }
  return pOut;
}

bool AudioMediaType::IsCompatible(const AudioMediaType& other) const
{
  if (codec != other.codec)
    return false;
  if (samplerate != other.samplerate)
    return false;
  if (bitspersample != other.bitspersample)
    return false;
  return true;
}

bool AudioMediaType::IsAudio(com::Ptr<IMFMediaType> pType)
{
  GUID guid;
  HRSucceedOrThrow hr = pType->GetGUID(MF_MT_MAJOR_TYPE, &guid);
  return guid == MFMediaType_Audio;
}

double AudioMediaType::sqDist(const AudioMediaType& other) const
{
  double total = 0;
  total += 1.0 * (this->samplerate - other.samplerate) * (this->samplerate - other.samplerate);
  total += 1.0 * (this->bitspersample - other.bitspersample) * (this->bitspersample - other.bitspersample);
  total += 1.0 * (this->channels - other.channels) * (this->channels - other.channels);
  return total;
}
