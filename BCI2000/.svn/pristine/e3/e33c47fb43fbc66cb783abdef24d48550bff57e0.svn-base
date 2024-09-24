###########################################################################
## Authors: t.moore
## Description: Build information for the NC_flexX source module
###########################################################################

# Set the executable name
SET( EXECUTABLE_NAME FlexEEG )

# Set the project specific sources
SET( FILES_PROJECT
  NC_flexXADC.cpp
  lib/flexX.imports.cpp
)
INCLUDE_DIRECTORIES( lib )

IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
  SET( FILES_PROJECT
    ${FILES_PROJECT}
    lib/flexX64.dll
  )
ELSE()
  SET( FILES_PROJECT
    ${FILES_PROJECT}
    lib/flexX32.dll
  )
ENDIF()

# Create the signal source module
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${FILES_PROJECT}" 
  SUPPORT batch/*=>/batch
  SUPPORT parms/*=>/parms/fragments/amplifiers
)
