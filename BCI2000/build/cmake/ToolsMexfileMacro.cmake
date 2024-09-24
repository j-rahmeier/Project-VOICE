###########################################################################
## $Id: ToolsMexfileMacro.cmake 7657 2023-10-13 15:15:52Z mellinger $
## Authors: juergen.mellinger@neurotechcenter.org
## Description: Create a mex file

option( BUILD_MEX_FILES "Build Matlab mex files" OFF )

macro( bci2000_add_mexfile )
  utils_parse_args( "NAME;SOURCES" ${ARGV} )

  if( BUILD_MEX_FILES )
    utils_get_mex_suffix(mex_suffix_)
    utils_include( frameworks/Core )
    bci2000_include( matlab_mex )
    set( mex_companions_
      COMPANION test_bcimex.m
      COMPANION testdata.dat
      COMPANION testdata.mat
    )
    if( EXISTS "${PROJECT_SRC_DIR}/core/Tools/mex/${NAME}.m" )
      set( mex_companions_
        ${mex_companions_}
        COMPANION "${NAME}.m"
        COMPANION missing_mex_file.m
      )
    endif()
    bci2000_add_target(
      INFO "Matlab mex file"
      DLL ${NAME}
      ${SOURCES}
      ${PROJECT_SRC_DIR}/core/Tools/mex/mexutils.cpp
      ${PROJECT_SRC_DIR}/shared/bcistream/BCIStream_mex.cpp
      OUTPUT_DIRECTORY "${PROJECT_BUILD_ROOT}/tools/mex/"
      ${mex_companions_}
    )
    set_property( TARGET ${NAME} PROPERTY SUFFIX ".${mex_suffix_}" )
  endif()
endmacro()
