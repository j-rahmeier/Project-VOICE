// Import definitions for AmplifierSDK library
// generated Thu Oct 31 11:39:44 2019 by DylibTool
#ifdef STATIC_LIBAMPLIFIERSDK

namespace Dylib { bool AmplifierSDK_Loaded() { return true; } }


#else // STATIC_LIBAMPLIFIERSDK

#include "AmplifierSDK.imports.h"
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( AmplifierSDK, "AmplifierSDK", functionImports, notFoundMsg, notFoundURL );

decltype(&__4f4b6f05_CloseDevice) CloseDevice = 0;
decltype(&__2c3b141c_EnumerateDevices) EnumerateDevices = 0;
decltype(&__26111544_GetData) GetData = 0;
decltype(&__35370a30_GetDeviceAddress) GetDeviceAddress = 0;
decltype(&__29031b49_GetInfo) GetInfo = 0;
decltype(&__477e7d35_GetProperty) GetProperty = 0;
decltype(&__26101a02_GetPropertyRange) GetPropertyRange = 0;
decltype(&__68701307_OpenDevice) OpenDevice = 0;
decltype(&__72641033_SetAmplifierFamily) SetAmplifierFamily = 0;
decltype(&__40171f28_SetMarker) SetMarker = 0;
decltype(&__537e7d35_SetProperty) SetProperty = 0;
decltype(&__26351e04_StartAcquisition) StartAcquisition = 0;
decltype(&__26336078_StartRecording) StartRecording = 0;
decltype(&__120b1971_StopAcquisition) StopAcquisition = 0;
decltype(&__14756571_StopRecording) StopRecording = 0;
decltype(&__34196269_StoreDataBlock) StoreDataBlock = 0;


namespace {
const Dylib::Import functionImports_[] =
{
  { "CloseDevice", (void**)&CloseDevice, Dylib::Import::cMangled },
  { "EnumerateDevices", (void**)&EnumerateDevices, Dylib::Import::cMangled },
  { "GetData", (void**)&GetData, Dylib::Import::cMangled },
  { "GetDeviceAddress", (void**)&GetDeviceAddress, Dylib::Import::cMangled },
  { "GetInfo", (void**)&GetInfo, Dylib::Import::cMangled },
  { "GetProperty", (void**)&GetProperty, Dylib::Import::cMangled },
  { "GetPropertyRange", (void**)&GetPropertyRange, Dylib::Import::cMangled },
  { "OpenDevice", (void**)&OpenDevice, Dylib::Import::cMangled },
  { "SetAmplifierFamily", (void**)&SetAmplifierFamily, Dylib::Import::cMangled },
  { "SetMarker", (void**)&SetMarker, Dylib::Import::cMangled },
  { "SetProperty", (void**)&SetProperty, Dylib::Import::cMangled },
  { "StartAcquisition", (void**)&StartAcquisition, Dylib::Import::cMangled },
  { "StartRecording", (void**)&StartRecording, Dylib::Import::cMangled },
  { "StopAcquisition", (void**)&StopAcquisition, Dylib::Import::cMangled },
  { "StopRecording", (void**)&StopRecording, Dylib::Import::cMangled },
  { "StoreDataBlock", (void**)&StoreDataBlock, Dylib::Import::cMangled },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBAMPLIFIERSDK

