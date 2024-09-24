###########################################################################
## $Id: ALGLIB.cmake 7465 2023-06-30 15:23:43Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Sets up CMAKE variables for including alglib in a project
## SETS:
##       SRC_EXTLIB - Required source files for the alglib
##       HDR_EXTLIB - Required header files for the alglib
##       INC_EXTLIB - Include directory for the alglib

# Define the source files
SET( SRC_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/alglib/ap.cpp
  ${PROJECT_SRC_DIR}/extlib/alglib/blas.cpp
  ${PROJECT_SRC_DIR}/extlib/alglib/qr.cpp
  ${PROJECT_SRC_DIR}/extlib/alglib/reflections.cpp
  ${PROJECT_SRC_DIR}/extlib/alglib/remmean.cpp
)

# Define the headers
SET( HDR_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/alglib/ap.h
  ${PROJECT_SRC_DIR}/extlib/alglib/apvt.h
  ${PROJECT_SRC_DIR}/extlib/alglib/blas.h
  ${PROJECT_SRC_DIR}/extlib/alglib/qr.h
  ${PROJECT_SRC_DIR}/extlib/alglib/reflections.h
  ${PROJECT_SRC_DIR}/extlib/alglib/remmean.h
)

# Define the include directory
SET( INC_EXTLIB ${PROJECT_SRC_DIR}/extlib/alglib )

# Set success
SET( EXTLIB_OK TRUE )
