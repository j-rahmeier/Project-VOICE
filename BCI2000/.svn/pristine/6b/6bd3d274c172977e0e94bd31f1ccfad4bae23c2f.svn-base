set( DIR_NAME "${DIR_NAME}Tests/" )
add_subdirectory(LibTiny)
utils_set_appicon(puzzlepiece)
utils_include(frameworks/Core)

bci2000_add_target(
  INFO "Test"
  CONSOLEAPP sockstream_test
  sockstream_test.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Lib/SockStream.cpp
  OUTPUT_DIRECTORY "${PROJECT_BUILD_ROOT}/test"
)
add_test(NAME sockstream COMMAND sockstream_test 12 1000)

bci2000_add_target(
  INFO "Test"
  CONSOLEAPP ioobjects_test
  ioobjects_test.cpp
  OUTPUT_DIRECTORY "${PROJECT_BUILD_ROOT}/test"
)
add_test(NAME ioobjects COMMAND ioobjects_test 12 200)

bci2000_add_target(
  INFO "Test"
  CONSOLEAPP clock_test
  clock_test.cpp
  OUTPUT_DIRECTORY "${PROJECT_BUILD_ROOT}/test"
)

bci2000_add_target(
  INFO "Test"
  CONSOLEAPP wall_clock_test
  wall_clock_test.cpp
  OUTPUT_DIRECTORY "${PROJECT_BUILD_ROOT}/test"
)

if(WIN32)
  add_test(
      NAME "Executable regression" 
      COMMAND ${PROJECT_SOURCE_DIR}/buildutils/tests/TestExecutables.bat
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/buildutils/tests
  )
endif()
