# -*- coding: utf-8 -*-

# ///////////////////////////////////////////////////////////////////////////
# $Id: Container.py 8257 2024-07-16 19:45:32Z jhill $
# Author: jeremy.hill@neurotechcenter.org
# Description: helper container class and pretty-printing routine
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
	'Summarize',
	'Bunch', 'Hierarchy',
	'VettedList',
]

import re
import sys
import ast
import copy
import gzip
import types

try: import cPickle as pickle     # Python 2
except ImportError: import pickle # Python 3

try: import numpy
except ImportError: numpy = None

if sys.version < '3': bytes = str
else: unicode = str; basestring = ( unicode, bytes )

def CompactRepr(a, limit=None):
	if isinstance(a, basestring): return repr(a)
	#if hasattr( a, '_report' ): a = a._report( summarize=limit )
	if isinstance( a, Bunch ): a = a._report( summarize=limit )
	else: a = str(a)
	out = ''
	inQuote = None
	backslashed = False
	for c in a:
		if not inQuote and c in ']})': out = out.rstrip( ', ' )
		if inQuote or not c.isspace(): out += c
		elif not out.endswith( tuple( ',:({[\'" ' ) ): out += ' '
		if backslashed: backslashed = False
		elif c == '\\': backslashed = True
		elif c == inQuote: inQuote = None
		elif not inQuote and c in '\'"': inQuote = c
		#print( 'processed %r, out=%s, backslashed=%r, inQuote=%r' % (c, out, backslashed, inQuote))
		if limit is not None and len(out) > limit: break
	#print('')
	return out
	
def Summarize(a, appendBeforeComment='', limit=50 ):
	"""
	Returns a somewhat easier-to-swallow string representation of
	an object, when the full repr() would otherwise be too long.
	In particular, summarize the interesting atrributes of numpy
	arrays and matrices.
	"""
	if a is None: return "None" + appendBeforeComment

	if   numpy and isinstance(a, numpy.matrix): atype = 'numpy.matrix'
	elif numpy and isinstance(a, numpy.ndarray): atype = 'numpy.array'
	elif isinstance(a, (types.MethodType, types.BuiltinMethodType)):
		obj = getattr(a, '__self__', None)
		if obj is None: atype = repr( a )
		else: atype = 'method %s of %s instance 0x%08x' % (a.__name__,obj.__class__.__name__, id(obj))
		return atype + appendBeforeComment
	elif isinstance(a, (types.FunctionType, types.BuiltinFunctionType)):
		atype = 'function ' + '.'.join(x for x in (a.__module__, a.__name__) if x)
		return atype + appendBeforeComment
	else: atype = a.__class__.__name__
	
	try: length = len(a)
	except: length = None
	try: shape = tuple(int(extent) for extent in a.shape)
	except: shape = ''
	nels = 1
	if shape != '':
		for extent in shape: nels *= extent
		shape = 'shape='+str(shape).replace(' ', '')
	elif length is not None:
		shape = 'length=%d' % length
		nels = length
	
	if nels <= limit:
		s = CompactRepr(a, limit)
		if len(s) <= limit and type(a) in [bool,int,float,str,bytes,unicode,tuple,list,dict]: atype = s
		elif s.startswith( atype + '(' ) and len(s) <= limit: atype = s
		elif len(s) <= limit - len(atype) - 2: atype += '(' + s + ')'
		else: atype += '(...)'
	else: atype += '(...)'

	if isinstance(a, (float,int,bytes,unicode,tuple)): addr = ''
	else: addr = 'id=0x%08x' % id(a)
	
	if numpy and isinstance(a, numpy.ndarray): base = a.base
	else: base = None
	if base is None: base = ''
	else: base = 'base=0x%08x' % id(base)
	
	try: dtype = 'dtype=numpy.' + a.dtype.name
	except: dtype = ''
	
	try: order = '' if a.ndim < 2 else "order='F'" if a.flags.farray else "order='C'"
	except: order = ''
	
	s =  ', '.join( x for x in ( addr, shape, dtype, order, base ) if len( x ) )
	s = ' # '.join( x for x in ( atype + appendBeforeComment, s )  if len( x ) )
	return s	

