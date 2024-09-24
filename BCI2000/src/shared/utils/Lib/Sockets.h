////////////////////////////////////////////////////////////////////////////////
// $Id: Sockets.h 8163 2024-05-25 17:33:44Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A wrapper for TCP and UDP sockets.
//    Addresses/ports are specified as in "192.2.14.18:21"
//    or as in "dog.animals.org:8080".
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2023: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#ifndef TINY_SOCKETS_H
#define TINY_SOCKETS_H

#include "Constants.h"
#include "FdObject.h"
#include "SockStream.h"

#include <atomic>

namespace Tiny
{

class Socket : public FdObject
{
  protected:
    Socket(sockio::socket *);

  public:
    ~Socket();

    static const char *Hostname()
    {
        return sockio::socket::hostname();
    }

    enum
    {
        unconnected = sockio::socket::disconnected,
        local = sockio::socket::local,
        remote = sockio::socket::remote
    };
    Socket &Open(const std::string &address);
    Socket &Open(const std::string &ip, int port);

    bool Listening() const
    {
        return mpSocket.load()->listening();
    }
    int Connected();
    bool ConnectionOriented() const;

    static const std::vector<std::string> &LocalIPs();
    std::string LocalIP() const
    {
        return mpSocket.load()->local_ip();
    }
    int LocalPort() const
    {
        return mpSocket.load()->local_port();
    }
    std::string LocalAddress() const
    {
        return mpSocket.load()->local_address();
    }
    std::string RemoteIP() const
    {
        return mpSocket.load()->remote_ip();
    }
    int RemotePort() const
    {
        return mpSocket.load()->remote_port();
    }
    std::string RemoteAddress() const
    {
        return mpSocket.load()->remote_address();
    }

    Socket &SetTCPNodelay(bool);
    bool TCPNodelay() const;

    Socket &SetFlushAfterWrite(bool b)
    {
        if (mpTcpSocket)
            mFlushAfterWrite = b;
        return *this;
    }
    bool FlushAfterWrite() const
    {
        return mFlushAfterWrite;
    }

    bool WaitForAccept(Socket &s, int timeout = InfiniteTimeout);
    bool WaitForAccept(int timeout = InfiniteTimeout);
    bool Accept()
    {
        return WaitForAccept(0);
    }

  protected:
    int64_t OnWrite(const char *, int64_t) override;

    std::atomic<sockio::socket *> mpSocket{ nullptr };
    std::atomic<sockio::tcpsocket *> mpTcpSocket{ nullptr };
    std::atomic<bool> mFlushAfterWrite{ false }, mTcpNodelay{ false };
};

template <class T> class Socket_ : public Socket
{
  public:
    Socket_() : Socket(new T)
    {
    }
};

typedef Socket_<tcpsocket> TCPSocket;
typedef Socket_<client_tcpsocket> ClientTCPSocket;
typedef Socket_<server_tcpsocket> ServerTCPSocket;
typedef Socket_<receiving_udpsocket> ReceivingUDPSocket;
typedef Socket_<sending_udpsocket> SendingUDPSocket;

} // namespace Tiny

using Tiny::ClientTCPSocket;
using Tiny::ReceivingUDPSocket;
using Tiny::SendingUDPSocket;
using Tiny::ServerTCPSocket;
using Tiny::Socket;
using Tiny::TCPSocket;

#endif // TINY_SOCKETS_H
