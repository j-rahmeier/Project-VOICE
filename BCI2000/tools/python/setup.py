#!/usr/bin/env -S python #
# -*- coding: utf-8 -*-

# ///////////////////////////////////////////////////////////////////////////
# $Id: setup.py 6650 2022-03-30 18:29:06Z jhill $
# Author: jeremy.hill@neurotechcenter.org
# Description: Python pip/setuptools helper file for installing tools/python suite
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
The purpose of this file is to make the contents of `tools/python`
available to your Python interpreter (i.e. listed on its `sys.path`) in any
future Python session.

The recommended way to use this file is to open a command prompt, change
your working directory to the location of this `setup.py` file, and then::

    python -m pip install -e .

The "editable" flag `-e` causes Python's setuptools to simply store a link
to the present location of this subdirectory. The advantage of an editable
installation is that you still have only one definitive copy of the code
(possibly under version control) and any update to those files will then be
adopted automatically following a new Python launch. Another advantage is
that the Python code will automatically be able to find other parts of the
same BCI2000 distribution (for example,  `BCI2000Tools.Chain.bci2000chain()`
relies on the executables in `tools/cmdline` for running offline analyses,
and `BCI2000Tools.Remote.BCI2000` relies on `prog/BCI2000Remote.py` for
talking directly to a running instance of BCI2000). The downside is that
Python will fail to find the files if you ever move the BCI2000 distribution
away from its current location (or rename it, or rename any of its parent
directories).

You could remove the `-e` flag if you want setuptools to make a static
*copy* of the modules/packages from the `tools/python` directory, within the
`site-packages` of your Python distribution or in your virtual environment.
This is the usual/expected way of using `pip`, but it means you give up
the advantages mentioned above. Upgrading the code will be harder (unless
we are also managing and distributing our code via pypi.org, which we do
not intend to do). You will also not be able to find your complete BCI2000
distribution programmatically (this might be fine if the only part you
are *ever* going to use is the file-reader).
"""

import os
import sys
import inspect
import setuptools

package_dir = '.' # the directory that would get added to the path, expressed relative to the location of this setup.py file

try: __file__
except:
	try: frame = inspect.currentframe(); __file__ = inspect.getfile( frame )
	finally: del frame  # https://docs.python.org/3/library/inspect.html#the-interpreter-stack
HERE = os.path.realpath( os.path.dirname( __file__ ) )

# import the package itself - but make sure it's the to-be-installed version and not some legacy version hanging over
#sys.path.insert( 0, os.path.realpath( os.path.join( HERE, package_dir ) ) )
#import BCI2000Tools  # could use this to get the __version__ etc
#sys.path.pop( 0 )

setup_args = dict(
    name='BCI2000Tools', # if we ever add another whole package at the level of BCI2000Tools, this should be changed
    package_dir={ '' : package_dir },
    packages=[ 'BCI2000Tools' ],
    install_requires=[ 'numpy' ], # matplotlib is also a nice-to-have too but not essential
)

if __name__ == '__main__' and getattr( sys, 'argv', [] )[ 1: ]:
	setuptools.setup( **setup_args )
else:
	sys.stderr.write( """
The BCI2000Tools setup.py file should not be run or imported directly.
Instead, it is used as follows::

    python -m pip install -e  "%s"

""" % HERE )

