#include <iostream>
#include <string>
#include <thread>
#include <algorithm>
#include <random>
#include <cassert>
#include "Waitable.h"
#include "MessageChannel.h"
#include "Sockets.h"
#include "Thread.h"
#include "Runnable.h"

void threadFunc(int messageCount, int seed)
{
    std::mt19937 r(seed);
    std::uniform_int_distribution<int> d(0, 1024 * 1024 - 1);
    ClientTCPSocket socket;
    socket.Open("127.0.0.1:4000");
    if (!socket.Connected())
        std::cerr << "Could not connect" << std::endl;
    Tiny::UnbufferedIO streamio;
    streamio.SetOutput(&socket.Output());
    for (int j = 0; j < messageCount; ++j) {
        Tiny::LockingOStream os(streamio);
        union { int i; char c[sizeof(i)]; } size = { d(r) };
        os.write(size.c, sizeof(size));
        int written = 0;
        while (written < size.i) {
            int length = d(r);
            length = std::min(length, size.i - written);
            std::uniform_int_distribution<int> d('a', 'z');
            std::string s;
            s.resize(length);
            for (int j = 0; j < length; ++j)
              s[j] = d(r);
            os.write(s.data(), s.length()).flush();
            written += length;
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3 || std::string(argv[1]) == "--help") {
        std::cerr << "usage: ioobjects_test <thread count> <message count>" << std::endl;
        return -1;
    }
    int threadCount = ::atoi(argv[1]);
    int messageCount = ::atoi(argv[2]);
    std::mt19937 r;
    std::vector<Thread*> threads;
    std::vector<TCPSocket*> sockets;
    std::vector<int> messages;
    ServerTCPSocket serverSocket;
    serverSocket.Open("127.0.0.1:4000");
    for (int i = 0; i < threadCount; ++i) {
        FunctionCall<void(int, int)> call(&threadFunc, messageCount, r());
        auto pThread = new Thread(false, &call);
        threads.push_back(pThread);
        pThread->Start();
        auto pSocket = new TCPSocket;
        sockets.push_back(pSocket);
        serverSocket.WaitForAccept(*pSocket);
        messages.push_back(0);
        std::cout << "." << std::flush;
        if (messages.size() % 80 == 0)
            std::cout << std::endl;
    }
    std::cout << std::endl;
    int done = 0;
    while (done < threads.size()) {
        Waitables waitables;
        for (auto socket : sockets)
            waitables.Add(socket);
        const Waitable* pWaitable = waitables.Wait();
        auto it = std::find(sockets.begin(), sockets.end(), pWaitable);
        assert(it != sockets.end());
        int idx = it - sockets.begin();
        Tiny::UnbufferedIO streamio;
        streamio.SetInput(sockets[idx]);
        Tiny::LockingIStream is(streamio);
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
            sockets[idx]->Close();
            threads[idx]->Wait();
            delete threads[idx];
            ++done;
            std::cout << '.' << std::flush;
            if (done % 80 == 0)
                std::cout << std::endl;
        }
    }
    std::cout << std::endl;
    return 0;
}

