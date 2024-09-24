###########################################################################
## $Id: CMakeLists.txt 6628 2022-03-18 17:35:29Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for AmpServerPro source module

# Create the signal source module
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  AmpServerPro 
  AmpServerProADC.cpp
  tinyxml2.cpp
  NetworkPacketTypes.h
  SUPPORT support/StimulusPresentation_AmpServerPro.bat=>/batch
  SUPPORT support/AmpServerPro.prm=>/parms/fragments/amplifiers
)

