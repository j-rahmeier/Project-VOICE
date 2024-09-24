////////////////////////////////////////////////////////////////////////////////
// $Id: eegoRTADC.h 7649 2023-10-07 16:01:04Z mellinger $
// Authors: pbrunner@neurotechcenter.org, mellinger@neurotechcenter.org
// Description: eegoRTADC header
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

#ifndef INCLUDED_eegoRTADC_H  // makes sure this header is not included more than once
#define INCLUDED_eegoRTADC_H

#include "BufferedADC.h"
#include "ThreadUtils.h"
#include <eemagine/sdk/factory.h>

class eegoRTADC : public BufferedADC
{
 public:
  eegoRTADC();
  ~eegoRTADC();
  void OnPublish() override;
  void OnAutoConfig() override;
  void OnPreflight(SignalProperties& Output) const override;
  void OnInitialize(const SignalProperties& Output) override;
  void OnStartAcquisition() override;
  void DoAcquire(GenericSignal& Output) override;
  void OnStopAcquisition() override;

 private:
  // Use this space to declare any eegoRTADC-specific methods and member variables you'll need
  eemagine::sdk::amplifier* mpDevice;
  eemagine::sdk::stream* mpStream;
  eemagine::sdk::buffer mBuffer;
  std::vector<int> mChannelMap;
  int mTriggerChannel;

  double mSamplingRate;
  int mSourceCh;
  int mCurrentReadSample;

  ThreadUtils::TimeSlice mTimeSlice;
};

#endif // INCLUDED_eegoRTADC_H
