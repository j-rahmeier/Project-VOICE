###########################################################################
## Authors: brunnerlab@3260-al-05006.attlocal.net
## Description: Build information for the ForagingTask application module
###########################################################################

# Use the bci2000_include macro if you need to link with frameworks from /src/extlib:
# bci2000_include( 3DAPI )

bci2000_add_application_module(
  ForagingTask NOBUNDLE
  ForagingTask.cpp
)
