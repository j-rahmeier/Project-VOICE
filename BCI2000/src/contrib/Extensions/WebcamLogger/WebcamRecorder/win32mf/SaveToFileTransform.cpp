////////////////////////////////////////////////////////////////////////////////
// $Id: SaveToFileTransform.cpp 8340 2024-09-09 13:57:34Z mellinger $
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
#include "SaveToFileTransform.h"
#include "ComStrings.h"
#include "Errors.h"
#include "FileUtils.h"
#include "Debugging.h"

#include <iostream>
#include <sstream>
#include <iomanip>

#include <codecapi.h>
#include <mfapi.h>

namespace {
  // MediaFoundation time stamps are in 100ns units
  const double MF_OneSecond = 1e7;
  const int cFontSize = 32;
  const int cWarningSamples = 100;

  time_t Now()
  {
    return ::time(0);
  }

  std::string TimeToString(time_t inTime)
  {
    struct ::tm* p = ::localtime(&inTime);
    if (!p)
      return "<n/a>";
    char buf[32];
    ::strftime(buf, sizeof(buf), "%d %b %y %T", p);
    return buf;
  }
}

SaveToFileTransform::SaveToFileTransform()
  : mVideoSampleCount(0), mAudioSampleCount(0),
  mWrittenVideoSamples(0), mWrittenAudioSamples(0),
  mDecimation(1), mVideoProfile(1),
  mTimestampMode(TimestampNone),
  mSinkWriterVideoStreamID(-1), mSinkWriterAudioStreamID(-1),
  mSinkWriterVideoStreamOpen(false), mSinkWriterAudioStreamOpen(false),
  mOverlayFont(NULL), mOffscreenBuffer(NULL)
{
  CreateOverlayFont();
}

SaveToFileTransform::~SaveToFileTransform()
{
  DeleteOffscreenBuffer();
  DeleteOverlayFont();
}

void SaveToFileTransform::SetTimestampMode(int mode)
{
  mTimestampMode = mode;
}

int SaveToFileTransform::TimestampMode() const
{
  return mTimestampMode;
}

void SaveToFileTransform::SetDecimation(int decimation)
{
  if (decimation < 1)
    decimation = 1;
  mDecimation = decimation;
}

int SaveToFileTransform::Decimation() const
{
  return mDecimation;
}

void SaveToFileTransform::SetVideoProfile(int profile)
{
  mVideoProfile = std::max(1, std::min(3, profile));
}

int SaveToFileTransform::VideoProfile() const
{
  return mVideoProfile;
}

HRESULT __stdcall SaveToFileTransform::SetInputType(
  DWORD dwInputStreamID,
  IMFMediaType* pType,
  DWORD dwFlags)
{
  HRESULT hr = GrabberTransform::SetInputType(dwInputStreamID, pType, dwFlags);
  if (hr == S_OK) {
    switch (dwInputStreamID)
    {
    case VideoStreamID:
      mVideoInputType = VideoMediaType(pType);
      CreateOffscreenBuffer();
      break;
    case AudioStreamID:
      mAudioInputType = AudioMediaType(pType);
      break;
    }
  }
  return hr;
}

bool SaveToFileTransform::OpenFile(const std::string& filename)
{
  std::string fullfile = FileUtils::StripExtension(filename) + ".mp4";
  if (FileUtils::Exists(fullfile)) {
    std::cout << "Output file \"" << fullfile << "\" exists, will not be touched." << std::endl;
    return false;
  }
  if (!FileUtils::IsWritableLocation(fullfile)) {
    std::cout << "Output file \"" << fullfile << "\" cannot be created at the given location." << std::endl;
    return false;
  }
  com::Ptr<IMFAttributes> pAttr;
  HRSucceedOrThrow hr = ::MFCreateAttributes(pAttr.Assignee(), 4);
  hr = pAttr->SetUINT32(MF_SINK_WRITER_DISABLE_THROTTLING, 1);
  hr = pAttr->SetUINT32(MF_READWRITE_DISABLE_CONVERTERS, 0);
  hr = pAttr->SetUINT32(MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS, TRUE);
  hr = pAttr->SetGUID(MF_TRANSCODE_CONTAINERTYPE, MFTranscodeContainerType_MPEG4);
  // Note that MFTranscodeContainerType_AVI is broken:
  // https://docs.microsoft.com/en-us/answers/questions/114418/mftranscodecontainertype-avi-not-creating-index.html

  hr = ::MFCreateSinkWriterFromURL(com::DualString(fullfile), nullptr, pAttr, mpSinkWriter.Assignee());

  mSinkWriterVideoStreamOpen = false;
  com::Ptr<IMFMediaType> pH264Type = mVideoInputType.Strip();
  hr = pH264Type->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264);
  hr = ::MFSetAttributeRatio(pH264Type, MF_MT_FRAME_RATE, mVideoInputType.framerateN, mVideoInputType.framerateD * mDecimation);
  UINT32 profile = 0;
  switch (mVideoProfile)
  {
    case 1:
      profile = eAVEncH264VProfile_Base;
      break;
    case 2:
      profile = eAVEncH264VProfile_Main;
      break;
    case 3:
      profile = eAVEncH264VProfile_High;
      break;
    default:
      profile = eAVEncH264VProfile_Base;
  }
  hr = pH264Type->SetUINT32(MF_MT_MPEG2_PROFILE, profile);
  UINT32 bitrate = 2.0 /* YUV420 */ * sizeof(uint8_t) * mVideoInputType.width * mVideoInputType.height * mVideoInputType.framerateN / mVideoInputType.framerateD / mDecimation;
  bitrate /= 4;
  hr = pH264Type->SetUINT32(MF_MT_AVG_BITRATE, bitrate);
  hr = mpSinkWriter->AddStream(pH264Type, &mSinkWriterVideoStreamID);
  mSinkWriterVideoStreamOpen = true;
  hr = mpSinkWriter->SetInputMediaType(mSinkWriterVideoStreamID, mVideoInputType.Strip(), nullptr);

  mSinkWriterAudioStreamOpen = false;
  com::Ptr<IMFMediaType> pAACType = mAudioInputType.Strip();
  if (pAACType)
  {
    hr = pAACType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_AAC);
    // According to MSDN, only the following byte rates are supported for the AAC encoder:
    // 12000, 16000, 20000, 24000 for 1 or 2 channels, and six times each for 6 channels.
    UINT32 byterate = 20000;
    hr = pAACType->SetUINT32(MF_MT_AUDIO_AVG_BYTES_PER_SECOND, byterate);
    hr = mpSinkWriter->AddStream(pAACType, &mSinkWriterAudioStreamID);
    mSinkWriterAudioStreamOpen = true;

    com::Ptr<IMFMediaType> pPCMType = mAudioInputType.Strip();
    hr = pPCMType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
    hr = mpSinkWriter->SetInputMediaType(mSinkWriterAudioStreamID, pPCMType, nullptr);
  }
  return true;
}

bool SaveToFileTransform::StartRecording(const std::string & filename)
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

bool SaveToFileTransform::SwitchRecording(const std::string& nextFileName)
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

bool SaveToFileTransform::StopRecording()
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

HRESULT SaveToFileTransform::OnProcess(DWORD streamID, com::Ptr<IMFSample> pSample)
{
  try {

    if (mState == StartRecordingPending)
    {
      mState = Recording;
      std::lock_guard<std::mutex> lock(mNextFileNameMutex);
      OnStartRecording(mNextFileName);
    }

    if (streamID == VideoStreamID)
      AddTimestamp(pSample);

    switch (mState.load()) {
    case Idle:
      break;
    case Recording:
      OnRecord(streamID, pSample);
      break;
    case StopRecordingPending:
      OnRecord(streamID, pSample);
      mState = Idle;
      OnStopRecording();
      break;
    default:
      Assert(false);
    }
  } catch(const std::exception& e) {
    std::string what = e.what();
    std::cout << what << std::endl;
    ::OutputDebugStringA((what + "\n").c_str());
  }
  return S_OK;
}

void SaveToFileTransform::OnRecord(DWORD streamID, IMFSample* pSample)
{
  switch (streamID)
  {
    case VideoStreamID:
      if (mVideoSampleCount == 0) {
        mBegin = TimeUtils::MonotonicTime();
      }
      else if (mVideoSampleCount == cWarningSamples) {
        Time now = TimeUtils::MonotonicTime();
        double fps = mVideoInputType.framerateN * 1.0 / mVideoInputType.framerateD;
        double ratio = fps * ((now - mBegin) / Time::OneSecond()).To<double>() / mVideoSampleCount;
        if (1){//ratio < 0.9 || ratio > 1.1) {
            double factor = std::round(10.0 / ratio) / 10.0;
            std::cerr << "Warning: Actual webcam frame rate deviates from nominal (" << fps << "fps) "
                      << "by a factor of " << factor << std::endl;
        }
      }
      OnRecordVideo(pSample);
      break;
    case AudioStreamID:
      OnRecordAudio(pSample);
      break;
  }
}

void SaveToFileTransform::OnRecordVideo(IMFSample* pSample)
{
  if (mVideoSampleCount % mDecimation) {
    ++mVideoSampleCount;
    return;
  }

  std::clog << mVideoSampleCount++ / mDecimation << std::endl;

  std::lock_guard<std::mutex> lock(mWriterMutex);

  com::Ptr<IMFSample> pDuplicatedSample;
  HRSucceedOrThrow hr = ::MFCreateSample(pDuplicatedSample.Assignee());

  com::Ptr<IMFAttributes> pSourceAttr, pDestAttr;
  hr = pSample->QueryInterface(IID_IMFAttributes, pSourceAttr.Assignee());
  hr = pDuplicatedSample->QueryInterface(IID_IMFAttributes, pDestAttr.Assignee());
  hr = pSourceAttr->CopyAllItems(pDestAttr);

  LONGLONG duration = GetVideoFrameDuration();
  hr = pDuplicatedSample->SetSampleDuration(duration);

  LONGLONG time = GetVideoFrameTimestamp();
  hr = pDuplicatedSample->SetSampleTime(time);
  if (mVideoSampleCount == 1)
    pDuplicatedSample->SetUINT32(MFSampleExtension_Discontinuity, TRUE);

  DWORD flags = 0;
  hr = pSample->GetSampleFlags(&flags);
  hr = pDuplicatedSample->SetSampleFlags(flags);
  DWORD count = 0;
  hr = pSample->GetBufferCount(&count);
  Assert(count == 1);
  for (int i = 0; i < count; ++i) {
    com::Ptr<IMFMediaBuffer> pBuffer;
    hr = pSample->GetBufferByIndex(i, pBuffer.Assignee());

    INT32 frameStride = 0;
    hr = mVideoInputType.mpType->GetUINT32(MF_MT_DEFAULT_STRIDE, reinterpret_cast<UINT32*>(&frameStride));
    if (frameStride < 0) // No flip necessary.
    {
        hr = pDuplicatedSample->AddBuffer(pBuffer);
    }
    else // Flip vertically to match expected input of H.264 codec.
    {
        BYTE* pData = nullptr;
        DWORD currentLength = 0;
        pBuffer->Lock(&pData, nullptr, &currentLength);
        com::Ptr<IMFMediaBuffer> pDuplicatedBuffer;
        hr = ::MFCreateMemoryBuffer(currentLength, pDuplicatedBuffer.Assignee());
        hr = pDuplicatedBuffer->SetCurrentLength(currentLength);
        BYTE* pDuplicatedData = nullptr;
        hr = pDuplicatedBuffer->Lock(&pDuplicatedData, nullptr, &currentLength);
        UINT32 width = 0, height = 0;
        hr = ::MFGetAttributeSize(mVideoInputType.mpType, MF_MT_FRAME_SIZE, &width, &height);
        hr = ::MFCopyImage(pDuplicatedData + frameStride * (height - 1), -frameStride,
          pData, frameStride, width * sizeof(RGBQUAD), height);
        hr = pDuplicatedBuffer->Unlock();
        hr = pBuffer->Unlock();
        hr = pDuplicatedSample->AddBuffer(pDuplicatedBuffer);
    }
  }
  hr = mpSinkWriter->WriteSample(mSinkWriterVideoStreamID, pDuplicatedSample);
  ++mWrittenVideoSamples;
}

