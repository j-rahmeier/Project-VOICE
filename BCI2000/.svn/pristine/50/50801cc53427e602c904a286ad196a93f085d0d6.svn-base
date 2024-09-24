#include "socket_init.h"

void send_udp(SocketWrapper sock, const char* message, int retryCount, timeval timey, char* buf, int size) {
    int retries = retryCount;
    bool success = false;

    do {
        int sendMess = sendto(sock.sock, message, size, 0, (sockaddr*)&sock.server, sizeof(sock.server));

        if (sendMess != SOCKET_ERROR)
        {
            // Wait for response
            ZeroMemory(buf, 4096);

            fd_set fdRead;
            FD_ZERO(&fdRead);
            FD_SET(sock.sock, &fdRead);
            int selection = select(0, &fdRead, NULL, NULL, &timey);
            if ((selection > 0) && (FD_ISSET(sock.sock, &fdRead)))
            {
                int bytesReceived = recv(sock.sock, buf, 4096, 0);


                if (bytesReceived > 0)
                {
                    success = true;
                    // Echo the response to console
                    cout << "SERVER > " << string(buf, 0, bytesReceived) << endl;
                }
                else {
                    cout << "send failed" << endl;
                }
            }
            else {

                cout << retries << endl;
            }
        }
        else {
            cout << "error" << endl;
        }
        retries = retries - 1;
    } while ((retries > 0) && !success);
}
