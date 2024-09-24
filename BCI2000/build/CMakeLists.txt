###########################################################################
## $Id: CMakeLists.txt 7837 2024-02-13 16:59:06Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for BCI2000 and CMake

cmake_minimum_required( VERSION 3.5 )
cmake_policy( VERSION 3.5 )
set( CMAKE_INCLUDE_CURRENT_DIR ON )

project( BCI2000 )
set( LATEST_RELEASE "3.6" )
string( TIMESTAMP PROJECT_TIMESPAN "2000-%Y" UTC )

set(PROJECT_SOURCE_ROOT "${CMAKE_SOURCE_DIR}/..")
cmake_path(ABSOLUTE_PATH PROJECT_SOURCE_ROOT NORMALIZE)
set( PROJECT_SRC_DIR "${PROJECT_SOURCE_ROOT}/src" )
get_filename_component(REAL_BINARY_DIR ${CMAKE_BINARY_DIR} REALPATH)
get_filename_component(REAL_SOURCE_DIR ${CMAKE_SOURCE_DIR} REALPATH)
if( REAL_BINARY_DIR STREQUAL REAL_SOURCE_DIR )
  # in-source build
  message(STATUS "In-source build detected")
  set( PROJECT_BUILD_ROOT ${PROJECT_SOURCE_ROOT} )
else()
  # out-of-source build
  message(STATUS "Out-of-source build detected")
  get_filename_component( PROJECT_BUILD_ROOT "${CMAKE_BINARY_DIR}/${PROJECT_NAME}" ABSOLUTE )
  file( COPY Configure.sh.cmd DESTINATION . )
endif()
set( PROJECT_ICON_TEMPLATE "${PROJECT_SRC_DIR}/shared/gui/images/brain-outline" CACHE FILEPATH "" )
set( BUILDUTILS_DIR "${CMAKE_BINARY_DIR}/buildutils" )
set( PROJECT_BUILD_DIR ${CMAKE_BINARY_DIR}/CMakeFiles )
set( PROJECT_OUTPUT_DIR "${PROJECT_BUILD_ROOT}/prog" )
configure_file( BuildEnv.sh.cmd.in BuildEnv.sh.cmd @ONLY )

INCLUDE( ${PROJECT_SOURCE_ROOT}/build/cmake/Utils.cmake NO_POLICY_SCOPE )
UTILS_INCLUDE( BuildOptions )
UTILS_INCLUDE( SetupBCI2000Macros )
UTILS_INCLUDE( BuildConfigurations )
set_crt_link_type( DYNAMIC )

SET( BCI2000_SRC_DIR "${PROJECT_SRC_DIR}" )

# Tell the user what we're about to setup
MESSAGE( STATUS "Configuring project ${PROJECT_NAME} Version ${PROJECT_VERSION}" )

utils_scan_for_files(globalLists ${PROJECT_SRC_DIR} ".*.CMakeLists.txt")
utils_sort_by_filename(globalLists ${globalLists})
foreach(globalList ${globalLists})
  include(${globalList})
endforeach()

INCLUDE( "${PROJECT_SRC_DIR}/contrib/Extensions/Extensions.cmake" )
IF( EXISTS ${PROJECT_SRC_DIR}/private AND (BUILD_PRIVATE OR BUILD_PRIVATE_EXTENSIONS) )
  INCLUDE( "${PROJECT_SRC_DIR}/private/PrivateConfig.cmake" )
ENDIF()
IF( EXISTS "${PROJECT_SRC_DIR}/custom/CustomConfig.cmake" AND BUILD_CUSTOM )
  INCLUDE( "${PROJECT_SRC_DIR}/custom/CustomConfig.cmake" )
ENDIF()
ADD_SUBDIRECTORY( ${PROJECT_SOURCE_ROOT}/build/cmake/frameworks "${PROJECT_BUILD_DIR}/frameworks" )
ADD_SUBDIRECTORY( ${PROJECT_SOURCE_ROOT}/build/cmake/extlib "${PROJECT_BUILD_DIR}/extlib" )
ADD_SUBDIRECTORY( ${PROJECT_SRC_DIR}/shared/utils/Expression/test "${PROJECT_BUILD_DIR}/shared/Expression" )
ADD_SUBDIRECTORY( ${PROJECT_SRC_DIR}/shared/test "${PROJECT_BUILD_DIR}/shared" )

# Recurse down into all project subdirectories

# src/private : stuff we use that is not released yet
IF( EXISTS ${PROJECT_SRC_DIR}/private AND BUILD_PRIVATE )
  ADD_SUBDIRECTORY( ${PROJECT_SRC_DIR}/private ${PROJECT_BUILD_DIR}/private )
ELSEIF( EXISTS ${PROJECT_SRC_DIR}/private/SignalSource AND BUILD_PRIVATE_SOURCEMODULES )
  ADD_SUBDIRECTORY( ${PROJECT_SRC_DIR}/private/SignalSource ${PROJECT_BUILD_DIR}/private/SignalSource )
ENDIF()

# the standard distro: build/buildutils, src/core and src/contrib
ADD_SUBDIRECTORY( buildutils "${PROJECT_BUILD_DIR}/buildutils" )
ADD_SUBDIRECTORY( ${PROJECT_SRC_DIR}/core "${PROJECT_BUILD_DIR}/core" )
ADD_SUBDIRECTORY( ${PROJECT_SRC_DIR}/contrib "${PROJECT_BUILD_DIR}/contrib" )

# src/custom : a convenient place for users' own development without causing svn chaos
IF( EXISTS ${PROJECT_SRC_DIR}/custom/CMakeLists.txt AND BUILD_CUSTOM )
  SET( saved_ ${DIR_NAME} )
  SET( DIR_NAME "${DIR_NAME}Custom/" )
  ADD_SUBDIRECTORY( ${PROJECT_SRC_DIR}/custom "${PROJECT_BUILD_DIR}/custom" )
  SET( DIR_NAME ${saved_} )
ENDIF()

MARK_AS_ADVANCED( CMAKE_INSTALL_PREFIX )
