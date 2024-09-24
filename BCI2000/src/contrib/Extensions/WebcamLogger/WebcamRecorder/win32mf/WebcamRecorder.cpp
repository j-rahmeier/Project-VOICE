////////////////////////////////////////////////////////////////////////////////
// $Id: WebcamRecorder.cpp 7882 2024-03-01 20:34:21Z mellinger $
// Author: mellinger@neurotechcenter.com
// Description: A Win32 application that records video from a camera,
//   optionally saving to file and/or displaying in a window.
//   Dynamic application settings are controlled through stdio.
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
#include <Windows.h>

#include "EventLoop.h"
#include "Window.h"
#include "WebcamPipeline.h"
#include "GrabberTransform.h"
#include "SaveToFileTransform.h"

#include "Exception.h"
#include "ProcessUtils.h"
#include "VersionInfo.h"
#include "IniFile.h"
#include "FileUtils.h"
#include "SysError.h"
#include "ExceptionCatcher.h"
#include "Errors.h"
#include "MFInit.h"

#include <iostream>
#include <thread>

struct ExceptionCatcher_: ExceptionCatcher
{
  void OnReportException(const std::string& msg) override
  {
    ::MessageBoxA(NULL, msg.c_str(), "Exception Caught", MB_OK);
  }
};

static int WebcamRecorderMain(int argc, char** argv);

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int nShowCmd)
{
  int argc = __argc;
  char** argv = __argv;

  ProcessUtils::WindowsEnableStdio(argc, argv);
  ProcessUtils::InitializeDesktopIntegration("org.bci2000.OnlineModules");

  FunctionCall<int(int, char**)> mainCall(&WebcamRecorderMain, argc, argv);
  if (ExceptionCatcher_().Run(mainCall))
    return mainCall.Result();
  return -1;
}

void IOThreadFunc(HWND window, SaveToFileTransform* pSaveToFile)
{
  try {
    std::string line;
    while (std::getline(std::cin, line)) {
      if (line.find("record ") == 0) {
        pSaveToFile->StartRecording(line.substr(7));
      }
      else if (line.find("switchto ") == 0) {
        pSaveToFile->SwitchRecording(line.substr(9));
      }
      else if (line == "stop") {
        pSaveToFile->StopRecording();
      }
      else if (line.find("timestamp ") == 0) {
        pSaveToFile->SetTimestampMode(std::stoi(line.substr(10)));
      }
      else if (line.find("decimation ") == 0) {
        pSaveToFile->SetDecimation(std::stoi(line.substr(11)));
      }
      else if (line == "show") {
        ::ShowWindow(window, SW_SHOWNORMAL);
      }
      else if (line == "hide") {
        ::ShowWindow(window, SW_HIDE);
      }
      else if (line == "quit") {
        ::SendMessageA(window, WM_DESTROY, 0, 0);
        std::cout << std::endl;
        return;
      }
      else {
        std::cout << "Unknown command: \"" << line << "\"";
      }
      std::cout << std::endl;
    }
  } catch(const std::exception& e) {
    ::SendMessageA(window, WM_DESTROY, 0, 0);
    std::cout << e.what() << std::endl;
  }
}

