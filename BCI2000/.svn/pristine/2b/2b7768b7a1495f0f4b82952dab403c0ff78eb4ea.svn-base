#ifndef RECORD_ENABLE_H
#define RECORD_ENABLE_H

#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <cstdlib>
#include <WinSock2.h>
#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#else 
#include <unistd.h>
#endif

#include "MicroLeadsADC.h"
#include "MicroLeads_Protocol.h"
#include "send_tcp.h"
#include "socket_init.h"
#include "variables.h"


bool record_enable(SocketWrapper socky, char *params);

#endif