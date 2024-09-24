###########################################################################
## Authors: engel@DESKTOP-FGIS0J3.wucon.wustl.edu
## Description: Build information for the PegasusAstroFilter project
###########################################################################

if(WIN32)
# Use the bci2000_include macro if you need to link with frameworks from /src/extlib:
# bci2000_include( MATLAB )

bci2000_add_signal_processing_module(
  PegasusAstroFilter #NOBUNDLE
  PipeDefinition.cpp
  PegasusAstroFilter.cpp
  SerialPort.cpp
  PegasusAstroUSB.cpp
)
endif(WIN32)
