###########################################################################
## $Id: CMakeLists.txt 7605 2023-09-21 18:16:09Z pbrunner $
## Authors: griffin.milsap@gmail.com
## Description: Build information for BCI2000 and CMake

# Set this directory name
SET( DIR_NAME "${DIR_NAME}Application/" )

# Recurse down into all project subdirectories
IF( BUILD_MFC )
  UTILS_MATCH_SUBDIR( BUILD_MODULES MFCDemo )
ENDIF()
UTILS_MATCH_SUBDIR( BUILD_MODULES ERPSpeller )
UTILS_MATCH_SUBDIR( BUILD_MODULES ANTTask  )
UTILS_MATCH_SUBDIR( BUILD_MODULES wcst)
UTILS_MATCH_SUBDIR( BUILD_MODULES ForagingTask)

