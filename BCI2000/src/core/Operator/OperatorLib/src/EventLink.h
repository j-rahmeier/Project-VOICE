////////////////////////////////////////////////////////////////////////////////
// $Id: EventLink.h 8165 2024-05-27 12:20:03Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A helper class that injects events into the source module.
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
#ifndef EVENT_LINK_H
#define EVENT_LINK_H

#include "Sockets.h"
#include "Streambuf.h"
#include "Thread.h"

class StateMachine;

class EventLink : public std::iostream, public Lockable<>, private Thread
{
  public:
    EventLink(StateMachine &);
    ~EventLink();
    void Open(const std::string& ip, int port);
    void Close();
    void ConfirmConnection();
    bool Connected() const
    {
        return mConnected;
    }

  private:
    int OnExecute();

    StateMachine &mrParent;
    SendingUDPSocket mSocket;
    BufferedIO mStreambuf;
    std::string mIP;
    int mPort;
    std::atomic<bool> mConnected = false;
};

#endif // EVENT_LINK_H
