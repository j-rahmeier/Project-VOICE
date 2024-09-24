###########################################################################
## $Id: CMakeLists.txt 8239 2024-07-03 14:00:09Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for BCI2000FileInfoQt

# Set the executable name
SET( EXECUTABLE_NAME BCI2000Watchdog )

# Set the project specific sources
SET( SRC_PROJECT
  main.cpp 
  MainWindow.cpp
  LogWidget.cpp
  Watchdog.cpp
  ProcessList.cpp
  ${BCI2000_SRC_DIR}/core/Operator/BCI2000Remote/BCI2000Connection.cpp
)
if(APPLE)
  list(APPEND SRC_PROJECT
    Info.plist
  )
endif()

utils_set_appicon( eye )

# Create the signal processing module
BCI2000_ADD_TOOLS_GUIAPP( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}"
  OUTPUT_DIRECTORY ${PROJECT_BUILD_ROOT}/prog
)
