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
#ifndef WEBCAMLOGGER_H
#define WEBCAMLOGGER_H

#include <vector>
#include <memory>

#include "Environment.h"
#include "WebcamController.h"

class WebcamLogger : public EnvironmentExtension
{
public:
  WebcamLogger();
  ~WebcamLogger();
  void Publish() override;
  void Preflight() const override;
  void Initialize() override;
  void StartRun() override;
  void StopRun() override;
  void Process() override;
  void Halt() override;
  void Halt2() override { Halt(); }

private:
  typedef std::vector<std::unique_ptr<WebcamController>> ControllerContainer;

  static ControllerContainer CreateControllers(int startIndex, int endIndex, const ParamRef& formats, const ParamRef& urls, const ParamRef& audiourls);
  static void FindFormat(int index, const ParamRef&, int& width, int& height, int& fps, int& videoprofile);
  static void FindURL(int index, const ParamRef&, std::string& url, int& bufferingtime, int& maxbuffertimems);

  mutable ControllerContainer mControllers;
  int mFilePart = 0;
};

#endif // WEBCAM_LOGGER_H
