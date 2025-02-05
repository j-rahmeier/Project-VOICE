////////////////////////////////////////////////////////////////////////////////
// $Id: ContextMenuExtension.cpp 8238 2024-07-03 13:34:20Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A shell context menu extension that provides access to
//     BCI2000 tools from Explorer windows.
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
#include <initguid.h>
#include "ContextMenuExtension.h"

enum {
    ViewerMenuEntry = 0,
    FileInfoMenuEntry = 1,
    AnalysisMenuEntry = 2,
    P300ClassifierMenuEntry = 3,

    NumMenuEntries
};

static com::ClassFactory<ContextMenuExtension, &CLSID_ContextMenuExtension> sFactory;

ContextMenuExtension::ContextMenuExtension()
: mRefCount(0), mHasPrmFiles(false)
{
    mShellExtInit.mpParent = this;
    mContextMenu.mpParent = this;
}

ContextMenuExtension::~ContextMenuExtension()
{
}

// IUnknown
HRESULT ContextMenuExtension::QueryInterface(const IID& IID, void** pInterface)
{
    *pInterface = nullptr;
    if (IID == IID_IUnknown)
        *pInterface = static_cast<IUnknown*>(this);
    else if (IID == IID_IShellExtInit)
        *pInterface = static_cast<IShellExtInit*>(&mShellExtInit);
    else if (IID == IID_IContextMenu)
        *pInterface = static_cast<IContextMenu*>(&mContextMenu);
    if (*pInterface)
        this->AddRef();
    return *pInterface ? NOERROR : E_NOINTERFACE;
}

ULONG ContextMenuExtension::AddRef()
{
    return ::InterlockedIncrement(&mRefCount);
}

ULONG ContextMenuExtension::Release()
{
    LONG count = ::InterlockedDecrement(&mRefCount);
    if (count == 0)
        delete this;
    return count;
}

HRESULT ContextMenuExtension::ShellExtInit::QueryInterface(const IID& IID, void** pInterface)
{
    return mpParent->QueryInterface(IID, pInterface);
}

ULONG ContextMenuExtension::ShellExtInit::AddRef()
{
    return mpParent->AddRef();
}

ULONG ContextMenuExtension::ShellExtInit::Release()
{
    return mpParent->Release();
}

HRESULT ContextMenuExtension::ContextMenu::QueryInterface(const IID& IID, void** pInterface)
{
    return mpParent->QueryInterface(IID, pInterface);
}

ULONG ContextMenuExtension::ContextMenu::AddRef()
{
    return mpParent->AddRef();
}

ULONG ContextMenuExtension::ContextMenu::Release()
{
    return mpParent->Release();
}

