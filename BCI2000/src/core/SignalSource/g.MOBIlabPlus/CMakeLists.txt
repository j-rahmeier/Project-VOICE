###########################################################################
## $Id: CMakeLists.txt 6628 2022-03-18 17:35:29Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for gMOBIlabPlus

if(WIN32)
  bci2000_include( SourceFilter )
  add_definitions( -DGMOBILABPLUS )
  bci2000_add_signal_source_module( gMOBIlabPlus
    ../g.MOBIlab/gMOBIlabADC.cpp
    lib/gMOBIlabplus.imports.cpp
    SUPPORT batch/*=>/batch
    SUPPORT parms/*=>/parms/fragments/amplifiers
  )
endif()

