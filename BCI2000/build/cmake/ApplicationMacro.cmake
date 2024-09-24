###########################################################################
## $Id: ApplicationMacro.cmake 7465 2023-06-30 15:23:43Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Contains a macro for creating an application module

MACRO( BCI2000_ADD_APPLICATION_MODULE )
  UTILS_PARSE_ARGS( "NAME;SOURCES" ${ARGV} )

  UTILS_INCLUDE( frameworks/AppModule )
  ADD_DEFINITIONS( -DMODTYPE=3 )
  utils_set_appicon( presentation-screen )
  BCI2000_ADD_CORE_MAIN( ${NAME} ${SOURCES} )
  BCI2000_ADD_TARGET( INFO "Application module" EXECUTABLE ${NAME} ${SOURCES} )
  IF( NOT FAILED )
    BCI2000_ADD_TO_INVENTORY( Application ${NAME} )
  ENDIF()

ENDMACRO()
