//////////////////////////////////////////////////////////////////////
// $Id: FileUtils.cpp 8245 2024-07-09 18:05:38Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: File-system related utility functions.
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
///////////////////////////////////////////////////////////////////////
#if _WIN32
#include <Windows.h>
#define S_ISLNK(x) (false)
#if _MSC_VER
#include <direct.h>
#define S_ISREG(x) ((x)&S_IFREG)
#define S_ISDIR(x) ((x)&S_IFDIR)
#else // _MSC_VER
#include <dir.h>
#endif // _MSC_VER
#elif __APPLE__
#include <mach-o/dyld.h>
#endif // _WIN32

#if _MSC_VER
#include "../compat/dirent_win.h"
#else // _MSC_VER
#include <dirent.h>
#endif // _MSC_VER

#ifndef _WIN32
#include <cstdio>
#include <unistd.h>
#endif // _WIN32

#include <cerrno>
#include <cstdlib>
#include <sys/stat.h>
#include <thread>

#if _WIN32
#define stat _stat64
#endif

#include "Debugging.h"
#include "FilePath.h"
#include "FileUtils.h"
#include "LCRandomGenerator.h"
#include "ProcessUtils.h"
#include "StringUtils.h"

using Tiny::FilePath;

namespace
{

std::string& ThreadWD()
{
    static thread_local std::string instance;
    if (instance.empty())
        instance = FileUtils::EnsureSeparator(ProcessUtils::InitialProcessWD());
    return instance;
}

std::thread::id sMainThreadId = std::this_thread::get_id();
bool inMainThread()
{
    return sMainThreadId == std::this_thread::get_id();
}

#if _WIN32

FilePath &NormalizePathElements(FilePath &path)
{
    int i = 0;
    std::string r;
    if (path.HasRoot())
    {
        if (path.RootIsDrive())
            path.SetRoot(StringUtils::ToUpper(path.Root()));
        if (path.Kind() & FilePath::ambiguous)
            r = FilePath(FileUtils::WorkingDirectory()).Left(0) + path.NativeSeparator();
    }
    else
    {
        while (path.IsRelativeElement(i))
            ++i;
        r = FileUtils::WorkingDirectory();
    }
    while (i < path.ElementCount())
    {
        std::string s = r + path.Left(i + 1);
        WIN32_FIND_DATAW findFileData = {0};
        HANDLE h = ::FindFirstFileW(StringUtils::ToWide(s).c_str(), &findFileData);
        if (h == INVALID_HANDLE_VALUE)
            break;
        ::FindClose(h);
        path.SetElement(i, StringUtils::ToNarrow(findFileData.cFileName));
        ++i;
    }
    return path;
}

#else  // WIN32

FilePath &NormalizePathElements(FilePath &path)
{
    int i = 0;
    std::string r;
    if (path.HasRoot())
    {
        if (path.RootIsDrive())
            path.SetRoot(StringUtils::ToUpper(path.Root()));
    }
    else
    {
        while (path.IsRelativeElement(i))
            ++i;
        r = Tiny::FileUtils::WorkingDirectory();
    }
    std::string s = r + path.Left(i);
    bool done = false;
    while (!done && i < path.ElementCount() && FileUtils::IsDirectory(s))
    {
        // This is inefficient because we need to obtain the canonical path for half the
        // siblings on each level but we may get away with it on efficiently implemented
        // file systems.
        FileUtils::List names;
        FileUtils::ListDirectory(s, names);
        s = FileUtils::EnsureSeparator(s, path.NativeSeparator());
        std::string c = FileUtils::CanonicalPath(s + path.Element(i));
        done = c.empty();
        if (!done)
        {
            for (const auto &name : names)
            {
                if (FileUtils::CanonicalPath(s + name) == c)
                {
                    path.SetElement(i, name);
                    break;
                }
            }
            s = r + path.Left(++i);
        }
    }
    return path;
}
#endif // WIN32

std::string GetExecutablePath()
{
    std::string path;
#if _WIN32
    char *pFileName = NULL;
    DWORD size = 1024; // Must be large enough to hold path in WinXP.
    DWORD result = 0;
    do
    {
        delete[] pFileName;
        pFileName = new char[size];
        result = ::GetModuleFileNameA(NULL, pFileName, size);
        size *= 2;
    } while (result != 0 && ::GetLastError() == ERROR_INSUFFICIENT_BUFFER);
    if (result != 0)
        path = pFileName;
    delete[] pFileName;
#else // _WIN32
#if _GNU_SOURCE
    path = program_invocation_name;
#elif __APPLE__
    uint32_t size = 0;
    ::_NSGetExecutablePath(NULL, &size);
    char buf[size];
    buf[0] = '\0';
    ::_NSGetExecutablePath(buf, &size);
    path = buf;
#else
#error Don't know how to obtain the executable's path on this platform.
#endif
    path = FilePath(path).ToAbsolute(ProcessUtils::InitialProcessWD()).ToString();
    char* buffer = ::realpath(path.c_str(), NULL);
    if (buffer)
    {
        path = buffer;
        ::free(buffer);
    }
#endif // _WIN32
    path = FileUtils::NormalizedPath(path);
    if (path.empty())
        throw std_runtime_error << "Could not determine executable path";
    return path;
}

std::string GetInstallationDirectory()
{
    std::string installationDirectory = FileUtils::ParentDirectory(FileUtils::ExecutablePath());
#if __APPLE__ // executable may run inside a bundle
    const std::string bundlepart = ".app/Contents/MacOS/";
    if (installationDirectory.find(bundlepart) == installationDirectory.length() - bundlepart.length())
    {
        installationDirectory = installationDirectory.substr(0, installationDirectory.length() - bundlepart.length());
        installationDirectory = FileUtils::ParentDirectory(installationDirectory);
        installationDirectory = FileUtils::NormalizedPath(installationDirectory);
    }
#endif
    return FileUtils::EnsureSeparator(installationDirectory);
}

std::string GetTemporaryDirectory()
{
    std::string dir;
#if _WIN32
    int length = ::GetTempPathA(0, nullptr);
    std::vector<char> buf(length);
    if (::GetTempPathA(buf.size(), buf.data()))
        dir = FileUtils::CanonicalPath(FileUtils::ExtractDirectory(buf.data()));
#else
    char *p = ::getenv("TMPDIR");
    if (p)
        dir = FileUtils::CanonicalPath(p);
#endif
    if (dir.empty())
        throw std_runtime_error << "Could not get temporary directory";
    return dir;
};
} // namespace

