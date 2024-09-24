#include "BCI2000Remote.h"
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char* argv[])
{
  // Instantiate a BCI2000Remote object
  BCI2000Remote bci;
  // Assume that Operator executable resides in the same directory as this program.
  std::string path = (argc > 0) ? argv[0] : "";
  size_t pos = path.find_last_of("\\/");
  path = (pos != std::string::npos) ? path.substr(0, pos + 1) : "";
  // Start the Operator module, and connect
  bci.OperatorPath(path + "Operator");
  if (!bci.Connect())
  {
    std::cerr << bci.Result();
    return -1;
  }
  // Startup modules
  const char* modules[] = { "SignalGenerator --LogMouse=1", "ARSignalProcessing", "CursorTask" };
  std::vector<std::string> vModules(&modules[0], &modules[0] + sizeof(modules) / sizeof(*modules));
  if (!bci.StartupModules(vModules))
  {
    std::cerr << bci.Result();
    return -1;
  }
  // Load a parameter file, and set subject information
  bci.LoadParametersRemote("../parms/examples/CursorTask_SignalGenerator.prm");
  bci.SubjectID("SUB");
  if (!bci.SetConfig())
  {
      std::cerr << bci.Result();
      return -1;
  }
  // Register a callback for the control signal
  auto callback = [](void*, const char* s) {
      std::cout << "packet#\tSignal(1,1)\tSignal(2,1)\t" << s;
  };
  auto watch = bci.NewWatch("Signal(1,1)\tSignal(2,1)", callback, nullptr);
  // Start a run
  if (!bci.Start())
  {
    std::cerr << bci.Result();
    return -1;
  }
  std::string state;
  while (bci.GetSystemState(state) && state == "Running")
      std::this_thread::sleep_for(std::chrono::seconds(1));
  delete watch;
  return 0;
}

