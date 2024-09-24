###########################################################################
## $Id: CMakeLists.txt 6252 2021-03-24 16:39:45Z abelsten $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for the ParallelSwitch example

IF( BUILD_DEMOS )
  IF( WIN32 )
		IF ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
      MESSAGE("-- ParallelSwitch: 64 bit library not available, will not build")
    ELSE()
			# Set the executable name
			SET( NAME ParallelSwitch )

			# Set the project specific sources
			SET( SRC_PROJECT
			  ParallelSwitch.cpp
			  ${BCI2000_SRC_DIR}/shared/utils/Lib/SockStream.cpp
			)

			SET_OUTPUT_DIRECTORY( ${BCI2000_ROOT_DIR}/src/contrib/AppConnectorApplications/ParallelSwitch )
			BCI2000_ADD_TARGET( INFO "AppConnector example" CONSOLEAPP ${NAME} ${SRC_PROJECT} )

			# Set the target build folder
			SET_PROPERTY( TARGET ${NAME} PROPERTY FOLDER Demos/AppConnector )

			IF( WIN32 )
			  TARGET_LINK_LIBRARIES( ${NAME} ws2_32 )
			ENDIF( WIN32 )
		ENDIF ()
	ENDIF( WIN32 )
ENDIF( BUILD_DEMOS )