###########################################################################
## $Id: CMakeLists.txt 6261 2021-04-07 18:59:38Z abelsten $
## Authors: griffin.milsap@gmail.com
## Description: Build information for the gHIampSource source module
###########################################################################

if(WIN32)
  ADD_SUBDIRECTORY( getinfo )
  BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
    gHIampSource
    gHIampADC.cpp
    gHIampDevice.cpp
    lib/gHIamp.imports.cpp
  )
endif()
