#   $Id: BciSourceTemplate.py 4575 2023-12-12 23:16:50Z dmehta $
#   
#   This file is a BCPy2000 demo file, which illustrates the capabilities
#   of the BCPy2000 framework.
# 
#   Copyright (C) 2007-10  Jeremy Hill
#   
#   bcpy2000@bci2000.org
#   
#   This program is free software: you can redistribute it
#   and/or modify it under the terms of the GNU General Public License
#   as published by the Free Software Foundation, either version 3 of
#   the License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
import numpy

#################################################################
#################################################################

class BciSource(BciGenericSource):	
	
	#############################################################
	
	def Construct(self):
		
		self.define_param(
			# Here is where you define additional parameters to be configured in your source module.
			# Format should follow the same conventions as BCI2000, i.e. "Section:Name variableName= ...", etc.
			# You can create your own tab or use existing tabs.

			"Source:Signal%20Properties int SourceCh= auto auto 1 % // number of digitized and stored channels",

    		"Source:Signal%20Properties int SampleBlockSize= auto 1 % // number of samples transmitted at a time",

    		"Source:Signal%20Properties float SamplingRate= auto auto 0.0 % // sample rate",

    		"Source:Signal%20Properties list SourceChGain= 1 auto // physical units per raw A/D unit",

    		"Source:Signal%20Properties list SourceChOffset= 1 auto // raw A/D offset to subtract, typically 0",

    		"Source:Signal%20Properties list ChannelNames= 1 auto // names of amplifier channels",
		)

		self.define_state(
			# Here is where you define additional states

			"DigitalInput 16 0"
		)

	#############################################################
	
	def Halt(self):
		# The Halt function is called before any reconfiguration fo the system takes place. 
		# If there are any asynchronous operations like playing a media file or executing threads, Halt should stop all such operations. 
		# As a result, please implement any function calls to stop asynchronous operations here, otherwise the module may become unresponsive.
		pass

	#############################################################
	
	def Preflight(self, inprops):
		# The Preflight function checks whether the preconditions are met for a successful operation. 
		# Here, you can add test cases to verify that the parameters that have been configured fall within proper bounds.
		# Whenever the parameters are changed and re-applied, i.e. "Set Config" is pressed, this function is run. 
		# If the parameters pass all test cases, the program will move on to the Initialize function
		pass

	#############################################################
	
	def Initialize(self, indim, outdim):
		# Initialize is called after a successful Preflight check. 
		# Usually, this is the place that you create variables based on user parameters. 
		# This is also where you run tasks or functions that need to be ran after parameters are configured.
		# Here, you set any variables for use in later functions using "self.'variable_name'"
		# The "self" is necessary to ensure access to variables outside the context of the function. Placeholder variables do not need "self"
		self.sound = 'meow'
		pass

	#############################################################
	
	def StartRun(self):
		# StartRun is called when the system enters the running state. 
		# StartRun is specifically for functions or tasks that need to be run whenever the user clicks "Run" or "Resume".
		# This is not the place for functions or tasks that need to be run whenever parameters change; that is what Initialize is for. 
		# Use this for anything you want called when the experiment run starts or resumes.
		pass
		
	#############################################################
	
	def Process(self, sig):
		# Process is called once for each block of data, as designated by the Sample Block Size and Sample Rate. 
		# It receives an input in its first argument, sig, and modifies sig before returning it as output. 
		# This is where you interpret a data stream, modify it, and more before returning it. 
		# The incoming signal is available as a numpy.matrix either as the input argument in_signal or as the instance attribute self.in_signal. 
		# The output may be returned from the function, or placed into self.out_signal.
		# For the source module, the input signal is a numpy.matrix (one row per channel, one column per sample as always) containing identical rows. 
		# The content of the each row is the sample number since the beginning of the run, translated into seconds. 
		# This makes it particularly easy to generate sine waves -
        #       return 100.0 * numpy.sin(2.0 * numpy.pi * in_signal)
        # - or any other function of time.
		# Make sure that the dimensions of the output match the dimensions of the input. 
		# One way to do this is to 1-out sig and then use np.multiply with a matrix of acquired data values before return sig.
		return numpy.sin(2.0 * 5.0 * numpy.pi * sig)
		
	#############################################################
	
	def StopRun(self):
		# StopRun is called when the system leaves the running state and enters the suspended state, usually after the user clicks "Suspend" 
		# StopRun is the only function which can change a parameter value. This change will propagate to the other modules. 
		# Use StopRun for anything you want called when the experiment run is initially paused. For repeated tasks, use Resting.
		pass

	#############################################################
	
	def Resting(self):
		# Anything here is called when the system is in a suspended state after StopRun is called. 
		# Inside source modules, this is called repeatedly, but in other modules it is called only once after the system is suspended. 
		# Use this for any repeated tasks you want called when the experiment run is paused. For one time tasks, use StopRun.
		pass

	#############################################################
	
	def Destruct(self):
		# This is where you free all resources acquired from the Contructor and Initialize. 
		# If your Halt function is not empty, you must call Halt() here. 
		# Otherwise, the freeing of resources is done automatically, so you should leave this blank.
		pass
		
#################################################################
#################################################################
