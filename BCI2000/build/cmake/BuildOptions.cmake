#################################################
## $Id: BuildOptions.cmake 7837 2024-02-13 16:59:06Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: User-configurable build options

include(CMakeDependentOption)

# General build options
if( WIN32 )
  OPTION( USE_APPLICATION_BUNDLES "Set to ON to bundle applications into single files together with their dependencies" OFF )
else()
  set( USE_APPLICATION_BUNDLES OFF )
endif()

if( APPLE )
  option( USE_MACOS_APPLICATION_BUNDLES "Create macOS app bundles" ON )
  if( USE_MACOS_APPLICATION_BUNDLES )
    add_definitions( -DUSE_MACOS_APPLICATION_BUNDLES )
  endif()
endif()

OPTION( USE_OPENMP "Set to OFF to disable OpenMP-based multithreading" ON )

OPTION( USE_ASSERTS_IN_RELEASE_BUILDS "Enable Assert() and SuggestDebugging() in release builds" )
IF( USE_ASSERTS_IN_RELEASE_BUILDS )
  ADD_DEFINITIONS( -DTINY_ASSERT )
ENDIF()

SET( BUILD_OPTIONS
  ${BUILD_OPTIONS}
  USE_OPENMP
  USE_PRECOMPILED_HEADERS
  USE_ASSERTS_IN_RELEASE_BUILDS
)  

IF( CMAKE_SYSTEM_PROCESSOR MATCHES ".*86" AND CMAKE_SIZEOF_VOID_P EQUAL 4 )
  OPTION( USE_SSE2 "Set to OFF when building for older processors" ON )
  list( APPEND BUILD_OPTIONS USE_SSE2 )
ELSE()
  SET( USE_SSE2 OFF )
ENDIF()

# Whether to include certain projects
OPTION( BUILD_DEMOS "Build demo projects" OFF )

option( BUILD_TESTS "Build ${PROJECT_NAME} library tests" OFF )
mark_as_advanced( BUILD_TESTS )
if(BUILD_TESTS)
  enable_testing()
endif()

option( BUILD_CONTRIB "Build all modules from the contrib section" OFF )
option( BUILD_ALL_SOURCEMODULES "Build all source modules" OFF )
option( BUILD_BCPY2000 "Build BCPy2000 modules" OFF )
IF( EXISTS ${PROJECT_SRC_DIR}/private )
  IF( $ENV{BCI2000_NO_PRIVATE} )
    SET( onoff_ OFF )
  ELSE()
    SET( onoff_ ON )
  ENDIF()
  option( BUILD_PRIVATE "Build contents of src/private directory" ${onoff_} )
  option( BUILD_PRIVATE_SOURCEMODULES "Build contents of src/private/SignalSource directory" OFF )
  option( BUILD_PRIVATE_EXTENSIONS "Include extensions from src/private and subdirectories directory" OFF )
  option( BUILD_INCLUDE_NDA "Include modules and extensions that require an NDA (non-disclosure agreement) to use" ON )
ENDIF()
IF( EXISTS ${PROJECT_SRC_DIR}/custom )
  option( BUILD_CUSTOM "Build contents of src/custom directory" ON )
ENDIF()

if( NOT BUILD_MODULES MATCHES "fixed;.*" )
  SET( mod_ "core" )
  IF( BUILD_CONTRIB )
    LIST( APPEND mod_ "contrib" )
  ENDIF()
  IF( BUILD_ALL_SOURCEMODULES )
    LIST( APPEND mod_ "SignalSource" )
  ENDIF()
  IF( BUILD_BCPY2000 )
    LIST( APPEND mod_ "BCPy2000" )
  ENDIF()
endif()
SET( BUILD_MODULES ${mod_} CACHE STRING "List of regular expressions matching subdirectory names" FORCE )
mark_as_advanced( BUILD_MODULES )

# MSVC specific
IF( MSVC )
  OPTION( BUILD_MFC "Build modules that use MFC" OFF )
ELSE()
  SET( BUILD_MFC OFF )
ENDIF()

SET( folders_ ON )
option( BUILD_USE_SOLUTION_FOLDERS "Enable target group folders" ${folders_} )
mark_as_advanced( BUILD_USE_SOLUTION_FOLDERS )

