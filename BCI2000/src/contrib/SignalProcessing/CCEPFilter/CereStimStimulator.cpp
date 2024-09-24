#include "DeviceConfiguration.h"
using namespace std;


/**************CERESTIM SPECIFIC DEFINITIONS**************************/
//everything below needs to be copied if you are implementing a new device
//set-up will vary depending on the set-up of the stimulator

CereStimStimulator::CereStimStimulator()
{
  trigParamName = "StimulationTriggers";
  channelRows = list<int>{ 2 };
}