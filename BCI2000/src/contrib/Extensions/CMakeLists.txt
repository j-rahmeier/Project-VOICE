###########################################################################
## $Id: CMakeLists.txt 7498 2023-07-28 20:57:02Z jhill $
## Authors: griffin.milsap@gmail.com
## Description: Build information for BCI2000 and CMake

# Only a small minority of extensions will need this (for standalone helper utilities,
# e.g. MSAutoConnectWiimote) so most will not have a CMakeLists.txt . However they
# will all have an IncludeExtension.cmake, which is detected in Extensions.cmake

SET( here ${CMAKE_CURRENT_LIST_DIR} )
FILE( GLOB subdirs RELATIVE "${here}" "${here}/*" )
FOREACH( dir ${subdirs} )
	IF( EXISTS ${here}/${dir}/CMakeLists.txt )
		ADD_SUBDIRECTORY( ${dir} )
	ENDIF()
ENDFOREACH()
