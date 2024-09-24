###########################################################################
## $Id: IncludeExtension.cmake 7465 2023-06-30 15:23:43Z mellinger $
## Authors: Alexander Belsten (belsten@neurotechcenter.org)

IF( WIN32 ) 
	SET( BCI2000_SIGSRC_FILES
	   ${BCI2000_SIGSRC_FILES}
	   ${BCI2000_EXTENSION_DIR}/XsensMTwLogger.cpp
	)

	IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
		INCLUDE_DIRECTORIES(
			${BCI2000_EXTENSION_DIR}/include
		)
	
		SET ( BCI2000_SIGSRC_FILES
			${BCI2000_SIGSRC_FILES}
			${BCI2000_EXTENSION_DIR}/extlib/x64/lib/xkf3hm_filter64.dll
			${BCI2000_EXTENSION_DIR}/extlib/x64/lib/xkf3i_filter64.dll
			${BCI2000_EXTENSION_DIR}/extlib/x64/lib/xkf700_filter64.dll
			${BCI2000_EXTENSION_DIR}/extlib/x64/lib/xsensdeviceapi64.dll
			${BCI2000_EXTENSION_DIR}/extlib/x64/lib/xstypes64.dll
		)
		
		list( APPEND BCI2000_SIGSRC_LIBS 
			${BCI2000_EXTENSION_DIR}/extlib/x64/lib/xsensdeviceapi64.lib
			${BCI2000_EXTENSION_DIR}/extlib/x64/lib/xstypes64.lib
		)
		
	ELSE()
		INCLUDE_DIRECTORIES(
			${BCI2000_EXTENSION_DIR}/include
		)
		
		SET ( BCI2000_SIGSRC_FILES
			${BCI2000_SIGSRC_FILES}
			${BCI2000_EXTENSION_DIR}/extlib/Win32/lib/xkf3hm_filter32.dll
			${BCI2000_EXTENSION_DIR}/extlib/Win32/lib/xkf3i_filter32.dll
			${BCI2000_EXTENSION_DIR}/extlib/Win32/lib/xkf700_filter32.dll
			${BCI2000_EXTENSION_DIR}/extlib/Win32/lib/xsensdeviceapi32.dll
			${BCI2000_EXTENSION_DIR}/extlib/Win32/lib/xstypes32.dll
		)
		
		list( APPEND BCI2000_SIGSRC_LIBS 
			${BCI2000_EXTENSION_DIR}/extlib/Win32/lib/xsensdeviceapi32.lib
			${BCI2000_EXTENSION_DIR}/extlib/Win32/lib/xstypes32.lib
		)
	ENDIF()
ELSE()
	MESSAGE( "--- Xsens libraries not available for this platform. Will not build XsensMTwLogger." )
ENDIF()
