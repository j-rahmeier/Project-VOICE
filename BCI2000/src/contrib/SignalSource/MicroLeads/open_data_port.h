#ifndef OPEN_DATA_PORT_H
#define OPEN_DATA_PORT_H

#include <iostream>
#include <fstream>
#include <WS2tcpip.h>
#include <string>
#include "MicroLeads_Protocol.h"
#include "send_udp.h"
#include "variables.h"
#include "BCIStream.h"
#include "BCIEvent.h"

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void open_data_port(SocketWrapper sock);

datapacket* getData(SocketWrapper sock, char* buf);

datapacket* getDummyData(ifstream* stream, char* buf);

#endif