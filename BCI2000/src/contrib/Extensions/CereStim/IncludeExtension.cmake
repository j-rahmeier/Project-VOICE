###########################################################################
## $Id: IncludeExtension.cmake 7465 2023-06-30 15:23:43Z mellinger $
## Authors: Alexander Belsten (belsten@neurotechcenter.org)


IF( NOT WIN32 )
  MESSAGE( "--- CereStim libraries not available for this platform. Will not build." )
  RETURN()
ENDIF()

 IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    # Set the project specific sources
    SET( BCI2000_APP_FILES
      ${BCI2000_APP_FILES}
      ${BCI2000_EXTENSION_DIR}/CereStim.cpp
      ${BCI2000_EXTENSION_DIR}/lib/x64/BStimAPIx64.dll
    )
    list( APPEND BCI2000_APP_LIBS 
			${CMAKE_CURRENT_LIST_DIR}/lib/x64/BStimAPIx64.lib
		)
    
  ELSE()
    # Set the project specific sources
    SET( BCI2000_APP_FILES
      ${BCI2000_APP_FILES}
      ${BCI2000_EXTENSION_DIR}/CereStim.cpp
      ${BCI2000_EXTENSION_DIR}/lib/x86/BStimAPIx86.dll
    )
    list( APPEND BCI2000_APP_LIBS 
			${CMAKE_CURRENT_LIST_DIR}/lib/x86/BStimAPIx86.lib
		)

  ENDIF()