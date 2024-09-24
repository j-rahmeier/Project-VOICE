###########################################################################
## $Id: IncludeExtension.cmake 7883 2024-03-01 20:59:08Z mellinger $
## Authors: jezhill@gmail.com

add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/WebcamRecorder ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/contrib/Extensions/WebcamRecorder )

list( APPEND BCI2000_SIGSRC_FILES
   ${BCI2000_EXTENSION_DIR}/WebcamLogger.cpp
   ${BCI2000_EXTENSION_DIR}/WebcamController.cpp
)



