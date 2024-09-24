set( DIR_NAME "${DIR_NAME}LibTiny/" )
utils_set_appicon(puzzlepiece)

utils_include(frameworks/LibTiny)
add_definitions(-DENABLE_UNITTEST)
bci2000_add_target(
  INFO "Test"
  CONSOLEAPP LibTiny_unit_tests
  WildcardMatchUnitTest.cpp
  RatioUnitTest.cpp
  StringUtilsUnitTest.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Lib/UnitTestMain.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Lib/UnitTest.cpp
  OUTPUT_DIRECTORY "${PROJECT_BUILD_ROOT}/test"
)
add_test(NAME LibTiny COMMAND LibTiny_unit_tests)

