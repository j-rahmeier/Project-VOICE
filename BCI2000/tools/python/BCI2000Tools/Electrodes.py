# -*- coding: utf-8 -*-

# ///////////////////////////////////////////////////////////////////////////
# $Id: Electrodes.py 8214 2024-06-20 23:01:35Z jhill $
# Author: jeremy.hill@neurotechcenter.org
# Description: linear arithmetic manipulation, and plotting, of electrodes
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

r"""
Example::

	from BCI2000Tools.Electrodes import ChannelSet
	
	c = ChannelSet( 'P3 C3 F3 Fz F4 C4 P4 Cz A1 Fp1 Fp2 T7 P7 O1 O2 F7 F8 A2 P8 T8 TRG-0@0', ref='Pz', gnd='AFz' )
	m = c.RerefMatrix( 'A1,A2', exclude='TRG' )
	cr = m.T @ c   #  numpy.dot(m.T, c) in Python versions older than 3.5
	
	print(c)
	print(cr)

"""

__all__ = [
	'ChannelSet',
	'channel',
	'trodeplot',
	'GetScheme',
	'GetSchemes',
	'GetCoords',
]
import os
import re
import sys
import copy
import inspect
import weakref
import collections


import numpy

import warnings; warnings.filterwarnings( action='ignore', category=PendingDeprecationWarning, message='the matrix subclass is not the recommended way' ) # https://stackoverflow.com/a/53254739

from . import ElectrodePositions; from .ElectrodePositions import GetCoords, GetScheme, GetSchemes
from . import Container;          from .Container          import Bunch
from . import Plotting

if sys.version < '3': bytes = str
else: unicode = str; basestring = ( unicode, bytes )

class ChannelNotFound( IndexError ): pass

natural_sort_key = lambda string: [int(part) if part.isdigit() else part.lower() for part in re.split('([0-9]+)', string)]

UNKNOWN_REF = 'REF?'
UNKNOWN_GND = 'GND?'

class channel( object ):
	
	def __init__(self, pos, neg, gnd, label=None):

		def cleanup(s):
			if isinstance(s, (tuple,list)): s = sum( [list(cleanup(x)) for x in s], [])
			if isinstance(s, basestring):
				s = s.replace(',',' ').lstrip('\r\n ([{').rstrip('}]) \n\r')
				s = s.replace('+',' ').replace('&',' ')
				s = s.split()
			s = tuple(s)
			if len( s ) == 1 and s[ 0 ] in [ 0, '0', '0v', '0V' ]: s = ()
			return s

		pos = cleanup(pos)
		if isinstance(pos, (tuple,list)): pos = [(k,1.0/len(pos)) for k in pos]
		self.pos = dict(pos)
		
		neg = cleanup(neg)
		if isinstance(neg, (tuple,list)): neg = [(k,1.0/len(neg)) for k in neg]
		self.neg = dict(neg)
		
		if isinstance(gnd, dict): gnd = list( gnd.keys() )
		gnd = cleanup(gnd)
		self.gnd = set(gnd)
		
		self.label = label
		self._update()
		
	def _update(self, add=(), subtract=()):
		t = {}; case = {}
		if subtract is None: subtract = {}
		for d in list(subtract) + [self.neg]:
			for k in d:
				u = k.upper(); case[u] = k
				t[u] = t.get(u, 0.0) - float(d[k])
		if add is None: add = {}
		for d in list(add) + [self.pos]:
			for k in d:
				u = k.upper(); case[u] = k
				t[u] = t.get(u, 0.0) + float(d[k])
		tol = 1e-6
		self.pos = dict([(case[k],+v) for k,v in t.items() if v > +tol])
		self.neg = dict([(case[k],-v) for k,v in t.items() if v < -tol])
		
		if len(self.pos) and len(self.neg):
			car = 1.0/(len(self.pos) + len(self.neg))
			carred = 1.0 - car
			if max([abs(v - carred) for v in self.pos.values()] + [abs(v - car) for v in self.neg.values()]) < tol:
				for k in self.pos:
					self.pos[k] = 1.0
					self.neg[k] = car
			elif max([abs(v - carred) for v in self.neg.values()] + [abs(v - car) for v in self.pos.values()]) < tol:
				for k in self.pos:
					self.neg[k] = 1.0
					self.pos[k] = car
		return self
	
	def __repr__(self):
		return '<%s object at 0x%08X>: %s' % (self.__class__.__name__, id(self), self.__str__())
	
	def __str__(self):
		s = self._dict2str(self.pos)+' - '+self._dict2str(self.neg)
		if self.label is not None: s = self.label + ' = ' + s
		return s
	
	def _dict2str(self, d):
		if len(d) == 0: return '0'
		s = ''
		tol = 1e-6
		k,w = list( d.keys() ), list( d.values() )
		avg = len(d) > 1 and max([abs(x - 1.0/len(d)) for x in w]) < tol
		if len(d) > 4:
			if avg: s = '(average of %d electrodes)' % len(d)
			else: s = '(%s and %d others)' % ( sorted( d, key=lambda k:-d[k] )[ 0 ], len(d) - 1 )
		elif avg:
			s = '(' + ' + '.join(sorted(k, key=natural_sort_key)) + ')/%d' % len(d)
		else:
			w,k = zip(*sorted(zip(w,k))[::-1])
			k = list(k)
			for i in range(len(w)):
				if abs(w[i] - 1.0) < tol: pass
				elif abs(1.0/w[i] - round(1.0/w[i])) < tol: k[i] = '%s/%d' % (k[i],round(1.0/w[i]))
				else: k[i] = '%g*%s' % (w[i],k[i])
			s = ' + '.join(k)
			if len(d) > 1: s = '(' + s + ')'
		return s
		
	def _maybe_adopt_gnd(self, other):
		if sum( self.pos.values() ) + sum( self.neg.values() ) < sum( other.pos.values() ) + sum( other.neg.values() ):
			self.gnd.clear()
			for x in other.gnd: self.gnd.add( x )
		return self
		
	def __iadd__(self, other):
		return self._maybe_adopt_gnd(other)._update(add=[other.pos], subtract=[other.neg])
	def __isub__(self, other):
		return self._maybe_adopt_gnd(other)._update(add=[other.neg], subtract=[other.pos])
	def __imul__(self, x):
		for k in self.pos: self.pos[k] *= float(x)
		for k in self.neg: self.neg[k] *= float(x)
		return self._update()
	def __idiv__(self, x):
		for k in self.pos: self.pos[k] /= float(x)
		for k in self.neg: self.neg[k] /= float(x)
		return self._update()
	__itruediv__ = __idiv__
	
	def __add__(self, x): return self.copy().__iadd__(x)
	def __sub__(self, x): return self.copy().__isub__(x)
	def __mul__(self, x): return self.copy().__imul__(x)
	def __rmul__(self, x): return self.copy().__imul__(x)
	def __div__(self, x): return self.copy().__idiv__(x)
	__truediv__ = __div__
	def __neg__(self):
		x = self.copy()
		x.pos,x.neg = x.neg,x.pos
		return x

	def get_label(self, lower=False):
		x = self.label
		if x is None:
			x = sorted([(v,k) for k,v in self.pos.items()])[-1][-1]
		if lower: x = x.lower()
		return x
		
	def get_position(self, type='schematic2D', coords=None):
		return get_position(self.get_label(), type=type, coords=coords)
		
	def copy(self):
		return copy.deepcopy(self)

