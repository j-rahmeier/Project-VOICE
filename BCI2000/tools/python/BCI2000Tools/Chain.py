# -*- coding: utf-8 -*-

# ///////////////////////////////////////////////////////////////////////////
# $Id: Chain.py 7583 2023-09-08 18:46:31Z jhill $
# Author: jeremy.hill@neurotechcenter.org
# Description: wrapper around BCI2000 command-line tool processing
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
	'bci2000chain',
	'bci2000root', 'bci2000path',
	'CallbackObject', 'TrapFilterCallbackObject',
	'BCI2000ChainError', 'TerminateChain',
]

import os
import sys
import ast
import time
import struct
import select
import weakref
import tempfile
import threading
import subprocess

import numpy

from . import Bootstrapping;  from .Bootstrapping  import bci2000root, bci2000path
from . import Parameters;     from .Parameters     import make_bciprm, read_bcidate
from . import Container;      from .Container      import Bunch as CONTAINER
from . import LoadStream2Mat; from .LoadStream2Mat import ReadMatFile
from . import Numerics;       from .Numerics       import pdiff

try: from . import Electrodes; from .Electrodes import ChannelSet # TODO: Electrodes, ElectrodeGrids, ElectrodePositions and Plotting don't really belong in this package
except ImportError: ChannelSet = None  # in this case, the optional .ChannelSet attribute will simply not be added to the output, at the end of bci2000chain()

if sys.version < '3': bytes = str
else: unicode = str; basestring = ( unicode, bytes )

DAT2STREAM_HAS_P_FLAG      = True  # new-style bci_dat2stream has a -p flag allowing it to read a .prm file to override parameter values that came from the .dat file. Without this, some source parameters like SampleBlockSize and SourceChGain cannot be overridden.
STREAM2MAT_SAVES_PARMS     = True  # new-style bci_stream2mat saves a string representation of the collated output parameters in the mat file, so we don't have to rely on a separate parameter file.
USE_INPUT_AND_OUTPUT_FLAGS = True  # using ' "--input=$DATFILE"' yields an order-of-magnitude speedup relative to ' < "$DATFILE"'. With `callback=None` (so that `bci_stream2mat` gets used) I see no great corresponding speedup for ' "--output=$MATFILE"' relative to ' > "$MATFILE"'

class BCI2000ChainError( OSError ):
	pass

class TerminateChain( Exception ):
	def __init__( self, keepBlock=False ):
		self.keepBlock = keepBlock

