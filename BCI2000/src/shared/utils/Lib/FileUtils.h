//////////////////////////////////////////////////////////////////////
// $Id: FileUtils.h 7443 2023-06-23 14:45:52Z mellinger $
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
#ifndef TINY_FILE_UTILS_H
#define TINY_FILE_UTILS_H

#include "TimeUtils.h"
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#undef RemoveDirectory

namespace Tiny
{

namespace FileUtils
{
std::string EnsureSeparator(const std::string &, const std::string & = "");
std::string StripSeparator(const std::string &);
// The current executable's full path.
std::string ExecutablePath();
// A pretty-printed version of the current executable's file name.
std::string ApplicationTitle();

// The current executable's parent directory.
std::string InstallationDirectory();
// The current executable's parent directory, without separator.
inline std::string InstallationDirectoryS()
{
    return StripSeparator(InstallationDirectory());
}

// FileUtils::WorkingDirectory is a per-thread variable used to transform relative
// into absolute paths, but is not the actual process working directory, and is not
// applied outside the Tiny library.
// Inside the main thread, however, it is identical to the process working directory.
std::string WorkingDirectory();
// The working directory, without trailing separator.
inline std::string WorkingDirectoryS()
{
    return StripSeparator(WorkingDirectory());
}

// This sets the current working directory. Inside the main thread, it also sets the process
// working directory.
bool ChangeDirectory(const std::string &);

// This transforms relative paths into absolute paths, using the
// second argument (or the per-thread working directory by default) as a reference.
std::string AbsolutePath(const std::string &, const std::string& = "");

// This returns true if a path is an absolute path.
// On Windows, paths are treated as absolute only when beginning with a drive letter.
bool IsAbsolutePath(const std::string &);

// This transforms absolute paths into relative paths, relative
// to the second argument. If any of the arguments is a relative path,
// it is resolved with respect to the per-thread working directory.
std::string RelativePath(const std::string &path, const std::string &ref);

// This returns true if a path is a relative path.
// On Windows, this is the case even if a path begins with a slash but contains
// no drive letter.
bool IsRelativePath(const std::string &);

// Normalize a path by moving any relative elements to the beginning, and by
// using "/" as a directory separator.
// Will return empty string for empty input only, "./" or "." otherwise.
// With case-insensitive but case-preserving file systems, case is adapted
// to be consistent with directory listing.
// On Windows, short file names are replaced with long ones.
// Determining the normalized form of a path may be expensive to compute.
std::string NormalizedPath(const std::string &);

// This returns a path free of relative elements, without symlinks, and suited to uniquely identify objects,
// as defined by the operating system.
// Result will be empty if the path does not point to an existing object.
// Result will contain a trailing directory separator if, and only if, it is a directory.
std::string CanonicalPath(const std::string &);

// Canonical path to an executable, using the "path" environment variable to resolve.
std::string ExecutableCanonicalPath(const std::string &);

// POSIX realpath() workalike: Normalized form of canonical path.
std::string RealPath(const std::string &);

// This returns the parent of the specified object (file or directory).
std::string ParentDirectory(const std::string &);
// Parent directory without trailing separator.
inline std::string ParentDirectoryS(const std::string &s)
{
    return StripSeparator(ParentDirectory(s));
}

// A list of entries in the specified directory.
typedef std::vector<std::string> List;
bool ListDirectory(const std::string &, List &);

// These return the directory portion, file name, base name portion, and extension of a path.
std::string ExtractDirectory(const std::string &);
inline std::string ExtractDirectoryS(const std::string &s)
{
    return StripSeparator(ExtractDirectory(s));
}
// Extract file name including extension, but excluding directory.
std::string ExtractName(const std::string &);
// Extract file name including extension, but excluding directory.
inline std::string ExtractFile(const std::string &s)
{
    return ExtractName(s);
}
// Extract file name without directory, and without extension.
// If there is more than one dot in the file name, only the rightmost extension is stripped.
std::string ExtractBase(const std::string &);
// Extract file extension.
// If there is more than one dot in the file name, only the rightmost extension is extracted.
std::string ExtractExtension(const std::string &);
// Extract file name including extension, but excluding directory.
inline std::string StripDirectory(const std::string &s)
{
    return ExtractFile(s);
}
// Extract file with directory, but without extension.
// If there is more than one dot in the file name, only the rightmost extension is stripped.
inline std::string StripExtension(const std::string &s)
{
    return ExtractDirectory(s) + ExtractBase(s);
}

// Whether a file name points to an existing normal file.
bool IsFile(const std::string &);
// Whether a file name points to a directory.
bool IsDirectory(const std::string &);
// Whether a file name points to a symlink.
bool IsSymbolicLink(const std::string &);
// Whether a file name points to an existing object (any of the above).
bool Exists(const std::string &);

// Whether a file name points to an existing writable file.
bool IsWritableFile(const std::string &);
// Whether a file name points to an existing writable directory.
bool IsWritableDirectory(const std::string &);
// Whether a file name points to an existing writable file or directory.
bool IsWritableLocation(const std::string &);

// A file's modification time.
Time ModificationTime(const std::string &);
// A file's length, in bytes.
int64_t Length(const std::string &);

// Rename an existing object.
bool Rename(const std::string &, const std::string &);
// Create a directory. If the "force" flag is given, create intermediate directories as well.
bool MakeDirectory(const std::string &, bool force = false);
// Remove a directory. If the "force" flag is given, recursively remove contents as well.
bool RemoveDirectory(const std::string &, bool force = false);
// Remove an existing file.
bool RemoveFile(const std::string &);
// Copy a file to another location.
bool Copy(const std::string & source, const std::string & destination);

// Create a uniquely named directory. If a directory with the given name exists, append
// a number to make it unique.
std::string MakeUniqueDirectory(const std::string &);

// Path to the user's temporary directory.
std::string TemporaryDirectory();

// A temporary file that is automatically deleted when going out of scope.
class TemporaryFile : public std::fstream
{
  public:
    TemporaryFile(const std::string &pattern = "");
    ~TemporaryFile()
    {
        Close();
    }
    const std::string &Name() const
    {
        return mpFile->name;
    }
    bool Open();
    void Close()
    {
        std::fstream::close();
    }
    bool IsOpen() const
    {
        return std::fstream::is_open();
    }
    bool Flush()
    {
        Close();
        return Open();
    }
    bool CopyTo(const std::string &s) const
    {
        return Copy(Name(), s);
    }

    static std::string GenerateName();

  private:
    struct File
    {
        ~File()
        {
            RemoveFile(name);
        }
        std::string name;
    };
    std::shared_ptr<File> mpFile;
};

// Construct a search URL from a list of search terms.
std::string SearchURL(const std::string &searchTerms = "");

} // namespace FileUtils

} // namespace Tiny

namespace FileUtils = Tiny::FileUtils;

#endif // TINY_FILE_UTILS_H
