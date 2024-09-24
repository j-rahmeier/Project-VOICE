###########################################################################
## $Id: CMakeLists.txt 6134 2020-07-09 15:13:07Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for MatlabSignalProcessing

if(WIN32)
#  bci2000_include( matlab_engine )
  bci2000_add_signal_processing_module(
    MatlabSignalProcessing
    PipeDefinition.cpp 
    MatlabFilter.cpp
    MatlabWrapper.cpp
    SUPPORT /prog/matlab/*.m
    SUPPORT /prog/matlab/*.html
    SUPPORT /batch/MatlabDemo_SignalGenerator.bat
    SUPPORT /parms/examples/MatlabDemo_SignalGenerator.prm
  )
endif()


