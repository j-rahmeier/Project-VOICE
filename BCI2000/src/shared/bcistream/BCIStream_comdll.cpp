////////////////////////////////////////////////////////////////////////////////
// $Id: BCIStream_comdll.cpp 8066 2024-04-23 22:47:26Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Implementation of bcierr and bciout message handlers for a
//              COM module (COM DLL).
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
#if _WIN32
#include <Windows.h>
#endif

#include "BCIStream.h"
#include "EncodedString.h"
#include "FileUtils.h"
#include "MessageChannel.h"
#include "ProcessUtils.h"
#include "Status.h"
#include <iostream>

bool BCIStream::InitialRedirection()
{
    return true;
}

bool BCIStream::CompressMessages()
{
    return true;
}

static void ShowMessageBox(const std::string &inText, const std::string &inTitle, unsigned int inFlags)
{
    std::string executable = FileUtils::InstallationDirectory() + "MessageBox.exe";
    std::ostringstream args;
    args << EncodedString(inText) << " " << EncodedString(inTitle) << " " << inFlags;
    int resultCode;
    if (!ProcessUtils::ExecuteAsynchronously(executable, args.str(), resultCode))
        ::MessageBoxA(NULL, inText.c_str(), inTitle.c_str(), inFlags);
}

void StatusMessage(const std::string &inText, int inCode)
{
    ShowMessageBox(inText, "BCI2000 Error", MB_OK | MB_ICONHAND | MB_SYSTEMMODAL | MB_SETFOREGROUND);
}

void BCIStream::DebugMessage(const std::string &s)
{
    StatusMessage(s, Status::debugMessage);
}

void BCIStream::NoteMessage(const std::string& s)
{
    StatusMessage(s, Status::noteMessage);
}

void BCIStream::PlainMessage(const std::string &s)
{
    StatusMessage(s, Status::plainMessage);
}

void BCIStream::Warning(const std::string &s)
{
    StatusMessage(s, Status::warningMessage);
}

void BCIStream::ConfigurationError(const std::string &s)
{
    StatusMessage(s, Status::configurationError);
}

void BCIStream::RuntimeError(const std::string &s)
{
    StatusMessage(s, Status::runtimeError);
}

void BCIStream::LogicError(const std::string &s)
{
    StatusMessage(s, Status::logicError);
}
