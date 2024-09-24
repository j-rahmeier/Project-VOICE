###########################################################################
## $Id: CMakeLists.txt 7043 2022-12-04 19:27:36Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for the Blackrock source module
###########################################################################

IF( WIN32 )

# Set the executable name
SET( EXECUTABLE_NAME Blackrock )

SET( SRC_PROJECT 
  BlackrockADC.cpp
)

SET( HDR_PROJECT
  BlackrockADC.h
)

SET( FILES_PROJECT
  ${FILES_PROJECT}
  SUPPORT batch/*=>/batch
  SUPPORT parms/*=>/parms/fragments/amplifiers
)

INCLUDE_DIRECTORIES( include )

# Add the source module to the Makefile or compiler project file:
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}"
  "${SRC_PROJECT}" "${HDR_PROJECT}"
  "${FILES_PROJECT}"
)

IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  target_link_libraries( ${EXECUTABLE_NAME} ${PROJECT_SRC_DIR}/contrib/SignalSource/Blackrock/lib/x64/cbsdkx64.lib )
ELSE()
  target_link_libraries( ${EXECUTABLE_NAME} ${PROJECT_SRC_DIR}/contrib/SignalSource/Blackrock/lib/x86/cbsdk.lib )
ENDIF()

ENDIF( WIN32 )

