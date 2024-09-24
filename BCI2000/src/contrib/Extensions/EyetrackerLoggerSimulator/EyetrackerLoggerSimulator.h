/////////////////////////////////////////////////////////////////////////////
// $Id: EyetrackerLoggerSimulator.h 7875 2024-02-29 17:20:42Z mellinger $
// Authors: mellinger@neurotechcenter.org
// Description: This Logger simulates gaze data from an eyetracker and
//   changes states accordingly.
//
// Parameterization:
//   Eyetracker logging is enabled from the command line adding
//     --LogEyetrackerSimulator=1
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
// Expressions:
//   For each state variable, there is a parameter with the same name, holding an expression
//   from which to compute that state. Within the expression, the parameter t is a timestamp
//   increased by 1 for each time step.
//
// Parameter TimeStep:
//   Parameter determining the duration of a time step.
//
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
#ifndef EYETRACKER_LOGGER_SIMULATED_H
#define EYETRACKER_LOGGER_SIMULATED_H

#include "Environment.h"

class EyetrackerLoggerSimulator : public EnvironmentExtension
{
 public:
  EyetrackerLoggerSimulator();
  ~EyetrackerLoggerSimulator();
  void Publish() override;
  void AutoConfig() override;
  void Preflight() const override;
  void Initialize() override;
  void StartRun() override;
  void StopRun() override;
  void Halt() override;

 private:
  struct Private;
  Private* p;
};

#endif // EYETRACKER_LOGGER_SIMULATED_H

