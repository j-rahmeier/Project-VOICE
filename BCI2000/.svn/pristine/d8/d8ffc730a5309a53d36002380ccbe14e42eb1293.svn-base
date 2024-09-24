//////////////////////////////////////////////////////////////////////
// $Id$
// Authors: mellinger@neurotechcenter.de
// Description: Scripting extension dealing with telnet servers
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
#include "TelnetType.h"

#include "BCI_OperatorLib.h"
#include "ParserToken.h"

namespace {
    ScriptingExtensions::TelnetType sInstance;
    const char* sHelp = "Start Telnet <address>, Stop Telnet <address>";
}

namespace ScriptingExtensions {
    const ObjectType::MethodEntry TelnetType::sMethodTable[] = {
      {"Start", &TelnetType::Start },
      {"Stop", &TelnetType::Stop },
      { 0 }
    };

    const char* TelnetType::Name() const
    {
        return "Telnet";
    }

    const char* TelnetType::Help() const
    {
        return sHelp;
    }

    const ObjectType::MethodEntry* TelnetType::MethodTable() const
    {
        return sMethodTable;
    }

    bool TelnetType::Start(std::istream& is, std::ostream& os)
    {
        ParserToken address;
        if (is >> address)
        {
            int ok = BCI_TelnetListen(address.c_str());
            if (!ok)
                os << "Could not start telnet server at " << address.c_str();
        }
        else
        {
            int ok = BCI_TelnetListen(nullptr);
            if (!ok)
                os << "Could not start telnet server at default address";
        }
        return true;
    }

    bool TelnetType::Stop(std::istream& is, std::ostream& os)
    {
        ParserToken address;
        if (is >> address)
        {
            int ok = BCI_TelnetCloseOne(address.c_str());
            if (!ok)
                os << "Could not stop telnet server at " << address.c_str();
        }
        else
        {
            int ok = BCI_TelnetClose();
            if (!ok)
                os << "Could not stop all telnet servers";
        }
        return true;
    }
}
