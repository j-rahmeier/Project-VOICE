////////////////////////////////////////////////////////////////////////////////
// $id$
// Author: mellinger@neurotechcenter.org
// Description: A utility that creates a Windows shortcut.
////////////////////////////////////////////////////////////////////////////////
#include <Windows.h>
#include <ShlObj.h>

#include <iostream>
#include <iomanip>
#include <sstream>

#include "ComPtr.h"
#include "ComStrings.h"
#include "FileUtils.h"

struct HR_SucceedOrThrow
{
    HR_SucceedOrThrow(HRESULT hr = S_OK)
    : hr(hr)
    {
        if (FAILED(hr)) {
            std::ostringstream oss;
            oss << std::showbase << std::hex << hr;
            throw std::runtime_error(oss.str());
        }
    }
    HRESULT hr;
};

struct CoInit
{
    CoInit()
    {
        HR_SucceedOrThrow hr = ::CoInitialize(nullptr);
    }
    ~CoInit()
    {
        ::CoUninitialize();
    }
};

static const char* sUsage = "make_win32_shortcut <existing file> <new shortcut>";

int main(int argc, char** argv)
{
    if (argc < 3) {
        std::cerr << "Too few arguments, usage:\n" << sUsage << std::endl;
        return -1;
    }
    if (argc > 3) {
        std::cerr << "Too many arguments, usage:\n" << sUsage << std::endl;
        return -1;
    }

    try {
        CoInit init;
        HR_SucceedOrThrow hr;
        com::Ptr<IShellLinkA> pLink;
        hr = ::CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLinkA, pLink.Assignee());
        // Remove all resolve options except relative path.
        com::Ptr<IShellLinkDataList> pDataList;
        hr = pLink->QueryInterface(IID_IShellLinkDataList, pDataList.Assignee());
        DWORD flags;
        pDataList->GetFlags(&flags);
        flags |= SLDF_FORCE_NO_LINKINFO;
        flags |= SLDF_FORCE_NO_LINKTRACK;
        pDataList->SetFlags(flags);

        std::string existingFile = argv[1];
        existingFile = FileUtils::AbsolutePath(existingFile);
        pLink->SetPath(existingFile.c_str());

        com::Ptr<IPersistFile> pLinkFile;
        hr = pLink->QueryInterface(IID_IPersistFile, pLinkFile.Assignee());
        std::string newShortcut = argv[2];
        if (FileUtils::IsDirectory(newShortcut)) {
            newShortcut = FileUtils::EnsureSeparator(newShortcut);
            newShortcut += FileUtils::ExtractBase(existingFile) + ".lnk";
        }
        pLinkFile->Save(com::DualString(newShortcut), FALSE);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}
