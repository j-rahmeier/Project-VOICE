#! ../prog/BCI2000Shell
@cls & ..\prog\BCI2000Shell %0 %* #! && exit /b 0 || exit /b 1\n

#######################################################################################
## $Id: SerialWidgetDemo.bat 7792 2024-01-17 18:30:04Z jhill $
## Description: BCI2000 startup Operator script for demonstrating functionality 
##              of the SerialWidget source module to communicate with the
##              ExampleSourceSketch.ino sketch running on an attached microcontroller
##
##              This batch file accepts up to 3 optional command-line arguments:
## 
##                       SerialWidgetDemo.bat  SerialPort TTLInputPin TTLOutputPin
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

system taskkill /F /FI "IMAGENAME eq SerialWidget.exe"          >NUL
system taskkill /F /FI "IMAGENAME eq DummySignalProcessing.exe" >NUL
system taskkill /F /FI "IMAGENAME eq DummyApplication.exe"      >NUL

change directory $BCI2000LAUNCHDIR
show window
set title ${extract file base $0}
reset system

set environment SERIALPORT COM4:baud=115200,dtr=on  # change this default value as needed...
if [ $1 ]; set environment SERIALPORT $1; end # ...or override it by supplying it as a command-line argument to the batch file

#add event TTLInput 1 0 0 0 # commented out because, with --PublishCommand, we can get ExampleSourceSketch.ino to declare this event itself

startup system localhost
start executable SerialWidget          --local --SerialPort=${SERIALPORT}  --PublishCommand=publish\n
start executable DummySignalProcessing --local
start executable DummyApplication      --local
wait for connected

set parameter StartCommand mute=0\n
set parameter StopCommand  mute=1\n

set parameter SamplingRate    1000  # verify that the widget can in fact achieve these parameters via the Arduino IDE's
set parameter SampleBlockSize   40  # Serial Monitor, using the method described on the SerialWidgetADC wiki page

set parameter Source intlist SourceChPins=   2  26 27   # change this to whichever pins you want (first number is the number of pins, after that it's pin addresses)

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
