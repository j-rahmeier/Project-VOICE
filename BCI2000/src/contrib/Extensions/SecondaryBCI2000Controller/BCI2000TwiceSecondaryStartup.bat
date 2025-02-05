#! ../prog/BCI2000Shell
@cls & ..\prog\BCI2000Shell %0 %* #! && exit /b 0 || exit /b 1\n
#######################################################################################
## $Id: BCI2000TwiceSecondaryStartup.bat 8335 2024-09-02 16:39:42Z mellinger $
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
Show window; #Set title "Secondary Instance"
Reset system
Startup system localhost Source:5000 SignalProcessing:5001 Application:5002
Start executable SignalGenerator localhost:5000 --local --AllowMultipleInstances --IsSecondaryBCI2000=1
Start executable DummySignalProcessing localhost:5001 --AllowMultipleInstances --local
Start executable DummyApplication localhost:5002 --AllowMultipleInstances --local
Wait for Connected
Load parameterfile "../parms/examples/StimulusPresentation_SignalGenerator.prm"
Set parameter OffsetMultiplier 0
Hide window
