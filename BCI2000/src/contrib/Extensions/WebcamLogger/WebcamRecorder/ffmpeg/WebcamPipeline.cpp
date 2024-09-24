////////////////////////////////////////////////////////////////////////////////
// $Id: WebcamPipeline.cpp 7882 2024-03-01 20:34:21Z mellinger $
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
#include "WebcamPipeline.h"

WebcamPipeline::WebcamPipeline()
  : mCameraIndex(-1), mpWidget(nullptr), mTimestampMode(0)
{
}

WebcamPipeline::~WebcamPipeline()
{
}

std::string WebcamPipeline::Error() const
{
  std::lock_guard<std::mutex> lock(mMutex);
  return mError;
}

void WebcamPipeline::SetError(const std::string& s)
{
  std::lock_guard<std::mutex> lock(mMutex);
  mError = s;
}

void WebcamPipeline::SetDesiredVideoMediaType(const VideoMediaType& type)
{
  std::lock_guard<std::mutex> lock(mMutex);
  mDesiredVideoMediaType = type;
}

const VideoMediaType& WebcamPipeline::ActualVideoMediaType() const
{
  std::lock_guard<std::mutex> lock(mMutex);
  return mActualVideoMediaType;
}

void WebcamPipeline::SetDesiredAudioMediaType(const AudioMediaType& type)
{
  std::lock_guard<std::mutex> lock(mMutex);
  mDesiredAudioMediaType = type;
}

const AudioMediaType& WebcamPipeline::ActualAudioMediaType() const
{
  std::lock_guard<std::mutex> lock(mMutex);
  return mActualAudioMediaType;
}

void WebcamPipeline::ActivateCameraFromWebcamIndex(int inCameraIndex)
{
  std::lock_guard<std::mutex> lock(mMutex);
  // Enumerate camera devices attached locally, and choose the one that has 
  // the speicifed index.
  // If there is no camera with the specified index, call SetError() with a descriptive
  // error message.
  // If the camera supports multiple video formats, choose the one that is closest to
  // mDesiredVideoMediaType. Then, copy it into mActualVideoMediaType.

  // ...

  mCameraName = "<User visible camera name obtained from API>";

  mCameraIndex = inCameraIndex;
}

void WebcamPipeline::ActivateCameraFromURL(
  int inCameraIndex, const std::string& inCameraURL,
  int inBufferingTime, int inMaxBufferTimeMs
)
{
  std::lock_guard<std::mutex> lock(mMutex);
  mCameraName = inCameraURL;
  mCameraIndex = inCameraIndex;
  // Set up video input from the specified URL, and copy the streamed video format into
  // mActualVideoMediaType.
  // If the URL cannot be opened, write a descriptive error message using SetError().
  // BufferingTime (in seconds) and MaxBufferTimeMs (in milliseconds) may not be relevant
  // for the implementation, just ignore them then.
}

void WebcamPipeline::ActivateAudioFromURL(
  const std::string& inURL,
  int inBufferingTime, int inMaxBufferTimeMs
)
{
  std::lock_guard<std::mutex> lock(mMutex);
  // Set up audio input from the specified URL, and copy the URLs audio format into mActualAudioMediaType.
  // If the URL cannot be opened (or if audio is not supported),
  // write a descriptive error message using SetError().
}

const std::string& WebcamPipeline::CameraName() const
{
  std::lock_guard<std::mutex> lock(mMutex);
  return mCameraName;
}

void WebcamPipeline::SetWidget(VideoWidget* pWidget)
{
  std::lock_guard<std::mutex> lock(mMutex);
  mpWidget = pWidget;
}

VideoWidget* WebcamPipeline::Widget() const
{
  std::lock_guard<std::mutex> lock(mMutex);
  return mpWidget;
}

void WebcamPipeline::StartAcquisition()
{
    // Here, start the acquisition thread.
    // The thread should do the following:
    // 1. Wait for a frame from the input.
    // 2. Decode the frame to RGB32 format.
    // 3. Add a timestamp to the frame according to the timestamp setting.
    // 4. Send a copy of the frame to the video widget using the VideoWidget::SetFrame() function.
    // 5. If decimation is active and the raw frame number modulo N is nonzero, go to 1.
    // 6. Write the decimated frame number to std::cout, followed with std::endl.
    // 7. Check if the file name has changed, and switch to the new file without
    //    losing frames, resetting frame number to 0.
    // 8. Encode/save the frame to the file using the h.264 codec. Do not use source time stamps
    //    but nominal time stamps constructed from the output frame rate, and the frame number.
    //    Otherwise, it will not be possible to match frame numbers with frame positions in the file
    //    when analyzing data.
    // 9. Go to 1.
}

void WebcamPipeline::StopAcquisition()
{
    // Stop the acquisition thread and release all objects that it may have allocated.
}

void WebcamPipeline::StartRecording(const std::string& filename)
{
    // Notify the acquisition thread that it should begin recording at the next processed frame.
}

void WebcamPipeline::SwitchRecording(const std::string& filename)
{
    // Notify the acquisition thread that it should close and re-open the output with a new filename
    // at the next processed frame.
}

void WebcamPipeline::StopRecording()
{
    // Notify the acquisition thread that it should close the output before writing the next frame.
}

void WebcamPipeline::SetTimestampMode(int mode)
{
    // Notify the acquisition thread to switch to a different timestamp mode.
    // Modes are:
    // 0: none,
    // 1: UpperRight,
    // 2: UpperLeft,
    // 3: LowerRight,
    // 4: LowerLeft,
    // 5: FrameCount
    mTimestampMode = mode;
}

void WebcamPipeline::SetDecimation(int decimation)
{
    // Notify the acquisition thread that decimation has changed.
    // A decimation of N means that only every Nth frame will be saved.
    mDecimation = std::max(decimation, 1);
}

