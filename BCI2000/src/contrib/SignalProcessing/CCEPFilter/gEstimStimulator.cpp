#include "DeviceConfiguration.h"
using namespace std;


/**************gESTIM SPECIFIC DEFINITIONS**************************/
//everything below needs to be copied if you are implementing a new device
//set-up will vary depending on the set-up of the stimulator

gEstimStimulator::gEstimStimulator()
{
  trigParamName = "StimConfig";
  channelRows = list<int>{ 1, 2 };
}