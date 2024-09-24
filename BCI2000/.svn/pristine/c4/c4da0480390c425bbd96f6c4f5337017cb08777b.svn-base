//////////////////////////////////////////////////////////////////////
// $Id$
// Authors: mellinger@neurotechcenter.de
// Description: Scripting extensions provided by the Operator module.
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
#ifndef SCRIPTING_EXTENSIONS_H
#define SCRIPTING_EXTENSIONS_H

#include <iostream>

namespace ScriptingExtensions {

  // Interprets and executes command, returns true if successful
  bool InterpretCommand(std::istream&, std::ostream&);
  // Lists scripting extensions commands
  const char* Help();

  // A class for self-registering object types that know how to interpret verbs
  class ObjectType
  {
    public:
        // Methods are provided with an istream for reading remaining arguments,
        // and an ostream for error messages.
        typedef bool (*Action)(std::istream&, std::ostream&);
        struct MethodEntry { const char* verb; Action action; };

    protected:
        ObjectType();

        ObjectType(const ObjectType&) = delete;
        ObjectType& operator=(const ObjectType&) = delete;

    public:
        virtual ~ObjectType() {}

        // The name of the object is typically a single word but
        // may be a regular expression, e.g. "Visualization|Vis".
        virtual const char* Name() const = 0;
        // The help string should list commands that this object type provides,
        // separated with commas, e.g.
        // "Move Visualization <visID> <x> <y>, Resize Visualization <visID> <width> <height>"
        virtual const char* Help() const = 0;

        bool Execute(const char* verb, std::istream&, std::ostream&);

    protected:
        // Method tables are supposed to end with a NULL entry.
        virtual const MethodEntry* MethodTable() const = 0;
  };
}

#endif // SCRIPTING_EXTENSIONS_H
