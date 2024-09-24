###########################################################################
## Authors:
## Description: Build information for the OpenBCISource source module
###########################################################################

# Use the bci2000_include macro if you need to link with frameworks from /src/extlib:
# bci2000_include( MATH )

# Set the executable name
SET( EXECUTABLE_NAME OpenBCISource )

SET( SRC_PROJECT 
  OpenBCIADC.cpp
)

SET( HDR_PROJECT
  OpenBCIADC.h
)

SET( FILES_PROJECT
  ${FILES_PROJECT}
  SUPPORT batch/*=>/batch
  SUPPORT parms/*=>/parms/fragments/amplifiers
)

if(WIN32)
# Add the source module to the Makefile or compiler project file:
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}"
  "${SRC_PROJECT}" "${HDR_PROJECT}"
  "${FILES_PROJECT}"
)
endif()
