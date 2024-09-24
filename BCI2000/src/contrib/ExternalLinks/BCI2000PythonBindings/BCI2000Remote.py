# ///////////////////////////////////////////////////////////////////////////
# $Id: BCI2000Remote.py 7298 2023-04-04 15:06:36Z mellinger $
# Author: juergen.mellinger@uni-tuebingen.de, jhill@neurotechcenter.org
# Description: Python bindings for the BCI2000RemoteLib library.
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
"""Python bindings for the BCI2000RemoteLib library.

This module provides a BCI2000Remote class that allows remote control of
the BCI2000 Operator module via Operator Scripting.
It must reside in the same directory as the BCI2000RemoteLib library, which
is usually located the BCI2000/prog directory.
For documentation, see
 http://doc.bci2000.org/index.php/Programming_Reference:BCI2000Remote_Class
 http://doc.bci2000.org/index.php/User_Reference:Operator_Module_Scripting
For a code example, see the end of this file.

"""

from __future__ import absolute_import
import os
import sys
import inspect
import string
import copy
import ctypes
import time

__all__ = [
	'BCI2000Remote',
	'BCI2000RemoteError', 'BCI2000ParameterTypeError', 'BCI2000RemoteRuntimeError','BCI2000NotConnectedError',
]

def string_arg(s):
    return ctypes.c_char_p(s.encode('utf-8'))

class BCI2000RemoteError(Exception): pass
class BCI2000ParameterTypeError(TypeError, BCI2000RemoteError): pass
class BCI2000RemoteRuntimeError(RuntimeError, BCI2000RemoteError): pass
class BCI2000NotConnectedError(BCI2000RemoteRuntimeError): pass

