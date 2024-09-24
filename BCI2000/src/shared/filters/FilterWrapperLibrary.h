////////////////////////////////////////////////////////////////////////////////
//  $Id: FilterWrapperLibrary.h 7464 2023-06-30 15:04:08Z mellinger $
//  Author: mellinger@neurotechcenter.org
//  Description: Interface definition for a library containing a GenericFilter.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2023: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#ifndef FILTER_WRAPPER_LIBRARY_H
#define FILTER_WRAPPER_LIBRARY_H

#ifdef _WIN32
#define STDCALL __stdcall
#ifdef FILTER_WRAPPER_LIBRARY
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif /* FILTER_WRAPPER_LIBRARY */
#elif __GNUC__ && __GNUC__ >= 4
#define STDCALL
#define DLLEXPORT __attribute__((visibility("default")))
#else
#define STDCALL
#define DLLEXPORT
#endif

class GenericFilter;
class SignalProperties;
class GenericSignal;
class ParamList;
class StateList;
class StateVector;
class MeasurementUnits;

namespace bci
{
class MessageChannel;
}

namespace BCIStream
{
class OutStream;
}

namespace Directory
{
class Node;
}

#include "Environment.h"

extern "C"
{
#define FILTER_WRAPPER_INTERFACE_VERSION "1.0"
    DLLEXPORT const char *STDCALL InterfaceVersion();
    DLLEXPORT const char *STDCALL Compiler();
    DLLEXPORT const char *STDCALL BuildType();
    DLLEXPORT const char *STDCALL BuildInfo();
    DLLEXPORT const char *STDCALL FilterName();
    // call ReleaseString() on all const char* return values
    DLLEXPORT void STDCALL ReleaseString(const char *);

    DLLEXPORT void STDCALL SetBcierr(BCIStream::OutStream *);
    DLLEXPORT void STDCALL SetBciout(BCIStream::OutStream *);
    DLLEXPORT void STDCALL SetBciwarn(BCIStream::OutStream *);
    DLLEXPORT void STDCALL SetBcidbg(BCIStream::OutStream *);

    DLLEXPORT void STDCALL SetVisualizationChannel(bci::MessageChannel *);
    DLLEXPORT void STDCALL SetMeasurementUnits(MeasurementUnits *);
    DLLEXPORT void STDCALL SetEnvironmentContext(Environment::Context *);

    DLLEXPORT GenericFilter *STDCALL Instantiate(Directory::Node *);
}

#endif // FILTER_WRAPPER_LIBRARY_H
