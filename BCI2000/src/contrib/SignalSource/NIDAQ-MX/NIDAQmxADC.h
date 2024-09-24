/* $BEGIN_BCI2000_LICENSE$
 * 
 * This file is part of BCI2000, a platform for real-time bio-signal research.
 * [ Copyright (C) 2000-2023: BCI2000 team and many external contributors ]
 * 
 * BCI2000 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * BCI2000 is distributed in the hope that it will be useful, but
 *                         WITHOUT ANY WARRANTY
 * - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * $END_BCI2000_LICENSE$
/*/
//---------------------------------------------------------------------------

#ifndef NImxADCH
#define NImxADCH
//---------------------------------------------------------------------------

#include "NIDAQmx.imports.h"
#include "GenericADC.h"

#include <mutex> // C++11
#include <condition_variable> // C++11


#define NIDAQ_ERR_GENERICERR    0
#define NIDAQ_ERR_NOERR         1

#define NIDAQ_MAX_CHANNELS      128
#define NIDAQ_MAX_BUFFERS       50      // how many buffers do we want ?

#define NIDAQ_iDBmodeON         1
#define NIDAQ_iDBmodeOFF        0

#define NIDAQ_MODE_CLEARALLMESSAGES   0
#define NIDAQ_MODE_ADDMESSAGE         1
#define NIDAQ_MODE_REMOVEMESSAGE      2
  
class NIDAQmxADC : public GenericADC
{
  private:
    int16      mDeviceNumber;
    char       mDeviceName[ 16 ];   // the name of the device (specified by board number)
  
    int        mSamplesPerSecond;
    int        mSamplesPerBlock;
    double     mSecondsPerBlock;
    int        mNumberOfChannels;

    int32      mResultCode;
    char       mErrorMessageBuffer[ 2048 ];
    TaskHandle mTaskHandle;         // NIDAQ Handle to task  
    int        mNumberOfBuffersQueued;
  
    std::mutex              mDataMutex;
    std::condition_variable mConditionVariable;
  
    int16    * mHalfBuffers[ NIDAQ_MAX_BUFFERS ];  

    static  int32 CVICALLBACK Callback( TaskHandle, int32, uInt32, void * );
  
    int     Start();
    int     Stop();
    int     GetData();
  
  public:
    NIDAQmxADC();
    ~NIDAQmxADC();
    virtual void     Publish();
    virtual void     AutoConfig(const SignalProperties&);
    virtual void     Preflight(const SignalProperties&, SignalProperties&) const;
    virtual void     Initialize(const SignalProperties&, const SignalProperties&);
    virtual void     Process(const GenericSignal&, GenericSignal&);
    virtual void     Halt();

};

#endif
