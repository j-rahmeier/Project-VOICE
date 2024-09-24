###########################################################################
## $Id: CMakeLists.txt 8173 2024-06-01 14:30:21Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for BCI2000ExportQt

# Set the executable name
SET( EXECUTABLE_NAME BCI2000Export )

# Set the project specific sources
SET( SRC_PROJECT
  main.cpp 
  MainWindow.cpp
  BCIReader.cpp
  Converters/ASCIIConverter.cpp
  Converters/BrainVisionConverter.cpp
)
SET( HDR_PROJECT
  MainWindow.h
  BCIReader.h
  Converters/ASCIIConverter.h
  Converters/BrainVisionConverter.h
)

if(APPLE)
  list(APPEND SRC_PROJECT
    Info.plist
  )
endif()

# Create the tool
BCI2000_ADD_TOOLS_GUIAPP( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
  "${GENERATED}"
)


