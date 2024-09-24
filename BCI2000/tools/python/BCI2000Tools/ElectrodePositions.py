# -*- coding: utf-8 -*-

# ///////////////////////////////////////////////////////////////////////////
# $Id: ElectrodePositions.py 7894 2024-03-04 22:04:38Z jhill $
# Author: jeremy.hill@neurotechcenter.org
# Description: tables of standard EEG electrode names and plotting positions
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

# TODO: limit the way coords files override each other
#       (e.g. having wacky EGI positions on the path can mess things up
#       when trying to plot other montages)

"""
TODO
"""
__all__ = [
	'GetScheme',
	'GetSchemes',
	'GetCoords',
	'Rescan',
]

import os
import re
import sys
import glob
import math
import collections; from collections import OrderedDict

if sys.version < '3': bytes = str
else: unicode = str; basestring = ( unicode, bytes )

from . import Bootstrapping; from .Bootstrapping import WhereAmI
from . import Container;     from .Container     import Bunch

PATH = [ WhereAmI() ]
CACHE = {}
SCHEME_FILE_IDENTIFIER = 'scheme'

class ElectrodeParsingError( Exception ): pass

BUILTIN_SCHEMES = {
	# Basics
	'nose':           'NAS',
	'ears':           'A1 A2',
	'eyes_bipolar':   'EH1 EV1 EH2 EV2',
	'eyes_monopolar': 'EL1 EO1 EU1 EI1 EL2 EO2 EU2 EI2',
	'extended1020':   'Fp1 Fpz Fp2 AF7 AF3 AFz AF4 AF8 F9 F7 F5 F3 F1 Fz F2 F4 F6 F8 F10 FT9 FT7 FC5 FC3 FC1 FCz FC2 FC4 FC6 FT8 FT10 T9 T7 C5 C3 C1 Cz C2 C4 C6 T8 T10 TP9 TP7 CP5 CP3 CP1 CPz CP2 CP4 CP6 TP8 TP10 P9 P7 P5 P3 P1 Pz P2 P4 P6 P8 P10 PO9 PO7 PO3 POz PO4 PO8 PO10 O9 O1 Oz O2 O10 Iz',
	'old1020':        'Fp1 Fp2 F7 F3 Fz F4 F8 T3 C3 Cz C4 T4 T5 P3 Pz P4 T6 O1 O2', # classic arrangement, old idiosyncratic names
	'new1020':        'Fp1 Fp2 F7 F3 Fz F4 F8 T7 C3 Cz C4 T8 P7 P3 Pz P4 P8 O1 O2', # same arrangement, more-regular names
	
	# A couple of specific examples
	'biosemi64':      'A01 A02 A03 A04 A05 A06 A07 A08 A09 A10 A11 A12 A13 A14 A15 A16 A17 A18 A19 A20 A21 A22 A23 A24 A25 A26 A27 A28 A29 A30 A31 A32 B01 B02 B03 B04 B05 B06 B07 B08 B09 B10 B11 B12 B13 B14 B15 B16 B17 B18 B19 B20 B21 B22 B23 B24 B25 B26 B27 B28 B29 B30 B31 B32',
	'berlin118':      'Fp1 AFp1 Fpz AFp2 Fp2 AF7 AF3 AF4 AF8 FAF5 FAF1 FAF2 FAF6 F7 F5 F3 F1 Fz F2 F4 F6 F8 FFC7 FFC5 FFC3 FFC1 FFC2 FFC4 FFC6 FFC8 FT9 FT7 FC5 FC3 FC1 FCz FC2 FC4 FC6 FT8 FT10 CFC7 CFC5 CFC3 CFC1 CFC2 CFC4 CFC6 CFC8 T7 C5 C3 C1 Cz C2 C4 C6 T8 CCP7 CCP5 CCP3 CCP1 CCP2 CCP4 CCP6 CCP8 TP9 TP7 CP5 CP3 CP1 CPz CP2 CP4 CP6 TP8 TP10 PCP7 PCP5 PCP3 PCP1 PCP2 PCP4 PCP6 PCP8 P9 P7 P5 P3 P1 Pz P2 P4 P6 P8 P10 PPO7 PPO5 PPO1 PPO2 PPO6 PPO8 PO7 PO3 PO1 POz PO2 PO4 PO8 OPO1 OPO2 O1 Oz O2 OI1 OI2 I1 I2',
}


