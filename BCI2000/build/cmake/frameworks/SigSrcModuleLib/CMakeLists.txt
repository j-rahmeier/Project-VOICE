###########################################################################
## $Id: CMakeLists.txt 7952 2024-03-20 13:56:54Z mellinger $
## Authors: griffin.milsap@gmail.com
## Authors: griffin.milsap@gmail.com, juergen.mellinger@uni-tuebingen.de
## Description: Builds a static library containing components specific to
##   source modules.

SET( NAME BCI2000FrameworkSigSrcModule )

utils_include( frameworks/SigSrcModuleLib/SigSrcFiles )

# Create the library project
UTILS_INCLUDE( frameworks/SigSrcModule )
list( APPEND SRC_BCI2000_FRAMEWORK ${BCI2000_SIGSRC_FILES} )
list( APPEND LIBS ${BCI2000_SIGSRC_LIBS} )
set(sources_)
foreach(extlib_ ${BCI2000_SIGSRC_INCLUDE_EXTLIB})
  list(APPEND sources_ INCLUDE_EXTLIB ${extlib_})
endforeach()
foreach(res_ ${BCI2000_SIGSRC_RESOURCES})
  utils_add_resource2(${res_})
endforeach()

BCI2000_ADD_REGISTRY( ${REGISTRY_NAME} SRC_BCI2000_FRAMEWORK DEPTARGETS )
BCI2000_ADD_TARGET( INFO "Framework library" STATIC_LIBRARY ${NAME} ${sources_})

IF( FAILED )
  utils_warn(
    "Could not satisfy dependencies of the SignalSource library.\n"
    "\tSignalSource modules will be omitted."
  )
ELSE( FAILED )

  TARGET_LINK_LIBRARIES( ${NAME} ${LIBS} )
  ADD_DEPENDENCIES( ${NAME} ${DEPTARGETS} )

ENDIF( FAILED )
