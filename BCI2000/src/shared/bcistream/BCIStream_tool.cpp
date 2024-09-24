////////////////////////////////////////////////////////////////////////////////
// $Id: BCIStream_tool.cpp 8066 2024-04-23 22:47:26Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Implementation of bcierr and bciout message handlers for a
//              console-based BCI2000 command line tool.
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
#include "BCIStream.h"
#include <iostream>

bool BCIStream::InitialRedirection()
{
    return true;
}

bool BCIStream::CompressMessages()
{
    return false;
}

void BCIStream::PlainMessage(const std::string &s)
{
    std::clog << s << std::endl;
}

void BCIStream::DebugMessage(const std::string &s)
{
    std::clog << "Dbg: " << s << std::endl;
}

void BCIStream::NoteMessage(const std::string& s)
{
    std::clog << "Note: " << s << std::endl;
}

void BCIStream::Warning(const std::string &s)
{
    std::clog << "Warning: " << s << std::endl;
}

void BCIStream::ConfigurationError(const std::string &s)
{
    std::cerr << "Configuration Error: " << s << std::endl;
}

void BCIStream::RuntimeError(const std::string &s)
{
    std::cerr << "Runtime Error: " << s << std::endl;
}

void BCIStream::LogicError(const std::string &s)
{
    std::cerr << "Logic Error: " << s << std::endl;
}
