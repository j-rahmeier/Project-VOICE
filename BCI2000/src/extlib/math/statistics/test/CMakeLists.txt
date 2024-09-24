###########################################################################
## $Id: CMakeLists.txt 7637 2023-10-02 23:56:47Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for ObserverTest

set(DIR_NAME Tests/Statistics/)

if(BUILD_TESTS)

bci2000_include(Math)
bci2000_include(Statistics)
utils_include(frameworks/Core)
utils_set_appicon(puzzlepiece)
bci2000_add_target(
  CONSOLEAPP
  ObserverTest
  ObserverTest.cpp
  ../WindowObserver.cpp
  ${PROJECT_SRC_DIR}/shared/bcistream/BCIStream_tool.cpp
  OUTPUT_DIRECTORY "${PROJECT_BUILD_ROOT}/test"
)

endif(BUILD_TESTS)
