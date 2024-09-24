/////////////////////////////////////////////////////////////////////////////
// $Id: ResponseTimeBox.cpp 7993 2024-04-04 20:44:05Z mellinger $
// Authors:  Ryan Gladwell (gladwell@neurotechcenter.org)
// Description: Response Box implementation to BCI2000 and
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
#include "ResponseTimeBox.h"
#include "Win32Defs.h"
#include <vector>


using namespace std;

Extension(ResponseTimeBox);

ResponseTimeBox::ResponseTimeBox() :
    mEnable(false),
    mDevice(nullptr) 
{
    PublishEnabler("LogResponseTimeBox");
}

ResponseTimeBox::~ResponseTimeBox()
{
}

void
ResponseTimeBox::Publish()
{
    if (!OptionalParameter("LogResponseTimeBox", 0))
        return;
  BEGIN_PARAMETER_DEFINITIONS
    "Source:ResponseTimeBox int /LogResponseTimeBox= 0 0 0 1"
    " // Enable response box (boolean)",
    "Source:ResponseTimeBox int RTButtonPress= 1 1 0 1"
      " // Enable button states (boolean)",
      "Source:ResponseTimeBox int RTLight= 1 1 0 1"
      " // Enable light detection (boolean)",
      "Source:ResponseTimeBox int RTSound= 1 1 0 1"
      " // Enable sound/pulse detection (boolean)",
  
      "Source:ResponseTimeBox int RTSensitivity= 2 2 1 4"
      " // Sound and light sensitivity: 1 Most Sensitive 2 More 3 Less 4 Least Sensitive (enumeration)",
    
      //defne active COM Port
     "Source:ResponseTimeBox string RTCOMPort= auto % % %// COM port the USB Hub is connected to",
    END_PARAMETER_DEFINITIONS



  //define any states
  BEGIN_EVENT_DEFINITIONS
      "RTButton1 1 0",
      "RTButton2 1 0",
      "RTButton3 1 0",
      "RTButton4 1 0",
      "RTEventTime1 24 0",
      "RTEventTime2 24 0",
      "RTConTime1 24 0",
      "RTConTime2 24 0",
      "RTSoundEvent 1 0",
      "RTLightEvent 8 0",
      "RTAuxEvent 1 0"
     
  END_EVENT_DEFINITIONS
}
void

ResponseTimeBox::AutoConfig()
{
  if (!OptionalParameter("LogResponseTimeBox", 0)) {
      return;
  }

  if (Parameter("RTCOMPort")=="auto") {

      bciwarn << "Checking Comports for RTBox..." << std::endl;
      for (int i = 1; i < 20; i++)
      {
          if (mDevice != nullptr)
          {
              delete mDevice;
              mDevice = nullptr;
          }
          std::string COMprefix = "\\\\.\\";
          std::string ComNum = "COM" + std::to_string(i);
          mDevice = new SerialPort((COMprefix + ComNum).c_str());
          if (mDevice != nullptr)
          {
              // check if the serial port is connected
              bool connected = mDevice->isConnected();
              if (connected) {
              }
              else {
                  continue;
              }      
          }
          else {  
              continue;
          }

          char getFirmwareVersion[] = { UINT8(88),'\n' };
          bool res = mDevice->writeSerialPort(getFirmwareVersion, sizeof(getFirmwareVersion));
          if (!res) {
              continue;
          }
          Sleep(2);
          char buffer[255];
          int bytes_read = mDevice->readSerialPort(buffer, sizeof(buffer));
          if (bytes_read == 0) {
                  continue;
              }
          std::string ID;
          for (int j = 0; j < 9; j++)
          {
              ID.push_back(buffer[j]);
          }
          bciout << ID << std::endl;
          std::string expectedID = "USTCRTBOX";
          if (ID.compare(expectedID) == 0) {
              Parameter("RTCOMPort") = "COM" + std::to_string(i);
              bciout << "Success! Connected to COMPort" << i << std::endl;
              break;
          }
      }
  }
}


void
ResponseTimeBox::Preflight() const
{
    if (!OptionalParameter("LogResponseTimeBox", 0)) return;
    
    Parameter("RTCOMPort");
    
  
}


