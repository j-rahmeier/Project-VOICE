###########################################################################
## $Id: CMakeLists.txt 8006 2024-04-08 14:24:29Z mellinger $
## Authors: mellinger@neurotechcenter.org

set (NAME WebcamRecorder)

#option (USE_WEBCAMRECORDER_LIBAV "Use ffmpeg/libav version of WebcamRecorder also in Windows" OFF)

if (WIN32)# AND NOT USE_WEBCAMRECORDER_LIBAV)

    utils_include(frameworks/Core)
    bci2000_include(COM)
    utils_set_appicon(video-camera)
    bci2000_add_target(
        INFO Helper
        WINAPP ${NAME}
        win32mf/WebcamRecorder.cpp
        win32mf/GrabberTransform.cpp
        win32mf/SaveToFileTransform.cpp
        win32mf/WebcamPipeline.cpp
        win32mf/AudioMediaType.cpp
        win32mf/VideoMediaType.cpp
        win32mf/MFInit.h
        ${PROJECT_SRC_DIR}/shared/utils/Win32/Window.cpp
        ${PROJECT_SRC_DIR}/shared/utils/Win32/EventLoop.cpp
        ${PROJECT_SRC_DIR}/shared/utils/IniFile.cpp
    )
    if( NOT failed )
        target_link_libraries(${NAME} mf mfplat mfreadwrite mfuuid strmiids propsys)
        set_property( TARGET ${NAME} PROPERTY FOLDER "Contrib/Extensions/WebcamLogger" )
        bci2000_add_to_inventory( Helper ${NAME} )
    endif()

else ()

    utils_include(frameworks/Core)
    utils_set_appicon(video-camera)
    bci2000_use(ffmpeg)
    bci2000_add_target(
        INFO Helper
        QTAPP ${NAME}
        ffmpeg/WebcamRecorder.cpp
        ffmpeg/WebcamPipeline.cpp
        ffmpeg/VideoWidget.cpp
        ffmpeg/VideoMediaType.cpp
        ffmpeg/AudioMediaType.cpp
        ${PROJECT_SRC_DIR}/shared/utils/IniFile.cpp
        ${PROJECT_SRC_DIR}/shared/utils/Qt/QtMain.cpp
    )
    if( NOT failed )
        set_property( TARGET ${NAME} PROPERTY FOLDER "Contrib/Extensions/WebcamLogger" )
        bci2000_add_to_inventory( Helper ${NAME} )
    endif()

endif ()
