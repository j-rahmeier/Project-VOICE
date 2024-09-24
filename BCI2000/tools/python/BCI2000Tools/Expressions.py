# -*- coding: utf-8 -*-

# ///////////////////////////////////////////////////////////////////////////
# $Id: Expressions.py 7371 2023-05-23 04:55:32Z jhill $
# Author: jeremy.hill@neurotechcenter.org
# Description: helper for working with BCI2000 Expressions
#
# $BEGIN_BCI2000_LICENSE$
#
# This file is part of BCI2000, a platform for real-time bio-signal research.
# [ Copyright (C) 2000-2022: BCI2000 team and many external contributors ]
#
# BCI2000 is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# BCI2000 is distributed in the hope that it will be useful, but
#                         WITHOUT ANY WARRANTY
# - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <http://www.gnu.org/licenses/>.
#
# $END_BCI2000_LICENSE$
# ///////////////////////////////////////////////////////////////////////////

__all__ = [
	'Expression',
]

import re
import sys

import numpy

if sys.version < '3': bytes = str
else: unicode = str; basestring = ( unicode, bytes )

apply = lambda x: x() # no longer builtin in Python 3

class Expression(object):
	"""
	NB: this is not a 1:1 implementation of BCI2000's syntax. It covers many
	of the bases, but in a rather hacky way.
	"""
	def __init__(self, string, states=None, channels=None, elements=None, bci=None):
		self.__pystring = '0'
		self.__string = '0'
		self.__channels = []
		self.__elements = []
		self.__signal = numpy.zeros((0,0))
		self.states = {}
		if bci is not None: # BCPy2000 instance
			if states is None: states = bci.states
			if channels is None: channels = bci.in_signal_props['ChannelLabels']
			if elements is None: elements = bci.in_signal_props['ElementLabels']
		if states is not None: self.states = states
		if channels is not None: self.channels = channels
		if elements is not None: self.elements = elements
		if string is not None: self.string = string
	
	def Signal(self, ch, el):
		if isinstance(ch, basestring):
			if ch.lower() not in self.__channels: raise ValueError('error in expression "%s": unrecognized channel label "%s"' % (self.string, ch))
			ch = self.__channels.index(ch.lower()) + 1
		elif not 1 <= ch <= self.__signal.shape[0]:
			raise ValueError('error in expression "%s": only channel indices from 1 to %d are valid' % (self.string,self.__signal.shape[0]))
		if isinstance(el, basestring):
			if el.lower() not in self.__elements: raise ValueError('error in expression "%s": unrecognized element label "%s"' % (self.string, el))
			el = self.__elements.index(el.lower()) + 1
		elif not 1 <= el <= self.__signal.shape[1]:
			raise ValueError('error in expression "%s": only element indices from 1 to %d are valid' % (self.string,self.__signal.shape[1]))
			
		return float(self.__signal[ch-1, el-1])
	
	def Evaluate(self, input=None):
		if input is None:
			input = numpy.zeros((len(self.channels),len(self.elements)))
		self.__signal = numpy.asarray(input)
		while len(self.__signal.shape) < 2: self.__signal.shape = tuple(self.__signal.shape) + (1,)
		return eval(self.__pystring)
	
	__call__ = Evaluate
	
	def _PythonizeExpression(self, x):
		x = x.replace('^', '**')
		x = re.sub(r'State\(([0-9A-Za-z]+)\)', r'(\1)', x)
		x = re.sub(r'[A-Za-z_][0-9A-Za-z_]*', lambda m:self._PythonizeExpressionSubstring(m.group()), x)
		return x
		
	def _PythonizeExpressionSubstring(self, x):
		if x in self.states: return 'float(self.states["%s"])' % x
		if hasattr(self, x): return 'self.' + x     # Signal(X,Y) becomes self.Signal(X, Y)
		if hasattr(numpy, x): return 'numpy.' + x   # abs, log, exp, ...
		# TODO: need to cope with quoteless literals with units, like 50ms or 10Hz, as in Signal("Cz", 10Hz)
		raise ValueError("failed to interpret '%s' in expression" % x)
		
	@apply
	def string():
		doc = "source string for the Expression"
		def fget(self): return self.__string
		def fset(self, val): self.__pystring = self._PythonizeExpression(val) ; self.__string = val
		return property(fget, fset, doc=doc)	
	@apply
	def channels():
		doc = "sequence of channel labels"
		def fget(self): return tuple(self.__channels)
		def fset(self, val): self.__channels = [str(x).lower() for x in val]
		return property(fget, fset, doc=doc)
	@apply
	def elements():
		doc = "sequence of element labels"
		def fget(self): return tuple(self.__elements)
		def fset(self, val): self.__elements = [str(x).lower() for x in val]
		return property(fget, fset, doc=doc)
