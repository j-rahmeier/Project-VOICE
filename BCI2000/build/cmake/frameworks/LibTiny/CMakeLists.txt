###########################################################################
## $Id: CMakeLists.txt 7465 2023-06-30 15:23:43Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Defines a static library containing LibTiny utils.

UTILS_INCLUDE( frameworks/LibTiny/Files )

IF( WIN32 )
  SET( DEPLIBS ws2_32 winmm )
ENDIF() 
IF( NOT APPLE AND NOT WIN32 )
  SET( DEPLIBS rt pthread dl )
ENDIF()

add_subdirectory( StaticCrt )
add_subdirectory( DynamicCrt )
