// Import definitions for xipplib library
// generated Sat Sep  3 08:53:03 2022 by DylibTool
#ifdef STATIC_LIBXIPPLIB

namespace Dylib { bool xipplib_Loaded() { return true; } }


#else // STATIC_LIBXIPPLIB

#include "xipplib.imports.h"
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( xipplib, "xipplib", functionImports, notFoundMsg, notFoundURL );



namespace {
const Dylib::Import functionImports_[] =
{
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBXIPPLIB

