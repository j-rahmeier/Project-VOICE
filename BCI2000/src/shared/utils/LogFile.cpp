////////////////////////////////////////////////////////////////////////////////
// $Id: LogFile.cpp 8009 2024-04-09 16:26:37Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A std::ofstream descendant that centralizes/encapsulates details
//   of a log file.
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
#include "LogFile.h"

#include "BCIStream.h"
#include "FileUtils.h"
#include "RunManager.h"

#include <ctime>

void LogFile::Preflight() const
{
    std::string name = RunManager()->CurrentSession() + mExtension;
    if (!FileUtils::IsWritableLocation(name))
        bcierr << "Cannot write to log file at \"" << name << "\"";
}

void LogFile::Initialize()
{
    OpenFile("@Initialize");
}

void LogFile::StartRun()
{
    OpenFile("@StartRun");
}

void LogFile::OpenFile(const std::string& s)
{
    close();
    clear();
    std::string name = RunManager()->CurrentSession() + mExtension;
    FileUtils::MakeDirectory(FileUtils::ExtractDirectory(name), true);
    open(name, std::ios::out | std::ios::app);
    if (!is_open())
        bcierr << "Cannot write to log file at \"" << name
               << "\" although this appeared to be a writable location during preflight phase";
    time_t now = ::time(NULL);
    *this << FileUtils::ExtractFile(name) << s << '\n' << ::asctime(::localtime(&now)) << std::endl;
}