void SaveToFileTransform::OnRecordAudio(IMFSample* pSample)
{
  std::lock_guard<std::mutex> lock(mWriterMutex);

  LONGLONG time = 0;
  HRSucceedOrThrow hr = pSample->GetSampleTime(&time);

  com::Ptr<IMFSample> pDuplicatedSample;
  hr = ::MFCreateSample(pDuplicatedSample.Assignee());

  com::Ptr<IMFAttributes> pSourceAttr, pDestAttr;
  hr = pSample->QueryInterface(IID_IMFAttributes, pSourceAttr.Assignee());
  hr = pDuplicatedSample->QueryInterface(IID_IMFAttributes, pDestAttr.Assignee());
  hr = pSourceAttr->CopyAllItems(pDestAttr);

  if (mAudioSampleCount++ < 1)
    pDuplicatedSample->SetUINT32(MFSampleExtension_Discontinuity, TRUE);
  DWORD flags = 0;
  hr = pSample->GetSampleFlags(&flags);
  hr = pDuplicatedSample->SetSampleFlags(flags);
  DWORD count = 0, totalLengthBytes = 0;
  hr = pSample->GetBufferCount(&count);
  for (int i = 0; i < count; ++i) {
    com::Ptr<IMFMediaBuffer> pBuffer;
    hr = pSample->GetBufferByIndex(i, pBuffer.Assignee());
    DWORD length = 0;
    hr = pBuffer->GetCurrentLength(&length);
    totalLengthBytes += length;
    union { BYTE* b; int16_t* s; } pData = { nullptr };
    hr = pDuplicatedSample->AddBuffer(pBuffer);
  }
  int numAudioFrames = totalLengthBytes * 8 / (mAudioInputType.bitspersample * mAudioInputType.channels);
  LONGLONG duration = numAudioFrames * MF_OneSecond / mAudioInputType.samplerate;
  hr = pDuplicatedSample->SetSampleDuration(duration);

  LONGLONG timestamp = mWrittenAudioFrames * MF_OneSecond / mAudioInputType.samplerate;
  hr = pDuplicatedSample->SetSampleTime(timestamp);

  hr = mpSinkWriter->WriteSample(mSinkWriterAudioStreamID, pDuplicatedSample);
  ++mWrittenAudioSamples;
  mWrittenAudioFrames += numAudioFrames;
}

void SaveToFileTransform::OnStopRecording()
{
  std::clog << "end" << std::endl;
  std::lock_guard<std::mutex> lock(mWriterMutex);
  CloseFile();
}

void SaveToFileTransform::OnStartRecording(const std::string& filename)
{
  std::lock_guard<std::mutex> lock(mWriterMutex);
  CloseFile();
  if (!OpenFile(filename))
    throw std::runtime_error("Could not open \"" + filename + "\" for writing");
  mVideoSampleCount = 0;
  mWrittenVideoSamples = 0;
  mAudioSampleCount = 0;
  mWrittenAudioSamples = 0;
  mWrittenAudioFrames = 0;
  HRSucceedOrThrow hr = mpSinkWriter->BeginWriting();
}

void SaveToFileTransform::CloseFile()
{
  // Don't call mpSinkWriter->Flush() here as it drops all outstanding samples.
  // Also, it may result in mpSinkWriter->Finalize() hanging indefinitely:
  // https://stackoverflow.com/questions/33767688
  mSinkWriterVideoStreamOpen = false;
  mSinkWriterAudioStreamOpen = false;
  if (mpSinkWriter)
  {
    if (mWrittenVideoSamples > 0 || mWrittenAudioSamples > 0)
      HRSucceedOrThrow hr = mpSinkWriter->Finalize();
    mpSinkWriter = nullptr;
  }
}

LONGLONG SaveToFileTransform::GetVideoFrameDuration()
{
    return mVideoInputType.framerateD * mDecimation * MF_OneSecond / mVideoInputType.framerateN;
}

LONGLONG SaveToFileTransform::GetVideoFrameTimestamp()
{
    return mVideoInputType.framerateD * mDecimation * MF_OneSecond * mWrittenVideoSamples / mVideoInputType.framerateN;
}

