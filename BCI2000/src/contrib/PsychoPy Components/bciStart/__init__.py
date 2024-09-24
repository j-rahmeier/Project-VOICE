#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Part of the PsychoPy library
# Copyright (C) 2018 Jonathan Peirce
# Distributed under the terms of the GNU General Public License (GPL).

from __future__ import absolute_import, print_function

from builtins import str
from os import path
from math import log2
from psychopy.experiment.components import BaseComponent, Param, _translate

# the absolute path to the folder containing this path
thisFolder = path.abspath(path.dirname(__file__))
iconFile = path.join(thisFolder, 'bci_start.png')
tooltip = _translate('BciConnect: Connect to BCI Remote')
_localized = { 'Path': _translate('Path'),
               'States': _translate('States'),
               'Source Module': _translate('Source Module')
             }


class BciConnectComponent(BaseComponent):
  # an attribute of the class, determines the section in the components panel
  categories = ['Custom']
  """An event class for connecting with the Remote BCI2000 Operator into Builder experiments"""

  def __init__(self, exp, parentName, name='BciConnect', path="", module="", states = "",abc=''):
    super(BciConnectComponent, self).__init__(exp, parentName, name)
    self.type = 'BciConnect'
    self.targets = ['PsychoPy']
    self.url = "http://www.psychopy.org/builder/components/code.html" ##need to update it
    # params
    # want a copy, else codeParamNames list gets mutated
    self.order = ['name','module','path','states']
    #Define parameters    
    msg = _translate("Path to BCI2000 Remote Operator")
    self.params['Path'] = Param(
            path, valType='str', allowedTypes=[],
            updates='constant', allowedUpdates=[],
            hint=msg,
            label=_localized['Path'])
    #define which startup module are used
    msg = _translate("Specify which source module to use")
    self.params['Source Module'] = Param(
            module, valType='str', allowedTypes=[],
            updates='constant', allowedUpdates=[],
            hint=msg,
            label=_localized['Source Module'])
    #Define parameters    
    msg = _translate("States to add to the BCI2000 - Format - list of - 'StateName defaultValue maxValue'")
    self.params['States'] = Param(
            states, valType='str', allowedTypes=[],
            updates='constant', allowedUpdates=[],
            hint=msg,
            label=_localized['States'])
    #Delete the inherited parameters because we have no use for them
    for p in ('startType', 'startVal', 'startEstim', 'stopVal', 'stopType', 'durationEstim','saveStartStop', 'syncScreenRefresh'):
      if p in self.params:
        del self.params[p]
             

  def writeInitCode(self, buff):
    status = True
    statesToAdd = []
    addStateCode = ""
    try:
      #Check if there are any states to add. Remove the first and last character that contains the " ' ". Split string with ',' to get separate states. 
      #Calculate the number of bits required to store the state using the maximum value attainable by state and format the string into BCI required format
      #BCI format - 'StateName BitsRequired InitialValue'
        states  = str(self.params['States'])[1:-1]
        if(len(states)):
        	statesExtracted = [s.strip() for s in states.split(',')]
        	statesToAdd  = [ s.split(' ')[0]+' '+str(int(log2(int(s.split(' ')[2]))+1))+' '+s.split(' ')[1] for s in statesExtracted]
         
    except Exception as e:
      raise e
      status = False
    if(status):
      #Start connection with BCI
      startCodePart1 = ("#importing BCI2000Remote\n"
                      "sys.path.append(%s)\n"
                      "import BCI2000Remote\n"
                      "bci = BCI2000Remote.BCI2000Remote()\n"
                      "print('Operator path:', bci.OperatorPath)\n"
                      "bci.WindowVisible = True\n"
                      "bci.WindowTitle = 'Python controlled'\n"
                      "bci.SubjectID = 'pysub'\n"
                      "bci.Connect()\n"
                      "bci.Execute('cd ${BCI2000LAUNCHDIR}')\n"
                    )
      #Startup the module and set bci config
      startCodePart2 = ("bci.StartupModules((%s, 'DummySignalprocessing', 'DummyApplication'))\n"
                        #bci.LoadParametersRemote(
                        #        "../parms/examples/StimulusPresentation_SignalGenerator.prm")
                      "bci.Execute('Wait for Connected')\n"
                      "bci.SetConfig()\n"
                      "print('SubjectName parameter:', bci.GetParameter('SubjectName'))\n"
                      )
      #Format states with the correct bci command to add new states
      if(len(statesToAdd)):
        addStateCode = "')\nbci.Execute('ADD STATE ".join(statesToAdd)
        addStateCode = "bci.Execute('ADD STATE "+ addStateCode + "')\n"

      buff.writeIndentedLines(startCodePart1 % self.params['Path'])
      buff.writeIndentedLines(addStateCode)
      buff.writeIndentedLines(startCodePart2 % self.params['Source Module'])
     
  def writeExperimentEndCode(self, buff):
    #Close connection with BCI
    endCode = ("##BCI stop code\n"
                    "if bci.Result != '':\n"
                    "    print('Result:', bci.Result)\n"
                    "bci.Stop()\n"
                    "del bci\n"
             )
    if len(str(endCode)):
      buff.writeIndentedLines(str(endCode) + '\n')
