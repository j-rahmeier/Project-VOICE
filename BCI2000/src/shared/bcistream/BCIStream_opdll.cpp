////////////////////////////////////////////////////////////////////////////////
// $Id: BCIStream_opdll.cpp 8066 2024-04-23 22:47:26Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Implementation of bcierr and bciout message handlers for the
//   BCI2000 operator DLL.
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
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include "BCIStream.h"
#include "BCI_OperatorLib.h"
#include "StateMachine.h"
#include <iostream>

extern std::atomic<StateMachine *> gpStateMachine;

// No redirection from stdio, would lead to infinite recursion in cmdline clients
bool BCIStream::InitialRedirection()
{
    return true;
}

static void Handle(const std::string &inMessage, int inCallback)
{
    std::string message = inMessage;
    if (message.find_last_of('\n') == message.length() - 1)
        message = message.substr(0, message.length() - 1);
    if (gpStateMachine && (*gpStateMachine).CallbackFunction(inCallback))
        (*gpStateMachine).LogMessage(inCallback, message.c_str());
    else
    {
        std::ostream *pOut = &std::clog;
        std::string title = "BCI2000 Operator";
        switch (inCallback)
        {
        case BCI_OnErrorMessage:
            title += " Error";
            pOut = &std::cerr;
            break;
        case BCI_OnWarningMessage:
            title += " Warning";
            pOut = &std::clog;
            break;
        }
#ifdef _WIN32
        ::MessageBoxA(NULL, message.c_str(), title.c_str(), MB_OK);
#else
        *pOut << message << std::endl;
#endif
    }
}

bool BCIStream::CompressMessages()
{
    return false;
}

void BCIStream::PlainMessage(const std::string &s)
{
    Handle(s, BCI_OnLogMessage);
}

void BCIStream::DebugMessage(const std::string &s)
{
    Handle(s, BCI_OnDebugMessage);
}

void BCIStream::NoteMessage(const std::string& s)
{
    Handle(s, BCI_OnNoteMessage);
}

void BCIStream::Warning(const std::string &s)
{
    Handle(s, BCI_OnWarningMessage);
}

void BCIStream::ConfigurationError(const std::string &s)
{
    Handle(s, BCI_OnErrorMessage);
}

void BCIStream::RuntimeError(const std::string &s)
{
    Handle(s, BCI_OnErrorMessage);
}

void BCIStream::LogicError(const std::string &s)
{
    Handle(s, BCI_OnErrorMessage);
}
