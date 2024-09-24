###########################################################################
## $Id: CMakeLists.txt 8120 2024-05-08 16:37:53Z mellinger $
## Authors: juergen.mellinger@neurotechcenter.org
## Description: Build information for BCI2000RemoteDemo

if(BUILD_DEMOS)

SET( NAME BCI2000RemoteDemo )
set( DIR_NAME Demos/BCI2000Remote )

SET( SRC_PROJECT
  BCI2000RemoteDemo.cpp
  ${PROJECT_SRC_DIR}/core/Operator/BCI2000Remote/BCI2000Connection.cpp
  ${PROJECT_SRC_DIR}/core/Operator/BCI2000Remote/BCI2000Remote.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Lib/SockStream.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Lib/SelfPipe.cpp
)

SET( LIBS "" )
IF( WIN32 )
  SET( LIBS
    ${LIBS}
    ws2_32
  )
ENDIF()

utils_set_appicon( prompt )
set_crt_link_type( STATIC )
BCI2000_ADD_TARGET( CONSOLEAPP ${NAME} ${SRC_PROJECT} )
SET_OUTPUT_DIRECTORY( "${PROJECT_BUILD_ROOT}/prog" ${NAME} )

endif()