class ChannelSet(numpy.matrix):
	"""
	ChannelSet('FCz - A1 @ A2')
		# returns a ChannelSet of length 1 (FCz, referenced to A1 and grounded at A2)
		
	ChannelSet('FCz-A1-A2 Cz-A1-A2')
		# returns a ChannelSet of length 2 (two channels, referenced to the average of A1
		# and A2, ground unspecified).
		
	ChannelSet('FCz Cz Pz'.split())
		# is the laziest way of creating the ChannelSet that you probably want (three channels,
		# referenced and grounded who-knows-where)
	
	ChannelSet('FCz Cz Pz TRG-0@0'.split(), ref='A1 A2', gnd='Nz')
		# The `ref` and `gnd` keywords specify the default reference and ground for
		# electrodes that don't explicitly specify their own.  '0' or '0V' can be used to
		# indicate a non-physiological reference or ground, e.g. for a trigger channel.
	
	The specification can also be a multi-line string, either passed directly or read from
	a named file. The recommended layout would then be one channel specification per line.
	A line that begins with - or @ would then specify the default reference or ground for
	any otherwise unreference/ungrounded channels defined above it.  Each line may also,
	optionally, contain numeric coordinates for plotting.  For example:
	
	    c = ChannelSet( '''
	        Cz       0.0   0.0    # note schematic 2D plot positions
	        C3      -0.5   0.0    # are specified, but no reference
	        C4      +0.5   0.0    # or ground, yet
	        
	        EU1 - EL1             # vertical bipolar left-eye EOG
	        
	        TRG - 0 @ 0           # non-physiological trigger channel
	        
	        -A1     -1.2   0.0    # averaged-ear reference
	        -A2     +1.2   0.0    #   for all EEG channels above
	        @AFz     0.0   0.6    # ground for all EEG channels above
	        
	        
	        EKG1                  # position unspecified
	        EKG2                  # (so, do not plot on scalp)
	        -EKGREF               # The EKG has a separate reference
	        @EKGGND               # and ground.	
	    ''' )
	    print(c)
	    c.plot()   # also try c.plot( scheme='auto', coords=None )
	
	
	Alternatively, put all that in a file, and then say something like:
	
	    c = ChannelSet('montage3EEG+1EOG+TRG+2EKG.txt')
	
	A ChannelSet is a numpy.matrix subclass, with the channels arranged in a column.
	Pre-multiplication by a spatial filtering matrix (each row is a filter) works as it should.
		
	"""
	__array_priority__ = numpy.matrix.__array_priority__ + 1.0
	def __new__(cls, s=None, ref=None, gnd=None):
		if s is None: self = []
		coords = None
		aliases = {}
		if isinstance( s, basestring ):
			parsed = ElectrodePositions.ParseElectrodeSpecification( s, interpretAsFileName='auto', returnType='all', collatedAliases=aliases )
			if parsed[ 'coords' ]: coords = parsed[ 'coords' ]
			s = parsed[ 'channels' ]
		self = cls.cparse(s, ref=ref, gnd=gnd)
		self = numpy.matrix(self, dtype=object).view(cls)
		self.__coords = coords                  # note that default values should actually be set in __array_finalize__
		self.__plotopts = Bunch( scheme='' )    # note that default values should actually be set in __array_finalize__
		self.__aliases = aliases
		if self.size == 0: self.shape = (0,1)
		return self

	def __array_finalize__(self, obj):
		if obj is None: return
		self.__coords = getattr(self, '__coords', Bunch() )
		self.__plotopts = getattr(self, '__plotopts', Bunch() )
		self.__aliases = getattr(self, '__aliases', Bunch() )
		super(ChannelSet, self).__array_finalize__(obj) # since we're subclassing a subclass, we must be careful to run the __array_finalize__ method for the intermediate class
		
	def plot(self, *pargs, **kwargs):
		return_handles = kwargs.pop('return_handles', False)
		opts = { 'coords' : self.__coords }
		if self.__plotopts: opts.update( self.__plotopts )
		opts.update( kwargs )
		h = trodeplot(self, *pargs, **opts)
		if return_handles: return h
		
	def rasterplot(self, img, x=None, y=None, interactive=False, drawnow=True, **imgArgs):
		#imgArgs['colorbar'] = imgArgs.get('colorbar', True)
		if x is not None and y is not None: raise ValueError( "supply either `x` or `y` but not both" )
		transposed = ( y is not None )
		channelDimIndex =  1  if transposed else  0
		channelDimName  = 'x' if transposed else 'y'
		paramDimIndex   =  0  if transposed else  1
		paramDimName    = 'y' if transposed else 'x'
		if img.shape[channelDimIndex] != self.size: raise ValueError("number of %s in image should match number of channels (=%d)" % ( ( 'columns' if transposed else 'rows' ), self.size ))
		imgArgs[ paramDimName ] = y if transposed else x
		
		plt = Plotting.load_plt()
		paramIndices = numpy.arange( img.shape[ paramDimIndex ], dtype=numpy.float64 )
		def samples2msec( samples, fs ): return 1000.0 * samples / float( fs )
		fs = None
		if 'fs' in imgArgs and imgArgs[ paramDimName ] is None:
			imgArgs[ paramDimName ] = imgArgs.pop( 'fs' )
		if isinstance( imgArgs[ paramDimName ], ( float, int ) ):
			fs = imgArgs[ paramDimName ]
			imgArgs[ paramDimName ] = samples2msec( paramIndices, fs )
			if imgArgs.get( paramDimName + 'label', None ) is None:
				imgArgs[ paramDimName + 'label' ] = 'time (ms)'
		if imgArgs[ paramDimName ] is None: imgArgs[ paramDimName ] = paramIndices
		if imgArgs.get( paramDimName + 'label', None ) is None:
			imgArgs[ paramDimName + 'label' ] = 'time-sample index'
		
		if interactive: plt.clf(); plt.subplot(121)
		h = Plotting.imagesc(img, aspect='auto', drawnow=False, picker=5, **imgArgs)
		ax = plt.gca()
		channelAxisLims = [ len( self ) - 0.5, -0.5 ]
		ax.set( **{
			channelDimName + 'lim' : channelAxisLims[ ::-1 ] if transposed else channelAxisLims,
			channelDimName + 'ticks' : range( img.shape[ channelDimIndex ] ),
			channelDimName + 'ticklabels' : [ '%d:%s%s' % ( i, ( '\n' if transposed else ' ' ), label ) for i, label in enumerate( self.get_labels() ) ],
		} )
		ax.tick_params( axis=channelDimName, which='major', labelsize=50/len(self)**0.5 )

		ax.grid(True)
		h.channels = self.copy()
		h.img = numpy.asarray( img )
		h.transposed = transposed
		h.paramValues = imgArgs[ paramDimName ]
		h.topoAxes = h.paramAxes = None
		h.chosenParamValue = None
		h.chosenChannelIndex = None
		h.chosenChannelInstance = None
		if interactive:
			h.topoAxes = plt.subplot(222)
			self.plot( act=numpy.zeros( self.size ) + numpy.nan, labels=0, indices=0, contour=0, surf=0 )
			h.paramAxes = plt.subplot(224)
			h.paramAxes.set( ylim=h.get_clim(), xlim=( h.paramValues[ 0 ], h.paramValues[ -1 ] ), xlabel=imgArgs[ paramDimName + 'label' ], title='' )
			h.paramAxes.grid( True )
		plt.figure( h.axes.figure.number ); plt.axes( h.axes )
		
		def onpick(evt):
			h = evt.artist
			mouse = evt.mouseevent
			paramValue, channelIndex = ( mouse.ydata, mouse.xdata ) if h.transposed else ( mouse.xdata, mouse.ydata )
			return h.select( channelIndex, paramValue )
			
		hRef = weakref.ref( h )
		def select( channelIndex, paramValue ):
			"""
			channelIndex is either a zero-based numeric index or a channel label
			"""
			h = hRef()
			if not h: return
			if isinstance( channelIndex, str ):
				channelIndex = h.channels.find_labels( channelIndex, error_if_not=True )[ 0 ]
			channelIndex = int( round( channelIndex ) )
			channelInstance = h.channels.flat[ channelIndex ]
			paramIndex = numpy.argmin( numpy.abs( numpy.array( h.paramValues ) - float( paramValue ) ).ravel() )
			paramValue = numpy.array( h.paramValues ).flat[ paramIndex ]
			paramAxisLabel = h.axes.get_ylabel() if h.transposed else h.axes.get_xlabel()
			topo      = h.img[ paramIndex,   : ] if h.transposed else h.img[ :,   paramIndex ]
			evolution = h.img[ :, channelIndex ] if h.transposed else h.img[ channelIndex, : ]
			
			if len( h.channels ) > 1:
				plt.figure( h.topoAxes.figure.number ); plt.axes( h.topoAxes )
				titleProps = { k: getattr( h.topoAxes.title, 'get_' + k )() for k in 'ha va position fontsize transform'.split() }
				handles, = h.channels.plot( act=topo, clim=h.get_clim(), drawnow=False, balance=None, return_handles=True )
				highlightedElectrode = handles[ 'dots' ][ channelIndex ]
				highlightedElectrode.set( radius=highlightedElectrode.get_radius() * 2, facecolor=( 0, 1, 0 ) )
				paramName = paramAxisLabel.split('\n')[ 0 ]
				title = ( '%g %s' % ( paramValue, paramName ) ) if paramName.lower() in 's sec seconds ms msec milliseconds hz'.split() else ( '%s = %g' % ( paramName, paramValue ) )
				h.topoAxes.set_title( title )
				h.topoAxes.title.set( **titleProps )
			
			plt.figure( h.paramAxes.figure.number ); plt.axes( h.paramAxes )
			titleProps = { k: getattr( h.paramAxes.title, 'get_' + k )() for k in 'ha va position fontsize transform'.split() }
			Plotting.plot( h.paramValues, evolution, drawnow=False)
			plt.plot( [ paramValue ] * 2, [ 0, 1 ], transform=h.paramAxes.get_xaxis_transform(), clip_on=False, linewidth=2, alpha=0.7, color=( 0, 0.8, 0 ) )
			h.paramAxes.set( ylim=h.get_clim(), xlim=( h.paramValues[ 0 ], h.paramValues[ -1 ] ), xlabel=paramAxisLabel, title='%d: %s' % ( channelIndex, channelInstance ) )
			h.paramAxes.title.set( **titleProps )
			
			
			h.chosenParamValue = paramValue
			h.chosenChannelIndex = channelIndex
			h.chosenChannelInstance = channelInstance
			plt.figure( h.axes.figure.number ); plt.axes( h.axes )
			plt.draw()
		h.select = select # now it will act like a "method" of that particular image handle
		fig = plt.gcf()
		Plotting.reconnect( fig, '_rasterplotConnectionID', 'pick_event', onpick if interactive else None )
		if drawnow: fig.canvas.draw()
		return h
	
	def get_labels(self, lower=False, flatten=True, assert_unique=False):
		flat = []
		nonflat = []
		for row in self.A:
			row = [element.get_label(lower=lower) for element in row]
			flat += row
			nonflat.append(row)
		if assert_unique and len(set([x.lower() for x in flat])) < len(flat): raise ValueError('duplicate channel labels in ChannelSet')	
		if flatten: return flat
		else: return nonflat
	
	def find_labels(self, labels, dict_output=False, error_if_not=False):
		candidates = self.get_labels(lower=True, flatten=True)
		if isinstance(labels, basestring): labels = labels.replace(',', ' ').split()
		out = list()
		for label in labels:
			if isinstance(label, int):
				if label < 0: label += len(self)
				if label >= len(self): label = None
			else:
				lowerLabel = label.lower()
				if lowerLabel in candidates: label = candidates.index(lowerLabel)
				elif lowerLabel in self.__aliases: label = candidates.index( self.__aliases[ lowerLabel ][ 1 ].lower() )
				else: label = None
			out.append(label)
			
		if error_if_not not in (None,False):
			if isinstance(error_if_not, basestring): suffixstr = error_if_not
			else: suffixstr = ''
			notfound = [str(label) for label,ind in zip(labels,out) if ind is None]
			if len(notfound) == 1: raise ValueError('channel %s was not found%s' % (notfound[0],suffixstr)) 
			elif len(notfound):    raise ValueError('the following channels were not found%s: %s' % (suffixstr, ' '.join(notfound))) 
			
		if dict_output: out = dict([(str(k),v) for k,v in zip(labels,out)])
		return out
		
	def set_default_reference( self, names ):
		for c in self.flat:
			if c.neg == { UNKNOWN_REF : 1.0 }: c -= channel( names, UNKNOWN_REF, c.gnd )
			
	def set_default_ground( self, name ):
		for c in self.flat:
			if c.gnd == { UNKNOWN_GND }: c.gnd.clear(); c.gnd.add( name )
	
	def select( self, *args ):
		targets = [ x for arg in args for x in ( arg.split() if isinstance( arg, basestring ) else [ arg ] if isinstance( arg, int ) else arg ) ]
		ind = self.find_labels( targets, error_if_not=True )
		return self.flat[ ind ]
	
	def set_plotopts(self, d=None, **kwargs):
		if d is not None: self.__plotopts = Bunch._convert( d )
		if self.__plotopts is None: self.__plotopts = Bunch()
		self.__plotopts.update(kwargs)
		return self
		
	def set_coords(self, value):
		"""
		By default (coords=None) electrode coordinates are looked
		up in the ElectrodePositions module. To override this
		default behaviour, supply a dict <coords> whose keys
		are channel labels, and whose values are coordinate tuples.
		
		The input argument may also be a dictionary of dictionaries,
		to cope with multiple coordinate types. In this case, the
		keys should match possible coordinate types, and the
		corresponding values should be coordinate dicts as described
		above - for example:
		
		{
		  'schematic2D':  {'F3':(-1.0,0.0),  'Fz', (0.0,0.0), ....},
		}
		
		
		"""
		if isinstance( value, basestring ): value = ElectrodePositions.GetCoords( value )
		self.__coords = Bunch() if value is None else Bunch( value )
		return self
	@property
	def coords( self ): return self.__coords
	@coords.setter
	def coords( self, value ): return self.set_coords( value )
	@property
	def plotopts( self ): return self.__plotopts
	@plotopts.setter
	def plotopts( self, value ): return self.set_plotopts( value )
	
	def get_positions(self, type='schematic2D', coords=None):
		if coords is None: coords = self.__coords
		x = numpy.zeros(self.shape, dtype=numpy.float64)
		y = numpy.zeros(self.shape, dtype=numpy.float64)
		lab = self.get_labels(lower=True, flatten=True)
		for i,ch in enumerate(lab):
			x.flat[i],y.flat[i] = get_position(ch, type=type, coords=coords)
		return x,y
	
	def sqdist(self, type='schematic2D', coords=None):
		if coords is None: coords = self.__coords
		x,y = self.flatten().get_positions(type=type, coords=coords)
		p = x + 1j * y
		q = numpy.abs(p - p.T)
		return q ** 2
	
	@staticmethod
	def cparse(s, ref=None, gnd=None):
		"""
		Parse a file, sequence of strings or multiline string.
		On each line, the first (space- or comma-delimited) field is
		the electrode position, the second is the reference electrode(s)
		and the third is the ground electrode, for a given channel.
		
		Optionally, each line may begin with a number, or number followed
		by a colon, indicating the electrode "index". This index is not
		obeyed strictly, but the outputs are sorted by increasing value of
		these indices. 
		"""

		if s is None: s = []
		if hasattr(s, 'readlines'): s = s.readlines()
		if isinstance(s, basestring): s = [x for x in s.replace('\r\n', '\n').replace('\r','\n').split('\n') if len(x)]
		if isinstance(s, numpy.ndarray): s = list(numpy.asarray(s).flat)
		if ref is None: ref = UNKNOWN_REF
		if gnd is None: gnd = UNKNOWN_GND

		out,ind = [],0
		for si in s:
			
			if isinstance(si, channel):
				out.append((ind,si.copy()))
				ind += 1
				continue
				
			if isinstance(si, (tuple,list)):
				row = list(si)
			else:
				if not isinstance(si, basestring): raise ValueError("inputs are expected to be strings")
				if len(si.strip()) == 0: continue
				
				b = [
					re.compile(r'^\s*\{(.*)\}\s*'),
					re.compile(r'^\s*\[(.*)\]\s*'),
					re.compile(r'^\s*\((.*)\)\s*'),
					re.compile(r'^\s*([^\{\[\(\)\]\}\s]+)\s*'),
				]
				r = re.compile(r'[\s,:;]+')
				(x,n) = r.subn(' ', si.strip())
				row = []
				while len(x):
					for r in b:
						m = r.match(x)
						if m is not None:
							row.append(m.groups()[0].split())
							x = r.sub('', x)
							break
					else:
						raise RuntimeError('could not parse channel definition string "%s"' % x)
			if len(row) == 4:
				ind = ' '.join(row.pop(0))
				try: ind = int(ind)
				except: raise RuntimeError('failed to interpret string "%s" as an index' % ind)		

			if len(row) == 1 and len(row[0]) == 1:
				spec = row[0][0]
				if '@' in spec: spec, g = spec.rsplit('@', 1) 
				else: g = ''
				if '-' in spec: spec, r = spec.split('-', 1)
				else: r = ''
				r = r.replace( '-', '+' )
				row[ : ] = spec, r, g
			while len(row) < 3: row.append(None)
			if len(row) != 3: raise RuntimeError('channel definition string "%s" should have 3 or 4 fields' % si) # the case of 4 fields has been reduced to 3 earlier
			if not row[1]: row[1] = ref
			if not row[2]: row[2] = gnd
			
			ch = channel(pos=row[0], neg=row[1], gnd=row[2])
			out.append((ind,ch))
			ind += 1
		
		out = [[ch] for ind,ch in sorted(out)]
		nch = len(out)
		return out
	
	def __grok_filtering_opts(self, exclude, keep, filters_as, coords):
		"""
		exclude      a specification of which input channels should be
		             excluded from computation of the filters, and also
		             left untouched by the re-referencing. Channels may
		             be specified as a list of 0-based indices, a list
		             of channel label strings, a space-delimited string
		             containing the labels, or the string 'auto' which
		             is the default. In 'auto' mode, channels whose
		             schematic2D position is unknown, or outside the
		             unit circle, are excluded.
		             
		keep         a specification of which output channels should be
		             retained.  Channels may be specified as a list of
		             0-based indices, a list of channel label strings,
		             a space-delimited string containing the labels,
		             the string 'all included' which keeps only those
		             channels that were actually re-referenced, or the
		             string 'all' which is the default. Channels are
		             NOT reordered.
		             
		filters_as   either the string 'columns' or the string 'rows' to
		             denote whether each single spatial filter should be
		             a column or a row of the output matrix
		             (default: 'columns')
		             
		coords        By default (coords=None) electrode coordinates are looked
		              up in the ElectrodePositions module. To override this
		              default behaviour, supply a dict <coords> whose keys
		              are channel labels, and whose values are coordinate tuples.
		"""
		def SplitChannelSpec(x): return x.replace( ',', ' ' ).replace( '+', ' ' ).replace( '-', ' ' ).replace( ';', ' ' ).split()
		labels = self.get_labels(lower=True, flatten=True)
		if coords is None: coords = self.__coords
		if exclude == 'auto': exclude = 1.02
		if isinstance( exclude, float ):
			x,y = self.flatten().get_positions(type='schematic2D', coords=coords)
			x[numpy.isnan(x)] = 2.0 * exclude; y[numpy.isnan(y)] = 2.0 * exclude # work around numpy warning for NaN elements in comparison
			exclude = numpy.logical_not((x.flatten()**2 + y.flatten()**2)**0.5 <= exclude) # "not <= 1.02"  is better than "> 1.02"  because it classifies NaNs as outside
		else:
			if exclude is None: exclude = []
			if isinstance(exclude, basestring): exclude = SplitChannelSpec(exclude)
			exclude = [x.lower() for x in exclude]
			exclude = [self.__aliases.get(x,[x,x])[1] for x in exclude]
			exclude = numpy.array([x in exclude for x in labels])
		
		if keep == 'all':
			keep = numpy.array([True] * len(self))
		elif keep == 'all included':
			keep = numpy.logical_not(exclude)
		else:
			if keep is None: keep = []
			if isinstance(keep, basestring): keep = SplitChannelSpec( keep )
			try:
				keep = [ int( x ) for x in keep ]
			except:
				keep = [x.lower() for x in keep]
				keep = [self.__aliases.get(x,[x,x])[1] for x in keep]
				keep = numpy.array([x in keep for x in labels])
			else:
				keep = numpy.array([x in keep for x in range( len( labels ) ) ] )
		exclude = numpy.flatnonzero(exclude)
		keep = numpy.flatnonzero(keep)
		# Note that unlike numpy.nonzero, numpy.flatnonzero will fail for things that have not yet been turned into numpy arrays
		# Alert: numpy.flatnonzero() may be clunky, but trying to emulate matlab "masking" (subscripting with logical arrays)
		# can lead to *really* unpredictable/unintuitive results in numpy....

		if filters_as == 'columns':
			filters_as_rows = False
		elif filters_as == 'rows':
			filters_as_rows = True
		else:
			raise ValueError('filters_as should be "rows" or "columns"')
		
		return exclude,keep,filters_as_rows
		
	def RerefMatrix(self, newReferenceLabels, exclude='auto', keep='all', filters_as='columns', coords=None, invertible=True):
		"""
		Re-referencing matrix: return an invertible matrix of spatial filters for
		the channels named in `newReferenceLabels` (which should all be found among
		`self.get_labels()`).
		
		When you multiply a signal matrix by this matrix (or when you apply this matrix
		to a `ChannelSet` object with the `.spfilt()` method), the first of the channels
		mentioned in `newReferenceLabels` will get replaced by the signal (or `channel`
		record) corresponding to the *old* reference. This ensures that the matrix is
		square, that the transformation is invertible, and that the covariance of the
		re-referenced signal still has the same rank.
		
		Alternatively, you can specify `invertible=False`. This makes no difference if
		there is only one new reference (the signal from the previous reference location
		replaces the one from the new reference). But in the case that there is more
		than one new reference (averaged together) this option results in a non-square,
		non-invertible matrix that preserves all the new reference channels. The previous
		reference signal is appended, creating one more signal than there was originally.
		The covariance matrix of the resulting signal will be rank-deficient (the
		problem is not the added previous-reference channel, it is the fact that one of
		the new-reference channels can be linearly predicted from the others; in the case
		of two signals, e.g. averaged mastoids, the two resulting reference signals
		will each be the negative of the other).
		"""
		if self.shape[0] != self.size: raise TypeError('RerefMatrix() method can only be called on single-column ChannelSet objects')
		if exclude == 'auto': exclude = 1000.0 # NB: doesn't work correctly with inf
		exclude, keep, filters_as_rows = self.__grok_filtering_opts(exclude, keep, filters_as, coords)
		exclude = numpy.array( exclude ).tolist()
		labels = [ label.lower() for label in self.get_labels() ]
		if hasattr( newReferenceLabels, 'get_labels' ):
			newReferenceLabels = newReferenceLabels.get_labels() 
		if isinstance( newReferenceLabels, basestring ):
			newReferenceLabels = newReferenceLabels.replace( ',', ' ' ).replace( '+', ' ' ).split()
		newReferenceIndices = []
		for label in newReferenceLabels:
			newReferenceIndex = self.find_labels( label )[ 0 ]
			if newReferenceIndex is None: raise ChannelNotFound( "cannot use %r as a reference as it is not in the montage" % label )
			newReferenceIndices.append( newReferenceIndex )
			if newReferenceIndex in exclude: exclude.remove( newReferenceIndex )
		n = len(self)
		if len( newReferenceIndices ) == 1: invertible = True
		if not invertible: keep = numpy.append( keep, n )
		pos = numpy.eye( n, n + int( not invertible ), dtype=float)
		neg = pos * 0
		neg[ newReferenceIndices, : ] = 1.0 / len( newReferenceIndices ) 
		neg[exclude, :] = 0.0
		neg[:, exclude] = 0.0 # TODO
		if invertible: pos[ newReferenceIndices[ 0 ], newReferenceIndices[ 0 ] ] = 0 # makes it invertible; retains the previous reference as a channel
		W = pos - neg
		W = W[:, keep]
		if filters_as_rows: W = W.T
		return numpy.asmatrix(W)
	RerefMatrix.__doc__ +=  __grok_filtering_opts.__doc__
	
	def CAR(self, exclude='auto', keep='all', filters_as='columns', coords=None):
		"""
		Common-Average Reference: return a matrix of spatial filters for
		applying a (possibly selective) common-average reference.
		
		"""
		if self.shape[0] != self.size: raise TypeError('CAR() method can only be called on single-column ChannelSet objects')
		exclude, keep, filters_as_rows = self.__grok_filtering_opts(exclude, keep, filters_as, coords)
		n = len(self)
		pos = numpy.eye(n, dtype=float)
		neg = numpy.ones((n,n))
		neg[exclude, :] = 0.0
		neg[:, exclude] = 0.0
		neg /= n - len(exclude)
		W = pos - neg
		W = W[:, keep]
		if filters_as_rows: W = W.T
		return numpy.asmatrix(W)
	CAR.__doc__ +=  __grok_filtering_opts.__doc__
	
	def SLAP(self, sigma1=0.04, sigma2=None, type='schematic2D', exclude='auto', keep='all', filters_as='columns', coords=None, threshold=None):
		"""
		Surface-Laplacian: return a matrix of spatial filters for applying a zero-sum
		difference-of-Gaussians center-surround spatial filter to each channel.
		
		"""
		if self.shape[0] != self.size: raise TypeError('SLAP() method can only be called on single-column ChannelSet objects')
		exclude, keep, filters_as_rows = self.__grok_filtering_opts(exclude, keep, filters_as, coords)
		if sigma2 is None: sigma2 = 3.0 * sigma1
		n = len(self)
		q = self.sqdist(type=type)
		nanrow = numpy.flatnonzero(numpy.isnan(q).all(axis=0))
		bad = [self.flat[x].get_label() for x in nanrow if x not in exclude]
		if len(bad) == 1: raise ValueError('channel %s must be excluded, since its %s position is unknown' % (','.join(bad), type))
		elif len(bad): raise ValueError('the following must be excluded, since their %s positions are unknown: %s' % (type, ','.join(bad)))
		q[:, exclude] = numpy.inf
		q[exclude, :] = numpy.inf
		q.flat[::n+1] = 0.0
		if sigma1 == 0.0: pos = numpy.eye(n)
		else: pos = numpy.exp(q * (-0.5/sigma1**2))
		pos.flat[::n+1] = 1.0
		neg = numpy.exp(q * (-0.5/sigma2**2))
		neg.flat[::n+1] = 0.0
		if threshold is not None:
			neg[ numpy.abs( neg ) < threshold ] = 0.0
			pos[ numpy.abs( pos ) < threshold ] = 0.0
		possum = pos.sum(axis=0)
		possum[possum == 0.0] = 1.0
		negsum = neg.sum(axis=0)
		negsum[negsum == 0.0] = 1.0
		neg = neg * numpy.expand_dims(possum / negsum, 0)
		
		W = pos - neg
		W = W[:, keep]
		if filters_as_rows: W = W.T
		return numpy.asmatrix(W)
	SLAP.__doc__ +=  __grok_filtering_opts.__doc__
	
	def McFarlandLaplacian(self, type='large', exclude='auto', keep='all', filters_as='columns', coords=None):
		"""
		TODO: still under construction.
		"""		
		exclude, keep, filters_as_rows = self.__grok_filtering_opts(exclude, keep, filters_as, coords)
		include = numpy.array( [ i for i in range( len( self ) ) if i not in exclude ] )
		W_all = numpy.eye(len(self), dtype=float)
		subset = self[include]
		masterdict, label2rc, rc2label = {}, {}, {}
		scheme = ElectrodePositions.GetScheme( 'extended1020' )
		schpos = ElectrodePositions.GetCoords( type )
		for lab in scheme:
			s = lab.upper().replace('Z', '0').replace('T', 'C')
			rows = dict([(k,i) for i,k in enumerate('FP AF F FC C CP P PO O I'.split())])
			cols = dict([(k,i) for i,k in enumerate('9 7 5 3 1 0 2 4 6 8 10'.split())])
			br = [x in '0123456789' for x in s].index(True)
			rc = (rows[s[:br]],cols[s[br:]])
			label2rc[lab.lower()] = rc
			rc2label[rc] = lab.lower()

		srcLabels = [x.lower() for x in subset.get_labels()]
		def sqdist(a, b): return (a[0]-b[0])**2 + (a[1]-b[1])**2
		W = numpy.zeros((len(subset), len(subset)), dtype=float)
		step = {'large':2, 'small':1}[type.lower()]
		for col,c in enumerate(subset.flat):
			thislab = c.get_label().lower()
			center = label2rc[thislab]
			flankers = {}
			flankers[1] = (center[0],center[1]-step)
			flankers[-1] = (center[0], center[1]+step)
			flankers[2] = (center[0]-step, center[1])
			flankers[-2] = (center[0]+step, center[1])
			surroundlabels = {}
			for direction,flanker in flankers.items():
				if flanker not in rc2label: continue
				if rc2label[flanker] not in srcLabels: continue # TODO: include this criterion?
				target = schpos[rc2label[flanker]]
				dsq,row,lab = min([(sqdist(schpos[lab],target),index,lab) for index,lab in enumerate(srcLabels)])
				if lab != thislab: W[row, col] = 1
				surroundlabels[direction] = lab
				
			for direction in flankers.keys():
				if direction in surroundlabels: continue
				if -direction not in surroundlabels: continue
				centerpos = schpos[thislab]
				oppositepos = schpos[surroundlabels[-direction]]
				target = [2*a-b for a,b in zip(centerpos,oppositepos)]
				dsq,row,lab = min([(sqdist(schpos[lab],target),index,lab) for index,lab in enumerate(srcLabels)])
				drat = sqdist(centerpos, schpos[lab]) / sqdist(centerpos, oppositepos)
				if not 0.8 < drat < 1.2: continue
				if lab != thislab: W[row, col] = 1
				surroundlabels[direction] = lab
				
			W[:,col] /= -W[:, col].sum()
			W[col,col] = 1.0
			masterdict[thislab] = ' '.join(surroundlabels.values())
			
			
		if 0:
			if   type == 'large': filt = {'C3':'F3  P3  Cz T7', 'C4':'F4  P4  Cz T8'}  # old-style
			elif type == 'small': filt = {'C3':'FC3 CP3 C1 C5', 'C4':'FC4 CP4 C2 C6'}  #   hard-coded dict
			else: raise ValueError('type must be "large" or "small"')
			allnames = ' '.join( list( filt.keys() ) + list( filt.values() ) ).split()
			ind = subset.find_labels(allnames, dict_output=True, error_if_not=' (required for %s Laplacian on [%s])' % (type,','.join(filt.keys())))
			
			W = numpy.zeros((len(subset),len(filt)), dtype=float)
			for col,(center,surround) in enumerate(sorted(filt.items())):
				W[ind[center], col] = 1.0
				surround = surround.split()
				for x in surround: W[ind[x], col] = -1.0 / len(surround)
		
		W_all[ include[ :, None ], include[ None, : ] ] = W
		W_all = W_all[:, keep]
		if filters_as_rows: W_all = W_all.T
		return W_all
		# TODO: also need to returning masterdict? That was an interim solution
		# to a now-forgotten problem, to which comments claimed: the best final
		# solution might be to make the result valid for only one montage,
		# and use a hard-coded masterdict that has been tweaked by hand in
		# place of the small hard-coded dicts in the old method above
	
	def copy( self ):
		other = super(ChannelSet, self).copy()
		other.plotopts = self.plotopts
		other.coords = self.coords
		other.__aliases = copy.deepcopy( self.__aliases )
		return other
		
	def spfilt( self, W, filters_as='columns' ):
		if filters_as not in [ 'rows', 'columns' ]: raise ValueError( "`filters_as` must be either 'rows' or 'columns'" )
		outputLabels = []
		if isinstance( W, basestring ) or W.__class__.__name__ in [ 'Param', 'ParamList' ]:
			W = self.ReadBCI2000SpatialFilter( W, filters_as=filters_as, containerForOutputLabels=outputLabels )
		if self.shape[ 0 ] == self.size:
			if filters_as == 'columns': W = W.T
			product = numpy.dot( W, self )
		elif self.shape[ 1 ] == self.size:
			if filters_as == 'rows': W = W.T
			product = numpy.dot( self, W )
		else:
			raise ValueError( 'do not know how to multiply - channels should be arranged as one row or one column' )
		self.resize( product.shape, refcheck=False )
		self.flat[ : ] = product.flat
		if outputLabels:
			for outputLabel, channel in zip( outputLabels, self.flat ):
				if outputLabel.lower() != channel.get_label().lower():
					channel.neg.clear()
					channel.pos.clear()
					channel.pos[ outputLabel ] = 1.0
					
		return self
	
	def BCI2000Parameters( self, outputFileName=None ):
		"""
		For example::
		
		    c = ChannelSet( 'P3 C3 F3 Fz F4 C4 P4 Cz A1 Fp1 Fp2 T7 P7 O1 O2 F7 F8 A2 P8 T8 TRG-0@0', ref='Pz', gnd='AFz' )
		    print( c.BCI2000Parameters() )
		
		The optional `outputFileName` may optionally begin with
		either '>' or '>>' to denote overwriting or appending,
		respectively (overwriting is the default).
		"""
		from . import Parameters; from .Parameters import ParamList
		p = ParamList( """
			Source int  SourceCh=          0
			Source list ChannelNames=   0
			Source list SourceChGain=   1  auto
			Source list SourceChOffset= 1  auto
			Source list TransmitChList= 1  auto
		""" )
		p.SourceCh.Value = self.size
		p.ChannelNames.Value = list( self.get_labels() )
		if outputFileName is not None:
			mode = 'at' if outputFileName.startswith( '>>' ) else 'wt'
			with open( outputFileName.lstrip( '>' ), mode ) as fh: fh.write( str( p ) + '\n' )
		return p
		
	def BCI2000SpatialFilterParameters( self, spatialFilterMatrix, filters_as='columns', outputFileName=None ):
		"""
		For example::
		
		    c = ChannelSet( 'P3 C3 F3 Fz F4 C4 P4 Cz A1 Fp1 Fp2 T7 P7 O1 O2 F7 F8 A2 P8 T8 TRG-0@0', ref='Pz', gnd='AFz' )
		    m = c.RerefMatrix( 'A1,A2', exclude='TRG' ) # do not apply it to `c` yet
		    print( c.BCI2000SpatialFilterParameters( m ) )
		
		The optional `outputFileName` may optionally begin with
		either '>' or '>>' to denote overwriting or appending,
		respectively (overwriting is the default).
		"""
		inputChannelSet = self
		outputChannelSet = self.copy().spfilt( spatialFilterMatrix, filters_as=filters_as )
		spatialFilterMatrix = numpy.asarray( spatialFilterMatrix )
		if spatialFilterMatrix.ndim != 2: raise ValueError( '`spatialFilterMatrix` must be 2-dimensional' )
		if filters_as == 'columns': spatialFilterMatrix = spatialFilterMatrix.T
		outputLabels = outputChannelSet.get_labels()
		inputLabels = inputChannelSet.get_labels()
		sparse = []
		for outputIndex, row in enumerate( spatialFilterMatrix ):
			for inputIndex, weight in enumerate( row.flat ):
				if not weight or not float( weight ): continue
				sparse.append( [ inputLabels[ inputIndex ], outputLabels[ outputIndex ], weight ] )
				
		from . import Parameters; from .Parameters import ParamList
		p = ParamList( """
			Filtering int    SpatialFilterType=                   2  // sparse matrix format
			Filtering matrix SpatialFilter=      0 { In Out Wt }  %
		""" )
		p.SpatialFilter.Value = sparse
		if outputFileName is not None:
			mode = 'at' if outputFileName.startswith( '>>' ) else 'wt'
			with open( outputFileName.lstrip( '>' ), mode ) as fh: fh.write( str( p ) + '\n' )
		return p
		
	def ReadBCI2000SpatialFilter( self, prm, filters_as='columns', containerForOutputLabels=None ):
		"""
		Interpret the spatial-filter-related parameters from `prm`, which can
		be a parameter filename, parameter file content, a Param instance or
		a ParamList instance.
		
		If `SpatialFilterType` and `SpatialFilterCAROutput` are present, these
		are interpreted appropriately. Otherwise the spatial filter type is
		inferred as being either full or sparse from the `SpatialFilter` matrix
		itself.
		
		A full numeric matrix is returned, with filters as columns unless
		`filters_as='rows'`.
		
		Optionally, pass a `list` as `containerForOutputLabels` to have it
		filled with the output labels specified by the `SpatialFilter`
		parameter (if any).
		"""
		if filters_as not in [ 'rows', 'columns' ]: raise ValueError( "`filters_as` must be either 'rows' or 'columns'" )
		from . import Parameters; from .Parameters import Param, ParamList
		if not isinstance( prm, Param ): prm = ParamList( prm )
		spatialFilterType = None
		carOutput = None
		if isinstance( prm, ParamList ):
			if 'SpatialFilterType' in prm: spatialFilterType = prm.SpatialFilterType.NumericValue
			if 'SpatialFilterCAROutput' in prm:
				carOutput = prm.SpatialFilterCAROutput.Value
				try: carOutput = [ int( item ) - 1 for item in carOutput ]
				except: pass
			if 'SpatialFilter' not in prm: raise ValueError( 'no SpatialFilter parameter found' )
			prm = prm.SpatialFilter
		if spatialFilterType is None: # No accompanying SpatialFilterType parameter?  infer whether sparse or full based on the shape and column headings
			spatialFilterType = 2 if ( prm.ColumnLabels and '\n'.join( prm.ColumnLabels ).upper() == 'IN\nOUT\nWT' ) else 1
		print
		inputLabels = []
		outputLabels = []
		if spatialFilterType == 0: # disable spatial filtering
			m = numpy.eye( self.size )
			outputLabels = self.get_labels()
		elif spatialFilterType == 1: # full matrix (BCI2000 defines these with filters_as='rows')
			if prm.ColumnLabels:  inputLabels = list( prm.ColumnLabels )
			if prm.RowLabels:    outputLabels = list( prm.RowLabels )
			m = numpy.matrix( prm.NumericValue, dtype=float ).T # work with filters_as='columns' for now, until the end
			if inputLabels: m = self.reorder_sf( m, inputs=inputLabels, exclude='', filters_as='columns' )
		elif spatialFilterType == 2: # sparse matrix
			sparse = {}
			try: inputLabels, outputLabels, weights = [ list( x ) for x in zip( *prm.Value ) ]
			except: raise ValueError( 'sparse spatial filters should have exactly 3 columns' )
			try:  inputIndices = [ int(  inputLabel ) - 1 for  inputLabel in  inputLabels ]
			except:  inputIndices = self.find_labels( inputLabels, error_if_not=True )
			try: outputIndices = [ int( outputLabel ) - 1 for outputLabel in outputLabels ]
			except:
				outputMapping = collections.OrderedDict()
				outputIndices = [ outputMapping.setdefault( outputLabel, len( outputMapping ) ) for outputLabel in outputLabels ]
				outputLabels = list( outputMapping.keys() )
			for inputIndex, outputIndex, weight in zip( inputIndices, outputIndices, weights ):
				if  inputIndex < 0: raise ValueError(  'invalid In index %d' %  inputIndex )
				if outputIndex < 0: raise ValueError( 'invalid Out index %d' % outputIndex )
				if inputIndex >= self.size: raise ValueError(  'In index %d is too large for the ChannelSet' %  inputIndex )
				sparse[ ( inputIndex, outputIndex ) ] = float( weight ) # work with filters_as='columns' for now, until the end
			m = numpy.zeros( [ self.size, max( outputIndices ) + 1 ], dtype=float )
			for k, v in sparse.items(): m[ k ] = v
		elif spatialFilterType == 3: # CAR
			reordering = self.find_labels( carOutput, error_if_not=True ) if carOutput else None
			m = self.CAR( exclude='', keep='all', filters_as='columns' ) # work with filters_as='columns' for now, until the end;  CAR() itself will do selection but not reordering, so we will keep='all'
			if reordering is not None: m = m[ :, reordering ]
			
		if containerForOutputLabels is not None: containerForOutputLabels[ : ] = outputLabels
		if filters_as == 'rows': m = m.T
		return m
		

	def reorder_sf(self, W, inputs=None, exclude='auto', filters_as='columns', coords=None):
		"""
		Pre-computed spatial filters are packed as the columns (or rows, if
		filters_as='rows') of matrix W. Let us assume a filter is a column
		(if not, swap the words 'rows' and 'columns' in everything that
		follows).
		
		<inputs>  if supplied, specifies the order and identity of the input
		          channels that correspond to the rows of W.  The input
		          channels must all be present in the ChannelSet.
		
		<exclude> if <inputs> is not supplied, then the input channels are
		          assumed to be the ChannelSet members, in order, with the
		          exclusions specified here. It works the same as the
		          <exclude> option in CAR.
		          
		The output is a set of spatial filters, in the same order they came
		in, but with the rows reordered and padded with 0s (matching up to
		the excluded channels) so that the filters are appropriate for
		application to a signal that can be described by this ChannelSet. 
		
		"""
		if self.shape[0] != self.size: raise TypeError('spfilt() method can only be called on single-column ChannelSet objects')
		exclude, keep, filters_as_rows = self.__grok_filtering_opts(exclude=exclude, keep='all', filters_as=filters_as, coords=coords)
		n = len(self)
		inputDimName = {'rows':'columns', 'columns':'rows'}.get(filters_as)
		if filters_as_rows: Win = W.T
		else: Win = W
		if inputs is None:
			include = sorted(set(range(n)) - set(exclude))
		else:
			if isinstance(inputs, basestring): inputs = inputs.split()
			include = self.find_labels(inputs, error_if_not=True)
			exclude = sorted(set(range(n)) - set(include))
		nInputs = len(include)
		nOutputs = Win.shape[1]
		if Win.shape[0] != nInputs: raise ValueError('W should have %d %s' % (nInputs, inputDimName))
		W = numpy.zeros((n, nOutputs), dtype=float)
		W[include, :] = Win
		# the following lines implement a different approach in which the spatial filter
		# matrix which preserves the excluded channels in the same way that CAR and SLAP do
		# with keep='all'.  This way was not chosen because, unlike CAR and SLAP, we do not
		# know that the input spatial filters preserve the identities (labels) of the channels
		#W = numpy.eye(n)
		#for inputRow,outputRow in enumerate(include):
		#	for inputCol,outputCol in enumerate(include):
		#		W[outputRow, outputCol] = Win[inputRow, inputCol]
		if filters_as_rows: W = W.T
		return numpy.asmatrix(W)
		
	
	def round(self, precision=1e-5):
		for c in self.flat:
			for k,v in c.pos.items(): c.pos[k] = precision * round( v / precision)
			for k,v in c.neg.items(): c.neg[k] = precision * round( v / precision)
			c._update()

