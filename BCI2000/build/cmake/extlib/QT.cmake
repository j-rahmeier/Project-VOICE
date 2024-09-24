###########################################################################
## $Id: QT.cmake 8288 2024-07-29 18:04:49Z mellinger $
## Author: juergen.mellinger@uni-tuebingen.de
## Description: Find and include Qt
##   No support for pre-compiled static Qt versions any more.
##   Set CMAKE_PREFIX_PATH to the desired Qt installation's 
##   subdirectory before configuring project, e.g. using cmake-gui.

SET( QT_MODULES
  Core
  Widgets
  Gui
)

FUNCTION( GET_QT_VERSION outVer )

  SET( ${outVer} NOTFOUND PARENT_SCOPE )
  IF( EXISTS ${QT_QMAKE_EXECUTABLE} )
    EXECUTE_PROCESS(
      COMMAND ${QT_QMAKE_EXECUTABLE} -query QT_VERSION
      OUTPUT_VARIABLE ver
      RESULT_VARIABLE result
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    IF( result EQUAL 0 )
      SET( ${outVer} ${ver} PARENT_SCOPE )
    ENDIF()
  ENDIF()

ENDFUNCTION()

# assume built-in pthreads on macOS
IF(APPLE)
    set(CMAKE_THREAD_LIBS_INIT "-lpthread")
    set(CMAKE_HAVE_THREADS_LIBRARY 1)
    set(CMAKE_USE_WIN32_THREADS_INIT 0)
    set(CMAKE_USE_PTHREADS_INIT 1)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
ENDIF()

set(QT_AVAILABLE FALSE)
set(qtmajor)
set(QT_NO_CREATE_VERSIONLESS_FUNCTIONS 1)
find_package(Qt6 COMPONENTS ${QT_MODULES} QUIET)
if(Qt6_DIR)
  set(qtmajor 6)
else()
  find_package(Qt5 COMPONENTS ${QT_MODULES} QUIET)
  if(Qt5_DIR)
    set(qtmajor 5)
  endif()
endif()

if(Qt5_DIR) # defined by find_package on success
  set(QT_AVAILABLE TRUE)
  if(WIN32)
    find_program( QT_QMAKE_EXECUTABLE qmake NO_DEFAULT_PATH PATHS ${Qt5_DIR}/../../../bin )
  else()
    find_program( QT_QMAKE_EXECUTABLE NAMES qmake5 qmake )
  endif()
  if( NOT QT_QMAKE_EXECUTABLE )
    set(QT_AVAILABLE FALSE)
  endif()
  if( APPLE AND QT_QMAKE_EXECUTABLE MATCHES "/opt/homebrew/")
    message("You specified the homebrew version of Qt5, which does not work "
            "with BCI2000.\nPlease use a downloaded open-source version from www.qt.io"
    )
    set(QT_AVAILABLE FALSE)
  endif()

  macro( QT_WRAP_CPP )
    qt5_wrap_cpp( ${ARGN} )
  endmacro()
  macro( QT_WRAP_UI )
    qt5_wrap_ui( ${ARGN} )
  endmacro()
  macro( QT_ADD_RESOURCES )
    qt5_add_resources( ${ARGN} )
  endmacro()

endif()

if(Qt6_DIR) # defined by find_package on success
  set(QT_AVAILABLE TRUE)
  if(WIN32)
    find_program( QT_QMAKE_EXECUTABLE qmake NO_DEFAULT_PATH PATHS ${Qt6_DIR}/../../../bin )
  else()
    find_program( QT_QMAKE_EXECUTABLE NAMES qmake6 qmake )
  endif()
  if( NOT QT_QMAKE_EXECUTABLE )
    set(QT_AVAILABLE FALSE)
  endif()
  if( APPLE AND QT_QMAKE_EXECUTABLE MATCHES "/opt/homebrew/")
    message("You specified the homebrew version of Qt6, which does not work "
            "with BCI2000.\nPlease use a downloaded open-source version from www.qt.io"
    )
    set(QT_AVAILABLE FALSE)
  endif()

  macro( QT_WRAP_CPP )
    qt6_wrap_cpp( ${ARGN} )
  endmacro()
  macro( QT_WRAP_UI )
    qt6_wrap_ui( ${ARGN} )
  endmacro()
  macro( QT_ADD_RESOURCES )
    qt6_add_resources( ${ARGN} )
  endmacro()

endif()

if(QT_AVAILABLE)
  # report version
  GET_QT_VERSION( qtver_ )
  GET_FILENAME_COMPONENT( qtdir_ ${QT_QMAKE_EXECUTABLE} PATH )
  SET( QT_VERSION ${qtver_} )
  UTILS_CONFIG_STATUS( "Using Qt ${qtver_} (${qtdir_})" )

  if(QT_QMAKE_EXECUTABLE)
    foreach( var_ IN ITEMS
        QT_INSTALL_LIBS
        QT_INSTALL_LIBEXECS
        QT_INSTALL_PLUGINS
        QT_INSTALL_HEADERS )
      execute_process(
          COMMAND ${QT_QMAKE_EXECUTABLE} -query ${var_}
          OUTPUT_VARIABLE ${var_}
          OUTPUT_STRIP_TRAILING_WHITESPACE
      )
    endforeach()
    set(QT_INCLUDES ${QT_INSTALL_HEADERS} ${QT_INSTALL_HEADERS}/QtCore)
    foreach(mod_ ${QT_MODULES})
        list(APPEND QT_INCLUDES ${QT_INSTALL_HEADERS}/Qt${mod_})
    endforeach()
  endif()


  SET( QT_LIBRARIES "" )
  SET( QT_PLUGINS "" )
  FOREACH( module_ ${QT_MODULES} )
    SET( QT_LIBRARIES ${QT_LIBRARIES} "Qt${qtmajor}::${module_}" )
  ENDFOREACH()
  list( APPEND QT_SOURCES ${PROJECT_SRC_DIR}/shared/utils/Qt/QtMessageHandler.cpp )
  IF( WIN32 )
    SET( QT_LIBRARIES
        ${QT_LIBRARIES}
        imm32
        opengl32
        glu32
        ws2_32
        winmm
      )
  ELSEIF( NOT APPLE )
    SET( QT_LIBRARIES
       ${QT_LIBRARIES}
         X11
    )
  ENDIF()
  set( EXTLIB_OK TRUE )
  add_definitions(-DUSE_QT)
  set( INC_EXTLIB ${QT_INCLUDES} )
  set( SRC_EXTLIB ${QT_SOURCES} )
  if(WIN32)
    set( dlls
      "platforms/qwindows$<$<CONFIG:Debug>:d>.dll"
      # image format plugins common to Qt5 and Qt6
      "imageformats/qgif$<$<CONFIG:Debug>:d>.dll"
      "imageformats/qsvg$<$<CONFIG:Debug>:d>.dll"
      "imageformats/qico$<$<CONFIG:Debug>:d>.dll"
      "imageformats/qjpeg$<$<CONFIG:Debug>:d>.dll"
    )
    foreach( dll ${dlls} )
      list( APPEND SRC_EXTLIB COMPANION "${QT_INSTALL_PLUGINS}/${dll}=qtplugins/${dll}" )
    endforeach()
    foreach( module IN ITEMS ${QT_MODULES} Core )
      set( dll "Qt${qtmajor}${module}$<$<CONFIG:Debug>:d>.dll" )
      list( APPEND SRC_EXTLIB COMPANION "${QT_INSTALL_LIBEXECS}/${dll}" )
    endforeach()
    # debug build tries to load libEGL
    foreach( depdll IN ITEMS libEGL libGLESv2 )
      if(EXISTS "${QT_INSTALL_LIBEXECS}/${depdll}.dll")
        set( dll "${depdll}$<$<CONFIG:Debug>:d>.dll" )
        list( APPEND SRC_EXTLIB COMPANION "${QT_INSTALL_LIBEXECS}/${dll}" )
      endif()
    endforeach()
    # older versions of Qt depend on libicu dlls
    foreach( depdll IN ITEMS icuin51 icuuc51 icudt51 opengl32sw )
      set( dll "${depdll}.dll" )
      if(EXISTS "${QT_INSTALL_LIBEXECS}/${dll}")
        list( APPEND SRC_EXTLIB COMPANION "${QT_INSTALL_LIBEXECS}/${dll}" )
      endif()
    endforeach()
  elseif(APPLE)
    set(MACDEPLOYQT ${qtdir_}/macdeployqt)
  endif()

  foreach(item Core ${QT_MODULES})
    list(APPEND LIBS_EXTLIB Qt${qtmajor}${item})
  endforeach()
  set( LIBS_EXTLIB ${QT_LIBRARIES} )
  set( LIBDIR_EXTLIB ${QT_INSTALL_LIBEXECS} )
else()
  set( EXTLIB_OK FALSE )
endif()







