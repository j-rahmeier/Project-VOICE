###########################################################################
## $Id: CMakeLists.txt 8204 2024-06-19 18:22:03Z mellinger $
## Author: mellinger@neurotechcenter.org
## Description: Build information for BCI2000Analysis

SET( NAME BCI2000Analysis )

SET( SRC_PROJECT
  main.cpp
  ${BCI2000_PARAMETER_GUI}
  AnalysisWindow/AnalysisWindow.cpp
  AnalysisWindow/AnalysisParamWidget.cpp
  AnalysisWindow/AnalysisTargetWidget.cpp
  AnalysisWindow/AnalysisLogWidget.cpp
  AnalysisWindow/FilesWidget.cpp
  AnalysisWindow/FiltersWidget.cpp
  AnalysisWindow/ProfilerWindow.cpp
  ClassifierDataWindow/ClassifierDataWindow.cpp
  ClassifierDataWindow/ChannelPlotsWindow.cpp
  ClassifierDataWindow/ClassifierParamWidget.cpp
  CSPDataWindow/CSPDataWindow.cpp
  ApplicationDataWindows/ApplicationDataWindow.cpp
  ApplicationDataWindows/HistogramWidget.cpp
  TopoplotWindow/TopoplotWidget.cpp
  TopoplotWindow/TopoplotWindow.cpp
  TopoplotWindow/ElectrodeNames.cpp
  TopoplotWindow/Interpolation/Interpolation.cpp
  TopoplotWindow/Interpolation/LinearInterpolation.cpp
  TopoplotWindow/Interpolation/ThinPlateSplinesInterpolation.cpp
  ClassifierWindow/ClassifierWindow.cpp
  ClassifierWindow/ClassifierPropsWidget.cpp
  ClassifierWindow/ClassifierChannelPlotWindow.cpp
  LogWindow/LogWindow.cpp
  LogWindow/BCIStream_LogWindow.cpp
  AnalysisData.cpp
  SignalWidget.cpp
  Statistics.cpp
  FileDialog.cpp
  WindowSettings.cpp
  WindowFactory.cpp
  Menus/EditMenu.cpp
  Menus/WindowMenu.cpp
  Menus/HelpMenu.cpp
  FilterGraph/FilterGraph.cpp
  FilterGraph/FilterGraphNode.cpp
  FilterGraph/GraphSearch.cpp
  FilterGraph/CreateFilterGraph.cpp
  FilterGraph/Nodes/StaticNodes.cpp
  FilterGraph/Nodes/ApplicationNodes.cpp
  FilterGraph/Nodes/ApplicationFilters.cpp
  FilterGraph/Nodes/FilterNode.h
  FilterGraph/Nodes/PlaceholderNode.cpp
  FilterGraph/Nodes/DllFilterNode.cpp
  FilterGraph/Nodes/LinearClassifierNode.cpp
  FilterGraph/Nodes/CSPNode.cpp
  FilterGraph/Nodes/ObserverWrapper.cpp
  FilterGraph/Nodes/SplitterNode.cpp
  FilterGraph/Nodes/ParallelFilterCombination.cpp
  FilterGraph/Visualization/GraphVisualizer.cpp
  FilterGraph/Visualization/FilterGraphicsItem.cpp
  FilterGraph/Visualization/SubchainGraphicsItem.cpp
  FilterGraph/Visualization/ConnectorGraphicsItem.cpp
  FilterGraph/CppTranslator.cpp
  Classifiers/Classifier.cpp
  Classifiers/LDAClassifier.cpp
  Classifiers/SWLDAClassifier.cpp
  Classifiers/SVMClassifier.cpp
  Classifiers/ManualClassifier.cpp

  ${PROJECT_SRC_DIR}/shared/gui/SignalDisplay.cpp
  ${PROJECT_SRC_DIR}/shared/gui/AboutBox.cpp
  ${PROJECT_SRC_DIR}/shared/gui/ExecutableHelp.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Settings.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Qt/QtMain.cpp
  ${PROJECT_SRC_DIR}/shared/utils/LogEntry.cpp

  ${PROJECT_SRC_DIR}/shared/modules/signalsource/AlignmentFilter.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalsource/TransmissionFilter.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/SpatialFilter.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/IIRFilterBase.cpp 
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/IIRBandpass.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/ThreadedFilter.cpp  
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/WindowingFilter.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/Spectrum.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/ARSpectrum.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/FFTSpectrum.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/SpectralEstimator.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/ARFilter.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/P3TemporalFilter.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/LinearClassifier.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/LPFilter.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/ExpressionFilter.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/Normalizer.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/FrequencyEstimator.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/PowerEstimator.cpp
)

if(APPLE)
  list(APPEND SRC_PROJECT
    Info.plist
  )
endif()

utils_include_extlib( math )
utils_include_extlib( FFT )
utils_include_extlib( Eigen )
utils_include_extlib( SWLDA )
utils_include_extlib( libsvm )
utils_include_extlib( Delaunator )
utils_include_extlib( fastapprox )

set_crt_link_type( DYNAMIC )
utils_include( frameworks/Core )
utils_set_appicon( "wrench;signal-document;text-document;generic-document" )
BCI2000_ADD_TARGET(
    INFO Tool
    QTAPP ${NAME}
    ${SRC_PROJECT}
    OUTPUT_DIRECTORY ${PROJECT_BUILD_ROOT}/tools/${NAME}
)
BCI2000_ADD_TO_INVENTORY( Tool ${NAME} )

