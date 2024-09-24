###########################################################################
## $Id: LODEPNG.cmake 7465 2023-06-30 15:23:43Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Sets up CMAKE variables for including the lodepng lib

SET( EXTLIB_OK TRUE )
SET( SRC_EXTLIB ${PROJECT_SRC_DIR}/extlib/lodepng/lodepng.cpp )
SET( HDR_EXTLIB ${PROJECT_SRC_DIR}/extlib/lodepng/lodepng.h )
SET( INC_EXTLIB ${PROJECT_SRC_DIR}/extlib/lodepng )
