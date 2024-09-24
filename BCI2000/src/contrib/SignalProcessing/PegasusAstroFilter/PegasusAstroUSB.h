// C++ abstraction class for controlling of PegasusAstro USB hub
// belsten@neurotechcenter.org
#pragma once

#include <iostream>
#include "SerialPort.h"

class PegasusAstroUSB
{
public:
   PegasusAstroUSB( const std::string _COMstring );
  ~PegasusAstroUSB();

  bool isConnected() const;
  void turnOn(char ID) const;
  void turnOff(char ID) const;
  const std::string getFirmwareVersion() const;

private:
  void disconnect();

  SerialPort* mDevice;
};