SILLY_FONTS = []
def trodeplot(channels=(), act='connected',
		ref=None, gnd=None, troderadius='auto',
		surf=70, contour='auto', scheme=None, coords=None,
		head=True, eegbg='auto', ears='auto', nose='auto',
		color='k', facecolor='auto', cmap='kelvin_i', clim='auto', balance=0.0,
		labels='auto', indices='auto', one_based=False, fontsize='auto',
		ax=None, layout=None, title=None, hold=False, drawnow=True, use_pcolor=False,
		mask_surf='auto', mask_contour='auto', zoom=None,
		axes=None,
	):
	"""
	channels      Channels to plot. May be a sequence of strings, a single
	              space-delimited string of electrode names, or a sequence
	              of channel objects.
	act           An optional sequence of activation values. May also be
	              the string 'connected' which has the same effect as
	              it does in <facecolor>.
	ref           An optional string (or sequence of strings) specifying
	              the names of the reference electrode(s).
	gnd           An optional string (or sequence of strings) specifying
	              the names of the ground electrode(s).
	troderadius   Radius of the circles representing the electrodes, in
	              the ballpark of, say, 0.05, or alternatively 'auto'.
	surf          Whether to draw an interpolated scalp map (assuming <act>
	              has been supplied).  May be True, False or an integer
	              indicating the resolution of the interpolation.
	contour       Whether to draw contours (assuming <act> has been supplied).
	              May be True, False or 'auto'.
	scheme        Whether to draw other electrodes, besides those mentioned,
	              from the same "schemes". May be True, False, 'auto', or a
	              space-delimited string (or tuple of strings or list of
	              strings) specifying explicitly which schemes to draw. The
	              'auto' or True setting makes an intelligent guess at which
	              schemes to use, although if no <channels> at all are
	              supplied, the default is to draw three schemes:
	              scheme='extended1020 eyes_monopolar ears'. The strings
	              must be keys of the dict ElectrodePositions.GetSchemes()
	coords        By default (coords=None) electrode coordinates are looked
	              up in the ElectrodePositions module. To override this
	              default behaviour, supply a dict <coords> whose keys
	              are channel labels, and whose values are coordinate tuples.
	head          Whether to draw a circle for the head: True or False
	eegbg         Whether to draw 10-20-system grid lines: True, False or
	              'auto'.
	ears          Whether to draw ears: True, False or 'auto'.
	nose          Whether to draw a nose: True, False or 'auto'.
	color         A colour specification for the lines, circles and text.
	facecolor     May be True, False or 'auto' (all of which determine
	              whether the <act> values are used to colour the electrode
	              circles). Alternatively it may be 'connected', which causes
	              four different types of electrodes (explicitly supplied 
	              <channels>, <ref>, <gnd> and others from the same <scheme>)
	              to be distinguishable by colour.  Finally, facecolor may
	              also be a single colour specification applied to all.
	              NB: this is overridden by `act='connected'`, so you may
	              need to explicitly set `act=None` or `act=someArray` to
	              see the result.
	cmap          A colormap for the interpolated scalp map.
	clim          Colour limits: may be 'auto' or a sequence (min,max).
	balance       Balance point for colour limits: if not None, the middle
	              of the colormap will be mapped to this value.
	labels        Whether to draw text labels indicating the electrode names.
	              May be True, False, 'auto', or a tuple indicating where to
	              draw them, the 'auto' default being equivalent to (0,0)
	              which is the centre of the electrode
	indices       Whether to draw text labels indicating the numeric indices
	              of the electrodes that were explicitly given in <channels>.
	              May be True, False or 'auto', or a tuple indicating where
	              to draw them, the 'auto' default being equivalent to (1,1)
	              in electrode-radius coordinates (above and to the right
	              of the electrode).
	one_based     Whether channel-index labels are one-based or zero-based
	              (by default, this is False, meaning the first channel will
	              be labelled 0 in Python style, rather than 1 in Matlab
	              style).
	fontsize      The size of labels and indices. May be 'auto', or a size
	              in points.
	ax            Axes on which to draw. Default is plt.gca()
	layout        (rows,cols) of subplots if plotting multiple <act> columns
	title         Axes title (or sequence of titles)
	hold          If True, do not clear the axes before starting.
	drawnow       Whether to call plt.draw() immediately at the end.
	mask_surf     False, True, or a floating-point number.  Dictates whether
	              to limit the visible portion of the interpolated surface
	              to only the convex hull of the points. A floating-point
	              number here allows the hull to be expanded slightly
	              outwards around the points---mask_surf=True is actually
	              equivalent to the default mask_surf=1.1, i.e. 10% expansion)
	mask_contour  As for mask_surf, but determines whether the contour lines
	              should be masked. If a floating-point number is specified
	              for both,  the expansion factor in mask_surf takes precedence.
	"""
	
	# TODO: meg mode, ecog mode
	if ax is None: ax = axes
	plt = Plotting.load_plt()
	import matplotlib
	known_schemes = ElectrodePositions.GetSchemes()
	if channels is None: channels = ()
	if isinstance(channels, basestring): channels = channels.split()
	if not hasattr(channels, '__len__'): channels = [channels]
	if isinstance(channels, numpy.ndarray): channels = list(numpy.asarray(channels).flat)
	if len(channels) and isinstance(channels[0], channel):
		if ref is None:
			ref = [list(ch.neg.keys()) for ch in channels if len(ch.neg) < 3]
			if len(ref): ref = sum(ref, [])
		if gnd is None:
			gnd = [list(ch.gnd) for ch in channels if len(ch.gnd) < 2]
			if len(gnd): gnd = sum(gnd, [])
		channels = [ch.get_label() for ch in channels]
	channels = list(channels)
	chk = [ch.lower() for ch in channels]
	
	number_of_explicitly_given_channels = len(channels)
	
	if ref is None: ref = []
	elif isinstance(ref, basestring): ref = ref.split()
	else: ref = list(ref)
	if gnd is None: gnd = []
	elif isinstance(gnd, basestring): gnd = gnd.split()
	else: gnd = list(gnd)

	def isequalto( x, y ): return isinstance( x, type( y ) ) and x == y # have to do this because act/facecolor might be strings, or they might be numpy arrays (in which case can't use == because it will want to perform elementwise comparison)
	highlight_connected = isequalto( act, 'connected' ) or isequalto( facecolor, 'connected' )
	if act is None or isequalto( act, 'connected' ): contour = surf = False; act = []
	if highlight_connected: facecolor = 'connected'
	

	act = numpy.asarray(act, dtype=numpy.float64)
	if act.shape[0] not in (0,len(channels)): raise ValueError('wrong number of activation values')
	if len(act.shape) > 1 and act.shape[1] > 1:
		try:    args,argnames = {},inspect.getfullargspec(trodeplot)[0]
		except: args,argnames = {},inspect.getargspec(trodeplot)[0]
		locals = inspect.currentframe().f_locals
		for a in argnames:
			if a in locals: args[a] = locals[a]
		act = numpy.asarray(act.view())
		nplots = int(numpy.prod(act.shape[1:]))
		act.shape = (act.shape[0], nplots)
		if layout is None: layout = nplots
		if ax is None:
			if not hold: plt.clf()
			ax = Plotting.subplots(layout, visible=False,xlim=(-1,1),ylim=(-1,1))
		h = []
		for i,ai in enumerate(act.T):
			args['act'] = list(ai)
			args['ax'] = ax.flat[i]
			if isinstance(title, (tuple,list)): args['title'] = title[i]
			h += trodeplot(**args)
		if i+1 < ax.size:
			
			global SILLY_FONTS
			if not SILLY_FONTS:
				# font-wrangling class copied from Shady
				def IfStringThenNormalString( x ):
					if str is not bytes and isinstance( x, bytes ): return x.decode( 'utf-8' )
					if str is not unicode and isinstance( x, unicode ): return x.encode( 'utf-8' )
					return x
				class Font( object ):
					def __init__( self, filename ):
						import warnings, matplotlib.ft2font
						filename = IfStringThenNormalString( filename )
						self.filename = filename.replace( '\\', '/' )
						try: ft2obj = matplotlib.ft2font.FT2Font( self.filename )
						except RuntimeError: self.good = False; return
						else: self.good = True
						self.family_name = ft2obj.family_name
						self.style = sorted( word for word in ft2obj.style_name.lower().split() if word != 'regular' )
						with warnings.catch_warnings():
							warnings.filterwarnings( 'error' )
							try: self.monospace = len( { ft2obj.load_char( ord( char ) ).horiAdvance for char in 'iM' } ) == 1
							except: self.monospace = False
						self.words = self.family_name.lower().split() + self.style
						if self.monospace: self.words += [ 'mono', 'monospace', 'monospaced' ]
						self.italic = 'italic' in self.style
						self.bold = 'bold' in self.style
					description = property( lambda self: 'xxx' if not self.good else ( '"' + self.family_name + '" ' + ' '.join( self.style ).title() + ( ' (monospace)' if self.monospace else '' ) ) )
					short_description = property( lambda self: 'xxx' if not self.good else ( self.family_name + ' '.join( self.style ).title() ) )
					def __repr__( self ): return '%s(%r)' % ( self.__class__.__name__, self.filename )
					def __str__( self ): return self.description
					@classmethod
					def List( cls ):
						import matplotlib.font_manager
						return [ cls( filename ) for filename in matplotlib.font_manager.findSystemFonts() ]			
				searchTerms = [ 'Savoye LET', 'Edwardian Script', 'French Script', 'Vivalidi', 'Script', 'BlackAdder', 'Zapfino' ]
				SILLY_FONTS = [ font for font in Font.List() for searchTerm in searchTerms if searchTerm.lower().replace( ' ', '' ) in font.description.lower().replace( ' ', '' ) ]
				
			import random; random.shuffle( SILLY_FONTS )
			fontname = SILLY_FONTS[ 0 ].family_name if SILLY_FONTS else 'fantasy'
			
			ax.flat[i+1].cla()
			ax.flat[i+1].add_artist(plt.Text(0,0, text='Fin', fontname=fontname, clip_on=False, size=20, visible=True, ha='center'))
			ax.flat[i+1].set(visible=True, frame_on=False,xticks=[],yticks=[],xlim=[-1,1],ylim=[-1,1])
			plt.draw()
		return h
			
	act = list(act.flat) # from now on, we're only dealing with one sequence
	if len(act):
		if clim == 'auto': clim = numpy.r_[min(act),max(act)]
		if balance is not None: clim = balance + numpy.r_[-1,+1] * max(abs(numpy.asarray(clim) - balance))

	if ax is None: ax = plt.gca()
	else: plt.figure(ax.figure.number); plt.axes(ax)
	fig = ax.get_figure()
	if not hold: ax.cla()
	
	h = {'figure':fig, 'axes':ax, 'head':None, 'surf':None, 'dots':None}

	def sqdist(x, x2=None):
		x  =  x[ :, [ 0 ] ] + 1j *  x[ :, [ 1 ] ]
		x2 = x2[ :, [ 0 ] ] + 1j * x2[ :, [ 1 ] ] if x2 is not None else x
		q = numpy.abs(x - x2.T)
		return numpy.asarray( q ) ** 2
	
	def green(x, x2=None):
		g = numpy.asarray(sqdist(x=x, x2=x2))
		mask = g.flat !=0.0
		gfm = g.flat[mask]
		g.flat[mask] = gfm * (0.5 * numpy.log(gfm) - 1.0)
		return g
		
	def interp(x, y, z, xi, yi):
		if hasattr(x, 'flat'): x = x.flat
		if hasattr(y, 'flat'): y = y.flat
		if hasattr(z, 'flat'): z = z.flat
		if hasattr(xi, 'flat'): xi = xi.flat
		if hasattr(yi, 'flat'): yi = yi.flat
		xy = numpy.array(list(zip(x,y))) # TODO: handle duplicate positions in xy
		xi,yi = numpy.meshgrid(xi,yi)
		zi = numpy.zeros(xi.shape, dtype=numpy.float64)
		xiyi = numpy.array(list(zip(xi.flat,yi.flat)))
		del xi,yi
		w = numpy.linalg.solve(green(xy), z)
		zi.flat = numpy.dot(green(xiyi, xy), w).flat
		return zi
	
	res = 70
	if isinstance(surf, (int, float)) and surf > 1: res = surf
	try: matplotlib.colormaps # goalposts moved matplotlib 3.7
	except: # old matplotlib
		if isinstance(cmap, basestring): cmap = plt.cm.get_cmap(cmap)
		if cmap is None: cmap = getattr(plt.cm, 'kelvin_i', plt.cm.jet)
	else: # new matplotlib
		if isinstance(cmap, basestring): cmap = matplotlib.colormaps[cmap]
		if cmap is None: cmap = matplotlib.colormaps.get('kelvin_i', matplotlib.colormaps['jet'])
	
	if contour == 'auto':
		contour = surf and len(channels) <= 100
	
	if isinstance( coords, basestring ):
		coords = ElectrodePositions.GetCoords( coords, interpretAsFileName=True )
		
	expand = 1.1
	if surf or contour:
		positions = [get_position(ch, default=None, coords=coords) for ch in chk]
		if mask_surf == 'auto':
			nOutOfBounds = sum( ( numpy.array( p ) ** 2 ).sum() ** 0.5 > 1.01 for p in positions if p is not None )
			mask_surf = nOutOfBounds > 5
		if mask_contour == 'auto':
			mask_contour = mask_surf
		if isinstance(mask_surf, float): expand = mask_surf
		elif isinstance(mask_contour, float): expand = mask_contour
		
		expand = numpy.array( expand )
		chx,chy,chz = zip(*[tuple( p )+(act[i],) for i,p in enumerate(positions) if p is not None])	
		xl = numpy.array([min(chx), max(chx)]); xl = xl.mean() + expand.flat[0] * (xl - xl.mean())
		yl = numpy.array([min(chy), max(chy)]); yl = yl.mean() + expand.flat[-1] * (yl - yl.mean())
		#xl = yl = [-1.0, 1.0]
		chxi = numpy.linspace(xl[0], xl[1], res, endpoint=True)
		chyi = numpy.linspace(yl[0], yl[1], res, endpoint=True)
		chzi = interp(chx, chy, chz, chxi, chyi)
		h['mask'] = h['contour'] = h['surf'] = None
		if surf:
			if use_pcolor:
				h['surf'] = plt.pcolor(chxi, chyi, chzi, zorder=1)
				h['surf'].set(cmap=cmap,clim=clim,clip_path=None)
			else:
				h['surf'] = matplotlib.image.NonUniformImage(ax, extent=(xl[0],xl[1],yl[0],yl[1]), interpolation='bilinear', zorder=1)
				h['surf'].set(cmap=cmap,clim=clim,clip_path=None)
				h['surf'].set_data(chxi, chyi, chzi)
				try: ax.add_image # doesn't exist in early matplotlib, so you have to do:
				except: ax.images.append(h['surf']) # but that fails in later matplotlib versions (e.g. 3.7)
				else: ax.add_image(h['surf'])
			clim = h['surf'].get_clim()
		if contour and chzi.ravel().ptp():
			h['contour'] = plt.contour(chxi,chyi,chzi, zorder=1.5, colors=[color], linestyles=['--']) # , hold='on'
		
		
		if mask_surf or mask_contour:
			pos = numpy.asarray( [ p for p in positions if p is not None ] )
			m = numpy.expand_dims(pos.mean(axis=0), 0)
			pos = (pos - m) * expand + m
			hull = convex_hull(pos)
			points = hull+(hull[0],)
			codes = [matplotlib.path.Path.MOVETO] + [matplotlib.path.Path.LINETO] * len(hull)
			
			def squares(points):
				def around(x, d=0.5): return ((x[0]-d,x[1]-d),(x[0]-d,x[1]+d),(x[0]+d,x[1]+d),(x[0]+d,x[1]-d))
				d = 0.5 * min([d for d in sqdist(points).flat if d != 0]) ** 0.5
				v = [around(tuple(x), d) for x in points]
				points,codes = (),()
				for vi in v:
					points = points + vi + (vi[0],)
					codes = codes + (matplotlib.path.Path.MOVETO,) + (matplotlib.path.Path.LINETO,) * len(vi)
				return points, codes
			if mask_surf == 'square' or (not mask_surf and mask_contour == 'square'):
				points,codes = squares(positions) # TODO: non-convex rectilinear hull, for ECoG grids
			if zoom is None:
				maxabs = numpy.abs( points ).max()
				zoom = 1.0 / maxabs if maxabs else None
			patch = matplotlib.patches.PathPatch(matplotlib.path.Path(points, codes), edgecolor='none', facecolor='none')
			ax.add_patch(patch)
			h['mask'] = patch
			if mask_surf:
				if h.get('surf', None): h['surf'].set_clip_path(patch)
			if mask_contour:
				for hh in getattr(h.get('contour'), 'collections', []): hh.set_clip_path(patch)
	
	if highlight_connected:
		for i,r in enumerate(ref):
			if r.lower() not in chk: channels.append(r); chk.append(r.lower())
			ref[i] = r.lower()
		for i,g in enumerate(gnd):
			if g.lower() not in chk: channels.append(g); chk.append(g.lower())
			gnd[i] = g.lower()
	if scheme == 'auto':
		if len(channels) == 0: 	scheme = 'extended1020 eyes_monopolar ears'
		else: scheme = not surf	
	if scheme:
		if not isinstance(scheme, (tuple,list,unicode,bytes,numpy.ndarray)): scheme = None
		channels,ans = match_schemes(channels, known_schemes=known_schemes, force_schemes=scheme)
	
	chk = [ch.lower() for ch in channels]
	act += [numpy.nan] * (len(channels)-len(act))
	chx,chy,chz = zip(*[get_position(ch, coords=coords)+(act[i],) for i,ch in enumerate(chk)])	
	chx,chy,chz = list(chx), list(chy), list(chz)
	
	if troderadius == 'auto':
		if surf: troderadius = 0.02
		elif len(channels) > 136: troderadius = 0.04
		else: troderadius = 0.06

	# line up the unknown electrodes down the side
	baseX = numpy.nanmax( list( chx ) + [ 1.0 ] ) * ( expand if mask_surf else 1.0 )
	y = 1.0
	for i,x in enumerate(chx):
		if numpy.isnan(x):
			chx[i] = baseX + troderadius * 2.2
			chy[i] = y
			y -= troderadius * 2.2
	
	eeg = True; meg = False
	if nose == 'auto': nose = (eeg or meg) and head
	h['nose'] = None
	if nose:
		nxy = numpy.array([[0.20, -0.01], [0.13, 0.10], [0.07, 0.14], [0.0, 0.16]])
		nxy = numpy.r_[nxy[:-1], nxy[::-1]*[[-1,1]]] + [[0,1]]
		h['nose'] = plt.Polygon(nxy, closed=False, edgecolor=color, facecolor='None', clip_on=False, zorder=2); ax.add_patch(h['nose'])
		#h['nose'] = Plotting.curve(nxy, edgecolor=color, facecolor='None', clip_on=False, zorder=2, hold=True)
	if ears == 'auto': ears = (eeg or meg) and head
	h['ears'] = []
	if ears:
		w = 0.1 # width factor
		q = 0.04 # length factor
		x = numpy.array([.492, .510, .518, .5299, .5419, .54, .547, .532, .510, .484]) * 2
		y = numpy.array([q+.0555, q+.0775, q+.0783, q+.0746, q+.0555, -.0055, -.0932, -.1313, -.1384, -.1199]) * 2
		x = x - 1; x = w * x/max(x) + 1
		y[[0, -1]] = numpy.sign(y[[0,-1]]) * (1 - x[[0,-1]] ** 2) ** 0.5;
		exy = numpy.c_[x,y]
		right = plt.Polygon(exy,            closed=False, edgecolor=color, facecolor='None', clip_on=False, zorder=2); ax.add_patch(right)
		left  = plt.Polygon(exy * [[-1,1]], closed=False, edgecolor=color, facecolor='None', clip_on=False, zorder=2); ax.add_patch(left)
		#right = Plotting.curve(exy,            edgecolor=color, facecolor='None', clip_on=False, zorder=2, hold=True)
		#left  = Plotting.curve(exy * [[-1,1]], edgecolor=color, facecolor='None', clip_on=False, zorder=2, hold=True)
		h['ears'] = [left,right]

	if head or h.get('surf') or h.get('contour'): #or h.get('nose') or h.get('ears'):
		h['head'] = plt.Circle((0.0,0.0), radius=1.0, edgecolor='none', facecolor='none', clip_on=False, zorder=2); ax.add_patch(h['head'])
		if head: h['head'].set_edgecolor(color)
		if not mask_surf and h.get('surf'): h['surf'].set_clip_path(h['head'])
		if not mask_contour:
			for hh in getattr(h.get('contour'), 'collections', []): hh.set_clip_path(h['head'])
		#if h.get('nose'): h['nose'].set_clip_path(h['head'])  # TODO: how to invert the path?
		#if h.get('ears'): h['ears'].set_clip_path(h['head'])  # TODO: how to invert the path?

	if eeg and eegbg:
		gridlines = [
			'f9 f7 f5 f3 f1 fz f2 f4 f6 f8 f10',
			'ft9 ft7 fc5 fc3 fc1 fcz fc2 fc4 fc6 ft8 ft10',
			'tp9 tp7 cp5 cp3 cp1 cpz cp2 cp4 cp6 tp8 tp10',
			'p9 p7 p5 p3 p1 pz p2 p4 p6 p8 p10',
			'f5 fc5 c5 cp5 p5',
			'af3 f3 fc3 c3 cp3 p3 po3',
			'f1 fc1 c1 cp1 p1',
			'f2 fc2 c2 cp2 p2',
			'af4 f4 fc4 c4 cp4 p4 po4',
			'f6 fc6 c6 cp6 p6',
		]
		if eegbg == 'auto':
			drawing = zip(chx,chy)
			gpos = [get_position(ch, coords=coords) for ch in ' '.join(gridlines).split()]
			eegbg = head and not surf and False not in [gp in drawing for gp in gpos]
		if eegbg:
			circ = plt.Circle((0.0,0.0), radius=0.82, edgecolor=color, facecolor='none', zorder=3); ax.add_patch(circ)
			h['gridlines'] = [circ]
			for gl in gridlines:
				x,y = zip(*[get_position(ch, coords=coords) for ch in gl.split()])
				h['gridlines'] += plt.plot(x, y, linestyle='-', marker='None', color=color, zorder=3)
			h['gridlines'] += plt.plot([0,0], [-1,+1], linestyle='-', marker='None', color=color, zorder=3)
			h['gridlines'] += plt.plot([-1,+1], [0,0], linestyle='-', marker='None', color=color, zorder=3)
	h['dots'] = []
	h['labels'] = []
	h['indices'] = []
	#dfc = fig.get_facecolor()
	dfc = None
	if dfc in ('none','None',None): dfc = 'w'
	if facecolor not in  ('auto','connected') and isinstance(facecolor, (unicode,bytes,tuple,list,float)): dfc = facecolor
	for i, (x,y) in enumerate(zip(chx,chy)):
		if fontsize == 'auto':
			fontsize = numpy.diff(ax.get_position(),axis=0).flat[-1] * fig.get_size_inches()[1] * 1.2
			fontsize = int(round(fontsize))
		circ = plt.Circle((x,y), radius=troderadius, edgecolor=color, facecolor=dfc, clip_on=False, zorder=4); ax.add_patch(circ); h['dots'].append(circ)
		z = act[i]
		if facecolor == 'auto': actface = not surf or (
			eeg and (x**2+y**2)**0.5 > 1.005 and not mask_surf
		)
		else: actface = isinstance(facecolor, (int,bool)) and facecolor
		if facecolor == 'connected':
			actface = False
			if i < number_of_explicitly_given_channels: circ.set_facecolor([1.0,0.5,0.5])
			elif chk[i] in ref: circ.set_facecolor([0.5,0.5,1.0])
			elif chk[i] in gnd: circ.set_facecolor([0.5,1.0,0.5])
		if actface and not numpy.isnan(z):
			z = (z - clim[0]) / (clim[-1] - clim[0])
			circ.set_facecolor(cmap(z))
		props = {'fontsize':fontsize,'color':color,'fontname':'monospace','verticalalignment':'center','horizontalalignment':'center', 'clip_on':False, 'zorder':5}
		if labels == 'auto': labels = {True:(0,0), False:False}[troderadius>=0.04]
		if labels:
			xy = numpy.array([x,y])
			if isinstance(labels, (tuple,list,numpy.ndarray)): xy = xy + numpy.asarray(labels) * troderadius
			txt = plt.Text(xy[0],xy[1], text=channels[i], **props); ax.add_artist(txt); h['labels'].append(txt)
		if indices == 'auto': indices = {True:(1,1), False:False}[troderadius>=0.04]
		if indices:
			xy = numpy.array([x,y])
			if indices == True: indices = (1,1)
			if isinstance(indices, (tuple,list,numpy.ndarray)): xy = xy + numpy.asarray(indices) * troderadius
			if i < number_of_explicitly_given_channels: text = str(i + int(bool(one_based)))
			elif chk[i] in ref: text = 'ref'
			elif chk[i] in gnd: text = 'gnd'
			else: text = ''
			txt = plt.Text(xy[0],xy[1], text=text, **props); ax.add_artist(txt); h['indices'].append(txt)
	
	lim = 1.0 / zoom if zoom else 1.0
	ax.set(aspect='equal', xticks=[],yticks=[],frame_on=False,xlim=(-lim,lim),ylim=(-lim,lim), visible=True)
	if title is not None:
		t = ax.set_title(title)
		t.set(position=(1,0),verticalalignment='bottom',rotation=45)
	if drawnow: plt.draw()
	return [h]