def bci2000chain(datfile, chain='SpectralSignalProcessing', parms=(), dims='auto', start=None, duration=None, verbose=False, keep=False, binpath=None, callback='default', **kwargs):
	"""
	This function wraps the processing of a BCI2000 `.dat` file through
	`bci_dat2stream.exe` followed by a specified series of command-line
	filter tools, as described at
	https://www.bci2000.org/mediawiki/index.php/User_Reference:Command_Line_Processing
	
	See also the equivalent Matlab wrapper, described at
	https://www.bci2000.org/mediawiki/index.php/User_Reference:Matlab_Tools
	and in the Matlab documentation of `bci2000chain.m`.
	
	Args:
		datfile (str):
			a path to a BCI2000 `.dat` file.
		
		chain (str):
			a list of BCI2000 filter tool names, or a pipe-delimited string
			composed of filter tool names.  The default value,
			`'SpectralSignalProcessing'`, is a shortcut for::
		
			    chain = [ 'TransmissionFilter', 'SpatialFilter', 
			              'SpectralEstimator', 'LinearClassifier', 'LPFilter'
			              'ExpressionFilter', 'Normalizer' ]
		
		parms (str, tuple, list):
			By default the parameter values used in the processing chain are
			the ones saved in the `.dat` file (where relevant) or the 
			filter defaults (where the `.dat` file does not specify the
			parameter).  You can override parameter values by using this
			argument, which can take the form of:
			
			- A name of, or path to, a BCI2000 `.prm` or `.dat` file.
			- A sequence of such filenames/paths.
			- A `dict` of   `{ paramName1: paramValue1, paramName2: paramValue2, ... }`
			- A sequence of `[ paramName1, paramValue1, paramName2, paramValue2, ... ]`.
			- Sequence of name + value pairs and/or filenames and/or dicts
			  (different formats may be interspersed).
			
			Sequences are read from left to right, with later parameter
			settings overwriting earlier ones. Any additional `**kwargs`
			are also interpreted as parameter specifications, and are
			applied last.
	
		dims (int, str):
			This should be 2, 3 or 'auto':
			
			- `dims=2` means the signal should be two-dimensional:
			  elements (concatenated across sample-blocks) by channels.
			- `dims=3` means the signal should be three-dimensional:
			  sample-blocks by channels by elements
			- `dims='auto'` means the choice of 2- or 3-dimensional
			  output should be made automatically.
		
		start (int, float, str):
			Passed through to `bci_dat2stream` as the `--start=` value:
			hence, specifies the number of blocks to skip at the beginning
			(or number of seconds or milliseconds, if expressed as a string
			with `'s'` or `'ms'` appended).
		
		duration (int, float, str):
			Passed through to `bci_dat2stream` as the `--duration=` value:
			hence, specifies the number of blocks to process (or number of
			seconds or milliseconds, if expressed as a string with `'s'`
			or `'ms'` appended).
			
		verbose (bool):
			If `True`, output diagnostic/debugging messages to the console.
		
		keep (bool):
			Whether or not to keep the temporary files created by the chain.
			By default, the files are deleted if there were no errors.
		
		binpath (str):
			Optionally specifies the path (or partial path relative to a
			previously-established `bci2000root()`) to the directory in
			which the filter-tool executables reside.
			
		callback (None, 'default', or a callable):
			Optionally specifies a function that will be called on every
			sampleblock. The callback should take one argument: a dict
			(let's call it `d`) and should return a boolean value that
			dictates whether the sampleblock should be kept. This decision
			might be made based on `d['NumberOfBlocksProcessed']` and the
			contents of `d['States']` and `d['Changed']`.  The callback
			will also be called one additional final time when processing
			is completed. On this call, `d['Changed']` will be empty (on
			all previous calls, it will contain `'States.SourceTime'` at
			the very least, and usually also `'Signal'`).
			
			If this is `None`, `bci_stream2mat` is used at the end of the
			chain. If not, `bci_stream2hybrid` is used.
			
			
	Examples:
	
		Excuse the relative paths - this example works if you're currently
		working in the root directory of the BCI2000 distro::
	
		    bci2000chain( datfile='data/samplefiles/eeg3_2.dat',
		                  chain='TransmissionFilter|SpatialFilter|ARFilter',
		                  binpath='tools/cmdline',
		                  parms=[ 'tools/matlab/ExampleParameters.prm' ],
		                  SpatialFilterType=3 )
	
		Or for more portable operation, you can use `bci2000root()` and
		`bci2000path()`::
	
		    bci2000root( '/PATH/TO/BCI2000' )
		    
		    bci2000chain( datfile=bci2000path( 'data/samplefiles/eeg3_2.dat' ),
		                  chain='TransmissionFilter|SpatialFilter|ARFilter',
		                  parms=[ bci2000path( 'tools/matlab/ExampleParameters.prm' ) ],
		                  SpatialFilterType=3 )
	
	"""
			
	if verbose: verbosityFlag = '-v'
	else: verbosityFlag = '-q'
	
	if isinstance(chain, basestring):
		if chain.lower() == 'SpectralSignalProcessing'.lower():
			chain = 'TransmissionFilter|SpatialFilter|SpectralEstimator|LinearClassifier|LPFilter|ExpressionFilter|Normalizer'
		elif chain.lower() == 'ARSignalProcessing'.lower():
			chain = 'TransmissionFilter|SpatialFilter|ARFilter|LinearClassifier|LPFilter|ExpressionFilter|Normalizer'
		elif chain.lower() == 'P3SignalProcessing'.lower():
			chain = 'TransmissionFilter|SpatialFilter|P3TemporalFilter|LinearClassifier'
		chain = chain.split('|')
	chain = [c.strip() for c in chain if len(c.strip())]
	
	if len(chain) == 0: print('WARNING: chain is empty')
	
	if start is not None and len(str(start).strip()): start = ' --start=' + str(start).replace(' ', '')
	else: start = ''
	if duration is not None and len(str(duration).strip()): duration = ' --duration=' + str(duration).replace(' ', '')
	else: duration = ''
	
	if dims is None or str(dims).lower() == 'auto': dims = 0
	if dims not in (0, 2, 3): raise ValueError("dims must be 2, 3 or 'auto'")
	
	out = CONTAINER()
	out._summarize = 60
	err = ''
	
	cmd = ''
	binaries = []
	tmpdir = tempfile.mkdtemp(prefix='bci2000chain_')
	
	tmpdatfile  = os.path.join(tmpdir, 'in.dat')
	prmfile_in  = os.path.join(tmpdir, 'in.prm')
	prmfile_out = os.path.join(tmpdir, 'out.prm')
	matfile     = os.path.join(tmpdir, 'out.mat')
	bcifile     = os.path.join(tmpdir, 'out.bci')
	shfile      = os.path.join(tmpdir, 'go.bat')
	logfile     = os.path.join(tmpdir, 'log.txt')
	
	if not isinstance(datfile, basestring):
		raise ValueError('datfile must be a filename') # TODO: if datfile contains the appropriate info, use some create_bcidat equivalent and do datfile = tmpdatfile
	datfile = os.path.realpath( os.path.expanduser( datfile ) )
	if not os.path.isfile(datfile): raise IOError('file not found: %s' % datfile)
	
	mappings = {
		'$DATFILE':     datfile,
		'$PRMFILE_IN':  prmfile_in,
		'$PRMFILE_OUT': prmfile_out,
		'$MATFILE':     matfile,
		'$BCIFILE':     bcifile,
		'$SHFILE':      shfile,
		'$LOGFILE':     logfile,
	}
	
	if binpath is None and bci2000root() is not None: binpath = 'tools/cmdline'
	if binpath is None: raise ValueError( 'must either pre-configure `bci2000root()` or supply `binpath` explicitly' )
	binpath = bci2000path( binpath )
	def exe(name):
		if binpath: return '"' + os.path.join(binpath, name) + '"'
		else: return name
	
	if parms is None: parms = []
	if isinstance(parms, tuple): parms = list(parms)
	if not isinstance(parms, list): parms = [parms]
	else: parms = list(parms)
	if len(kwargs): parms.append(kwargs)
	
	def StartChain(  target ): return ( ' "--input=%s"'  if USE_INPUT_AND_OUTPUT_FLAGS else ' < "%s"' ) % target
	def FinishChain( target ): return ( ' "--output=%s"' if USE_INPUT_AND_OUTPUT_FLAGS else ' > "%s"' ) % target
	
	if parms is None or len(parms) == 0:
		cmd += exe('bci_dat2stream') + start + duration + StartChain( '$DATFILE' )
		binaries.append('bci_dat2stream')
	else:
		if verbose: print( '# writing custom parameter file %s' % prmfile_in )
		try: parms = make_bciprm(verbosityFlag, datfile, parms, '>', prmfile_in)
		except:
			try: make_bciprm( verbosityFlag, datfile )
			except: raise BCI2000ChainError( 'failed to decode BCI2000 parameters from ' + datfile ) 
			raise
		
		if DAT2STREAM_HAS_P_FLAG:
			cmd += exe('bci_dat2stream') + ' "-p$PRMFILE_IN"' + start + duration + StartChain( '$DATFILE' ) # new-style bci_dat2stream with -p option
			binaries.append('bci_dat2stream')
		else:
			if len(start) or len(duration):
				raise ValueError('old versions of bci_dat2stream have no --start or --duration option')
			cmd += '('   # old-style bci_dat2stream with no -p option
			cmd +=         exe('bci_prm2stream') + ' < "$PRMFILE_IN"'
			cmd += '&& ' + exe('bci_dat2stream') + ' --transmit-sd' + StartChain( '$DATFILE' )
			cmd += ')'
			binaries.append('bci_dat2stream')
			binaries.append('bci_prm2stream')

	for c in chain:
		cmd += ' | ' + exe(c)
		binaries.append(c)
	
	if callback == 'default': callback = CallbackObject()
	if callback:
		exe_name = 'bci_stream2hybrid'
		cmd += ' | ' + exe(exe_name)
		binaries.append(exe_name)
	elif STREAM2MAT_SAVES_PARMS:
		cmd += ' | ' + exe('bci_stream2mat') + FinishChain( '$MATFILE' ) # new-style bci_stream2mat with Parms output
		binaries.append('bci_stream2mat')
	else:
		cmd += FinishChain( '$BCIFILE' )
		cmd += ' && ' + exe('bci_stream2mat') + StartChain( '$BCIFILE' ) + FinishChain( '$MATFILE' )
		cmd += ' && ' + exe('bci_stream2prm') + StartChain( '$BCIFILE' ) + FinishChain( '$PRMFILE_OUT' ) # old-style bci_stream2mat without Parms output
		binaries.append('bci_stream2mat')
		binaries.append('bci_stream2prm')
	
	for k,v in mappings.items(): cmd = cmd.replace(k, v)
	
	win = sys.platform.lower().startswith('win')
	if win: preamble, postscript = '@',             ' %*'
	else:   preamble, postscript = '#!/bin/bash\n\n', ' "$@"'
	open(shfile, 'wt').write( preamble + cmd + postscript + '\n' )
	if not win: os.chmod(shfile, 484) # rwxr--r--
	
	failsig = 'Configuration Error: '
	failsig = ' Error: '  # TODO: this used to be 'Configuration Error:' but that misses some errors.
	                      #       Hopefully the more general signature doesn't give us false positives...
	                      #       There has to be a better way of detecting failure...
	def tidytext(x):
		return x.strip().replace('\r\n', '\n').replace('\r', '\n')
	def getoutput(cmd):
		return getstatusoutput(cmd)[1]
	def getstatusoutput(cmd): # simpler case, older implementation, used for chains that have no callback and use bci_stream2mat
		pipe = os.popen(cmd + ' 2>&1', 'r')
		output = pipe.read()
		status = pipe.close()
		if status is None: status = 0
		return status,tidytext(output)
	def process(cmd, callback, logfile): # block-by-block processing with a callback
		def bytes2str(s): return s if str is bytes else s.decode( 'utf-8' )
		def str2bytes(s): return s if str is bytes else s.encode( 'utf-8' )
		emptyBytes = str2bytes( '' )
		d = CONTAINER( Changed=[], Parameters=[], CollatedSignal=[], CollatedStates=CONTAINER(), CollatedEvents=CONTAINER(), RetainedBlockNumbers=[] )
		#pipe = os.popen( cmd + ' 2>"%s"' % logfile, 'r' ) # the problem with this is that it opens the output in text mode. unsure how to switch it to binary, so instead we'll use subprocess:
		sp = subprocess.Popen( [ cmd ], shell=False, stdout=subprocess.PIPE, stderr=subprocess.PIPE ); pipe = sp.stdout # text=False option does not exist in Python 2 but it seems to be fine in both if omitted
		shape = None
		class StandardErrorWatcher( object ):
			def __init__( self, sp ):
				self.sp = weakref.ref( sp )
				self.stderr = sp.stderr
				self.configFailed = False
				self.configPassed = False
				self.text = ''
				self.keepgoing = True
				self.thread = threading.Thread( target=self )
				self.thread.setDaemon( True ) # last resort - even if the thread hangs around forever zombified, at least don't prevent Python from terminating 
				self.thread.start()
			def __call__( self ):
				while self.keepgoing:
					try: ready, _, _ = select.select( [ self.stderr ], [], [], 0.010 )
					except: ready = [ self.stderr ] # fails on Python 2.7 under Windows with error message "An operation was attempted on something that is not a socket" - fall back on .setDaemon( True ) above
					if not ready: continue # timed out: so, take a moment to check whether self.keepgoing has been unset, and if it hasn't then try again
					line = self.stderr.read() if self.configFailed else self.stderr.readline()
					if not line: break
					if isinstance( line, bytes ): line = bytes2str( line )
					#sys.stderr.write( 'STDERR: ' + line ); sys.stderr.flush()
					if 'bci_stream2hybrid: CONFIGURATION ACCEPTED' in line: self.configPassed = True; continue
					self.text += line
					if failsig in line:
						self.configFailed = True
						try: self.sp().kill()
						except: pass
						break # without this, the thread will never end even after the chain has returned; unfortunately it means only the first error message is preserved
		stderr = StandardErrorWatcher( sp )
		try:
			while not stderr.configPassed and not stderr.configFailed:
				if sp.poll() is not None: stderr.configFailed = True
				time.sleep( 0.001 )
			terminator = None
			while not stderr.configFailed and sp.poll() is None:
				line = pipe.readline()
				if not line: break
				#sys.stderr.write( 'STDOUT: ' + bytes2str( line ) ); sys.stderr.flush()
				line = line.strip()
				if not line: continue
				line = bytes2str( line )
				if line == 'Parameter':
					d[ 'Parameters' ].append( bytes2str( pipe.readline().strip() ) )
					continue
				words = line.split()
				valueType = words.pop( -1 )
				if valueType == ':': fmt = words.pop( -1 )
				container = d
				for key in words[ :-1 ]: container = container[ key ]
				lastKey = words[ -1 ]
				fullKey = '.'.join( words )
				if valueType == ':':
					raw = pipe.read( struct.calcsize( fmt ) )
					#value = struct.unpack( fmt, raw )  # non-numpy-dependent version, decodes values as tuple
					#value = numpy.fromfile( pipe, dtype=fmt[ -1 ], count=int( fmt[ 1:-1 ] ) ) # this looked like it might be a good way of going straight to numpy array from the filehandle - but it results in OSError: seeking file failed
					value = numpy.frombuffer( raw, dtype=fmt[ -1 ] )
				elif valueType == '=':
					arg = bytes2str( pipe.readline().strip() )
					value = ast.literal_eval( arg )
					if isinstance( value, dict ) and isinstance( CONTAINER(), dict ): value = CONTAINER( value )
				else:
					raise RuntimeError( 'unrecognized valueType %r' % valueType )			
				if fullKey == 'Signal':
					if shape is None: shape = ( d[ 'NumberOfElements' ], d[ 'NumberOfChannels' ] )
					value = value.reshape( shape )
				container[ lastKey ] = value
				if fullKey == 'NumberOfBlocksProcessed' and value:
					try:
						keepBlock = callback( d )
					except TerminateChain as exc:
						keepBlock = exc.keepBlock
						terminator = exc
					if keepBlock:
						d[ 'RetainedBlockNumbers' ].append( value - 1 )
						d[ 'CollatedSignal' ].append( d[ 'Signal' ] ) # NB: inefficient for very large files, unless you write your callback such that it clears this field
						for name, value in d[ 'States' ].items(): d[ 'CollatedStates' ].setdefault( name, [] ).append( value )
						for name, value in d[ 'Events' ].items(): d[ 'CollatedEvents' ].setdefault( name, [] ).append( value.astype( 'int64' ) )
					d[ 'Changed' ][ : ]	= []
					if terminator: raise terminator
				else:
					d[ 'Changed' ].append( fullKey )
			#status = pipe.close()
			status = sp.wait()
		except KeyboardInterrupt:
			pass
		except TerminateChain:
			pass
		finally:
			try: sp.kill()
			except: pass
			status = sp.returncode
		if status == 0 or terminator: # only call the callback again if everything's OK
			try: callback( d ) # d['Changed'] will be empty, to signal to the callback that it should wrap things up, if necessary
			except TerminateChain: pass
		stderr.keepgoing = False # some implementations don't need this; some do
		return d, status, tidytext( stderr.text )
		
	if verbose: print( '# querying version information'	)
	binaries = CONTAINER([(bin, getoutput(exe(bin) + ' --version').replace('\n', '  ') ) for bin in binaries])

	if verbose: print( cmd )
	t0 = time.time()	
	if callback: callbackResult, failed, output = process(shfile, callback, logfile)
	else: failed, output = getstatusoutput(shfile)
	chaintime = time.time() - t0
	
	if failsig in output: failed = 1
	printable_output = output
	printable_lines = output.split('\n')
	maxlines = 10
	if len(printable_lines) > maxlines:
		printable_output = '\n'.join(printable_lines[:maxlines] + ['[%d more lines omitted]' % (len(printable_lines) - maxlines)])
	def AddContextToError( message ):
		if verbose: return 'system call failed:\n' + message # cmd has already been printed, so don't clutter things further
		else: return 'system call failed:\n\n%s\n\n%s' % ( cmd, message )
	if failed: err = AddContextToError( printable_output )
	
	if not err and not callback:
		if verbose: print( '# loading %s' % matfile )
		try:
			mat = ReadMatFile(matfile, containerClass=CONTAINER)
		except:
			err = "The chain must have failed: could not load %s\nShell output was as follows:\n%s" % (matfile, printable_output)
		else:
			if 'Data' not in mat:  err = "The chain must have failed: no 'Data' variable found in %s\nShell output was as follows:\n%s" % (matfile, printable_output)
			if 'Index' not in mat: err = "The chain must have failed: no 'Index' variable found in %s\nShell output was as follows:\n%s" % (matfile, printable_output)

	if not err:
		out.FileName = datfile
		if callback:
			if verbose: print( '# decoding parameters loaded from callback result' )
			parms = make_bciprm( verbosityFlag, callbackResult.get( 'Parameters', [] ) )
			if not parms: err = AddContextToError( output )
		elif STREAM2MAT_SAVES_PARMS:
			if verbose: print( '# decoding parameters loaded from the mat-file' )
			parms = make_bciprm( verbosityFlag, mat[ 'Parms' ][ 0 ] )
		else:
			if verbose: print( '# reading output parameter file' + prmfile_out )
			parms = ParmList(prmfile_out) # if you get an error that prmfile_out does not exist, recompile your bci_dat2stream and bci_stream2mat binaries from up-to-date sources, and ensure that DAT2STREAM_HAS_P_FLAG and STREAM2MAT_SAVES_PARMS, at the top of this file, are both set to 1
			
	if not err:
		out.DateStr = read_bcidate(parms, 'ISO')
		out.DateNum = read_bcidate(parms)
		out.FilterChain = chain
		out.ToolVersions = binaries
		out.ShellInput = cmd
		out.ShellOutput = output
		out.ChainTime = chaintime
		out.ChainSpeedFactor = None
		out.Megabytes = None
		out.Parms = parms
		out.Parms.sort( key=lambda p: p.Name )
		
		if callback:
			nChannels = callbackResult.get( 'NumberOfChannels', None )
			nElements = callbackResult.get( 'NumberOfElements', None )
			nBlocksProcessed = callbackResult.get( 'NumberOfBlocksProcessed', None )
			nBlocksRetained = len( callbackResult.get( 'CollatedSignal', [] ) )
			mat = callbackResult
		else:
			mat['Index'] = mat['Index'][0,0]
			sigind = mat['Index'].Signal - 1 # indices vary across channels fastest, then elements
			nChannels,nElements = sigind.shape
			nBlocksRetained = nBlocksProcessed = mat['Data'].shape[1]
		
		out.BlocksProcessed = nBlocksProcessed
		out.BlocksRetained = nBlocksRetained
		out.BlocksPerSecond = float(parms.SamplingRate.ScaledValue) / float(parms.SampleBlockSize.ScaledValue)
		out.SecondsPerBlock = float(parms.SampleBlockSize.ScaledValue)  / float(parms.SamplingRate.ScaledValue)
		out.ChainSpeedFactor = float(out.BlocksProcessed * out.SecondsPerBlock) / float(out.ChainTime)
		
		def unnumpify(x):
			while isinstance(x, numpy.ndarray) and x.size == 1: x = x[0]
			if isinstance(x, (numpy.ndarray,tuple,list)): x = [unnumpify(xi) for xi in x]
			return x
		
		out.Channels = nChannels
		out.ChannelLabels = unnumpify(mat.get('ChannelLabels', []))
		if ChannelSet: out.ChannelSet = ChannelSet(out.ChannelLabels)
		else: out.ChannelSet = None
		
		out.Elements = nElements
		out.ElementLabels = unnumpify(mat.get('ElementLabels', []))
		out.ElementValues = numpy.ravel(mat.get('ElementValues', []))
		out.ElementUnit = unnumpify(mat.get('ElementUnit', None))
		out.ElementRate = out.BlocksPerSecond * out.Elements
		
		out.Time = out.SecondsPerBlock * ( numpy.array( callbackResult[ 'RetainedBlockNumbers' ] ) if callback else numpy.arange(0, nBlocksProcessed) )
		out.FullTime = out.Time
		out.FullElementValues = out.ElementValues

		def seconds(s):  # -1 means "no information", 0 means "not units of time",  >0 means the scaling factor
			if getattr(s, 'ElementUnit', None) in ('',None,()) or s.ElementUnit == []: return -1
			s = s.ElementUnit
			if   s.endswith('seconds'): s = s[:-6]
			elif s.endswith('second'):  s = s[:-5]
			elif s.endswith('sec'):     s = s[:-2]		
			if s.endswith('s'): return { 'ps': 1e-12, 'ns': 1e-9, 'us': 1e-6, 'mus': 1e-6, 'ms': 1e-3, 's' : 1e+0, 'ks': 1e+3, 'Ms': 1e+6, 'Gs': 1e+9, 'Ts': 1e+12,}.get(s, 0)
			else: return 0
	
		if dims == 0: # dimensionality has not been specified explicitly: so guess, based on ElementUnit and/or filter name
			# 3-dimensional output makes more sense than continuous 2-D whenever "elements" can't just be concatenated into an unbroken time-stream
			if len(chain): lastfilter = chain[-1].lower()
			else: lastfilter = ''
			if nBlocksRetained != nBlocksProcessed:
				dims = 3
			elif lastfilter in [ 'p3temporalfilter', 'trapfilter' ]:
				dims = 3
			else:
				factor = seconds(out)
				if factor > 0:  # units of time.  TODO: could detect whether the out.ElementValues*factor are (close enough to) contiguous from block to block; then p3temporalfilter and trapfilter wouldn't have to be special cases above 
					dims = 2 
				elif factor == 0: # not units of time: use 3D by default
					dims = 3
				elif lastfilter in ['trapfilter', 'p3temporalfilter', 'arfilter', 'fftfilter', 'coherencefilter', 'coherencefftfilter']:  # no ElementUnit info? guess based on filter name
					dims = 3
				else:
					dims = 2
					
		if dims == 2:
			out.FullTime = numpy.repeat(out.Time, nElements) # but this means time is standing still during a block...
			factor = seconds(out)
			if len(out.ElementValues):
				out.FullElementValues = numpy.tile(out.ElementValues, nBlocksRetained)
				if factor > 0: out.FullTime = out.FullTime + out.FullElementValues * factor # ...so here we fix that if we can
					
		if callback:
			collatedSignal = callbackResult.get( 'CollatedSignal', [] )
			collatedStates = CONTAINER( { name : numpy.array( values )               for name, values in callbackResult.get( 'CollatedStates', {} ).items() } )
			collatedEvents = CONTAINER( { name : numpy.concatenate( values, axis=0 ) for name, values in callbackResult.get( 'CollatedEvents', {} ).items() } )
			if nBlocksRetained == 0:
				out.Signal = numpy.zeros( ( 0, nChannels, nElements ) if dims == 3 else ( 0, nChannels ), dtype='f' )
				out.States = CONTAINER( { name : numpy.array( [], dtype='L' ) for name in callbackResult.get( 'States', {} ) } )
				out.Events = CONTAINER( { name : numpy.array( [], dtype='L' ) for name in callbackResult.get( 'Events', {} ) } )
			elif dims == 3:
				out.Signal = numpy.array( collatedSignal )          # nBlocksRetained - by - nElements - by - nChannels
				out.Signal = numpy.transpose(out.Signal, (0,2,1))   # nBlocksRetained - by - nChannels - by - nElements
				out.Signal = out.Signal + 0.0 # make a contiguous copy
				out.States = collatedStates
				out.Events = collatedEvents
			elif dims == 2:
				out.Signal = numpy.concatenate( collatedSignal, axis=0 )  # nBlocksRetained*nElements - by - nChannels
				# out.States = CONTAINER( { name : value.repeat( nElements ) for name, value in collatedStates.items() } )
				# The line above is what we *would* do if the bci_dat2stream protocol was able to distinguish between event- and non-event states.
				# Unfortunately neither the .dat file nor the protocol is able to make this distinction, for backward compatibility reasons.
				# So bci_stream2hybrid transfers *both* scalar and vector-covering-entire-sampleblock representations of *all* states
				# (see the comments marked @@@ in bci_stream2hybrid.cpp).  Therefore, the right thing to do is simply:
				out.States = collatedEvents # <- because the "event" representation has lost less information (or no information, if the sampleblock size hasn't changed during the chain)
				out.Events = collatedEvents # <- this we would do in any case
			else:
				raise RuntimeError('internal error')
			# TODO: events might need their own (possibly original-sampleclock-based) timebase, and it will have discontinuities if some blocks were not retained

		else:
			# Why does sigind have to be transposed before vectorizing to achieve the same result as sigind(:) WITHOUT a transpose in Matlab?
			# This will probably forever be one of the many deep mysteries of numpy dimensionality handling
			out.Signal = mat['Data'][sigind.T.ravel(), :]  # nChannels*nElements - by - nBlocks
			out.Signal = out.Signal + 0.0 # make a contiguous copy			
			
			if dims == 3:
				out.Signal = numpy.reshape(out.Signal, (nChannels, nElements, nBlocksRetained), order='F') # nChannels       - by - nElements - by - nBlocksRetained
				out.Signal = numpy.transpose(out.Signal, (2,0,1))                                          # nBlocksRetained - by - nChannels - by - nElements
				out.Signal = out.Signal + 0.0 # make a contiguous copy
			elif dims == 2:
				out.Signal = numpy.reshape(out.Signal, (nChannels, nElements*nBlocksRetained), order='F')  # nChannels - by - nSamples
				out.Signal = numpy.transpose(out.Signal, (1,0))                                            # nSamples  - by - nChannels
				out.Signal = out.Signal + 0.0 # make a contiguous copy
			else:
				raise RuntimeError('internal error')
				
			out.States = CONTAINER()
			try: fieldnames = mat['Index']._fieldnames
			except AttributeError: items = mat['Index'].items()
			else: items = [ ( k, getattr(mat['Index'], k) ) for k in fieldnames ]
			states = [(k,int(v)-1) for k, v in items if k != 'Signal']
			for k,v in states: setattr(out.States, k, mat['Data'][v,:])
			# bci_stream2mat.cpp does not handle Event states
				
		try: out.Megabytes = megs(out)
		except: out.Megabytes = sys.exc_info()
		def NotifyCleanup( message ): print( message )
	else:
		#print( err ); print( '' )
		out = [ err + '\n\n' ]
		keep = True
		vars = locals()
		def NotifyCleanup( message ): out[ 0 ] += message + '\n'
		
	if os.path.isdir(tmpdir):
		files = sorted([os.path.join(tmpdir, file) for file in os.listdir(tmpdir) if file not in ['.','..']])
		if keep: NotifyCleanup( 'The following commands should be executed to clean up temporary files:' )
		elif verbose: print( '# removing temp files and directory ' + tmpdir )
		
		for file in files:
			if keep: NotifyCleanup( "os.remove(r'%s')" % file )
			else:
				try: os.remove(file)
				except Exception as err: sys.stderr.write( "failed to remove %s:\n    %s\n" % ( file, str( err ) ) )
			
		if keep: NotifyCleanup( "os.rmdir(r'%s')" % tmpdir )
		else:
			try: os.rmdir(tmpdir)
			except Exception as err: sys.stderr.write( "failed to remove %s:\n    %s" % ( tmpdir, str( err ) ) )
		if keep: NotifyCleanup( "" )
	
	if isinstance( out, list ): raise BCI2000ChainError( out[ 0 ].strip() )
	return out

