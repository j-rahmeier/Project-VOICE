###########################################################################
## $Id: LIBSNDFILE.cmake 8167 2024-05-28 14:54:57Z mellinger $
## Authors: mellinger@neurotechcenter.org
## Description: Sets up CMAKE variables for including libsndfile in a project

if(WIN32)

  set( INC_EXTLIB "${PROJECT_SRC_DIR}/extlib/libsndfile/include" )
  file( GLOB importfiles_ "${INC_EXTLIB}/../imports/*" )
  list( APPEND SRC_EXTLIB ${importfiles_} )
  if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set( libdir_ "${INC_EXTLIB}/../win32-amd64/dynamic" )
  else()
    set( libdir_ "${INC_EXTLIB}/../win32-x86/dynamic" )
  endif()
  file( GLOB dlls_ "${libdir_}/*.dll" )
  foreach( _R0 ${dlls_} )
    list( APPEND SRC_EXTLIB COMPANION ${_R0} )
  endforeach()
  set( EXTLIB_OK TRUE )

else()

  set(lib_sf)
  find_library(lib_sf sndfile NO_CACHE)
  set(inc_sf)
  find_path(inc_sf sndfile.h NO_CACHE)
    
  if(NOT lib_sf)
    set(inc_sf "${PROJECT_SRC_DIR}/extlib/homebrew/include/")
    set(lib_sf "${PROJECT_SRC_DIR}/extlib/homebrew/lib/libsndfile.a")
    message("Portaudio could not be found by CMake. Using the portable homebrew instead")
  endif()
  
  set(LIBS_EXTLIB ${lib_sf})
  set(INC_EXTLIB ${inc_sf})
  
  set(EXTLIB_OK true)

endif()