def match_schemes(channels, known_schemes=None, force_schemes=None):
	
	if not isinstance(channels, basestring):
		for ch,i in enumerate(channels):
			if hasattr(ch, 'get_label'): ch = ch.get_label()
		channels = ' '.join(channels)
	lchannels = channels.lower().split()
	channels = channels.split()

	if known_schemes is None:
		known_schemes = ElectrodePositions.GetSchemes()
		
	if force_schemes is None:
		count = {}
		matched = {}
		lengths = {}
		for k,v in known_schemes.items():
			v = v.lower().split()
			lengths[k] = len(v)
			for ch in channels:
				if ch.lower() in v:
					count[k] = count.get(k, 0) + 1
					matched[ch] = matched.get(ch, [])
					matched[ch].append(k)
		matchingschemes = {}
		for ch, cand in matched.items():
			cand = [(-count[scheme],lengths[scheme],scheme) for scheme in cand]
			best = sorted(cand)[0][2]
			matchingschemes[best] = tuple(known_schemes[best].split())
	else:
		if isinstance(force_schemes, basestring): force_schemes = force_schemes.split()
		matchingschemes	= dict([
			(
				sch,
				known_schemes[sch.lower()].split() if sch.lower() in known_schemes else ElectrodePositions.GetScheme( sch )
			)
			for sch in force_schemes
		])
	
	universe = {}
	for v in matchingschemes.values():
		for vi in v:
			if vi.lower() not in lchannels: universe[vi.lower()] = vi
	universe = channels + sorted(universe.values())
	return universe,matchingschemes	

