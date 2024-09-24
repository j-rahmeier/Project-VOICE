////////////////////////////////////////////////////////////////////////////////
// $Id: BCIStream_guiapp.cpp 8184 2024-06-11 19:52:26Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Implementation of bcierr and bciout message handlers for a
//              GUI application.
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

#if _WIN32
#include <Windows.h>
#endif

static std::ostream cerr_(nullptr), clog_(nullptr), cout_(nullptr);

bool BCIStream::InitialRedirection()
{
    cerr_.rdbuf(std::cerr.rdbuf());
    clog_.rdbuf(std::clog.rdbuf());
    cout_.rdbuf(std::cout.rdbuf());
    return RedirectFromStdio();
}

namespace
{

void WriteDebugString(const std::string &inText)
{
#if _WIN32
    if (::IsDebuggerPresent())
    {
        ::OutputDebugStringA(inText.c_str());
        return;
    }
#endif
    cerr_ << inText << std::endl;
}

void DisplayMessageBox(const std::string &inTitle, const std::string &inText)
{
#if _WIN32
    ::MessageBoxA(NULL, inText.c_str(), inTitle.c_str(), MB_OK | MB_ICONHAND | MB_SYSTEMMODAL | MB_SETFOREGROUND);
#else
    cerr_ << inTitle << ": " << inText << std::endl;
#endif
}

enum
{
    debug,
    warning,
    error
};

void DisplayMessage(const std::string &inText, int inType = debug)
{
    std::string text = inText;
    if (text.find_last_of(".!?") != text.length() - 1)
        text += '.';
    std::string kind;
    switch (inType)
    {
    case error:
        DisplayMessageBox("BCI2000 Error", text);
        break;
    case warning:
        DisplayMessageBox("BCI2000 Warning", text);
        break;
    default:
        WriteDebugString(text);
    }
}
} // namespace

bool BCIStream::CompressMessages()
{
    return false;
}

void BCIStream::PlainMessage(const std::string &message)
{
    DisplayMessage(message);
}

void BCIStream::DebugMessage(const std::string &message)
{
    DisplayMessage(message);
}

void BCIStream::NoteMessage(const std::string& message)
{
    DisplayMessage(message);
}

void BCIStream::Warning(const std::string &message)
{
    DisplayMessage(message, warning);
}

void BCIStream::ConfigurationError(const std::string &message)
{
    DisplayMessage(message, error);
}

void BCIStream::RuntimeError(const std::string &message)
{
    DisplayMessage(message, error);
}

void BCIStream::LogicError(const std::string &message)
{
    DisplayMessage(message, error);
}
