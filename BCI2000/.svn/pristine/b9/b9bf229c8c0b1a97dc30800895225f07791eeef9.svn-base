#include "record_disable.h"
#include "MicroLeads_Protocol.h"
#include "socket_init.h"
#include "send_tcp.h"
#include "variables.h"

#pragma pack(1)

void record_disable()
{
    string ip = "192.168.1.10";
    int port = 3031;

    SocketWrapper sock;

    sock.Create_TCP(port, ip);

    timeval timey;

    timey.tv_sec = SOCK_TIMEOUT_SECONDS;
    timey.tv_usec = SOCK_TIMEOUT_MICROSECONDS;

    // Connect to server
    sock.Connect();

    // Do-while loop to send and receive data
    char buf[BUF_LEN];
    // byte array
    unsigned char a[] = { 0x7e, 0x1a, 0x09, 0x01, 0x00, 0xbc, 0x0a };
    unsigned char b[] = { 0x7e, 0x24, 0x1e, 0x01, 0x00, 0xf9, 0x0a };

    // recasting byte array as char
    const char* p = reinterpret_cast<const char*>(a);
    const char* q = reinterpret_cast<const char*>(b);

    int i = 1;

    do
    {
        int retries = RETRIES;
        send_tcp(sock, p, retries, timey, buf, 7);

        send_tcp(sock, q, retries, timey, buf, 7);

        i = 0;
    } while (i > 0);

    // Close down everything
    closesocket(sock.sock);
    WSACleanup();
}
