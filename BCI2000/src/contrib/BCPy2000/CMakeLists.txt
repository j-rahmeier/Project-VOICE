###########################################################################
## $Id: CMakeLists.txt 8326 2024-08-23 15:48:06Z nluczak $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for BCI2000 and CMake

IF( WIN32 )

# Set this directory name
SET( DIR_NAME "${DIR_NAME}BCPy2000/" )

OPTION( BUILD_BCPY2000 "Build BCPy2000 modules" OFF )

# Recurse down into all project subdirectories
ADD_SUBDIRECTORY( foundation )
file(COPY demo/batch/FindPortablePython.bat DESTINATION ${PROJECT_BUILD_ROOT}/batch/)
file(COPY demo/batch/PythonDemo_Template.bat DESTINATION ${PROJECT_BUILD_ROOT}/batch/)
file(COPY demo/batch/PythonDemo_Triangle.bat DESTINATION ${PROJECT_BUILD_ROOT}/batch/)
file(COPY demo/batch/PythonRipple.bat DESTINATION ${PROJECT_BUILD_ROOT}/batch/)
file(COPY demo/batch/Ripple_DummyApplication_macrohires_64ch.bat DESTINATION ${PROJECT_BUILD_ROOT}/batch/)
file(COPY demo/parms/PythonDemo1_Triangle.prm DESTINATION ${PROJECT_BUILD_ROOT}/parms/)
file(COPY demo/parms/PythonDemo2_Playback.prm DESTINATION ${PROJECT_BUILD_ROOT}/parms/)
file(COPY demo/parms/RipplePyADC_macrohires_64ch.prm DESTINATION ${PROJECT_BUILD_ROOT}/parms/)
file(COPY demo/parms/ChannelList.prm DESTINATION ${PROJECT_BUILD_ROOT}/parms/)
file(MAKE_DIRECTORY ${PROJECT_BUILD_ROOT}/python/)
file(COPY demo/python/BciApplication.py DESTINATION ${PROJECT_BUILD_ROOT}/python/)
file(COPY demo/python/BciSignalProcessing.py DESTINATION ${PROJECT_BUILD_ROOT}/python/)
file(COPY demo/python/BciSource.py DESTINATION ${PROJECT_BUILD_ROOT}/python/)
file(COPY demo/python/feedback-drums.WAV DESTINATION ${PROJECT_BUILD_ROOT}/python/)
file(COPY demo/python/feedback-piano.WAV DESTINATION ${PROJECT_BUILD_ROOT}/python/)
file(COPY demo/python/feedback-strings.WAV DESTINATION ${PROJECT_BUILD_ROOT}/python/)
file(COPY demo/python/TrefoilSource.py DESTINATION ${PROJECT_BUILD_ROOT}/python/)
file(COPY demo/python/TriangleApplication.py DESTINATION ${PROJECT_BUILD_ROOT}/python/)
file(COPY demo/python/RipplePyADC.py DESTINATION ${PROJECT_BUILD_ROOT}/python/)

ENDIF( WIN32 )
