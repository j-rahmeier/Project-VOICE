#include <iostream>
#include <string>
#include <thread>
#include <sstream>
#include <algorithm>
#include <random>
#include <cassert>
#include "SockStream.h"

void threadFunc(int messageCount, int seed)
{
    std::mt19937 r(seed);
    std::uniform_int_distribution<int> d(0, 1024 * 1024 - 1);
    client_tcpsocket socket("127.0.0.1:4000");
    if (!socket.connected())
        std::cerr << "Could not connect" << std::endl;
    fdio::fd_streambuf buf(socket);
    std::ostream oss(&buf);
    for (int k = 0; k < messageCount; ++k) {
        union { int i; char c[sizeof(i)]; } size = { d(r) };
        oss.write(size.c, sizeof(size));
        int written = 0;
        while (written < size.i) {
            int length = d(r);
            length = std::min(length, size.i - written);
            std::uniform_int_distribution<int> d('a', 'z');
            std::string s;
            s.resize(length);
            for (int j = 0; j < length; ++j)
              s[j] = d(r);
            oss.write(s.data(), s.length()).flush();
            written += length;
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3 || std::string(argv[1]) == "--help") {
        std::cerr << "usage: sockstream_test <thread count> <message count>" << std::endl;
        return -1;
    }
    int threadCount = ::atoi(argv[1]);
    int messageCount = ::atoi(argv[2]);
    std::mt19937 r;
    std::vector<std::thread*> threads;
    std::vector<fdio::fd_object*> sockets;
    std::vector<int> messages;
    server_tcpsocket serverSocket("127.0.0.1:4000");
    for (int i = 0; i < threadCount; ++i) {
        threads.push_back(new std::thread(&threadFunc, messageCount, r()));
        auto pSocket = new tcpsocket;
        sockets.push_back(pSocket);
        serverSocket.wait_for_accept(*pSocket);
        messages.push_back(0);
        std::cout << "." << std::flush;
        if (messages.size() % 80 == 0)
            std::cout << std::endl;
    }
    std::cout << std::endl;
    int done = 0;
    while (done < threads.size()) {
        auto pReadableSocket = fdio::fd_object::select(sockets.data(), sockets.size(), nullptr, 0, fdio::fd_object::infinite_timeout);
        auto it = std::find(sockets.begin(), sockets.end(), pReadableSocket);
        assert(it != sockets.end());
        int idx = it - sockets.begin();
        fdio::fd_streambuf buf(*pReadableSocket);
        std::istream is(&buf);
        union { int i; char c[sizeof(i)]; } size = {0};
        is.read(size.c, sizeof(size));
        auto pData = new char[size.i];
        is.read(pData, size.i);
        for (int j = 0; j < size.i; ++j) {
            if (pData[j] < 'a' || pData[j] > 'z')
                throw std::runtime_error("bad data");
        }
        delete[] pData;
        if (++messages[idx] >= messageCount) {
            pReadableSocket->close();
            threads[idx]->join();
            ++done;
            std::cout << '.' << std::flush;
            if (done % 80 == 0)
                std::cout << std::endl;
        }
    }
    return 0;
}
