#!/usr/bin/env -S  python  #
# -*- coding: utf-8 -*-

# ///////////////////////////////////////////////////////////////////////////
# $Id: EventRelated.py 8347 2024-09-12 20:41:05Z jhill $
# Author: jeremy.hill@neurotechcenter.org
# Description: high-level ERP analyzer
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

__doc__ = """

A runnable script, or a module that provides the `EpochSet` class
with the same functionality.

TODO: MORE GENERAL DOCUMENTATION HERE

You can use '@' as a substitute for '*' in wildcard expressions to
avoid getting your IPython command-line tangled up.

"""
# before we do any third-party imports, parse command-line arguments
if __name__ == '__main__':
	import argparse, ast
	
	def TwoNumbers( s ):
		seq = ast.literal_eval( s )
		x = a, b = [ float(element) for element in seq ] # will throw an exception if there aren't exactly two elements, both interpretable as floats
		return x

	class HelpFormatter( argparse.RawDescriptionHelpFormatter ): pass
	#class HelpFormatter( argparse.RawDescriptionHelpFormatter, argparse.ArgumentDefaultsHelpFormatter ): pass
	parser = argparse.ArgumentParser( description=__doc__, formatter_class=HelpFormatter, )#   prog='python -m RecruitmentCurveFitting', )
	parser.add_argument(       "patterns",           metavar='PATTERN',  nargs='*',                        help='filename of the .dat file to analyze, or glob pattern matching multiple dat files' )
	parser.add_argument( "-t", "--trigger",          metavar='CHANNEL_OR_STATE',  default='DigitalInput*', help="channel name or state name/pattern for triggers. If you specify a state name with a '*' in it, like the default value 'DigitalInput*', then all matching state names are examined and the one with the most activity is used.  If you suffix `:4` to a state name then the state is bit-ANDed with 2**4.  If you suffix `:-4` then the state is bit-NANDed with 2**4." )
	parser.add_argument( "-b", "--responseState",    metavar='STATE',                                      help='optional state name for extracting response events' )
	parser.add_argument(       "--lookBackMsec",     metavar='MSEC',              default=100, type=float, help="number of milliseconds to look back from time zero" )
	parser.add_argument( "-l", "--lookAheadMsec",    metavar='MSEC',              default=800, type=float, help="number of milliseconds to look ahead from time zero" )
	parser.add_argument(       "--filterOrder",      metavar='N',                 default=4,   type=int,   help="band-pass filter order (int)" )
	parser.add_argument( "-f", "--filterBand",       metavar='LO_HZ,HI_HZ',       default=( 0.5, 8.0 ), type=TwoNumbers, help="high-pass and low-pass cutoff frequencies in Hz" )
	parser.add_argument( "-r", "--rereference",      metavar='REF_CHANNELS',      default=None,            help='comma-separated names(s) of the channel(s) to which the data should be re-referenced.  Alternatively, use one the special strings CAR or CAR++ or CAR--:  CAR is common-average reference excluding any electrodes outside the unit circle; CAR++ is CAR including even the electrodes outside the unit circle; CAR-- is CAR excluding only the channels that seem bad.' )
	parser.add_argument( "-d", "--defaultReference", metavar='REF_CHANNELS',      default=None,            help="comma-separated names(s) of the reference channel(s) used for recording in the file (assuming a --channelMap was not specified or omits this information)" )
	parser.add_argument( "-p", "--plot",             metavar='SELECTION',                                  help="a string like '[2]-[1]' or '[2,*]-[1]' that specifies which stimulus codes to subtract from which others. Add flags for V or Z or S,  and B or N, to tweak how the signal is processed (see docstring for the `Processing` class)." )
	parser.add_argument( "-e", "--plotEach",         action='store_true',                                  help='whether to plot after reading each file' )
	parser.add_argument( "-y", "--yScale",           metavar='MAXABS',            default=None, type=float, help="absolute value indicating max. y scale for plot" )
	parser.add_argument( "-k", "--keep",             action='store_true',                                  help='whether to keep signals etc.' )
	parser.add_argument( "-s", "--superTitle",       metavar='TITLE',                                      help='optional supertitle on figure' )
	parser.add_argument( "-c", "--highlightChannel", metavar='CHANNEL',           default='auto',          help="specifies which channel to select initially in plots. The default, 'auto', selects the one with the largest peak. You can go semi-automatic by supplying a comma-separated list of channel names to consider.  Append '+' or '-' after the name(s) (or after 'auto') to restrict the search to positive or negative peaks. After the channel specification, optionally append (e.g.) @300 or @80-150 to highlight a particular millisecond latency, or to search for the peak within a specified latency range. So the string Cz-@80-150 might be good for the N1 potential, and Pz+@250-600 might be good for the P3." )
	parser.add_argument( "-m", "--channelMap",       metavar='FILENAME',                                   help="optional file" )
	parser.add_argument( "-o", "--output",           metavar='FILENAME',                                   help='filename for figures' )
	parser.add_argument( "-x", "--close",            metavar='FIGURE_NUMBERS',                             help='close the specified figures before plotting (can be a comma-separated list of numbers and/or ranges like "1-5", and/or the word "all")' )
	#parser.add_argument( "-w", "--welchWindowSec",   metavar='SEC',              default=None, type=float, help="triggers analysis of spectral instead of time-domain features" )
	parser.add_argument(       "--removeChannels",   metavar='CHANNELS',                                   help='comma-separated list of channels to remove' )
	parser.add_argument(       "--help-processing",  action='store_true',                                  help="print documentation for the `Processing` class (which decodes the shorthand strings you pass to `--plot`) and exit" )
	OPTS = parser.parse_args()

# standard-library imports
import os
import re
import ast
import sys
import glob
import copy
import time

# third-party imports (and comments indicating how to install them)
import numpy                      # python -m conda install numpy          or    python -m pip install numpy
import matplotlib                 # python -m conda install matplotlib     or    python -m pip install matplotlib
import scipy.signal, scipy.stats  # python -m conda install scipy          or    python -m pip install scipy

# import our own modules
# NB: we'll make these absolute imports so it's possible to run this file
from BCI2000Tools.FileReader import bcistream, DivertWarnings
from BCI2000Tools.Numerics   import events, match_events, refrac, epochs
from BCI2000Tools.Plotting   import load_plt, finish_plt, cax, look, hotfig, resolve_figure, resolve_axes, savepdf, imagesc, closefigs
from BCI2000Tools.Container  import Bunch
from BCI2000Tools.Electrodes import ChannelSet