void SaveToFileTransform::AddTimestamp(IMFSample* pSample)
{
  if (mTimestampMode == TimestampNone)
    return;

  DWORD count = 0;
  HRSucceedOrThrow hr = pSample->GetBufferCount(&count);
  if (count > 0) {
    com::Ptr<IMFMediaBuffer> pBuffer;
    hr = pSample->GetBufferByIndex(0, pBuffer.Assignee());
    BYTE* pData = nullptr;
    DWORD currentLength = 0;
    hr = pBuffer->Lock(&pData, nullptr, &currentLength);

    UINT32 frameStride = 0;
    hr = mVideoInputType.mpType->GetUINT32(MF_MT_DEFAULT_STRIDE, &frameStride);
    int width = mVideoInputType.width, height = mVideoInputType.height;
    int bitmapStride = width * sizeof(RGBQUAD);

    // Copy data from buffer into bitmap. Negative stride accounts for vertical flip.
    // Possible optimization: Only copy area used by time stamp.
    hr = ::MFCopyImage(mpOffscreenBufferMemory + bitmapStride * (height - 1), -bitmapStride, 
      pData, frameStride, width * sizeof(RGBQUAD), height);

    RECT rect;
    rect.top = 0;
    rect.bottom = height;
    rect.left = 0;
    rect.right = width;

    int flags = DT_NOCLIP | DT_SINGLELINE;
    switch (mTimestampMode) {
    case TimestampTopLeft:
      flags |= DT_LEFT | DT_TOP;
      break;
    case TimestampTopRight:
    case FrameCounter:
      flags |= DT_RIGHT | DT_TOP;
      break;
    case TimestampBottomLeft:
      flags |= DT_LEFT | DT_BOTTOM;
      break;
    case TimestampBottomRight:
      flags |= DT_RIGHT | DT_BOTTOM;
      break;
    }

    std::string time;
    if (mTimestampMode == FrameCounter)
        time = std::to_string(mVideoSampleCount / mDecimation);
    else
        time = TimeToString(Now());

    HDC dc = ::GetDC(NULL);
    HDC memDC = ::CreateCompatibleDC(dc);
    ::ReleaseDC(NULL, dc);
    HGDIOBJ oldBitmap = ::SelectObject(memDC, mOffscreenBuffer);

    HGDIOBJ oldFont = ::SelectObject(memDC, mOverlayFont);
    ::SetTextColor(memDC, RGB(0, 255, 0));
    ::SetBkMode(memDC, TRANSPARENT);
    ::DrawTextA(memDC, time.c_str(), -1, &rect, flags);
    ::SelectObject(memDC, oldFont);

    ::SelectObject(memDC, oldBitmap);
    ::DeleteDC(memDC);

    // Copy data from bitmap back into buffer.
    hr = ::MFCopyImage(pData, frameStride,
      mpOffscreenBufferMemory + bitmapStride * (height - 1), -bitmapStride, bitmapStride, height);

    hr = pBuffer->Unlock();
  }
}

void SaveToFileTransform::CreateOffscreenBuffer()
{
  DeleteOffscreenBuffer();

  BITMAPV5HEADER bi = { 0 };
  bi.bV5Size = sizeof(BITMAPV5HEADER);
  bi.bV5Width = mVideoInputType.width;
  bi.bV5Height = mVideoInputType.height;
  bi.bV5Planes = 1;
  bi.bV5BitCount = 32;
  bi.bV5Compression = BI_RGB;
  bi.bV5RedMask = 0x00FF0000;
  bi.bV5GreenMask = 0x0000FF00;
  bi.bV5BlueMask = 0x000000FF;
  bi.bV5AlphaMask = 0xFF000000;

  HDC dc = ::GetDC(NULL);
  mOffscreenBuffer = ::CreateDIBSection(dc, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS,
    reinterpret_cast<void**>(&mpOffscreenBufferMemory), NULL, 0);
  ::ReleaseDC(NULL, dc);
}

void SaveToFileTransform::DeleteOffscreenBuffer()
{
  if (mOffscreenBuffer) {
    ::DeleteObject(mOffscreenBuffer);
    mOffscreenBuffer = NULL;
    mpOffscreenBufferMemory = nullptr;
  }
}

void SaveToFileTransform::CreateOverlayFont()
{
  DeleteOverlayFont();
  mOverlayFont = ::CreateFontA(cFontSize, 0, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE,
    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
    FIXED_PITCH | FF_MODERN, nullptr);
}

void SaveToFileTransform::DeleteOverlayFont()
{
  if (mOverlayFont) {
    ::DeleteObject(mOverlayFont);
    mOverlayFont = NULL;
  }
}
