//////////////////////////////////////////////////////////////////////
// $Id$
// Authors: mellinger@neurotechcenter.de
// Description: Scripting extension dealing with the Operator's
//  function buttons.
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
#include "ButtonType.h"

#include "MainWindow.h"
#include "ParserToken.h"

namespace {
    ScriptingExtensions::ButtonType sInstance;
    const char* sHelp = "Set Button <idx> <title> <script>";
}

namespace ScriptingExtensions {
    const ObjectType::MethodEntry ButtonType::sMethodTable[] = {
      {"Set", &ButtonType::Set },
      { 0 }
    };

    const char* ButtonType::Name() const
    {
        return "Button";
    }

    const char* ButtonType::Help() const
    {
        return sHelp;
    }

    const ObjectType::MethodEntry* ButtonType::MethodTable() const
    {
        return sMethodTable;
    }

    bool ButtonType::Set(std::istream& is, std::ostream& os)
    {
        ParserToken idx, caption, script;
        is >> idx >> caption >> script;
        int n = ::atoi(idx.c_str());
        if (n < 1 || n > 4) {
            os << "Function button " << n << " does not exist";
            return false;
        }
        QMetaObject::invokeMethod(MainWindow::Instance(), "SetFunctionButton", Qt::AutoConnection,
            Q_ARG(int, n - 1), // cannot use typedefs here (eg, size_t)
            Q_ARG(QString, QString::fromLocal8Bit(caption.c_str())),
            Q_ARG(QString, QString::fromLocal8Bit(script.c_str())));
        return true;
    }

}