__all__ = [ # symbols listed here will be the ones the user gets when they `import *` from here
	'EpochSet',
	'Processing',
	'siqr', 
	'Pearson', 
	'NoDataError', 
	'FixScaling',
]

def siqr( x, q=0.25, axis=-1 ):
	"""
	Semi-interquaNNNtile or (with default q=0.25) semi-interquaRRRtile
	range width of `x`, computed along the specified `axis`.
	
	`q` is the pair of quantiles that define the range, e.g. `[0.25, 0.75]`.
	If you supply a single scalar, then `[q, 1-q]` is used.
	
	If you supply a *negative* scalar, that's a shorthand for
	`norm.cdf(q)`, so for example `q=-1` is equivalent to `q=[0.159,0.841]`
	which would be +/-1 std in a normal distribution, `q=-2` gets you
	`[0.023,0.977]`, etc.
	
	So, `siqr(x,q=-1)` gets you a non-parametric, robust estimate of
	the spread of `x` that is comparable in coverage to the standard deviation
	of a normal distribution. (In fact, if `x` is normally distributed, this
	measure converges to the std for numerous-enough `x`---and to the extent that
 	`x` *isn't* normal, why would you even trust the usual std that is computed
	according to the parametric assumption that it *is*? Provided the number of
	samples in `x` is large, just use this instead from the get-go.)
	"""
	if numpy.asarray( q ).size == 1:
		if q < 0: q = scipy.stats.norm.cdf( q )
		q = sorted( [ q, 1 - q ] )
	return 0.5 * numpy.diff( numpy.quantile( x, q, axis=axis ), axis=axis )

def Pearson( x, y, axis=0 ):
	x = numpy.asarray( x, dtype=float ).view()
	y = numpy.asarray( y, dtype=float ).view()
	x.shape = list( x.shape ) + [ 1 ] * ( len( y.shape ) - len( x.shape ) )
	y.shape = list( y.shape ) + [ 1 ] * ( len( x.shape ) - len( y.shape ) )
	xm = numpy.expand_dims( x.mean( axis=axis), axis )
	ym = numpy.expand_dims( y.mean( axis=axis), axis )
	xs = numpy.expand_dims( x.std(  axis=axis), axis )
	ys = numpy.expand_dims( y.std(  axis=axis), axis )
	x = x - xm # creates a new array (up till now, still potentially a view of the old data)
	y = y - ym # creates a new array (up till now, still potentially a view of the old data)
	xs.flat[ xs.ravel() == 0 ] = 1.0
	ys.flat[ ys.ravel() == 0 ] = 1.0
	x /= xs
	y /= ys
	return ( x * y ).mean( axis=axis )

class NoDataError( ValueError ): pass
class EndUserError( Exception ): pass

def MatchFiles( pattern, sortByStorageTime=True ):
	if not isinstance( pattern, str ): return [ pattern ]
	pattern = pattern.replace( '@', '*' ) # so that you can define a pattern on the command line without IPython auto-expanding it
	pattern = os.path.expanduser( pattern )
	matches = glob.glob( pattern, recursive=True ) if '**' in pattern else glob.glob( pattern )
	sortKey = None
	if sortByStorageTime:
		def sortKey( fn ):
			try:
				with bcistream( fn ) as b: return ( b.params.StorageTime, fn )
			except:
				return ( '', fn )
	matches = sorted( [ os.path.realpath( match ) for match in matches ], key=sortKey )
	if not matches:
		errClass = EndUserError if __name__ == '__main__' else IOError
		if '*' in pattern: raise errClass( 'found no files matching %s' % pattern )
		else:              raise errClass( 'failed to find %s' % os.path.realpath( pattern ) )
	return matches

def FixScaling( *handles, **kwargs ):
	"""
	Pass figure numbers, Figure instances, magic AxesImage instances
	created by `BCI2000Tools.Electrodes.ChannelSet.rasterplot()`, or
	sequences-of any of the above.
	
	Keyword arguments are forwarded to `BCI2000Tools.Plotting.cax()`
	with `common` defaulting to True. To set limits explicitly, you
	might pass (say) `lim=[-0.2, +0.2]`.
	"""
	kwargs.setdefault( 'common', True )
	kwargs.setdefault( 'balance', True )
	if kwargs.get( 'lim', None ) == 'auto': kwargs[ 'auto' ] = True; kwargs.pop( 'lim' )
	plt = load_plt()
	if not handles and plt.get_fignums(): handles = [ plt.gcf() ]
	handles = sum( [ list( handle ) if hasattr( handle, '__iter__' ) else [ handle ] for handle in handles ] , [] )
	handles = [ plt.figure( n ) if isinstance( n, int ) else n for n in handles if not isinstance( n, int ) or n in plt.get_fignums() ]
	handles = [ ( [ img for ax in f.axes for img in ax.images if hasattr( img, 'chosenChannelIndex' ) ] + [ f ] )[ 0 ] if isinstance( f, plt.Figure ) else f for f in handles ]
	handles = [ x for x in handles if x is not None ]
	cax( handles=[ each if getattr( each, 'figure', None ) is None else each.figure for each in handles ], **kwargs )
	for each in handles:
		if getattr( each, 'chosenChannelIndex', None ) is not None:
			import matplotlib.pyplot as plt
			plt.figure( each.figure )
			each.select( each.chosenChannelIndex, each.chosenParamValue )
			
			
