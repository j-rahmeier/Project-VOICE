###########################################################################
## $Id: CMakeLists.txt 7835 2024-02-13 13:39:45Z mellinger $
## Author: juergen.mellinger@uni-tuebingen.de
## Description: Build information for BCI2000ViewerQt

# Set the executable name
SET( EXECUTABLE_NAME BCI2000Viewer )

# Set the project specific sources
SET( SRC_PROJECT
  main.cpp 
  BCI2000Viewer.ui
  BCI2000Viewer.cpp
  SignalWidget.cpp
  ${BCI2000_SRC_DIR}/shared/gui/SignalDisplay.cpp
  ${BCI2000_SRC_DIR}/shared/utils/DisplayFiltering/DisplayFilter.cpp
  AVControl.cpp
  VideoWidget.cpp
  ${BCI2000_SRC_DIR}/shared/utils/AV/AVDecoder.cpp
  ${BCI2000_SRC_DIR}/shared/utils/AV/AVUtils.cpp
  ${BCI2000_SRC_DIR}/shared/utils/AV/AVPlayer.cpp
  ${BCI2000_SRC_DIR}/shared/utils/AV/AudioSink.cpp
  ${BCI2000_SRC_DIR}/shared/utils/AV/AudioDevice.cpp
  ${BCI2000_SRC_DIR}/shared/utils/AV/AudioPlayer.cpp
  ${BCI2000_SRC_DIR}/shared/utils/ClockTick.cpp
  NotesWidget.cpp
)
if(APPLE)
  list(APPEND SRC_PROJECT
    Info.plist
  )
endif()

BCI2000_INCLUDE(math)
BCI2000_INCLUDE(ffmpeg)
BCI2000_INCLUDE(PortAudio)
BCI2000_ADD_TOOLS_GUIAPP( ${EXECUTABLE_NAME} ${SRC_PROJECT} )
