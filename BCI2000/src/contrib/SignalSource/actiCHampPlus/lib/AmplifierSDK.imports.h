// Import definitions for AmplifierSDK library
// generated Thu Oct 31 11:39:44 2019 by DylibTool
#ifndef AMPLIFIERSDK_IMPORTS_H
#define AMPLIFIERSDK_IMPORTS_H

#include "Win32Defs.h"

#ifndef STATIC_LIBAMPLIFIERSDK
// Disable function declarations in the original header
// file by #defining function names to point to symbols that
// are not referenced by any code.
#define CloseDevice __4f4b6f05_CloseDevice
#define EnumerateDevices __2c3b141c_EnumerateDevices
#define GetData __26111544_GetData
#define GetDeviceAddress __35370a30_GetDeviceAddress
#define GetInfo __29031b49_GetInfo
#define GetProperty __477e7d35_GetProperty
#define GetPropertyRange __26101a02_GetPropertyRange
#define OpenDevice __68701307_OpenDevice
#define SetAmplifierFamily __72641033_SetAmplifierFamily
#define SetMarker __40171f28_SetMarker
#define SetProperty __537e7d35_SetProperty
#define StartAcquisition __26351e04_StartAcquisition
#define StartRecording __26336078_StartRecording
#define StopAcquisition __120b1971_StopAcquisition
#define StopRecording __14756571_StopRecording
#define StoreDataBlock __34196269_StoreDataBlock

#define __declspec(x)
#endif // STATIC_LIBAMPLIFIERSDK

#include "AmplifierSDK.h"

#ifndef STATIC_LIBAMPLIFIERSDK
#undef __declspec
// Use #undef to restore function names before declaring
// function pointers with the names originally used to
// declare imports.
#undef CloseDevice
extern decltype(&__4f4b6f05_CloseDevice) CloseDevice;
#undef EnumerateDevices
extern decltype(&__2c3b141c_EnumerateDevices) EnumerateDevices;
#undef GetData
extern decltype(&__26111544_GetData) GetData;
#undef GetDeviceAddress
extern decltype(&__35370a30_GetDeviceAddress) GetDeviceAddress;
#undef GetInfo
extern decltype(&__29031b49_GetInfo) GetInfo;
#undef GetProperty
extern decltype(&__477e7d35_GetProperty) GetProperty;
#undef GetPropertyRange
extern decltype(&__26101a02_GetPropertyRange) GetPropertyRange;
#undef OpenDevice
extern decltype(&__68701307_OpenDevice) OpenDevice;
#undef SetAmplifierFamily
extern decltype(&__72641033_SetAmplifierFamily) SetAmplifierFamily;
#undef SetMarker
extern decltype(&__40171f28_SetMarker) SetMarker;
#undef SetProperty
extern decltype(&__537e7d35_SetProperty) SetProperty;
#undef StartAcquisition
extern decltype(&__26351e04_StartAcquisition) StartAcquisition;
#undef StartRecording
extern decltype(&__26336078_StartRecording) StartRecording;
#undef StopAcquisition
extern decltype(&__120b1971_StopAcquisition) StopAcquisition;
#undef StopRecording
extern decltype(&__14756571_StopRecording) StopRecording;
#undef StoreDataBlock
extern decltype(&__34196269_StoreDataBlock) StoreDataBlock;

#endif // STATIC_LIBAMPLIFIERSDK

namespace Dylib { bool AmplifierSDK_Loaded(); }

#endif // AMPLIFIERSDK_IMPORTS_H