void
ResponseTimeBox::Initialize()
{
  mEnable = OptionalParameter("LogResponseTimeBox", 0);
  if (!mEnable) return;

  EnabledStates = 48;
  //Initializing button states as off
 
  if (Parameter("RTButtonPress") == 1) {
    EnabledStates += 3;
  }
  if (Parameter("RTSound") == 1) {
    EnabledStates += 4;
  }
  if (Parameter("RTLight") == 1) {
    EnabledStates += 8;
  }


  
  if (mDevice != nullptr)
  {
    delete mDevice;
    mDevice = nullptr;
  }
  std::string COMprefix = "\\\\.\\";
  std::string ComNum = Parameter("RTCOMPort");
  mDevice = new SerialPort((COMprefix + ComNum).c_str());
  char getFirmwareVersion[] = { UINT8(101) ,63, '\n' };
  bool res = mDevice->writeSerialPort(getFirmwareVersion, sizeof(getFirmwareVersion));
  
  if (!res)
    bcierr << "Response Box Error: Unable to send command to hub" << std::endl;
  Sleep(2);
  char buffer[255];
  //This reads the button state information, sets to advanced mode, and sets the sensibtivity

  char ButtonStates[] = { UINT8(69), UINT8(88),'\n' };
  bool res1 = mDevice->writeSerialPort(ButtonStates, sizeof(ButtonStates));
  if (!res1)
    bcierr << "Response Box Error: Unable to send command to hub" << std::endl;
  Sleep(2);

  int bytes_read = mDevice->readSerialPort(buffer, sizeof(buffer));
  if (bytes_read == 0)
    bcierr << "ResponseTimeBox Error: No response from hub" << std::endl;


  
  std::string ID;
  for (int j = 3; j < 24; j++)
  {
    ID.append(to_string(buffer[j]));
  }
  bciout << ID << std::endl;



  int initializeSense = 2;

  if ((int)Parameter("RTSensitivity") == 1) {
    initializeSense = 0;
  }
  if ((int)Parameter("RTSensitivity") == 2) {
    initializeSense = 8;
  }
  if ((int)Parameter("RTSensitivity") == 3) {
    initializeSense = 64;
  }
  if ((int)Parameter("RTSensitivity") == 4) {
    initializeSense = 72;
  }
  
  

  char setsensitivity[] = { UINT8(16),UINT8(225), UINT8(1),UINT8(initializeSense),UINT8(3), UINT8(2), UINT8(17),224,6,'\n' };
  bool res2 = mDevice->writeSerialPort(setsensitivity, sizeof(setsensitivity));
  if (!res2)
    bcierr << "response box error: unable to send command to hub" << std::endl;
  Sleep(2);
  char buffer1[255];
  int bytes_read1 = mDevice->readSerialPort(buffer1, sizeof(buffer1));

  
  char SetTTLWidth[] = { UINT8(16),UINT8(224), UINT8(1),UINT8(0),UINT8(3), UINT8(2), UINT8(17),224,6,'\n' };

  bool res3 = mDevice->writeSerialPort(SetTTLWidth, sizeof(SetTTLWidth));
  if (!res3)
    bcierr << "response box error: unable to send command to hub" << std::endl;
  Sleep(2);
  char buffer2[255];
  int bytes_read2 = mDevice->readSerialPort(buffer2, sizeof(buffer2));
 
}


void
ResponseTimeBox::StartRun()
{
    if (!mEnable) return;
    for (int i = 0; i < 4; i++)
    {
      bcievent << "RTButton" + std::to_string(i + 1) + " " << 0;
    }
    
    //vars for time checks
    /*seconds = 0;
    count = 1;
    counter = 0;*/
    Start();
}


