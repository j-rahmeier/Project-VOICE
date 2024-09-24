////////////////////////////////////////////////////////////////////////////////
// $Id: actiCHampPlusADC.h 7463 2023-06-30 14:59:17Z mellinger $
// Authors: Alex Belsten belsten@neurotechcenter.org
// Description: actiCHampPlusADC header
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

#ifndef INCLUDED_actiCHampPlusADC_H 
#define INCLUDED_actiCHampPlusADC_H

#include <map>
#include <queue>
#include <concurrent_queue.h>
#include "BufferedADC.h"
#include "AmplifierSDK.imports.h"
#include "RawDataHandlerExample.h"
#include "SDK.h"

#define SAMPLING_RATE_10KHZ   10000.0
#define SAMPLING_RATE_50KHZ   50000.0
#define SAMPLING_RATE_100KHZ 100000.0


using namespace concurrency;

class actiCHampPlusADC : public BufferedADC
{
 public:
  actiCHampPlusADC();
  ~actiCHampPlusADC();
  void OnPublish() override;
  void OnAutoConfig () override;
  void OnPreflight( SignalProperties& Output ) const override;
  void OnInitialize( const SignalProperties& Output ) override;
  void OnStartAcquisition() override;
  void DoAcquire( GenericSignal& Output ) override;
  void OnStopAcquisition() override;
  void CheckImpedence ();

 private:
  void AquisitionWorker ();
  int                  m_block_size;           // block size passed to BCI2000
  float                m_sample_rate;          // sampling rate defined by subsampledivisor and hardware sample rate
  float                m_hardware_sample_rate; // valid values are 10kHz, 50kHz, 100kHz
  int                  m_device_number;
  int                  m_available_channels;
  bool                 m_device_open;
  int                  m_imped_pauseMS;
  CAmplifier           m_actiCHampplus;
  RawDataHandler       m_rdh;                  // class that manages raw data returned from amp
  GenericVisualization mVis;                   // used to display impedence values
  std::vector<int>                     m_eeg_aux_channel_idxs; // indexes of desired EEG and AUX channels returned from amp
  std::vector<int>                     m_trig_channel_idxs;    // indexes of TRIG channels returned from amp
  std::vector<std::vector<float>>      m_fData;                // Float data returned from amp
  std::vector<int>                     m_channel_types;
  std::thread*                         m_acqThread;
  volatile bool                        m_doAquire;
  concurrent_queue<std::vector<float>> m_acqQueue;
};

#endif // INCLUDED_actiCHampPlusADC_H
