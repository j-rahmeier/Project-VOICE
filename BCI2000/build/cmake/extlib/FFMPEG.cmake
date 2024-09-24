###########################################################################
## $Id: FFMPEG.cmake 8208 2024-06-20 16:45:34Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Sets up CMAKE variables for including ffmpeg-LibAV in a project

if(WIN32)

  set( INC_EXTLIB "${PROJECT_SRC_DIR}/extlib/ffmpeg/include" )
  file( GLOB libavfiles_ "${INC_EXTLIB}/../imports/*" )
  list( APPEND SRC_EXTLIB ${libavfiles_} )
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

  set(INC_EXTLIB)
  find_path(INC_EXTLIB avcodec.h PATH_SUFFIXES libavcodec NO_CACHE)
  if (NOT INC_EXTLIB)
    set(INC_EXTLIB ${PROJECT_SRC_DIR}/extlib/homebrew/include)
    message( "FFMPEG could not be found by CMake -- Using the portable homebrew location instead. Please run install_dependencies, if you have not already." )
  else()
    cmake_path(GET INC_EXTLIB PARENT_PATH INC_EXTLIB)
    cmake_path(GET INC_EXTLIB PARENT_PATH INC_EXTLIB)
  endif()
  
  # We use full paths to libraries because LIBDIRS_EXTLIB will not be
  # propagated from libraries to consuming executables
  set (libnames avdevice avcodec avutil avformat swresample swscale)
  foreach (libname ${libnames})
    set(lib)
    find_library (lib ${libname} PATHS NO_CACHE)
    if (NOT lib)
      set (lib ${PROJECT_SRC_DIR}/extlib/homebrew/lib/${libname})
    endif()
    list (APPEND LIBS_EXTLIB ${lib})
  endforeach()
  
  if (NOT found)
  endif()

  set( EXTLIB_OK true )

endif()

