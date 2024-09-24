###########################################################################
## Authors: Robbie@DESKTOP-U5JGQCU
## Description: Build information for the LabJackSource source module
###########################################################################

# Use the bci2000_include macro if you need to link with frameworks from /src/extlib:
# bci2000_include( MATH )

if( WIN32 )

# Set the executable name
SET( EXECUTABLE_NAME LabJackADC )
SET ( SRC_PROJECT
  LabJackADC.cpp
)

IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  list( APPEND SRC_PROJECT lib/x64/LabJack.LJM.dll
			   lib/x64/LJUDDotNet.dll )
ELSE()
  list( APPEND SRC_PROJECT lib/x86/LabJack.LJM.dll
			   lib/x86/LJUDDotNet.dll )
ENDIF()

SET( HDR_PROJECT
  LabJackADC.h
  include/LabJackM.h
  include/LabJackUD.h
  include/LJM_Utilities.h
  include/LJM_StreamUtilities.h
)

bci2000_add_signal_source_module(
    "${EXECUTABLE_NAME}" 
    "${SRC_PROJECT}" "${HDR_PROJECT}"
)

IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  target_link_libraries( ${EXECUTABLE_NAME} ${PROJECT_SRC_DIR}/contrib/SignalSource/LabJackSource/lib/x64/LabJackM.lib
					    ${PROJECT_SRC_DIR}/contrib/SignalSource/LabJackSource/lib/x64/LabJackUD.lib )
ELSE()
  target_link_libraries( ${EXECUTABLE_NAME} ${PROJECT_SRC_DIR}/contrib/SignalSource/LabJackSource/lib/x86/LabJackM.lib
					    ${PROJECT_SRC_DIR}/contrib/SignalSource/LabJackSource/lib/x86/LabJackUD.lib )
ENDIF()

endif( WIN32 )
