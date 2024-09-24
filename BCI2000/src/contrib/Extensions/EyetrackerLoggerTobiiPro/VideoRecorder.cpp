////////////////////////////////////////////////////////////////////////////////
// $Id: VideoRecorder.cpp 7743 2023-12-05 20:05:31Z mellinger $
// Author: mellinger@neurotechcenter.com
// Description: A class that displays video frames in a window, and records
//   to a file.
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
#include "VideoRecorder.h"
#include "SysError.h"
#include "ComPtr.h"
#include "ComStrings.h"
#include "Ratio.h"
#include "FileUtils.h"
#include "Debugging.h"
#include "WaitableEvent.h"

#include "Window.h"
#include "EventLoop.h"
#include "IniFile.h"

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <codecapi.h>

#include <mutex>
#include <thread>

namespace {
  DWORD CreateOffscreenBuffer(int width, int height, HBITMAP* pHBitmap, BYTE** ppMemory)
  {
    BITMAPV5HEADER bi = { 0 };
    bi.bV5Size = sizeof(BITMAPV5HEADER);
    bi.bV5Width = width;
    bi.bV5Height = height;
    bi.bV5Planes = 1;
    bi.bV5BitCount = 32;
    bi.bV5Compression = BI_RGB;
    bi.bV5RedMask = 0x00FF0000;
    bi.bV5GreenMask = 0x0000FF00;
    bi.bV5BlueMask = 0x000000FF;
    bi.bV5AlphaMask = 0xFF000000;

    HDC dc = ::GetDC(NULL);
    *pHBitmap = ::CreateDIBSection(dc, reinterpret_cast<BITMAPINFO*>(&bi),
      DIB_RGB_COLORS, reinterpret_cast<void**>(ppMemory), NULL, 0);
    int err = *pHBitmap ? NO_ERROR : ::GetLastError();
    ::ReleaseDC(NULL, dc);
    return err;
  }

  // The video window is running in its own detached thread, which is terminated
  // when calling Close() on the window.
  class VideoWindow : public Window
  {
    public:
      ~VideoWindow();
      void SetContent(int width, int height, BYTE* data);
      static VideoWindow* VideoWindow::CreateWindowThread(int cameraId, int left, int top, int width, int height);
      void WaitForWindowThread();
    protected:
      void WindowThreadDone();
      void OnCreate() override;
      void OnPaint() override;
      bool OnPreTranslate(MSG*) override;
    private:
      std::mutex mMutex;
      int mContentWidth = 0, mContentHeight = 0;
      HBITMAP mBitmap = NULL;
      BYTE* mpData = nullptr;
      WaitableEvent mDone;

      static std::mutex sIniFileMutex;
  };

  std::mutex VideoWindow::sIniFileMutex;

  VideoWindow::~VideoWindow()
  {
    if (mBitmap)
      ::DeleteObject(mBitmap);
  }

  void VideoWindow::WindowThreadDone()
  {
    mDone.Set();
  }

  void VideoWindow::OnCreate()
  {
    HMENU hSysMenu = ::GetSystemMenu(Handle(), FALSE);
    if (hSysMenu)
      ::EnableMenuItem(hSysMenu, SC_CLOSE, (MF_DISABLED | MF_GRAYED | MF_BYCOMMAND));
  }

  bool VideoWindow::OnPreTranslate(MSG* pMsg)
  {
    if (pMsg->message == WM_SYSKEYDOWN) {
      if (pMsg->wParam == VK_F4)
        return true;
    }
    return Window::OnPreTranslate(pMsg);
  }

  void VideoWindow::SetContent(int width, int height, BYTE* pData)
  {
    std::lock_guard<std::mutex> lock(mMutex);
    if (width != mContentWidth || height != mContentHeight)
    {
      if (mBitmap)
        ::DeleteObject(mBitmap);
      DWORD err = CreateOffscreenBuffer(width, height, &mBitmap, &mpData);
      if (err)
        throw std::runtime_error(SysError(err).Message());
      mContentWidth = width;
      mContentHeight = height;
    }
    int size = width * height * sizeof(RGBQUAD);
    ::memcpy(mpData, pData, size);
    Invalidate();
  }

