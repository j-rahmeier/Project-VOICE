/////////////////////////////////////////////////////////////////////////////
// $Id: WebcamController.cpp 7889 2024-03-03 18:23:32Z mellinger $
// Authors: adam.wilson@uc.edu & Alex Belsten belsten@neurotechcenter.org,
//   mellinger@neurotechcenter.org
//
// Description: The WebcamController controls a WebcamRecorder instance
//   and records the current frame number to a bcievent.
//
// Event Variables:
//   WebcamFrame<n> - The current frame number for camera index n 
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
/////////////////////////////////////////////////////////////////////////////
#include "WebcamController.h"

#include "WebcamThread.h"
#include "BCIEvent.h"
#include "BCIStream.h"
#include "GenericVisualization.h"

struct WebcamController::Private
{
  std::string mError;

  std::string mBaseName;
  std::atomic<int> mFilePart = 0;

  int mCameraIndex = -1;
  WebcamThread* mpWebcamThread = nullptr;

  VideoFrame mFrame;
  GenericVisualization mVis;

  std::string createFileName() const;
};

std::string WebcamController::Private::createFileName() const
{
  std::string fileName = mBaseName + "_" + std::to_string(mCameraIndex) + "_vid.";
  if (mFilePart > 1)
    fileName += "part" + std::to_string(mFilePart) + ".";
  fileName += "mp4";
  return fileName;
}

WebcamController::WebcamController()
  : p(new Private)
{
    p->mpWebcamThread = new WebcamThread;
}

WebcamController::~WebcamController()
{
  HideWindow();
  p->mpWebcamThread->Stop();
  delete p->mpWebcamThread;
  delete p;
}

const std::string& WebcamController::Error() const
{
  return p->mError;
}

void WebcamController::OnWebcamFrameNumber(uint64_t frame)
{
  if (p->mFilePart > 0)
    bcievent << "WebcamFrame" << p->mCameraIndex << " " << frame;
  if (!p->mpWebcamThread->Error().empty())
    bcierr << "Camera " << p->mCameraIndex << ": " << p->mpWebcamThread->Error();
}

void WebcamController::OnWebcamFrameData(int width, int height, const void* pData)
{
  bool firstFrame = p->mFrame.Empty();
  p->mFrame.AssignData(width, height, pData);
  if (firstFrame && GenericVisualization::OutputChannelIsLocal())
    p->mFrame.ShareAcrossModules();
  p->mVis.Send(p->mFrame);
  if (!p->mpWebcamThread->Error().empty())
    bcierr << "Camera " << p->mCameraIndex << ": " << p->mpWebcamThread->Error();
}

void WebcamController::OnWebcamMessage(const std::string& message)
{
    bciwarn << "Camera " << p->mCameraIndex << ": " << message;
}

void WebcamController::Preflight(int camIndex, const Parameters& params)
{
    p->mpWebcamThread->SetPreflight(true);
    if (!p->mpWebcamThread->Start(camIndex, params, nullptr))
        p->mError = p->mpWebcamThread->Error();
}

void WebcamController::Initialize(int camIndex, const Parameters &params)
{
  p->mCameraIndex = camIndex;
  p->mpWebcamThread->SetPreflight(false);
  if (!p->mpWebcamThread->Start(camIndex, params, this))
    p->mError = p->mpWebcamThread->Error();
  p->mVis.SetVisID("Webcam" + std::to_string(camIndex));
  p->mVis.Send(CfgID::WindowTitle, p->mpWebcamThread->WindowTitle());
}

void WebcamController::ShowWindow()
{
  p->mVis.Send(CfgID::Visible, true);
}

void WebcamController::HideWindow()
{
  p->mVis.Send(CfgID::Visible, false);
}

void WebcamController::SetDecimation(int decimation)
{
  if (!p->mpWebcamThread->SetDecimation(decimation))
    p->mError = p->mpWebcamThread->Error();
}

void WebcamController::SetTimestampOption(int option)
{
  if (!p->mpWebcamThread->SetTimestampMode(option))
    p->mError = p->mpWebcamThread->Error();
}

void WebcamController::StartRecording(const std::string& baseName)
{
  p->mBaseName = baseName;
  p->mFilePart = 1;
  if (!p->mpWebcamThread->StartRecording(p->createFileName()))
    p->mError = p->mpWebcamThread->Error();
}

void WebcamController::StopRecording()
{
  if (!p->mpWebcamThread->StopRecording())
    p->mError = p->mpWebcamThread->Error();
  p->mFilePart = 0;
}

void WebcamController::IncrementFilePart()
{
  if (p->mFilePart > 0) {
    ++p->mFilePart;
    bcievent << "WebcamFrame" << p->mCameraIndex << " 0";
    if (!p->mpWebcamThread->SwitchRecording(p->createFileName()))
      p->mError = p->mpWebcamThread->Error();
  }
}

