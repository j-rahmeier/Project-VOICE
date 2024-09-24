###########################################################################
## $Id: CMakeLists.txt 8120 2024-05-08 16:37:53Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for BCI2000Command

# Set the executable name
SET( EXECUTABLE_NAME BCI2000Command )

ADD_DEFINITIONS(
  -DEXECUTABLE_NAME=\"${EXECUTABLE_NAME}\"
  -DOPERATOR_NAME=\"Operator\"
)

# Set the project specific sources
SET( SRC_PROJECT
  BCI2000Command.cpp
  ${BCI2000_SRC_DIR}/shared/utils/Lib/SockStream.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Lib/SelfPipe.cpp
  ${BCI2000_SRC_DIR}/core/Operator/BCI2000Remote/BCI2000Remote.cpp  
  ${BCI2000_SRC_DIR}/core/Operator/BCI2000Remote/BCI2000Connection.cpp  
)

IF( WIN32 )
  SET( LIBS
    ${LIBS}
    ws2_32
    shlwapi
  )
ENDIF( WIN32 )

utils_set_appicon( prompt )
BCI2000_ADD_TARGET( EXECUTABLE ${EXECUTABLE_NAME} ${SRC_PROJECT} )
