//DeviceConfiguration.cpp: Helper class to abstract the configuration for a specific device.
//If a new device is desired to be used, you only have to change the first constructor

#include "CCEPFilter.h"
#include "DeviceConfiguration.h"
using namespace std;


StimulatorConfiguration::StimulatorConfiguration()
: stimulator(nullptr)
{

  if (OptionalParameter("ActivateSwitchingUnit", false))
  {
    //gEstimSwitchingUnit
    stimulator = new gEstimStimulator();
  }
  else if (OptionalParameter("EnableCereStim", false))
  {
    //CereStim
    stimulator = new CereStimStimulator();
  }
  else if (OptionalParameter("EnableNeuroOmegaStim", false))
  {
    //NeuroOmega
    stimulator = new NeuroOmegaStimulator();
  }
  else
    stimulator = new AbstractStimulator();
}

//Abstract and default definitions below

StimulatorConfiguration::~StimulatorConfiguration()
{
  if (stimulator != nullptr)
  {
    delete stimulator;
  }
}

set<int>
StimulatorConfiguration::getChannels(const GenericSignal* input)
{
  return stimulator->getChannels(input);
}

void
StimulatorConfiguration::checkParameters(const SignalProperties& Input) const
{
  return stimulator->checkParameters(Input);
}

void
StimulatorConfiguration::initializeStimulator()
{
  return stimulator->initializeStimulator();
}

//Functions that are the same for each device
AbstractStimulator::AbstractStimulator()
: trigParamName(""),
  channelRows(),
  idsToChannels()
{
}

void
AbstractStimulator::checkParameters(const SignalProperties& Input) const
{
  if (!trigParamName.empty())
  {
    GenericSignal inputSig(Input);
    auto trigs = Parameter(trigParamName);
    for (int c = 0; c < trigs->NumColumns(); c++)
    {
      Expression exp = trigs(0, c).ToString();
      exp.Compile();
      exp.Evaluate(&inputSig);
    }
  }
  preflightParams(); //any additional device parameters we need to check
}

void
AbstractStimulator::preflightParams() const
{
  Parameter("SourceCh");
}

set<int>
AbstractStimulator::getChannels(const GenericSignal* input)
{
  for (auto it = triggeredChannels.begin(); it != triggeredChannels.end(); it++)
  {
    Expression* e = it->first;
    if (e->Evaluate(input))
    {
      //only one expression should be true
      bcidbg << "Stim " << e->AsString() << " is true" << endl;
      return it->second;
    }
  }

  set<int> v;
  return v;
}

void
AbstractStimulator::initializeStimulator()
{
  if (trigParamName.empty())
    return;

  setIDtoChannelMap(); //first set ch map
  //go thru StimulationTriggers
  auto trigs = Parameter(trigParamName);
  for (int c = 0; c < trigs->NumColumns(); c++)
  {
    Expression* exp = new Expression(trigs(0, c));
    set<int> s;
    //make sure it is not in map
    auto it = triggeredChannels.find(exp);
    if (it == triggeredChannels.end())
    {
      //insert, then find pointer to it
      triggeredChannels.insert({ exp, s });
      it = triggeredChannels.find(exp);
    }
    //channel(s)
    for (auto r = channelRows.begin(); r != channelRows.end(); r++)
    {
      ParamRef v = trigs(*r, c);
      //insert all values
      for (int n = 0; n < v->NumValues(); n++)
      {
        //convert chs to between 0 - numChannels
        if (v(n, 0) >= 0) //skip negative ids
        {
          int ch = idsToChannels[v(n, 0)];
          //add to list of channels for trigger
          (it->second).insert(ch);
        }
      }
    }
  }
}

//happens in initialize
//default initialization is 1:1 map
void
AbstractStimulator::setIDtoChannelMap()
{
  for (int i = 0; i < Parameter("SourceCh"); i++)
  {
    idsToChannels.insert({ i, i }); //index is id
  }
}

AbstractStimulator::~AbstractStimulator()
{
  //clean-up
  for (auto it = triggeredChannels.begin(); it != triggeredChannels.end(); it++)
  {
    delete it->first;
  }
}
