////////////////////////////////////////////////////////////////////////////////
// $Id: ContextMenuExtension.h 8238 2024-07-03 13:34:20Z mellinger $
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
#include <string>
#include <list>
#include <shlobj.h>

// {DEFE0E20-067C-4455-B031-E1FBB3AAFC91}
DEFINE_GUID(CLSID_ContextMenuExtension,
    0xdefe0e20, 0x67c, 0x4455, 0xb0, 0x31, 0xe1, 0xfb, 0xb3, 0xaa, 0xfc, 0x91);

class ContextMenuExtension: IUnknown
{
public:
    ContextMenuExtension();
    ~ContextMenuExtension();

    // IUnknown
    HRESULT __stdcall QueryInterface(const IID&, void**) override;
    ULONG __stdcall AddRef() override;
    ULONG __stdcall Release() override;

private:
    // ContextMenuHandler
    struct ShellExtInit : IShellExtInit
    {
        // IUnknown
        HRESULT __stdcall QueryInterface(const IID&, void**) override;
        ULONG __stdcall AddRef() override;
        ULONG __stdcall Release() override;
        // IShellExtInit
        HRESULT __stdcall Initialize(LPCITEMIDLIST, LPDATAOBJECT, HKEY) override;

        ContextMenuExtension* mpParent = nullptr;
    } mShellExtInit;

    struct ContextMenu : IContextMenu
    {
        // IUnknown
        HRESULT __stdcall QueryInterface(const IID&, void**) override;
        ULONG __stdcall AddRef() override;
        ULONG __stdcall Release() override;
        // IContextMenu
        HRESULT __stdcall GetCommandString(UINT_PTR, UINT, UINT*, LPSTR, UINT) override;
        HRESULT __stdcall InvokeCommand(LPCMINVOKECOMMANDINFO) override;
        HRESULT __stdcall QueryContextMenu(HMENU, UINT, UINT, UINT, UINT) override;

        ContextMenuExtension* mpParent = nullptr;
    } mContextMenu;

private:
    void AddIfBciFile(const std::wstring&);
    bool HasPrmFiles() const;
    void OpenFilesWith(HWND, const wchar_t*, bool singleInstance, const wchar_t* prefix = nullptr) const;

    friend struct ShellExtInit;
    friend struct ContextMenu;
    LONG mRefCount;
    std::list<std::wstring> mFileNames;
    bool mHasPrmFiles;
};