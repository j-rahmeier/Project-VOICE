////////////////////////////////////////////////////////////////////////////////
// $Id: VisID.cpp 7536 2023-08-17 17:46:25Z mellinger $
// Author: griffin.milsap@gmail.com, juergen.mellinger@uni-tuebingen.de
// Description: A class representing a Visualization ID.
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
////////////////////////////////////////////////////////////////////////////////
#include "VisID.h"

#include "BCIStream.h"

static const char cSeparator = ':';

VisID::VisID()
{
}

VisID::VisID(const std::string &s) : EncodedString(s)
{
}

VisID::VisID(const char *s) : EncodedString(s)
{
}

VisID &VisID::SetWindowID(const std::string &s)
{
    if (s.find(cSeparator) != std::string::npos)
        bcierr << "Window IDs may not contain the '" << cSeparator << "' character";

    if (IsLayer())
        *this = s + cSeparator + LayerID();
    else
        *this = s;
    return *this;
}

std::string VisID::WindowID() const
{
    return Substr(0, Find(cSeparator));
}

VisID &VisID::SetLayerID(const std::string &s)
{
    return *this = WindowID() + cSeparator + s;
}

std::string VisID::LayerID() const
{
    size_t pos = Find(cSeparator);
    if (pos != std::string::npos)
        return Substr(pos).Substr(1);
    return std::string();
}

bool VisID::IsLayer() const
{
    return Find(cSeparator) != std::string::npos;
}

VisID &VisID::ToLayer()
{
    return SetLayerID(LayerID());
}
