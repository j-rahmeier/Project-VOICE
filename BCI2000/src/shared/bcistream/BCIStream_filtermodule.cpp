////////////////////////////////////////////////////////////////////////////////
// $Id: BCIStream_filtermodule.cpp 8203 2024-06-19 16:53:47Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Implementation of bcierr and bciout message handlers for a
//              filter module. bcierr and friends should be redirected to
//              outside the module, and no messages should arrive here.
//              Still, we send them to the redirected streams to make sure
//              messages are displayed.
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

bool BCIStream::InitialRedirection()
{
    return RedirectFromStdio();
}

bool BCIStream::CompressMessages()
{
    return false;
}

void BCIStream::PlainMessage(const std::string &message)
{
    bciout__ << message << std::flush;
}

void BCIStream::DebugMessage(const std::string &message)
{
    bcidbg__ << message << std::flush;
}

void BCIStream::Warning(const std::string &message)
{
    bciwarn__ << message << std::flush;
}

void BCIStream::ConfigurationError(const std::string &message)
{
    bcierr__ << message << std::flush;
}

void BCIStream::RuntimeError(const std::string &message)
{
    bcierr__ << message << std::flush;
}

void BCIStream::LogicError(const std::string &message)
{
    bcierr__ << message << std::flush;
}

void BCIStream::NoteMessage(const std::string &message)
{
    bcinote__ << message << std::flush;
}