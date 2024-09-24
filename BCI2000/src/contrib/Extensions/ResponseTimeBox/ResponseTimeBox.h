/////////////////////////////////////////////////////////////////////////////
// $Id: ResponseTimeBox.h 7463 2023-06-30 14:59:17Z mellinger $
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
#ifndef RESPONSE_BOX_H
#define RESPONSE_BOX_H

//#include "GenericFilter.h"
#include "Environment.h"
#include "Thread.h"
#include "Expression/Expression.h"
#include "SerialPort.h"
#include "Win32Defs.h"
#include <vector>
#include <chrono>



class ResponseTimeBox : public EnvironmentExtension, public Thread
{
 public:

  //Constructors and virtual interface
   ResponseTimeBox();
  ~ResponseTimeBox();
  void Publish() override;
  void AutoConfig() override;
  void Preflight() const override;
  void Initialize() override;
  void StartRun() override;
  void StopRun() override;
  void Halt() override;
  

 private:

   // Inherited from Thread
   virtual int OnExecute() override;
   int timeStamp1;
   int timeStamp2;
   double ratio;
   
   /*int counter = 0;*/
   long long int ContTime;
   /*double seconds = 0;
   long long int SubContTime=0;
   int count = 1;*/
   /*std::chrono::steady_clock::time_point firsttime;
   std::chrono::steady_clock::time_point updatetime;*/
   int TTLEventCode = 0;
   bool TTLEnabled = 0;
   bool mEnable;
   bool mEnableB;
   bool mEnableS;
   bool mEnableL;
   bool mEnableA;
   int EnabledStates;
   int testLight = 1;
   std::vector<int> m_capture;
   int StimulusCode = 0;
   SerialPort* mDevice;
};

#endif // EYETRACKER_LOGGER_EYE_LINK_H