def GetScheme( scheme, interpretAsFileName='auto' ):
	"""
	Get a list of electrode names, for a given "scheme",
	i.e. a set of electrodes that belong together.
	
	May read a single file (usually named according to the
	convention *.scheme.txt), or may use `GetSchemes()` to
	scan for known schemes and then specify the name of a
	single scheme to pick.
	"""
	if interpretAsFileName == 'auto': interpretAsFileName = '/' in scheme or '\\' in scheme or '.' in scheme
	if not interpretAsFileName: return GetSchemes()[ scheme ]
	return ParseElectrodeSpecification( scheme, interpretAsFileName=True, returnType='scheme' )
	
def GetSchemes( rescan=False ):
	"""
	Return a `dict` containing named lists of electrodes that
	belong together (partial or complete sets).
	
	The data are partly hardcoded, but extensible by putting
	files named `$SCHEMENAME.scheme.txt` in directories that
	are included on the global list `PATH`. Disregarding
	comments starting with `#`, each word of the file is
	considered to be an electrode name.
	
	Once the `PATH` has been searched for schemes once, the
	results are cached and the path will not be searched on
	subsequent calls.  To search the path again, call with
	`rescan=True` or call the function `Rescan()`.
	"""
	if not rescan and SCHEME_FILE_IDENTIFIER in CACHE: return CACHE[ SCHEME_FILE_IDENTIFIER ]
	
	# First, some built-in ones
	schemes = dict( BUILTIN_SCHEMES )
	
	# Additionally, look for any SCHEMENAME.scheme.txt file in any of the directories
	# on the PATH and add their plain text contents (remove # comments from each line,
	# then collapse all whitespace to one space)
	for stem, filename in GetFiles( '.' + SCHEME_FILE_IDENTIFIER + '.txt' ):
		schemes[ stem ] = GetScheme( filename, interpretAsFileName=True )
	CACHE[ SCHEME_FILE_IDENTIFIER ] = schemes
	return schemes

def GetCoords( type='schematic2D', interpretAsFileName='auto', rescan=False ):
	"""
	Return a `dict` mapping electrode names to coordinates.
	
	The coordinates may be read from a single named text
	file: disregarding comments starting with `#`, each line
	of the file corresponds to one electrode and provides
	the electrode name followed by numeric coordinates
	(or an electrode name followed by a single other
	electrode name to which this name is to be aliased).
	
	Alternatively, you may supply a more-generic `type`
	of coordinate system, and collate coordinate data
	from all known sources. These data are partly
	hardcoded in this module, but are also extensible
	by putting files named  `*.$TYPE.txt` in directories
	that are included on the global list `PATH`.
	
	In this case, once the `PATH` has been searched for
	coordinates of a given `type`, the results are cached
	and the path will not be searched for this type on
	subsequent calls.  To search the path again, call
	with `rescan=True` or call the function `Rescan()`.
	"""
	if interpretAsFileName == 'auto': interpretAsFileName = '/' in type or '\\' in type or '.' in type
	if interpretAsFileName: return ParseElectrodeSpecification( type, interpretAsFileName=True, returnType='coords' )
	
	if not rescan and type in CACHE: return CACHE[ type ]
	
	# First, some built-in ones (see global definition after this function)
	coords = dict( schematic2D=dict( BUILTIN_SCHEMATIC2D ) ).get( type, {} )
	aliases = {}
	
	# Additionally, look for any *.TYPE.txt file in any of the directories
	# on the PATH and add their plain text contents: remove # comments from
	# each line, then take the first word of each line the as label,
	# and treat subsequent words as numeric coordinates (or as an alias
	# target, if there is only one word)
	for stem, filename in GetFiles( '.' + type + '.txt' ):
		try: ParseElectrodeSpecification( filename, interpretAsFileName=True, returnType='coords', collatedCoords=coords, collatedAliases=aliases )
		except ElectrodeParsingError as error: sys.stderr.write( 'WARNING: %s\n' % error )
	ResolveAliases( coords, aliases )	
	CACHE[ type ] = coords
	return coords

