# -*- coding: utf-8 -*-

# ///////////////////////////////////////////////////////////////////////////
# $Id: __init__.py 6648 2022-03-30 16:51:49Z jhill $
# Author: jeremy.hill@neurotechcenter.org
# Description: root namespace of the BCI2000Tools package
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



from . import Bootstrapping; from .Bootstrapping import *

# NB: do NOT import .Remote automatically (it has side effects and may fail, depending on bci2000root() setting)
