////////////////////////////////////////////////////////////////////////////////
// $Id: WebcamPipeline.h 7882 2024-03-01 20:34:21Z mellinger $
// Author: mellinger@neurotechcenter.com
// Description: A skeleton for a pipeline that acquires and saves video data.
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
#ifndef WEBCAM_PIPELINE_H
#define WEBCAM_PIPELINE_H

#include <string>
#include <mutex>
#include <atomic>

#include "VideoMediaType.h"
#include "AudioMediaType.h"

class VideoWidget;

class WebcamPipeline
{
public:
  WebcamPipeline();
  ~WebcamPipeline();

  std::string Error() const;

  void SetDesiredVideoMediaType(const VideoMediaType&);
  const VideoMediaType& ActualVideoMediaType() const;

  void SetDesiredAudioMediaType(const AudioMediaType&);
  const AudioMediaType& ActualAudioMediaType() const;

  void ActivateCameraFromWebcamIndex(int index);
  void ActivateCameraFromURL(int index, const std::string&, int, int);
  void ActivateAudioFromURL(const std::string&, int, int);
  const std::string& CameraName() const;

  void SetWidget(VideoWidget*);
  VideoWidget* Widget() const;

  void StartAcquisition();
  void StopAcquisition();

  void StartRecording(const std::string&);
  void SwitchRecording(const std::string&);
  void StopRecording();
  void SetTimestampMode(int);
  void SetDecimation(int);

private:
  void SetError(const std::string&);

  int mCameraIndex;
  std::string mCameraName;
  VideoWidget* mpWidget;

  VideoMediaType mDesiredVideoMediaType, mActualVideoMediaType;
  AudioMediaType mDesiredAudioMediaType, mActualAudioMediaType;

  mutable std::mutex mMutex;
  std::string mError;

  std::atomic_int mTimestampMode, mDecimation;
};

#endif // WEBCAM_PIPELINE_H
