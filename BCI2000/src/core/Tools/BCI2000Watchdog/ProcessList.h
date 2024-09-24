//////////////////////////////////////////////////////////////////////
// $Id$
// Authors: mellinger@neurotechcenter.org
// Description: A class for watching a BCI2000 instance.
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
#ifndef PROCESS_LIST_H
#define PROCESS_LIST_H

#include <string>
#include <vector>

namespace ProcessList {

struct ProcessData
{
    unsigned int pid;
    std::string name;
#if _WIN32
    // On Windows, we keep an open process handle because a Windows
    // PID is not unique (it may be reused after process termination).
    void* handle;
#else
    unsigned int ppid, pgid;
#endif
};

// Find all child, grandchild, etc processes of the given process.
// If pid is 0, use the pid of the current process.
std::vector<ProcessData> FindChildProcessesRecursively(unsigned int pid = 0);

// Terminate the process represented by the argument.
// On POSIX systems, use SIGKILL as the termination signal.
// Return true if the termination call was successful.
bool KillProcess(ProcessData&);

void CleanupProcessData(ProcessData&);

} // namespace ProcessList

#endif // PROCESS_LIST_H
