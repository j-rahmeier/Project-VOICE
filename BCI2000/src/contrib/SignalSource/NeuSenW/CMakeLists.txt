###########################################################################
## Authors: lijintao@SH-lijintao
## Description: Build information for the NeuSenW source module
###########################################################################

# Use the bci2000_include macro if you need to link with frameworks from /src/extlib:
# bci2000_include( MATH )

LINK_LIBRARIES(${CMAKE_CURRENT_SOURCE_DIR}/NeusenWSDK/NeusenW3SDKDll.lib)

IF (CMAKE_SIZEOF_VOID_P EQUAL 8 AND WIN32 ) 

bci2000_add_signal_source_module(
  NeuSenW NOBUNDLE
  SUPPORT batch/*=>/batch
  SUPPORT parms/*=>/parms/fragments/amplifiers
  NeuSenWADC.cpp
  NeuSenWSDK/depends.dll
  NeusenWSDK/filter.dll
  NeusenWSDK/hasp_windows_x64_112370.dll
  NeusenWSDK/ippcore.dll
  NeusenWSDK/ipps.dll
  NeusenWSDK/ippsl9.dll
  NeusenWSDK/libpthread-stdcall.dll
  # The following file is from an old version of the VS2012 Redistributables.
  # If it copied into /prog, it will break other executables that require a newer
  # version of this file.
  # Please install it from the VS2012 Redistributables installer if it should be
  # be missing from your machine's system folder.
  # NeusenWSDK/msvcr110.dll
  NeusenWSDK/liquid-dsp.dll
  NeusenWSDK/NeusenW3SDKDll.dll
)
ELSE()
  MESSAGE( "--- NeuSenW libaries not available for this platform. Will not build." )
ENDIF()
