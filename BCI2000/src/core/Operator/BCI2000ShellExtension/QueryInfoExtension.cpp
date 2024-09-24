////////////////////////////////////////////////////////////////////////////////
// $Id: QueryInfoExtension.cpp 7516 2023-08-04 13:58:22Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A shell extension that displays tooltip information for
//    BCI2000 files.
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
#include "ComClassFactory.h"
#include "ComModule.h"
#include "ComStrings.h"
#include "BCI2000FileReader.h"

#include <shlwapi.h>
#include <shlobj_core.h>

#include <sstream>
#include <iomanip>

#include <initguid.h>
#include "QueryInfoExtension.h"

enum {
    ViewerMenuEntry = 0,
    FileInfoMenuEntry = 1,
    AnalysisMenuEntry = 2,

    NumMenuEntries
};

static com::ClassFactory<QueryInfoExtension, &CLSID_QueryInfoExtension> sFactory;

namespace {
  std::string FormatTime(int seconds)
  {
      std::ostringstream oss;
      int hrs = seconds / 3600;
      seconds %= 3600;
      int mins = seconds / 60;
      seconds %= 60;
      oss << std::setfill('0') << std::setw(2) << hrs << ":" 
          << std::setw(2) << mins << ":" << std::setw(2) << seconds;
      return oss.str();
  }
}

QueryInfoExtension::QueryInfoExtension()
: mRefCount(0)
{
    mPersistFile.mpParent = this;
    mQueryInfo.mpParent = this;
}

QueryInfoExtension::~QueryInfoExtension()
{
}

// IUnknown
HRESULT QueryInfoExtension::QueryInterface(const IID& IID, void** pInterface)
{
    *pInterface = nullptr;
    if (IID == IID_IUnknown)
        *pInterface = static_cast<IUnknown*>(this);
    else if (IID == IID_IPersistFile)
        *pInterface = static_cast<IPersistFile*>(&mPersistFile);
    else if (IID == IID_IQueryInfo)
        *pInterface = static_cast<IQueryInfo*>(&mQueryInfo);
    if (*pInterface)
        this->AddRef();
    return *pInterface ? NOERROR : E_NOINTERFACE;
}

ULONG QueryInfoExtension::AddRef()
{
    return ::InterlockedIncrement(&mRefCount);
}

ULONG QueryInfoExtension::Release()
{
    LONG count = ::InterlockedDecrement(&mRefCount);
    if (count == 0)
        delete this;
    return count;
}

HRESULT QueryInfoExtension::PersistFile::QueryInterface(const IID& IID, void** pInterface)
{
    return mpParent->QueryInterface(IID, pInterface);
}

ULONG QueryInfoExtension::PersistFile::AddRef()
{
    return mpParent->AddRef();
}

ULONG QueryInfoExtension::PersistFile::Release()
{
    return mpParent->Release();
}

HRESULT QueryInfoExtension::QueryInfo::QueryInterface(const IID& IID, void** pInterface)
{
    return mpParent->QueryInterface(IID, pInterface);
}

ULONG QueryInfoExtension::QueryInfo::AddRef()
{
    return mpParent->AddRef();
}

ULONG QueryInfoExtension::QueryInfo::Release()
{
    return mpParent->Release();
}

// IPersistFile
HRESULT QueryInfoExtension::PersistFile::Load(LPCOLESTR pName, DWORD mode)
{
    DWORD attr = ::GetFileAttributesW(pName);
    if (attr == INVALID_FILE_ATTRIBUTES)
        return E_FAIL;
    if (attr & FILE_ATTRIBUTE_OFFLINE) // avoid recall
        return E_FAIL;
    mpParent->mFileName = pName;
    mpParent->mFile.Open(com::DualString(pName), 0);
    return mpParent->mFile.IsOpen() ? S_OK : E_FAIL;
}

// IQueryInfo
HRESULT QueryInfoExtension::QueryInfo::GetInfoTip(DWORD flags, LPWSTR* ppInfo)
{
    std::wstring info;
    if (flags & QITIPF_USENAME)
        info = mpParent->GetObjectName();
    else if (flags & QITIPF_SINGLELINE)
        info = mpParent->GetShortInfo();
    else
        info = mpParent->GetLongInfo();
    wchar_t* buf = static_cast<wchar_t*>(::CoTaskMemAlloc(sizeof(info.front()) * (info.length() + 1)));
    if (!buf)
        return E_OUTOFMEMORY;
    *ppInfo = ::StrCpyW(buf, info.c_str());
    return S_OK;
}

// QueryInfoExtension
std::wstring QueryInfoExtension::GetObjectName() const
{
    return mFileName;
}

std::wstring QueryInfoExtension::GetShortInfo() const
{
    int durationSeconds = ::floor(mFile.NumSamples() / mFile.SamplingRate());
    std::ostringstream oss;
    oss << "Type: BCI2000 data file, "
        << "Duration: " << FormatTime(durationSeconds);
    return com::DualString(oss.str());
}

std::wstring QueryInfoExtension::GetLongInfo() const
{
    for (const char* name : { "SamplingRate", "SourceCh", "StorageTime" })
        if (!mFile.Parameters()->Exists(name))
            return GetShortInfo();

    std::string samplingRate = mFile.Parameter("SamplingRate");
    if (samplingRate.find("Hz") == std::string::npos)
        samplingRate += "Hz";
    int durationSeconds = ::floor(mFile.NumSamples() / mFile.SamplingRate());
    double updateRate = mFile.SamplingRate() / mFile.SignalProperties().Elements();
    std::ostringstream oss;
    oss << "Type: BCI2000 data file\n"
        << "Sampling rate: " << samplingRate << "\n"
        << "Update rate: " << updateRate << "Hz\n"
        << "Duration: " << FormatTime(durationSeconds) << "\n"
        << "Channels: " << mFile.Parameter("SourceCh") << "\n"
        << "Storage time: " << mFile.Parameter("StorageTime");
    return com::DualString(oss.str());
}

