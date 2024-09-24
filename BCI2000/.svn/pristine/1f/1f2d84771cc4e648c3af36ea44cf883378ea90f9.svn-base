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
iconFile = path.join(thisFolder, 'bci_updateState.png')
tooltip = _translate('BciUpdateState: Update a BCI state')
_localized = { 'stateName': _translate('State Name'),
               'responseObjectVariable':_translate('Response component variable'),
               'useExpression': _translate('Use Expression'),
               'expression': _translate('Expression'),
               'device': _translate('Device')
            }


class BciUpdateStateComponent(BaseComponent):
  # an attribute of the class, determines the section in the components panel
  categories = ['Custom']
  """An event class for updating the states in the BCI2000 into Builder experiments"""

  def __init__(self, exp, parentName, name='BciUpdateState', stateName = "", responseObjectVariable = "",
                useExpression = False, expression = "", device = "None", startType='time (s)', startVal=0.0,
                stopType='duration (s)', stopVal='', startEstim='', durationEstim='',
                syncScreenRefresh=True ):
    super(BciUpdateStateComponent, self).__init__(exp, parentName, name, startType=startType, startVal=startVal,
            stopType=stopType, stopVal=stopVal, startEstim=startEstim, durationEstim=durationEstim)
    self.type = 'BciUpdateState'
    self.targets = ['PsychoPy', 'PsychoJS']
    self.url = "http://www.psychopy.org/builder/components/code.html"
    # params
    # want a copy, else codeParamNames list gets mutated
    self.order = ['stateName','responseObjectVariable','useExpression','expression', 'device']
        
    msg = _translate("State to update")
    self.params['stateName'] = Param(
            stateName, valType='str', allowedTypes=[],
            updates='constant', allowedUpdates=[],
            hint=msg,
            label=_localized['stateName'])
    
    msg = _translate("Variable to update value in accordance with")
    self.params['responseObjectVariable'] = Param(
            responseObjectVariable, valType='str', allowedTypes=[],
            updates='constant', allowedUpdates=[],
            hint=msg,
            label=_localized['responseObjectVariable'])

    msg = _translate("Use an expression")
    self.params['useExpression'] = Param(
            useExpression, valType='bool', allowedTypes=[],
            updates='constant', allowedUpdates=[],
            hint=msg,
            label=_localized['useExpression'])

    msg = _translate("Expression to evaluate")
    self.params['expression'] = Param(
            expression, valType='extendedCode', allowedTypes=[],
            updates='constant', allowedUpdates=[],
            hint=msg,
            label=_localized['expression'])

    msg = _translate("Select device")
    self.params['device'] = Param(
            device, valType='str', allowedVals=['None','Keyboard','Mouse','Joystick','Other'],
            updates='constant', allowedUpdates=[],
            hint=msg,
            label=_localized['device'])

  def writeRoutineStartCode(self, buff):
    #Make an empty object for BCIUpdateState, otherwise an error is thrown.
    code = "%(name)s = type('',(),{})()\n"
    buff.writeIndentedLines(code % self.params)

  def writeFrameCode(self, buff):
    try:
      #Check if the variable to check if an variable or attribute of an object
      objectVariable = str(self.params['responseObjectVariable']).split('.')
      isObject = True if (len(objectVariable) > 1) else False
      
      if( not self.params['useExpression']):
        #Keep bci running
        codeStart = ("    bci.Execute('SET STATE RUNNING 1')\n"
                    )
        #Update the state
        codeEnd = ("        if (type(%(responseObjectVariable)s) == str and len(%(responseObjectVariable)s)==1):\n"
                   "            bci.Execute('SET STATE %(stateName)s %%d' %% ord(%(responseObjectVariable)s) )\n"
                   "        elif (type(%(responseObjectVariable)s) == int):\n"
                   "            bci.Execute('SET STATE %(stateName)s %%d' %% %(responseObjectVariable)s)\n"
                   "        else:\n"
                   "            print('Incorrect variable type used to set state')\n"
                  )
        #Check if the variable exists before updating the state
        codeIfCondition = ""
        if(isObject):
          codeIfCondition = ("    if (hasattr(%s,'%s')):\n" % (objectVariable[0], objectVariable[1]))
        else:
          codeIfCondition = ("    if ('%s' in locals()):\n" % (objectVariable[0]))
        #buff.writeIndentedLines(codeStart)
        buff.writeIndentedLines(codeIfCondition)  
        buff.writeIndentedLines(codeEnd % self.params)
      else:
        #Include user defined code
        if(len(str(self.params['expression']))):
          buff.writeIndentedLines(str(self.params['expression']) + '\n')

    except Exception as e:
      raise e             

  def writeRoutineEndCode(self, buff):
    try:
      #Check if the variable to check if an variable or attribute of an object
      objectVariable = str(self.params['responseObjectVariable']).split('.')
      isObject = True if (len(objectVariable) > 1) else False
      
      if( not self.params['useExpression']):
        #Keep bci running
        codeStart = ("bci.Execute('SET STATE RUNNING 1')\n"
                    )
        #Update the state
        codeEnd = ("    if (type(%(responseObjectVariable)s) == str and len(%(responseObjectVariable)s)==1):\n"
                   "        bci.Execute('SET STATE %(stateName)s %%d' %% ord(%(responseObjectVariable)s) )\n"
                   "    elif (type(%(responseObjectVariable)s) == int):\n"
                   "        bci.Execute('SET STATE %(stateName)s %%d' %% %(responseObjectVariable)s)\n"
                   "    else:\n"
                   "        print('Incorrect variable type used to set state')\n"
                  )
        #Check if the variable exists before updating the state
        codeIfCondition = ""
        if(isObject):
          codeIfCondition = ("if (hasattr(%s,'%s')):\n" % (objectVariable[0], objectVariable[1]))
        else:
          codeIfCondition = ("if ('%s' in locals()):\n" % (objectVariable[0]))
        #buff.writeIndentedLines(codeStart)
        buff.writeIndentedLines(codeIfCondition)  
        buff.writeIndentedLines(codeEnd % self.params) 

    except Exception as e:
      raise e             