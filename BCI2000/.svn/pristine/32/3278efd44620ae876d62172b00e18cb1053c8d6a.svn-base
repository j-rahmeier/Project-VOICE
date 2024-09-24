/////////////////////////////////////////////////////////////////////////////
// $Id: WebcamLogger.h 2118 6/30/2010
// Authors: adam.wilson@uc.edu, mellinger@neurotechcenter.org
// Description: The Webcam Logger logs video data from standard webcams,
// saves the videos in a compressed video format, and stores the frame numbers
// as event states
//
// Parameterization:
//   Webcam logging is enabled from the command line adding
//     --LogWebcam=1
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
#include "WebcamLogger.h"
#include "FileUtils.h"
#include "RunManager.h"

#define NUM_OF_WEBCAM_EVENTS 4

Extension( WebcamLogger );

static void IndexTooLargeInfo()
{
  bcierr << "The max webcam index supported by this version of BCI2000 is " << NUM_OF_WEBCAM_EVENTS
         << ". To record from more webcams or webcams of higher index, change NUM_OF_WEBCAM_EVENTS macro in WebcamLogger.cpp to the "
         << "max index or number of webcams you want to record from and recompile.";
}

WebcamLogger::WebcamLogger()
{
  PublishEnabler("LogWebcam");
}

WebcamLogger::~WebcamLogger()
{
  Halt();
}

void WebcamLogger::Publish()
{
  bool webcamEnable = ( (int)OptionalParameter("LogWebcam", 0) != 0 );
  if (!webcamEnable)
    return;

  BEGIN_PARAMETER_DEFINITIONS
    "Source:Webcam int /LogWebcam= 1 0 0 1"
      " // Allow logging from webcam (boolean)",

    "Source:Webcam int CameraNumber= 0 0 0 %"
      " // Camera index in case of a single webcam",

    "Source:Webcam int RecordAll= 0 0 0 1"
      " // Record from all available webcams (boolean)",

    "Source:Webcam int StartIndex= 0 0 0 %"
      " // Camera index to start searching at",

    "Source:Webcam int EndIndex= 2 2 0 %"
    " // Camera index to stop searching at",

    "Source:Webcam int Decimation= 1 1 1 %"
      " // Save every nth frame",

    "Source:Webcam matrix WebcamFormat= 1 { camera%20index width height fps encoder%20profile }"
      " 0 720 480 15 2 % %"
      " // Desired resolution and fps for each webcam",

    "Source:Webcam matrix WebcamURL= 0 { camera%20index URL BufferingTime MaxBufferTime }"
      " // Source URL to use instead of physical webcam at given index",

    "Source:Webcam matrix AudioURL= 0 { camera%20index URL BufferingTime MaxBufferTime }"
     " // Audio URL to use for webcam at given index",

    "Source:Webcam int DateTimeLocation= 0 0 0 5"
      " // Date/time text location in saved video: "
        " 0: none,"
        " 1: UpperRight,"
        " 2: UpperLeft,"
        " 3: LowerRight,"
        " 4: LowerLeft,"
        " 5: FrameCount "
          " (enumeration)",

    "Source:Webcam int DisplayStream= 1 1 0 1"
      " // Display camera data in a window (boolean)",

  END_PARAMETER_DEFINITIONS

  // declare NUM_OF_WEBCAM_EVENTS event states
  for (int i = 0; i < NUM_OF_WEBCAM_EVENTS; i++) 
  {
    std::ostringstream definition;
    definition << "WebcamFrame" << i << " 32 0 0 0" << std::flush;
    BEGIN_EVENT_DEFINITIONS
      definition.str().c_str(),
    END_EVENT_DEFINITIONS
  }
}

void WebcamLogger::Preflight() const
{
  if (OptionalParameter("LogWebcam", 0) == 0)
    return;

  mControllers.clear(); // make sure no controller instance is connected to a camera
  if (FileUtils::ExecutableCanonicalPath(WebcamController::ExecutableName()).empty())
    throw bcierr << "WebcamLogger depends on " << WebcamController::ExecutableName() 
                 << " executable, which cannot be found";

  if (Parameter("RecordAll")) {
    int start = Parameter("StartIndex"),
      end = Parameter("EndIndex");
    if (start > end)
      bcierr << "StartIndex must be less than EndIndex";
    if (end > NUM_OF_WEBCAM_EVENTS)
      IndexTooLargeInfo();
    int n = CreateControllers(
      start, end, 
      Parameter("WebcamFormat"), 
      Parameter("WebcamURL"), Parameter("AudioURL")
    ).size();
    if (n == 0)
      bciwarn << "No webcams found between indices " << start << " and " << end;
  }
  else {
    int idx = Parameter("CameraNumber");
    if (idx > NUM_OF_WEBCAM_EVENTS)
      IndexTooLargeInfo();
    CreateControllers(
      idx, idx, 
      Parameter("WebcamFormat"), 
      Parameter("WebcamURL"), Parameter("AudioURL")
    );
  }

  State("FilePart");
}

