###########################################################################
## $Id: IncludeExtension.cmake 7647 2023-10-06 14:58:50Z mellinger $
## Authors: jezhill@gmail.com

IF( NOT WIN32 )
  # Unconditionally including Windows.h
  MESSAGE( "**** SerialInterface failed: Not supported on this platform" )
  RETURN()
ENDIF()

SET( BCI2000_SIGSRC_FILES
   ${BCI2000_SIGSRC_FILES}
   ${BCI2000_EXTENSION_DIR}/SerialInterface.h
   ${BCI2000_EXTENSION_DIR}/SerialInterface.cpp
   ${BCI2000_EXTENSION_DIR}/SerialConnection.h
   ${BCI2000_EXTENSION_DIR}/SerialConnection.cpp
   ${BCI2000_EXTENSION_DIR}/SerialInterfaceDemo.bat
)
