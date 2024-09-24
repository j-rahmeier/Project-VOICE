//////////////////////////////////////////////////////////////////////
// $Id: EncodedString.h 7563 2023-09-01 12:40:45Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A string class that allows for transparent handling of
//   character codes using the '%' character.
//
//   InsertInto() will always output white space in encoded
//   hexadecimal form.
//   Additional characters may be listed in the "encodeThese"
//   parameter.
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
#ifndef ENCODED_STRING_H
#define ENCODED_STRING_H

#include "StringUtils.h"
#include <iostream>
#include <string>

class EncodedString : public StringUtils::String
{
  public:
    EncodedString();
    EncodedString(const StringUtils::String &);
    EncodedString(const char *);
    EncodedString& operator=(const StringUtils::String &);
    EncodedString& operator=(const char*);

    // The "encodeThese" parameter lists additional characters that will be written
    // in encoded form.
    std::ostream &InsertInto(std::ostream &, const std::string &encodeThese = "") const;
    std::istream &ExtractFrom(std::istream &);
};

inline EncodedString::EncodedString()
{
}

inline EncodedString::EncodedString(const StringUtils::String &s) : StringUtils::String(s)
{
}

inline EncodedString::EncodedString(const char *s) : StringUtils::String(s)
{
}

inline EncodedString& EncodedString::operator=(const StringUtils::String& s)
{
    StringUtils::String::operator=(s); return *this;
}

inline EncodedString& EncodedString::operator=(const char* s)
{
    StringUtils::String::operator=(s); return *this;
}

inline std::ostream &operator<<(std::ostream &s, const EncodedString &e)
{
    return e.InsertInto(s);
}

inline std::istream &operator>>(std::istream &s, EncodedString &e)
{
    return e.ExtractFrom(s);
}

#endif // ENCODED_STRING_H
