###########################################################################
## $Id: ToolsFilterDllMacro.cmake 7465 2023-06-30 15:23:43Z mellinger $
## Authors: juergen.mellinger@neurotechcenter.org
## Description: Create a filter module

option( BUILD_FILTER_DLLS "Build filter DLLs" ON )

macro( bci2000_add_filter_dll )
  utils_parse_args( "NAME;SOURCES" ${ARGV} )

  if( BUILD_FILTER_DLLS )

    set( old_crt_link_type_ ${crt_link_type} )
    set_crt_link_type( DYNAMIC )
    string(REPLACE lib "" FILTER_NAME ${NAME})
    utils_include( frameworks/Core )
    add_definitions( -DFILTER_WRAPPER_LIBRARY=1 )
    bci2000_add_target(
      INFO "Filter module"
      DLL ${NAME}
      ${SOURCES}
      ${PROJECT_SRC_DIR}/shared/filters/FilterWrapperLibrary.cpp
      ${PROJECT_SRC_DIR}/shared/bcistream/BCIStream_filtermodule.cpp
      ${PROJECT_SRC_DIR}/shared/utils/Multithreading.cpp
      OUTPUT_DIRECTORY "${PROJECT_BUILD_ROOT}/tools/FilterDLLs/"
    )
    set_target_properties(${NAME} PROPERTIES OUTPUT_NAME ${FILTER_NAME})
    set_crt_link_type( ${old_crt_link_type_} )

  endif()

endmacro()
