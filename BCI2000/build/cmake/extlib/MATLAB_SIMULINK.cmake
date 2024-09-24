###########################################################################
## $Id: MATLAB_SIMULINK.cmake 7668 2023-10-17 10:49:34Z mellinger $
## Authors: mellinger@neurotechcenter.org
## Description: Sets up CMAKE variables for including auxiliary sources to
##   build a Simulink MEX-file. Requires an actual Matlab installation to
##   be present.

set(Matlab_ROOT "" CACHE PATH
  "Matlab root path to a Matlab installation used for compiling Simulink S-functions."
)
find_package(Matlab)
if (NOT Matlab_FOUND)
  message(
     "Cannot build Simulink Mex files without a Matlab installation.\n"
     "Please install a full Matlab version (not MCR)."
  )
  set(EXTLIB_OK FALSE)
  return()
endif()


set(INC_EXTLIB ${Matlab_INCLUDE_DIRS})
set(LIBS_EXTLIB
  ${Matlab_MEX_LIBRARY}
  ${Matlab_MX_LIBRARY}
)

find_file(SIMULINK_MEX_SOURCE
  simulink.c
  PATHS ${Matlab_ROOT_DIR}/simulink/include
  NO_CACHE
)
if (NOT SIMULINK_MEX_SOURCE)
  message("simulink.c not found below ${Matlab_ROOT_DIR}.\n"
          "CMake appears to have found a Matlab Runtime (MCR) installation.\n"
          "To fix this, set Matlab_ROOT to the directory of a full Matlab installation."
  )
  set(EXTLIB_OK FALSE)
  return()
endif()
message(STATUS "Using simulink.c at ${SIMULINK_MEX_SOURCE}.")

get_filename_component(inc_simulink_ ${SIMULINK_MEX_SOURCE} DIRECTORY)
list(APPEND INC_EXTLIB ${inc_simulink_})
set(DEF_EXTLIB -DMATLAB_MEX_FILE;-DMW_NEEDS_VERSION_H)
set(EXTLIB_OK TRUE)

