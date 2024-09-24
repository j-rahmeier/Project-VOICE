###########################################################################
## $Id: CMakeLists.txt 6628 2022-03-18 17:35:29Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for gMOBIlab

if(WIN32)
  bci2000_include( SourceFilter )
  bci2000_add_signal_source_module( gMOBIlab
    gMOBIlabADC.cpp
    lib/spa20a.imports.cpp
    SUPPORT batch/*=>/batch
    SUPPORT parms/*=>/parms/fragments/amplifiers
  )
endif()

