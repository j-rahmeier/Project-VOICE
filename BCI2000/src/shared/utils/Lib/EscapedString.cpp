//////////////////////////////////////////////////////////////////////
// $Id: EscapedString.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: See header file.
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
#include "EscapedString.h"
#include <iomanip>

namespace Tiny
{

std::istream &EscapedString::ExtractFrom(std::istream &is)
{
    if ((is >> std::ws).eof())
        is.setstate(std::ios::failbit);
    else
        ReadUntil(is >> std::ws, ::isspace);
    return is;
}

std::ostream &EscapedString::InsertInto(std::ostream &os) const
{
    const struct
    {
        char in;
        const char *out;
    } replacements[] = {
        {' ', "\\ "},  {'\"', "\\\""}, {'\\', "\\\\"}, {'\0', "\\0"}, {'\a', "\\a"},
        {'\t', "\\t"}, {'\v', "\\v"},  {'\f', "\\f"},  {'\r', "\\r"}, {'\n', "\\n"},
    };
    const size_t count = sizeof(replacements) / sizeof(*replacements);
    for (const_iterator i = begin(); i != end(); ++i)
    {
        size_t j = 0;
        while (j < count && *i != replacements[j].in)
            ++j;
        if (j < count)
            os << replacements[j].out;
        else if (::isprint(*i))
            os.put(*i);
        else
            os << "\\x" << std::setw(2) << std::setfill('0') << std::hex << *i;
    }
    return os;
}

} // namespace Tiny
