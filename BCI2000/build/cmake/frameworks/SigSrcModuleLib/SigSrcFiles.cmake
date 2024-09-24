###########################################################################
## $Id: SigSrcFiles.cmake 7527 2023-08-14 19:21:11Z mmarcus $
## Authors: griffin.milsap@gmail.com, juergen.mellinger@uni-tuebingen.de

set( SRC_BCI2000_FRAMEWORK
  ${SRC_BCI2000_FRAMEWORK}

  ${PROJECT_SRC_DIR}/shared/fileio/FileWriterBase.cpp
  ${PROJECT_SRC_DIR}/shared/fileio/FileSplittingCondition.cpp
  ${PROJECT_SRC_DIR}/shared/fileio/GenericFileWriter.cpp
  ${PROJECT_SRC_DIR}/shared/fileio/GenericOutputFormat.cpp
  ${PROJECT_SRC_DIR}/shared/fileio/NullFileWriter.cpp

  ${PROJECT_SRC_DIR}/shared/fileio/dat/BCI2000FileWriter.cpp
  ${PROJECT_SRC_DIR}/shared/fileio/dat/BCI2000OutputFormat.cpp

  ${PROJECT_SRC_DIR}/shared/fileio/edf_gdf/EDFHeader.cpp
  ${PROJECT_SRC_DIR}/shared/fileio/edf_gdf/EDFFileWriter.cpp
  ${PROJECT_SRC_DIR}/shared/fileio/edf_gdf/EDFOutputBase.cpp
  ${PROJECT_SRC_DIR}/shared/fileio/edf_gdf/EDFOutputFormat.cpp
  ${PROJECT_SRC_DIR}/shared/fileio/edf_gdf/GDF.cpp
  ${PROJECT_SRC_DIR}/shared/fileio/edf_gdf/GDFFileWriter.cpp
  ${PROJECT_SRC_DIR}/shared/fileio/edf_gdf/GDFOutputFormat.cpp

  ${PROJECT_SRC_DIR}/shared/modules/signalsource/BufferedADC.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalsource/GenericADC.h

  ${PROJECT_SRC_DIR}/shared/modules/signalsource/AlignmentFilter.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalsource/DataIOFilter.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalsource/TransmissionFilter.cpp

  ${PROJECT_SRC_DIR}/shared/modules/signalsource/logging/EventLink.cpp

  ${PROJECT_SRC_DIR}/shared/utils/Multithreading.cpp
)

if( WIN32 OR APPLE )
  list( APPEND SRC_BCI2000_FRAMEWORK
    ${PROJECT_SRC_DIR}/shared/modules/signalsource/logging/JoystickLogger.cpp
    ${PROJECT_SRC_DIR}/shared/modules/signalsource/logging/KeyLogger.cpp
  )
endif()
