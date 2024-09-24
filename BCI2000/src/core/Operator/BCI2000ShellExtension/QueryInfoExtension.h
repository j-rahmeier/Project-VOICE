////////////////////////////////////////////////////////////////////////////////
// $Id: QueryInfoExtension.h 7463 2023-06-30 14:59:17Z mellinger $
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
#include <shlobj.h>
#include "BCI2000FileReader.h"

// {DEFE0E21-067C-4455-B031-E1FBB3AAFC91}
DEFINE_GUID(CLSID_QueryInfoExtension,
    0xdefe0e21, 0x67c, 0x4455, 0xb0, 0x31, 0xe1, 0xfb, 0xb3, 0xaa, 0xfc, 0x91);

class QueryInfoExtension: IUnknown
{
public:
    QueryInfoExtension();
    ~QueryInfoExtension();

    // IUnknown
    HRESULT __stdcall QueryInterface(const IID&, void**) override;
    ULONG __stdcall AddRef() override;
    ULONG __stdcall Release() override;

private:
    struct PersistFile : IPersistFile
    {
        // IUnknown
        HRESULT __stdcall QueryInterface(const IID&, void**) override;
        ULONG __stdcall AddRef() override;
        ULONG __stdcall Release() override;
        // IPersistFile
        HRESULT __stdcall GetClassID(LPCLSID) override { return E_NOTIMPL; }
        HRESULT __stdcall IsDirty() override { return E_NOTIMPL; }
        HRESULT __stdcall Load(LPCOLESTR, DWORD) override;
        HRESULT __stdcall Save(LPCOLESTR, BOOL) override { return E_NOTIMPL; }
        HRESULT __stdcall SaveCompleted(LPCOLESTR) override { return E_NOTIMPL; }
        HRESULT __stdcall GetCurFile(LPOLESTR*) override { return E_NOTIMPL; }

        QueryInfoExtension* mpParent = nullptr;
    } mPersistFile;

    struct QueryInfo : IQueryInfo
    {
        // IUnknown
        HRESULT __stdcall QueryInterface(const IID&, void**) override;
        ULONG __stdcall AddRef() override;
        ULONG __stdcall Release() override;
        // IQueryInfo
        HRESULT __stdcall GetInfoFlags(DWORD*) override { return E_NOTIMPL; }
        HRESULT __stdcall GetInfoTip(DWORD, LPWSTR*) override;

        QueryInfoExtension* mpParent = nullptr;
    } mQueryInfo;

private:
    std::wstring GetObjectName() const;
    std::wstring GetShortInfo() const;
    std::wstring GetLongInfo() const;

    friend struct PersistFile;
    friend struct QueryInfo;
    LONG mRefCount;
    BCI2000FileReader mFile;
    std::wstring mFileName;
};