////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000ShellExtension.cpp 8236 2024-07-03 13:14:37Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A shell extension that displays BCI2000 document icons,
//    provides an info tooltip and a context menu.
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
#include "ComModule.h"
#include <Shlobj.h>

using namespace com;

HRESULT DllInstallRefresh(void);

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
  if( dwReason == DLL_PROCESS_ATTACH )
    Module::Init( hInstance );
  return TRUE;
}

// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
  return Module::DllCanUnloadNow();
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
  return Module::DllGetClassObject( rclsid, riid, ppv );
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
  // registers object, typelib and all interfaces in typelib
  HRESULT result = Module::DllRegisterServer();
  ::SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, nullptr, nullptr);
  return result;
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
  HRESULT result = Module::DllUnregisterServer();
  ::SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, nullptr, nullptr);
  return result;
}

// DllInstall - Adds/Removes entries to the system registry per user
//              per machine.
STDAPI DllInstall(BOOL bInstall, LPCWSTR pszCmdLine)
{
  HRESULT result = S_OK;
  if (bInstall && pszCmdLine && !::_wcsicmp(pszCmdLine, L"refresh"))
    result = DllInstallRefresh();
  else
    result = Module::DllInstall(bInstall, pszCmdLine);

  ::SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, nullptr, nullptr);
  return result;
}

// DllInstallRefresh - Re-install if path has changed
HRESULT DllInstallRefresh(void)
{
    bool callDllInstall = false;
    // In BCI2000ShellExtension.rgs, we register our own path under
    // HKCR\bcidatfile\comdll. As HKCR is synthesized from the class registry of both
    // HKCU and HKLM, it will appear there no matter whether per-user or per-system
    // registration was used.
    // If the path agrees with our module path, we have nothing to do.
    // Otherwise, we run DllInstall() to install in the current user's registry.
    HKEY hKey = NULL;
    LSTATUS status = ::RegOpenKeyExW(HKEY_CLASSES_ROOT, L"bcidatfile\\comdll", 0, KEY_READ, &hKey);
    if (status == ERROR_FILE_NOT_FOUND)
        callDllInstall = true;
    else if (status == ERROR_SUCCESS)
    {
        DWORD type = 0, length = 0;
        if (ERROR_SUCCESS == ::RegQueryValueExW(hKey, nullptr, nullptr, &type, nullptr, &length))
        {
            wchar_t* pBuffer = new wchar_t[(length + 2)/2];
            if (ERROR_SUCCESS == ::RegQueryValueExW(hKey, nullptr, nullptr, &type, (BYTE*)pBuffer, &length));
            {
                pBuffer[(length + 2)/2 - 1] = 0;
                // Now, determine and compare canonical paths to have the result independent of
                // long/short path, and capitalization.
                DWORD length2 = ::GetShortPathNameW(pBuffer, nullptr, 0);
                if (length2 == 0)
                {
                    DWORD err = ::GetLastError();
                    switch (err)
                    {
                        case ERROR_PATH_NOT_FOUND:
                        case ERROR_FILE_NOT_FOUND:
                            callDllInstall = true;
                            break;
                    }
                }
                else if (length2 > 0)
                {
                    wchar_t* pBuffer2 = new wchar_t[length2];
                    ::GetShortPathNameW(pBuffer, pBuffer2, length2);
                    DWORD length3 = ::GetShortPathNameW(Module::GetFileName(), nullptr, 0);
                    if (length3 > 0)
                    {
                        wchar_t* pBuffer3 = new wchar_t[length3];
                        ::GetShortPathNameW(Module::GetFileName(), pBuffer3, length3);
                        if (_wcsicmp(pBuffer2, pBuffer3))
                            callDllInstall = true;
                        delete[] pBuffer3;
                    }
                    delete[] pBuffer2;
                }
            }
            delete[] pBuffer;
        }
        ::RegCloseKey(hKey);
    }
    if (callDllInstall)
        return DllInstall(TRUE, L"user");
    return S_OK;
}
