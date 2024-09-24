###########################################################################
## $Id: IncludeExtension.cmake 7465 2023-06-30 15:23:43Z mellinger $
## Authors: griffin.milsap@gmail.com

# Add the StimBoxFilter to all application modules

if(WIN32)
SET( BCI2000_APP_FILES
   ${BCI2000_APP_FILES}
   ${BCI2000_EXTENSION_DIR}/StimBoxFilter.cpp
   ${BCI2000_EXTENSION_DIR}/lib/gSTIMbox.imports.cpp
)
else()
  message("gStimBoxFilter: Only available on Windows")
endif()