int WebcamRecorderMain(int argc, char** argv)
{
  int result = 0;

  std::string url, audiourl;
  int cameraIdx = 0, width = 640, height = 480, fps = 15,
      videoprofile = 0,
      bufferingtime = 0, maxbuffertimems = 1000,
      audiobufferingtime = 0, audiomaxbuffertimems = 1000;
  bool version = false, help = false;
  for (int i = 1; i < argc; ++i) {
    if (!::strcmp(argv[i], "--camera") && ++i < argc)
      cameraIdx = ::atoi(argv[i]);
    else if (!::strcmp(argv[i], "--url") && ++i < argc)
      url = argv[i];
    else if (!::strcmp(argv[i], "--audiourl") && ++i < argc)
      audiourl = argv[i];
    else if (!::strcmp(argv[i], "--width") && ++i < argc)
      width = ::atoi(argv[i]);
    else if (!::strcmp(argv[i], "--height") && ++i < argc)
      height = ::atoi(argv[i]);
    else if (!::strcmp(argv[i], "--fps") && ++i < argc)
      fps = ::atoi(argv[i]);
    else if (!::strcmp(argv[i], "--video-profile") && ++i < argc)
      videoprofile = ::atoi(argv[i]);
    else if (!::strcmp(argv[i], "--bufferingtime") && ++i < argc)
      bufferingtime = ::atoi(argv[i]);
    else if (!::strcmp(argv[i], "--audiobufferingtime") && ++i < argc)
      audiobufferingtime = ::atoi(argv[i]);
    else if (!::strcmp(argv[i], "--maxbuffertimems") && ++i < argc)
      maxbuffertimems = ::atoi(argv[i]);
    else if (!::strcmp(argv[i], "--audiomaxbuffertimems") && ++i < argc)
      audiomaxbuffertimems = ::atoi(argv[i]);
    else if (!::strcmp(argv[i], "--version"))
      version = true;
    else
      help = true;
  }

  if (version) {
    VersionInfo::Current.InsertInto(std::cout, true) << std::endl;
    return 0;
  }

  if (help) {
    std::cout << "usage: WebcamRecorder [--camera <idx>] [--url <url>] [--audiourl <url>] "
              << "[--width <px>] [--height <px>] [--fps <fps>] "
              << "[--video-profile <1-3>] "
              << "[--bufferingtime <s>] [--maxbuffertimems <ms>] "
              << "[--audiobufferingtime <s>] [--audiomaxbuffertimems <ms>] "
              << "[--version]"
              << std::endl;
    return -1;
  }

  MFInit mfInit;

  struct : Window
  {
    com::Ptr<IMFVideoDisplayControl> mpVideoDisplayControl;
    com::Ptr<WebcamPipeline> mpPipeline;
    bool OnPreTranslate(MSG* pMsg) override
    {
      if (pMsg->message == WM_USER && mpPipeline)
      {
        std::string error = mpPipeline->Error();
        if (!error.empty())
          throw std_runtime_error << error;
        return true;
      }
      if (pMsg->message == WM_SYSKEYDOWN) {
        if (pMsg->wParam == VK_F4)
          return true;
      }
      return Window::OnPreTranslate(pMsg);
    }
    bool OnClose() override
    {
      return false;
    }
    void OnCreate() override
    {
      HMENU hSysMenu = ::GetSystemMenu(Handle(), FALSE);
      if (hSysMenu)
        ::EnableMenuItem(hSysMenu, SC_CLOSE, (MF_DISABLED | MF_GRAYED | MF_BYCOMMAND));
    }
    void OnResize(int width, int height) override
    {
      if (mpVideoDisplayControl) {
        MFVideoNormalizedRect sourceRect = { 0, 0, 1, 1 };
        RECT destRect = { 0, 0, width, height };
        mpVideoDisplayControl->SetVideoPosition(&sourceRect, &destRect);
      }
    }
  } window;

  std::thread* pIOThread = nullptr;

  try {
    com::Ptr<WebcamPipeline> pPipeline = new WebcamPipeline;
    window.mpPipeline = pPipeline;

    VideoMediaType videoType;
    videoType.codec = FCC('raw ');
    videoType.width = width;
    videoType.height = height;
    videoType.framerateN = fps;
    videoType.framerateD = 1;
    pPipeline->SetDesiredVideoMediaType(videoType);

    AudioMediaType audioType;
    audioType.codec = FCC('sowt'); // 16bit pcm little-endian
    audioType.channels = 2;
    audioType.bitspersample = 16;
    audioType.samplerate = 44100;
    pPipeline->SetDesiredAudioMediaType(audioType);

    if (!url.empty())
        pPipeline->ActivateCameraFromURL(cameraIdx, url, bufferingtime, maxbuffertimems);
    else
        pPipeline->ActivateCameraFromWebcamIndex(cameraIdx);
    if (!audiourl.empty())
        pPipeline->ActivateAudioFromURL(audiourl, audiobufferingtime, audiomaxbuffertimems);
    std::string error = pPipeline->Error();
    if (!error.empty())
      throw std_runtime_error << error;

    std::string title = "Camera " + std::to_string(cameraIdx) + ": " + pPipeline->CameraName();
    window.SetTitle(title);
    pPipeline->SetWindowHandle(window.Handle());
    error = pPipeline->Error();
    if (!error.empty())
      throw std_runtime_error << error;

    com::Ptr<SaveToFileTransform> pSaveToFile = new SaveToFileTransform;
    pPipeline->SetTransform(static_cast<IMFTransform*>(pSaveToFile));
    error = pPipeline->Error();
    if (!error.empty())
      throw std_runtime_error << error;

    pPipeline->CreateTopology();
    error = pPipeline->Error();
    if (!error.empty())
      throw std_runtime_error << error;

    pSaveToFile->SetVideoProfile(videoprofile);

    ::OutputDebugStringA((pPipeline->ActualVideoMediaType().Describe() + "\n").c_str());
    auto pType = pPipeline->ActualVideoMediaType().Strip();
    pType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);
    pSaveToFile->AddInputAvailableType(SaveToFileTransform::VideoStreamID, pType);

    if (pPipeline->ActualAudioMediaType().mpType)
    {
      ::OutputDebugStringA((pPipeline->ActualAudioMediaType().Describe() + "\n").c_str());
      auto pType = pPipeline->ActualAudioMediaType().Strip();
      pType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
      pType->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, 16);
      pType->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, 44100);
      pType->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, 2);

      pSaveToFile->AddInputAvailableType(SaveToFileTransform::AudioStreamID, pType);
    }

    auto videoControl = pPipeline->ResolveTopology();
    error = pPipeline->Error();
    if (!error.empty())
      throw std_runtime_error << error;

    SIZE size = {0};
    if (videoControl)
    {
      videoControl->SetAspectRatioMode(MFVideoARMode_PreservePicture);
      videoControl->GetNativeVideoSize(&size, nullptr);
      window.mpVideoDisplayControl = videoControl;
    }

    pPipeline->Start();
    error = pPipeline->Error();
    if (!error.empty())
      throw std_runtime_error << error;

    std::string iniFilePath = FileUtils::StripExtension(FileUtils::ExecutablePath());
    iniFilePath += "_" + std::to_string(cameraIdx) + ".ini";
    IniFile iniFile(iniFilePath);
    auto& geom = iniFile["Geometry"];
    if (geom.Empty()) {
      window.SetLeft(100 + 50 * cameraIdx).SetTop(100 + 50 * cameraIdx);
      window.SetWidth(size.cx / 2).SetHeight(size.cy / 2);
    }
    else {
      window.SetLeft(geom["Left"]).SetTop(geom["Top"]);
      window.SetWidth(geom["Width"]).SetHeight(geom["Height"]);
    }

    std::cout << "OK" << std::endl; // signal that initialization is finished successfully
    pIOThread = new std::thread(IOThreadFunc, window.Handle(), pSaveToFile);
    result = EventLoop::Run();
    error = pPipeline->Error();
    if (!error.empty())
      throw std_runtime_error << error;
    pIOThread->join();

    pPipeline->Stop();
    error = pPipeline->Error();
    if (!error.empty())
      throw std_runtime_error << error;
    pPipeline->Close();
    error = pPipeline->Error();
    if (!error.empty())
      throw std_runtime_error << error;
    pPipeline->Shutdown();
    error = pPipeline->Error();
    if (!error.empty())
      throw std_runtime_error << error;

    window.mpPipeline = nullptr;

    geom["Left"] = window.NormalLeft();
    geom["Top"] = window.NormalTop();
    geom["Width"] = window.NormalWidth();
    geom["Height"] = window.NormalHeight();
  }
  catch(const std::exception& e)
  {
    std::string what = e.what();
    std::cout << what << std::endl;
    result = -1;
  }
#if 0
  if (pIOThread && pIOThread->joinable())
  {
    EventLoop::Run();
    pIOThread->join();
  }
#endif
  return result;
}