// IShellExtInit
HRESULT ContextMenuExtension::ShellExtInit::Initialize(LPCITEMIDLIST, LPDATAOBJECT pDataObject, HKEY)
{
    FORMATETC fmt = { CF_HDROP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    STGMEDIUM stg = { TYMED_HGLOBAL };
    HDROP hDrop = NULL;

    // Look for CF_HDROP data in the data object.
    if (FAILED(pDataObject->GetData(&fmt, &stg)))
        return E_INVALIDARG;

    hDrop = static_cast<HDROP>(::GlobalLock(stg.hGlobal));
    if (hDrop == NULL)
        return E_INVALIDARG;

    int fileCount = ::DragQueryFileW(hDrop, 0xFFFFFFFF, nullptr, 0);
    for (int i = 0; i < fileCount; ++i)
    {
        int nameLength = ::DragQueryFileW(hDrop, i, nullptr, 0);
        if (nameLength > 0)
        {
            wchar_t* buf = new wchar_t[nameLength + 1];
            if (::DragQueryFileW(hDrop, i, buf, nameLength + 1) > 0)
                mpParent->AddIfBciFile(buf);
            delete[] buf;
        }
    }
    ::GlobalUnlock(stg.hGlobal);
    ::ReleaseStgMedium(&stg);
    return S_OK;
}

// IContextMenu
HRESULT ContextMenuExtension::ContextMenu::GetCommandString(UINT_PTR, UINT, UINT*, LPSTR, UINT)
{
    return E_NOTIMPL;
}

HRESULT ContextMenuExtension::ContextMenu::InvokeCommand(LPCMINVOKECOMMANDINFO pInfo)
{
    // If lpVerb points to a string, ignore this function call.
    if (0 != HIWORD(pInfo->lpVerb))
        return E_INVALIDARG;

    // Get the command index.
    switch (LOWORD(pInfo->lpVerb))
    {
    case ViewerMenuEntry:
        mpParent->OpenFilesWith(pInfo->hwnd, L"BCI2000Viewer", false);
        return S_OK;
    case FileInfoMenuEntry:
        mpParent->OpenFilesWith(pInfo->hwnd, L"BCI2000FileInfo", false);
        return S_OK;
    case AnalysisMenuEntry:
        mpParent->OpenFilesWith(pInfo->hwnd, L"BCI2000Analysis", true);
        return S_OK;
    case P300ClassifierMenuEntry:
        mpParent->OpenFilesWith(pInfo->hwnd, L"P300Classifier", true, L"-TrainingDataFiles");
        return S_OK;
    }
    return E_INVALIDARG;
}

HRESULT ContextMenuExtension::ContextMenu::QueryContextMenu(HMENU hMenu, UINT uMenuIndex, UINT uidFirstCmd,
    UINT uidLastCmd, UINT uFlags)
{
    // If the flags include CMF_DEFAULTONLY then we shouldn't do anything.
    if (uFlags & CMF_DEFAULTONLY)
        return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);

    if (mpParent->mFileNames.empty())
        return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);

    if (uidLastCmd - uidFirstCmd < NumMenuEntries + 1)
        return E_INVALIDARG;

    HMENU hSubmenu = ::CreatePopupMenu();
    DWORD grayed = 0;
    if (mpParent->HasPrmFiles())
        grayed = MF_GRAYED;
    ::InsertMenuA(hSubmenu, ViewerMenuEntry, MF_BYPOSITION | grayed,
        uidFirstCmd + ViewerMenuEntry, "Open with BCI2000&Viewer");
    ::InsertMenuA(hSubmenu, FileInfoMenuEntry, MF_BYPOSITION,
        uidFirstCmd + FileInfoMenuEntry, "Open with BCI2000File&Info");
    ::InsertMenuA(hSubmenu, AnalysisMenuEntry, MF_BYPOSITION | grayed,
        uidFirstCmd + AnalysisMenuEntry, "Open with BCI2000&Analysis");
    ::InsertMenuA(hSubmenu, P300ClassifierMenuEntry, MF_BYPOSITION | grayed,
        uidFirstCmd + P300ClassifierMenuEntry, "Open with &P300Classifier");

    MENUITEMINFOA mii = { sizeof(MENUITEMINFOA) };
    mii.fMask = MIIM_SUBMENU | MIIM_STRING | MIIM_ID;
    mii.wID = uidFirstCmd + NumMenuEntries;
    mii.hSubMenu = hSubmenu;
    mii.dwTypeData = "&BCI2000";
    ::InsertMenuItemA(hMenu, uMenuIndex, TRUE, &mii);

    return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, NumMenuEntries + 1);
}

void ContextMenuExtension::AddIfBciFile(const std::wstring& file)
{
    DWORD attr = ::GetFileAttributesW(file.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES)
        return;
    if (attr & FILE_ATTRIBUTE_OFFLINE) // avoid recall
        return;
    BCI2000FileReader reader;
    if (reader.Open(com::DualString(file), 0).IsOpen()) {
        mFileNames.push_back(file);
        return;
    }
    ParamList paramlist;
    if (paramlist.Load(com::DualString(file))) {
        mFileNames.push_back(file);
        mHasPrmFiles = true;
        return;
    }
}

bool ContextMenuExtension::HasPrmFiles() const
{
    return mHasPrmFiles;
}

void ContextMenuExtension::OpenFilesWith(HWND hWnd, const wchar_t* program, bool singleInstance, const wchar_t* prefix) const
{
    std::wstring exe = com::Module::GetLocation();
    exe += L"..\\Tools\\";
    exe += program;
    exe += L"\\";
    exe += program;
    exe += L".exe";
    if (singleInstance)
    {
        std::wstring args;
        if (prefix)
            args += prefix;
        for (const auto& file : mFileNames)
            args += L" \"" + file + L"\"";
        ::ShellExecuteW(hWnd, nullptr, exe.c_str(), args.c_str(), nullptr, SW_SHOW);
    }
    else
    {
        for (const auto& file : mFileNames)
        {
            std::wstring arg;
            if (prefix)
                arg += prefix;
            arg += L" \"" + file + L"\"";
            ::ShellExecuteW(hWnd, nullptr, exe.c_str(), arg.c_str(), nullptr, SW_SHOWNORMAL);
        }
    }
}

