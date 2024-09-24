////////////////////////////////////////////////////////////////////////////////
// Authors: ratko.petrovic@brainproducts.com
// Description: LiveAmpADC header
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

#ifndef INCLUDED_LIVEAMPADC_H  // makes sure this header is not included more than once
#define INCLUDED_LIVEAMPADC_H

#include "BufferedADC.h"
#include "GenericVisualization.h"
#include <windows.h>
#include <vector>
#include <string>
#include <algorithm>
#include "LiveAmpLib2.imports.h"

class LiveAmpADC : public BufferedADC
{
 public:
  LiveAmpADC();
  ~LiveAmpADC();
  void OnPublish() override;
  void OnAutoConfig() override;
  void OnPreflight( SignalProperties& Output ) const override;
  void OnInitialize( const SignalProperties& Output ) override;
  void OnStartAcquisition() override;
  void DoAcquire( GenericSignal& Output ) override;
  void OnStopAcquisition() override;

  void OnTrigger( int ); // for asynchronous triggers only
  void OnHalt() override;
   	
 private:

	 BOOL InitializeLiveAmp(void);
	 BOOL CheckAvailableChannels(void);
	 BOOL GetChannelIndices(void);
	 BOOL DisableAllAvailableChannels(int avlbchannels);
	 BOOL EnablePhysicalChannels();
	 BOOL SetGains(void);
	 void GeneratedisplayChannelIndexes(void);
	 int LiveAmpSampleSize(void);
	 void LiveAmpExtractData(int size, std::vector<std::vector<double>> &extractData);

  // Use this space to declare any LiveAmpADC-specific methods and member variables you'll need
	HANDLE mDeviceHandle;
	char* mpBuffer;
	int mBufferSize; 
	BYTE* mpDataBuffer;
	int mNumberOfSignalChannels;
	int mUsedChannelsCounter;

	int mSamplingRate;
	int mSampleSize;
	int mNumberOfDevices;
	BOOL mUseBipolarChannels;

	int mAvailableEEG;
	int mAvailableBip;
	int mAvailableAUX;
	int mAvailableACC;
	int mAvailableTrig;
  
	std::vector<int> mDataTypeArray;
	std::vector<std::vector<double>> mSendBuffer;
	
	std::list<std::pair<int, int>> mPhysicalChannelMap;
	std::vector<int> mDisplayChannelIndexes;

};

#endif // INCLUDED_LIVEAMPADC_H
