////////////////////////////////////////////////////////////////////////////////
// $Id: PCHIncludes.h 7948 2024-03-20 13:38:20Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: This file contains frequently used, stable header files from
//   external libraries. These headers may then be pre-compiled by compilers
//   that support it (MSVC, Borland).
//   When building without pre-compiled headers, specify NO_PCHINCLUDES as a
//   compiler flag to avoid unnecessary inclusion of the headers specified
//   here.
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
#ifndef PCHINCLUDES_H
#define PCHINCLUDES_H

#ifdef __BORLANDC__
#pragma warn - 8004 // suppress warning on proper initialization of variables
#pragma warn - 8027 // suppress warning on inline expansion failure
#pragma warn - 8058 // suppress warning on initialized data in header
#endif

#if defined(NO_PCHINCLUDES)
#undef PRECOMPILED_HEADERS
#elif __BORLANDC__
#define PRECOMPILED_HEADERS 1
#endif

#if PRECOMPILED_HEADERS && defined(__cplusplus)

#ifdef __BORLANDC__
#include <vcl.h>
#endif

// Don't #include <windows.h> from this file. In some projects, it is necessary
// to have *other* #includes or #defines before <windows.h>. Including it here
// would break those projects.

// STL headers
#include <list>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

#endif // PRECOMPILED_HEADERS

#endif // PCHINCLUDES_H
