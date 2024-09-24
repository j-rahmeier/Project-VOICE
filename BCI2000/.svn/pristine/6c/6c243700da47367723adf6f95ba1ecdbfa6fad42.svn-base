#   $Id: RipplePyADC.py 2900 2023-12-12 15:44:21Z dmehta $
#   
#   This file is a BCPy2000 source module for Ripple devices.
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
import numpy as np
import math
import time
import xipppy as xp 
import os

#################################################################
#################################################################

streamInfo = {
    1: ('raw', 30),
    2: ('lfp', 1),
    3: ('hires', 2),
    4: ('hifreq', 7.5),
    5: ('emg', 2), 
    6: ('status', 2),
    7: ('raw', 30)
}

#################################################################

class BciSource(BciGenericSource):
    
    #############################################################

    def Construct(self):
        self.define_param(
            "Source:Signal%20Properties   int    DataStream=  1     1     1   7 // type of data stream to acquire: 1 macro, 2 lfp, 3 hires, 4 hifreq, 5 emg, 6 status, 7 micro (enumeration)",
            "Source:Signal%20Properties intlist   SpecificChannels= [1 2 3 4] 0 1 2 3    0    0    15 // list of channels to acquire data from",
            "Source:Digital%20Input int UsingDIOFrontEnd= 0 0 0 1 // Check if you are using a Digital I/O front end and would like to use it (boolean)",

            "Ripple:Stimulation int EnableStim= 0 0 0 1 // Check if you would like to stimulate (boolean)",
            "Ripple:Stimulation matrix Segments= "
                "{ Length[33.3us] Amplitude[uA] Polarity Enable%20Current }"
                "{ Segment%201 Segment%202 Segment%203 }"
                "60 30 60 "
                "100 0 100 "
                "1 1 -1 "
                "1 0 1 ",
            "Ripple:Stimulation matrix Sequences= "
                "{ Segments%20Order Period Repetitions }"
                "{ Sequence%201 Sequence%202 }"
                "{ intlist 3 1 2 3 } { intlist 5 3 1 2 3 1 } "
                "3000 3000 "
                "30 10 ",
            "Ripple:Stimulation matrix StimulationTriggers= "
                "{ Expression Sequence%20ID(s) Electrode(s) }"
                "{ 1 2 3 }"
                "StimulusCode==1 StimulusCode==2 StimulusCode==2 "
                "1 1 2 "
                "{ intlist 1 0 } { intlist 2 0 1 } { intlist 2 2 3 }"
        )
        self.define_state(
            "RippleTimestamp   32 0 0 0",
        )
    #####################################################################
  
    def Preflight(self, sigprops):

        # sanity checks for stimulation parameters
        if self.params['EnableStim'] == '1':
            # STIMULATION SEGMENT PARAMETERS

            # check that length for each segment is minimum 1 and maximum 65535
            if any((length <= 0 or length > 65535) for length in map(int,self.params['Segments'][0])):
                print('Error! One or more of your stimulation segment lengths is not between 1 and 65535!')
            # check that amplitude for each segment is minimum 1 and maximum 500
            if any((amp < 0 or amp > 500) for amp in map(int,self.params['Segments'][1])):
                print('Error! One or more of your stimulation segment amplitudes is not between 0 and 500!')
            # check that the polarity for each segment is either 1 or -1. 1 means it is cathodic, -1 means it is anodic
            if any((polarity != 1 and polarity != -1) for polarity in map(int,self.params['Segments'][2])):
                print('Error! One or more of your stimulation segment polarities is not between 1 or -1!')
            # check that the current enable property for each segment is either 0 or 1. 1 means current is enabled for this segment, 0 means it is disabled ( interphase )
            if any((current != 0 and current != 1) for current in map(int,self.params['Segments'][3])):
                print('Error! One or more of your stimulation segments current enable property is not 0 or 1!')

            # STIMULATION SEQUENCE PARAMETERS

            # check that the values for the segments list are between 1 and 99. CHANGE THIS TO CHECK THAT ALL THE IDS ARE VALID SEGMENTS
            numSegments = len(self.params['Segments'][0])
            for segmentList in self.params['Sequences'][0]:
                if any(segID > numSegments for segID in map(int, segmentList)):
                    print('Error! One or more of your sequences segment lists contains an invalid ID!')

            # check that the sequence period is minimum 1
            if any(period < 1 for period in map(int, self.params['Sequences'][1])):
                print('Error! One or more of your sequence periods is less than 1!')

            # check that the number of repeats is minimum 1 and maximum 4095
            if any((repeat < 1 or repeat > 4095) for repeat in map(int, self.params['Sequences'][2])):
                print('Error! One or more sequences repetition value is not between 1 and 4095!')


            # check that sequence ids are valid
            numSequences = len(self.params['Sequences'][0])

            if any((seqID > numSequences) for seqID in map(int, self.params['StimulationTriggers'][1])):
                print('Error! One or more of your stimulation triggers has an invalid sequence ID!')

            # checking the electrode list
            for electrodeList in self.params['StimulationTriggers'][2]:
                elecs = map(int, electrodeList)
                # check that all electrode choices are minimum 0 and maximum 511
                if any((elec < 0 or elec > 511) for elec in elecs):
                    print('Error! One or more of your stimulation triggers electrode lists contains an invalid electrode! Electrodes must be between 0 and 511!')

                # check that there are no duplicate electrodes
                if len(set(electrodeList)) != len(electrodeList):
                    print('Error! One or more of your stimulation triggers electrode lists contains a duplicate electrode!')

            # MATH CHECKS

            # check that the stimulation sequence can mathematically exist, i.e., length of full segment x repeats <= period length
            sequences = self.params['Sequences']
            segLengths = list(map(int, self.params['Segments'][0]))
            for i in range(len(sequences[0])):
                segList = list(map(int, sequences[0][i]))
                period = int(sequences[1][i]) # in milliseconds
                repeats = int(sequences[2][i]) # number of repeats in the period
                projectedLength = period/repeats # theoretical length of each repetition (overall period length/number of repeats)
                totalLength = sum([segLengths[segID-1]/30 for segID in map(int, segList)]) # length is in 33.33 microsecond units, i.e. length of 30 = 1 millisecond
                if projectedLength < totalLength:
                    print('Error! Sequence ' + str(i+1) + ' has a shorter projected stimulation length per repetition than actual stimulation length per repetition. Please increase the period, decrease the repetitions or remake your stimulation sequence!')

            # check that the stimulation train can mathematically exist, i.e., length of full sequence x repeats <= train duration

    #################################################################
    
    def Initialize(self, indim, outdim):
        
        self.gElec = [int(electrode) for electrode in self.params['SpecificChannels']]
        self.gNSamples = int(self.params['SampleBlockSize']) # number of samples to get per call, 
        self.stream, self.gSampleFreq = streamInfo[int(self.params['DataStream'])] # set to 2 because hires sample frequency is 2 kHz
        self.gDuration = self.gNSamples * 1/self.gSampleFreq # sets number of samples based on buffer duration and sample frequency
        self.waitTime = max(self.gDuration/100, 0.2) #sleep time for first data acquisition
        
        self.NIP_clock = 30000 #HZ
        self.NIP_bufferSize_sec = 5
        self.timeToWaitFor_inNIPcycles = ((self.gDuration  + 1) * self.NIP_clock)

        self.gTimeStamp = 0 #internal timestamp
        self.currentTime = 0 # xp.time of current process block
        self.prevTime = 0 # xp.time of previous process block

        self.digin = self.params['UsingDIOFrontEnd']
 
            
        funcName = f'cont_{self.stream}'
        self.dataFunc = getattr(xp, funcName)
        
        xp._close()
        xp._open(use_tcp=True)
        
        for elec in self.gElec:
            if self.stream == "raw":
                xp.signal_set(elec, self.stream, True)
        
        time.sleep(0.2)

    #################################################################

    def Process(self, sig):
        sig = (sig * 0) + 1 # in uV
        
        if self.currentTime == 0: # if first iteration
            self.currentTime = int(xp.time() - self.gDuration * 30) # first timestamp to get data from buffer
            self.initialTime = self.currentTime
            time.sleep(self.waitTime)
        else: # if not first iteration
            self.currentTime = int(self.currentTime + self.gDuration * 30) # update our timestamp 

        [data, timestamp] = self.dataFunc(self.gNSamples, self.gElec, self.currentTime) # acquire data
        # [data, timestamp] = self.dataFunc(self.gNSamples, self.gElec) # acquire data
        required_elements = int(self.params['SourceCh']) * int(self.params['SampleBlockSize'])
        if len(data) % int(self.params['SourceCh']) != 0:
            pad = required_elements - len(data)
            data = np.pad(data, (0, pad), mode='constant')

        if len(data) < required_elements and len(data) % int(self.params['SourceCh']) == 0:
            pad = required_elements - len(data)
            data = np.pad(data, (0, pad), mode='constant')

        data_arrays = np.array(data).reshape(int(self.params['SourceCh']), self.gNSamples)
      
        newTS = timestamp - self.initialTime
        if newTS >= 2**32:
            self.initialTime = self.initialTime + 2**32
            newTS = timestamp - self.initialTime
        self.states['RippleTimestamp'] = newTS
        result = np.multiply(sig, data_arrays)
        return result

#################################################################
#################################################################
