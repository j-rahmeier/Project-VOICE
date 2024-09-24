////////////////////////////////////////////////////////////////////////////////
//engelhardt@neurotechcenter.org
//For implementation in PegasusAstroFilter
////////////////////////////////////////////////////////////////////////////////

#include "PegasusAstroUSB.h"

PegasusAstroUSB::PegasusAstroUSB( const std::string _COMstring )
{
  mDevice = nullptr;
  std::string COMprefix = "\\\\.\\"; 
  mDevice = new SerialPort((COMprefix + _COMstring).c_str());
}

PegasusAstroUSB::~PegasusAstroUSB()
{
  this->disconnect();
}


void PegasusAstroUSB::disconnect()
{
  if (this->isConnected())
  {
    mDevice->closeSerial();
    delete mDevice;
    mDevice = nullptr;
  }
}


bool  PegasusAstroUSB::isConnected() const
{
  // check if the pointer to the device is null...
  if (mDevice != nullptr)
    // check if the serial port is connected
    return mDevice->isConnected();
  return false;
}


const std::string PegasusAstroUSB::getFirmwareVersion() const
{
  if (!this->isConnected())
    return "";
  
  // write the command to the serial port
  char getFirmwareVersion[] = { 'P','V','\n' };
  bool res = mDevice->writeSerialPort(getFirmwareVersion, sizeof(getFirmwareVersion));
  if (!res)
    std::cerr << "PegasusAstro Error: Unable to send command to hub" << std::endl;
  Sleep(50);

  // read response
  char buffer[255];
  int bytes_read = mDevice->readSerialPort(buffer, sizeof(buffer));
  if (bytes_read == 0)
    std::cerr << "PegasusAstro Error: No response from hub" << std::endl;
  
  return std::string(buffer, bytes_read);
}

void PegasusAstroUSB::turnOn(char ID) const
{
    char turnOn[] = { 'U', ID, ':', '1', '\n' };
    bool res = mDevice->writeSerialPort(turnOn, sizeof(turnOn));
    if (!res)
        std::cerr << "PegasusAstro Error: Unable to send command to hub" << std::endl;

    //Takes too long to receive a response
    /*Sleep(50);
    char buffer[255];
    int bytes_read = 0;
    bytes_read = mDevice->readSerialPort(buffer, sizeof(buffer));
    if (bytes_read == 0)
        return false;
    return true;*/
}

void PegasusAstroUSB::turnOff(char ID) const
{
    char turnOff[] = { 'U', ID, ':', '0', '\n' };
    bool res = mDevice->writeSerialPort(turnOff, sizeof(turnOff));
    if (!res)
        std::cerr << "PegasusAstro Error: Unable to send command to hub" << std::endl;

    //Takes too long to receive a response
    /*Sleep(50);
    char buffer[255];
    int bytes_read = 0;
    bytes_read = mDevice->readSerialPort(buffer, sizeof(buffer));
    if (bytes_read == 0)
        return false;
    return true;*/
}