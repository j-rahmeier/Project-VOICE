////////////////////////////////////////////////////////////////////////////////
// $Id: Win32Defs.h 7977 2024-03-28 06:55:45Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A header file that allows to use header files that are written
//   for Windows platforms, but only depend on Windows types rather than the
//   Windows OS itself.
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
#ifndef WIN32_DEFS_H
#define WIN32_DEFS_H

#if _WIN32
#include <Windows.h>
#else // _WIN32
#include <cstdint>

#define __declspec(x)
#define __stdcall
#define __cdecl

typedef char CHAR;
typedef char *LPSTR;

typedef int32_t BOOL;
typedef float FLOAT;

typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;

typedef uint8_t UCHAR;
typedef uint32_t UINT;

typedef void *HANDLE;

typedef void OVERLAPPED;

#endif // _WIN32

#endif // WIN32_DEFS_H
