////////////////////////////////////////////////////////////////////////////////
// $Id: SaveToFileTransform.h 8340 2024-09-09 13:57:34Z mellinger $
// Author: mellinger@neurotechcenter.com
// Description: A GrabberTransform descendant that saves grabbed content to an 
//   H264 encoded video file, with optional time stamp.
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
#ifndef SAVE_TO_FILE_TRANSFORM_H
#define SAVE_TO_FILE_TRANSFORM_H

#include <mfidl.h>
#include <mfreadwrite.h>
#include <string>
#include <mutex>

#include "GrabberTransform.h"
#include "VideoMediaType.h"
#include "AudioMediaType.h"
#include "ComPtr.h"
#include "TimeUtils.h"

class SaveToFileTransform : public GrabberTransform
{
public:
  SaveToFileTransform();
  ~SaveToFileTransform();

  enum {
    TimestampNone,
    TimestampTopRight,
    TimestampTopLeft,
    TimestampBottomRight,
    TimestampBottomLeft,
    FrameCounter,
  };
  void SetTimestampMode(int);
  int TimestampMode() const;

  void SetDecimation(int);
  int Decimation() const;

  void SetVideoProfile(int);
  int VideoProfile() const;

  bool StartRecording(const std::string&);
  bool SwitchRecording(const std::string&);
  bool StopRecording();

  enum {
    VideoStreamID = 0,
    AudioStreamID = 1,
  };

protected:
  HRESULT __stdcall SetInputType(
    DWORD dwInputStreamID,
    IMFMediaType* pType,
    DWORD dwFlags) override;

  HRESULT OnProcess(DWORD streamID, com::Ptr<IMFSample> pSample) override;
  void AddTimestamp(IMFSample*);
  void OnStartRecording(const std::string& filename);
  void OnRecord(DWORD streamID, IMFSample*);
  void OnRecordVideo(IMFSample*);
  void OnRecordAudio(IMFSample*);
  void OnStopRecording();

private:
  void CreateOffscreenBuffer();
  void DeleteOffscreenBuffer();
  void CreateOverlayFont();
  void DeleteOverlayFont();

  bool OpenFile(const std::string&);
  void CloseFile();

  LONGLONG GetVideoFrameDuration();
  LONGLONG GetVideoFrameTimestamp();

private:
  uint64_t mVideoSampleCount, mAudioSampleCount,
           mWrittenVideoSamples, mWrittenAudioSamples, mWrittenAudioFrames;
  Time mBegin;

  enum {
    Idle = 0,
    StartRecordingPending,
    Recording,
    StopRecordingPending,
  };
  std::atomic<int> mState;
  std::atomic<int> mTimestampMode;
  std::atomic<int> mDecimation;

  std::string mNextFileName;
  std::mutex mNextFileNameMutex;

  std::mutex mWriterMutex;
  com::Ptr<IMFSinkWriter> mpSinkWriter;
  int mVideoProfile;
  DWORD mSinkWriterVideoStreamID, mSinkWriterAudioStreamID;
  bool mSinkWriterVideoStreamOpen, mSinkWriterAudioStreamOpen;
  VideoMediaType mVideoInputType;
  AudioMediaType mAudioInputType;
  HFONT mOverlayFont;
  HGDIOBJ mOffscreenBuffer;
  BYTE* mpOffscreenBufferMemory;
};

#endif // SAVE_TO_FILE_TRANSFORM_H
