//////////////////////////////////////////////////////////////////////
// $Id$
// Authors: mellinger@neurotechcenter.de
// Description: Scripting extension for scripting watches in the
//   Watches window.
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
#include "WatchType.h"

#include "WatchesWindow.h"
#include "ParserToken.h"

namespace {
    ScriptingExtensions::WatchType sInstance;
    const char* sHelp = "Visualize Watch [decimate <x>] [range <a> <b>] <expr> [<expr> ...]";
}

namespace ScriptingExtensions {
    const ObjectType::MethodEntry WatchType::sMethodTable[] = {
      {"Visualize", &WatchType::Visualize },
      { 0 }
    };

    const char* WatchType::Name() const
    {
        return "Watch";
    }

    const char* WatchType::Help() const
    {
        return sHelp;
    }

    const ObjectType::MethodEntry* WatchType::MethodTable() const
    {
        return sMethodTable;
    }

    bool WatchType::Visualize(std::istream& is, std::ostream& os)
    {
        ParserToken token;
        int decimation = -1; // "auto"
        double min = std::numeric_limits<double>::quiet_NaN(),
               max = min;
        is >> token;
        if (!::stricmp(token.c_str(), "Decimate")) {
            std::string value;
            if (!(is >> value)) {
                os << "Visualize watch: Expected decimation argument";
                return false;
            }
            else if (value == "auto") {
                decimation = -1;
            }
            else if (::atoi(value.c_str()) < 1) {
                os << "Visualize watch: Invalid decimation value";
                return false;
            }
            else {
                decimation = ::atoi(value.c_str());
            }
            is >> token;
        }
        if (!::stricmp(token.c_str(), "Range")) {
            if (!(is >> min >> max)) {
                os << "Visualize watch: Invalid range limits";
                return false;
            }
            is >> token;
        }
        QString list = token.c_str();
        int count = 1;
        while (is >> token) {
            list += QString(", ") + token.c_str();
            ++count;
        }
        if (count > 1)
            list = "{ " + list + " }";
        QMetaObject::invokeMethod(WatchesWindow::Instance(), "AddWatch", Qt::AutoConnection,
            Q_ARG(QString, list),
            Q_ARG(int, decimation),
            Q_ARG(double, min),
            Q_ARG(double, max)
        );
        return true;
    }
}