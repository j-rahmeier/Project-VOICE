###########################################################################
## $Id: ToolsGUIAppMacro.cmake 7678 2023-10-26 14:21:43Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Contains a macro for creating a GUI based tool application

MACRO( BCI2000_ADD_TOOLS_GUIAPP ) 
  UTILS_PARSE_ARGS( "NAME;SOURCES" ${ARGV} )

  SET( SOURCES
    ${SOURCES}
    ${PROJECT_SRC_DIR}/shared/bcistream/BCIStream_guiapp.cpp
    ${PROJECT_SRC_DIR}/shared/gui/AboutBox.cpp
    ${PROJECT_SRC_DIR}/shared/gui/ExecutableHelp.cpp
    ${PROJECT_SRC_DIR}/shared/utils/Settings.cpp
    ${PROJECT_SRC_DIR}/shared/gui/ColorListChooser.cpp
    ${PROJECT_SRC_DIR}/shared/utils/Qt/QtMain.cpp
  )
  LIST( FIND SOURCES "OUTPUT_DIRECTORY" idx )
  IF( idx EQUAL -1 )
    SET( SOURCES
      ${SOURCES}
      OUTPUT_DIRECTORY ${PROJECT_BUILD_ROOT}/tools/${NAME}
    )
  ENDIF()

  UTILS_INCLUDE( frameworks/Core )
  utils_set_appicon( "wrench;signal-document;text-document" )
  BCI2000_ADD_TARGET(
    INFO Tool
    QTAPP ${NAME}
    ${SOURCES}
  )
  if( NOT failed )
    BCI2000_ADD_TO_INVENTORY( Tool ${NAME} )
  endif()

ENDMACRO()
