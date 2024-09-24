#! ../prog/BCI2000Shell
@cls & ..\prog\BCI2000Shell %0 %* #! && exit /b 0 || exit /b 1\n


execute script FindPortablePython.bat  # this is necessary so that BCI2000 can find Python


change directory $BCI2000LAUNCHDIR
show window; set title ${Extract file base $0}
reset system
startup system localhost

start executable PythonSource             --local --PythonSrcClassFile=RipplePyADC.py --PythonSrcShell=0 --PythonSrcLog=SourceLogger.txt
start executable DummySignalProcessing    --local 
start executable DummyApplication         --local 

wait for connected 600



load parameterfile "../parms/RipplePyADC_macrohires_64ch.prm"
load parameterfile "../parms/ChannelList.prm"