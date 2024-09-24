/////////////////////////////////////////////////////////////////////////////
// $Id: EyetrackerLoggerEyeLink.cpp 7993 2024-04-04 20:44:05Z mellinger $
// Authors:  William Engelhardt (engelhardt@neurotechcenter.org)
// Description: Implementation with EyeLink's eye tracker to BCI2000 and
// writes foundational data to states.
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
#include "BCIEvent.h"
#include "EyetrackerLoggerEyeLink.h"
#include <utils/Expression/ArithmeticExpression.cpp>

using namespace std;

Extension(EyetrackerLoggerEyeLink);

#define EYELINK_DUMMY

EyetrackerLoggerEyeLink::EyetrackerLoggerEyeLink() :
  mEnable(false),
  mHostAddress(nullptr)
{
    PublishEnabler("LogEyetrackerEyeLink");
}

EyetrackerLoggerEyeLink::~EyetrackerLoggerEyeLink()
{
  // deallocate at memory

}

void
EyetrackerLoggerEyeLink::Publish()
{
  if (!OptionalParameter("LogEyetrackerEyeLink", 0))
    return;

  BEGIN_PARAMETER_DEFINITIONS
    "Source:Eyetracker int /LogEyetrackerEyeLink= 0 0 0 1"
    " // Enable eye tracker logging (boolean)",
    "Source:Eyetracker int DataType= 1 0 0 1"
    " // Data type is 0 HREF, 1 GAZE (enumeration)",
    "Source:Eyetracker int SaveEDF= 0 0 0 1"
    " // Save all sample and event data to an EDF (boolean)",
    "Source:Eyetracker string EyeLinkHostAddress= 100.1.1.1 % % %"
    " // EyeLink IPv4 address",
    "Source:Eyetracker matrix ExperimentParams=  "
    "{ Use Remote PhysX PhysY EyeToTop EyeToBottom LensToScreen }"
    "{ Parameters }"
    "0 1 auto auto auto auto auto",
    "Source:Eyetracker matrix EyeLinkInfo= "
    "{ WhichEye TrackerVersion Prescaler Width Height FrameRate}" //column labels
    "{ Answers }" //row label
    "auto auto auto auto auto auto // (readonly)(noedit)",
    "Source:Eyetracker float GazeScale= 1 1 0 1"
    " // value to scale gaze data by (deprecated)",
    "Source:Eyetracker float GazeOffset= 0 0 & &"
    " // value to offset gaze data after scaled (deprecated)",
    "Source:Eyetracker int LogGazeData= 1 1 auto "
    "// GazeMonitorFilter compatability (boolean)",

    END_PARAMETER_DEFINITIONS

    auto m_eye_input = OptionalParameter("WhichEye", 2);
  int c = 2;
  m_desired_eye = 2;
  vector<string> side = { "Left", "Right" };
  if ((m_eye_input.ToNumber() == 0) || (m_eye_input.ToString() == "Left"))
  {
    c = 1;
    m_desired_eye = 0;
  }
  StringUtils::String eventDef;
  vector<string> dType = { "PosX", "PosY", "GazeX", "GazeY", "HRefX", "HRefY", "PupilSize", "Validity" };
  int startV = 0;
  for (int i = 0; i < c; i++)
  {
    if ((m_eye_input.ToNumber() == 1) || (m_eye_input.ToString() == "Right"))
    {
      i = 1;
      m_desired_eye = 1;
    }
    for (int j = 0; j < dType.size(); j++)
    {
      //startV = 0;
      //if (dType[i] == "Validity")
       // startV = 15;
      eventDef << "Eyetracker" << side[i] << "Eye" << dType[j] << " 16 " << startV << " 0 0";

      BEGIN_EVENT_DEFINITIONS
        eventDef.c_str()
      END_EVENT_DEFINITIONS
    }
  }

  //not dependent on which eye
  BEGIN_EVENT_DEFINITIONS
    "EyetrackerEyeResX 16 0 0 0",
    "EyetrackerEyeResY 16 0 0 0",
    "EyetrackerTime 32 0 0 0",
  END_EVENT_DEFINITIONS
}

void
EyetrackerLoggerEyeLink::AutoConfig()
{
  if (!OptionalParameter("LogEyetrackerEyeLink", 0)) return;
  // set all parameters that may be set to auto

  Parameter("EyeLinkHostAddress") = "100.1.1.1";
  // initialize connection to device

  if (Parameter("ExperimentParams")(0, 0)) //set default values
  {
    Parameter("ExperimentParams")(2, 0) = 340; //physical x (mm)
    Parameter("ExperimentParams")(3, 0) = 270; //physical y (mm)
    if (!Parameter("ExperimentParams")(1, 0)) //not remote
    {
      Parameter("ExperimentParams")(4, 0) = 930; //eye to top of screen dist (mm)
      Parameter("ExperimentParams")(5, 0) = 960; //eye to bottom of screen dist (mm)
    }
    else //remote mode
    {
      Parameter("ExperimentParams")(6, 0) = 70; //lens to screen dist (mm)
    }
  }
}


