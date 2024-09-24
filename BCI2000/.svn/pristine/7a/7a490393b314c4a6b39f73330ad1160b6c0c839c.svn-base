###########################################################################
## $Id: LIBSNDFILE.cmake 7647 2023-10-06 14:58:50Z mellinger $
## Authors: mellinger@neurotechcenter.org
## Description: Sets up CMAKE variables for including EEGO in a project.

set( INC_EXTLIB "${PROJECT_SRC_DIR}/extlib/eemagine/sdk" )

if(WIN32)

  if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set( libdir_ "${INC_EXTLIB}/../lib/win32-amd64" )
  else()
    set( libdir_ "${INC_EXTLIB}/../lib/win32-x86" )
  endif()

  file( GLOB dlls "${libdir_}/*.dll" )
  if( NOT dlls )
    message("No EEGO DLL available for this platform. The source module will build but not run.")
  endif()
  foreach( dll ${dlls} )
    list( APPEND SRC_EXTLIB COMPANION ${dll} )
  endforeach()

  set( DEF_EXTLIB
    -DEEGO_SDK_BIND_DYNAMIC
  )
  list( APPEND SRC_EXTLIB "${INC_EXTLIB}/wrapper.cc" )
  list( APPEND INC_EXTLIB "${PROJECT_SRC_DIR}/extlib" )

  set( EXTLIB_OK TRUE )

else()

  message("EEGO sdk not available for this platform, will not build")
  set( EXTLIB_OK FALSE )

endif()

