###########################################################################
## $Id: CMakeLists.txt 7678 2023-10-26 14:21:43Z mellinger $
## Authors: mellinger@neurotechcenter.org
## Description: Build information for BCI2000ShellHelper

if(APPLE)

utils_set_appicon("wrench;prompt")
utils_include(Frameworks/LibTiny)

bci2000_add_target(
  QTAPP
  BCI2000ShellHelper
  INFO Helper
  main.cpp
  Info.plist
  ${PROJECT_SRC_DIR}/shared/utils/Qt/QtMain.cpp
  ${PROJECT_SRC_DIR}/shared/types/Blob.cpp
)

endif(APPLE)
