/////////////////////////////////////////////////////////////////////////////
// $Id: EyetrackerLoggerEyeLink.h 7463 2023-06-30 14:59:17Z mellinger $
// Authors: Alexander Belsten (belsten@neurotechcenter.org)
// Description: 
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
#ifndef EYETRACKER_LOGGER_EYE_LINK_H
#define EYETRACKER_LOGGER_EYE_LINK_H

#include "Environment.h"
#include "Thread.h"
#include "Lockable.h"
#include "Expression/Expression.h"
#include "include/core_expt.h"
#include "include/gdi_expt.h"
#include <chrono>

class EyetrackerLoggerEyeLink : public EnvironmentExtension, public Thread
{
 public:

  //Constructors and virtual interface
   EyetrackerLoggerEyeLink();
  ~EyetrackerLoggerEyeLink();
  void Publish() override;
  void AutoConfig() override;
  void Preflight() const override;
  void Initialize() override;
  void StartRun() override;
  void StopRun() override;
  void Halt() override;
  void ProcessEye(bool whichEye);

 private:

   // Inherited from Thread
   virtual int OnExecute() override;

   bool OpenConnection(),
        IsConnected();
   void CloseConnection();
   char* ToChar(std::string stringIn);

   ISAMPLE          misample;
   DISPLAYINFO      mDispinfo;

   
   float            mTimer, 
                    mCounter;
   int              mCheck_eye,
                    mPrevPrescaler,
                    mRecordStatus;
   size_t          m_desired_eye;
    
   UINT16            mValidity; 
   UINT16           mP;

   INT32            mPosX, 
                    mPosY, 
                    mGazX, 
                    mGazY,
                    mHX, 
                    mHY, 
     mResX,
     mResY;



   char*            mHostAddress;
   char*            mFileName;
   char*            mPathName;
   bool             mEnable, mSaveEDF;
   std::string      mSide,
                    mTrackerVersion;
               
};

#endif // EYETRACKER_LOGGER_EYE_LINK_H