def Rescan():
	for fileType in CACHE:
		CACHE[ fileType ] = GetSchemes( rescan=True ) if fileType == SCHEME_FILE_IDENTIFIER else GetCoords( fileType, rescan=True )

def GetFiles( suffix ):
	for d in PATH:
		d = os.path.realpath( os.path.expanduser( d ) )
		if not os.path.isdir( d ): continue
		for filename in sorted( glob.glob( os.path.join( d, '*' + suffix ) ) ):
			yield ( os.path.basename( filename )[ :-len( suffix ) ], filename, )

def ParseElectrodeSpecification( src, interpretAsFileName='auto', returnType='all', collatedCoords=None, collatedAliases=None, expectedDimensions=None ):

	returnTypes = [ 'all', 'scheme', 'channels', 'coords' ]
	if returnType.lower() not in returnTypes: raise ValueError( 'unrecognized returnType %r (acceptable values are %r)' % ( returnType, returnTypes ) )
	
	def ReadFile( filename ):
		with open( os.path.expanduser( filename ), 'rt' ) as fh:
			return fh.read().replace( '\r\n', '\n' ).replace( '\r', '\n' )
	
	def Lines( txt ):
		if isinstance( txt, basestring ): txt = txt.split( '\n' )
		return tuple(
			line
			for commentedLine in txt
			for line in [ commentedLine.split( '#', 1 )[ 0 ].strip() ]
			if line
		)

	filename = None
	if interpretAsFileName == 'auto':
		if isinstance( src, basestring ) and ' ' not in src and '\t' not in src and '\n' not in src: interpretAsFileName = True
		elif isinstance( src, basestring ) and '\n' not in src and ( '/' in src or '\\' in src or '.' in src ): interpretAsFileName = True
		else: interpretAsFileName = False
	if interpretAsFileName:
		filename = os.path.realpath( os.path.expanduser( src ) )
		src = ReadFile( filename )
	
	def AddContext( msg ):
		if not filename: return msg
		#return msg + ' in ' + filename
		return 'failed to parse file %r: ' % filename + msg
	
	src = Lines( src )
	scheme = OrderedDict()
	channels = OrderedDict()
	coords = OrderedDict()
	aliases = OrderedDict()
	sharedRef = None
	sharedGnd = None
	
	for line in src:	
		for ch in '\'":=,;()[]{}': line = line.replace( ch, ' ' )
		line = re.sub( r'\-?>+', ' > ', line )  # -> and > and >> are equivalent
		line = re.sub( r'<+\-?', ' < ', line )  # <- and < and << are equivalent
		line = re.sub( r'\-0\s*([^\.0-9])', r' - 0 \1', line + '#' )[ :-1 ]
		line = re.sub( r'\-([A-Za-z])', r' - \1', line )
		line = line.replace( '@', ' @ ' )
		words = line.split()
		previousWord = None
		for iWord, word in enumerate( words ):
			isZeroButMeansNull = ( previousWord in [ '-', '@' ] and word in [ '0' ] )
			if isZeroButMeansNull: word = words[ iWord ] = '0V'
			previousWord = word
		#print( words )
		mode = None
		coordsKey = None
		signalElectrode = None
		ADD_TO_PREVIOUS_REF = 1
		ADD_TO_PREVIOUS_GND = 2
		ADD_TO_CURRENT_REF  = 3
		ADD_TO_CURRENT_GND  = 4
		DEFINE_BY_ALIASING  = 5
		CREATE_INLINE_ALIAS = 6
		for wordPosition, word in enumerate( words ):
			headless = wordPosition == 0 or mode in [ ADD_TO_PREVIOUS_REF, ADD_TO_PREVIOUS_GND ]
			if   word == '-': mode = ADD_TO_PREVIOUS_REF if headless else ADD_TO_CURRENT_REF; continue
			elif word == '@': mode = ADD_TO_PREVIOUS_GND if headless else ADD_TO_CURRENT_GND; continue
			elif word == '>': mode = DEFINE_BY_ALIASING; continue
			elif word == '<': mode = CREATE_INLINE_ALIAS; continue
			try: number = float( word )
			except: pass
			else:
				if coordsKey is None: raise ElectrodeParsingError( AddContext( 'unexpected numeric value %s%s' % word ) )
				coords.setdefault( coordsKey.lower(), [] ).append( number )
				signalElectrode = None
				mode = None
				continue
			# Now we have a label
			if word.upper() in [ '0', '0V', 'ZERO', 'NUL', 'NULL' ]: word = '0V'
			else: scheme[ word ] = 1
			if mode == ADD_TO_PREVIOUS_REF:
				if coordsKey is None: coordsKey = word
				if sharedRef is None: raise ElectrodeParsingError( AddContext( 'unexpected reference -%s without previous electrode definitions' % word ) )
				sharedRef[ word ] = 1
			elif mode == ADD_TO_PREVIOUS_GND:
				if coordsKey is None: coordsKey = word
				if sharedGnd is None: raise ElectrodeParsingError( AddContext( 'unexpected ground @%s without previous electrode definitions' % word ) )
				sharedGnd[ word ] = 1
			elif mode == ADD_TO_CURRENT_REF:
				if signalElectrode is None: raise ElectrodeParsingError( AddContext( 'unexpected reference -%s without context' % word ) )
				channelRecord = channels[ signalElectrode ]
				if channelRecord[ 'ref' ] is sharedRef: channelRecord[ 'ref' ] = OrderedDict()
				channelRecord[ 'ref' ][ word ] = 1
				mode = None
			elif mode == ADD_TO_CURRENT_GND:
				if signalElectrode is None: raise ElectrodeParsingError( AddContext( 'unexpected ground @%s without context' % word ) )
				channelRecord = channels[ signalElectrode ]
				if channelRecord[ 'gnd' ] is sharedGnd: channelRecord[ 'gnd' ] = OrderedDict()
				channelRecord[ 'gnd' ][ word ] = 1
				mode = None
			elif mode == DEFINE_BY_ALIASING:
				if word[ 0 ].lower() not in 'abcdefghijklmnopqrstuvwxyz': raise ElectrodeParsingError( AddContext( 'invalid alias target %r)' % word ) )
				if coordsKey is None: raise ElectrodeParsingError( AddContext( 'unexpected alias target ->%s without context' % word ) )
				aliases[ coordsKey.lower() ] = ( filename, word.lower() )
				mode = None
			elif mode == CREATE_INLINE_ALIAS:
				if word[ 0 ].lower() not in 'abcdefghijklmnopqrstuvwxyz': raise ElectrodeParsingError( AddContext( 'invalid alias name %r)' % word ) )
				if coordsKey is None: raise ElectrodeParsingError( AddContext( 'unexpected alias %s without target' % word ) )
				newKey = word.lower()
				if newKey != coordsKey.lower(): aliases[ newKey ] = ( filename, coordsKey.lower() )
				mode = None
			else:
				if word[ 0 ].lower() not in 'abcdefghijklmnopqrstuvwxyz': raise ElectrodeParsingError( AddContext( 'invalid electrode label %r)' % word ) )
				coordsKey = word
				signalElectrode = word
				# if sharedRef/sharedGnd have been used, now it's time to empty them:
				if sharedRef is None or len( sharedRef ): sharedRef = OrderedDict()
				if sharedGnd is None or len( sharedGnd ): sharedGnd = OrderedDict()
				channels[ signalElectrode ] = { 'ref' : sharedRef, 'gnd' : sharedGnd }
				mode = None
	scheme = list( scheme )	
	channels = [ ''.join( [
		label,
		''.join( '-' + ref for ref in rec[ 'ref' ] ),
		''.join( '@' + gnd for gnd in rec[ 'gnd' ] ),
	] ) for label, rec in channels.items() ]
	nDimensions = set( len( pos ) for pos in coords.values() )
	if len( nDimensions ) > 1: raise ElectrodeParsingError( AddContext( 'unequal numbers of dimensions in coordinates' ) )
	if nDimensions:
		nDimensions = list( nDimensions )[ 0 ]
		if expectedDimensions is None: expectedDimensions = len( list( collatedCoords.values() )[ 0 ] ) if collatedCoords else nDimensions
		if nDimensions != expectedDimensions: raise ElectrodeParsingError( AddContext( 'coordinates are %d-dimensional (expected %d)' % ( nDimensions, expectedDimensions ) ) )
	for k, v in coords.items(): coords[ k ] = tuple( v )
	if collatedAliases is None: ResolveAliases( coords, aliases )
	else: collatedAliases.update( aliases )
	if collatedCoords is not None: collatedCoords.update( coords )
	out = dict( channels=channels, scheme=scheme, coords=coords )
	if returnType in out: out = out[ returnType ]
	if returnType == 'coords' and not coords: raise ElectrodeParsingError( AddContext( 'no coordinates found' ) )
	out = Bunch._convert( out )
	return out

