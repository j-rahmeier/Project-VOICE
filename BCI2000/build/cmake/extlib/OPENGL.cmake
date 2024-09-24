###########################################################################
## $Id: OPENGL.cmake 7465 2023-06-30 15:23:43Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Sets up CMAKE variables for including OpenGL in a project

SET( INC_EXTLIB "${PROJECT_SRC_DIR}/extlib/openGL" )
SET( LIBS_EXTLIB )

IF( WIN32 )
  SET( LIBS_EXTLIB
      ${LIBS_EXTLIB}
      d3d9
      opengl32
  )
elseif(APPLE)
  set( LIBS_EXTLIB "-framework OpenGL" )
ELSE()
  SET( LIBS_EXTLIB -lGL -lGLU )
ENDIF()

SET( EXTLIB_OK TRUE )
