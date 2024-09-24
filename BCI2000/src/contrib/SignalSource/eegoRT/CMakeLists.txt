###########################################################################
## $Id: CMakeLists.txt 7648 2023-10-07 15:24:53Z mellinger $
## Authors:
## Description: Build information for the eegoRT source module
###########################################################################

bci2000_include(eemagine)
bci2000_add_signal_source_module(
  eegoRT
  eegoRTADC.cpp
)