def ResolveAliases( coords, aliases ):
	for key, ( filename, target ) in aliases.items():
		context = ' in file %r' % filename if filename else ''
		try: coords[ key.lower() ] = coords[ target.lower() ]
		except: sys.stderr.write( 'WARNING: electrode %r%s was aliased to %r, for which no coordinates were found\n' % ( key, context, target ) )	
	

def ReadChanLocs( src, scale=1.5, asDataFrame=False ):
	"""
	Parse an EEGLAB `chanlocs.mat` file or the
	`chanlocs` variable loaded from it.
	"""
	if isinstance( src, basestring ):
		import scipy.io
		d = scipy.io.loadmat( src )
	
	try: d = d[ 'chanlocs' ]
	except: pass

	columns = d.dtype.names
	d = [
		Bunch( zip( columns, [ cell.flat[ 0 ] if cell.size == 1 else '' for cell in row ] ) )
		for row in d.flat
	]
	radiansPerDegree = math.pi / 180.0
	for row in d:
		r, theta = row[ 'radius' ] * scale, row[ 'theta' ] * radiansPerDegree
		row[ 'schematicX' ] = r * math.sin( theta )
		row[ 'schematicY' ] = r * math.cos( theta )
	
	if asDataFrame:
		import pandas
		d = pandas.DataFrame( d )
		d.index = d.pop( 'labels' )

	return d


