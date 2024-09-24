////////////////////////////////////////////////////////////////////
// $Id: copy_if_newer.cpp 7462 2023-06-30 14:27:11Z mellinger $
// Author:      juergen.mellinger@uni-tuebingen.de
// Description: Copy a file if it is newer than, or missing from,
//              the destination.
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
////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>

#include "FileUtils.h"

std::string usage = "Usage: copy_if_newer <file> <destination>";

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << usage << std::endl;
        return -1;
    }
    std::string origin = argv[1], dest = argv[2];
    if (FileUtils::IsDirectory(origin))
    {
        std::cerr << "Origin \"" << origin << "\" is a directory, cannot handle directories" << std::endl;
        return -1;
    }
    if (!FileUtils::IsFile(origin))
    {
        std::cerr << "Origin \"" << origin << "\" does not exist" << std::endl;
        return -1;
    }
    if (FileUtils::IsDirectory(dest))
        dest = FileUtils::EnsureSeparator(dest) + FileUtils::ExtractFile(origin);
    bool doCopy = true;
    if (FileUtils::IsFile(dest))
    {
        doCopy = FileUtils::ModificationTime(dest) < FileUtils::ModificationTime(origin);
        if (doCopy)
            std::clog << "\"" << origin << "\" is newer than\n\t\"" << dest << "\", copying" << std::endl;
    }
    else
    {
        std::clog << "\"" << dest << "\" does not exist,\n\tcopying from \"" << origin << "\"" << std::endl;
    }

    if (doCopy && !FileUtils::Copy(origin, dest))
    {
        std::cerr << "Could not copy \"" << origin << "\"\n\tto \"" << dest << "\"" << std::endl;
        return -1;
    }
    return 0;
}