class BCI2000Remote(object):
    """BCI2000 remote control object.

    This class allows remote control of the BCI2000 Operator module via
    Operator Scripting.
    For documentation, see
     http://doc.bci2000.org/index.php/Programming_Reference:BCI2000Remote_Class
     http://doc.bci2000.org/index.php/User_Reference:Operator_Module_Scripting
    """
    # Constructor/destructor

    def __init__(self):
        pyfile = inspect.getfile(inspect.currentframe())
        bcidir = os.path.dirname(os.path.realpath(pyfile))
        bcilib = os.path.join(bcidir, "BCI2000RemoteLib")
        bcioperator = os.path.join(bcidir, "Operator")
        LoadLibrary = ctypes.cdll.LoadLibrary
        if sys.platform.startswith('win'):
            dylib_extension = ".dll"
            LoadLibrary = ctypes.windll.LoadLibrary
            bcioperator += ".exe"
        elif sys.platform.startswith('linux'):
            dylib_extension = ".so"
        elif sys.platform.startswith('darwin'):
            dylib_extension = ".dylib"
        else:
            dylib_extension = ""
        for platform_inflection in [ '64', '.64', '.64bit', '.arm64', '.x86_64', '.amd64', '32', '.32', '.32bit', '.x86' ]:
            try: self._lib = LoadLibrary(bcilib + platform_inflection + dylib_extension)
            except: pass
            else: break
        else: # this will run if the `for` loop did not `break`
            self._lib = LoadLibrary(bcilib + dylib_extension) # no exception-catching this time

        self._lib.BCI2000Remote_New.restype = ctypes.c_void_p
        self._instance = ctypes.c_void_p(self._lib.BCI2000Remote_New())
        self.OperatorPath = bcioperator

    def __del__(self):
        try:
            self._lib.BCI2000Remote_Delete(self._instance)
        except:
            pass

    # Properties
    class _Timeout(object):
        def __get__(self, obj, cls):
            obj._lib.BCI2000Remote_GetTimeout.restype = ctypes.c_double
            return obj._lib.BCI2000Remote_GetTimeout(obj._instance)

        def __set__(self, obj, val):
            obj._lib.BCI2000Remote_SetTimeout(
                obj._instance, ctypes.c_double(val))
    Timeout = _Timeout()

    class _OperatorPath(object):
        def __get__(self, obj, cls):
            return obj._get_string_result(obj._lib.BCI2000Remote_GetOperatorPath)

        def __set__(self, obj, val):
            obj._lib.BCI2000Remote_SetOperatorPath(
                obj._instance, string_arg(val))
    OperatorPath = _OperatorPath()

    INVISIBLE = 0
    VISIBLE = 1
    NO_CHANGE = 2

    class _WindowVisible(object):
        def __get__(self, obj, cls):
            return obj._lib.BCI2000Remote_GetWindowVisible(obj._instance)

        def __set__(self, obj, val):
            obj._lib.BCI2000Remote_SetWindowVisible(
                obj._instance, ctypes.c_int(val))
    WindowVisible = _WindowVisible()

    class _WindowTitle(object):
        def __get__(self, obj, cls):
            return obj._get_string_result(obj._lib.BCI2000Remote_GetWindowTitle)

        def __set__(self, obj, val):
            obj._lib.BCI2000Remote_SetWindowTitle(
                obj._instance, string_arg(val))
    WindowTitle = _WindowTitle()

    class _TelnetAddress(object):
        def __get__(self, obj, cls):
            return obj._get_string_result(obj._lib.BCI2000Remote_GetTelnetAddress)

        def __set__(self, obj, val):
            obj._lib.BCI2000Remote_SetTelnetAddress(
                obj._instance, string_arg(val))
    TelnetAddress = _TelnetAddress()

    class _Result(object):
        def __get__(self, obj, cls=None):
            return obj._get_string_result(obj._lib.BCI2000Remote_GetResult)
    Result = _Result()

    class _Connected(object):
        def __get__(self, obj, cls):
            return obj._lib.BCI2000Remote_GetConnected(obj._instance)
    Connected = _Connected()

    class _SubjectID(object):
        def __get__(self, obj, cls):
            return obj._get_string_result(obj._lib.BCI2000Remote_GetSubjectID)

        def __set__(self, obj, val):
            obj._lib.BCI2000Remote_SetSubjectID(obj._instance, string_arg(val))
    SubjectID = _SubjectID()

    class _SessionID(object):
        def __get__(self, obj, cls):
            return obj._get_string_result(obj._lib.BCI2000Remote_GetSessionID)

        def __set__(self, obj, val):
            obj._lib.BCI2000Remote_SetSessionID(obj._instance, string_arg(val))
    SessionID = _SessionID()

    class _DataDirectory(object):
        def __get__(self, obj, cls):
            return obj._get_string_result(obj._lib.BCI2000Remote_GetDataDirectory)

        def __set__(self, obj, val):
            obj._lib.BCI2000Remote_SetDataDirectory(
                obj._instance, string_arg(val))
    DataDirectory = _DataDirectory()

    # Methods
    def Connect(self):
        return self._lib.BCI2000Remote_Connect(self._instance)

    def Disconnect(self):
        return self._lib.BCI2000Remote_Disconnect(self._instance)

    def Encode(self, val):
        return self._get_string_result(self._lib.BCI2000Remote_Encode, string_arg(val))

    def Execute(self, command, exitCode=[]):
        """Execute an Operator Scripting command.

        This function appends the last command's execution status to the optional list argument.
        """
        i = ctypes.c_int()
        result = self._lib.BCI2000Remote_Execute(
            self._instance, string_arg(command), ctypes.byref(i))
        exitCode.append(i)
        return result

    def StartupModules(self, modules):
        """Start up and connect to a set of core modules.

        Specify a tuple of strings as argument. Each string corresponds to a
        BCI2000 core module, and contains the module's name, followed with any
        command line arguments.
        """
        modulestr = ''
        for module in modules:
            modulestr += module + '\x00'
        modulebytes = (modulestr + '\x00').encode('utf-8')
        return self._lib.BCI2000Remote_StartupModules(self._instance, modulebytes)

    def LoadParametersLocal(self, file):
        return self._lib.BCI2000Remote_LoadParametersLocal(self._instance, string_arg(file))

    def LoadParametersRemote(self, file):
        return self._lib.BCI2000Remote_LoadParametersRemote(self._instance, string_arg(file))

    def SetConfig(self):
        return self._lib.BCI2000Remote_SetConfig(self._instance)

    def Start(self):
        return self._lib.BCI2000Remote_Start(self._instance)

    def Stop(self):
        return self._lib.BCI2000Remote_Stop(self._instance)

    def Quit(self):
        return self._lib.BCI2000Remote_Quit(self._instance)

    def GetSystemState(self):
        return self._get_string_result(self._lib.BCI2000Remote_GetSystemState)

    def GetControlSignal(self, ch, el):
        result = ctypes.c_double()
        if not self._lib.BCI2000Remote_GetControlSignal(self._instance, ch, el, ctypes.byref(result)):
            raise self._error('BCI2000Remote_GetControlSignal() call failed')
        return result

    def SetParameter(self, name, value):
        return self._lib.BCI2000Remote_SetParameter(self._instance, string_arg(name), string_arg(value))

    def GetParameter(self, name):
        return self._get_string_result(self._lib.BCI2000Remote_GetParameter, string_arg(name))

    def GetListParameter(self, name):
        if not self.Execute('list parameter ' + name):
            raise self._error('BCI2000Remote_Execute("list parameter ...") call failed')
        content = self.Result.split() # TODO: under some circumstances self.Result can be unchanged, hanging over from previous call
        if not content[1].lower().endswith('list'): # in which case this test may fail or return the wrong result
            raise BCI2000ParameterTypeError('%s is not a list parameter' % name)
        length = int(content[3])
        return content[4: 4 + length]

    def GetMatrixParameter(self, name):
        if not self.Execute('list parameter ' + name):
            raise self._error('BCI2000Remote_Execute("list parameter ...") call failed')
        content = self.Result.split() # TODO: under some circumstances self.Result can be unchanged, hanging over from previous call
        if not content[1].lower().endswith('matrix'): # in which case this test may fail or return the wrong result
            raise BCI2000ParameterTypeError('%s is not a matrix parameter' % name)
        content = content[3:]
        if content[0] == '{':
            rows = content.index('}') - 1
            content = content[rows + 2:]
        else:
            rows = int(content[0])
            content = content[1:]
        if content[0] == '{':
            columns = content.index('}') - 1
            content = content[columns + 2:]
        else:
            columns = int(content[0])
            content = content[1:]
        content = content[: rows * columns]
        return [[content.pop(0) for column in range(columns)] for row in range(rows)]

    def AddStateVariable(self, name, bit_width, initial_value):
        return self._lib.BCI2000Remote_AddStateVariable(self._instance, string_arg(name), ctypes.c_int(bit_width), ctypes.c_double(initial_value))

    def SetStateVariable(self, name, val):
        result = self._lib.BCI2000Remote_SetStateVariable(
            self._instance, string_arg(name), ctypes.c_double(val))
        if not result:
            raise self._error('BCI2000Remote_SetStateVariable() call failed')

    def GetStateVariable(self, name):
        result = ctypes.c_double()
        if not self._lib.BCI2000Remote_GetStateVariable(self._instance, string_arg(name), ctypes.byref(result)):
            raise self._error('BCI2000Remote_GetStateVariable() call failed')
        return result

    def AddEventVariable(self, name, bit_width, initial_value):
        return self._lib.BCI2000Remote_AddEventVariable(self._instance, string_arg(name), ctypes.c_int(bit_width), ctypes.c_double(initial_value))

    def SetEventVariable(self, name, val):
        result = self._lib.BCI2000Remote_SetEventVariable(
            self._instance, string_arg(name), ctypes.c_double(val))
        if not result:
            raise self._error('BCI2000Remote_SetEventVariable() call failed')

    def PulseEventVariable(self, name, val):
        result = self._lib.BCI2000Remote_PulseEventVariable(
            self._instance, string_arg(name), ctypes.c_double(val))
        if not result:
            raise self._error('BCI2000Remote_PulseEventVariable() call failed')

    def GetEventVariable(self, name):
        result = ctypes.c_double()
        if not self._lib.BCI2000Remote_GetEventVariable(self._instance, string_arg(name), ctypes.byref(result)):
            raise self._error('BCI2000Remote_GetEventVariable() call failed')
        return result

    def SetScript(self, event, script):
        return self._lib.BCI2000Remote_SetScript(self._instance, string_arg(event), string_arg(script))

    def GetScript(self, event):
        return self._get_string_result(self._lib.BCI2000Remote_GetScript, string_arg(event))

    # Internal functions

    def _get_string_result(self, fn, *args):
        fn.restype = ctypes.c_char_p
        s = fn(self._instance, *args)
        if s is None:
            raise self._error('%s() call failed' % fn.__name__)
        result = copy.deepcopy(s)
        result = result.decode()
        self._lib.BCI2000Release(s)
        return result

    def _error(self, msg='BCI2000RemoteLib API call failed'):
        cls = BCI2000RemoteRuntimeError
        try:
            if self.Connected:
                msg += ' - ' + self.Result
            else:
                cls = BCI2000NotConnectedError
                msg += ' - not connected'
        except:
            pass
        return cls(msg)

if __name__ == '__main__':
    # Example code
    bci = BCI2000Remote()
    print("Operator path: " + bci.OperatorPath)
    bci.WindowVisible = True
    bci.WindowTitle = "Python controlled"
    bci.SubjectID = "pysub"
    bci.Connect()
    bci.Execute("cd ${BCI2000LAUNCHDIR}")
    bci.StartupModules(("SignalGenerator", "ARSignalProcessing", "CursorTask"))
    bci.LoadParametersRemote(
        "../parms/examples/CursorTask_SignalGenerator.prm")
    bci.SetConfig()
    print("SubjectName parameter: " + bci.GetParameter("SubjectName"))
    bci.Start()
    bci.Execute("Wait for Suspended 5")
    if bci.Result != "":
        print("Result: " + bci.Result)
    bci.Stop()
    del bci
