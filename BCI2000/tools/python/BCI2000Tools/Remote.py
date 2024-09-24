# -*- coding: utf-8 -*-

# ///////////////////////////////////////////////////////////////////////////
# $Id: Remote.py 7217 2023-02-23 22:37:12Z jhill $
# Author: jeremy.hill@neurotechcenter.org
# Description: import helper for prog/BCI2000Remote.py
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

"""
For historical reasons and modularity-related reasons the BCI2000RemoteLib
dynamic library, and its wrapper BCI2000Remote.py, live in the `prog` directory
of the BCI2000 distribution, and there is no bootstrapping---from Python,
you would have to manually find it and import/execute it.

The current submodule is a convenience wrapper for finding `BCI2000Remote.py`,
importing the `BCI2000Remote` class from it, and creating a singular
instance of it called, quite simply, `BCI2000`.

Example::

	from BCI2000Tools.Remote import BCI2000
	
	if not BCI2000.Connect(): raise OSError( "failed to connect to BCI2000" )
	BCI2000.StartupModules(['SignalGenerator', 'DummySignalProcessing', 'DummyApplication'])
	BCI2000.Start()

It will only work if the `BCI2000Tools` Python package has been editably-
installed (i.e. with `pip install -e`) in the expected location (i.e. in
the `tools/python` directory of the relevant BCI2000 distribution), as
recommended in our `setup.py` file.

NB: if you have installed the `BCI2000Tools` Python package editably from
one BCI2000 distribution, but temporarily want to access a *different*
BCI2000 distribution, you can say `from BCI2000Tools import bci2000root`
and use `bci2000root()` to configure the location of your desired BCI2000
distribution *before* importing `BCI2000Tools.Remote`.

This module also wraps the `BCI2000Remote` class to give it additional
superpowers in atomically reading and writing to `.States` and `.Parameters`.
To demonstrate, let's start with what we already know::

	from BCI2000Tools.Remote import BCI2000
	BCI2000.Connect()
	BCI2000.Execute('show window')
	BCI2000.StartupModules(['SignalGenerator', 'DummySignalProcessing', 'DummyApplication'])
	BCI2000.Execute('add state StimulusType 16 0 0 0')
	BCI2000.Execute('add state StimulusCode 16 0 0 0')
	BCI2000.SetConfig()

Now let's examine interaction with `.States`::

	import time
	for i in range( 3 ):
		time.sleep( 1.0 )
		print(BCI2000.States) # it doesn't change.
	
	BCI2000.States.Running = 1  # accessing a single state is easy

	for i in range( 3 ):
		time.sleep( 1.0 )
		print(BCI2000.States) # now you'll see it change
		
	# To set multiple states atomically:
	
	BCI2000.SetStates( StimulusType=1, StimulusCode=11 )       # this is one way
	
	BCI2000.States =  { 'StimulusType':2, 'StimulusCode':22 }  # this is another
	
	BCI2000.States += { 'StimulusType':3, 'StimulusCode':33 }  # this also works	
	
	time.sleep( 1.0 )     # there's a lag of 1 sampleblock before the change takes
	print(BCI2000.States) # effect (2 sampleblocks before it can be detected this way)
	
Now let's examine interaction with `.Parameters`::

	BCI2000.States.Running = 0
	while BCI2000.States.Running: time.sleep( 0.010 )
	
	print( BCI2000.Parameters )
	print( BCI2000.Parameters.SamplingRate )
	
	BCI2000.Parameters.SamplingRate = 512     # change numeric value, keep same unit
	BCI2000.Parameters.SamplingRate = '512'   # explicitly make unitless
	BCI2000.Parameters.SamplingRate = '1kHz'  # explicitly set unit
	
	BCI2000.SetConfig()
	BCI2000.States.Running = 1

"""

__all__ = [
	'BCI2000',
	'bci2000root', 'bci2000path',
]

import os
import sys
import weakref

from . import Bootstrapping; from .Bootstrapping import bci2000root, bci2000path
from . import Container;     from .Container     import Bunch
from . import Parameters;    from .Parameters    import Param, ParamList # TODO: unused - but could add ParamInterface along same lines as StateInterface

class BCI2000LayoutError( ImportError ): pass
	
if bci2000root() is None: # it shouldn't be, if the BCI2000Tools package is in the expected place in tools/python
	raise BCI2000LayoutError( "need to set bci2000root() before importing %s" % __name__ )
prog = bci2000path( 'prog' )
if prog is None or not os.path.isabs( prog ):
	raise BCI2000LayoutError( "something went wrong while looking for the `prog` directory" )
if not os.path.isdir( prog ):
	raise BCI2000LayoutError( "directory not found: %s" % prog )
target = os.path.join( prog, 'BCI2000Remote.py' )
if not os.path.isfile( target ):
	raise BCI2000LayoutError( "file not found: %s" % target )
