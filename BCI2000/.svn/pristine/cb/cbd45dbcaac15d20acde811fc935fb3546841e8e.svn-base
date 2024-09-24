////////////////////////////////////////////////////////////////////////////////
// Authors: Robbie@DESKTOP-U5JGQCU
// Description: LabJackADC header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_LABJACKADC_H  // makes sure this header is not included more than once
#define INCLUDED_LABJACKADC_H

#include "BufferedADC.h"
#include "LabJackM.h"
#include "LJM_Utilities.h"

class LabJackADC : public BufferedADC
{
 public:
  LabJackADC();
  ~LabJackADC();
  void OnPublish() override;
  void OnAutoConfig() override;
  void OnPreflight( SignalProperties& Output ) const override;
  void OnInitialize( const SignalProperties& Output ) override;
  void OnStartAcquisition() override;
  void DoAcquire( GenericSignal& Output ) override;
  void OnStopAcquisition() override;
  int FindChannelIndex(std::string) const;
  void HardcodedConfigureStream(int);
  void BCIErrorCheck(int, int,const char*);
  void BCIWriteNameOrError(int, const char*, double);
 

private:
double samplingRate;
int mHandle;
int mNumberOfSignalChannels;
char* mSignalChannelNames[14];
char* masterList[36] = { "AIN0", "AIN1", "AIN2", "AIN3", "AIN4", "AIN5", "AIN6", "AIN7", "AIN8", "AIN9", "AIN10", "AIN11", "AIN12", "AIN13","FIO0", "FIO1", "FIO2", "FIO3", "FIO4", "FIO5", "FIO6", "FIO7","MIO0","MIO1","EIO0","EIO1","EIO2","EIO3","EIO4","EIO5","EIO6","EIO7","CIO0", "CIO1","CIO2","CIO3"};
int connectionType;
int* mScanList;
double* mData;
int deviceScanBacklog = 0;
int LJMScanBacklog = 0;
unsigned int receiveBufferBytesSize = 0;
unsigned int receiveBufferBytesBacklog = 0;
int err;
int firstDigitalInput = -1;
};

#endif // INCLUDED_LABJACKADC_H
