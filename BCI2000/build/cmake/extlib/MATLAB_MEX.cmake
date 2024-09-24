###########################################################################
## $Id: MATLAB_MEX.cmake 7667 2023-10-17 10:47:52Z mellinger $
## Authors: juergen.mellinger@neurotechcenter.org
## Description: Sets up CMAKE variables for including the EXTLIB Matlab libs

set( matlabdir_ ${PROJECT_SRC_DIR}/extlib/matlab )
set( SRC_EXTLIB
    ${matlabdir_}/imports/libmex.imports.cpp
    ${matlabdir_}/imports/libmx.imports.cpp
)
set( HDR_EXTLIB
    ${matlabdir_}/mex.h
    ${matlabdir_}/matrix.h
)
# use #include "matlab/libmex.imports.h" to avoid name clashes
set( INC_EXTLIB ${PROJECT_SRC_DIR}/extlib )
set(DEF_EXTLIB -DMATLAB_MEX_FILE;-DMW_NEEDS_VERSION_H)
set( EXTLIB_OK TRUE )