def CaseSensitiveNaturalSortKey( x ):
    return [ int( part ) if part.isdigit() else part for part in re.split( '([0-9]+)', str( x ) ) ]

class Unspecified: __repr__ = lambda self: self.__class__.__name__
Unspecified = Unspecified()

class Bunch( dict ):
	"""
	A `dict` subclass whose members can be addressed lazily `like.this` as well as
	`like['this']` provided the key does not start with an underscore.
	
	Optionally set the `._summarize` attribute to a number of characters to invoke
	`Summarize()` when converting the object to `str`.   Set `._maxIndent` to
	limit the number of levels that are expanded in the `str` representation.
	
	Useful methods include `._read(filename)`, `._write(filename)` and also
	`._convert(d)` which hierarchically converts any dict `d` (and its values, where
	appropriate) to this class.
	"""
	def __getattr__( self, name ):
		b = self
		for name in name.split( '.' ): b = b[ name ] if name in b else getattr( super( b.__class__, b ), name ) if isinstance( b, Bunch ) else getattr( b, name )
		return b
	def __setattr__( self, name, value ):
		if name.startswith( '_' ): return dict.__setattr__( self, name, value )
		if isinstance( getattr( type( self ), name, None ), property ): return getattr( type( self ), name ).__set__( self, value )
		container = self
		parts = name.split( '.' )
		for name in parts[ :-1 ]:
			try: container = getattr( container, name )
			except: sub = container[ name ] = self.__class__(); container = sub
		container.__setitem__( parts[ -1 ], value )
	def __dir__( self ): return self.keys()
	_getAttributeNames = __dir__
	def __repr__( self ): return self._report()
	_display_sorted = tuple( sys.version_info ) < ( 3, 0 )
	_summarize = False
	_maxIndent = None
	def _report( self, indent=0, minColonPosition=0, sortUnknownKeys=None, summarize=Unspecified, maxIndent=Unspecified ):
		indentSize = 4
		if summarize is Unspecified: summarize = self._summarize
		if maxIndent is Unspecified: maxIndent = self._maxIndent
		s = ' ' * indent * indentSize + '{\n'
		keys = list( self.keys() )
		order = getattr( self, '_fieldOrder', '' )
		known = order.replace( '/', '' ).split()
		order = order.split()
		unknown = [ key for key in keys if key not in known ]
		if sortUnknownKeys or ( sortUnknownKeys is None and self._display_sorted ):
			try: unknown.sort()
			except: unknown.sort( key=CaseSensitiveNaturalSortKey )
		keys = [ key for key in order if key.strip( '/' ) in keys ] + unknown
		maxLen = max( len( repr( key ) ) for key in keys ) if keys else 0
		minColonPosition = max( minColonPosition - indentSize, maxLen + indentSize * ( indent + 1 ) + 1 )
		#minColonPosition = max( minColonPosition, maxLen + indentSize * ( indent + 1 ) + 1 )
		#minColonPosition = maxLen + indentSize * ( indent + 1 ) + 1
		for key in keys:
			if isinstance( key, basestring ):
				if key.startswith( '//' ): s += '\n'; key = key[ 2: ]
			krepr = repr( key )
			spaces = minColonPosition - len( krepr ) - 1
			spacesBefore = indentSize * ( indent + 1 )
			#spacesBefore = spaces
			spacesAfter = spaces - spacesBefore
			s += ' ' * spacesBefore + krepr + ' ' * spacesAfter + ' : '
			value = self[ key ]
			reachingMaxIndent = maxIndent is not None and indent >= maxIndent - 1
			#if isinstance(getattr(value, '_report', None), (types.MethodType, types.BuiltinMethodType)):
			if isinstance( value, Bunch ):
				if reachingMaxIndent: s += Summarize( value, appendBeforeComment=',', limit=2 )
				else: s += '\n' + value._report( indent=indent + 1, minColonPosition=minColonPosition + indentSize, sortUnknownKeys=sortUnknownKeys, summarize=summarize, maxIndent=maxIndent ).rstrip() + ','
			elif summarize:
				s += Summarize( value, appendBeforeComment=',', limit=summarize if summarize > 1 else 50 )
			else:
				if numpy: npopts = numpy.get_printoptions(); numpy.set_printoptions( linewidth=numpy.inf, threshold=numpy.inf )
				vrepr = repr( value ).strip()
				if numpy: numpy.set_printoptions( **npopts )
				if '\n' in vrepr: vrepr = ( '\n' + vrepr ).replace( '\n', '\n' + ' ' * ( spacesBefore + indentSize ) )
				s += vrepr + ','
			s += '\n'
		s += ' ' * ( indent * indentSize ) + '}'
		return s
	def _copy( self, deep=True ):
		return copy.deepcopy( self ) if deep else copy.copy( self )
	def _set( self, **kwargs ):
		for k, v in kwargs.items(): setattr( self, k, v )
		return self
	def _write( self, filename, pickleProtocol=Unspecified, prefix='', suffix='' ):
		"""
		Write a representation of `self` to `filename`.
		
		If `pickleProtocol` is an integer, use the corresonding `pickle`
		protocol to encode the content. If `pickleProtocol` is `None`, use
		a plain-text representation.  If it is unspecified, use `pickle`
		protocol 2 if the `filename` ends in `'.pk'` or `'.pk.gz'`, or
		plain text otherwise.
		
		Optional `prefix` and `suffix` are applied when writing plain
		text; they are ignored in pickle mode.

		The filename may be optionally prefixed by '>' or '>>'. If it is
		'>>' then the file is appended-to rather than overwritten.
		
		If the filename ends in `'.gz'`, `gzip` will be used to compress the
		file content.		
		"""
		openfcn = gzip.open if filename.lower().endswith( '.gz' ) else open
		if pickleProtocol is Unspecified: pickleProtocol = 2 if filename.lower().endswith( ( '.pk', '.pk.gz' ) ) else None
		mode = 'a' if filename.startswith( '>>' ) else 'w'
		filename = filename.lstrip( '>' )
		if pickleProtocol is None:
			with openfcn( filename, mode + 't' ) as fh: fh.write( prefix + self._report( summarize=False ) + suffix )
		else:
			with openfcn( filename, mode + 'b' ) as fh: pickle.dump( self._convert( self, to_dict=True ), fh, protocol=pickleProtocol )
	def _import( self, filename, unpickle=Unspecified ):
		"""
		`._read()` from the specified filename and merge the result into
		`self` using `.update()` (NB: not a hierarchical merge).
		"""
		self.update( self._read( filename, unpickle=unpickle ) )
		return self
	@classmethod
	def _read( cls, filename, unpickle=Unspecified, go_into=( tuple, list ), func=None ):
		"""
		Read the contents of `filename` and `._convert()` it recursively
		to type `cls`.
		
		If the filename ends in `'.gz'`, use `gzip` to uncompress the file
		content.
		
		If `unpickle` is `True`, or if it is unspecified but the `filename`
		ends in `'.pk'` or `'.pk.gz'`, expect the file content to be in `pickle`
		format. Otherwise, expect it to be plain text.
		"""
		openfcn = gzip.open if filename.lower().endswith( '.gz' ) else open
		if unpickle is Unspecified: unpickle = filename.lower().endswith( ( '.pk', '.pk.gz' ) )
		if unpickle:
			with openfcn( filename, 'rb' ) as fh: instance = pickle.load( fh )              # instance is a dict at this point
		else:
			with openfcn( filename, 'rt' ) as fh: instance = ast.literal_eval( fh.read() )  # instance is a dict at this point
		return cls._convert( instance, go_into=go_into, func=func )
	@classmethod
	def _convert( cls, d, to_dict=False, go_into=( tuple, list ), func=None ):
		if not go_into: go_into = ()
		if not func: func = lambda x: x
		if isinstance( d, go_into ): return type( d )( [ cls._convert( each, to_dict=to_dict, go_into=go_into, func=func ) for each in d ] )
		if to_dict: return dict( { k : cls._convert( v, go_into=go_into, func=func, to_dict=True ) for k, v in d.items() } ) if isinstance( d, dict ) and type( d ) != dict        else func( d ) 
		else:       return  cls( { k : cls._convert( v, go_into=go_into, func=func )               for k, v in d.items() } ) if isinstance( d, dict ) and not isinstance( d, cls ) else func( d )
	
	@classmethod
	def _getall( cls, sequence, key, defaultValue=None ):
		return [ cls( item ).get( key, defaultValue ) for item in sequence ]

