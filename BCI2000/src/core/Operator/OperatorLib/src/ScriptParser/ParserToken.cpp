//////////////////////////////////////////////////////////////////////
// $Id: ParserToken.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A string class representing a ScriptInterpreter parser token.
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
#include "ParserToken.h"

#include "EncodedString.h"

#include <functional>
#include <ios>
#include <sstream>

std::ostream &ParserToken::InsertInto(std::ostream &os) const
{
    return EncodedString(*this).InsertInto(os, ";#");
}

namespace
{
bool TerminateIf(int c)
{
    return ::isspace(c) || c == ';';
}
} // namespace

std::istream &ParserToken::ExtractFrom(std::istream &is)
{
    if ((is >> std::ws).eof())
        is.setstate(std::ios::failbit);
    else if (ReadUntil(is, TerminateIf))
        Decode();
    return is;
};

std::istream &ParserToken::GetLine(std::istream &is, char delim)
{
    struct EqualToDelim
    {
        int delim;
        bool operator()(int c) const { return c == delim; }
    } equalToDelim = { delim };

    if (ReadUntil(is, equalToDelim))
    {
        is.ignore();
        Decode();
    }
    return is;
}

void ParserToken::Decode()
{
    std::istringstream iss(*this);
    clear();
    EncodedString s;
    while (iss)
    {
        while (::isspace(iss.peek()))
            *this += iss.get();
        if (iss >> s)
            *this += s;
    }
}
