###########################################################################
## $Id: CMakeLists.txt 7678 2023-10-26 14:21:43Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for the AppConnectorExample

IF( BUILD_DEMOS )

# Set the executable name
SET( NAME AppConnectorExample )

# Set the project specific sources
SET( SRC_PROJECT
  main.cpp
  mainUI.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Qt/QtMain.cpp
  ${PROJECT_SRC_DIR}/shared/types/Blob.cpp
)

set( DIR_NAME Demos/AppConnector )
SET_OUTPUT_DIRECTORY( ${PROJECT_BUILD_ROOT}/src/contrib/AppConnectorApplications/AppConnectorExample )
utils_include( frameworks/LibTiny )
BCI2000_ADD_TARGET( QTAPP ${NAME} ${SRC_PROJECT} ${HDR_PROJECT} )
IF( WIN32 AND NOT FAILED )
  TARGET_LINK_LIBRARIES( ${NAME} ws2_32 )
ENDIF()

ENDIF( BUILD_DEMOS )
