////////////////////////////////////////////////////////////////////////////////
// $Id: WebsocketServer.h 8244 2024-07-09 18:04:31Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A web socket server that runs inside a BCI2000
//   operator module. Functionality is identical to telnet server but allows
//   for connection from JavaScript running in a browser.
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
#ifndef WEBSOCKET_SERVER_H
#define WEBSOCKET_SERVER_H

#include "Sockets.h"
#include "Thread.h"

class WebsocketServer : private Thread
{
public:
    WebsocketServer(class StateMachine&, const std::string& address);
    ~WebsocketServer();
    std::string Address() const;

protected:
    int OnExecute() override;

private:
    class Session;
    friend class Session;
    ServerTCPSocket mListeningSocket;
    class StateMachine& mrStateMachine;
};

#endif // WEBSOCKET_SERVER_H
