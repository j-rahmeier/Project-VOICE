/////////////////////////////////////////////////////////////////////////////
// $Id: EyetrackerLoggerTobiiPro.h 7733 2023-11-24 15:39:37Z mellinger $
// Authors: markus.adamek@gmail.com
// Description: The Eyetracker Logger logs incoming gaze data from a Tobii
// Eyetracker and changes states
//
// Parameterization:
//   Eyetracker logging is enabled from the command line adding
//     --LogEyetrackerTobiiPro=1
//   As a command line option.
//   Information may be selectively logged by setting these parameters to true
//     LogGazeData - record gaze data to states
//     LogEyePosition - record eye position to states
//     LogPupilSize - record pupil size to states
//     LogEyeDist - record eye distances to states
//
// State Variables:
//   EyetrackerLeftEyeGazeX   - Left Eye X Gaze Position (0 = leftmost, 65535 = rightmost)
//   EyetrackerLeftEyeGazeY   - Left Eye Y Gaze Position (0 = topmost, 65535 = bottommost)
//   EyetrackerRightEyeGazeX  - Right Eye X Gaze Position (0 = leftmost, 65535 = rightmost)
//   EyetrackerRightEyeGazeY  - Right Eye Y Gaze Position (0 = topmost, 65535 = bottommost)
//   EyetrackerLeftEyePosX    - Left Eye X Position (0 = leftmost, 65535 = rightmost)
//   EyetrackerLeftEyePosY    - Left Eye Y Position (0 = topmost, 65535 = bottommost)
//   EyetrackerRightEyePosX   - Right Eye X Position (0 = leftmost, 65535 = rightmost)
//   EyetrackerRightEyePosY   - Right Eye Y Position (0 = topmost, 65535 = bottommost)
//   EyetrackerLeftPupilSize  - Left Eye Pupil Size (mm), Depends on eye pos and dist
//   EyetrackerRightPupilSize - Right Eye Pupil Size (mm), Depends on eye pos and dist
//   EyetrackerLeftEyeDist    - Left Eye Distance from Screen (mm), Approx
//   EyetrackerRightEyeDist   - Right Eye Distance from Screen (mm), Approx
//   EyetrackerTimeStamp      - The timestamp for the gaze data relative to BCI timer
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
#ifndef EYETRACKER_LOGGER_TOBII_PRO_H
#define EYETRACKER_LOGGER_TOBII_PRO_H

#include "Environment.h"
#include "tobii_research.imports.h"
#include <mutex>

class EyetrackerLoggerTobiiPro : public EnvironmentExtension
{
 public:
  //Constructors and virtual interface
  EyetrackerLoggerTobiiPro();
  ~EyetrackerLoggerTobiiPro();
  void Publish() override;
  void AutoConfig() override;
  void Preflight() const override;
  void Initialize() override;
  void StartRun() override;
  void Process() override;
  void StopRun() override;
  void Halt() override;

 private:
  bool         m_eyetrackerEnable;
  bool         m_firstData;
  std::mutex   m_Mutex;
  bool         m_isLogging, m_subscribed;
  uint64_t     m_timeOffset;
  bool         m_sync_calculated;
  TobiiResearchEyeTracker* m_connectedDevice;
  TobiiResearchEyeTrackers* m_deviceList;
  TobiiResearchGazeData m_prevGazeData;

  TobiiResearchEyeTracker* FindDevice();
  void storeEyetrackerInformation(TobiiResearchEyeTracker* tracker);

  static void gaze_data_callback(TobiiResearchGazeData* gaze_data, void* user_data);
  void SetGazeData(TobiiResearchGazeData const&);

  bool logEyeData(const TobiiResearchEyeData &data, const TobiiResearchEyeData &prev_data, const char* eye);
  struct TobiiProScreenCoords;
  TobiiProScreenCoords* mpCoordSystem;

  void RegisterGazeCallbacks();
  void UnregisterGazeCallbacks();

  // Record eye images into a video file.
  static void eye_image_callback(TobiiResearchEyeImage*, void*);
  void ViewEyeImageData(const TobiiResearchEyeImage*);
  std::string ConstructVideoFileName(int cameraIndex) const;
  struct
  {
    int count = 0;
    int64_t prevts = -1;
  } mFullFrameData, mRoiFrameData;
  std::mutex mEyeImageMutex;
  class VideoRecorder* mpVideoRecorderFull = nullptr, *mpVideoRecorderRoi = nullptr;
  class FrameComposer* mpFrameComposerFull = nullptr, *mpFrameComposerRoi = nullptr;
  bool mDisplayEyeGazeVideo = false, mRecordEyeGazeVideo = false;

  int mFilePart = 0;
  std::string mFileBase;
};

#endif // EYETRACKER_LOGGER_TOBII_PRO_H
