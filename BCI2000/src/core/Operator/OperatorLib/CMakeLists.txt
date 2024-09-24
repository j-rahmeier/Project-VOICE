###########################################################################
## $Id: CMakeLists.txt 8091 2024-05-01 13:48:52Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for the OperatorLib

# Set the executable name
SET( NAME OperatorLib )

SET( SRC_PROJECT
  include/BCI_OperatorLib.h
  src/BCI_OperatorLib.cpp
  src/CallbackBase.cpp
  src/StateMachine.cpp
  src/CoreClient.cpp
  src/EventLink.cpp
  src/TelnetServer.cpp
  src/WebsocketServer.cpp
  src/ScriptInterpreter.cpp
  src/CommandInterpreter.cpp

  src/ObjectTypes/ObjectType.cpp
  src/ObjectTypes/ConditionType.cpp
  src/ObjectTypes/ExecutableType.cpp
  src/ObjectTypes/ImpliedType.cpp
  src/ObjectTypes/ConnectionType.cpp
  src/ObjectTypes/SystemTypes.cpp
  src/ObjectTypes/SystemStates.cpp
  src/ObjectTypes/ParameterTypes.cpp
  src/ObjectTypes/StateTypes.cpp
  src/ObjectTypes/EventTypes.cpp
  src/ObjectTypes/VariableTypes.cpp
  src/ObjectTypes/SignalTypes.cpp
  src/ObjectTypes/VisPropertyTypes.cpp
  src/ObjectTypes/MessageTypes.cpp
  src/ObjectTypes/ScriptType.cpp
  src/ObjectTypes/ScriptEvents.cpp
  src/ObjectTypes/InterpreterExpression.cpp
  src/ObjectTypes/Watches.cpp
  src/ObjectTypes/WatchTypes.cpp
  src/ObjectTypes/ExpressionType.cpp
  src/ObjectTypes/FileSystemTypes.cpp
  src/ObjectTypes/LineType.cpp
  src/ObjectTypes/ProcessType.cpp

  src/ScriptParser/Script.cpp
  src/ScriptParser/ParserToken.cpp
  src/ScriptParser/ParserNodes.cpp
  src/ScriptParser/ScriptParser.cpp

  ${BCI2000_SRC_DIR}/shared/bcistream/BCIStream_opdll.cpp
)

IF( WIN32 )
  SET( SRC_PROJECT
    ${SRC_PROJECT}
    src/Main.cpp
  )
ENDIF( WIN32 )

utils_include( frameworks/Core )
utils_include( frameworks/LibTiny )
bci2000_use_inventory( SRC_PROJECT )
utils_set_appicon( puzzlepiece )
bci2000_add_target( INFO "Operator library" DLL ${NAME} ${SRC_PROJECT} )

# Add Pre-processor defines
set_property( TARGET ${NAME} APPEND PROPERTY COMPILE_FLAGS "-DLIBOPERATOR_LIBRARY" )
