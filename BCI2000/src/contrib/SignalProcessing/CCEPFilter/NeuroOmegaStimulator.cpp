#include "DeviceConfiguration.h"
using namespace std;


/**************NEURO OMEGA SPECIFIC DEFINITIONS**************************/
//everything below needs to be copied if you are implementing a new device
//set-up will vary depending on the set-up of the stimulator

NeuroOmegaStimulator::NeuroOmegaStimulator()
{
  trigParamName = "StimulationTriggers";
  channelRows = list<int>{ 2 };
}

void
NeuroOmegaStimulator::preflightParams() const
{
  Parameter("RecordingChIDs");
}

void
NeuroOmegaStimulator::setIDtoChannelMap()
{
  //go thru RecordingChIDs
  auto ids = Parameter("RecordingChIDs");
  for (int i = 0; i < ids->NumRows(); i++)
  {
    idsToChannels.insert(std::make_pair(static_cast<int>(ids(i, 0).ToNumber()), i));
  }
}