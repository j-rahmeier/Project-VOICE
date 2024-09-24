////////////////////////////////////////////////////////////////////////////////
// $Id: BlackrockADC.h 8067 2024-04-24 12:17:05Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: Implementation of a source module for Blackrock systems
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
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_BLACKROCKADC_H
#define INCLUDED_BLACKROCKADC_H

#include "BufferedADC.h"
#include "PrecisionTime.h"
#include "Expression.h"
#include "WaitableEvent.h"

#include <Windows.h>
#include "cbsdk.h"
// ===========================================================================
// Added this from from the old cbSDK, not sure if griffin added this to the 
// old SDK or if blackrock removed it. - Alex
#define cbFIRST_ANAIN_CHAN    cbNUM_FE_CHANS                             // 256 First Analog Input channel
#define cbFIRST_ANAOUT_CHAN   (cbFIRST_ANAIN_CHAN + cbNUM_ANAIN_CHANS)   // 288 First Analog Output channel
#define cbFIRST_AUDOUT_CHAN   (cbFIRST_ANAOUT_CHAN + cbNUM_ANAOUT_CHANS) // 296 First Audio Output channel
#define cbFIRST_DIGIN_CHAN    (cbFIRST_AUDOUT_CHAN + cbNUM_AUDOUT_CHANS) // 300 First Digital Input channel
#define cbFIRST_SERIAL_CHAN   (cbFIRST_DIGIN_CHAN + cbNUM_DIGIN_CHANS)   // 302 First Serial Input channel
#define cbFIRST_DIGOUT_CHAN   (cbFIRST_SERIAL_CHAN + cbNUM_SERIAL_CHANS) // 304 First Digital Output channel
// ===========================================================================


#include <vector>
#include <queue>
#include <mutex>

class BlackrockADC : public BufferedADC
{
 public:
           BlackrockADC();
  virtual ~BlackrockADC();
  virtual void OnAutoConfig();
  virtual void OnHalt();
  virtual void OnPreflight( SignalProperties& Output ) const;
  virtual void OnInitialize( const SignalProperties& Output );
  virtual void OnProcess();
  virtual void StartRun();
  virtual void OnStartAcquisition();
  virtual void DoAcquire( GenericSignal& Output );
  virtual void OnStopAcquisition();
  virtual void StopRun();

  // Bit of a hack necessary for digital output based on expressions
  virtual void Process( const GenericSignal& Input, GenericSignal& Output );

 private:
  struct ChanInfo
  {
    unsigned int inst;
    unsigned int idx;
    double gain;
    double offset;
    std::string label;
  };

  bool Connect( int nInstances = 1 ) const;
  void Disconnect( int nInstances = 1 ) const;
  bool GetChannelConfig( int iNumInstances, int iGroup,
                         std::vector< ChanInfo > &oChanConfig,
                         std::vector< int > &oSyncChans ) const;
  static void DataCallback( UINT32 iInstance, const cbSdkPktType iType, const void* iData, void* iBlackrockADC );
  static bool CereLinkError( cbSdkResult res );
  int GetRequestedSampleGroup() const;
  double ScalingToGain( cbSCALING scaling ) const;

  // Debug Functionality
  void OutputChannelDebugInfo( int nInstances ) const;

  std::recursive_mutex mDataMutex, mCommandMutex;
  std::vector< std::queue< std::vector< INT16 > > > mDataPacketBuffers;
  std::vector< ChanInfo > mChannelConfig;
  std::vector< int > mSyncChans;
  int mSampleGroup;
  unsigned int mNSPInstances;
  unsigned int mSampleBlockSize;
  bool mSyncEnabled, mPauseForSync;
  WaitableEvent mDataAvailable;

  struct DigitalOutputExpression
  {
    unsigned int inst;
    unsigned int dout;
    Expression exp;
  };
  std::vector< DigitalOutputExpression > mDigitalOutputEx;

};

#endif // INCLUDED_BLACKROCKADC_H
