###########################################################################
## $Id: CMakeLists.txt 5873 2019-02-07 14:33:51Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for BCI2000 and CMake

# Set this directory name
SET( DIR_NAME "${DIR_NAME}SignalProcessing/" )

# Recurse down into all project subdirectories
UTILS_MATCH_SUBDIR( BUILD_MODULES AR )
UTILS_MATCH_SUBDIR( BUILD_MODULES P3 )
UTILS_MATCH_SUBDIR( BUILD_MODULES FFT )
UTILS_MATCH_SUBDIR( BUILD_MODULES Matlab )
UTILS_MATCH_SUBDIR( BUILD_MODULES Spectral )
UTILS_MATCH_SUBDIR( BUILD_MODULES VisualizationDemos )
UTILS_MATCH_SUBDIR( BUILD_MODULES SignalSharingDemo)
ADD_SUBDIRECTORY( Dummy )
