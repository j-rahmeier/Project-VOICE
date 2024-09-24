////////////////////////////////////////////////////////////////////////////////
// $Id: WebcamRecorder.cpp 7895 2024-03-05 12:44:22Z mellinger $
// Author: mellinger@neurotechcenter.com
// Description: An application that records video from a camera,
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
#include "WebcamPipeline.h"
#include "VideoWidget.h"
#include "VideoMediaType.h"
#include "AudioMediaType.h"
#include "IniFile.h"
#include "VersionInfo.h"
#include "FileUtils.h"

// for htonl()
#if _WIN32
# include <Winsock2.h>
#else
# include <arpa/inet.h>
#endif

#include <iostream>
#include <thread>
#include <stdexcept>

#include "QtMain.h"

void IOThreadFunc(QWidget* pWindow, WebcamPipeline* pPipeline)
{
  try {
    std::string line;
    while (std::getline(std::cin, line)) {
      if (line.find("record ") == 0) {
          pPipeline->StartRecording(line.substr(7));
      }
      else if (line.find("switchto ") == 0) {
          pPipeline->SwitchRecording(line.substr(9));
      }
      else if (line == "stop") {
          pPipeline->StopRecording();
      }
      else if (line.find("timestamp ") == 0) {
          pPipeline->SetTimestampMode(std::stoi(line.substr(10)));
      }
      else if (line.find("decimation ") == 0) {
          pPipeline->SetDecimation(std::stoi(line.substr(11)));
      }
      else if (line == "show") {
          QMetaObject::invokeMethod(pWindow, "show", Qt::AutoConnection);
      }
      else if (line == "hide") {
          QMetaObject::invokeMethod(pWindow, "hide", Qt::AutoConnection);
      }
      else if (line == "quit") {
          QMetaObject::invokeMethod(pWindow, "show", Qt::AutoConnection);
          QMetaObject::invokeMethod(pWindow, "close", Qt::AutoConnection);
          std::cout << std::endl;
        return;
      }
      else {
        std::cout << "Unknown command: \"" << line << "\"";
      }
      std::cout << std::endl;
    }
  } catch(const std::exception& e) {
      QMetaObject::invokeMethod(pWindow, "show", Qt::AutoConnection);
      QMetaObject::invokeMethod(pWindow, "close", Qt::AutoConnection);
      std::cout << e.what() << std::endl;
  }
}

int main(int argc, char** argv)
{
  QtApplication app(argc, argv);

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

  std::thread* pIOThread = nullptr;

  try {
    WebcamPipeline pipeline;

    VideoMediaType videoType;
    videoType.codec = htonl('raw ');
    videoType.width = width;
    videoType.height = height;
    videoType.framerateN = fps;
    videoType.framerateD = 1;
    pipeline.SetDesiredVideoMediaType(videoType);

    AudioMediaType audioType;
    audioType.codec = htonl('sowt'); // 16bit pcm little-endian
    audioType.channels = 2;
    audioType.bitspersample = 16;
    audioType.samplerate = 44100;
    pipeline.SetDesiredAudioMediaType(audioType);

    if (!url.empty())
        pipeline.ActivateCameraFromURL(cameraIdx, url, bufferingtime, maxbuffertimems);
    else
        pipeline.ActivateCameraFromWebcamIndex(cameraIdx);
    if (!audiourl.empty())
        pipeline.ActivateAudioFromURL(audiourl, audiobufferingtime, audiomaxbuffertimems);
    std::string error = pipeline.Error();
    if (!error.empty())
      throw std::runtime_error(error);

    VideoWidget videoWidget;

    std::string title = "Camera " + std::to_string(cameraIdx) + ": " + pipeline.CameraName();
    videoWidget.setWindowTitle(title.c_str());
    pipeline.SetWidget(&videoWidget);
    error = pipeline.Error();
    if (!error.empty())
      throw std::runtime_error(error);

    pipeline.StartAcquisition();
    error = pipeline.Error();
    if (!error.empty())
        throw std::runtime_error(error);

    std::string iniFilePath = FileUtils::StripExtension(FileUtils::ExecutablePath());
    iniFilePath += "_" + std::to_string(cameraIdx) + ".ini";
    IniFile iniFile(iniFilePath);
    auto& geom = iniFile["Geometry"];
    if (geom.Empty()) {
      videoWidget.move(100 + 50 * cameraIdx, 100 + 50 * cameraIdx);
      auto videoType = pipeline.ActualVideoMediaType();
      int width = videoType.width / 2, height = videoType.height / 2;
      width = std::max(width, 160);
      height = std::max(height, 100);
      videoWidget.resize(width, height);
    }
    else {
      videoWidget.move(geom["Left"], geom["Top"]);
      videoWidget.resize(geom["Width"], geom["Height"]);
    }

    std::cout << "OK" << std::endl; // signal that initialization is finished successfully

    pIOThread = new std::thread(IOThreadFunc, &videoWidget, &pipeline);
    result = app.Run();
    error = pipeline.Error();
    if (!error.empty())
      throw std::runtime_error(error);
    pIOThread->join();

    pipeline.StopAcquisition();
    error = pipeline.Error();
    if (!error.empty())
        throw std::runtime_error(error);

    geom["Left"] = videoWidget.pos().x();
    geom["Top"] = videoWidget.pos().y();
    geom["Width"] = videoWidget.size().width();
    geom["Height"] = videoWidget.size().height();
  }
  catch(const std::exception& e)
  {
    std::string what = e.what();
    std::cout << what << std::endl;
    result = -1;
  }
  return result;
}
