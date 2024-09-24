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
#include "ScriptingExtensions.h"

#include "ParserToken.h"
#include <sstream>
#include <vector>
#include <string>
#include <regex>

namespace {
    std::vector<ScriptingExtensions::ObjectType*> sObjectTypes;

    std::string ConstructHelpString()
    {
        std::string s;
        for (const auto pType : sObjectTypes) {
            s += pType->Help();
            s += ", ";
        }
        if (!s.empty()) {
            s.pop_back();
            s.pop_back();
        }
        return s;
    }

    const std::string& HelpString()
    {
        // Must reside in static memory because data pointer is returned from
        // function in Help()
        static std::string sHelpString = ConstructHelpString();
        return sHelpString;
    }
}

namespace ScriptingExtensions {

    bool InterpretCommand(std::istream& is, std::ostream& os)
    {
        ParserToken verb, object;
        if (!(is >> verb >> object))
            return false;
        for (const auto pObjectType : sObjectTypes) {
            std::regex pattern(pObjectType->Name(), std::regex::icase);
            if (std::regex_match(object, pattern))
                return pObjectType->Execute(verb.c_str(), is, os);
        }
        return false;
    }

    const char* Help()
    {
        return HelpString().c_str();
    }

    ObjectType::ObjectType()
    {
        sObjectTypes.push_back(this);
    }

    bool ObjectType::Execute(const char* verb, std::istream& is, std::ostream& os)
    {
        auto pEntry = MethodTable();
        while (pEntry && pEntry->verb) {
            if (!::stricmp(pEntry->verb, verb))
                return pEntry->action(is, os);
            ++pEntry;
        }
        return false;
    }

}

