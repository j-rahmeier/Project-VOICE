###########################################################################
## $Id: CMakeLists.txt 7678 2023-10-26 14:21:43Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for SchedulingTool.

SET( DIR_NAME Build/Tools )
UTILS_INCLUDE( frameworks/LibTiny )
UTILS_INCLUDE( frameworks/Core )
ADD_DEFINITIONS( -DUSE_QT )
if( WIN32 )
  BCI2000_ADD_TARGET( QTAPP SchedulingTool
    SchedulingTool.cpp
    ${PROJECT_SRC_DIR}/shared/utils/Qt/QtSignalConnector.h
    ${PROJECT_SRC_DIR}/shared/utils/Qt/QtMain.cpp
    OUTPUT_DIRECTORY ${BUILDUTILS_DIR}/SchedulingTool
    LIBRARY pdh
  )
endif()


