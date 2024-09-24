###########################################################################
## $Id: CMakeLists.txt 6725 2022-05-06 14:08:30Z mellinger $
## Description: Build information for the SoundcardSource source module

BCI2000_INCLUDE(PortAudio)
BCI2000_INCLUDE(Statistics)
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  SoundcardSource
  SoundcardADC.cpp
  ${BCI2000_SRC_DIR}/shared/utils/AV/AudioDevice.cpp
  ${BCI2000_SRC_DIR}/shared/utils/ClockTick.cpp
)

