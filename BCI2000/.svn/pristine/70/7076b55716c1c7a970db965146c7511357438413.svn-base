// Author: Alex Belsten (belsten at neurotechcenter.org) 
//  This file contains common structures that are sent 
//  between the PC and Arduino. 

#pragma once
#pragma pack(1) // tell compiler not to pad memory

#include "stdint.h"

typedef uint8_t CommandType;

// Commands that can be sent to the Arduino
enum Command
{
  Configure       = 1,
  Start           = 2,
  Stop            = 3,
  NotImplemented  = 4,
  UpdateAmplitude = 5
};

// A packet that gives the Arduino a command 
struct CommandPacket
{
  CommandType Command;
};

// packet that configures relevant parameters of vibration, is immediately sent after a "Configure" command packet
struct ConfigurationPacket {
  uint8_t    Amplitude;        // [0-100]
  uint16_t   Frequency;        // in 0.1Hz
};
#pragma pack()
