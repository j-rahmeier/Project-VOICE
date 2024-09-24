/////////////////////////////////////////////////////////////////////////////
// $Id$
// Authors: mellinger@neurotechcenter.org
// Description: An intermediate class handling communication between a
//   webcam implementation, and the WebcamLogger class
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
/////////////////////////////////////////////////////////////////////////////
#ifndef WEBCAM_CLIENT_H
#define WEBCAM_CLIENT_H

#include <cstdint>

class WebcamClient
{
public:
    virtual ~WebcamClient() {}
    // Called for each increment of decimated frame number
    virtual void OnWebcamFrameNumber(uint64_t) = 0;
    // Called for each raw input frame
    virtual void OnWebcamFrameData(int, int, const void*) = 0;
    // Called for (warning) messages from webcam processing
    virtual void OnWebcamMessage(const std::string&) = 0;
};

#endif // WEBCAM_CLIENT_H
