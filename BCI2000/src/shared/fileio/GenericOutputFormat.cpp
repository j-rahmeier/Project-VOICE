////////////////////////////////////////////////////////////////////////////////
// $Id: GenericOutputFormat.cpp 7464 2023-06-30 15:04:08Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A virtual class interface for output data formats.
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
#include "GenericOutputFormat.h"

void GenericOutputFormat::Preflight(const SignalProperties &, const StateVector &) const
{
    OptionalParameter("StorageTime");
}

void GenericOutputFormat::AcquireTime(time_t &t, tm *&time) const
{
    if (OptionalParameter("StorageTime", "") != "")
    {
        const char *time_str = Parameter("StorageTime").c_str();
        std::time(&t);
        time = std::localtime(&t);
        int y, mo, d, h, mi, s;
        std::sscanf(time_str, "%d-%d-%dT%d:%d:%d", &y, &mo, &d, &h, &mi, &s);
        time->tm_year = y - 1900;
        time->tm_mon = mo - 1;
        time->tm_mday = d;
        time->tm_hour = h;
        time->tm_min = mi;
        time->tm_sec = s;
        t = std::mktime(time);
    }
    else
    {
        t = std::time(nullptr);
        time = std::localtime(&t);
    }
}

