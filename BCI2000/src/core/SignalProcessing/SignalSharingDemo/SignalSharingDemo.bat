#! ../prog/BCI2000Shell
@cls & ..\prog\BCI2000Shell %0 %* #! && exit /b 0 || exit /b 1\n
#######################################################################################
## $Id: SignalSharingDemo.bat 8297 2024-08-01 16:15:30Z mellinger $
## Description: BCI2000 startup Operator module script. For an Operator scripting
##   reference, see
##   https://www.bci2000.org/mediawiki/index.php/User_Reference:Operator_Module_Scripting
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
Change directory $BCI2000LAUNCHDIR
Show window; Set title ${Extract file base $0}
Reset system

Set variable CLIENTAPP ../demos/SignalSharingDemoClientApp
Set variable CLIENTPID ${Create process $CLIENTAPP -qwindowgeometry 320x320+10+320 }
Set script OnShutdown "Terminate process $CLIENTPID"
Set script OnStart "Show process $CLIENTPID"
Set script OnResume "Show process $CLIENTPID"

Startup system localhost
Start executable SignalGenerator --local
Start executable DummySignalProcessing --local
Start executable DummyApplication --local
Wait for Connected
Load parameterfile "../parms/examples/CursorTask_SignalGenerator.prm"
Set Parameter SamplingRate 256Hz
Set Parameter SampleBlockSize 32
Set Parameter ShareTransmissionFilter localhost:1879

Set config
