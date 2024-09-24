###########################################################################
## $Id: IncludeExtension.cmake 7883 2024-03-01 20:59:08Z mellinger $
## Authors: jezhill@gmail.com

if (NOT EXTENSIONS_WEBCAMLOGGER) # avoid linking issues

list(APPEND BCI2000_SIGSRC_INCLUDE_EXTLIB
  FFMPEG
  LIBSCHRIFT
)
list(APPEND BCI2000_SIGSRC_RESOURCES
  FreeMonoBold_ttf=${BCI2000_EXTENSION_DIR}/resources/FreeMonoBold.ttf
)
list( APPEND BCI2000_SIGSRC_FILES
   ${BCI2000_EXTENSION_DIR}/WebcamLogger.cpp
   ${BCI2000_EXTENSION_DIR}/WebcamController.cpp
   ${BCI2000_EXTENSION_DIR}/WebcamClient.h
   ${BCI2000_EXTENSION_DIR}/WebcamThread.cpp
   ${BCI2000_EXTENSION_DIR}/AVSource.h
   ${BCI2000_EXTENSION_DIR}/AVSourceEnumerator.cpp
   ${BCI2000_EXTENSION_DIR}/ffmpeg/AVSourceLibAVDevice.cpp
   ${BCI2000_EXTENSION_DIR}/ffmpeg/AVSourceEnumeratorLibAVDevice.cpp
   ${BCI2000_EXTENSION_DIR}/ffmpeg/SaveToFile.cpp
   ${BCI2000_EXTENSION_DIR}/AudioMediaType.cpp
   ${BCI2000_EXTENSION_DIR}/VideoMediaType.cpp   
   ${BCI2000_SRC_DIR}/shared/utils/Rendering/TextRenderer.cpp
   ${BCI2000_SRC_DIR}/shared/utils/AV/AVEncoder.cpp
   ${BCI2000_SRC_DIR}/shared/utils/AV/AVLog.cpp
   ${BCI2000_SRC_DIR}/shared/utils/AV/AVError.cpp
)

if (WIN32)
   list(APPEND BCI2000_SIGSRC_FILES
      ${BCI2000_SRC_DIR}/extlib/com/ComPtr.h
      ${BCI2000_SRC_DIR}/extlib/com/ComStrings.cpp
      ${BCI2000_EXTENSION_DIR}/win32mf/MFInit.h
      ${BCI2000_EXTENSION_DIR}/win32mf/AVSourceMF.cpp
      ${BCI2000_EXTENSION_DIR}/win32mf/AVSourceEnumeratorMF.cpp
   )
   list(APPEND BCI2000_SIGSRC_LIBS
      mfplat.lib mf.lib mfreadwrite.lib mfuuid.lib
   )
elseif (APPLE)
   list(APPEND BCI2000_SIGSRC_FILES
     ${BCI2000_EXTENSION_DIR}/ffmpeg/AVFoundationHelper.mm
  )
  list(APPEND BCI2000_SIGSRC_LIBS
    "-framework Foundation"
    "-framework CoreMedia"
    "-framework AVFoundation"
  )
else()
   list(APPEND BCI2000_SIGSRC_FILES
     ${BCI2000_EXTENSION_DIR}/ffmpeg/Video4LinuxHelper.cpp
  )
endif()

endif()


