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
#ifndef SAVE_TO_FILE_H
#define SAVE_TO_FILE_H

#include <string>
#include <sstream>
#include <mutex>
#include <atomic>

#include "AVEncoder.h"

class WebcamClient;

class SaveToFile
{
public:
  SaveToFile();
  ~SaveToFile();

  const std::string& Error() const;

  void SetClient(WebcamClient*);
  WebcamClient* Client() const;

  void SetDecimation(int);
  int Decimation() const;

  void SetVideoProfile(int);
  int VideoProfile() const;

  void SetCompressionFactor(int);
  int CompressionFactor() const;

  int CurrentFrameCount() const;

  bool StartRecording(const std::string&);
  bool SwitchRecording(const std::string&);
  bool StopRecording();

  void SetVideoProperties(int width, int height, const Rate& fps);

  bool HandleVideo(const void* data);

protected:
  void OnStartRecording(const std::string& filename);
  void OnRecordVideo(const void* data);
  void OnStopRecording();

private:
  void OpenFile(const std::string&);
  void CloseFile();

  Time::Interval GetVideoFrameTimestamp();

private:
  uint64_t mVideoSampleCount = 0, mWrittenVideoSamples = 0;

  enum {
    Idle = 0,
    StartRecordingPending,
    Recording,
    StopRecordingPending,
  };
  std::atomic<int> mState;
  std::atomic<WebcamClient*> mpWebcamClient = nullptr;
  std::atomic<int> mDecimation = 1;

  std::string mNextFileName;
  std::mutex mNextFileNameMutex;

  std::mutex mWriterMutex;
  int mVideoProfile = 1;
  int mCompressionFactor = 8;

  AVEncoder mEncoder;
  int mVideoStreamIndex = -1;
  int mWidth = 0, mHeight = 0;
  Rate mFps;

  std::string mError;
};

#endif // SAVE_TO_FILE_H
