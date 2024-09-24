###########################################################################
## $Id: CMakeLists.txt 7657 2023-10-13 15:15:52Z mellinger $
## Authors: mellinger@neurotechcenter.org
## Description: Build information for BCI2000 and CMake

# Set this directory name
set(DIR_NAME "${DIR_NAME}/Simulink/")

add_subdirectory(BCI2000OnlineLink)
add_subdirectory(SignalProcessing)


