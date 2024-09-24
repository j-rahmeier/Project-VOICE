bci2000_add_signal_processing_module(
  SimulinkConnectorSignalProcessing
  SimulinkConnectorFilter.cpp
  PipeDefinition.cpp
  SUPPORT SimulinkConnector.bat=>/batch
  DEPENDS Operator
  DEPENDS SignalGenerator
  DEPENDS DummyApplication
)