class Processing( Bunch ):
	"""
	Examples::
	
		 opts = Processing('*')     # select any trialType
		 opts = Processing('1')     # select only trialType==1
		 opts = Processing('2-1')   # select trialType==2 and contrast it against trialType==1
		 
		 opts = Processing('[2,*] - [1]' # select trialType==2 along with any other non-zero
										 # trialType that isn't 2 or 1, and contrast these
										 # against trialType==1
	
	For command-line friendliness you can use `...` or `@` as synonyms for `*`.
	You can also use any shape of brackets `()` `{}` or `[]`, or no brackets at all.
	
	You can append any of the symbols `~`, `!`, `^`, `D`, or `d` followed by a sequence of
	numeric trialTypes that you want to discard (these will not be used in a contrast, but
	also not picked up by `*`). Discarded trialType numbers must appear last, after any
	trialTypes that you want to keep.  Example:  '* - 1  ~2' means "discard trialType 2
	and contrast everything else against trialType 1".
	
	Include the following letters somewhere in your Processing string to change the way
	signals are extracted/contrasted:
	
		'V'  for plain voltages or voltage differences (no transformation) - this is the
			 default.
		
		'Z'  for z-scored voltages (voltage or voltage difference, divided by the
			 std-across-trials or std-across-differences)
		
		'S' or 'R'  for the SCD (signed coefficient of determination or "signed R-squared")
		
		'B'  for baseline subtraction (this is the default in V or Z mode)
		
		'N'  for no baseline subtraction  (this is the default in SCD mode)
			
	"""
	def __init__( self, spec ):
		spec = str( spec ).strip( '\'" \t\n' )
		snr = False
		baseline = False
		
		flags = dict(
			scd = dict( snr='SCD' ),
			s = dict( snr='SCD' ),
			r = dict( snr='SCD' ),
			v = dict( snr=False ),
			z = dict( snr=True ),
			b = dict( baseline=True ),
			n = dict( baseline=False ),
		)
		for flag, options in sorted( flags.items(), key=lambda item: -len( item[ 0 ] ) ):
			if flag.lower() in spec.lower():
				self.update( options )
				spec = re.sub( flag, '', spec, flags=re.IGNORECASE ).strip()
		
		spec = spec.replace( '~', '--' ).replace( '!', '--' ).replace( '^', '--' ).replace( 'D', '--' ).replace( 'd', '--' )
		spec += '-' # there will be at least two parts (second and/or later parts may be empty).
		spec = spec.split( '-' )
		try: spec = [
			ast.literal_eval(
				'[' + part.strip('[](){} \t').replace( '...', 'None' ).replace( '*', 'None' ).replace( '@', 'None' ) + ']'
				# NB: numpy.isin() will fail if you evaluate this as a set() in braces
			) for part in spec
		]
		except: raise ValueError( 'could not parse %r as a %s specifcation' % ( spec, self.__class__.__name__ ) )
		integersMentioned = [ number for part in spec for number in part if isinstance( number, int ) ]
		# NB: numpy.isin() will fail if integersMentioned is a set()
		if spec[ 0 ]: self[ 'select'  ] = lambda x, ids=spec[ 0 ]: numpy.isin( x, ids ) | ( ( None in ids ) & ( x != 0 ) & ~numpy.isin( x, integersMentioned ) )
		if spec[ 1 ]: self[ 'select2' ] = lambda x, ids=spec[ 1 ]: numpy.isin( x, ids ) | ( ( None in ids ) & ( x != 0 ) & ~numpy.isin( x, integersMentioned ) )
		self._spec = ' - '.join( repr( part ).replace( 'None', '*' ).replace( ' ', '' ) for part in spec )
	def __repr__( self ):
		return re.sub( r'\{', '{ # %s(%r)' % ( self.__class__.__name__, '  ' + self._spec + '  ' ), Bunch.__repr__( self ), count=1 )
		