def get_position(label, type='schematic2D', default=(numpy.nan,numpy.nan), coords=None):
	if coords is None or len( coords ) == 0: return ElectrodePositions.GetCoords( type ).get(label.lower(), default)
	if hasattr(coords, type): coords = getattr(coords, type)
	elif type in coords: coords = coords[type]
	for k,v in coords.items():
		if k.lower() == label.lower(): return v
	return default

def convex_hull(points):
	"""
	Calculate the convex hull of a set of points.
	Adapted from http://code.activestate.com/recipes/66527/
	"""
	
	def _myDet(p, q, r):
		sum1 = q[0]*r[1] + p[0]*q[1] + r[0]*p[1]
		sum2 = q[0]*p[1] + r[0]*q[1] + p[0]*r[1]
		return sum1 - sum2
		
	def _isRightTurn( p, q, r ):
		#assert p != q and q != r and p != r
		return _myDet(p, q, r) < 0
		
	# Get a local list copy of the points and sort them lexically.
	points = sorted(set([tuple(p) for p in points]))
	# Build upper half of the hull.
	upper = [points[0], points[1]]
	for p in points[2:]:
		upper.append(p)
		while len(upper) > 2 and not _isRightTurn(*upper[-3:]): del upper[-2]
	points.reverse()
	# Build lower half of the hull.
	lower = [points[0], points[1]]
	for p in points[2:]:
		lower.append(p)
		while len(lower) > 2 and not _isRightTurn(*lower[-3:]): del lower[-2]
	# Remove duplicates.
	del lower[0]
	del lower[-1]
	# Concatenate both halfs and return.
	return tuple(upper + lower)
	