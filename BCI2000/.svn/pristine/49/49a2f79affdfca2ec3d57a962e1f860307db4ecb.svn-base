###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Authors: Alex Belsten belsten@neurotechcenter.org

IF (CMAKE_SIZEOF_VOID_P EQUAL 8 AND WIN32 ) 
  # Set the executable name
  SET( EXECUTABLE_NAME HandVibrationFilter)

  BCI2000_INCLUDE( "FFT" )


  # Set the project specific sources
  SET( SRC_PROJECT
    HandVibrationFilter.cpp
    HandVibration.cpp  
    SerialPort.cpp
	PipeDefinition.cpp
  )

  SET( HDR_PROJECT
    HandVibrationFilter.h
    HandVibration.h
    SerialPort.h
	Vibrotactile_Firmware/command_packet.h
  )

  # Create the signal processing module
  BCI2000_ADD_SIGNAL_PROCESSING_MODULE( 
    "${EXECUTABLE_NAME}" 
    "${SRC_PROJECT}" "${HDR_PROJECT}" 
  )
ELSE()
  MESSAGE( "--- HandVibrationFilter libraries not available for this platform. Will not build." )
ENDIF()
