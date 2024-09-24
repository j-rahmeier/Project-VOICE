#! ../prog/BCI2000Shell
@cls & ..\prog\BCI2000Shell %0 %* #! && exit /b 0 || exit /b 1\n

#######################################################################################
## $Id: SerialInterfaceDemo.bat 7532 2023-08-16 03:20:24Z jhill $
## Description: BCI2000 startup Operator script for demonstrating functionality 
##              of the SerialInterface extension to communicate with the
##              TTLExampleSketch.ino sketch running on an attached microcontroller
##
##              This batch file accepts up to 3 optional command-line arguments:
## 
##                       SerialInterfaceDemo.bat  SerialPort TTLInputPin TTLOutputPin
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

change directory $BCI2000LAUNCHDIR
show window
set title ${extract file base $0}
reset system

set environment SERIALPORT COM4:dtr=on  # change this default value as needed...
if [ $1 ]; set environment SERIALPORT $1; end # ...or override it by supplying it as a command-line argument to the batch file

#add event TTLInput 1 0 0 0 # commented out because, with --PublishCommand, we can get TTLExampleSketch.ino to declare this itself

startup system localhost
start executable SignalGenerator       --local --SerialPort=${SERIALPORT}  --PublishCommand=publish\n
start executable DummySignalProcessing --local
start executable DummyApplication      --local

wait for connected

set parameter SamplingRate    1000
set parameter SampleBlockSize   40

set parameter Source:Serial%20Port matrix SerialOutputs= 2 { When%20this%20Expression%20becomes%20true, send%20these%20bytes: }    StimTrigger!=0 output=1\n    StimTrigger==0 output=0\n
set parameter StartCommand mute=0\n
set parameter StopCommand  mute=1\n
if [ $2 ]; set parameter TTLInputPin  $2; end  # optional second command-line argument to the batch file: reconfigures sketch's TTLInputPin setting
if [ $3 ]; set parameter TTLOutputPin $3; end  # optional third command-line argument to the batch file: reconfigures sketch's TTLOutputPin setting


add state StimTrigger  1 0 0 0

set button 1 "StimTrigger On/Off"     "set state StimTrigger %24{evaluate expression 1-StimTrigger}"

visualize watch TTLInput
if [ ${exists event PulseDurationMsec} ]
	visualize watch PulseDurationMsec
end

setconfig
set state Running 1
