###########################################################################
## $Id: IncludeExtension.cmake 7732 2023-11-23 17:07:03Z mellinger $
## Authors: kaleb.goering@gmail.com

if(WIN32)

SET( BCI2000_SIGSRC_FILES
   ${BCI2000_SIGSRC_FILES}
   ${BCI2000_EXTENSION_DIR}/EyetrackerLoggerTobiiPro.cpp
   ${BCI2000_EXTENSION_DIR}/extlib/include/tobii_research.imports.cpp
   ${BCI2000_EXTENSION_DIR}/VideoRecorder.cpp
   ${BCI2000_EXTENSION_DIR}/FrameComposer.cpp
   ${PROJECT_SRC_DIR}/shared/utils/Win32/Window.cpp
   ${PROJECT_SRC_DIR}/shared/utils/Win32/EventLoop.cpp
   ${PROJECT_SRC_DIR}/shared/utils/IniFile.cpp
   ${PROJECT_SRC_DIR}/extlib/com/ComStrings.cpp
   ${PROJECT_SRC_DIR}/extlib/com/ComPtr.h
)

IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	SET ( BCI2000_SIGSRC_FILES
		${BCI2000_SIGSRC_FILES}
		${BCI2000_EXTENSION_DIR}/extlib/dylib/x64/tobii_research.dll 
	)
ELSE()
	SET ( BCI2000_SIGSRC_FILES
		${BCI2000_SIGSRC_FILES}
		${BCI2000_EXTENSION_DIR}/extlib/dylib/x86/tobii_research.dll 
	)
ENDIF()

	list( APPEND BCI2000_SIGSRC_LIBS 
		mf mfplat mfreadwrite mfuuid strmiids
	)

else()
  message("EyetrackerLoggerTobiiPro: Only available on Windows")
endif()
