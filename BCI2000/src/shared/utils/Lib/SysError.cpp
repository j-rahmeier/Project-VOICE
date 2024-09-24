#include "SysError.h"
/////////////////////////////////////////////////////////////////////////////
// $Id: SysError.cpp 7718 2023-11-20 14:46:51Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class for OS error codes and messages.
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
#include "SysError.h"

#if _WIN32
#include "Windows.h"
#else
#include <cerrno>
#include <cstring>
#endif

namespace Tiny
{

SysError::SysError() : mCode(0)
{
#ifdef _WIN32
    mCode = ::GetLastError();
#else
    mCode = errno;
#endif
};

bool SysError::IsKnown(void* inContext) const
{
    bool result = true;
#if _WIN32
    char *pMessage = NULL;
    int flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
    if (inContext)
        flags |= FORMAT_MESSAGE_FROM_HMODULE;
    result = ::FormatMessageA(flags, inContext, mCode, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
        reinterpret_cast<char*>(&pMessage), 0, NULL);
    if (result)
        ::LocalFree(pMessage);
#endif
    return result;
}

const char *SysError::Message(void *inContext) const
{
    mMessage = "unknown error";
#if _WIN32
    char *pMessage = NULL;
    int flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
    if (inContext)
        flags |= FORMAT_MESSAGE_FROM_HMODULE;
    bool success = ::FormatMessageA(flags, inContext, mCode, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
                                    reinterpret_cast<char *>(&pMessage), 0, NULL);
    if (success)
    {
        mMessage = pMessage;
        ::LocalFree(pMessage);
    }
#else
    mMessage = std::strerror(mCode);
#endif // _WIN32
    while (!mMessage.empty() && (::isspace(mMessage.back()) || mMessage.back() == '.'))
        mMessage = mMessage.substr(0, mMessage.length() - 1);
    mMessage += " (0x";
    unsigned long code = mCode;
    bool leadingZeros = true;
    for (int i = 2 * sizeof(mCode) - 1; i >= 0; --i)
    {
        char digit = (code >> (4 * i)) & 0xf;
        if (digit < 10)
            digit += '0';
        else
            digit += 'a' - 10;
        leadingZeros = leadingZeros && digit == '0' && i > 1;
        if (!leadingZeros)
            mMessage += digit;
    }
    mMessage += ")";
    return mMessage.c_str();
}

} // namespace Tiny
