###########################################################################
## $Id: CMakeLists.txt 7805 2024-01-26 12:39:40Z mellinger $
## Authors: mellinger@neurotechcenter.org
## Description: Build information for BCI2000 and CMake

bci2000_add_simulink_mexfile(
  BCI2000OnlineLink
  BCI2000OnlineLink.cpp
  SUPPORT BCI2000.mdl=>/prog/Simulink
  SUPPORT bci2000logo256.png=>/prog/Simulink
  SUPPORT MinimalExample.slx=>/prog/Simulink
  SUPPORT BCI2000OnlineLink.cpp=>/prog/Simulink
  OUTPUT_DIRECTORY ${PROJECT_BUILD_ROOT}/prog/Simulink
)