void
EyetrackerLoggerEyeLink::Preflight() const
{
  if (!OptionalParameter("LogEyetrackerEyeLink", 0)) return;

  // check that all parameters are valid
  Parameter("EyeLinkHostAddress");
  Parameter("EyeLinkInfo");
  Parameter("DataType");
  Parameter("SaveEDF");
  //for using EDF file
  OptionalParameter("DataDirectory", 0);
  OptionalParameter("SubjectName", 0);
  OptionalParameter("SubjectSession", 0);
  OptionalParameter("SubjectRun", 0);
  //for setting experiment parameters
  Parameter("ExperimentParams");
  OptionalParameter("WindowWidth", 0); //in Application Window tab
  OptionalParameter("WindowHeight", 0);
}


void
EyetrackerLoggerEyeLink::Initialize()
{
  mEnable = OptionalParameter("LogEyetrackerEyeLink", 0);
  if (!mEnable) return;

  mSaveEDF = Parameter("SaveEDF");

  // parameters are valid, open device and initialize all members for recording 
  mHostAddress = (char*)Parameter("EyeLinkHostAddress").c_str();

  if (!IsConnected())
    if (!this->OpenConnection())
      mEnable = false;

  //${SubjectName}${SubjectSession} / ${SubjectName}S${SubjectSession}R${SubjectRun}.${FileFormat}

  //open data file
  if (mSaveEDF)
  {
    std::string stringFile = Parameter("DataDirectory").ToString() + "/" +
      Parameter("SubjectName").ToString() +
      Parameter("SubjectSession").ToString() + "/" +
      //Parameter("SubjectName").ToString() + "S" +
      //Parameter("SubjectSession").ToString() + "R" +
      Parameter("SubjectRun").ToString() + ".edf";

    //check/create file path
    if (create_path(ToChar(stringFile), 1, 0) == -1)
    {
      bcierr << "Could not create path";
    }
    std::string sFile = Parameter("SubjectRun").ToString() + ".edf";
    mFileName = new char[sFile.length() + 1];
    std::strcpy(mFileName, sFile.c_str());
    //open the file
    if (open_data_file(mFileName))
    {
      bcierr << "Could not open data file";
    }
  }

  //find tracker version for later data analysis
  if (eyelink_get_tracker_version(NULL) == 0)
    mTrackerVersion = "Not connected";
  else if (eyelink_get_tracker_version(NULL) == 1)
    mTrackerVersion = "EyeLink I";
  else if (eyelink_get_tracker_version(NULL) == 2)
    mTrackerVersion = "EyeLink II";
  else if (eyelink_get_tracker_version(NULL) == 3)
    mTrackerVersion = "EyeLink 1000";

  //send pixel coordinates
  std::string width = std::to_string(Parameter("WindowWidth") - 1);
  std::string height = std::to_string(Parameter("WindowHeight") - 1);
  std::string coords = "screen_pixel_coords = 0, 0, " + width + ", " + height;
  eyelink_send_command(ToChar(coords));

  //specify which type of data we are recording
  //coords = "recording_parse_type = ";
  //std::string coords2 = "analog_out_data_type = ";
  coords = "link_sample_data = LEFT,RIGHT,AREA,BUTTON,GAZERES,INPUT,RAW,STATUS,HTARGET,PUPIL,";
  int dataType = Parameter("DataType");
  if (dataType == 1)
  {
    coords += "GAZE";
  }
  else if (dataType == 0)
  {
    coords += "HREF";
  }
  eyelink_send_command(ToChar(coords));

  //if experimental set-up is changed
  if (Parameter("ExperimentParams")(0, 0)) {
    //send physical coordinates 
    coords = "screen_phys_coords = ";
    for (int i = 0; i < 2; i++)
    {
      width = std::to_string(pow(-1, i + 1) * Parameter("ExperimentParams")(2, 0) / 2);
      height = std::to_string(pow(-1, i) * Parameter("ExperimentParams")(3, 0) / 2);
      coords += width + ", " + height;
      if (i == 0) coords += ", ";
    }
    eyelink_send_command(ToChar(coords));

    //non-remote mode
    if (!Parameter("ExperimentParams")(1, 0)) {
      width = std::to_string(Parameter("ExperimentParams")(4, 0)); //to top
      height = std::to_string(Parameter("ExperimentParams")(5, 0)); //to bottom
      eyelink_send_command(ToChar("screen_distance = " + width + height));
    }
    else //remote mode
    {
      //width = std::to_string(Parameter("ExperimentParams")(6, 0)); //focal length
      //eyelink_send_command(ToChar("camera_lens_focal_length = " + width));

      width = std::to_string(Parameter("ExperimentParams")(6, 0)); //screen to lens dist
      //the 4 other numbers are EyeLink's defaults, they seem like they're never configured
      eyelink_send_command(ToChar("remote_camera_position = -10 17 80 60 -" + width));
    }
  }

  Parameter("EyeLinkInfo")(1, 0) = mTrackerVersion;

  //record experiment specific details
  get_display_information(&mDispinfo);

  Parameter("EyeLinkInfo")(3, 0) = mDispinfo.width;
  Parameter("EyeLinkInfo")(4, 0) = mDispinfo.height;
  Parameter("EyeLinkInfo")(5, 0) = mDispinfo.refresh;

  Parameter("EyeLinkInfo")(2, 0) = eyelink_position_prescaler();
}

