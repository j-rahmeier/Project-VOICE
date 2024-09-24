###########################################################################
## $Id: CMakeLists.txt 8087 2024-04-29 19:29:46Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for OperatorQt

# Set the executable name
SET( EXECUTABLE_NAME Operator )

# Set the project specific sources
SET( SRC_PROJECT
  main.cpp 
  MainWindow.ui
  MainWindow.cpp
  ConnectionInfo.cpp
  ShowStates.cpp
  WatchesWindow.cpp
  NotesWindow.cpp
  VisRecorder.cpp
  SysLog.cpp

  ${BCI2000_PARAMETER_GUI}

  ../OperatorLib/src/ScriptParser/ParserToken.cpp
  ScriptingExtensions/ScriptingExtensions.cpp
  ScriptingExtensions/WindowType.cpp
  ScriptingExtensions/TitleType.cpp
  ScriptingExtensions/ButtonType.cpp
  ScriptingExtensions/WatchType.cpp
  ScriptingExtensions/VisualizationType.cpp
  ScriptingExtensions/NoteType.cpp
  ScriptingExtensions/TelnetType.cpp
  ScriptingExtensions/WebsocketType.cpp

  Visualizations/SignalWidget.cpp
  Visualizations/VisDisplay.cpp
  Visualizations/VisDisplayWindow.cpp
  Visualizations/VisDisplayContainer.cpp
  Visualizations/VisDisplayLayer.cpp
  Visualizations/VisDisplayBase.cpp
  Visualizations/VisDisplayMemo.cpp
  Visualizations/VisDisplayGraph.cpp
  Visualizations/VisDisplayBitmap.cpp
  Visualizations/VisDisplayVideoFrame.cpp

  ${PROJECT_SRC_DIR}/shared/utils/AV/AVEncoder.cpp
  ${PROJECT_SRC_DIR}/shared/utils/AV/AVError.cpp
  ${PROJECT_SRC_DIR}/shared/utils/AV/AVLog.cpp
  ${PROJECT_SRC_DIR}/shared/utils/DisplayFiltering/DisplayFilter.cpp
  ${PROJECT_SRC_DIR}/shared/utils/DisplayFiltering/DecimationFilter.cpp
  ${PROJECT_SRC_DIR}/shared/utils/DisplayFiltering/ScaleObservationFilter.cpp
  ${PROJECT_SRC_DIR}/shared/bcistream/BCIStream_guiapp.cpp
  ${PROJECT_SRC_DIR}/shared/gui/SignalDisplay.cpp
  ${PROJECT_SRC_DIR}/shared/gui/AboutBox.cpp
  ${PROJECT_SRC_DIR}/shared/gui/ColorListChooser.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Settings.cpp
  ${PROJECT_SRC_DIR}/shared/utils/LogEntry.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Qt/QtSignalConnector.h

  ${PROJECT_SRC_DIR}/shared/utils/Qt/QtMain.cpp
)
include_directories( ${PROJECT_SRC_DIR}/core/Operator/OperatorLib/include/ )


if( WIN32 )
  if( USE_APPLICATION_BUNDLES )
    set( OPERATOR_LIB ${PROJECT_BUILD_ROOT}/prog/OperatorLib.dll )
    list( APPEND SRC_PROJECT ${OPERATOR_LIB} )
    set_source_files_properties( ${OPERATOR_LIB} PROPERTIES GENERATED TRUE )
  endif()
endif()

if( APPLE )
  list(APPEND SRC_PROJECT
    Info.plist
  )
endif()

bci2000_include( math )
bci2000_include( ffmpeg )
utils_include( frameworks/Core )
utils_include( frameworks/LibTiny )
bci2000_add_to_inventory( Operator ${EXECUTABLE_NAME} )
utils_set_appicon( gui-screen;signal-document )
bci2000_add_target(
  INFO "Operator module" QTAPP ${EXECUTABLE_NAME}
  ${SRC_PROJECT}
  LIBRARY OperatorLib
)