class Hierarchy( Bunch ):
	"""
	The same as a `Bunch`, but with hierarchical behavior:
	`value = container[key]` and `container[key] = value` operate
	in a special way when `key` is a tuple or list, de-referencing
	sub-members hierarchically in sequence.
	
	Special methods `_hkeys()`, `_hvalues()` and `_hitems()` return
	iterators that go through the whole hierarchy.
	
	"""
	def __init__( self, *pargs, **kwargs ):
		super( Hierarchy, self ).__init__()
		self.update( *pargs, **kwargs )
	def update( self, *pargs, **kwargs ):
		for seq in pargs + ( kwargs.items(), ):
			try:
				seq = seq._hitems()
			except:
				try: seq = seq.items()
				except: pass
			for k, v in seq:
				self[ k ] = v
		# return self # this is useful but it diverges from expected dict.update() behavior and may create unwanted references in IPython
	def __setitem__( self, key, value ):
		cls = type( self )
		if isinstance( key, ( tuple, list ) ) and key:
			for k in key[ :-1 ]:
				try: self = self[ k ]
				except KeyError: new = cls(); self[ k ] = new; self = new
			self[ key[ -1 ] ] = value
			return
		return super( Hierarchy, self ).__setitem__( key, value )
	def pop( self, key, defaultValue=Unspecified ):
		if isinstance( key, ( tuple, list ) ) and key:
			for k in key[ :-1 ]: self = self[ k ]
			return self.pop( key[ -1 ] ) if defaultValue is Unspecified else self.pop( key[ -1 ], defaultValue )
		self = super( Hierarchy, self )
		return self.pop( key ) if defaultValue is Unspecified else self.pop( key, defaultValue )
	def __delitem__( self, key ):
		self.pop( key )
	def __getitem__( self, key ):
		if isinstance( key, ( tuple, list ) ) and key:
			for k in key: self = self[ k ]
			return self
		return super( Hierarchy, self ).__getitem__( key )
	def get( self, key, defaultValue=None ):
		try: return self[ key ]
		except KeyError: return defaultValue
	def _hkeys( self ):
		for k, v in self.items():
			if  hasattr( v, '_hkeys' ):
				for kk in v._hkeys(): yield ( k, ) + ( kk if isinstance( kk, tuple ) else ( kk, ) )
			elif isinstance( v, dict ):
				for kk in v.keys(): yield ( k, kk )
			else: yield k
	def _hvalues( self ):
		for k in self._hkeys(): yield self[ k ]
	def _hitems( self ):
		for k in self._hkeys(): yield ( k, self[ k ] )
		
	@classmethod
	def _hcommon( cls, firstObj, *objs ):
		common = []
		for k, v in cls( firstObj )._hitems():
			for obj in objs:
				try: other = cls( obj )[ k ]
				except KeyError: break
				if EffectivelyDifferent( v, other ): break
			else:
				common.append( ( k, v ) )
		return cls( common )
	@classmethod
	def _hdiff( cls, *objs ):
		commonKeys = list( cls._hcommon( *objs )._hkeys() )
		return [ cls( [ ( k, v ) for k, v in cls( obj )._hitems() if k not in commonKeys ] ) for obj in objs ]
		
		
