###########################################################################
## $Id: LIBSVM.cmake 7465 2023-06-30 15:23:43Z mellinger $
## Authors: mellinger@neurotechcenter.org
## Description: Sets up CMAKE variables for including libsvm in a project
## SETS:
##       SRC_EXTLIB - Required source files for the libsvm
##       HDR_EXTLIB - Required header files for the libsvm
##       INC_EXTLIB - Include directory for the libsvm

# Define the source files
SET( SRC_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/libsvm/libsvm-3.24/svm.cpp
)

# Define the headers
SET( HDR_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/libsvm/libsvm-3.24/svm.cpp
)

# Define the include directory
SET( INC_EXTLIB ${PROJECT_SRC_DIR}/extlib/libsvm/libsvm-3.24 )

# Set success
SET( EXTLIB_OK TRUE )
