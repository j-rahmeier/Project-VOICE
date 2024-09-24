////////////////////////////////////////////////////////////////////////////////
// $Id: DSISerialADC.h 7195 2023-02-07 18:31:03Z mellinger $
// Authors:     Jeremy Hill <jezhill@gmail.com>
// Description: DSISerialADC class header file.
//              Acquires data from Dry Sensor Interface (DSI) headsets by
//              Wearable Sensing LLC / Quasar USA.  Tested with Wearable Sensing
//              DSI7 and DSI24.  May also work with the ANI-SI headset by
//              Advanced Neurometrics, Inc. The dynamic library from DSI_API is
//              used to make a direct serial-port connection to the headset (as
//              distinct from the TCP/IP streaming approach via the DSIStreamer
//              application). For best results, set the DSISerialPort
//              environment variable, or use the --DSISerialPort command-line
//              parameter on launch: this allows auto-configuration of most of
//              the necessary BCI2000 parameters using data downloaded from the
//              headset.
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
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_DSISerialADC_H  // makes sure this header is not included more than once
#define INCLUDED_DSISerialADC_H

#include "BufferedADC.h"
#include "Thread.h"
#include "ThreadUtils.h" // for SleepFor()
#include "DSI.h"

class AcquisitionThread: public Thread
{
 public:
  AcquisitionThread( DSI_Headset headset ) { mHeadset = headset; }
 private:
  DSI_Headset mHeadset;
  virtual int OnExecute() override;
};

class DSISerialADC : public BufferedADC
{

 public:
           DSISerialADC();
  virtual ~DSISerialADC();

  void OnPublish () override;
  void OnAutoConfig() override;
  void OnHalt() override;
  void OnPreflight( SignalProperties& Output ) const override;
  void OnInitialize( const SignalProperties& Output ) override;
  void OnStartAcquisition() override;
  void DoAcquire( GenericSignal& Output ) override;
  void OnStopAcquisition() override;
  void OnProcess() override;
  //void Publish() override { OnPublish(); }
  //void AutoConfig() override { OnAutoConfig(); }
  //void Halt() override { OnStopAcquisition(); OnHalt(); }
  //void Preflight(  const SignalProperties & Input, SignalProperties & Output ) const override { OnPreflight( Output ); }
  //void Initialize( const SignalProperties & Input, const SignalProperties & Output ) override { OnInitialize( Output ); OnStartAcquisition(); }
  //void Process( const GenericSignal & Input, GenericSignal & Output ) override { DoAcquire( Output ); }

 private:
  mutable DSI_Headset  mHeadset;
  AcquisitionThread *  mThread;
  size_t               mOverflow;
  size_t               mBlock;
  size_t               mBlockZero;
  double               mPrecisionTimeZero;
  size_t               mSamplesPerBlock;
  double               mNominalSecondsPerBlock;
  double               mNominalSamplesPerSecond;
  double               mEstimatedSamplesPerSecond;
  double               mPendingSamplesPerSecond;
  double               mPreviousBufferingError;
  double               mCumulativeBufferingError;
  size_t               mBufferAhead;
  double               mRemainder;
  int                  mBatteryLevel1;
  int                  mBatteryLevel2;
  size_t               mBlocksSinceBatteryChecked;
  size_t               mBufferingController_PeriodInBlocks;
  double               mBufferingController_Smoothing;
  double               mBufferingController_Gain;
  double               mBufferingController_P;
  double               mBufferingController_I;
  double               mBufferingController_D;
  int                  mAlarm;
  int                  mBufferedSamplesAfterRead;
  bool                 mPerformReset;
  std::string          mDllName;

  GenericSignal *        mImpedances;
  GenericVisualization * mVis;
  std::string            Join( const char * paramName ) const;
};

#endif // INCLUDED_DSISerialADC_H
