###########################################################################
## $Id: CMakeLists.txt 6171 2020-12-31 13:20:16Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for BCI2000 and CMake

# Set this directory name
SET( DIR_NAME "${DIR_NAME}SignalSource/" )

# Recurse down into all project subdirectories
ADD_SUBDIRECTORY( SignalGenerator )
ADD_SUBDIRECTORY( SoundcardSource )
UTILS_MATCH_SUBDIR( BUILD_MODULES g.USBamp )
UTILS_MATCH_SUBDIR( BUILD_MODULES g.MOBIlab )
UTILS_MATCH_SUBDIR( BUILD_MODULES g.MOBIlabPlus )
UTILS_MATCH_SUBDIR( BUILD_MODULES g.HIampSource )
UTILS_MATCH_SUBDIR( BUILD_MODULES g.NautilusSource )
UTILS_MATCH_SUBDIR( BUILD_MODULES g.Unicorn )