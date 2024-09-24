###########################################################################
## Authors: bci2000@DESKTOP-503T7ML
## Description: Build information for the RippleADC source module
###########################################################################

# Use the bci2000_include macro if you need to link with frameworks from /src/extlib:
# bci2000_include( MATH )

SET( EXECUTABLE_NAME RippleADC )

if(WIN32)
IF( CMAKE_SIZEOF_VOID_P EQUAL 4 )
    set(	FILES_PROJECT
		${FILES_PROJECT}
		${SRC_PROJECT}/lib/x86/xipplib.dll
		)
ELSE()
    set(	FILES_PROJECT
		${FILES_PROJECT}
		${SRC_PROJECT}/lib/x64/xipplib.dll
		)
		


ENDIF()
bci2000_add_signal_source_module(
  "${EXECUTABLE_NAME}"
  SUPPORT batch/*=>/batch
  SUPPORT parms/*=>/parms/fragments/amplifiers
  RippleADC.cpp
  lib/imports/xipplib.h
  ${FILES_PROJECT}
)
IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  target_link_libraries( ${EXECUTABLE_NAME} ${PROJECT_SRC_DIR}/contrib/SignalSource/RippleADC/lib/x64/xipplib.lib )
ELSE()
  target_link_libraries( ${EXECUTABLE_NAME} ${PROJECT_SRC_DIR}/contrib/SignalSource/RippleADC/lib/x86/xipplib.lib )
ENDIF()

endif()