  void VideoWindow::OnPaint()
  {
    PAINTSTRUCT ps = {0};
    HDC dc = ::BeginPaint(Handle(), &ps);
    if (mBitmap)
    {
      HDC dc2 = ::CreateCompatibleDC(dc);
      HGDIOBJ originalBitmap = ::SelectObject(dc2, mBitmap);
      ::StretchBlt(dc, 0, 0, Width(), Height(), dc2, 0, 0, mContentWidth, mContentHeight, SRCCOPY);
      ::SelectObject(dc2, originalBitmap);
      ::DeleteDC(dc2);
    }
    else
    {
      RECT rect = { 0, 0, Width(), Height() };
      HBRUSH brush = reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));
      ::FillRect(dc, &rect, brush);
    }
    ::EndPaint(Handle(), &ps);
  }

  VideoWindow* VideoWindow::CreateWindowThread(int cameraId, int left, int top, int width, int height)
  {
    VideoWindow* pWindow = nullptr;
    std::mutex mutex;
    std::condition_variable cond;
    std::thread windowThread(
      [&pWindow, &mutex, &cond, cameraId, left, top, width, height]()
      {
        VideoWindow window;
        window.Handle();
        {
          std::lock_guard<std::mutex> lock(mutex);
          pWindow = &window;
          cond.notify_all();
        }
        // From here, the stack frame of the caller will be invalid.
        std::string iniFilePath = FileUtils::ExtractDirectory(FileUtils::ExecutablePath());
        iniFilePath += "EyetrackerLogger.ini";
        {
          std::lock_guard<std::mutex> lock(VideoWindow::sIniFileMutex);
          IniFile iniFile(iniFilePath);
          auto& geom = iniFile["Geometry" + std::to_string(cameraId)];
          if (geom.Empty()) {
            window.SetLeft(left).SetTop(top);
            window.SetWidth(width).SetHeight(height);
          }
          else {
            window.SetLeft(geom["Left"]).SetTop(geom["Top"]);
            window.SetWidth(geom["Width"]).SetHeight(geom["Height"]);
          }
        }

        EventLoop::Run();

        {
          std::lock_guard<std::mutex> lock(VideoWindow::sIniFileMutex);
          IniFile iniFile(iniFilePath);
          auto& geom = iniFile["Geometry" + std::to_string(cameraId)];
          geom["Left"] = window.Left();
          geom["Top"] = window.Top();
          geom["Width"] = window.Width();
          geom["Height"] = window.Height();
        }
        window.WindowThreadDone();
      }
    );
    // Wait until window has been created:
    {
      std::unique_lock<std::mutex> lock(mutex);
      cond.wait(lock, [&] { return pWindow != nullptr; });
    }
    windowThread.detach();
    return pWindow;
  }
  void VideoWindow::WaitForWindowThread()
  {
    mDone.Wait();
  }
}

struct VideoRecorder::Private
{
  std::mutex mMutex;
  int mId = 0, mHeight = 0, mWidth = 0;
  std::atomic<bool> mBlank{ false };
  float mFramesPerSecond = 0;
  int64_t mFrameCount = 0;
  std::string mName, mError;

  HBITMAP mOffscreenBuffer = NULL;
  BYTE* mpOffscreenBufferMemory = nullptr;

  com::Ptr<IMFSinkWriter> mpSinkWriter;
  DWORD mStreamIndex = 0;

  VideoWindow* mpVideoWindow = nullptr;

  Private();
  ~Private();
  com::Ptr<IMFMediaType> createInputType();
  com::Ptr<IMFMediaType> createOutputType();
};

VideoRecorder::Private::Private()
{
  HRESULT hr = ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
  if (!SUCCEEDED(hr))
  {
    mError = "Could not initialize COM: ";
    mError += SysError(hr).Message();
  }
  else
  {
    hr = ::MFStartup(MF_VERSION);
    if (!SUCCEEDED(hr))
    {
        mError = "Could not initialize Media Foundation: ";
        mError += SysError(hr).Message();
    }
  }
}

VideoRecorder::Private::~Private()
{
  mpSinkWriter = nullptr;
  ::MFShutdown();
  ::CoUninitialize();
}

com::Ptr<IMFMediaType> VideoRecorder::Private::createInputType()
{
  com::Ptr<IMFMediaType> pType;
  HRESULT hr = ::MFCreateMediaType(pType.Assignee());
  if (SUCCEEDED(hr))
    hr = pType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
  if (SUCCEEDED(hr))
    hr = pType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);
  if (SUCCEEDED(hr))
    hr = pType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
  if (SUCCEEDED(hr))
    hr = ::MFSetAttributeSize(pType, MF_MT_FRAME_SIZE, mWidth, mHeight);
  Ratio fps = Ratio::FromDouble(mFramesPerSecond).Simplified(1e-3);
  if (SUCCEEDED(hr))
    hr = ::MFSetAttributeRatio(pType, MF_MT_FRAME_RATE, fps.Numerator(), fps.Denominator());
  if (SUCCEEDED(hr))
    hr = ::MFSetAttributeRatio(pType, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);
  if (!SUCCEEDED(hr))
    pType = nullptr;
  return pType;
}


