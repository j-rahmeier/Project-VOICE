#ifndef SOCKET_INIT_H
#define SOCKET_INIT_H

#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include "BCIStream.h"
#include "BCIEvent.h"
#pragma comment (lib, "ws2_32.lib")

using namespace std;

class SocketWrapper
{
public:
	int Create_UDP(int port, string ip);
	int Create_TCP(int port, string ip);
	int Connect();
	void Set_Recv_Params(int port);
	SOCKET sock;
	sockaddr_in server;
  sockaddr_in receive_server;
};

#endif