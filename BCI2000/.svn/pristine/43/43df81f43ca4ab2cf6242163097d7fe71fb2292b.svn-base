#include <iostream>
#include <chrono>
#include <thread>

#include "SerialPort.h"
#include "Vibrotactile_Firmware/command_packet.h"

#include "HandVibration.h"


void lowLevelControl ()
{
  const char* portName = "\\\\.\\COM6";

  //Declare a global object
  SerialPort* arduino;
  CommandPacket             cmd_pkt;
  StimulationConfiguration  config_pkt;

  std::cout << "Attempting to connect..." << std::endl;
  arduino = new SerialPort (portName);
  std::cout << "Is connected: " << arduino->isConnected () << std::endl;
  std::cout << "size of cmd packet " << sizeof (StimulationConfiguration) << std::endl;
  if (arduino->isConnected ())
  {
    bool hasWritten = false;

    std::this_thread::sleep_for (std::chrono::milliseconds (1000));

    /*
    // send command to blink LED
    cmd_pkt.Command = BlinkLED;
    hasWritten      = arduino->writeSerialPort ((char*)& cmd_pkt, sizeof (CommandPacket));
    if (hasWritten) std::cout << "Blink Command Written Successfully" << std::endl;
    else            std::cerr << "Command was not written"            << std::endl;
    */
    // send command to configure
    cmd_pkt.Command = Configure;
    hasWritten = arduino->writeSerialPort ((char*)& cmd_pkt, sizeof (CommandPacket));
    if (hasWritten) std::cout << "Configure command Written Successfully" << std::endl;
    else            std::cerr << "Command was not written" << std::endl;

    // send the configuration
    config_pkt.Amplitude = 50;
    config_pkt.Frequency = 1000;

    hasWritten = arduino->writeSerialPort ((char*)& config_pkt, sizeof (StimulationConfiguration));
    if (hasWritten) std::cout << "Configuration Written Successfully" << std::endl;
    else            std::cerr << "Configuration was not written" << std::endl;

    for (int i = 0; i < 2; i++)
    {

      // send command to start
      cmd_pkt.Command = Start;
      hasWritten = arduino->writeSerialPort ((char*)& cmd_pkt, sizeof (CommandPacket));
      if (hasWritten) std::cout << "Start command Written Successfully.... should be blinking" << std::endl;
      else            std::cerr << "Command was not written" << std::endl;

      int aInt;
      std::cout << "enter a number:" << std::endl;
      std::cin >> aInt;

      // send command to stop
      cmd_pkt.Command = Stop;
      hasWritten = arduino->writeSerialPort ((char*)& cmd_pkt, sizeof (CommandPacket));
      if (hasWritten) std::cout << "Stop Command Written Successfully" << std::endl;
      else            std::cerr << "Command was not written" << std::endl;


      // sleep for x seconds
      std::this_thread::sleep_for (std::chrono::milliseconds (5000));

    }
  }
  delete arduino;
}


void highLevelControl ()
{
  bool success = false;

  HandVibration hand = HandVibration();
  std::string comport = "COM6";

  if (hand.Open (comport))
    std::cout << "Connected to hand" << std::endl;
  else 
  {
    std::cout << "Not able to connect to hand" << std::endl;
    return;
  }

  std::this_thread::sleep_for (std::chrono::milliseconds (1000));

  int amplitude = 20;
  float frequency = 4;

  if (hand.ConfigureVibration(amplitude, frequency))
    std::cout << "Set configuration" << std::endl;
  else 
  {
    std::cout << "Unable to set configuration" << std::endl;
    return;
  }

  for (int i = 0; i < 2; i++)
  {
    // start vibration
    std::cout << "Starting vibration" << std::endl;
    hand.StartVibration ();
    std::this_thread::sleep_for (std::chrono::milliseconds (5000));

    // stop vibration
    std::cout << "Stopping vibration" << std::endl;
    hand.StopVibration ();
    std::this_thread::sleep_for (std::chrono::milliseconds (3000));
  }
}

int main (void)
{
  //lowLevelControl ();
  highLevelControl ();
  return 1;
}
