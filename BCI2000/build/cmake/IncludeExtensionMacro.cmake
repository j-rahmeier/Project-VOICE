###########################################################################
## $Id: IncludeExtensionMacro.cmake 7465 2023-06-30 15:23:43Z mellinger $
## Authors: jezhill@gmail.com
## Description: Contains a function for including optional extensions to the BCI2000 framework
##              (for example, loggers which will by incorporated into every SignalSource module)
##              gated by environment variables.

macro( INCLUDE_EXTENSION NAME DIR )

  if ("${ARGN}" STREQUAL "NDA" AND NOT BUILD_INCLUDE_NDA)
    message(
      "Omitting custom extension ${NAME} because it requires an NDA, and BUILD_INCLUDE_NDA is OFF"
    )
  else()
    STRING( TOUPPER ${NAME} UPPERNAME )
    SET( SETTINGNAME "EXTENSIONS_${UPPERNAME}" )
    set( HELPTEXT "Whether to augment the BCI2000 Framework using the contributed ${NAME} extension" )
    OPTION( ${SETTINGNAME} ${HELPTEXT} OFF )
    SET( SETTINGVAL  ${${SETTINGNAME}} )
    get_filename_component( BCI2000_EXTENSION_DIR "${DIR}" ABSOLUTE )
    IF( ${SETTINGVAL} )
      UTILS_CONFIG_STATUS(
        "Including custom extension ${NAME}\n"
        "---- (to disable, set ${SETTINGNAME} to OFF)"
      )
      INCLUDE( "${BCI2000_EXTENSION_DIR}/IncludeExtension.cmake" )
    ELSE()
      UTILS_CONFIG_STATUS(
        "Skipping custom extension ${NAME}\n"
        "---- (to enable, set ${SETTINGNAME} to ON)"
      )
    ENDIF( ${SETTINGVAL} )
  endif()

endmacro( INCLUDE_EXTENSION NAME DIR)
