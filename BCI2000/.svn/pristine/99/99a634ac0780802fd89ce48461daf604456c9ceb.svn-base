#ifndef SEND_TCP_H
#define SEND_TCP_H

#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <cstdlib>
#include <WinSock2.h>
#include <stdio.h>
#include "socket_init.h"
#ifdef _WIN32
#include <Windows.h>

#else 
#include <unistd.h>
#endif

bool send_tcp(SocketWrapper sock, const char* message, int retries, timeval timey, char* buf, int size);

#endif