int
ResponseTimeBox::OnExecute()
{
    if (!mEnable) return 1;

    while (!Terminating())
    {
      
        char ButtonStates1[] = { UINT8(69),UINT8(89), '\n'};

        bool res11 = mDevice->writeSerialPort(ButtonStates1, sizeof(ButtonStates1));
        if (!res11)
            bcierr << "Response Box Error: Unable to send command to hub" << std::endl;
        Sleep(2);
       
        
        
        char buffer[255];
        int bytes_read = mDevice->readSerialPort(buffer, sizeof(buffer));
     
        if (bytes_read != 0) {
            int newL = m_capture.size() + bytes_read;
            vector<int> newVec(newL);
            for (int i = 0; i < m_capture.size(); i++)
            {
                newVec[i] = m_capture[i];
            }
            for (int i = 0; i < bytes_read; i++)
            {
                newVec[i + m_capture.size()] = buffer[i];
            }
            ;
            
            while (size(newVec) > 6) {



              if (newVec[0] == 89) {

                //attempt to sync box and system time
                /*if (count == 1) {
                  SubContTime = (intmax_t)((long long)newVec[1] << 40) | (((long long)newVec[2]) << 32) | ((long long)newVec[3] << 24) | ((long long)newVec[4] << 16) | ((long long)newVec[5] << 8) | ((long long)newVec[6]);
                  count += 1;
                  firsttime=std::chrono::steady_clock::now();
                }*/

                timeStamp2 = (int)((newVec[4] << 16) | (newVec[5] << 8) | (newVec[6]));
                timeStamp1 = (int)((newVec[1] << 16) | (newVec[2] << 8) | (newVec[3]));
                bcievent << "RTConTime2 " << timeStamp2;
                bcievent << "RTConTime1 " << timeStamp1;


                //absolute value of time
                //ContTime = (intmax_t)((long long)newVec[1] << 40)  | (((long long)newVec[2]) << 32)  | ((long long)newVec[3] << 24)  | ((long long)newVec[4] << 16) | ((long long)newVec[5] << 8)  | ((long long)newVec[6]);

                //System time update
                /* updatetime= std::chrono::steady_clock::now();
                counter += 1;*/

                //This attempted to convert box clock to seconds-difficult, could not find an exact ratio between a unit of box time and seconds
                 /*seconds= seconds + double(ContTime - SubContTime) / 921645;
                 bciout << "time " << seconds;*/

                 //This retrieved the system clock time to compare to the box time
                /* auto SystemSeconds=chrono::duration_cast<chrono::milliseconds>(updatetime - firsttime).count();
                 bciout << "System Clock " << (double)SystemSeconds/1000;
                 bciout << "Difference in box and system " << (double)(((double)SystemSeconds/1000) - (double)seconds);
                 bciout << "Rate differece "<< ((double)(((double)SystemSeconds / 1000) - (double)seconds))/(double)counter;*/



                 //Part of time test
                 //SubContTime = (intmax_t)((long long)newVec[1] << 40) | (((long long)newVec[2]) << 32) | ((long long)newVec[3] << 24) | ((long long)newVec[4] << 16) | ((long long)newVec[5] << 8) | ((long long)newVec[6]);


                newVec.erase(newVec.begin(), newVec.begin() + 7);
                continue;

              }

              if (newVec[0] == 69) {
                if (newVec[1] != EnabledStates) {
                  char getFirmwareVersion[] = { UINT8(101) ,EnabledStates, '\n' };
                  bool res = mDevice->writeSerialPort(getFirmwareVersion, sizeof(getFirmwareVersion));
                }

                newVec.erase(newVec.begin(), newVec.begin() + 2);
                continue;
              }
              if (newVec[0] == 101) {
                newVec.erase(newVec.begin());
                continue;
              }

              for (int i = 0; i < 7; i++)
              {
                if (i == 0 && (newVec[i] > 48 && newVec[i] < 57)) {
                  newVec[i] = newVec[i] - 49;
                }

              }
              string t;
             
                bool timeSet = 0;
               
                
                if ((newVec[0] >= 0 && newVec[0] <= 7)) {
                    for (int i = 1; i < 8; i = i + 2)
                    {
                        
                        if (i - newVec[0] == 1 || i - newVec[0] == 0) {
                            bcievent << "RTButton" << to_string((int)(i / 2) + 1) << " " << i - newVec[0] << std::flush;
                            
                            timeSet = 1;
                            break;
                            
                        }
                        
                    }
                }
                else if (newVec[0] == 48) {
                    
                   bcievent << "RTLightEvent " << 1 <<" "<<0<< std::flush;
                   
                   timeSet = 1;
                }
                else if (newVec[0] == 97) {
                    bcievent << "RTSoundEvent " << 1 <<" "<<0<< std::flush;
                    
                    timeSet = 1;
                }
                else if (newVec[0] == 57) {
                    
                    timeSet = 1;
                } 
                else if (newVec[0] == 98) {
                    bcievent << "RTAuxEvent " << 1<< " "<< 0<< std::flush;
                    
                    timeSet = 1;
                }
               
                
                if (timeSet) {
                  bcievent << "RTEventTime2 " << (int)((newVec[4] << 16) | (newVec[5] << 8) | (newVec[6])) << " " << 0 << std::flush;
                  bcievent << "RTEventTime1 " << (int)((newVec[1] << 16) | (newVec[2] << 8) | (newVec[3])) << " " << 0 << std::flush;
                }

               
                
                newVec.erase(newVec.begin(), newVec.begin() + 7);
                
            }
            
            m_capture = newVec;
        }
    }
  return 0;
}

void
ResponseTimeBox::StopRun()
{
  if (!mEnable) return;

  TerminateAndWait();
}

void
ResponseTimeBox::Halt()
{
}


