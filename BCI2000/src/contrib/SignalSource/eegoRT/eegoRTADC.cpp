////////////////////////////////////////////////////////////////////////////////
// $Id: eegoRTADC.cpp 7649 2023-10-07 16:01:04Z mellinger $
// Authors: pbrunner@neurotechcenter.org, mellinger@neurotechcenter.org
// Description: eegoRTADC implementation
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
#include "eegoRTADC.h"
#include "BCIStream.h"

// Make the source filter known to the framework.
RegisterFilter( eegoRTADC, 1 ); // ADC filters must be registered at location "1" in order to work.

static const int cTimeSliceMs = 2; // improve timing resolution for polling loop

namespace {

    int GetNumEegChannels(const std::vector<eemagine::sdk::channel>& channelList, std::vector<int>* outpMap = nullptr)
    {
        std::vector<int> channelMap;
        int channels = 0;
        for (int i = 0; i < channelList.size(); ++i) {
            if (channelList[i].getType() == eemagine::sdk::channel::reference) {
                ++channels;
                channelMap.push_back(i);
            }
            else if (channelList[i].getType() == eemagine::sdk::channel::bipolar) {
                ++channels;
                channelMap.push_back(i);
            }
        }
        if (outpMap)
            *outpMap = channelMap;
        return channels;
    }

    std::vector<int> GetTriggerChannels(const std::vector<eemagine::sdk::channel>& channelList)
    {
        std::vector<int> channels;
        for (int i = 0; i < channelList.size(); ++i)
            if (channelList[i].getType() == eemagine::sdk::channel::trigger)
                channels.push_back(i);
        return channels;
    }
}

eegoRTADC::eegoRTADC()
: mpDevice(nullptr), mpStream(nullptr),
  mCurrentReadSample(0), mTriggerChannel(-1),
  mSamplingRate(0), mSourceCh(0), mTimeSlice(Time::FromMilliseconds(cTimeSliceMs))
{
}

eegoRTADC::~eegoRTADC()
{
  delete mpStream;
  delete mpDevice;
}

void
eegoRTADC::OnPublish()
{
  BEGIN_PARAMETER_DEFINITIONS

    "Source:Signal%20Properties int SourceCh= auto "
       "auto 1 % // number of digitized and stored channels",

    "Source:Signal%20Properties int SampleBlockSize= auto "
       "auto 1 % // number of samples transmitted at a time",

    "Source:Signal%20Properties float SamplingRate= auto "
       "auto 0.0 % // sample rate",

    "Source:Signal%20Properties list SourceChGain= 1 auto "
       " // physical units per raw A/D unit",

    "Source:Signal%20Properties list SourceChOffset= 1 auto "
       " // raw A/D offset to subtract, typically 0",

    "Source:Signal%20Properties list ChannelNames= 1 auto "
       " // names of amplifier channels",

  END_PARAMETER_DEFINITIONS

  BEGIN_STATE_DEFINITIONS
    "eegoTriggers 8 0", // <name> <bit width> <initial value>
  END_STATE_DEFINITIONS
}

void
eegoRTADC::OnAutoConfig()
{
  // The user has pressed "Set Config" and some parameters may be set to "auto",
  // indicating that they should be set from the current amplifier configuration.
  // In this handler, we behave as if any parameter were set to "auto", and the
  // framework will transparently make sure to preserve user-defined values.

  delete mpStream;
  mpStream = nullptr;
  delete mpDevice;
  try {
      eemagine::sdk::factory fact("eego-SDK.dll"); // Make sure that eego-SDK.dll resides in the working directory
      mpDevice = fact.getAmplifier();
  }
  catch (const std::exception& e) {
      throw bcierr << e.what();
  }

  auto channelList = mpDevice->getChannelList();
  int channels = GetNumEegChannels(channelList);

  Parameter("SourceCh") = channels; // Set SourceCh in case of "auto"
  // If SourceCh != auto but e.g. SourceChGain != auto, we need to use the actual
  // rather than the auto-set value of SourceCh in order to consistently configure
  // SourceChGain.
  // For this purpose, an ActualParameter() call will retrieve a parameter's current
  // value, no matter whether auto-configured or user-configured.
  channels = ActualParameter("SourceCh");
  Parameter("SourceChGain")->SetNumValues(channels);
  Parameter("SourceChOffset")->SetNumValues(channels);
  for(int i = 0; i < channels; ++i)
  {
    Parameter("SourceChGain")(i) << "1V"; // Always provide correct physical unit for documentation and display purposes.
    // For most amplifiers, offset removal is done within the amp hardware or driver. Otherwise, you may set this to a nonzero value.
    Parameter("SourceChOffset")(i) = 0;
  }

  auto samplingRates = mpDevice->getSamplingRatesAvailable();
  if (samplingRates.empty())
    throw bcierr << "List of sampling rates is empty";
  int maxSamplingRate = *std::max(samplingRates.begin(), samplingRates.end());
  Parameter("SamplingRate") << maxSamplingRate << "Hz";

  Parameter("SampleBlockSize") = 32; // cannot be set nor retrieved via the SDK
}

