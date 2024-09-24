// Import definitions for xipplib library
// generated Sat Sep  3 08:53:03 2022 by DylibTool
#ifndef XIPPLIB_IMPORTS_H
#define XIPPLIB_IMPORTS_H

#include "Win32Defs.h"

#ifndef STATIC_LIBXIPPLIB
// Disable function declarations in the original header
// file by #defining function names to point to symbols that
// are not referenced by any code.

#define __declspec(x)
#endif // STATIC_LIBXIPPLIB

#include "XippStimCmd.h"

#ifndef STATIC_LIBXIPPLIB
#undef __declspec
// Use #undef to restore function names before declaring
// function pointers with the names originally used to
// declare imports.

#endif // STATIC_LIBXIPPLIB

namespace Dylib { bool xipplib_Loaded(); }

#endif // XIPPLIB_IMPORTS_H
