###########################################################################
## $Id: LIBSVM.cmake 7465 2023-06-30 15:23:43Z mellinger $
## Authors: mellinger@neurotechcenter.org
## Description: Sets up CMAKE variables for including libschrift in a project
## SETS:
##       SRC_EXTLIB - Required source files for the libschrift
##       HDR_EXTLIB - Required header files for the libschrift
##       INC_EXTLIB - Include directory for the libschrift

set(LIBSCHRIFT_SRC_FILE
  ${PROJECT_SRC_DIR}/extlib/libschrift/schrift.c
)
set_source_files_properties(${LIBSCHRIFT_SRC_FILE} PROPERTIES SKIP_PRECOMPILE_HEADERS TRUE)

set(SRC_EXTLIB ${LIBSCHRIFT_SRC_FILE})
set(INC_EXTLIB ${PROJECT_SRC_DIR}/extlib/libschrift)

set(EXTLIB_OK TRUE)