def megs(x, rec=0):
	"""
	Very rough calculation of the number of megabytes occupied by an object and its children.
	Note that some of the children (and hence the memory) may be shared by other objects.
	Note also that mutual references by objects to each other will screw things up.
	"""
	rec += 1
	if rec > 100: raise ValueError( 'megs failed (too much recursion)' )
	denom = 1024.0 * 1024.0
	def siz(x, rec):
		if hasattr(sys, 'getsizeof'): m = sys.getsizeof(x) / denom # only the container overhead
		else: # Python versions <2.6 do not have sys.getsizeof. Container overhead will be underestimated.
			try: m = numpy.dtype( x.__class__ ).itemsize
			except: m = 0
		if hasattr(x, '__dict__'): m += megs(x.__dict__, rec)  # works for sstruct and Param objects very nicely; should work for most old- and new-type objects
		return m
	if isinstance(x, dict):          return siz(x, rec) + sum( megs( key, rec ) + megs( value, rec ) for key, value in x.items() )
	if isinstance(x, numpy.ndarray):
		m = siz( x, rec )
		if m < x.nbytes / denom: m += x.nbytes / denom # depending on Python and/or numpy version, x.nbytes (size of raw packed content) may or may not be included in the unadjusted siz() output
		return m
	if isinstance(x, basestring ):   return siz(x, rec)
	try: # try treating it as a sequence of any kind
		for xi in x: break
	except: pass
	else: return siz( x, rec ) + sum( [ megs( xi, rec ) for xi in x ] )
	return siz(x, rec) # other esoteric types may not be covered - numpy.ndarray, for example, has no __dict__ and requires special handling, and perhaps there are others like it out there

