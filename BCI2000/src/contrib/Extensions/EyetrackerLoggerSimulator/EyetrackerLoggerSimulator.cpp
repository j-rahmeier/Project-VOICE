/////////////////////////////////////////////////////////////////////////////
// $Id: EyetrackerLoggerSimulator.cpp 7993 2024-04-04 20:44:05Z mellinger $
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
#include "EyetrackerLoggerSimulator.h"
#include "BCIEvent.h"
#include "Thread.h"
#include "Expression.h"
#include "PhysicalUnit.h"
#include "Rate.h"
#include "Waitable.h"

#include <vector>
#include <map>
#include <string>
#include <algorithm>

RegisterExtension(EyetrackerLoggerSimulator);

static const double sGazeOffset = 1 << 15;

struct EyetrackerLoggerSimulator::Private : Thread
{
  int OnExecute() override;

  bool mEnabled = false;
  Expression mInit;
  std::vector<std::pair<std::string, Expression>> mExpressions;
  std::map<std::string, PhysicalUnit> mUnits;
  TimeUtils::TimeInterval mPeriod = 0;
  PhysicalUnit mUnitX, mUnitY, mUnitZ, mUnitPupil;
};


EyetrackerLoggerSimulator::EyetrackerLoggerSimulator()
: p(new Private)
{
    PublishEnabler("LogEyetrackerSimulator");
}

EyetrackerLoggerSimulator::~EyetrackerLoggerSimulator()
{
  Halt();
  delete p;
}

void EyetrackerLoggerSimulator::Publish()
{
  bool enabled = (OptionalParameter("LogEyetrackerSimulator") != 0);
  if (!enabled)
    return;

  BEGIN_PARAMETER_DEFINITIONS
    "Source:Eyetracker int /LogEyetrackerSimulator= 1 0 0 1"
    " // enable eye tracker logging (boolean)",
    "Source:Eyetracker int LogGazeData= 1 0 0 1"
    " // record gaze data to states (boolean)(readonly)",
    "Source:Eyetracker int LogEyePosition= 1 0 0 1"
    " // record eye position to states (boolean)(readonly)",
    "Source:Eyetracker int LogPupilSize= 1 0 0 1"
    " // record pupil size to states (boolean)(readonly)",
    "Source:Eyetracker int LogEyeDist= 1 0 0 1"
    " // record eye distance to states (boolean)(readonly)",

    "Source:Eyetracker string GazeResolution= 0.1mm % % % "
    "// eye tracker gaze recording resolution",
    "Source:Eyetracker string PupilResolution= 0.01mm % % % "
    "// eye tracker pupil diameter recording resolution",

    "Source:Eyetracker floatlist EyetrackerLegacyCorrectionOffset= 1 auto "
    "// subtract offset to get gaze/pupil in legacy units (readonly)(noedit)",
    "Source:Eyetracker floatlist EyetrackerLegacyCorrectionFactor= 1 auto "
    "// multiply by respective factor to get gaze/pupil in legacy units (readonly)(noedit)",
      
    "Source:Eyetracker floatlist EyetrackerScreenSize= {width height} 640mm 480mm // screen size in mm",
    "Source:Eyetracker float EyetrackerUpdateRate= 10Hz // update rate for simulated eyetracker data",
    "Source:Eyetracker string EyetrackerInitialization= omega:=2*pi/25 // initialization expression",
    "Source:Eyetracker matrix EyetrackerExpressions= {Left Right} "
       "{EyeGazeX EyeGazeY EyePosX EyePosY EyeDist PupilSize} "
       " 250-sin(omega*t)*100 250+cos(omega*t)*100 250-sin(omega*t)*100 250+cos(omega*t)*100 600 5 "
       " 250+cos(omega*t)*100 250+sin(omega*t)*100 250+cos(omega*t)*100 250+sin(omega*t)*100 600 5 "
       " // expressions for eyetracker data in mm",
  END_PARAMETER_DEFINITIONS

  BEGIN_EVENT_DEFINITIONS
    "EyetrackerTime 16 0 0 0",
    "EyetrackerLeftPupilSize 32 0 0 0",
    "EyetrackerRightPupilSize 32 0 0 0",
    "EyetrackerLeftEyeGazeX 32 0 0 0",
    "EyetrackerRightEyeGazeX 32 0 0 0",
    "EyetrackerLeftEyeGazeY 32 0 0 0",
    "EyetrackerRightEyeGazeY 32 0 0 0",
    "EyetrackerLeftEyePosX 32 0 0 0",
    "EyetrackerLeftEyePosY 32 0 0 0",
    "EyetrackerRightEyePosX 32 0 0 0",
    "EyetrackerRightEyePosY 32 0 0 0",
    "EyetrackerLeftEyeDist 32 0 0 0",
    "EyetrackerRightEyeDist 32 0 0 0",
  END_EVENT_DEFINITIONS
}

