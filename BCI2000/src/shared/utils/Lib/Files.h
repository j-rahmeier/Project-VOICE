////////////////////////////////////////////////////////////////////////////////
// $Id: Files.h 8120 2024-05-08 16:37:53Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A wrapper for various kind of local files.
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
#ifndef TINY_FILES_H
#define TINY_FILES_H

#include "FdObject.h"
#include "SockStream.h"

namespace Tiny
{

class File : public FdObject, private fdio::fd_object
{
  public:
    enum
    {
        append = std::ios::app,
        in = std::ios::in,
        out = std::ios::out,

        // Create file with "exclusive" creation access, i.e. fail if file exists.
        exclusive = 1 << 16,

        // For consistency with unix systems, unlink() should succeed while file
        // is still open, and subsequently file should be deleted when last handle
        // closed. On Win32, this behavior requires FILE_SHARE_DELETE to be specified
        // when opening a file.
        // In certain situations, the default Win32 behavior is actually desired, so
        // you may specify the "nodelete" flag. Note it has no effect on POSIX systems.
        nodelete = 1 << 17,
    };
    File();
    ~File();
    bool Open(const std::string &, int = in | out);
    File &AttachToFd(int fd);
    int64_t Length();

  protected:
    const char *OnDescribeIOState() const override;

  private:
    struct Private;
    Private *p;
};

} // namespace Tiny

using Tiny::File;

#endif // TINY_FILES_H
