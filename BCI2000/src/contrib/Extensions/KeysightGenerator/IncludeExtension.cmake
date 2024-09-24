#######################################################################################
# Author: Robbie@DESKTOP-U5JGQCU.wucon.wustl.edu
# Description: KeysightTestLogger extension definition
#######################################################################################

# It is likely that your logger requires hardware drivers that are only
# available under Windows. If so, uncomment the following lines:
# if(NOT WIN32)
#   message("**** KeysightGenerator cannot be built on this platform")
#   return()
# endif()

if (NOT WIN32 OR CMAKE_SIZEOF_VOID_P EQUAL 4)
  # NOTE: visa32.dll is just a forwarding DLL to visa64.dll, NOT a 32 bit DLL
  message("**** KeysightGenerator cannot be built for this platform")
  return()
endif()

# The directory where this extension resides is available as ${BCI2000_EXTENSION_DIR}.
# Put all required headers and libraries inside that directory, or into suitable
# subdirectories.


list(APPEND BCI2000_SIGSRC_FILES
   ${BCI2000_EXTENSION_DIR}/KeysightGenerator.cpp
   ${BCI2000_EXTENSION_DIR}/lib/x64/visaConfMgr.dll
   ${BCI2000_EXTENSION_DIR}/lib/x64/visaUtilities.dll
   # Add any further cpp files or DLLs here,
   # they will be copied to the /prog folder at build time:
   # {BCI2000_EXTENSION_DIR}/dylib/mylib.dll
)

IF( CMAKE_SIZEOF_VOID_P EQUAL 4 )
  list(APPEND BCI2000_SIGSRC_FILES
   ${BCI2000_EXTENSION_DIR}/lib/x32/visa32.dll
  )
  INCLUDE_DIRECTORIES(
    ${BCI2000_EXTENSION_DIR}/include/x32
  )
  list(APPEND BCI2000_SIGSRC_LIBS
    # Add any libraries required at link time here:
    ${BCI2000_EXTENSION_DIR}/lib/x32/agvisa32.lib
    ${BCI2000_EXTENSION_DIR}/lib/x32/agvisaext.lib
    ${BCI2000_EXTENSION_DIR}/lib/x32/ktvisaext.lib
    ${BCI2000_EXTENSION_DIR}/lib/x32/visa32.lib
    ${BCI2000_EXTENSION_DIR}/lib/x32/visaext.lib
  )
ELSE()
  list(APPEND BCI2000_SIGSRC_FILES
    ${BCI2000_EXTENSION_DIR}/lib/x64/visa64.dll
  )
  INCLUDE_DIRECTORIES(
    ${BCI2000_EXTENSION_DIR}/include/x64
    ${BCI2000_EXTENSION_DIR}/lib/x64
  )
  list(APPEND BCI2000_SIGSRC_LIBS
    # Add any libraries required at link time here:
    #${BCI2000_EXTENSION_DIR}/lib/x64/visa32.lib
    ${BCI2000_EXTENSION_DIR}/lib/x64/visa64.lib
  )
  #target_link_libraries( ${BCI2000FrameworkSigSrcModule} ${BCI2000_SRC_DIR}/contrib/Extensions/KeysightGenerator/lib/x64/visa32.lib )
ENDIF()