void
eegoRTADC::OnPreflight(SignalProperties& Output) const
{
  // The user has pressed "Set Config" and we need to sanity-check everything.
  // For example, check that all necessary parameters and states are accessible:
  State("eegoTriggers");

  eemagine::sdk::stream* pStream = mpDevice->OpenEegStream(Parameter("SamplingRate").InHertz());
  if (!pStream)
  {
      std::ostringstream oss;
      auto samplingRates = mpDevice->getSamplingRatesAvailable();
      for (const auto& rate : samplingRates)
        oss << rate << "Hz ";
      throw bcierr << "Illegal sampling rate, allowed sampling rates are:\n"
                   << oss.str();
  }

  auto channelList = pStream->getChannelList();
  delete pStream;

  int channels = GetNumEegChannels(channelList);
  int SourceCh = Parameter("SourceCh");
  if (channels < SourceCh)
    throw bcierr << "SourceCh is " << SourceCh << ", exceeds available channels (" << channels << ")";

  SignalType sigType = SignalType::float32;
  int samplesPerBlock = Output.Elements();
  int numberOfSignalChannels = Output.Channels();
  int numberOfSyncStates = 1;
  Output = SignalProperties( numberOfSignalChannels + numberOfSyncStates, samplesPerBlock, sigType );
  // A channel name starting with @ indicates a trigger channel.
  Output.ChannelLabels()[numberOfSignalChannels] = "@eegoTriggers";
}

void
eegoRTADC::OnInitialize(const SignalProperties& Output)
{
  // The user has pressed "Set Config" and all Preflight checks have passed.
  // The system will now transition into an "Initialized" state.

  mSourceCh = Parameter("SourceCh");
  mSamplingRate = Parameter("SamplingRate").InHertz();
}

void
eegoRTADC::OnStartAcquisition()
{
  delete mpStream;
  mpStream = mpDevice->OpenEegStream(mSamplingRate);
  if (!mpStream)
    throw bcierr << "Could not open stream";
  auto channelList = mpStream->getChannelList();
  GetNumEegChannels(channelList, &mChannelMap);
  auto triggerChannels = GetTriggerChannels(channelList);
  if (!triggerChannels.empty())
    mTriggerChannel = triggerChannels.front();
  else
    mTriggerChannel = -1;
  mCurrentReadSample = 0;
}

void
eegoRTADC::DoAcquire(GenericSignal& Output)
{
  for (int currentWriteSample = 0; currentWriteSample < Output.Elements(); ++currentWriteSample)
  {
    if (mCurrentReadSample == 0)
    {
      mBuffer = mpStream->getData();
      while (mBuffer.getSampleCount() == 0)
      {
        ThreadUtils::SleepForMs(cTimeSliceMs); // We need to do active polling
        mBuffer = mpStream->getData();
      }
    }
    for (int ch = 0; ch < mChannelMap.size(); ++ch)
    {
      double value = mBuffer.getSample(mChannelMap[ch], mCurrentReadSample);
      Output(ch, currentWriteSample) = value;
    }
    if (mTriggerChannel >= 0)
    {
      double value = mBuffer.getSample(mTriggerChannel, mCurrentReadSample);
      Output(Output.Channels() - 1, currentWriteSample) = value;
    }
    if (++mCurrentReadSample >= mBuffer.getSampleCount())
      mCurrentReadSample = 0;
  }
}

void
eegoRTADC::OnStopAcquisition()
{
  delete mpStream;
  mpStream = nullptr;
}

