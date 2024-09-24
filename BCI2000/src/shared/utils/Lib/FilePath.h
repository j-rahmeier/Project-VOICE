//////////////////////////////////////////////////////////////////////
// $Id: FilePath.h 7563 2023-09-01 12:40:45Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A class for parsing file paths.
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
#ifndef TINY_FILE_PATH_H
#define TINY_FILE_PATH_H

#include <string>
#include <vector>

namespace Tiny
{

namespace StringUtils {
class String;
}

class FilePath
{
  public:
#if _WIN32
    static const char DirSeparator = '\\';
    static const char DriveSeparator = ':';
    static const char PathSeparator = ';';
    inline static const char *SeparatorSet()
    {
        return "\\/";
    }
#else
    static const char DirSeparator = '/';
    static const char DriveSeparator = '/';
    static const char PathSeparator = ':';
    inline static const char *SeparatorSet()
    {
        return "/";
    }
#endif

    static bool IsDirSeparator(char);
    static bool IsDriveSeparator(char);

    FilePath(const std::string &);
    FilePath(const StringUtils::String &);
    FilePath& operator=(const std::string &);
    FilePath& operator=(const StringUtils::String &);

    FilePath(const FilePath&) = delete;
    
    FilePath &ToAbsolute(const std::string &wd);
    // Remove relative elements.
    FilePath &Simplify();

    enum
    {
        unknown = 0,
        relative = 1,
        absolute = 2,
        ambiguous = 4,
        UNC = 16,
    };
    int Kind() const
    {
        return mKind;
    }

    bool HasRoot() const
    {
        return mRootLength >= 0;
    }
    bool RootIsDrive() const;
    std::string Root() const;
    FilePath &SetRoot(const std::string &);

    // Use negative indices to address elements from the end of the path.
    int ElementCount() const;
    bool IsRelativeElement(int) const;
    std::string Element(int) const;
    FilePath &SetElement(int, const std::string &);

    const std::string &NativeSeparator() const;
    std::string ToString(const std::string &sep = "") const;
    std::string Left(int elementCount, const std::string &sep = "") const;
    std::string Truncated(int inTruncateTo = 70, const std::string &inEllipsis = "...",
                          const std::string &sep = "") const;

  private:
    void IdentifyRoot();
    void IdentifyElements() const;
    void EnsureParsed() const;

    mutable bool mParsed;
    int mKind, mRootLength;
    std::string mData;
    mutable std::string mNativeSeparator;
    mutable std::vector<std::string> mElements;
};

} // namespace Tiny

using Tiny::FilePath;

#endif // TINY_FILE_PATH_H
