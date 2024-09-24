###########################################################################
## $Id: CMakeLists.txt 7792 2024-01-17 18:30:04Z jhill $
## Authors: Jeremy Hill <jezhill@gmail.com>
## Description: Build information for the SerialWidget source module
##              
##              
###########################################################################

SET( EXECUTABLE_NAME SerialWidget )

SET( SERIALINTERFACE_DIR ${BCI2000_SRC_DIR}/contrib/Extensions/SerialInterface )

# Set the project specific sources
SET( SRC_PROJECT
  SerialWidgetADC.cpp  #includes SerialInterface.cpp
  ${SERIALINTERFACE_DIR}/SerialConnection.cpp
)

SET( HDR_PROJECT
  SerialWidgetADC.h    #includes SerialInterface.h
  ${SERIALINTERFACE_DIR}/SerialConnection.h
)

INCLUDE_DIRECTORIES(
  # if the headers are not in the current directory, specify the directories where they can be found   
  ${SERIALINTERFACE_DIR}
)

LINK_DIRECTORIES(
   # ...the same goes for directories where third-party libraries are found (see below) 
)

# Use the BCI2000_INCLUDE macro if you need to link with frameworks from /src/extlib:
#BCI2000_INCLUDE( "SOURCEFILTER" )

# Add the source module to the Makefile or compiler project file:
LIST( APPEND SRC_PROJECT ${BCI2000_SRC_DIR}/shared/modules/CoreMain.cpp )
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
  SerialWidgetDemo.bat
  SerialWidgetDemoMPU6050.bat
)