void EyetrackerLoggerSimulator::AutoConfig()
{
    if (OptionalParameter("LogEyetrackerSimulator"))
    {
        std::string gazeResolution = Parameter("GazeResolution");
        std::string pupilResolution = Parameter("PupilResolution");

        const double xyzRange = 1ull << 32;

        p->mUnitX.SetGainWithSymbol(gazeResolution).SetOffset(sGazeOffset);
        p->mUnitX.SetRawMin(0).SetPhysicalMax("4m");
        if (p->mUnitX.RawMax() >= xyzRange)
            bcierr << "X coordinate range exceeds state value range";

        p->mUnitY.SetGainWithSymbol(gazeResolution).SetOffset(sGazeOffset);
        p->mUnitY.SetRawMin(0).SetPhysicalMax("4m");
        if (p->mUnitY.RawMax() >= xyzRange)
            bcierr << "Y coordinate range exceeds state value range";

        p->mUnitZ.SetGainWithSymbol(gazeResolution).SetOffset(sGazeOffset);
        p->mUnitZ.SetRawMin(0).SetPhysicalMax("3m");
        if (p->mUnitZ.RawMax() >= xyzRange)
            bcierr << "Z coordinate range exceeds state value range";

        double pupilRange = 1ull << 32;
        p->mUnitPupil.SetGainWithSymbol(pupilResolution);
        p->mUnitPupil.SetRawMin(0).SetPhysicalMax("0.1m");
        if (p->mUnitPupil.RawMax() >= pupilRange)
            bcierr << "Pupil size range exceeds state value range";

        const double legacy_xyRange = 1ull << 16;
        PhysicalUnit sizeUnit;
        sizeUnit.SetGainWithSymbol("1mm");
        double xLen = sizeUnit.PhysicalToRaw(Parameter("EyetrackerScreenSize")("width"));
        double yLen = sizeUnit.PhysicalToRaw(Parameter("EyetrackerScreenSize")("height"));

        auto LegacyCorrectionFactor = Parameter("EyetrackerLegacyCorrectionFactor"),
                LegacyCorrectionOffset = Parameter("EyetrackerLegacyCorrectionOffset");
        LegacyCorrectionFactor->SetNumValues(4);
        LegacyCorrectionOffset->SetNumValues(4);
        int idx = 0;
        for (const char* label : { "x", "y", "z", "pupil" }) {
            LegacyCorrectionFactor->Labels()[idx] = label;
            LegacyCorrectionOffset->Labels()[idx] = label;
            ++idx;
        }

        LegacyCorrectionFactor("x") = p->mUnitX.Gain() / (xLen * 1e-3 / legacy_xyRange);
        LegacyCorrectionOffset("x") = p->mUnitX.Offset();
        LegacyCorrectionFactor("y") = p->mUnitY.Gain() / (yLen * 1e-3 / legacy_xyRange);
        LegacyCorrectionOffset("y") = p->mUnitY.Offset();
        LegacyCorrectionFactor("z") = p->mUnitZ.Gain() / 1e-3;
        LegacyCorrectionOffset("z") = p->mUnitZ.Offset();
        LegacyCorrectionFactor("pupil") = p->mUnitPupil.Gain() / 1e-4;
        LegacyCorrectionOffset("pupil") = p->mUnitPupil.Offset();
    }
}

