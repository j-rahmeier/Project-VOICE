###########################################################################
## $Id: SWLDA.cmake 7465 2023-06-30 15:23:43Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Sets up CMAKE variables for including swlda in a project
## SETS:
##       SRC_EXTLIB - Required source files for the swlda
##       HDR_EXTLIB - Required header files for the swlda
##       INC_EXTLIB - Include directory for the swlda

# Define the source files
SET( SRC_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/alglib/ap.cpp
  ${PROJECT_SRC_DIR}/extlib/alglib/blas.cpp
  ${PROJECT_SRC_DIR}/extlib/alglib/qr.cpp
  ${PROJECT_SRC_DIR}/extlib/alglib/reflections.cpp
  ${PROJECT_SRC_DIR}/extlib/alglib/remmean.cpp
  ${PROJECT_SRC_DIR}/extlib/swlda/nrerror.cpp
  ${PROJECT_SRC_DIR}/extlib/swlda/in_out_variable.cpp
  ${PROJECT_SRC_DIR}/extlib/swlda/solve_linear_equation.cpp
  ${PROJECT_SRC_DIR}/extlib/swlda/stepcalc.cpp
  ${PROJECT_SRC_DIR}/extlib/swlda/stepnext.cpp
  ${PROJECT_SRC_DIR}/extlib/swlda/stepwisefit.cpp
  ${PROJECT_SRC_DIR}/extlib/swlda/tcdf.cpp
  ${PROJECT_SRC_DIR}/extlib/swlda/betacf.cpp
  ${PROJECT_SRC_DIR}/extlib/swlda/betai.cpp
  ${PROJECT_SRC_DIR}/extlib/swlda/gammln.cpp
)

# Define the headers
SET( HDR_EXTLIB
)

# Define the include directory
SET( INC_EXTLIB ${PROJECT_SRC_DIR}/extlib/alglib )
SET( INC_EXTLIB ${PROJECT_SRC_DIR}/extlib/swlda )

# Set success
SET( EXTLIB_OK TRUE )