def EffectivelyDifferent( a, b ):
	if a is None and b is None: return False
	if a is None or  b is None: return True
	try: import numpy
	except ImportError: numpy = None
	equal = None
	if numpy:
		try: equal = numpy.equal( a, b )
		except ValueError: return True
		except: 
			if isinstance( a, numpy.ndarray ) or isinstance( b, numpy.ndarray ): return True
	if equal is None: equal = ( a == b )
	if isinstance( equal, bool ): return not equal
	if not numpy: return True
	try: a, b = numpy.asarray( a ), numpy.asarray( b )
	except: return True
	if tuple( a.shape ) != tuple( b.shape ): return True
	a, b = a.ravel(), b.ravel()
	try: aIsNotNan = ~numpy.isnan( a )
	except: aIsNotNan = numpy.ones( a.shape, dtype=bool )
	try: bIsNotNan = ~numpy.isnan( b )
	except: bIsNotNan = numpy.ones( b.shape, dtype=bool )
	if ( aIsNotNan != bIsNotNan ).any(): return True
	return numpy.asarray( a[ aIsNotNan ] != b[ bIsNotNan ] ).any()


class VettedList( list ):
	"""
	Examples of use::
	
	    class ListOfDicts( VettedList ):
	        _acceptable = dict
	    
	    class ListOfBuiltinSequences( VettedList ):
	        _acceptable = ( tuple, list )

	Instances of such subclasses have all the powers of a builtin `list`.
	In addition, any item appended, inserted, or otherwise introduced into
	such a list must pass an `isinstance()` check against the `_acceptable`
	type or types (which can be specified on a per-subclass or per-instance
	basis). As per the rules for `isinstance()`,  `_acceptable` itself must
	be either a type, or a tuple (not a list) of types.
	"""
	_acceptable = ()
	def _vet( self, x ):
		acceptable = self._acceptable
		if acceptable and not isinstance( x, acceptable ):
			if not isinstance( acceptable, tuple ): acceptable = ( acceptable, )
			raise TypeError( 'list items may only be of %s %s' % ( 'type' if len( acceptable ) == 1 else 'types', '/'.join( cls.__name__ for cls in acceptable ) ) )
		return x
	
	# wrappers around list superclass methods
	
	def __getitem__( self, y ): # this is a boilerplate wrapper required for any list subclass, just to ensure that slices of the original preserve the same class as the original
		result = list.__getitem__( self, y )
		if isinstance( y, slice ): # happens in Python 3 slice subscripting, OR in Python 2 slice subscripting iff an explicit step size is specified
			result = self.__class__( result ) # otherwise the return value will have type `list`
		return result
		
	def __setitem__( self, key, value ): # apply vetting when new items are introduced
		if isinstance( key, slice ): value = [ self._vet( x ) for x in value ]  # happens in Python 3 slice subscripting, OR in Python 2 slice subscripting iff an explicit step size is specified
		else: value = self._vet( value )
		return list.__setitem__( self, key, value )
			
	# additional boilerplate wrappers that ensure the new customized __setitem__ is called whenever and however new items are introduced
	def __init__( self, iterable=() ): self.__setitem__( slice( None,        None  ), iterable )
	def __iadd__( self, value ):       self.__setitem__( slice( len( self ), None  ), value    ); return self
	def extend( self, iterable ):      self.__setitem__( slice( len( self ), None  ), iterable )
	def append( self, object ):        self.__setitem__( slice( len( self ), None  ), [ object ] )
	def insert( self, index, object ): self.__setitem__( slice( index,       index ), [ object ] )
	if hasattr( list, '__setslice__' ):  # this one is only needed in Python 2 (when using slice subscripting WITHOUT an explicit step size) and only works in Python 2
		def __setslice__( self, i, j, y ): self.__setitem__( slice( i, j ), y )
		
	# more boilerplate required for any list subclass, just to ensure class identity is preserved across common operations
	def __add__(  self, value ): return self.__class__( self ).__iadd__( value )
	def __mul__(  self, value ): return self.__class__( self ).__imul__( value )
	def __rmul__( self, value ): return self.__class__( self ).__imul__( value )
	if hasattr( list, '__getslice__' ):  # this one is only needed in Python 2 (when using slice subscripting WITHOUT an explicit step size) and only works in Python 2
		def __getslice__( self, i, j ): return self.__getitem__( slice( i, j ) )
