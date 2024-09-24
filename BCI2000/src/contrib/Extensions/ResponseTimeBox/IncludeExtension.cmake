###########################################################################
## $Id: IncludeExtension.cmake 7465 2023-06-30 15:23:43Z mellinger $
## Authors: Ryan Gladwell (gladwell@neurotechcenter.org)


if(WIN32)

SET( BCI2000_SIGSRC_FILES
   ${BCI2000_SIGSRC_FILES}
   ${BCI2000_EXTENSION_DIR}/ResponseTimeBox.cpp
   ${BCI2000_EXTENSION_DIR}/SerialPort.cpp
)

else()
  message("ResponseTimeBox: Only available on Windows")
endif()
