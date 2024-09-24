//////////////////////////////////////////////////////////////////////
// $Id$
// Authors: mellinger@neurotechcenter.de
// Description: Scripting extension dealing with the Operator's
//  window title.
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
#include "TitleType.h"

#include "MainWindow.h"

#include "ParserToken.h"

namespace {
    ScriptingExtensions::TitleType sInstance;
    const char* sHelp = "Set Title <title>";
}

namespace ScriptingExtensions {
    const ObjectType::MethodEntry TitleType::sMethodTable[] = {
      {"Set", &TitleType::Set },
      { 0 }
    };

    const char* TitleType::Name() const
    {
        return "Title";
    }

    const char* TitleType::Help() const
    {
        return sHelp;
    }

    const ObjectType::MethodEntry* TitleType::MethodTable() const
    {
        return sMethodTable;
    }

    bool TitleType::Set(std::istream& is, std::ostream& os)
    {
        std::string title;
        ParserToken token;
        while (is >> token)
            title += token + " ";
        if (!title.empty())
            title.pop_back();
        QString qtitle = QString::fromUtf8(title.c_str());
        QMetaObject::invokeMethod(MainWindow::Instance(), "SetWindowTitle", Qt::AutoConnection, Q_ARG(QString, qtitle));
        return true;
    }

}