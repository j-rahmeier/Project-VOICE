###########################################################################
## $Id: CMakeLists.txt 7465 2023-06-30 15:23:43Z mellinger $
## Authors: Alexander Belsten (belsten@neurotechcenter.org)
## Description: Build information for the NeuroOmegaADC source module
###########################################################################

if(WIN32)

# Set the executable name
SET( EXECUTABLE_NAME NeuroOmegaADC)

# Set the project specific sources
SET( SRC_PROJECT
  NeuroOmegaADC.cpp
  include/NeuroOmega.imports.cpp
)

# specify dll files to be copied
IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
  SET( FILES_PROJECT
    ${FILES_PROJECT}
    lib/x64/NeuroOmega_x64.dll
  )
ELSE()
  SET( FILES_PROJECT
    ${FILES_PROJECT}
    lib/x86/NeuroOmega.dll
  )
ENDIF()


# Add the source module to the Makefile or compiler project file:
BCI2000_ADD_SIGNAL_SOURCE_MODULE(
  "${EXECUTABLE_NAME}"
  "${SRC_PROJECT}" "${FILES_PROJECT}"
)

endif()
