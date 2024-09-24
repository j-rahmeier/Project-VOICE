###########################################################################
## $Id: CMakeLists.txt 5985 2019-07-01 15:46:09Z abelsten $
## Description: Build information for FieldTripBufferSource

# Set the executable name
SET( EXECUTABLE_NAME FieldTripBufferSource )

# Set the project specific sources
SET( FILES_PROJECT
  FieldTripBufferADC.cpp 
)

IF (CMAKE_SIZEOF_VOID_P EQUAL 8 AND WIN32 ) 
  IF( MSVC )
    list( APPEND SRC_PROJECT ${PROJECT_SRC_DIR}/extlib/fieldtrip/pthreads-win32/dll/x64/pthreadVC2.dll )
  ELSEIF( MINGW )
    list( APPEND SRC_PROJECT ${PROJECT_SRC_DIR}/extlib/fieldtrip/pthreads-win32/dll/x64/pthreadGC2.dll )
  ENDIF()
ELSEIF(NOT CMAKE_SIZEOF_VOID_P EQUAL 8 AND WIN32 )
  IF( MSVC )
    list( APPEND SRC_PROJECT ${PROJECT_SRC_DIR}/extlib/fieldtrip/pthreads-win32/dll/x86/pthreadVC2.dll )
  ELSEIF( MINGW )
    list( APPEND SRC_PROJECT ${PROJECT_SRC_DIR}/extlib/fieldtrip/pthreads-win32/dll/x86/pthreadGC2.dll )
  ENDIF()
ENDIF()

BCI2000_INCLUDE( "FIELDTRIP" )
IF( WIN32 )
  BCI2000_INCLUDE( "PTHREADS" )
ENDIF( WIN32 )

# Create the signal source module
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${FILES_PROJECT}"
)

