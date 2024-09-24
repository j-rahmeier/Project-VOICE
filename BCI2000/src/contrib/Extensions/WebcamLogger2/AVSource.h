////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.com
// Description: Base class for an AVSource to be used as input to the 
//   WebcamLogger.
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
#ifndef AV_SOURCE_H
#define AV_SOURCE_H

#include <sstream>
#include <string>
#include <vector>
#include "Rate.h"

class AVSource
{
public:
    AVSource() {}
    virtual ~AVSource() {}
    AVSource(const AVSource&) = delete;
    AVSource& operator=(const AVSource&) = delete;

    struct Properties
    {
        int width = 0, height = 0;
        Rate fps;
    };
    virtual const struct Properties& Properties() const = 0;

    virtual bool WaitForData(std::vector<uint8_t>&) = 0;

    virtual void AbortWaiting() {}

    std::string Error() const { return mError.str(); }

protected:
    std::ostringstream& ErrorStream() { return mError; }

private:
    std::ostringstream mError;
};

#endif // AV_SOURCE_H