class EpochSet( Bunch ):
	_summarize = 100
	_maxIndent = 1
	_newFigureSize = ( 10, 7 )
	def __init__( self,
		patterns,
		trigger='DigitalInput*',
		responseState=None,
		welchWindowSec=None,
		filterBand=( 0.5, 8 ), filterOrder=4,
		lookBackMsec=100, lookAheadMsec=800, refractoryMsec=800,
		badChannelThreshold=3.0, badEpochThreshold=3.0,
		channelMap=None, defaultReference=None, removeChannels=None, rereference=None, highlightChannel='auto', superTitle='',
		keep=False,
		plot=False, # if you set plot=True, it gets set to plot='{2,*} - 1 BS' but you can supply a different Processing string, Processing instance, dict, or list specifying a different [ select ] or [ select, select2 ]
		plotEach=False,
		yScale=None,
	 ):
	
		if isinstance( patterns, str ): patterns = patterns.split( '\n' )
		elif not isinstance( patterns, ( tuple, list ) ): patterns = [ patterns ]
		filenames = [ filename for pattern in patterns for filename in MatchFiles( pattern ) ]
		if len( filenames ) == 1:
			filename = filenames[ 0 ]
		else:
			plotEach = ( plot or plotEach ) if plotEach == 1 else plotEach
			objs = [ EpochSet( filename,
				trigger=trigger, responseState=responseState, welchWindowSec=welchWindowSec,
				filterBand=filterBand, filterOrder=filterOrder,
				lookBackMsec=lookBackMsec, lookAheadMsec=lookAheadMsec, refractoryMsec=refractoryMsec,
				badChannelThreshold=badChannelThreshold, badEpochThreshold=badEpochThreshold,
				channelMap=channelMap, defaultReference=defaultReference, removeChannels=removeChannels, rereference=rereference, highlightChannel=highlightChannel,
				superTitle=superTitle, keep=keep, plot=plotEach, plotEach=False, yScale=yScale,
			) for filename in filenames ]
			self.update( EpochSet.Combine( objs ) if objs else { k : [] for k in 'filenames warnings subjects datestamps'.split() } )
			self[ 'individualFiles' ] = objs
			print( '\nCollated warnings:\n' + '\n'.join( self.warnings ) )
			
			if plot and not objs: print( 'No data to plot' )
			elif plot == 'triggers': self.PlotTriggers( figure='new' )
			elif plot: self.Plot( plot, figure='new', highlightChannel=highlightChannel, superTitle=superTitle, yScale=yScale )
			if plotEach == 1 or plotEach and ( plotEach == plot ):
				handles = [ each.plotted.handle for each in self.individualFiles if getattr( each, 'plotted', None ) is not None ] + ( [ self.plotted.handle ] if getattr( self, 'plotted', None ) is not None else [] )
				if yScale is None: FixScaling( handles )
				
			return
			
		self.warnings = []
		DivertWarnings( self.warnings )
		if hasattr( filename, 'filename' ): b = filename; filename = b.filename
		else: b = bcistream( filename )
		self.filenames = [ filename ]
		print( '' ); print( b )
		self.subjects = [ b.params.SubjectName ]
		self.datestamps = [ b.params.StorageTime.replace( 'T', ' ' ) ]
		if keep: self.params = Bunch( b.params )
		if channelMap:
			self.channels = ChannelSet( channelMap )
			if len( self.channels ) != b.params.SourceCh:
				raise ValueError( 'channel map in %r specifies %d channels, whereas the SourceCh parameter in %r specifies %d' % ( channelMap, len( self.channels ), filename, b.params.SourceCh ) )
			if b.params.ChannelNames and '\n'.join( b.params.ChannelNames ).lower() != '\n'.join( self.channels.get_labels() ).lower():
				self.Warn( filename, 'ChannelNames parameter does not match the names in %r' % ( channelMap ) )
		else:
			self.channels = ChannelSet( b.params.ChannelNames )
		if defaultReference: self.channels.set_default_reference( defaultReference )
		self.samplesPerSecond = b.samplingfreq_hz
		try: iconRow, audioRow = '\n'.join( b.paramdefs.Stimuli[ 'dimlabels' ][ 0 ] ).lower().split( '\n' ).index( 'icon' ), '\n'.join( b.paramdefs.Stimuli[ 'dimlabels' ][ 0 ] ).lower().split( '\n' ).index( 'audio' )
		except: iconRow, audioRow = 1, 2
		row = iconRow if b.params.IconSwitch and list( set( b.params.Stimuli[ iconRow ] ) ) != [ '' ] else audioRow
		self.stimuli = [ os.path.splitext( os.path.basename( stimulusFilename ) )[ 0 ] for stimulusFilename in b.params.Stimuli[ row ] ]

		rawSignal, states = b.decode()
		b.close()
		seconds = b.samples2msec( numpy.arange( b.samples() ) ) / 1000.0	
		
		if removeChannels:
			removeChannelIndices = self.channels.find_labels( removeChannels )
			mask = numpy.array( [ x not in removeChannelIndices for x in range( self.channels.size ) ] )
			self.channels = self.channels[ mask ]
			rawSignal = rawSignal[ mask, : ]
		
		if rereference:
			if rereference.upper().replace( '+', '' ).replace( '-', '' )  == 'CAR':
				if '-' in rereference:
					stds = rawSignal.A.std( axis=1 )
					exclude = self.channels[ stds > numpy.median( stds ) + badChannelThreshold * siqr( stds, -1 ) ].get_labels()
					if exclude: self.Warn( filename, '%d channels excluded from common average reference (%s)' % ( len( exclude ), ','.join( exclude ) )  )
				elif '+' in rereference:
					exclude = []
				else:
					exclude = 'auto'
				rerefMatrix = self.channels.CAR( filters_as='rows', exclude=exclude )
			elif rereference.upper().startswith( 'SLAP' ): # --rereference=SLAP  (only good for frequency-domain mode)
				rerefMatrix = self.channels.SLAP( filters_as='rows', exclude='auto' )
			elif rereference.upper().startswith( 'SLAP(' ): # --rereference=SLAP(0.06)  (only good for frequency-domain mode) 
				rerefMatrix = self.channels.SLAP( float( rereference[ 4: ].strip( '() ' ) ), filters_as='rows', exclude='auto' )
			else:
				rerefMatrix = self.channels.RerefMatrix( rereference, filters_as='rows' )
			self.channels.spfilt( rerefMatrix, filters_as='rows' )
			rawSignal = numpy.matmul( rerefMatrix, rawSignal )
			
		if keep: self.rawSignal, self.states, self.seconds = rawSignal, Bunch( states )._set( _summarize=80 ), seconds
		self.trigger = trigger.replace( '@', '*' )
		if self.trigger in b.params.ChannelNames:
			triggerSignal = rawSignal.A[ b.params.ChannelNames.index( self.trigger ) ]
			self.triggerState = None
		else:
			stateName, bit = ( self.trigger + ':' ).split( ':' )[ :2 ]
			if '*' in stateName:
				candidates = [ candidate for candidate in states if re.match( stateName.replace( '*', '.*' ), candidate ) ]
				if candidates: stateName = max( candidates, key=lambda name: ( numpy.diff( numpy.asarray( states[ name ] ).flatten() ) != 0 ).sum() )
			triggerSignal = numpy.asarray( states[ stateName ] ).flatten()
			if bit:
				triggerSignal = ( triggerSignal & ( 2 ** abs( int( bit ) ) ) ) != 0 
				if int( bit ) < 0: triggerSignal = ~triggerSignal
			triggerSignal = triggerSignal.astype( float )
			self.triggerState = stateName
			
		self.responseState = responseState
		self.responseEvents = events( states[ self.responseState ] ) if self.responseState else []
			
		if keep: self.triggerSignal = triggerSignal
		self.hardwareEvents = events( triggerSignal )
		self.hardwareEvents = refrac( self.hardwareEvents, refractoryMsec / 1000.0, self.samplesPerSecond )
		self.hardwareEvents = numpy.array( self.hardwareEvents, dtype=int )
		self.softwareEvents = numpy.array( events( states[ 'StimulusCode' ] ), dtype=int )
		epochIndices = self.hardwareEvents
		self.usingHardwareEvents = True
		if not epochIndices.size:
			epochIndices = self.softwareEvents
			self.usingHardwareEvents = False
			self.Warn( filename, '!!! No hardware events found in %s - falling back on StimulusCode' % self.trigger )
			#epochIndices = epochIndices + b.msec2samples( expectedStimulusLatencyMsec )
			# NB: actually the first two such files seem to have very different latencies if you believe the EEG

		self.filterBand  = filterBand
		self.filterOrder = None if welchWindowSec else filterOrder
		if self.filterOrder and self.filterBand:
			filterB, filterA = scipy.signal.butter( self.filterOrder, self.filterBand, btype='bandpass', fs=self.samplesPerSecond )
			bandpassSignal = scipy.signal.filtfilt( filterB, filterA, rawSignal, method='gust', axis=1 )
		else:
			bandpassSignal = rawSignal
		if keep: self.bandpassSignal = bandpassSignal
		
		# consider only the epochIndices that match known softwareEvents to within, say 600ms (aims to eliminate extraneous trigger pulses)
		matchedEpochIndices = match_events( self.softwareEvents, epochIndices, min_offset=0, max_offset=0.6, fs=self.samplesPerSecond)
		matchedEpochIndices = [ ind for ind in matchedEpochIndices if ind is not None ]
		self.extraneousTriggerIndices = numpy.array( list( set( epochIndices ) - set( matchedEpochIndices ) ) )
		
		self.epochs, t, self.epochIndices = epochs(
			bandpassSignal,
			matchedEpochIndices,
			offset = -abs( lookBackMsec ) / 1000.0, # TODO: for frequency-domain analysis, you might actually want a positive offset here.... @@@
			length = ( abs( lookBackMsec ) + lookAheadMsec ) / 1000.0,
			fs = self.samplesPerSecond,
			return_array = True,
		)
		self.epochTimeMsec = t * 1000.0
		
		nDiscarded = len( matchedEpochIndices ) - len( self.epochIndices )
		if nDiscarded: self.Warn( filename, 'Epochs discarded: %d of %d' % ( nDiscarded, len( matchedEpochIndices ) ) )
		#self.trialType = states[ 'StimulusCode' ].flatten()[ self.epochIndices ]
		# but when audio latency is high, it's easily possible for StimulusCode to have gone back to 0.
		# So don't do it as above. Instead, find the last StimulusCode rising edge working back from each trigger point:
		matchedSoftwareEvents = match_events( self.epochIndices, self.softwareEvents, min_offset=None, max_offset=0 )
		self.trialType = states[ 'StimulusCode' ].flat[ matchedSoftwareEvents ]
		self.triggersMissing = self.softwareEvents.size - self.hardwareEvents.size
		if self.triggersMissing > 0 and self.usingHardwareEvents: self.Warn( filename, 'Triggers missing: %d' % self.triggersMissing )
		if self.triggersMissing < 0 and self.usingHardwareEvents: self.Warn( filename, 'Extraneous triggers: %d' % -self.triggersMissing )
		self.triggersMisaligned = int( ( self.trialType == 0 ).sum() ) # should be impossible now
		if self.triggersMisaligned: self.Warn( filename, '!!! TRIGGERS MISALIGNED: %d' % self.triggersMisaligned )
		self.stimulusLatenciesMsec = ( ( numpy.array( self.epochIndices ) - numpy.array( matchedSoftwareEvents ) ) / self.samplesPerSecond * 1000 ) if self.usingHardwareEvents else []
		
		self.badChannelThreshold = badChannelThreshold
		stds = rawSignal.A.std( axis=1 )
		self.channelIsBad = stds > numpy.median( stds ) + self.badChannelThreshold * siqr( stds, -1 )
		if any( self.channelIsBad ): self.Warn( filename, 'Bad channels: %d of %d (%s)' % ( self.channelIsBad.sum(), self.channelIsBad.size, ', '.join( numpy.array( self.channels.get_labels() )[ self.channelIsBad ] ) ) )
		
		self.badEpochThreshold = badEpochThreshold
		self.epochIsBad = numpy.array( [] )
		if self.epochs.size:
			stds = self.epochs[ :, ~self.channelIsBad, : ].std( axis=2 ).max( axis=1 )
			self.epochIsBad = stds > numpy.median( stds ) + self.badEpochThreshold * siqr( stds, -1 )
			if any( self.epochIsBad ): self.Warn( filename, 'Bad epochs: %d of %d' % ( self.epochIsBad.sum(), self.epochIsBad.size ) )
			
			self.epochs = self.epochs - numpy.median( self.epochs, axis=2, keepdims=True ) # subtracts median (zero-order "detrend") from every channel of every epoch
			
		if welchWindowSec:
			# TODO: need more flexibility in specifying where the epoch starts and ends relative to the trigger (might want to skip ahead instead of look back) @@@
			self.frequencyHz, psd = Spectrum( self.epochs, windowLengthInSeconds=welchWindowSec, samplesPerSecond=self.samplesPerSecond, windowShape='hann', overlap=0.5, axis=-1 )
			self.spectra = 10.0 * numpy.log10( numpy.maximum( 1e-6, psd ) )
			frequencyRange = [ 0.001, 40 ]  # TODO: need to parameterize this
			mask = self.frequencyHz <= max( frequencyRange )
			mask = numpy.logical_and( mask, self.frequencyHz >= min( frequencyRange ) )
			self.frequencyHz = self.frequencyHz[ mask ]
			self.spectra = self.spectra[ :, :, mask ]
	
		if plot == 'triggers': self.PlotTriggers( figure='new' )
		elif plot: self.Plot( plot, figure='new', highlightChannel=highlightChannel, superTitle=superTitle, yScale=yScale )
		
		
		
	def Warn( self, filename, msg ):
		filestem = os.path.splitext( os.path.basename( filename ) )[ 0 ]
		msg = 'WARNING (%s): %s' % ( filestem, msg )
		self.warnings.append( msg )
		print( msg )
	
	def Baselined( self, x ):
		subs = [ slice( None ) for dim in x.shape ]
		subs[ -1 ] = ( self.epochTimeMsec < 0.0 )
		return x - numpy.median( x[ tuple( subs ) ], axis=-1, keepdims=True )
	
	def Select( self, selectFunc=None, excludeBadEpochs=True ):
		"""
		`selectFunc` is a callable that takes a numpy array of integers
		(`self.trialType` will get passed into it) and returns an identically-
		shaped array of booleans, indicating which epochs to select.
		
		As a shortcut instead of a callable, `selectFunc` may be an integer or
		list of integers, dictating which `.trialType` values to select.
		
		With `excludeBadEpochs=True`, epochs will be deselected if they have
		been identified as bad in `self.epochIsBad`.
		"""
		if not self.epochs.size: return numpy.array( [], dtype=bool )
		if isinstance( selectFunc, ( float, int ) ): selectFunc = [ selectFunc ]
		try: listOfNumbers = list( selectFunc )
		except: pass
		else: selectFunc = lambda x: numpy.isin( x, listOfNumbers )
		selected = ~self.epochIsBad
		if not excludeBadEpochs: selected.flat = True
		if selectFunc: selected = numpy.logical_and( selected, selectFunc( self.trialType ) )
		return selected
		
	def Mean( self, selectFunc=None, excludeBadEpochs=True ):
		return self.epochs[ self.Select( selectFunc=selectFunc, excludeBadEpochs=excludeBadEpochs ), :, : ].mean( axis=0 )
		
	def Std( self, selectFunc=None, excludeBadEpochs=True ):
		return self.epochs[ self.Select( selectFunc=selectFunc, excludeBadEpochs=excludeBadEpochs ), :, : ].std( axis=0, ddof=1 )
	
	def Title( self ):
		if len( self.filenames ) == 1:
			t = os.path.basename( self.filenames[ 0 ] )
		else:
			t = '%d files' % len( self.filenames )
			if len( set( self.subjects ) ) == 1: t += ' (subject %s)' % re.sub( '-Oddball$', '', self.subjects[ 0 ] )
			else: t += ' (%d subjects)' % len( set( self.subjects ) )
		earliest = min( self.datestamps )	
		latest = max( self.datestamps )	
		if earliest == latest: t += '\n' + earliest
		elif earliest.split()[ 0 ] == latest.split()[ 0 ]: t += '\n%s' % earliest.replace( ' ', ', from ' )
		else: t += '\n%s -- %s' % ( earliest.split()[ 0 ], latest.split()[ 0 ] )
		return t

	def PlotTriggers( self, maxTrialType=3, axes=None, figure=None ):
		if maxTrialType is None: maxTrialType = numpy.inf
		sc = self.states.StimulusCode[ 0 ].astype( float )
		sc[ sc > maxTrialType ] = maxTrialType
		trigger = sc * numpy.nan
		for iSample, trialType in zip( self.epochIndices, self.trialType ):
			trigger[ iSample - 1 ] = 0.0
			trigger[ iSample ] = min( maxTrialType, trialType ) * 1.01
		
		load_plt()
		axes = resolve_axes( axes, figure, self._newFigureSize, cla=True )
		axes.plot( self.seconds, sc, self.seconds, trigger )
		axes.set_title( self.Title() )

	def StimulusCodeString( self, tt ):
		if callable( tt ): tt = tt( self.trialType )
		dtype = getattr( tt, 'dtype', None )
		if dtype is not None:
			if dtype == bool: tt = sorted( set( self.trialType[ tt ] ) )
			else: tt = sorted( set( tt.ravel().tolist() ) )
		if isinstance( tt, ( tuple, list, set ) ):
			tt = list( tt )
			if len( tt ) == 1: return self.StimulusCodeString( tt[ 0 ] )
			if len( tt ) <= 3: return '{%s}' % ','.join( self.StimulusCodeString( x ) for x in tt )
			return '{%s and %d others}' % ( self.StimulusCodeString( tt[ 0 ] ), len( tt ) - 1 )
		return self.stimuli[ int( tt ) - 1 ]

	def Plot(    self, select=None, select2=None, snr=False, baseline='auto', excludeBadEpochs=True, figure=None, highlightChannel='auto', superTitle='', minEpochsPerClass=4, yScale=None ):
		try: return self.Summary( select=select, select2=select2, snr=snr, baseline=baseline, excludeBadEpochs=excludeBadEpochs, plot=figure, highlightChannel=highlightChannel, superTitle=superTitle, minEpochsPerClass=minEpochsPerClass, yScale=yScale )
		except NoDataError as err: print( 'WARNING: %s - nothing to plot in %s' % ( err, self.Title().replace( '\n', ' / ' ) ) )
			
	def Summary( self, select=None, select2=None, snr=False, baseline='auto', excludeBadEpochs=True, plot=False, highlightChannel='auto', superTitle='', minEpochsPerClass=0, yScale=None, returnPlotArgs=True ):
		"""
		`select` is any valid argument to `.Select()`: i.e., an integer, a list
		of integers, or a callable that `self.trialType` can be passed into.
		
		`select2` is similar, for optionally specifying the second class against
		which to compare the first (i.e. to be subtracted from the first).
		
		`excludeBadEpochs` is passed through to `.Select()`.
		
		With `baseline=True` the median-across-time-samples-before-time-zero
		is computed for, and subtracted from, each epoch and channel, using
		the `.Baselined()` method.
		
		With `snr=False`, you get a mean-across-epochs (or difference between
		the two classes' means-across-epochs if there are two classes).
		
		With `snr=True`, you get a z-score: mean-across-epochs divided by
		std-across-epochs (or a d-prime value if there are two classes---i.e.
		the difference of means-across-epochs divided by std of differences).
		
		With `snr='SCD'` you get a signed coefficient of determination (i.e.,
		signed r-squared) and that will only work with two classes.
		
		"""
		
		if select == 1:
			select = '{2,*} - {1} BS' # Hill oddball contrast
		if isinstance( select, str ):
			select = Processing( select ) # this is a dict subclass
		if isinstance( select, dict ):
			interpretation = select
			select           = interpretation.get( 'select',           None )
			select2          = interpretation.get( 'select2',          None )
			snr              = interpretation.get( 'snr',              snr )
			baseline         = interpretation.get( 'baseline',         baseline )
			excludeBadEpochs = interpretation.get( 'excludeBadEpochs', excludeBadEpochs )
			highlightChannel = interpretation.get( 'highlightChannel', highlightChannel )
			superTitle       = interpretation.get( 'superTitle',       superTitle )
			
		useFrequencyDomain = self.get( 'spectra', None ) is not None
		if useFrequencyDomain: baseline = False
		
		isSCD = snr in [ 'scd', 'SCD' ]
		if baseline == 'auto': baseline = not isSCD

		select = self.Select( select, excludeBadEpochs=excludeBadEpochs )
		n = select.sum()
		if not n: raise NoDataError( 'found no trials matching first criteria' )
		if n < minEpochsPerClass: raise NoDataError( 'found fewer than %d trials matching second criteria' % minEpochsPerClass )
		z = ( self.spectra if useFrequencyDomain else self.epochs )[ select, :, : ]
		if baseline:
			z = self.Baselined( z )
			if snr: z[ :, :, self.epochTimeMsec < 0 ] = 0.0 # spurious patterns may emerge in SNR or SCD in the baseline period because baselining has made the std() denominators so artificially low in that region => let's just blank it out
		s = z.std( axis=0, ddof=1 if z.shape[ 0 ] > 1 else 0 )
		m = z.mean( axis=0 )
		
		n2 = 0
		if select2:
			select2 = self.Select( select2, excludeBadEpochs=excludeBadEpochs )
			n2 = select2.sum()
			if not n2: raise NoDataError( 'found no trials matching second criteria' )
			if n2 < minEpochsPerClass: raise NoDataError( 'found fewer than %d trials matching second criteria' % minEpochsPerClass )
			z2 = ( self.spectra if useFrequencyDomain else self.epochs )[ select2, :, : ]
			if baseline:
				z2 = self.Baselined( z2 )
				if snr: z2[ :, :, self.epochTimeMsec < 0 ] = 0.0 # spurious patterns may emerge in SNR or SCD in the baseline period because baselining has made the std() denominators so artificially low in that region => let's just blank it out
			s = ( s ** 2.0 + z2.std( axis=0, ddof=1 if z2.shape[ 0 ] > 1 else 0 ) ** 2.0 ) ** 0.5
			m -= z2.mean( axis=0 )
			
		s.flat[ s.ravel() == 0.0 ] = 1.0
		if isSCD:
			if not n2: raise ValueError( 'SCD needs two classes' )
			y1 = +numpy.ones_like(  z[ :, [ 0 ], [ 0 ] ] )
			y2 = -numpy.ones_like( z2[ :, [ 0 ], [ 0 ] ] )
			yy = numpy.concatenate( [ y1, y2 ], axis=0 )
			zz = numpy.concatenate( [  z, z2 ], axis=0 )
			r = Pearson( zz, yy, axis=0 )
			m = r * numpy.abs( r )
		elif snr:
			m /= s  # will generate divide-by-zero warnings if you only have 1 trial
		
		tt1 = sorted( set( self.trialType[ select  ] ) )
		tt2 = sorted( set( self.trialType[ select2 ] ) ) if select2 is not None else []
		
		bcstr = ' (baseline-corrected)' if baseline else ''
		if isSCD:        description = 'SCD %s\n%s vs. %s' % ( bcstr, self.StimulusCodeString( tt1 ), self.StimulusCodeString( tt2 ) )
		elif snr and n2: description = 'SNR %s\n%s - %s'   % ( bcstr, self.StimulusCodeString( tt1 ), self.StimulusCodeString( tt2 ) )
		elif snr:        description = 'SNR %s\n%s'        % ( bcstr, self.StimulusCodeString( tt1 ) )
		elif n2:         description = 'Mean %s\n%s - %s'  % ( bcstr, self.StimulusCodeString( tt1 ), self.StimulusCodeString( tt2 ) )
		else:            description = 'Mean %s\n%s'       % ( bcstr, self.StimulusCodeString( tt1 ) )
		if n2: description += '\n%d + %d = %d epochs' % ( n, n2, n + n2 )
		else: description += '\n%d epochs' % n
		
		
		physicalUnit = r'dB' if useFrequencyDomain else r'$\mu{}$V'
		result = Bunch(
			img = m,
			x = self.frequencyHz if useFrequencyDomain else self.epochTimeMsec,
			xlabel = 'Hz' if useFrequencyDomain else 'msec',
			colorbar = True,
			balance = None if ( useFrequencyDomain and not n2 ) else 0.0,
			interactive = True,
			colorbartitle = 'SCD' if isSCD else 'z-score' if snr else r'$\Delta{}$ ' + physicalUnit if n2 else physicalUnit, 
			title = self.Title() + '\n' + description,
		)
		result._summarize = self._summarize
		if highlightChannel:
			peak = self[ 'highlighted' ] = self.FindPeak( result.img, highlightChannel )
		if plot != False:
			if plot == True: plot = None
			plt = load_plt()
			figure = resolve_figure( plot, self._newFigureSize, clf=True, scf=True )
			result[ 'handle' ] = self.channels.rasterplot( **result )
			result.handle.axes.title.set( ha='left', x=0, fontsize=10 )
			result.handle.topoAxes.title.set( ha='right', x=1, transform=matplotlib.transforms.blended_transform_factory( result.handle.paramAxes.get_yaxis_transform(), result.handle.topoAxes.get_xaxis_transform() ) )
			result.handle.paramAxes.title.set( ha='right', x=1 )
			figure.tight_layout( pad=1.2 )
			if superTitle: figure.suptitle( superTitle, fontsize=20 )
			self[ 'plotted' ] = result
			if highlightChannel: result.handle.select( peak[ 'peakChannelIndex' ], peak[ 'peakParamValue' ] )
			if yScale is not None: FixScaling( [ result.handle.figure ], lim=[ -abs( yScale ), +abs( yScale ) ] )
		return result if returnPlotArgs else self
	Plot.__doc__ = Summary.__doc__
		
	def FindPeak( self, img, channels=None, paramRange=None, polarity=None ):
	
		useFrequencyDomain = self.get( 'spectra', None ) is not None
		paramValues = self.frequencyHz if useFrequencyDomain else self.epochTimeMsec
		modifiedImg = img * numpy.nan
		
		if isinstance( channels, str ):
			if '@' in channels:
				channels, delim, paramRangeFromString = channels.rpartition( '@' )
				if paramRange is None and paramRangeFromString.strip(): paramRange = paramRangeFromString.strip()
			channels = channels.strip()
			if   channels.endswith(   ( '-', '+' ) ): channels, polarityFromString = channels[ :-1 ], channels[ -1 ]
			elif channels.startswith( ( '-', '+' ) ): channels, polarityFromString = channels[ 1:  ], channels[  0 ]
			else: polarityFromString = None
			if polarity is None: polarity = polarityFromString
			channels = channels.replace( ',', ' ' ).strip()
			if channels.lower() == 'auto': channels = 'all'
			channels = channels.split()
			if 'all' in channels or '*' in channels: channels = None
		if isinstance( channels, int ):
			channels = [ channels ]
		if channels is None:
			channels = numpy.arange( self.channels.size )
		else:
			channels = self.channels.find_labels( channels )
		modifiedImg[ channels, : ] = img[ channels, : ]
				
		if isinstance( paramRange, str ):
			paramRange = paramRange.strip()
			if paramRange.lower() == 'auto' or not paramRange:
				paramRange = None
			else:
				match = re.match(r'(\-?\s*[\d\.eE]+)[\-:,](\-?\s*[\d\.eE]+)', paramRange )
				if match: paramRange = [ float( x ) for x in match.groups() ]
				else: paramRange = float( paramRange )
		if isinstance( paramRange, ( int, float ) ):
			paramRange = [ paramRange, paramRange ]
		if paramRange is None: paramRange = paramValues
		paramRange = ( min( paramRange ), max( paramRange ) )
		minParam = min( paramValues, key=lambda v: abs( v - min( paramRange ) ) )
		maxParam = min( paramValues, key=lambda v: abs( v - max( paramRange ) ) )	
		modifiedImg[ :, paramValues < minParam ] = numpy.nan
		modifiedImg[ :, paramValues > maxParam ] = numpy.nan
			
		if   polarity in [ '+',    +1, 'positive', numpy.positive       ]: polarity = numpy.positive
		elif polarity in [ '-',    -1, 'negative', numpy.negative       ]: polarity = numpy.negative
		elif polarity in [ 'abs', abs, 'absolute', numpy.absolute, None ]: polarity = numpy.absolute
		else: raise ValueError( "polarity must be '+', '-' or 'abs'" )
		modifiedImg = polarity( modifiedImg )
		
		channelIndex, paramIndex = numpy.unravel_index( numpy.nanargmax( modifiedImg ), modifiedImg.shape )
		return Bunch(
			channelsConsidered = tuple( self.channels.flat[ channels ].get_labels() ),
			paramRangeConsidered = paramRange,
			peakPolarity = polarity.__name__,
			peakChannelIndex = channelIndex,
			peakParamIndex = paramIndex,
			peakChannelLabel = self.channels.flat[ channelIndex ].get_label(),
			peakParamValue = float( paramValues[ paramIndex ] ),
			peakValue = img[ channelIndex, paramIndex ],
		)
		
	
	@classmethod
	def Combine( cls, objs ):
		class absent: pass
		plotted = objs[ 0 ].pop( 'plotted', absent ) # may contain a matplotlib handle - deepcopy() would wreak havoc
		out = copy.deepcopy( objs[ 0 ] )
		out.pop( 'highlighted', None )
		out.channels = objs[ 0 ].channels.copy() # deepcopy messes up ChannelSet coordinates for some reason...
		out.extraneousTriggerIndices = [ obj.extraneousTriggerIndices for obj in objs ]
		if plotted is not absent: objs[ 0 ][ 'plotted' ] = plotted
		
		for field in 'params states seconds rawSignal triggerSignal bandpassSignal'.split(): out.pop( field, None )
		for obj in objs[ 1: ]:
			for field in 'filenames warnings subjects datestamps triggersMissing triggersMisaligned'.split(): out[ field ] += obj[ field ]
			for field in 'channelIsBad'.split(): out[ field ] |= obj[ field ]
			for field in 'hardwareEvents softwareEvents stimulusLatenciesMsec responseEvents epochs spectra epochIndices trialType epochIsBad'.split():
				if ( out.get( field, None ) is None ) != ( obj.get( field, None ) is None ): raise ValueError( 'cannot combine a dataset that has %r with one that does not' % field )
				if obj.get( field, None ) is not None and len( obj[ field ] ): out[ field ] = numpy.concatenate( [ out[ field ], obj[ field ] ], axis=0 )
			for field in 'stimuli! channels! samplesPerSecond! usingHardwareEvents! epochTimeMsec! frequencyHz! filterBand filterOrder badChannelThreshold badEpochThreshold triggerState'.split():
				strict = field.endswith( '!' )
				field = field.strip( '!' )
				a, b = out.get( field, None ), obj.get( field, None )
				if a is None and b is None: continue
				if a is None or b is None: raise ValueError( 'cannot combine a dataset that has %r with one that does not' % field )
				if isinstance( a, ChannelSet ): a, b = a.get_labels(), b.get_labels()
				a, b = numpy.array( a ), numpy.array( b )
				if list( a.shape ) != list( b.shape ) or ( a != b ).sum():
					if strict: raise ValueError( 'cannot combine runs with inconsistent values for %r' % field )
					message = 'WARNING: combining runs with inconsistent values for %r' % field
					out.warnings.append( message )
					print( message )
		return out
	
	def ComputeResponses( self, select, minResponseMsec=100, maxResponseMsec=1000 ):
		if 'individualFiles' in self:
			return sum( [ each.ComputeResponses( select=select, minResponseMsec=minResponseMsec, maxResponseMsec=maxResponseMsec ) for each in self.individualFiles ], [] )
		if isinstance( select, ( int, list, tuple, str ) ):
			select = Processing( select )[ 'select' ]
		if callable( select ):
			select = select( self.trialType )
		if select.shape != self.trialType.shape or select.dtype != bool:
			raise ValueError( 'select should be a boolean array the same shape as self.trialType' )
		stimulusEvents = self.hardwareEvents[ :select.size ][ select ]
		matchingResponseEvents = match_events( stimulusEvents, self.responseEvents, min_offset=minResponseMsec / 1000.0, max_offset=maxResponseMsec / 1000.0, fs=self.samplesPerSecond )
		responseTimeInMsec = ( numpy.array( matchingResponseEvents, dtype=float ) - stimulusEvents ) / self.samplesPerSecond * 1000.0
		return [ Bunch( filename=self.filenames[ 0 ], sampleIndexOfStimulus=si, sampleIndexOfResponse=ri, trialType=tt, responseTimeInMsec=rt, missing=ri is None ) for si, ri, tt, rt in zip( stimulusEvents, matchingResponseEvents, self.trialType[ select ], responseTimeInMsec ) ]
	
	def Raster( self, channel, selectionString='*', excludeBadEpochs=True, axes=None, figure=None, plot=False ):
		useFrequencyDomain = self.get( 'spectra', None ) is not None
		opts = Processing( selectionString )
		channelIndex = self.channels.find_labels( channel )[ 0 ]
		epochs = epochIndices = None
		epochIndices = self.Select( opts[ 'select'  ], excludeBadEpochs=excludeBadEpochs ).nonzero()[ 0 ]
		if opts.get( 'snr', None ): print( 'WARNING: ignoring snr=%r option indicated by selectionString %r' % ( opts[ 'snr' ], selectionString ) )
		if opts.get( 'select2', None ): print( 'WARNING: ignoring select2 option indicated by selectionString %r' % ( selectionString ) )
		epochs = self.epochs[ :, channelIndex, : ][ epochIndices ]
		title = self.Title() + '\n' + channel + ', ' + self.StimulusCodeString( self.trialType[ epochIndices ] )
		if getattr( opts, 'baseline', True ):
			epochs = self.Baselined( epochs )
			title += ' (baseline-corrected)'
		
		axes = resolve_axes( axes, figure, self._newFigureSize, cla=True ) if plot else None
		out = Bunch()
		out[ 'args' ] = Bunch(
			img = epochs,
			x = ( self.frequencyHz if useFrequencyDomain else self.epochTimeMsec ).copy(),
			#y = epochIndices, # cannot do this because y needs to be evenly spaced
			xlabel = 'Frequency (Hz)' if useFrequencyDomain else 'Time (ms)',
			title = title,
			colorbar = True,
			balance = None if useFrequencyDomain else 0.0,
			aspect = 'auto',
			axes = axes,
		) 
		out[ 'handle' ] = imagesc( **out[ 'args' ] ) if plot else None
		out[ 'epochIndices' ] = epochIndices
		out._summarize = out[ 'args' ]._summarize = 80
		return out
		
