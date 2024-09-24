#ifndef BASESTATION_SOH_H
#define BASESTATION_SOH_H

#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <cstdlib>
#include <WinSock2.h>
#include <stdio.h>
#include "MicroLeads_Protocol.h"
#include "socket_init.h"
#include "send_tcp.h"
#include "variables.h"
#include "BCIStream.h"
#include "BCIEvent.h"
#ifdef _WIN32
#include <Windows.h>
#else 
#include <unistd.h>
#endif

void basestation_SOH(SocketWrapper sock);

#endif
