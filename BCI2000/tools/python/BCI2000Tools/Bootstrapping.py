# -*- coding: utf-8 -*-

# ///////////////////////////////////////////////////////////////////////////
# $Id: Bootstrapping.py 6663 2022-04-07 00:32:47Z jhill $
# Author: jeremy.hill@neurotechcenter.org
# Description: infer/specify/use the location of the BCI2000 distribution
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
Use `bci2000root()` to set the path to the root directory of the BCI2000
distribution you want to use. It will be stored as a global variable in the
module and remembered for the rest of the session.

Use `bci2000path()` to resolve partial paths relative to the currently
configured `bci2000root()` setting.

By default, when you first import this submodule, it attempts to set
`bci2000root()` based on the assumption that the module itself is installed
within the `tools/python` subtree of the relevant BCI2000 distribution
(perhaps using `pip install -e`, as recommended in our `setup.py` file). If
this fails, `bci2000root()` will be left as `None`.
"""

__all__ = [
	'bci2000root', 'bci2000path',
]

import os
import sys
import inspect

if sys.version < '3': bytes = str
else: unicode = str; basestring = ( unicode, bytes )

BCI2000_ROOT_DIR = None

def bci2000root( set=None ):
	"""
	Specify, or query, the absolute path to the root directory of the BCI2000
	distribution (the parent directory of `batch`, `parms`, `prog` and `tools`).
	
	If this has been set, `bci2000chain()` will look for command-line binaries
	in the `tools/cmdline` subdirectory of the root, by default.
	"""
	global BCI2000_ROOT_DIR
	if set is not None: BCI2000_ROOT_DIR = os.path.realpath( os.path.expanduser( set ) )
	return BCI2000_ROOT_DIR

def bci2000path( partial_path ):
	"""
	If `bci2000root()` has been set, and the specified `partial_path` exists
	relative to it, join them together and return the resulting absolute path.
	
	If not, just return the result of `os.path.realpath(partial_path)`.
	"""
	if isinstance( partial_path, basestring ) and isinstance( BCI2000_ROOT_DIR, basestring ) and not os.path.exists( partial_path ) and os.path.exists( os.path.join( BCI2000_ROOT_DIR, partial_path ) ):
		return os.path.realpath( os.path.join( BCI2000_ROOT_DIR, partial_path ) )
	return os.path.realpath( partial_path )


# Now let's automatically try to fill in a default for BCI2000_ROOT_DIR based
# on the location of this Python module itself.

class NoParentDirectory( ValueError ): pass

def WhereAmI( nFileSystemLevelsUp=1, nStackLevelsBack=0 ):
	"""
	`WhereAmI( 0 )` is equivalent to `__file__`
	
	`WhereAmI()` or `WhereAmI(1)` gives you the current source file's
	parent directory.
	"""
	my_getfile = inspect.getfile
	if getattr( sys, 'frozen', False ) and hasattr( sys, '_MEIPASS' ):
		# sys._MEIPASS indicates that we're in PyInstaller which, in a surprise reversal
		# of the old py2exe situation, supports `__file__` but NOT `inspect.getfile()`.
		# The following workaround is adapted from
		# http://lists.swapbytes.de/archives/obspy-users/2017-April/002395.html
		def my_getfile( object ):
			if inspect.isframe( object ):
				try: return object.f_globals[ '__file__' ]
				except: pass
			return inspect.getfile( object )
			
	try:
		frame = inspect.currentframe()
		for i in range( abs( nStackLevelsBack ) + 1 ):
			frame = frame.f_back
		file = my_getfile( frame )
	finally:
		del frame  # https://docs.python.org/3/library/inspect.html#the-interpreter-stack
	result = file = os.path.realpath( file )
	nFileSystemLevelsUp = abs( nFileSystemLevelsUp )
	for i in range( nFileSystemLevelsUp ):
		pardir = os.path.dirname( result )
		if pardir == result: raise NoParentDirectory( 'cannot go %d file-system levels up from %s (maximum is %d)' % ( nFileSystemLevelsUp, file, i ) )
		result = pardir
	return result

try:  candidate = WhereAmI( 4 ) # assume we are in $BCI2000_ROOT_DIR/tools/python/BCI2000Tools/Bootstrapping.py ...
except NoParentDirectory: pass
else:
	for partial in [ 'tools/python' ]: # but if we go up 4 levels and don't find all the partial paths in this list beneath us, cancel that assumption
		if not os.path.isdir( os.path.join( candidate, *partial.split( '/' ) ) ): break
	else: # if no break has occurred
		BCI2000_ROOT_DIR = candidate
