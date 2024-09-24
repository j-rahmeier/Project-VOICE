###########################################################################
## $Id: CMakeLists.txt 8107 2024-05-03 20:21:30Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for testing and build utilities

CONFIGURE_FILE( buildenv.h.in ${BUILDUTILS_DIR}/buildenv.h )

SET( DIR_NAME "${DIR_NAME}Build/BuildUtils/" )
SET( USE_PRECOMPILED_HEADERS OFF )

ADD_SUBDIRECTORY( BootstrapCustomProjects )
ADD_SUBDIRECTORY( DylibTool.dir )
ADD_SUBDIRECTORY( SchedulingTool.dir )
SET_OUTPUT_DIRECTORY( ${BUILDUTILS_DIR} )
if( USE_APPLICATION_BUNDLES )
  ADD_SUBDIRECTORY( AppBundler.dir )
else()
  add_subdirectory( AppCopier.dir )
endif()
configure_file( deploy_app.bat "${BUILDUTILS_DIR}/deploy_app.bat" @ONLY )

SET_OUTPUT_DIRECTORY( ${BUILDUTILS_DIR}/tests )
set_crt_link_type( STATIC )
BCI2000_ADD_TARGET( CONSOLEAPP NOBUNDLE NOVERSION bci_datadiff bci_datadiff.cpp )

SET_OUTPUT_DIRECTORY( ${BUILDUTILS_DIR} )
# extract_registry
INCLUDE_DIRECTORIES(
  ${PROJECT_SRC_DIR}/shared/config
)
BCI2000_ADD_TARGET( CONSOLEAPP NOBUNDLE NOVERSION extract_registry extract_registry.cpp )

# create_resource
UTILS_INCLUDE( frameworks/LibTiny )
BCI2000_ADD_TARGET( CONSOLEAPP NOBUNDLE NOVERSION create_resource
  create_resource.cpp
  ${PROJECT_SRC_DIR}/shared/types/Blob.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Resource.h
)

# copy_if_newer (not available through CMake)
bci2000_add_target( CONSOLEAPP NOBUNDLE NOVERSION copy_if_newer
  copy_if_newer.cpp
)

# fix_dat_headerlen
bci2000_add_target( CONSOLEAPP NOBUNDLE NOVERSION fix_dat_headerlen
  fix_dat_headerlen.cpp
)

# list_dependencies
if( WIN32 )
  BCI2000_ADD_TARGET( CONSOLEAPP NOBUNDLE NOVERSION list_dependencies
    list_dependencies.cpp
    ${BUILDUTILS_DIR}/buildenv.h
  )
endif()

if( WIN32 )
  bci2000_add_target( CONSOLEAPP NOBUNDLE NOVERSION make_win32ico
    make_win32ico.cpp
    ${PROJECT_SRC_DIR}/shared/utils/BinaryDataIO/Win32BmpFormat.cpp
  )
  utils_include( frameworks/LibTiny )
  bci2000_add_target( CONSOLEAPP NOBUNDLE NOVERSION png2bmp
    png2bmp.cpp
    ${PROJECT_SRC_DIR}/shared/utils/BinaryDataIO/Win32BmpFormat.cpp
    ${PROJECT_SRC_DIR}/extlib/lodepng/lodepng.cpp
  )
endif()
bci2000_add_target( CONSOLEAPP NOBUNDLE NOVERSION combine_png
  combine_png.cpp
  ${PROJECT_SRC_DIR}/extlib/lodepng/lodepng.cpp
)
utils_include( frameworks/Core )
bci2000_add_target( CONSOLEAPP NOBUNDLE NOVERSION transform_png
  transform_png.cpp
  ${PROJECT_SRC_DIR}/shared/bcistream/BCIStream_tool.cpp
  ${PROJECT_SRC_DIR}/extlib/lodepng/lodepng.cpp
)

if (WIN32)
  bci2000_include(com)
  bci2000_add_target(CONSOLEAPP NOBUNDLE NOVERSION make_win32_shortcut
    make_win32_shortcut.cpp
  )
elseif(APPLE)
  bci2000_add_target(CONSOLEAPP NOBUNDLE NOVERSION make_macOS_alias
    make_macOS_alias.mm
  )
  target_link_libraries(make_macOS_alias "-framework Foundation")
endif()
