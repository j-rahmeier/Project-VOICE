////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.com
// Description: A WebcamLogger component that writes AV data to files.
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
#include "SaveToFile.h"

#include "FileUtils.h"
#include "Debugging.h"
#include "WebcamClient.h"

SaveToFile::SaveToFile()
{
}

SaveToFile::~SaveToFile()
{
}

const std::string& SaveToFile::Error() const
{
  return mError;
}

void SaveToFile::SetClient(WebcamClient* pClient)
{
  mpWebcamClient = pClient;
}

WebcamClient* SaveToFile::Client() const
{
  return mpWebcamClient;
}

void SaveToFile::SetDecimation(int decimation)
{
  if (decimation < 1)
    decimation = 1;
  mDecimation = decimation;
}

int SaveToFile::Decimation() const
{
  return mDecimation;
}

void SaveToFile::SetVideoProfile(int profile)
{
  mVideoProfile = std::max(0, std::min(3, profile));
}

int SaveToFile::VideoProfile() const
{
  return mVideoProfile;
}

void SaveToFile::SetCompressionFactor(int compressionFactor)
{
    mCompressionFactor = compressionFactor;
}

int SaveToFile::CompressionFactor() const
{
    return mCompressionFactor;
}

int SaveToFile::CurrentFrameCount() const
{
    return mVideoSampleCount / mDecimation;
}

void SaveToFile::OpenFile(const std::string& filename)
{
  std::string fullfile = FileUtils::StripExtension(filename) + ".mp4";
  if (FileUtils::Exists(fullfile)) {
    throw std::runtime_error ("Output file \"" + fullfile + "\" exists, will not be touched.");
  }
  if (!FileUtils::IsWritableLocation(fullfile)) {
    throw std::runtime_error("Output file \"" + fullfile + "\" cannot be created at the given location.");
  }

  mEncoder.OpenFile(fullfile);
  if (!mEncoder.Error().empty()) {
      throw std::runtime_error(mEncoder.Error());
  }

  mEncoder.AddVideoStream(&mVideoStreamIndex);
  if (!mEncoder.Error().empty()) {
      throw std::runtime_error(mEncoder.Error());
  }

  Rate fileFps = mFps / Ratio(mDecimation, 1);
  mEncoder.InitializeVideoStream(mVideoStreamIndex, mWidth, mHeight, fileFps, mVideoProfile, mCompressionFactor);
  if (!mEncoder.Error().empty()) {
      throw std::runtime_error(mEncoder.Error());
  }
}

bool SaveToFile::StartRecording(const std::string & filename)
{
  switch (mState) {
  case Idle:
  {
      std::lock_guard<std::mutex> lock(mNextFileNameMutex);
      mNextFileName = filename;
  }
  mState = StartRecordingPending;
    return true;
  default:
    ;
  }
  return false;
}

bool SaveToFile::SwitchRecording(const std::string& nextFileName)
{
  switch (mState) {
  case Recording:
    {
      std::lock_guard<std::mutex> lock(mNextFileNameMutex);
      mNextFileName = nextFileName;
    }
    mState = StartRecordingPending;
    return true;
  default:
    ;
  }
  return false;
}

bool SaveToFile::StopRecording()
{
  switch (mState) {
  case Recording:
    mState = StopRecordingPending;
    return true;
  default:
    ;
  }
  return false;
}

void SaveToFile::SetVideoProperties(int width, int height, const Rate& fps)
{
    std::lock_guard<std::mutex> lock(mWriterMutex);
    mWidth = width;
    mHeight = height;
    mFps = fps;
}

bool SaveToFile::HandleVideo(const void* inpData)
{
  try {

    if (mState == StartRecordingPending)
    {
      mState = Recording;
      std::lock_guard<std::mutex> lock(mNextFileNameMutex);
      OnStartRecording(mNextFileName);
    }

    switch (mState.load()) {
    case Idle:
      break;
    case Recording:
      OnRecordVideo(inpData);
      break;
    case StopRecordingPending:
      OnRecordVideo(inpData);
      mState = Idle;
      OnStopRecording();
      break;
    default:
      Assert(false);
    }
  } catch(const std::exception& e) {
    mError = e.what();
    return false;
  }
  return true;
}

void SaveToFile::OnRecordVideo(const void* inpData)
{
  if (mVideoSampleCount % mDecimation) {
    ++mVideoSampleCount;
    return;
  }

  uint64_t frameNumber = mVideoSampleCount++ / mDecimation;
  if (mpWebcamClient)
    mpWebcamClient.load()->OnWebcamFrameNumber(frameNumber);

  std::lock_guard<std::mutex> lock(mWriterMutex);
  Time::Interval time = GetVideoFrameTimestamp();
  int bytes = mWidth * mHeight * 4;
  mEncoder.WriteFrame(mVideoStreamIndex, inpData, bytes, time);
  ++mWrittenVideoSamples;
}

void SaveToFile::OnStopRecording()
{
  std::lock_guard<std::mutex> lock(mWriterMutex);
  CloseFile();
}

void SaveToFile::OnStartRecording(const std::string& filename)
{
  std::lock_guard<std::mutex> lock(mWriterMutex);
  CloseFile();
  OpenFile(filename);
  mVideoSampleCount = 0;
  mWrittenVideoSamples = 0;
  mEncoder.BeginWriting();
  if (!mEncoder.Error().empty()) {
      throw std::runtime_error(mEncoder.Error());
  }
}

void SaveToFile::CloseFile()
{
  if (mVideoStreamIndex >= 0) {
      mEncoder.FinalizeStream(mVideoStreamIndex);
      mVideoStreamIndex = -1;
  }
  if (!mEncoder.Error().empty()) {
      throw std::runtime_error(mEncoder.Error());
  }

  mEncoder.CloseFile();
  if (!mEncoder.Error().empty()) {
      throw std::runtime_error(mEncoder.Error());
  }
}

Time::Interval SaveToFile::GetVideoFrameTimestamp()
{
    return mWrittenVideoSamples * mDecimation / mFps;
}

