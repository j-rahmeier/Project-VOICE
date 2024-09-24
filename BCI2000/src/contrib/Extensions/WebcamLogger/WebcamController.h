/////////////////////////////////////////////////////////////////////////////
// $Id: WebcamController.h 7749 2023-12-08 17:03:51Z mellinger $
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
#ifndef WEBCAM_CONTROLLER_H
#define WEBCAM_CONTROLLER_H

#include <string>

class WebcamController
{
public:
  static const std::string& ExecutableName();

  WebcamController();
  ~WebcamController();
  WebcamController(const WebcamController&) = delete;
  WebcamController& operator=(const WebcamController&) = delete;

  const std::string& Error() const;

  struct Parameters
  {
      int width = 640, height = 480, fps = 15, videoprofile = 1;
      std::string url;
      int bufferingtime = -1, maxbuffertimems = -1;
      std::string audiourl;
      int audiobufferingtime = -1, audiomaxbuffertimems = -1;
  };

  void Initialize(int camIndex, const Parameters&);
  void ShowWindow();
  void HideWindow();
  void SetDecimation(int decimation);
  void SetTimestampOption(int option);
  void StartRecording(const std::string& baseName);
  void StopRecording();
  void IncrementFilePart();

private:
  struct Private;
  Private* p;
};

#endif // WEBCAM_CONTROLLER_H





