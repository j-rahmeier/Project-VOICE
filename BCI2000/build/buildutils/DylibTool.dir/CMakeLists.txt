###########################################################################
## $Id: CMakeLists.txt 7659 2023-10-14 11:31:18Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for DylibTool.

SET( DIR_NAME Build/Tools )
utils_add_resource( HeaderTemplate templates/imports.h.in )
utils_add_resource( CppTemplate templates/imports.cpp.in )
utils_add_resource( CdeclTemplate templates/cdecl_imports.cpp.in )
UTILS_INCLUDE( frameworks/Core )
if( WIN32 )
  BCI2000_ADD_TARGET( CONSOLEAPP NOBUNDLE DylibTool
    DylibTool.cpp
    OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
  )
endif()
