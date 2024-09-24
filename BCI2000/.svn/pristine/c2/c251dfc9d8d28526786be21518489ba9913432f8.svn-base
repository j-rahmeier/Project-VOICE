////////////////////////////////////////////////////////////////////////////////
// Authors: schalkadmin@DESKTOP-OTQRNP6.wucon.wustl.edu
// Description: MicroLeadsADC header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_MICROLEADSADC_H  // makes sure this header is not included more than once
#define INCLUDED_MICROLEADSADC_H

#include "BufferedADC.h"
#include "BCIStream.h"
#include "BCIEvent.h"

#include "MicroLeads_Protocol.h"
#include "variables.h"

#include "basestation_SOH.h"
#include "powerpad_SOH.h"
#include "record_disable.h"
#include "record_enable.h"
#include "record_init.h"
#include "send_tcp.h"
#include "send_udp.h"
#include "socket_init.h"
#include "stim_enable.h"
#include "open_data_port.h"


#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <cstdlib>
#include <WinSock2.h>
#include <stdio.h>




class MicroLeadsADC : public BufferedADC
{
 public:
  MicroLeadsADC();
  ~MicroLeadsADC();
  void OnPublish() override;
  void OnAutoConfig() override;
  void OnPreflight( SignalProperties& Output ) const override;
  void OnInitialize( const SignalProperties& Output ) override;
  void OnStartAcquisition() override;
  void DoAcquire( GenericSignal& Output ) override;
  void OnStopAcquisition() override;
  void OnStartRun() override;
  void OnStopRun() override;


 private:
  // Use this space to declare any MicroLeadsADC-specific methods and member variables you'll need
  SocketWrapper TCP_Socket;
  SocketWrapper UDP_2323_Socket;
  SocketWrapper UDP_2020_Socket;
  int mNumberOfSignalChannels;
  std::ofstream myfile;

  char params[25];

  volatile bool finished;
};

#endif // INCLUDED_MICROLEADSADC_H