try:
	sys.path.insert( 0, prog )  # or you could navigate the whole version-dependent labyrinth of https://stackoverflow.com/a/67692
	from BCI2000Remote import * # includes the BCI2000Remote class and various Exception subclasses
finally:
	try: sys.path.remove( prog )
	except: pass

#####################################################################
StateValueType = int
def GetStates( self ):
	"""
	Get all State values, atomically, as a dict.
	"""
	if self._lastSynched is None: self._lastSynched = {}
	if self.Execute( 'freeze states; list states' ):
		try:
			stateDefinitions = self.Result.split( '\n' )
			for stateDefinition in stateDefinitions:
				stateName, _ = stateDefinition.split( ' ', 1 )
				self._lastSynched[ stateName ] = StateValueType( self.GetStateVariable( stateName ).value )
		finally:
			self.Execute( 'thaw states' )
	return Bunch( self._lastSynched )

def SetStates( self, *dicts, **kwargs ):
	"""
	Set one or more State values, atomically, wherever their values have changed
	since the last call to `SetStates()`.
	"""
	if self._lastSynched is None: self._lastSynched = {}
	collated = {}
	for eachDict in dicts:
		if isinstance( eachDict, ( list, tuple ) ) and len( eachDict ) == 2 and not isinstance( eachDict[ 0 ], ( list, tuple ) ): eachDict = [ eachDict ]
		collated.update( eachDict )
	collated.update( kwargs )
	changed = { key : StateValueType( value ) for key, value in collated.items() } #if value != self._lastSynched.get( key, None ) }   # removed the conditional to avoid race conditions with *other* processes/threads that might be controlling BCI2000 state
	command = 'set states ' + ' '.join( '%s %g' % ( key, value ) for key, value in changed.items() )
	if self.Execute( command ): self._lastSynched.update( changed )

class ParameterInterface( object ):
	_connection = None
	def __init__( self, connection ):
		self.__dict__[ '_connection' ] = weakref.proxy( connection )
	def finalize( self, param, **attributes ):
		self._connection.Execute( 'set parameter %s' % param )
		self._connection.Execute( 'list parameters %s' % param.Name )
		param._update( Parameters.make_param( self._connection.Result ) )
	def __setattr__( self, name, value ):
		# TODO: this handles a case like BCI2000.Parameters.TransmitChList = [2,3,4]
		#                        but not BCI2000.Parameters.TransmitChList[1] = 3
		if isinstance( value, Param ): p = value.copy().set( finalize=self.finalize )
		else: p = getattr( self, name ).set( Value=value ) # TODO: how to detect errors?
	@property
	def _values( self ):
		self._connection.Execute( 'list parameters' )
		pl = ParamList( self._connection.Result )
		for p in pl: p.set( finalize=self.finalize, writable='Value' )
		return pl
	def __getattr__( self, name ):
		self._connection.Execute( 'list parameters ' + name )
		p = self._connection.Result
		if not p: raise AttributeError( 'no such parameter %r' % name )
		p = ParamList( p )[ name ].set( finalize=self.finalize, writable='Value' )
		return p
	def __iter__( self ): return self._values.__iter__()
	def __repr__( self ): return repr( self._values )
	def __dir__( self ): return [ p.Name for p in self._values ]
	__getitem__ = __getattr__
	_getAttributeNames = __dir__

class ParameterInterfaceDescriptor( object ):
	def __get__( self, instance, owner=None ):
		return ParameterInterface( instance )
	def __set__( self, instance, value ):
		for p in ParamList( value ):
			instance.Execute( 'set parameter %s' % p ) # TODO: how to detect errors?
	
class StateInterface( object ):
	_connection = None
	def __init__( self, connection ):
		self.__dict__[ '_connection' ] = weakref.proxy( connection )
	def __setattr__( self, name, value ):
		self._connection.SetStates( { name : value } )
	def __iadd__( self, d ):
		self._connection.SetStates( d )
		# don't return self! it results in a follow-up assignment of the whole .States dictionary to itself - because of the lag in setting states, this will actually set everything back to the previous values!
	@property
	def _values( self ):
		return self._connection.GetStates()
	def __getattr__( self, name ):
		d = self._values
		if name == 'keys': return d.keys  # allows `dict(BCI2000.States)`
		return d[ name ]
	def __iter__( self ): return self._values.__iter__()
	def __repr__( self ): return repr( self._values )
	def __dir__( self ): return dir( self._values )
	__getitem__ = __getattr__
	_getAttributeNames = __dir__
	
class StateInterfaceDescriptor( object ):
	def __get__( self, instance, owner=None ):
		return StateInterface( instance )
	def __set__( self, instance, value ):
		if value is not None: instance.SetStates( value )

BCI2000Remote._lastSynched = None
BCI2000Remote.SetStates = SetStates
BCI2000Remote.GetStates = GetStates
BCI2000Remote.States = StateInterfaceDescriptor()
BCI2000Remote.Parameters = ParameterInterfaceDescriptor()

#####################################################################

BCI2000 = BCI2000Remote()