class sys_dot_stderr: pass

class CallbackObject( object ):
	def __init__( self, stream=sys_dot_stderr, announcementPeriodInSeconds=2, debug=False, **counters ):
		if stream is sys_dot_stderr: stream = sys.stderr # defer the actual dereferencing until now, in case someone has been messing with it
		self.counters = { 'StimulusCode' : 'StimulusCode events', 'TrialsCompleted' : 'trials' }
		self.counters.update( counters )
		self.counters = { k : v for k, v in self.counters.items() if v is not None }
		self.counts = {}
		self.lastValues = {}
		self.stream = stream
		self.announcementPeriodInSeconds = announcementPeriodInSeconds
		self.debug = debug
	def set( self, **kwargs ):
		for k, v in kwargs.items(): setattr( self, k, v )
		return self
	def write( self, x ):
		self.stream.write( x )
		try: self.stream.flush()
		except: pass
	def __call__( self, d ):
		if self.debug: sys._debug_bci2000chain = d
		changed = d.get( 'Changed', [] )
		numberOfBlocksProcessed = d.get( 'NumberOfBlocksProcessed', 0 )
		if changed and numberOfBlocksProcessed > 1:
			for stateName in self.counters:
				values = d[ 'Events' ].get( stateName, None )
				if values is None: continue
				if values.size == 0: values = d[ 'States' ].get( stateName, None )
				if values is None: continue
				values = numpy.asarray( values ).ravel()
				nEvents = numpy.logical_and( pdiff( values ) != 0, values != 0 ).sum()
				if values[ 0 ] not in [ 0, self.lastValues.get( stateName, 0 ) ]: nEvents += 1
				if nEvents: self.counts[ stateName ] = self.counts.setdefault( stateName, 0 ) + nEvents
				self.lastValues[ stateName ] = values[ -1 ]
		t = time.time()
		secondsElapsed = t - d.setdefault( 'ProcessingStartTime', t )
		if self.debug: d.setdefault( 'SecondsElapsedAtCalls', [] ).append( secondsElapsed )
		#self.write( '%s %s %s\n' % ( numberOfBlocksProcessed, bool(changed), self.announcementPeriodInSeconds + d.setdefault( 'LastAnnouncementTime', t ) - t ) )
		if numberOfBlocksProcessed == 0: return False
		if not changed or t >= self.announcementPeriodInSeconds + d.setdefault( 'LastAnnouncementTime', t ):
			secondsProcessed = numberOfBlocksProcessed * d[ 'SamplesPerBlock' ] / float( d[ 'SamplesPerSecond' ] )
			eventsString = ',  '.join( self.counters[ stateName ] + ': ' + str( nEvents ) for stateName, nEvents in self.counts.items() )
			if eventsString: eventsString = ' - ' + eventsString
			self.write( 'processed %5.1f seconds of signal in %4.1fs%s\n' % ( secondsProcessed, secondsElapsed, eventsString ) )
			d[ 'LastAnnouncementTime' ] = t
		return self.KeepBlock( d )
	def KeepBlock( self, d ):
		# to terminate the chain early:  raise TerminateChain(keepBlock=...)
		return True

class TrapFilterCallbackObject( CallbackObject ):
	def KeepBlock( self, d ):
		# to terminate the chain early:  raise TerminateChain(keepBlock=...)
		isNewTrial = d.NumberOfBlocksProcessed > 1 and 'States.TrialsCompleted' in d.Changed and d.States.TrialsCompleted > 0
		return isNewTrial
