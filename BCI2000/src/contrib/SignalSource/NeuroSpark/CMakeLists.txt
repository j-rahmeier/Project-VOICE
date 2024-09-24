###########################################################################
## Authors: lijintao@56B3
## Description: Build information for the NeuroSpark source module
###########################################################################
if(WIN32)
# Use the bci2000_include macro if you need to link with frameworks from /src/extlib:
# bci2000_include( MATH )

bci2000_add_signal_source_module(
  NeuroSpark NOBUNDLE
  NeuroSparkADC.cpp
)
endif(WIN32)
