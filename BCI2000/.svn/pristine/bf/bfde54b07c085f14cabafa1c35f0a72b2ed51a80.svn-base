//////////////////////////////////////////////////////////////////////
// $Id$
// Authors: mellinger@neurotechcenter.de
// Description: Scripting extension dealing with notes (remarks saved
//   with data and viewed with BCI2000Viewer)
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
#include "NoteType.h"

#include "NotesWindow.h"
#include "ParserToken.h"

namespace {
    ScriptingExtensions::NoteType sInstance;
    const char* sHelp = "Put Note <note>";
}

namespace ScriptingExtensions {
    const ObjectType::MethodEntry NoteType::sMethodTable[] = {
      {"Put", &NoteType::Put },
      {"Add", &NoteType::Put },
      { 0 }
    };

    const char* NoteType::Name() const
    {
        return "Note";
    }

    const char* NoteType::Help() const
    {
        return sHelp;
    }

    const ObjectType::MethodEntry* NoteType::MethodTable() const
    {
        return sMethodTable;
    }

    bool NoteType::Put(std::istream& is, std::ostream& os)
    {
        std::string note;
        ParserToken token;
        while (is >> token)
            note += token + " ";
        if (!note.empty())
            note.pop_back();
        QString qnote = QString::fromUtf8(note.c_str());
        QMetaObject::invokeMethod(NotesWindow::Instance(), "AddNote", Qt::AutoConnection, Q_ARG(QString, qnote));
        return true;
    }

}