com::Ptr<IMFMediaType> VideoRecorder::Private::createOutputType()
{
  com::Ptr<IMFMediaType> pType = createInputType();
  if (!pType)
    return pType;

  HRESULT hr = pType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264);
  if (SUCCEEDED(hr))
    hr = pType->SetUINT32(MF_MT_MPEG2_PROFILE, eAVEncH264VProfile_Base);
  // raw bitrate
  UINT32 bitrate = mWidth * mHeight * mFramesPerSecond * cBitsPerPixel;
  // desired bitrate (some trial-and-error compression ratio)
  bitrate *= 0.1;
  if (SUCCEEDED(hr))
    hr = pType->SetUINT32(MF_MT_AVG_BITRATE, bitrate);
  if (!SUCCEEDED(hr))
    pType = nullptr;
  return pType;
}

VideoRecorder::VideoRecorder()
: p(new Private)
{
}

VideoRecorder::~VideoRecorder()
{
  {
    std::lock_guard<std::mutex> lock(p->mMutex);
    if (p->mpVideoWindow)
      p->mpVideoWindow->Close();
  }
  delete p;
}

VideoRecorder& VideoRecorder::setName(const std::string& name)
{
  std::lock_guard<std::mutex> lock(p->mMutex);
  p->mName = name;
  if (p->mpVideoWindow)
    p->mpVideoWindow->SetTitle(name);
  return *this;
}

const std::string& VideoRecorder::name() const
{
  return p->mName;
}

VideoRecorder& VideoRecorder::setId(int id)
{
  std::lock_guard<std::mutex> lock(p->mMutex);
  p->mId = id;
  return *this;
}

int VideoRecorder::id() const
{
  std::lock_guard<std::mutex> lock(p->mMutex);
  return p->mId;
}

VideoRecorder& VideoRecorder::setHeight(int height)
{
  std::lock_guard<std::mutex> lock(p->mMutex);
  p->mHeight = height;
  return *this;
}

int VideoRecorder::height() const
{
  std::lock_guard<std::mutex> lock(p->mMutex);
  return p->mHeight;
}

VideoRecorder& VideoRecorder::setWidth(int width)
{
  std::lock_guard<std::mutex> lock(p->mMutex);
  p->mWidth = width;
  return *this;
}

int VideoRecorder::width() const
{
  std::lock_guard<std::mutex> lock(p->mMutex);
  return p->mWidth;
}

VideoRecorder& VideoRecorder::setFramesPerSecond(float fps)
{
  std::lock_guard<std::mutex> lock(p->mMutex);
  p->mFramesPerSecond = fps;
  return *this;
}

float VideoRecorder::framesPerSecond() const
{
  std::lock_guard<std::mutex> lock(p->mMutex);
  return p->mFramesPerSecond;
}

VideoRecorder& VideoRecorder::startRecording(const std::string& file)
{
  std::lock_guard<std::mutex> lock(p->mMutex);

  p->mFrameCount = 0;
  Assert(p->mHeight > 0 && p->mWidth > 0);

  com::Ptr<IMFAttributes> pAttr;
  HRESULT hr = ::MFCreateAttributes(pAttr.Assignee(), 4);
  if (SUCCEEDED(hr))
    hr = pAttr->SetUINT32(MF_SINK_WRITER_DISABLE_THROTTLING, 1);
  if (SUCCEEDED(hr))
    hr = pAttr->SetUINT32(MF_READWRITE_DISABLE_CONVERTERS, 0);
  if (SUCCEEDED(hr))
    hr = pAttr->SetUINT32(MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS, TRUE);
  if (SUCCEEDED(hr))
    hr = pAttr->SetGUID(MF_TRANSCODE_CONTAINERTYPE, MFTranscodeContainerType_MPEG4);
  // Note that MFTranscodeContainerType_AVI is broken:
  // https://docs.microsoft.com/en-us/answers/questions/114418/mftranscodecontainertype-avi-not-creating-index.html
  if (SUCCEEDED(hr))
    hr = ::MFCreateSinkWriterFromURL(com::DualString(file), nullptr, pAttr, p->mpSinkWriter.Assignee());
  if (!SUCCEEDED(hr))
  {
    p->mError = "Could not create sink writer: ";
    p->mError += SysError(hr).Message();
    return *this;
  }
  com::Ptr<IMFMediaType> pOutputType = p->createOutputType(),
                         pInputType = p->createInputType();
  if (pInputType && pOutputType)
  {
    hr = p->mpSinkWriter->AddStream(pOutputType, &p->mStreamIndex);
    if (!SUCCEEDED(hr))
    {
      p->mError = "Could not add stream: ";
      p->mError += SysError(hr).Message();
      return *this;
    }
    hr = p->mpSinkWriter->SetInputMediaType(p->mStreamIndex, pInputType, nullptr);
    if (!SUCCEEDED(hr))
    {
      p->mError = "Could not set input media type: ";
      p->mError += SysError(hr).Message();
      return *this;
    }
    hr = p->mpSinkWriter->BeginWriting();
    if (!SUCCEEDED(hr))
    {
      p->mError = "Could not begin writing: ";
      p->mError += SysError(hr).Message();
      return *this;
    }
  }
  return *this;
}

