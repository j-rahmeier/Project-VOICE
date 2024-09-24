###########################################################################
## $Id: CMakeLists.txt 6628 2022-03-18 17:35:29Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for gUSBampSource

if(WIN32)
  ADD_SUBDIRECTORY( getinfo )
  BCI2000_ADD_SIGNAL_SOURCE_MODULE( gUSBampSource
    gUSBampADC.cpp
    lib/gUSBamp.imports.cpp
    SUPPORT batch/*=>/batch
    SUPPORT parms/*=>/parms/fragments/amplifiers
  )
endif()
