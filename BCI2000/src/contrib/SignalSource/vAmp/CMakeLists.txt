###########################################################################
## $Id: CMakeLists.txt 7678 2023-10-26 14:21:43Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for vAmpSource

set( FirstAmpDll FirstAmp32.dll )
set( MSVRCDLL msvrc32/msvcr100.dll )
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  set( FirstAmpDll FirstAmp64.dll )
  set( MSVRCDLL msvrc64/msvcr100.dll )
endif()

bci2000_include( Math )
bci2000_use( Qt )
bci2000_add_signal_source_module( vAmpSource
  vAmpADC.cpp 
  vAmpChannelInfo.cpp
  vAmpThread.cpp
  vAmpDisplay.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Qt/QtMain.cpp
  lib/FirstAmp.imports.cpp
  ${BCI2000_SRC_DIR}/shared/gui/GUI.h
  lib/bin/${FirstAmpDll}
  lib/${MSVRCDLL}
)