VideoRecorder& VideoRecorder::stopRecording()
{
  std::lock_guard<std::mutex> lock(p->mMutex);
  if (p->mpSinkWriter)
  {
    if (p->mFrameCount > 0)
    {
      HRESULT hr = p->mpSinkWriter->Finalize();
      if (!SUCCEEDED(hr))
      {
        p->mError = "Could not finalize output sink: ";
        p->mError += SysError(hr).Message();
      }
    }
    p->mpSinkWriter = nullptr;
  }
  return *this;
}

bool VideoRecorder::isRecording() const
{
  std::lock_guard<std::mutex> lock(p->mMutex);
  return p->mpSinkWriter;
}

VideoRecorder& VideoRecorder::setBlank(const Presentation* pPres, bool b)
{
  if (b && !p->mBlank)
    writeBlackFrame(pPres);
  p->mBlank = b;
  return *this;
}

VideoRecorder& VideoRecorder::writeBlackFrame(const Presentation* pPresentation)
{
  int bufferSize = p->mWidth * p->mHeight * sizeof(RGBQUAD);
  BYTE* pData = new BYTE[bufferSize];
  ::memset(pData, 0, bufferSize);
  writeFrame(pPresentation, pData, false);
  delete[] pData;
  return *this;
}

VideoRecorder& VideoRecorder::writeFrame(const Presentation* pPresentation, void* pData, bool autoBlack)
{
  if (autoBlack && p->mBlank)
    return writeBlackFrame(pPresentation);

  std::lock_guard<std::mutex> lock(p->mMutex);
  p->mpVideoWindow->SetContent(p->mWidth, p->mHeight, static_cast<BYTE*>(pData));

  if (!p->mpSinkWriter)
    return *this;

  // Put image into an MFMediaBuffer object.
  com::Ptr<IMFMediaBuffer> pMediaBuffer;
  int bufferSize = p->mWidth * p->mHeight * sizeof(RGBQUAD);
  HRESULT hr = ::MFCreateMemoryBuffer(bufferSize, pMediaBuffer.Assignee());
  BYTE* pMediaBufferData = nullptr;
  if (SUCCEEDED(hr))
  {
    hr = pMediaBuffer->Lock(&pMediaBufferData, nullptr, nullptr);
  }
  if (SUCCEEDED(hr))
  {
    int stride = p->mWidth * sizeof(RGBQUAD);
    hr = ::MFCopyImage(pMediaBufferData, stride, static_cast<BYTE*>(pData), stride, stride, p->mHeight);
  }
  if (pMediaBuffer)
    pMediaBuffer->Unlock();
  hr = pMediaBuffer->SetCurrentLength(bufferSize);

  float frameDurationMs = 1e3 / p->mFramesPerSecond;
  float framePositionMs = 1e3 * p->mFrameCount / p->mFramesPerSecond;

  // Wrap the MFMediaBuffer object into an MFSample object.
  com::Ptr<IMFSample> pSample;
  if (SUCCEEDED(hr))
    hr = ::MFCreateSample(pSample.Assignee());
  if (SUCCEEDED(hr))
    hr = pSample->AddBuffer(pMediaBuffer);
  if (SUCCEEDED(hr))
    hr = pSample->SetSampleTime(std::round(framePositionMs * 10000));
  if (SUCCEEDED(hr))
    hr = pSample->SetSampleDuration(std::round(frameDurationMs * 10000));

  // Send the sample object to the sink.
  if (SUCCEEDED(hr))
  {
      hr = p->mpSinkWriter->WriteSample(p->mStreamIndex, pSample);
      ++p->mFrameCount;
  }
  return *this;
}

VideoRecorder& VideoRecorder::showWindow()
{
  std::lock_guard<std::mutex> lock(p->mMutex);
  if (!p->mpVideoWindow)
    p->mpVideoWindow = VideoWindow::CreateWindowThread(p->mId, 100 + 10 * p->mId, 100 + 10 * p->mId, p->mWidth, p->mHeight);
  p->mpVideoWindow->SetTitle(p->mName);
  p->mpVideoWindow->Show();
  return *this;
}

VideoRecorder& VideoRecorder::hideWindow()
{
  std::lock_guard<std::mutex> lock(p->mMutex);
  if (p->mpVideoWindow)
    p->mpVideoWindow->Hide();
  return *this;
}

std::string VideoRecorder::error() const
{
  std::lock_guard<std::mutex> lock(p->mMutex);
  return p->mError;
}