def Spectrum( signal, windowLengthInSeconds, samplesPerSecond, windowShape='hann', overlap=0.5, axis=-1 ):
	windowLengthInSamples = int( round( windowLengthInSeconds * samplesPerSecond ) )
	overlapInSamples = None if overlap is None else int( round( overlap * windowLengthInSeconds * samplesPerSecond ) )
	return scipy.signal.welch(
		signal, fs=samplesPerSecond, axis=axis,
		window=windowShape, nperseg=windowLengthInSamples, noverlap=overlapInSamples, nfft=None,
		detrend='constant', return_onesided=True, scaling='density', average='mean',
	)
		
if __name__ == '__main__':
	if OPTS.__dict__.pop( 'help_processing', 0 ):
		print( Processing.__doc__ )
		raise SystemExit( 0 )
	output = OPTS.__dict__.pop( 'output' )
	close = OPTS.__dict__.pop( 'close' )
	if not OPTS.patterns: raise SystemExit( 'no .dat files specified' )
	if close: closefigs( close )
	try: self = epochs = EpochSet( **OPTS.__dict__ )
	except EndUserError as error: raise SystemExit( error )
	if ( OPTS.plot or OPTS.plotEach ) and epochs.filenames:
		if output:
			figures = [ each.plotted.handle.figure for each in epochs.individualFiles if getattr( each, 'plotted', None ) is not None ] + ( [ epochs.plotted.handle.figure ] if getattr( epochs, 'plotted', None ) is not None else [] )
			savepdf( output, figures )
		hotfig()
		finish_plt()
