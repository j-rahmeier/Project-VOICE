////////////////////////////////////////////////////////////////////////////////
// Authors: Brunnerlab@DESKTOP-F8KRI7F.wucon.wustl.edu
// Description: DeviceConfiguration header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_DEVICECONFIGURATION_H  // makes sure this header is not included more than once
#define INCLUDED_DEVICECONFIGURATION_H


#include "GenericFilter.h"
#include "Environment.h"
#include "Expression.h"
#include <unordered_map>
#include <unordered_set>

//Abstract class for abstract interface
class AbstractStimulator : public Environment {
public:
  AbstractStimulator();
  ~AbstractStimulator();
  void initializeStimulator();
  virtual void setIDtoChannelMap();

  std::set<int> getChannels(const GenericSignal*);
  void checkParameters(const SignalProperties&) const;
  virtual void preflightParams() const;

  void setTrigParamName(std::string name) { trigParamName = name; };
private:
  std::unordered_map<Expression*, std::set<int>> triggeredChannels;
protected:
  std::string trigParamName;
  std::list<int> channelRows;
  std::map<int, int> idsToChannels; // {channel id, channel index}
};

//interface that is used with CCEPFilter
class StimulatorConfiguration : public Environment
{
public:
  //StimulatorConfiguration();
  StimulatorConfiguration();
  ~StimulatorConfiguration();
  std::set<int> getChannels(const GenericSignal*);
  void initializeStimulator();
  void checkParameters(const SignalProperties&) const;

private:
  AbstractStimulator* stimulator;
  enum deviceNames { //  <-------ADD NAME FOR NEW STIMULATORS
    basic, 
    NeuroOmega, 
    gEstim,
    CereStim
  }; 
};

//Neuro Omega implementation
class NeuroOmegaStimulator : public AbstractStimulator {
public:
  NeuroOmegaStimulator();
  void preflightParams() const override;
  void setIDtoChannelMap() override;

};

//gEstim implementation
class gEstimStimulator : public AbstractStimulator {
public:
  gEstimStimulator();
};

//CereStim implementation
class CereStimStimulator : public AbstractStimulator {
public:
  CereStimStimulator();
};

//Declare any future implemenations here...


#endif // INCLUDED_DEVICECONFIGURATION_H
