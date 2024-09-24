////////////////////////////////////////////////////////////////////////////////
//  $Id: FilterWrapperLibrary.cpp 7464 2023-06-30 15:04:08Z mellinger $
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
#include "FilterWrapperLibrary.h"
#include "GenericFilter.h"
#include "NullMessageChannel.h"
#include "VersionInfo.h"

namespace
{
NullMessageChannel sNullChannel;

const char *allocateString(const char *s)
{
    char *p = new char[::strlen(s) + 1];
    ::strcpy(p, s);
    return p;
}

void freeString(const char *p)
{
    delete[] p;
}
} // namespace

const char *STDCALL InterfaceVersion()
{
    return allocateString(FILTER_WRAPPER_INTERFACE_VERSION);
}

const char *STDCALL Compiler()
{
    return allocateString(VersionInfo::Current[VersionInfo::Compiler].c_str());
}

const char *STDCALL FilterName()
{
    const auto &reg = GenericFilter::Registrar::Registrars();
    if (reg.size() == 0)
        return allocateString("<no filter>");
    if (reg.size() > 1)
        return allocateString("<multiple filters>");
    return allocateString(ClassName((*reg.begin())->Typeid()).c_str());
}

const char *STDCALL BuildInfo()
{
    std::ostringstream oss;
    oss << VersionInfo::Current;
    return allocateString(oss.str().c_str());
}

const char *STDCALL BuildType()
{
    return allocateString(VersionInfo::Current[VersionInfo::BuildType].c_str());
}

void STDCALL ReleaseString(const char *s)
{
    freeString(s);
}

void STDCALL SetBcierr(BCIStream::OutStream *pStream)
{
    bcierr___(*pStream);
}

void STDCALL SetBciout(BCIStream::OutStream *pStream)
{
    bciout___(*pStream);
}

void STDCALL SetBciwarn(BCIStream::OutStream *pStream)
{
    bciwarn___(*pStream);
}

void STDCALL SetBcidbg(BCIStream::OutStream *pStream)
{
    bcidbg___(*pStream);
}

void STDCALL SetVisualizationChannel(bci::MessageChannel *pChannel)
{
    if (pChannel)
        GenericVisualization::SetOutputChannel(pChannel);
    else
        GenericVisualization::SetOutputChannel(&sNullChannel);
}

void STDCALL SetMeasurementUnits(MeasurementUnits *pInstance)
{
    MeasurementUnits::SetInstance(pInstance);
}

void STDCALL SetEnvironmentContext(Environment::Context *pContext)
{
    Environment::Context::SetCurrentInstance(pContext);
}

GenericFilter *STDCALL Instantiate(Directory::Node *pNode)
{
    const auto &reg = GenericFilter::Registrar::Registrars();
    if (reg.size() == 0)
        return nullptr;
    if (reg.size() > 1)
        return nullptr;
    auto pRegistrar = *reg.begin();
    auto pFilter = pRegistrar->NewInstance();
    if (pFilter)
        pFilter->SetParent(pNode);
    return pFilter;
}