BUILTIN_SCHEMATIC2D = {

	'fp1' :   (  -0.26  ,  +0.78  ),
	'fpz' :   (   0.00  ,  +0.82  ),
	'fp2' :   (  +0.26  ,  +0.78  ),
	'fp1p':   (  -0.12  ,  +0.71  ),
	'fp2p':   (  +0.12  ,  +0.71  ),
	'f7a' :   (  -0.49  ,  +0.66  ),
	'af7' :   (  -0.49  ,  +0.66  ),
	'f5a' :   (  -0.40  ,  +0.55  ),
	'af5' :   (  -0.40  ,  +0.55  ),
	'f3a' :   (  -0.24  ,  +0.58  ),
	'af3' :   (  -0.24  ,  +0.58  ),
	'fza' :   (   0.00  ,  +0.61  ),
	'afz' :   (   0.00  ,  +0.61  ),
	'f1a' :   (  -0.10  ,  +0.52  ),
	'af1' :   (  -0.10  ,  +0.52  ),
	'f2a' :   (  +0.10  ,  +0.52  ),
	'af2' :   (  +0.10  ,  +0.52  ),
	'f4a' :   (  +0.24  ,  +0.58  ),
	'af4' :   (  +0.24  ,  +0.58  ),
	'f6a' :   (  +0.40  ,  +0.55  ),
	'f8a' :   (  +0.49  ,  +0.66  ),
	'af8' :   (  +0.49  ,  +0.66  ),
	'f9'  :   (  -0.81  ,  +0.58  ),
	'f7'  :   (  -0.67  ,  +0.48  ),
	'f5'  :   (  -0.51  ,  +0.44  ),
	'f3'  :   (  -0.34  ,  +0.41  ),
	'f1'  :   (  -0.16  ,  +0.40  ),
	'fz'  :   (   0.00  ,  +0.40  ),
	'f2'  :   (  +0.16  ,  +0.40  ),
	'f4'  :   (  +0.34  ,  +0.41  ),
	'f6'  :   (  +0.51  ,  +0.44  ),
	'f8'  :   (  +0.67  ,  +0.48  ),
	'f7p' :   (  -0.64  ,  +0.36  ),
	'f5p' :   (  -0.45  ,  +0.32  ),
	'f3p' :   (  -0.26  ,  +0.30  ),
	'f1p' :   (  -0.09  ,  +0.29  ),
	'f2p' :   (  +0.09  ,  +0.29  ),
	'f4p' :   (  +0.26  ,  +0.30  ),
	'f6p' :   (  +0.45  ,  +0.32  ),
	'f8p' :   (  +0.64  ,  +0.36  ),
	'f10' :   (  +0.81  ,  +0.58  ),
	'ft9' :   (  -0.95  ,  +0.32  ),
	'ft7' :   (  -0.78  ,  +0.26  ),
	'fc5' :   (  -0.59  ,  +0.23  ),
	'fc3' :   (  -0.39  ,  +0.22  ),
	'fc1' :   (  -0.19  ,  +0.20  ),
	'fcz' :   (   0.00  ,  +0.20  ),
	'fc2' :   (  +0.19  ,  +0.20  ),
	'fc4' :   (  +0.39  ,  +0.22  ),
	'fc6' :   (  +0.59  ,  +0.23  ),
	'ft8' :   (  +0.78  ,  +0.26  ),
	'ft10':   (  +0.95  ,  +0.32  ),
	'c7a' :   (  -0.71  ,  +0.13  ),
	'c5a' :   (  -0.51  ,  +0.12  ),
	'c3a' :   (  -0.30  ,  +0.11  ),
	'c1a' :   (  -0.10  ,  +0.10  ),
	'c2a' :   (  +0.10  ,  +0.10  ),
	'c4a' :   (  +0.30  ,  +0.11  ),
	'c6a' :   (  +0.51  ,  +0.12  ),
	'c8a' :   (  +0.71  ,  +0.13  ),
	't9'  :   (  -1.00  ,  +0.00  ),
	't7'  :   (  -0.82  ,  +0.00  ),
	'c5'  :   (  -0.62  ,  +0.00  ),
	'c3'  :   (  -0.42  ,  +0.00  ),
	'c1'  :   (  -0.21  ,  +0.00  ),
	'cz'  :   (   0.00  ,  +0.00  ),
	'c2'  :   (  +0.21  ,  +0.00  ),
	'c4'  :   (  +0.42  ,  +0.00  ),
	'c6'  :   (  +0.62  ,  +0.00  ),
	't8'  :   (  +0.82  ,  +0.00  ),
	't10' :   (  +1.00  ,  +0.00  ),
	'c7p' :   (  -0.71  ,  -0.11  ),
	'c5p' :   (  -0.50  ,  -0.10  ),
	'c3p' :   (  -0.30  ,  -0.09  ),
	'c1p' :   (  -0.10  ,  -0.09  ),
	'c2p' :   (  +0.10  ,  -0.09  ),
	'c4p' :   (  +0.30  ,  -0.09  ),
	'c6p' :   (  +0.50  ,  -0.10  ),
	'c8p' :   (  +0.71  ,  -0.11  ),
	'tp9' :   (  -0.95  ,  -0.32  ),
	'tp7' :   (  -0.78  ,  -0.26  ),
	'cp5' :   (  -0.59  ,  -0.23  ),
	'cp3' :   (  -0.39  ,  -0.22  ),
	'cp1' :   (  -0.19  ,  -0.20  ),
	'cpz' :   (   0.00  ,  -0.20  ),
	'cp2' :   (  +0.19  ,  -0.20  ),
	'cp4' :   (  +0.39  ,  -0.22  ),
	'cp6' :   (  +0.59  ,  -0.23  ),
	'tp8' :   (  +0.78  ,  -0.26  ),
	'tp10':   (  +0.95  ,  -0.32  ),
	'p7a' :   (  -0.65  ,  -0.34  ),
	'p5a' :   (  -0.45  ,  -0.32  ),
	'p3a' :   (  -0.27  ,  -0.29  ),
	'p1a' :   (  -0.09  ,  -0.29  ),
	'p2a' :   (  +0.09  ,  -0.29  ),
	'p4a' :   (  +0.27  ,  -0.29  ),
	'p6a' :   (  +0.45  ,  -0.32  ),
	'p8a' :   (  +0.65  ,  -0.34  ),
	'p9'  :   (  -0.81  ,  -0.58  ),
	'p7'  :   (  -0.67  ,  -0.48  ),
	'p5'  :   (  -0.51  ,  -0.44  ),
	'p3'  :   (  -0.34  ,  -0.41  ),
	'p1'  :   (  -0.16  ,  -0.40  ),
	'pz'  :   (   0.00  ,  -0.40  ),
	'p2'  :   (  +0.16  ,  -0.40  ),
	'p4'  :   (  +0.34  ,  -0.41  ),
	'p6'  :   (  +0.51  ,  -0.44  ),
	'p8'  :   (  +0.67  ,  -0.48  ),
	'p10' :   (  +0.81  ,  -0.58  ),
	'pzp' :   (   0.00  ,  -0.62  ),
	'o9a' :   (  -0.59  ,  -0.85  ),
	'po9' :   (  -0.59  ,  -0.81  ),
	'p9p' :   (  -0.61  ,  -0.72  ),
	'p7p' :   (  -0.49  ,  -0.66  ),
	'po7' :   (  -0.49  ,  -0.66  ),
	'p5p' :   (  -0.40  ,  -0.53  ),
	'p3p' :   (  -0.24  ,  -0.58  ),
	'po3' :   (  -0.24  ,  -0.58  ),
	'p1p' :   (  -0.10  ,  -0.52  ), 			  
	'pzp' :   (   0.00  ,  -0.61  ),
	'poz' :   (   0.00  ,  -0.61  ),
	'p2p' :   (  +0.10  ,  -0.52  ),
	'p4p' :   (  +0.24  ,  -0.58  ),
	'po4' :   (  +0.24  ,  -0.58  ),
	'p6p' :   (  +0.40  ,  -0.53  ),
	'p8p' :   (  +0.49  ,  -0.66  ),
	'po8' :   (  +0.49  ,  -0.66  ),
	'p10p':   (  +0.61  ,  -0.72  ),
	'o10a':   (  +0.59  ,  -0.85  ),
	'po10':   (  +0.59  ,  -0.81  ),
	'o1a' :   (  -0.12  ,  -0.72  ),
	'o2a' :   (  +0.12  ,  -0.72  ),
	'o9'  :   (  -0.33  ,  -0.95  ),
	'o1'  :   (  -0.26  ,  -0.78  ),
	'oz'  :   (   0.00  ,  -0.82  ),
	'o2'  :   (  +0.26  ,  -0.78  ),
	'o10' :   (  +0.33  ,  -0.95  ),
	'o1p' :   (  -0.14  ,  -0.90  ),
	'o2p' :   (  +0.14  ,  -0.90  ),
	'iz'  :   (   0.00  ,  -1.00  ),

	'el1' :   (  -0.33  ,  +1.15  ),
	'eo1' :   (  -0.55  ,  +1.05  ),
	'ei1' :   (  -0.11  ,  +1.05  ),
	'eu1' :   (  -0.33  ,  +1.01  ),
	'el2' :   (  +0.33  ,  +1.15  ),
	'eo2' :   (  +0.55  ,  +1.05  ),
	'ei2' :   (  +0.11  ,  +1.05  ),
	'eu2' :   (  +0.33  ,  +1.01  ),

	'eh1' :   (  -0.43  ,  +1.05  ),
	'ev1' :   (  -0.24  ,  +1.05  ),
	'eh2' :   (  +0.43  ,  +1.05  ),
	'ev2' :   (  +0.24  ,  +1.05  ),

	'nas' :   (   0.00  ,  +1.00  ),
	
	'a1'  :   (  -1.20  ,  +0.00  ),
	'a2'  :   (  +1.20  ,  +0.00  ),

	'laud' :  (  +1.30  ,  +0.90  ),
	'raud' :  (  +1.30  ,  +0.75  ),
	'vmrk' :  (  +1.30  ,  +0.60  ),
}

