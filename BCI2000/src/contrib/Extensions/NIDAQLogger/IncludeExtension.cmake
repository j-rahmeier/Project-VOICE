
if(WIN32)

SET( BCI2000_SIGSRC_FILES
   ${BCI2000_SIGSRC_FILES}
   ${BCI2000_EXTENSION_DIR}/NIDAQLogger.cpp
   ${BCI2000_EXTENSION_DIR}/NIDAQLogger.h
   ${BCI2000_EXTENSION_DIR}/include/NIDAQmx.h
   ${BCI2000_EXTENSION_DIR}/include/NIDAQmx.imports.cpp
   ${BCI2000_EXTENSION_DIR}/include/NIDAQmx.imports.h
   


)

  SET( BCI2000_SIGSRC_LIBS
      ${BCI2000_SIGSRC_LIBS}
      ${BCI2000_EXTENSION_DIR}/lib/msvc/NIDAQmx.lib
  )


else()
  message("NIDAQLogger: Only available on Windows")
endif()
