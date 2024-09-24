#include "open_data_port.h"

void open_data_port(SocketWrapper sock)
{

    unsigned char z[] = { 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x62, 0x61, 0x73, 0x65, 0x73, 0x74, 0x69, 0x6f, 0x6e, 0x0a };

    const char* y = reinterpret_cast<const char*>(z);

    char bufu[BUF_LEN];

    int retries = RETRIES;

    // set selection wait window
    timeval timey;

    timey.tv_sec = SOCK_TIMEOUT_SECONDS;
    timey.tv_usec = SOCK_TIMEOUT_MICROSECONDS;

    send_udp(sock, y, retries, timey, bufu, 16);
 
}

datapacket* getData(SocketWrapper sock, char* buf){

    
    int cli_length = sizeof(sock.receive_server);
    ZeroMemory(buf, 992);

    // Wait for message
    int bytesIn = recvfrom(sock.sock, buf, sizeof(datapacket), 0, (sockaddr*)&sock.receive_server, &cli_length);
    if (bytesIn == SOCKET_ERROR)
    {
        bciwarn << "Error receiving from client " << WSAGetLastError() << endl;
    }

    datapacket* packet = reinterpret_cast<datapacket*>(buf);

    return packet;
}

datapacket* getDummyData(ifstream* stream, char* buf) {
  int cli_length = 992;
  ZeroMemory(buf, 992);

  stream->read(buf, cli_length);
  
  datapacket* packet = reinterpret_cast<datapacket*>(buf);

  return packet;
}