void WebcamLogger::Initialize()
{
  if (OptionalParameter("LogWebcam", 0) == 0)
    return;

  mControllers.clear(); // make sure no controller instance is connected to a camera
  if (Parameter("RecordAll"))
    mControllers = CreateControllers(
      Parameter("StartIndex"), Parameter("EndIndex"), 
      Parameter("WebcamFormat"), 
      Parameter("WebcamURL"), Parameter("AudioURL")
    );
  else
    mControllers = CreateControllers(
      Parameter("CameraNumber"), Parameter("CameraNumber"), 
      Parameter("WebcamFormat"), 
      Parameter("WebcamURL"), Parameter("AudioURL")
    );

  for (auto& p : mControllers)
  {
    if (Parameter("DisplayStream"))
      p->ShowWindow();
    p->SetTimestampOption(Parameter("DateTimeLocation"));
    p->SetDecimation(Parameter("Decimation"));
    if (!p->Error().empty())
      bcierr << p->Error();
  }
}

void WebcamLogger::StartRun()
{
  mFilePart = 1;
  std::string baseName = FileUtils::StripExtension(RunManager()->CurrentRun());
  for (const auto& p : mControllers) {
    p->StartRecording(baseName);
    if (!p->Error().empty())
      bcierr << p->Error();
  }
}

void WebcamLogger::StopRun()
{
  for (const auto& p : mControllers) {
    p->StopRecording();
    if (!p->Error().empty())
      bcierr << p->Error();
  }
}

void WebcamLogger::Process()
{
  if (OptionalParameter("LogWebcam", 0) == 0)
    return;

  int nextFilePart = State("FilePart")(Statevector->Samples() -1);
  if (nextFilePart > mFilePart)
  {
    mFilePart = nextFilePart;
    for (const auto& p : mControllers) {
      p->IncrementFilePart();
      if (!p->Error().empty())
        bcierr << p->Error();
    }
  }
}

void WebcamLogger::Halt()
{
  mControllers.clear();
}

WebcamLogger::ControllerContainer WebcamLogger::CreateControllers(
  int startIndex, int endIndex, 
  const ParamRef& formats, const ParamRef& urls, const ParamRef& audiourls
)
{
  ControllerContainer controllers;
  for (int camIndex = startIndex; camIndex <= endIndex; ++camIndex)
  {
    auto pController = std::make_unique<WebcamController>();
    WebcamController::Parameters params;
    params.width = 720;
    params.height = 480;
    params.fps = 15;
    params.bufferingtime = 0;
    params.maxbuffertimems = 1000;
    params.videoprofile = 2;
    FindFormat(camIndex, formats, params.width, params.height, params.fps, params.videoprofile);
    FindURL(camIndex, urls, params.url, params.bufferingtime, params.maxbuffertimems);
    FindURL(camIndex, audiourls, params.audiourl, params.audiobufferingtime, params.audiomaxbuffertimems);
    if (params.audiobufferingtime < 0)
      params.audiobufferingtime = params.bufferingtime;
    if (params.audiomaxbuffertimems < 0)
      params.audiomaxbuffertimems = params.maxbuffertimems;
    pController->Initialize(camIndex, params);
    if (pController->Error().empty())
      controllers.push_back(std::move(pController));
    else if (pController->Error().find("No camera available") != 0)
      bcierr << "Camera " << camIndex << ": " << pController->Error();
  }
  return controllers;
}

void WebcamLogger::FindFormat(int index, const ParamRef& formats, int& width, int& height, int& fps, int& videoprofile)
{
  for (int i = 0; i < formats->NumRows(); ++i) {
    if (formats(i, 0) == index) {
      width = formats(i, "width");
      height = formats(i, "height");
      fps = formats(i, "fps");
      videoprofile = formats(i, "encoder profile");
      return;
    }
  }
}

void WebcamLogger::FindURL(int index, const ParamRef& urls, std::string& url, int& bufferingtime, int& maxbuffertimems)
{
  PhysicalUnit timeunit;
  timeunit.SetSymbol("s");
  try {
    for (int i = 0; i < urls->NumRows(); ++i) {
      if (urls(i, 0) == index) {
        url = urls(i, "URL").ToString();
        if (urls->ColumnLabels().Exists("bufferingtime"))
          bufferingtime = std::round(timeunit.PhysicalToRaw(urls(i, "bufferingtime")));
        if (urls->ColumnLabels().Exists("maxbuffertime"))
          maxbuffertimems = std::round(1000 * timeunit.PhysicalToRaw(urls(i, "maxbuffertime")));
        return;
      }
    }
  }
  catch (const Exception& e) {
    bcierr << "Parameter " << urls->Name() << ": " << e.What();
  }
}

