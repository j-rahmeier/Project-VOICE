//////////////////////////////////////////////////////////////////////
// $Id: AVError.cpp 7742 2023-12-05 20:03:39Z mellinger $
// Authors: mellinger@neurotechcenter.org
// Description: A class that deals with translation of libav error
//    codes into messages.
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
#include "AVError.h"

AVError::AVError(int code) : mCode(code)
{
  ::av_strerror(code, mMessage, sizeof(mMessage));
}

int AVError::Code()
{
    return mCode;
}

const char* AVError::Message()
{
    return mMessage;
}

