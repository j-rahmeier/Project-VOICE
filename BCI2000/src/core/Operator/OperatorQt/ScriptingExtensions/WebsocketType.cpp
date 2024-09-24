//////////////////////////////////////////////////////////////////////
// $Id$
// Authors: mellinger@neurotechcenter.de
// Description: Scripting extension dealing with websocket servers
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
///////////////////////////////////////////////////////////////////////
#include "WebsocketType.h"

#include "BCI_OperatorLib.h"
#include "ParserToken.h"

namespace {
    ScriptingExtensions::WebsocketType sInstance;
    const char* sHelp = "Start Websocket <address>, Stop Websocket <address>";
}

namespace ScriptingExtensions {
    const ObjectType::MethodEntry WebsocketType::sMethodTable[] = {
      {"Start", &WebsocketType::Start },
      {"Stop", &WebsocketType::Stop },
      { 0 }
    };

    const char* WebsocketType::Name() const
    {
        return "Websocket";
    }

    const char* WebsocketType::Help() const
    {
        return sHelp;
    }

    const ObjectType::MethodEntry* WebsocketType::MethodTable() const
    {
        return sMethodTable;
    }

    bool WebsocketType::Start(std::istream& is, std::ostream& os)
    {
        ParserToken address;
        if (is >> address)
            BCI_WebsocketListen(address.c_str());
        else
            BCI_WebsocketListen(nullptr);
        return true;
    }

    bool WebsocketType::Stop(std::istream& is, std::ostream& os)
    {
        ParserToken address;
        if (is >> address)
            BCI_WebsocketCloseOne(address.c_str());
        else
            BCI_WebsocketClose();
        return true;
    }
}
