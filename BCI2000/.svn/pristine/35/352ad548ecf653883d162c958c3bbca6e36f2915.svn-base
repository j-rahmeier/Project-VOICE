#include "socket_init.h"

int SocketWrapper::Create_UDP(int port, string ip) {
    WSADATA data;
    WORD version = MAKEWORD(2, 2);
    int wsOk = WSAStartup(version, &data);
    if (wsOk != 0)
    {
      return -1;
    }

    this->server.sin_family = AF_INET;
    this->server.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server.sin_addr);

    this->sock = socket(AF_INET, SOCK_DGRAM, 0);
    return 0;
}

int SocketWrapper::Create_TCP(int port, string ip) {
    WSADATA data;
    WORD version = MAKEWORD(2, 2);
    int wsOk = WSAStartup(version, &data);
    if (wsOk != 0)
    {
      return -1;
    }

    this->server.sin_family = AF_INET;
    this->server.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server.sin_addr);

    this->sock = socket(AF_INET, SOCK_STREAM, 0);
    return 0;
}

void SocketWrapper::Set_Recv_Params(int port) {

    this->receive_server.sin_addr.S_un.S_addr = ADDR_ANY;
    this->receive_server.sin_family = AF_INET;
    this->receive_server.sin_port = htons(port);
}

int SocketWrapper::Connect(){
    int connResult = connect(this->sock, (sockaddr*)&server, sizeof(server));
    if (connResult == SOCKET_ERROR)
    {
        bciwarn << "Can't connect to server, Err #" << WSAGetLastError() << endl;
        closesocket(this->sock);
        WSACleanup();
        return -1;
    }
    else {
      return 0;
    }
}