namespace Tiny
{

std::string FileUtils::ExecutablePath()
{
    static std::string instance = GetExecutablePath();
    return instance;
}

std::string FileUtils::InstallationDirectory()
{
    static std::string instance = GetInstallationDirectory();
    return instance;
}

std::string FileUtils::TemporaryDirectory()
{
    static std::string instance = GetTemporaryDirectory();
    return instance;
}

std::string FileUtils::EnsureSeparator(const std::string &inDir, const std::string &inSep)
{
    if (inDir.empty() || FilePath::IsDirSeparator(inDir.back()))
        return inDir;
    if (inSep.empty())
        return inDir + FilePath::DirSeparator;
    return inDir + inSep;
}

std::string FileUtils::StripSeparator(const std::string &inDir)
{
    if (inDir.empty() || !FilePath::IsDirSeparator(inDir.back()))
        return inDir;
    if (inDir.length() == 1 && FilePath::IsDirSeparator(inDir.front()))
        return inDir;
    int i = inDir.length() - 1;
    while (i > 0 && FilePath::IsDirSeparator(inDir[i - 1]))
        --i;
    return inDir.substr(0, i);
}

std::string FileUtils::ApplicationTitle()
{
    std::string baseName = ExtractBase(ExecutablePath()), title;
    bool wasSpace = true, wasUpper = false;
    for (std::string::const_iterator i = baseName.begin(); i != baseName.end(); ++i)
    {
        if (::isspace(*i))
        {
            wasSpace = true;
            wasUpper = false;
        }
        else
        {
            if (::isupper(*i) && !wasSpace && !wasUpper)
                title += " ";
            if (::isupper(*i))
                wasUpper = true;
            else
                wasUpper = false;
            wasSpace = false;
        }
        title += *i;
    }
    return title;
}

std::string FileUtils::WorkingDirectory()
{
    if (inMainThread())
        return EnsureSeparator(ProcessUtils::ProcessWD());
    return ThreadWD();
}

bool FileUtils::ChangeDirectory(const std::string &inDir)
{
    std::string dir = RealPath(inDir);
    if (!IsDirectory(dir))
        return false;
    dir = EnsureSeparator(dir);
    if (inMainThread())
        ProcessUtils::SetProcessWD(dir);
    else
        ThreadWD() = dir;
    return true;
}

std::string FileUtils::AbsolutePath(const std::string &inPath, const std::string &inRelto)
{
    if (inPath.empty())
        return inPath;
    FilePath path(inPath);
    if (path.Kind() & FilePath::absolute)
        return inPath;
    std::string relto = inRelto.empty() ? WorkingDirectory() : inRelto;
    if (!IsAbsolutePath(relto))
        relto = AbsolutePath(relto);
    relto = EnsureSeparator(relto);
#if _WIN32
    if (path.Kind() & FilePath::ambiguous)
        return FilePath(relto).Root() + inPath;
#else
    if (path.Kind() & FilePath::ambiguous)
        return inPath;
#endif
    return relto + inPath;
}

bool FileUtils::IsAbsolutePath(const std::string &inPath)
{
    if (inPath.empty())
        return false;
    FilePath path(inPath);
    if (path.Kind() & FilePath::absolute)
        return true;
#if _WIN32
    if (path.Kind() & FilePath::ambiguous)
        return false;
#else
    if (path.Kind() & FilePath::ambiguous)
        return true;
#endif
    return false;
}

bool FileUtils::IsRelativePath(const std::string &inPath)
{
    if (inPath.empty())
        return false;
    FilePath path(inPath);
    if (path.Kind() & FilePath::absolute)
        return false;
#if _WIN32
    if (path.Kind() & FilePath::ambiguous)
        return true;
#else
    if (path.Kind() & FilePath::ambiguous)
        return false;
#endif
    return true;
}

std::string FileUtils::RelativePath(const std::string &inPath, const std::string &inRef)
{
    FilePath path(inPath);
    path.ToAbsolute(WorkingDirectory()).Simplify();
    NormalizePathElements(path);
    std::string ref = inRef;
    if (!IsDirectory(ref))
        ref = ParentDirectory(ref);
    FilePath refpath(ref);
    refpath.ToAbsolute(WorkingDirectory()).Simplify();
    NormalizePathElements(refpath);
    if (path.HasRoot() && refpath.HasRoot() && path.Root() != refpath.Root())
        return "";
    int i = 0;
    while (i < path.ElementCount() && path.Element(i) == refpath.Element(i))
        ++i;
    std::string rel;
    for (int j = i; j < refpath.ElementCount(); ++j)
        rel += ".." + refpath.NativeSeparator();
    for (int j = i; j < path.ElementCount() - 1; ++j)
        rel += path.Element(j) + path.NativeSeparator();
    if (i < path.ElementCount() && path.ElementCount() > 0)
        rel += path.Element(path.ElementCount() - 1);
    return rel;
}

std::string FileUtils::CanonicalPath(const std::string &inPath)
{
    if (inPath.empty())
        return inPath;

    FilePath path(inPath);
    path.ToAbsolute(WorkingDirectory()).Simplify();
    std::string result;
#if _WIN32
    // GetFullPathNameW() would return \\?\ when given \\?\C:\.. as input,
    // but we expect \\?\C:\
    // So let's avoid GetFullPathName() altogether, and do our own path normalization
    // before calling Get*PathName().
    std::wstring wpath;
    if (path.Kind() & FilePath::UNC)
        wpath = StringUtils::ToWide(path.ToString("\\"));
    else
        wpath = StringUtils::ToWide("\\\\?\\" + path.ToString("\\"));
    DWORD size = ::GetShortPathNameW(wpath.c_str(), 0, 0);
    if (size == 0)
    {
        wpath += L'\\';
        size = ::GetShortPathNameW(wpath.c_str(), 0, 0);
    }
    if (size > 0)
    {
        std::vector<wchar_t> buf(size + 1);
        if (::GetShortPathNameW(wpath.c_str(), buf.data(), size))
            wpath = buf.data();
        if (path.Kind() & FilePath::UNC)
          result = StringUtils::ToNarrow(wpath);
        else
          result = StringUtils::ToNarrow(wpath).substr(std::string("\\\\?\\").length());
    }
    result = StringUtils::ToUpper(result);
#elif _GNU_SOURCE
    char *pPath = ::canonicalize_file_name(path.ToString("/").c_str());
    if (pPath)
    {
        result = pPath;
        ::free(pPath);
    }
#elif __APPLE__
    char buffer[PATH_MAX + 1] = "";
    if (::realpath(path.ToString("/").c_str(), buffer))
        result = buffer;
#else
#error Don`t know how to canonicalize a path on the current target OS.
#endif
    if (IsDirectory(result))
        result = EnsureSeparator(result);
    return result;
}

std::string FileUtils::ExecutableCanonicalPath(const std::string &inExecutable)
{
    std::vector<std::string> extensions;
#if _WIN32
    std::string ext = StringUtils::ToLower(FileUtils::ExtractExtension(inExecutable));
    extensions = StringUtils::Split<std::vector<std::string>>(";", ::getenv("PATHEXT"));
    if (extensions.empty())
    {
        extensions.push_back(".cmd");
        extensions.push_back(".exe");
        extensions.push_back(".bat");
    }
#endif
    extensions.push_back("");
    if (!(FilePath(inExecutable).Kind() & FilePath::absolute))
    {
        std::string pathvar = ::getenv("PATH");
#if _WIN32
        std::string sep = ";";
#else
        std::string sep = ":";
#endif
        auto paths = StringUtils::Split<std::vector<std::string>>(sep, pathvar);
        paths.push_back("");
        for (const auto &path : paths)
        {
            auto spath = path.empty() ? path : path + "/";
            for (const auto &ext : extensions)
            {
                auto executable = CanonicalPath(spath + inExecutable + ext);
                if (!executable.empty())
                    return executable;
            }
        }
    }
    return CanonicalPath(inExecutable);
}

std::string FileUtils::RealPath(const std::string &inPath)
{
    return NormalizedPath(CanonicalPath(inPath));
}

std::string FileUtils::NormalizedPath(const std::string &inPath)
{
    if (inPath.empty())
        return inPath;

    bool haveSeparator = FilePath::IsDirSeparator(inPath.back());
    FilePath path(inPath);
    path.Simplify();
    NormalizePathElements(path);
    std::string s = path.ToString("/");
    if (haveSeparator)
        s = EnsureSeparator(s, "/");
    return s;
}

std::string FileUtils::ParentDirectory(const std::string &inPath)
{
    if (IsFile(inPath))
        return FileUtils::ExtractDirectory(inPath);
    FilePath path = inPath + "/..";
    path.Simplify();
    return EnsureSeparator(path.ToString());
}

std::string FileUtils::ExtractDirectory(const std::string &inPath)
{
    if (inPath.empty())
        return inPath;
    size_t pos = inPath.find_last_of(FilePath::SeparatorSet());
    if (pos == std::string::npos)
        return "";
    return inPath.substr(0, pos + 1);
}

std::string FileUtils::ExtractName(const std::string &inPath)
{
    size_t pos = inPath.find_last_of(FilePath::SeparatorSet());
    if (pos == std::string::npos)
        return inPath;
    return inPath.substr(pos + 1);
}

std::string FileUtils::ExtractBase(const std::string &inPath)
{
    std::string file = ExtractFile(inPath);
    size_t pos = file.find_last_of(".");
    if (pos == std::string::npos)
        return file;
    return file.substr(0, pos);
}

std::string FileUtils::ExtractExtension(const std::string &inPath)
{
    std::string file = ExtractFile(inPath);
    size_t pos = file.find_last_of(".");
    if (pos == std::string::npos)
        return "";
    return file.substr(pos);
}

bool FileUtils::IsFile(const std::string &inPath)
{
    struct stat s;
    return !::stat(AbsolutePath(inPath).c_str(), &s) && (S_ISREG(s.st_mode));
}

bool FileUtils::IsDirectory(const std::string &inPath)
{
    struct stat s;
    std::string abs = StripSeparator(AbsolutePath(inPath));
    bool exists = !::stat(abs.c_str(), &s) || !::stat(EnsureSeparator(abs).c_str(), &s);
    return exists && (S_ISDIR(s.st_mode));
}

bool FileUtils::IsSymbolicLink(const std::string &inPath)
{
    struct stat s;
    std::string abs = StripSeparator(AbsolutePath(inPath));
    return !::stat(abs.c_str(), &s) && (S_ISLNK(s.st_mode));
}

bool FileUtils::Exists(const std::string &inPath)
{
    struct stat s;
    std::string abs = StripSeparator(AbsolutePath(inPath));
    return !::stat(abs.c_str(), &s) || !::stat(EnsureSeparator(abs).c_str(), &s);
}

bool FileUtils::IsWritableFile(const std::string &inPath)
{
    std::string abs = AbsolutePath(inPath);
    return IsFile(abs) && std::ofstream(abs, std::ios::out | std::ios::app).is_open();
}

static LCRandomGenerator& RandomGenerator()
{
    static LCRandomGenerator instance;
    return instance;
}

bool FileUtils::IsWritableDirectory(const std::string &inPath)
{
    if (!IsDirectory(inPath))
        return false;
    std::string testfile = EnsureSeparator(AbsolutePath(inPath)) + "." + ExtractFile(ExecutablePath()) + ".writetest";
    while (Exists(testfile))
    {
        testfile += RandomGenerator().RandomCharacter();
    }
    if (!std::ofstream(testfile, std::ios::out | std::ios::app).is_open())
        return false;
    RemoveFile(testfile);
    return true;
}

bool FileUtils::IsWritableLocation(const std::string &inPath)
{
    if (IsFile(inPath))
        return IsWritableFile(inPath);
    std::string dir = ParentDirectory(inPath);
    while (!Exists(dir) && dir != ParentDirectory(dir))
        dir = ParentDirectory(dir);
    if (IsFile(dir))
        return false;
    return IsWritableDirectory(dir);
}

Time FileUtils::ModificationTime(const std::string &inPath)
{
    struct stat s;
    std::string abs = AbsolutePath(inPath);
    if (!::stat(abs.c_str(), &s))
        return Time::FromUnixTime(s.st_mtime);
    return Time::Never();
}

int64_t FileUtils::Length(const std::string &inPath)
{
    struct stat s;
    std::string abs = AbsolutePath(inPath);
    if (!::stat(abs.c_str(), &s))
        return s.st_size;
    return -1;
}

bool FileUtils::MakeDirectory(const std::string &inName, bool inForce)
{
    bool result = false;
    std::string abs = AbsolutePath(inName);
#ifdef _WIN32
    result = !::mkdir(abs.c_str());
#else
    const int rwxr_xr_x = 0755;
    result = !::mkdir(abs.c_str(), rwxr_xr_x);
#endif
    if (inForce)
        result = result || IsDirectory(abs);
    if (!result && inForce)
    {
        auto parent = FilePath(ParentDirectory(abs)).Simplify().ToString();
        if (parent != abs)
            result = MakeDirectory(parent, true) && MakeDirectory(abs, false);
    }
    return result;
}

bool FileUtils::RemoveDirectory(const std::string &inName, bool inForce)
{
    bool success = true;
    std::string name = CanonicalPath(inName);
    if (inForce)
    {
        List entries;
        success = ListDirectory(name, entries);
        for (size_t i = 0; success && i < entries.size(); ++i)
        {
            std::string path = name + FilePath::DirSeparator + entries[i];
            if (IsFile(path) || IsSymbolicLink(path))
                success = RemoveFile(path);
            else if (IsDirectory(path))
                success = RemoveDirectory(path, true);
        }
    }
    if (success)
        success = !::rmdir(name.c_str());
    return success;
}

bool FileUtils::Rename(const std::string &inName, const std::string &inNewName)
{
    std::string abs1 = AbsolutePath(inName), abs2 = AbsolutePath(inNewName);
    return !::rename(abs1.c_str(), abs2.c_str());
}

bool FileUtils::RemoveFile(const std::string &inName)
{
    return inName.empty() || !::unlink(AbsolutePath(inName).c_str());
}

bool FileUtils::Copy(const std::string &inSource, const std::string &inDest)
{
    auto cSource = CanonicalPath(inSource), cDest = CanonicalPath(inDest);
    if (cSource.empty())
        return false;
    if (cSource == cDest)
        return true;
    std::ifstream source(cSource, std::ios::binary | std::ios::in);
    if (!source.is_open())
        return false;
    auto aDest = AbsolutePath(inDest);
    std::ofstream dest(aDest, std::ios::binary | std::ios::out | std::ios::trunc);
    if (dest.is_open())
    {
        if (FileUtils::Length(cSource) == 0)
            return true;
        return !!(dest << source.rdbuf()); // will return false for empty file
    }
    // If destination could not be opened for writing, it might be due to the file being open,
    // so we test whether files are the same.
    std::ifstream idest(aDest, std::ios::binary | std::ios::in | std::ios::ate);
    if (!idest.is_open())
        return false;
    source.seekg(0, std::ios::end);
    if (source.tellg() != idest.tellg())
        return false;
    source.seekg(0, std::ios::beg);
    idest.seekg(0, std::ios::beg);
    return std::equal(std::istreambuf_iterator<char>(source.rdbuf()), std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(idest.rdbuf()));
}

bool FileUtils::ListDirectory(const std::string &inPath, List &outList)
{
    bool success = true;
    outList.clear();
    std::string cpath = CanonicalPath(inPath);
    success = !cpath.empty();
    DIR* dir = nullptr;
    if (success)
        dir = ::opendir(cpath.c_str());
    success = (dir != nullptr);
    if (success)
    {
        struct dirent *entry;
        while (NULL != (entry = ::readdir(dir)))
        {
            std::string name = entry->d_name;
            if (name != "." && name != "..")
                outList.push_back(name);
        }
        ::closedir(dir);
    }
    return success;
}

std::string FileUtils::MakeUniqueDirectory(const std::string &inName)
{
    if (!IsWritableDirectory(ParentDirectory(inName)))
        return "";
    std::string name = inName;
    int i = 0;
    while (!FileUtils::MakeDirectory(name))
    {
        std::ostringstream oss;
        oss << "." << ++i;
        name = inName + oss.str();
    }
    return name;
}

FileUtils::TemporaryFile::TemporaryFile(const std::string &inExt) : mpFile(new File)
{
    for (int trial = 0; trial < 20 && !is_open(); ++trial)
    {
        mpFile->name = GenerateName() + inExt;
        open(mpFile->name, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
    }
    if (!is_open())
        throw std_runtime_error << "Could not create temporary file: " << mpFile->name;
}

namespace
{
struct TempPrefix : std::string
{
    TempPrefix() : std::string(FileUtils::ExtractBase(FileUtils::ExecutablePath()) + "~")
    {
    }
};
} // namespace

std::string FileUtils::TemporaryFile::GenerateName()
{
    static TempPrefix prefix;
    return TemporaryDirectory() + prefix + RandomGenerator().RandomName(6);
}

bool FileUtils::TemporaryFile::Open()
{
    open(mpFile->name, std::ios::in | std::ios::out | std::ios::binary);
    return is_open();
}

std::string FileUtils::SearchURL(const std::string &inSearchTerms)
{
#ifndef WEBSEARCH_DOMAIN
    return "";
#else
    std::string url = "http://" WEBSEARCH_DOMAIN "/search?q=", searchTerms = inSearchTerms;
    if (searchTerms.empty())
        searchTerms = ApplicationTitle();
    std::string site;
#ifdef PROJECT_DOMAIN
    if (!searchTerms.empty())
        site = PROJECT_DOMAIN;
#endif
#ifdef PROJECT_NAME
    if (searchTerms.empty())
        searchTerms = PROJECT_NAME;
#endif
    if (searchTerms.empty())
        return "";
    if (!site.empty())
        searchTerms = "site%3a" + site + " " + searchTerms;

    std::istringstream iss(searchTerms);
    std::string term;
    iss >> std::ws >> term;
    url += term;
    while (iss >> std::ws >> term)
        url += "+" + term;
    return url;
#endif // WEBSEARCH_DOMAIN
}

} // namespace Tiny
