#! ../prog/BCI2000Shell
@cls & ..\prog\BCI2000Shell %0 %* #! && exit /b 0 || exit /b 1\n

#######################################################################################
## $Id: SerialWidgetDemoMPU6050.bat 7792 2024-01-17 18:30:04Z jhill $
## Description: BCI2000 startup Operator script for demonstrating functionality 
##              of the SerialWidget source module to communicate with the
##              ExampleSourceSketchMPU6050.ino sketch running on an attached
##              microcontroller, with the MPU6050 accelerometer/gyroscope attached
##              as specified in the sketch comments.
##
##              This batch file accepts up to 3 optional command-line arguments:
## 
##                       SerialWidgetDemoMPU6050.bat  SerialPort TTLInputPin TTLOutputPin
##
## $BEGIN_BCI2000_LICENSE$
##
## This file is part of BCI2000, a platform for real-time bio-signal research.
## [ Copyright (C) 2000-2023: BCI2000 team and many external contributors ]
##
## BCI2000 is free software: you can redistribute it and/or modify it under the
## terms of the GNU General Public License as published by the Free Software
## Foundation, either version 3 of the License, or (at your option) any later
## version.
##
## BCI2000 is distributed in the hope that it will be useful, but
##                         WITHOUT ANY WARRANTY
## - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
## A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License along with
## this program.  If not, see <http://www.gnu.org/licenses/>.
##
## $END_BCI2000_LICENSE$
#######################################################################################

system taskkill /F /FI "IMAGENAME eq SerialWidget.exe"             >NUL
system taskkill /F /FI "IMAGENAME eq SpectralSignalProcessing.exe" >NUL
system taskkill /F /FI "IMAGENAME eq DummyApplication.exe"         >NUL

change directory $BCI2000LAUNCHDIR
show window
set title ${extract file base $0}
reset system

set environment SERIALPORT COM13  # change this default value as needed...
if [ $1 ]; set environment SERIALPORT $1; end # ...or override it by supplying it as a command-line argument to the batch file

startup system localhost
start executable SerialWidget             --local --SerialPort=${SERIALPORT}  --PublishCommand=publish\n --StartCommand=mute=0\n --StopCommand=mute=1\n
start executable SpectralSignalProcessing --local
start executable DummyApplication         --local
wait for connected

# The following channel names and channel addresses are intended to refer to ExampleSourceSketchMPU6050
# but you can change them as needed  (first number is the number of pins/channels, after that it's pin/channel names or addresses)
set parameter Source intlist ChannelNames=    7   Ax Ay Az Gx Gy Gz TTL
set parameter Source intlist SourceChPins=    7    0  1  2  3  4  5  6 

                                                # NB: for the MPU6050, stick to 625Hz which is its native rate.
set parameter SamplingRate                       625  # verify that the widget can in fact achieve these parameters via the Arduino IDE's
set parameter SampleBlockSize                     20  # Serial Monitor, using the method described on the SerialWidgetADC wiki page

set parameter SourceMin                        -2000uV
set parameter SourceMax                         2000uV

set parameter VisualizeTiming                      1
set parameter VisualizeSource                      1
set parameter VisualizeSpectralEstimator           1

set parameter WindowLength                         1s
set parameter FirstBinCenter                       1Hz
set parameter LastBinCenter                      300Hz
set parameter BinWidth                             1Hz
set parameter SpectralEstimator                    2    # FFT

set parameter SpatialFilterType                    0
set parameter Filtering matrix Classifier=  1 4    1 1 1 0
set parameter Filtering matrix Expressions= 1 1    0
set parameter Filtering list   Adaptation=    1    0

# And now the same parameters/mechanisms as for SerialInterfaceDemo.bat <-> TTLExampleSketch.ino:
# this allows a TTL input and TTL output pin to operate independently of the main signal stream
# (the functionality of ExampleSourceSketch.ino is a superset of TTLExampleSketch.ino)
if [ $2 ]; set parameter TTLInputPin  $2; end  # optional second command-line argument to the batch file: reconfigures sketch's TTLInputPin setting
if [ $3 ]; set parameter TTLOutputPin $3; end  # optional third command-line argument to the batch file: reconfigures sketch's TTLOutputPin setting
set parameter Source:Serial%20Port matrix SerialOutputs= 2 { When%20this%20Expression%20becomes%20true, send%20these%20bytes: }    StimTrigger!=0 output=1\n    StimTrigger==0 output=0\n
add state StimTrigger  1 0 0 0
set button 1 "StimTrigger On/Off"     "set state StimTrigger %24{evaluate expression 1-StimTrigger}"
if [ ${exists event TTLInput} ];          visualize watch TTLInput;          end
if [ ${exists event PulseDurationMsec} ]; visualize watch PulseDurationMsec; end

setconfig
set state Running 1
