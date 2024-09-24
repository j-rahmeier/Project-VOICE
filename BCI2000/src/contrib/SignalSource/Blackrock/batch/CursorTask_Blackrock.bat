#! ../prog/BCI2000Shell
@cls & ..\prog\BCI2000Shell %0 %* #! && exit /b 0 || exit /b 1\n
#######################################################################################
## $Id: CursorTask_Blackrock.bat 8056 2024-04-22 19:54:35Z mellinger $
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
Startup system localhost
Start executable Blackrock --local
Start executable SpectralSignalProcessing --local
Start executable CursorTask --local
Wait for Connected
Load Parameterfile "../parms/examples/CursorTask_SignalGenerator.prm"

# The Blackrock module will autoconfigure
# based on the channels that exist in the samplegroup
# chosen by the SamplingRate parameter.

# Set a number of channels to record @ 1000 Hz
# or change the SamplingRate parameter to match
# your desired SamplingRate and the rest of the 
# configuration will be done for you.

Set Parameter SamplingRate 1000
Set Parameter SourceCh auto
Set Parameter SampleBlockSize auto
Set Parameter "% list ChannelNames= 1 auto"
Set Parameter SourceChOffset auto
Set Parameter SourceChGain auto
