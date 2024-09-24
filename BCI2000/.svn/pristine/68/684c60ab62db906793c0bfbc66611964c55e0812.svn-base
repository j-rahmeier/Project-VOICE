#include "record_init.h"

void record_init(SocketWrapper sock)
//remember to change parameters in header file if you change the input here to take port and ip
{
    // 76657273696f6e0a
    // 6d6163616464720a
    // 6770696f7374617475730a
    // 73657474696d6520313637313133373439380a


    unsigned char a[] = { 0x62, 0x73, 0x72, 0x65, 0x73, 0x65, 0x74, 0x0a };
    unsigned char b[] = { 0x67, 0x6f, 0x0a };

    const char* aa = reinterpret_cast<const char*>(a);
    const char* bb = reinterpret_cast<const char*>(b);

    int send = sendto(sock.sock, aa, 8, 0, (sockaddr*)&sock.server, sizeof(sock.server));

    if (send == SOCKET_ERROR)
    {
        bciwarn << "record init 1 didn't work! " << WSAGetLastError() << endl;
    }
    else {
        bciwarn << "record init 1 worked" << endl;
    }

    int sendOk = sendto(sock.sock, bb, 3, 0, (sockaddr*)&sock.server, sizeof(sock.server));

    if (sendOk == SOCKET_ERROR)
    {
        bciwarn << "record init 2 didn't work! " << WSAGetLastError() << endl;
    }
    else {
        bciwarn << "record init 2 worked" << endl;
    }

    //int send3 = sendto(sock.sock, cc, 9, 0, (sockaddr*)&sock.server, sizeof(sock.server));

    //if (send3 == SOCKET_ERROR)
    //{
    //  bciwarn << "record init 3 didn't work! " << WSAGetLastError() << endl;
    //}
    //else {
    //  bciwarn << "record init 3 worked" << endl;
    //}

    //int send4 = sendto(sock.sock, dd, 6, 0, (sockaddr*)&sock.server, sizeof(sock.server));

    //if (send4 == SOCKET_ERROR)
    //{
    //  bciwarn << "record init 4 didn't work! " << WSAGetLastError() << endl;
    //}
    //else {
    //  bciwarn << "record init 4 worked" << endl;
    //}
}