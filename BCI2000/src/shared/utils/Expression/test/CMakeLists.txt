###########################################################################
## $Id: CMakeLists.txt 7677 2023-10-25 16:38:30Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for Calculator

if(BUILD_DEMOS)

set(DIR_NAME Demos/Expressions/)
utils_include(frameworks/Core)
utils_set_appicon(puzzlepiece)
bci2000_add_target(CONSOLEAPP
  Calculator
  Calculator.cpp
  ${PROJECT_SRC_DIR}/shared/bcistream/BCIStream_tool.cpp
  OUTPUT_DIRECTORY "${PROJECT_BUILD_ROOT}/demos"
)

endif()
