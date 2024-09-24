###########################################################################
## $Id: CMakeLists.txt 4536 2024-03-13 14:30:13Z wengelhardt $
## Authors: engelhardt@neurotechcenter.org
## Description: Build information for PACFilter module

SET( EXECUTABLE_NAME PAC )
SET(vis_src_path ${BCI2000_SRC_DIR}/contrib/ExternalLinks/PythonVisualizations)
SET(vis_dest_path /python/visualizations)

# Set the project specific sources
SET( SRC_PROJECT
  PACFilter.cpp
  PipeDefinition.cpp
  ${BCI2000_SRC_DIR}/contrib/SignalProcessing/HilbertSignalProcessing/HilbertFilter.cpp
)
SET( HDR_PROJECT
  PACFilter.h
  ${BCI2000_SRC_DIR}/contrib/SignalProcessing/HilbertSignalProcessing/HilbertFilter.h
)

# Create the signal processing module
BCI2000_ADD_SIGNAL_PROCESSING_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 

  SUPPORT PAC_SignalGenerator.bat=>/batch
  SUPPORT pac_pipeline.prm=>/parms/PAC

  SUPPORT pacDemo.py=>${vis_dest_path}
  SUPPORT ${vis_src_path}/*=>${vis_dest_path}
  SUPPORT ${vis_src_path}/scripts/*=>${vis_dest_path}/scripts
)
