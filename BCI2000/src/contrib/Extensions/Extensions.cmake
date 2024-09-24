###########################################################################
## $Id: Extensions.cmake 7498 2023-07-28 20:57:02Z jhill $
## Authors: jezhill@gmail.com
## Description: Build information BCI2000 Framework extensions

set( HERE ${CMAKE_CURRENT_LIST_DIR} )  # bad naming => seems counterintuitive, but this is the variable
                                       # that indicates the parent directory of the current file as it is
                                       # being processed via INCLUDE() regardless of whether it is a list
                                       # file or not

FILE( GLOB subdirs RELATIVE "${HERE}" "${HERE}/*" )
FOREACH( subdir ${subdirs} )
	IF( EXISTS ${HERE}/${subdir}/IncludeExtension.cmake )
		INCLUDE_EXTENSION( ${subdir} ${HERE}/${subdir} )
	ENDIF()	
ENDFOREACH()

# NB: in the previous approach, where everything was named explicitly, the one anomaly was:
#        INCLUDE_EXTENSION( EyetrackerLoggerSimulated    "${BCI2000_SRC_DIR}/contrib/Extensions/EyetrackerLoggerSimulator" )
#                                                  ^^                                                                  ^^
# This has been renamed to EyetrackerLoggerSimulator (the impact of which is just to change the name of the CMake option)