void
EyetrackerLoggerEyeLink::StartRun()
{
  if (!mEnable) return;

  if (mSaveEDF)
    mRecordStatus = start_recording(1, 1, 1, 1); //record all data
  else
    mRecordStatus = start_recording(0, 0, 1, 0); //only send samples thru link
  if (mRecordStatus != 0)
  {
    bcierr << "EyeLinkError: Unable to start recording" << endl;
  }
  begin_realtime_mode(100);
  if (!eyelink_wait_for_block_start(100, 1, 0)) //wait for link sample data
  {
    bcierr << "EyeLink Error: No link samples received!" << endl;
  }
  mCheck_eye = eyelink_eye_available();

  if ((m_desired_eye != mCheck_eye) && (m_desired_eye != 2))
  {
    bcierr << "EyeLink Error: Desired eye is not being recorded. Check the EyeLink computer to see which eye it is recording" << endl;
  }

  //for later analysis
  if (mCheck_eye == RIGHT_EYE)
  {
    Parameter("EyeLinkInfo")(0, 0) = "Right eye";
    mSide = "Right";
  }
  else if (mCheck_eye == LEFT_EYE)
  {
    Parameter("EyeLinkInfo")(0, 0) = "Left eye";
    mSide = "Left";
  }
  else
    Parameter("EyeLinkInfo")(0, 0) = "Both eyes";

  // start thread
  Start();
}


int
EyetrackerLoggerEyeLink::OnExecute()
{
  if (!mEnable) return 1;

  chrono::time_point<chrono::system_clock> start_aq_time = chrono::system_clock::now();
  mCounter = 0;

  //chrono::duration_cast<std::chrono::milliseconds>(chrono::system_clock::now() - start_aq_time).count();

  while (!Terminating())
  {
    if (!IsConnected())
    {
      bcierr << "Eyelink not connected";
      return -1;
    }

    if (eyelink_newest_sample(NULL) > 0)  // check for new sample update
    {
      mTimer = chrono::duration_cast<std::chrono::milliseconds>(chrono::system_clock::now() - start_aq_time).count();
      mCounter++;

      mPrevPrescaler = eyelink_position_prescaler();

      //Update new data
      eyelink_get_sample(&misample);

      //Get timestamp
      bcievent << "EyetrackerTime " << misample.time << std::flush;

      //Gaze resolution isn't eye dependent
      mResX = misample.rx;
      mResY = misample.ry;

      //LEFT EYE
      if ((mCheck_eye == LEFT_EYE) || (mCheck_eye == BINOCULAR))
      {
        ProcessEye(LEFT_EYE);
      }

      //RIGHT EYE
      if ((mCheck_eye == RIGHT_EYE) || (mCheck_eye == BINOCULAR))
      {
        ProcessEye(RIGHT_EYE);
      }

      if (mPrevPrescaler != eyelink_position_prescaler())
        bcierr << "Prescaler has changed!";
    }
    //else
    //{
    //  Sleep(0);
    //}

    //if (!OptionalState("GazeCorrectionMode", 0))
    //{
    //  // Update stimuli and continue performing task related things
    //}
    //if (OptionalState("FixationViolated", 0))
    //{
    //  // React to the fixation being violated
    //  bciwarn << "Fixation has been violated";
    //}
  }
  return 0;
}

void
EyetrackerLoggerEyeLink::StopRun()
{
  if (!mEnable) return;

  stop_recording();

  // stop thread
  TerminateAndWait();
}

void
EyetrackerLoggerEyeLink::Halt()
{
  // stop connection
  if (IsConnected())
    this->CloseConnection();
}