void EyetrackerLoggerSimulator::Preflight() const
{
  bool enabled = (OptionalParameter("LogEyetrackerSimulator") != 0);
  if (enabled) {
    PreflightCondition(Parameter("EyetrackerUpdateRate").InHertz() > 0);
    ArithmeticExpression::VariableContainer variables;
    variables["t"] = 0.0;
    Expression init = Parameter("EyetrackerInitialization").ToString();
    init.Compile(variables);
    ParamRef expressions = Parameter("EyetrackerExpressions");
    for (int row = 0; row < expressions->NumRows(); ++row) {
      for (int col = 0; col < expressions->NumColumns(); ++col) {
        std::string stateName = "Eyetracker";
        stateName += expressions->RowLabels()[row];
        stateName += expressions->ColumnLabels()[col];
        if (!States->Exists(stateName))
          bcierr << "Invalid labels: there is no state named " << stateName;
        Expression expr(expressions(row, col));
        expr.Compile(variables);
        if (expr.RefersSignal())
          bcierr << "Eyetracker expression may not refer to signal: " << expr.AsString();
        if (expr.RefersStates())
          bcierr << "Eyetracker expression may not refer to states: " << expr.AsString();
      }
    }
  }
}


void EyetrackerLoggerSimulator::Initialize()
{
  p->Terminate();
  p->mExpressions.clear();
  p->mUnits.clear();
  p->mEnabled = (OptionalParameter("LogEyetrackerSimulator") != 0);
  if(p->mEnabled)
  {
    p->mPeriod = Rate::FromHertz(Parameter("EyetrackerUpdateRate").InHertz()).Period();
    p->mInit = Expression(Parameter("EyetrackerInitialization").c_str());
    ParamRef expressions = Parameter("EyetrackerExpressions");
    for (int row = 0; row < expressions->NumRows(); ++row) {
      for (int col = 0; col < expressions->NumColumns(); ++col) {
        std::string colHeader = expressions->ColumnLabels()[col];
        std::string state = "Eyetracker";
        state += expressions->RowLabels()[row]; // Left/Right
        state += colHeader; // Pos/Gaze/Pupil
        Expression expr = expressions(row, col).ToString();
        p->mExpressions.push_back(std::make_pair(state, expr));
        if (colHeader == "PupilSize") {
            p->mUnits[state] = p->mUnitPupil;
        }
        else if (!colHeader.empty()) {
            switch (colHeader.back()) {
                case 'X':
                    p->mUnits[state] = p->mUnitX;
                    break;
                case 'Y':
                    p->mUnits[state] = p->mUnitY;
                    break;
                default:
                    p->mUnits[state] = p->mUnitZ;
            }
        }
      }
    }
  }
}

void EyetrackerLoggerSimulator::StartRun()
{
  if(p->mEnabled) {
    p->Start();
  }
}

void EyetrackerLoggerSimulator::StopRun()
{
  p->Terminate();
}

void EyetrackerLoggerSimulator::Halt()
{
  StopRun();
}

int EyetrackerLoggerSimulator::Private::OnExecute()
{
  ArithmeticExpression::VariableContainer variables;
  variables["t"] = 0;
  mInit.Compile(variables);
  mInit.Evaluate();

  for (auto& entry : mExpressions)
    entry.second.Compile(variables);

  Waitable::Timeout timeout;
  while (timeout.Wait(mPeriod)) {
    for (auto& entry : mExpressions) {
      double value = entry.second.Evaluate();
      value *= 1e-3; // convert to meters
      value = mUnits[entry.first].PhysicalToRawValue(value);
      value = ::floor(value + 0.5);
      bcievent << entry.first << " " << value;
    }
    variables["t"] += 1;
    bcievent << "EyetrackerTime " << PrecisionTime::Now();
  }
  return 0;
}
