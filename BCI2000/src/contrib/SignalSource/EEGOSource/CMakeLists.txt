###########################################################################
## $Id: CMakeLists.txt 7648 2023-10-07 15:24:53Z mellinger $
## Authors: sheimes@ant-neuro.com
## Description: Build information for the EEGO source module
###########################################################################

bci2000_include(eemagine)
BCI2000_ADD_SIGNAL_SOURCE_MODULE(
  EEGO
  EEGO.cpp
  SUPPORT batch/*=>/batch
  SUPPORT parms/*=>/parms/fragments/amplifiers
)
