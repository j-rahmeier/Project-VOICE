###########################################################################
## $Id: ToolsSimulinkMexfileMacro.cmake 7668 2023-10-17 10:49:34Z mellinger $
## Authors: juergen.mellinger@neurotechcenter.org
## Description: Create a Simulink MEX file

option(BUILD_SIMULINK_MEX_FILES "Build Matlab Simulink Mex files" OFF)

macro(bci2000_add_simulink_mexfile)
  utils_parse_args("NAME;SOURCES" ${ARGV})

  if(BUILD_SIMULINK_MEX_FILES)
    utils_include(frameworks/Core)
    bci2000_include(matlab_simulink)
    bci2000_add_target(
      INFO "Matlab Simulink mex file"
      DLL ${NAME}
      ${SOURCES}
      ${PROJECT_SRC_DIR}/shared/bcistream/BCIStream_mex.cpp
      OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    )
    if (TARGET ${NAME})
      utils_get_mex_suffix(mex_suffix_)
      set_property(TARGET ${NAME} PROPERTY SUFFIX ".${mex_suffix_}")
    endif()
  endif()
endmacro()
