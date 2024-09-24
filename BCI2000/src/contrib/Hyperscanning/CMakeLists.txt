###########################################################################
## $Id: CMakeLists.txt 7471 2023-07-06 19:24:43Z mmarcus $
## Description: Build information for BCI2000 and CMake

# Set directory name in IDE
SET( DIR_NAME "${DIR_NAME}Hyperscanning/" )

ADD_SUBDIRECTORY( HyperscanningTest )
ADD_SUBDIRECTORY( HyperscanningApplicationBase )