void
EyetrackerLoggerEyeLink::ProcessEye(bool whichEye)
{
  mValidity = 0;
  mPosX = misample.px[whichEye];
  mPosY = misample.py[whichEye];
  mGazX = misample.gx[whichEye] / eyelink_position_prescaler();
  mGazY = misample.gy[whichEye] / eyelink_position_prescaler();
  mP = misample.pa[whichEye];
  mHX = misample.hx[whichEye];
  mHY = misample.hy[whichEye];
  int offset = 0x7FFF;
  //gaze position
  if ((mGazX == MISSING_DATA / eyelink_position_prescaler())
    || (mGazY == MISSING_DATA / eyelink_position_prescaler()))
  {
    //bciwarn << "Missing gaze position data";
    mValidity += pow(2, 0);
  }
  else
  {
    //switch negative numbers to unsigned ints
    if (mGazX < 0) mGazX = -mGazX + offset;
    if (mGazY < 0) mGazY = -mGazY + offset;

    bcievent << "Eyetracker" << mSide << "EyeGazeX " << mGazX << std::flush;
    bcievent << "Eyetracker" << mSide << "EyeGazeY " << mGazY << std::flush;
  }

  //pupil size
  if (mP == 0)
  {
    //bciwarn << "Missing pupil size data";
    mValidity += pow(2, 1);
  }
  else
  {
    bcievent << "Eyetracker" << mSide << "EyePupilSize " << mP << std::flush;
  }

  //camera eye position
  if ((mPosX == MISSING_DATA) || (mPosY == MISSING_DATA))
  {
    //bciwarn << "Missing gaze position data";
    mValidity += pow(2, 2);
  }
  else
  {
    //switch negative numbers to unsigned ints
    if (mPosX < 0) mPosX = -mPosX + offset;
    if (mPosY < 0) mPosY = -mPosY + offset;
    bcievent << "Eyetracker" << mSide << "EyePosX " << mPosX << std::flush;
    bcievent << "Eyetracker" << mSide << "EyePosY " << mPosY << std::flush;
  }

  //headRef
  if ((mHX == MISSING_DATA) || (mHY == MISSING_DATA))
  {
    //bciwarn << "Missing headREF data";
    mValidity += pow(2, 3);
  }
  else
  {
    //switch negative numbers to unsigned ints
    if (mHX < 0) mHX = -mHX + offset;
    if (mHY < 0) mHY = -mHY + offset;

    bcievent << "Eyetracker" << mSide << "EyeHRefX " << mHX << std::flush;
    bcievent << "Eyetracker" << mSide << "EyeHRefY " << mHY << std::flush;
  }

  //gaze resolution
  if ((mResX == MISSING_DATA) || (mResY == MISSING_DATA))
  {
    //bciwarn << "Missing res: rx: " << resX << ", ry: " << resY;
    mValidity += pow(2, 4);
  }
  else
  {
    //switch negative numbers to unsigned ints
    if (mResX < 0) mResX = -mResX + offset;
    if (mResY < 0) mResY = -mResY + offset;
    bcievent << "EyetrackerEyeResX " << mResX << std::flush;
    bcievent << "EyetrackerEyeResY " << mResY << std::flush;
  }

  //validity
  bcievent << "Eyetracker" << mSide << "EyeValidity " << mValidity << std::flush;

}

bool
EyetrackerLoggerEyeLink::OpenConnection()
{
#ifndef EYELINK_DUMMY
  if (set_eyelink_address(mHostAddress))
  {
    bcierr << "EyeLink Error: Unable to set host address" << endl;
    return false;
  }
  if (open_eyelink_connection(0))
  {
    bcierr << "EyeLink Error: Unable to open connection to EyeLink" << endl;
    return false;
  }
  return true;
#else
  if (set_eyelink_address(mHostAddress))
  {
    bcierr << "EyeLink Error: Unable to set host address" << endl;
    return false;
  }
  if (open_eyelink_connection(0))
  {
    bcierr << "EyeLink Error: Unable to open connection to EyeLink" << endl;
    return false;
  }
  return true;
#endif
}


void
EyetrackerLoggerEyeLink::CloseConnection()
{
  end_realtime_mode();
  pump_delay(100);
  stop_recording();

  if (mSaveEDF)
  {
    close_data_file();
    if (!receive_data_file((char*)"", mPathName, 0))
    {
      bcierr << "File transfer was cancelled";
    }
  }

  close_eyelink_connection();
}

bool
EyetrackerLoggerEyeLink::IsConnected()
{
  return eyelink_is_connected();
}

char*
EyetrackerLoggerEyeLink::ToChar(std::string stringIn)
{
  char* tempChar = new char[stringIn.length() + 1];
  std::strcpy(tempChar, stringIn.c_str());
  return tempChar;
}