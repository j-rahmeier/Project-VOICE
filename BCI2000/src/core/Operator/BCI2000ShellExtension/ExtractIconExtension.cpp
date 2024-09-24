////////////////////////////////////////////////////////////////////////////////
// $Id: ExtractIconExtension.cpp 7516 2023-08-04 13:58:22Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A shell icon extension that dynamically displays icons for
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
#include <initguid.h>
#include "ExtractIconExtension.h"

static com::ClassFactory<ExtractIconExtension, &CLSID_ExtractIconExtension> sFactory;

ExtractIconExtension::ExtractIconExtension()
: mRefCount(0), mIsBciFile(false)
{
    mPersistFile.mpParent = this;
    mExtractIcon.mpParent = this;
}

ExtractIconExtension::~ExtractIconExtension()
{
}

// IUnknown
HRESULT ExtractIconExtension::QueryInterface(const IID& IID, void** pInterface)
{
    *pInterface = nullptr;
    if (IID == IID_IUnknown)
        *pInterface = static_cast<IUnknown*>(this);
    else if (IID == IID_IPersistFile)
        *pInterface = static_cast<IPersistFile*>(&mPersistFile);
    else if (IID == IID_IExtractIconW)
        *pInterface = static_cast<IExtractIconW*>(&mExtractIcon);
    if (*pInterface)
        this->AddRef();
    return *pInterface ? NOERROR : E_NOINTERFACE;
}

ULONG ExtractIconExtension::AddRef()
{
    return ::InterlockedIncrement(&mRefCount);
}

ULONG ExtractIconExtension::Release()
{
    LONG count = ::InterlockedDecrement(&mRefCount);
    if (count == 0)
        delete this;
    return count;
}

HRESULT ExtractIconExtension::PersistFile::QueryInterface(const IID& IID, void** pInterface)
{
    return mpParent->QueryInterface(IID, pInterface);
}

ULONG ExtractIconExtension::PersistFile::AddRef()
{
    return mpParent->AddRef();
}

ULONG ExtractIconExtension::PersistFile::Release()
{
    return mpParent->Release();
}

HRESULT ExtractIconExtension::ExtractIcon::QueryInterface(const IID& IID, void** pInterface)
{
    return mpParent->QueryInterface(IID, pInterface);
}

ULONG ExtractIconExtension::ExtractIcon::AddRef()
{
    return mpParent->AddRef();
}

ULONG ExtractIconExtension::ExtractIcon::Release()
{
    return mpParent->Release();
}

// IPersistFile
HRESULT ExtractIconExtension::PersistFile::Load(LPCOLESTR pName, DWORD mode)
{
    DWORD attr = ::GetFileAttributesW(pName);
    if (attr == INVALID_FILE_ATTRIBUTES)
        return S_OK;
    if (attr & FILE_ATTRIBUTE_OFFLINE) // avoid recall
        return S_OK;
    BCI2000FileReader reader;
    mpParent->mIsBciFile = reader.Open(com::DualString(pName), 0).IsOpen();
    return S_OK;
}

// IExtracIconW
HRESULT ExtractIconExtension::ExtractIcon::GetIconLocation(UINT uFlags, LPWSTR szIconFile, UINT cchMax, int* piIndex, UINT* pwFlags)
{
    if (mpParent->mIsBciFile)
    {
        const wchar_t* pModulePath = com::Module::GetFileName();
        int length = ::wcslen(pModulePath);
        if (length >= cchMax)
            return E_FAIL;
        ::wcscpy(szIconFile, pModulePath);
        *piIndex = 1;
        *pwFlags = 0;
        return S_OK;
    }
    return S_FALSE;
}

HRESULT ExtractIconExtension::ExtractIcon::Extract(LPCWSTR pszFile, UINT nIconIndex, HICON* phiconLarge, HICON* phiconSmall, UINT nIconSize)
{
    return S_FALSE;
}
