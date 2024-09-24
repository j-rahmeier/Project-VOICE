////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.com
// Description: Base class for an AVSource enumerator.
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
#ifndef AV_SOURCE_ENUMERATOR_H
#define AV_SOURCE_ENUMERATOR_H

#include <sstream>
#include <vector>
#include "VideoMediaType.h"

class AVSource;

class AVSourceEnumerator
{
public:
    AVSourceEnumerator() {}
    virtual ~AVSourceEnumerator() {}
    AVSourceEnumerator(const AVSourceEnumerator&) = delete;
    AVSourceEnumerator& operator=(const AVSourceEnumerator&) = delete;

    struct Device { std::string driver, name, url, friendlyName; };
    virtual std::vector<Device> ListDevices() const = 0;

    virtual std::vector<VideoMediaType> ListVideoMediaTypes(const Device&) const = 0;
    virtual VideoMediaType MatchVideoMediaType(const std::vector<VideoMediaType>&, const VideoMediaType&) const;

    virtual AVSource* CreateSource(const Device&, const VideoMediaType&) const = 0;
    virtual AVSource* CreateSource(const std::string& url, double timeoutSec, int bufferSizeBytes) const;

    std::string Error() const { return mError.str(); }

protected:
    std::ostringstream& ErrorStream() { return mError; }

private:
    std::ostringstream mError;
};

#endif // AV_SOURCE_ENUMERATOR_H
