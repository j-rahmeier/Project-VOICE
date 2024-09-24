bci2000_include(Qt)
bci2000_include(OpenGL)
set(GUI_DIR ${BCI2000_SRC_DIR}/shared/gui)

bci2000_add_signal_processing_module(
  ComplexVisDemoSignalProcessing #NOBUNDLE
  PipeDefinition.cpp
  ComplexVisualizationDemoFilter.cpp
  ComplexVisualizationDemo.bat
  ${GUI_DIR}/GUI.cpp
  ${GUI_DIR}/OpenGLContext.cpp
  ${GUI_DIR}/GraphDisplay.cpp
  ${GUI_DIR}/GraphObject.cpp
  ${GUI_DIR}/Shapes.cpp
  ${GUI_DIR}/TextField.cpp
  ${GUI_DIR}/Bitmap.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Qt/QtMain.cpp
  ${BCI2000_SRC_DIR}/shared/utils/Qt/QtProxyThread.cpp
  ${BCI2000_SRC_DIR}/shared/types/BitmapImage_QtGlue.cpp
)

