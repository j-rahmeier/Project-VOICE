IF( WIN32 )

# Set the executable name
SET( EXECUTABLE_NAME BlackrockGemini )
INCLUDE_DIRECTORIES( include )
INCLUDE_DIRECTORIES( lib )
# Add the source module to the Makefile or compiler project file:
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  ${EXECUTABLE_NAME}
  BlackrockGeminiADC.cpp
  SUPPORT batch/*=>/batch
  SUPPORT parms/*=>/parms/fragments/amplifiers
)

IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  target_link_libraries( ${EXECUTABLE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/cbsdkx64.lib )
ELSE()
  target_link_libraries( ${EXECUTABLE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/lib/x86/cbsdk.lib )
ENDIF()

ENDIF( WIN32 )