s = BUILTIN_SCHEMATIC2D
# aliases for the 1020 system (obsolete "classic" names)
s['t3'] = s['t7']
s['t4'] = s['t8']
s['t5'] = s['p7']
s['t6'] = s['p8']
s['m1'] = s['tp9']
s['m2'] = s['tp10']
# misc. other aliases
s['nz'] = s['nas']
s['audl'] = s['laud']
s['audr'] = s['raud']

# Aliases for the biosemi64 system:
s['a01'] = s['fp1']
s['a02'] = s['f7a']
s['a03'] = s['f3a']
s['a04'] = s['f1']
s['a05'] = s['f3']
s['a06'] = s['f5']
s['a07'] = s['f7']
s['a08'] = s['ft7']
s['a09'] = s['fc5']
s['a10'] = s['fc3']
s['a11'] = s['fc1']
s['a12'] = s['c1']
s['a13'] = s['c3']
s['a14'] = s['c5']
s['a15'] = s['t7']
s['a16'] = s['tp7']
s['a17'] = s['cp5']
s['a18'] = s['cp3']
s['a19'] = s['cp1']
s['a20'] = s['p1']
s['a21'] = s['p3']
s['a22'] = s['p5']
s['a23'] = s['p7']
s['a24'] = s['p9']
s['a25'] = s['p7p']
s['a26'] = s['p3p']
s['a27'] = s['o1']
s['a28'] = s['iz']
s['a29'] = s['oz']
s['a30'] = s['pzp']
s['a31'] = s['pz']
s['a32'] = s['cpz']
s['b01'] = s['fpz']
s['b02'] = s['fp2']
s['b03'] = s['f8a']
s['b04'] = s['f4a']
s['b05'] = s['fza']
s['b06'] = s['fz']
s['b07'] = s['f2']
s['b08'] = s['f4']
s['b09'] = s['f6']
s['b10'] = s['f8']
s['b11'] = s['ft8']
s['b12'] = s['fc6']
s['b13'] = s['fc4']
s['b14'] = s['fc2']
s['b15'] = s['fcz']
s['b16'] = s['cz']
s['b17'] = s['c2']
s['b18'] = s['c4']
s['b19'] = s['c6']
s['b20'] = s['t8']
s['b21'] = s['tp8']
s['b22'] = s['cp6']
s['b23'] = s['cp4']
s['b24'] = s['cp2']
s['b25'] = s['p2']
s['b26'] = s['p4']
s['b27'] = s['p6']
s['b28'] = s['p8']
s['b29'] = s['p10']
s['b30'] = s['p8p']
s['b31'] = s['p4p']
s['b32'] = s['o2']
