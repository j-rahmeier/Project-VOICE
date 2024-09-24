// Import definitions for flexX library
// generated Thu Feb 22 18:25:51 2024 by DylibTool
#ifdef STATIC_LIBFLEXX

namespace Dylib { bool flexX_Loaded() { return true; } }


#else // STATIC_LIBFLEXX

#include "flexX.imports.h"
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( flexX, "flexX", functionImports, notFoundMsg, notFoundURL );

decltype(&__3e323744_flexX_ClosePort) flexX_ClosePort = 0;
decltype(&__18135064_flexX_GetBatteryStatus) flexX_GetBatteryStatus = 0;
decltype(&__607f5b71_flexX_GetChannelData) flexX_GetChannelData = 0;
decltype(&__05172405_flexX_GetEnabledChannels) flexX_GetEnabledChannels = 0;
decltype(&__3e103971_flexX_GetFilter) flexX_GetFilter = 0;
decltype(&__5112331f_flexX_GetRegister) flexX_GetRegister = 0;
decltype(&__4c625c1a_flexX_GetResolution) flexX_GetResolution = 0;
decltype(&__407c0602_flexX_GetSWversion) flexX_GetSWversion = 0;
decltype(&__5d0c2a1e_flexX_GetSampling) flexX_GetSampling = 0;
decltype(&__29297a67_flexX_OpenPort) flexX_OpenPort = 0;
decltype(&__5d7d581d_flexX_RecNo) flexX_RecNo = 0;
decltype(&__05173005_flexX_SetEnabledChannels) flexX_SetEnabledChannels = 0;
decltype(&__3e102d71_flexX_SetFilter) flexX_SetFilter = 0;
decltype(&__5112271f_flexX_SetRegister) flexX_SetRegister = 0;
decltype(&__4c62481a_flexX_SetResolution) flexX_SetResolution = 0;
decltype(&__5d0c3e1e_flexX_SetSampling) flexX_SetSampling = 0;
decltype(&__5f41420c_flexX_Start) flexX_Start = 0;
decltype(&__5143360c_flexX_Stop) flexX_Stop = 0;


namespace {
const Dylib::Import functionImports_[] =
{
  { "flexX_ClosePort", (void**)&flexX_ClosePort, Dylib::Import::cMangled },
  { "flexX_GetBatteryStatus", (void**)&flexX_GetBatteryStatus, Dylib::Import::cMangled },
  { "flexX_GetChannelData", (void**)&flexX_GetChannelData, Dylib::Import::cMangled },
  { "flexX_GetEnabledChannels", (void**)&flexX_GetEnabledChannels, Dylib::Import::cMangled },
  { "flexX_GetFilter", (void**)&flexX_GetFilter, Dylib::Import::cMangled },
  { "flexX_GetRegister", (void**)&flexX_GetRegister, Dylib::Import::cMangled },
  { "flexX_GetResolution", (void**)&flexX_GetResolution, Dylib::Import::cMangled },
  { "flexX_GetSWversion", (void**)&flexX_GetSWversion, Dylib::Import::cMangled },
  { "flexX_GetSampling", (void**)&flexX_GetSampling, Dylib::Import::cMangled },
  { "flexX_OpenPort", (void**)&flexX_OpenPort, Dylib::Import::cMangled },
  { "flexX_RecNo", (void**)&flexX_RecNo, Dylib::Import::cMangled },
  { "flexX_SetEnabledChannels", (void**)&flexX_SetEnabledChannels, Dylib::Import::cMangled },
  { "flexX_SetFilter", (void**)&flexX_SetFilter, Dylib::Import::cMangled },
  { "flexX_SetRegister", (void**)&flexX_SetRegister, Dylib::Import::cMangled },
  { "flexX_SetResolution", (void**)&flexX_SetResolution, Dylib::Import::cMangled },
  { "flexX_SetSampling", (void**)&flexX_SetSampling, Dylib::Import::cMangled },
  { "flexX_Start", (void**)&flexX_Start, Dylib::Import::cMangled },
  { "flexX_Stop", (void**)&flexX_Stop, Dylib::Import::cMangled },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBFLEXX

