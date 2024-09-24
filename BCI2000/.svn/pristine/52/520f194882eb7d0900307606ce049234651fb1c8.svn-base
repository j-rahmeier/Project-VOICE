###########################################################################
## Authors:
## Description: Build information for the actiCHampPlus source module
###########################################################################
IF( WIN32 )

# Use the bci2000_include macro if you need to link with frameworks from /src/extlib:
# bci2000_include( MATH )

# Set the project specific sources
SET( SRC_PROJECT
  actiCHampPlusADC.cpp
  lib/AmplifierSDK.imports.cpp
  lib/AmplifierSDK.imports.h
  actiCHampPlusADC.h
  lib/AmplifierSDK.h
  lib/Amplifier_LIB.h
  lib/RawDataHandlerExample.h
  SDK.h
)


IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  list( APPEND SRC_PROJECT lib/dylib/x64/AmplifierSDK.dll )
  list( APPEND SRC_PROJECT lib/dylib/x64/actiCHampLib.dll )
  list( APPEND SRC_PROJECT lib/dylib/x64/ActiCHamp_x64.dll)
ELSE()
  list( APPEND SRC_PROJECT lib/dylib/x86/AmplifierSDK.dll )
  list( APPEND SRC_PROJECT lib/dylib/x86/actiCHampLib.dll )
  list( APPEND SRC_PROJECT lib/dylib/x86/ActiCHamp_x86.dll)
ENDIF()

bci2000_add_signal_source_module(
  actiCHampPlus NOBUNDLE
  "${SRC_PROJECT}"
)

ENDIF( WIN32 )
