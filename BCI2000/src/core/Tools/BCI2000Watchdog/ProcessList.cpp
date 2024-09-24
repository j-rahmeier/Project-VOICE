//////////////////////////////////////////////////////////////////////
// $Id$
// Authors: mellinger@neurotechcenter.org
// Description: A helper function to list all subprocesses related to
//   a given process.
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
#include "ProcessList.h"

#if _WIN32
#include <windows.h>
#include <TlHelp32.h>
#include "StringUtils.h"
#elif __APPLE__
#include <unistd.h>
#include <signal.h>
#include <libproc.h>
#include <set>
#elif __linux__
#include <signal.h>
#include <regex>
#include <fstream>
#include "FileUtils.h"
#endif

namespace ProcessList {

#if __APPLE__
ProcessData GetProcessData(unsigned int pid)
{
    ProcessData data = {0};
    data.pid = pid;
    struct proc_bsdshortinfo info;
    int size = ::proc_pidinfo(pid, PROC_PIDT_SHORTBSDINFO, 0,
                              &info, PROC_PIDT_SHORTBSDINFO_SIZE);
    if (size == PROC_PIDT_SHORTBSDINFO_SIZE) {
        data.ppid = info.pbsi_ppid;
        data.pgid = info.pbsi_pgid;
    }
    char buf [PROC_PIDPATHINFO_MAXSIZE];
    ::proc_pidpath(pid, buf, sizeof(buf));
    int pos = ::strlen(buf);
    while (pos >= 0 && buf[pos] != '/')
        --pos;
    data.name = buf + pos + 1;
    
    return data;
}
#endif // __APPLE__

#if __linux__
// Read relevant process information from a /proc subdirectory.
ProcessData ReadProcessData(const std::string& procdir)
{
    ProcessData data = {0};
    // read from stat file
    std::ifstream stream(procdir + "/stat");
    stream >> data.pid;
    stream >> std::ws;
    stream.ignore(); // '(' character
    std::getline(stream, data.name, ')');
    std::string ignore; // process state
    stream >> ignore;
    stream >> data.ppid >> data.pgid;
    
    return data;
}
#endif // __linux__


std::vector<ProcessData> FindChildProcessesRecursively(unsigned int pid)
{
    std::vector<ProcessData> childProcessIDs;
#if _WIN32
    if (pid == 0)
        pid = ::GetProcessId(::GetCurrentProcess());
    // Compare process creation time of parent and child to avoid falling prey to reused
    // PIDs. Parent's creation time must be before child's creation time.
    HANDLE hParent = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!hParent)
        return childProcessIDs;
    FILETIME parentCreationTime, exitTime, kernelTime, userTime;
    bool ok = ::GetProcessTimes(hParent, &parentCreationTime, &exitTime, &kernelTime, &userTime);
    ::CloseHandle(hParent);
    if (!ok)
        return childProcessIDs;
    
    HANDLE h = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32W pe = { 0 };
    pe.dwSize = sizeof(PROCESSENTRY32W);
    
    if (Process32FirstW(h, &pe)) {
        do {
            if (pe.th32ParentProcessID == pid) {
                HANDLE hChild = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pe.th32ProcessID);
                if (!hChild)
                    continue;
                FILETIME childCreationTime;
                bool ok = ::GetProcessTimes(hChild, &childCreationTime, &exitTime, &kernelTime, &userTime);
                if (!ok)
                    continue;
                ::CloseHandle(hChild);
                if (::CompareFileTime(&parentCreationTime, &childCreationTime) < 0) {
                    HANDLE hChildTermination = ::OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
                    std::string name = StringUtils::FromWide(pe.szExeFile);
                    ProcessData procdata = { pe.th32ProcessID, name, hChildTermination};
                    childProcessIDs.push_back(procdata);
                    auto grandchildProcessIDs = FindChildProcessesRecursively(pe.th32ProcessID);
                    childProcessIDs.insert(childProcessIDs.end(), grandchildProcessIDs.begin(), grandchildProcessIDs.end());
                }
            }
        } while (Process32NextW(h, &pe));
    }
    ::CloseHandle(h);
#elif __APPLE__
    if (pid == 0)
        pid = ::getpid();
    int count = ::proc_listpids(PROC_ALL_PIDS, 0, nullptr, 0);
    // There is a race condition here which is why we initialize
    // processes to 0.
    std::vector<pid_t> processes(count, 0);
    ::proc_listpids(PROC_ALL_PIDS, 0, processes.data(), processes.size());
    for (auto procpid : processes) {
        if (procpid > 0) {
            ProcessData procdata = GetProcessData(procpid);
            if ((procdata.ppid == pid || procdata.pgid == pid) && procdata.pid != pid) {
                childProcessIDs.push_back(procdata);
                auto grandchildProcessIDs = FindChildProcessesRecursively(procdata.pid);
                childProcessIDs.insert(childProcessIDs.end(), grandchildProcessIDs.begin(), grandchildProcessIDs.end());
            }
        }
    }
    // Remove any duplicates
    struct ProcessDataCmp
    {
        bool operator()(const ProcessData& a, const ProcessData& b) const
        {
            return a.pid < b.pid;
        }
    };
    std::set<ProcessData, ProcessDataCmp> uniqueProcesses;
    for (const auto& process : childProcessIDs)
        uniqueProcesses.insert(process);
    childProcessIDs.clear();
    childProcessIDs.reserve(uniqueProcesses.size());
    for (const auto& process : uniqueProcesses)
        childProcessIDs.push_back(process);
#elif __linux__
    if (pid == 0)
        pid = ::getpid();
    // Read numeric entries in the /proc filesystem.
    FileUtils::List subdirs;
    FileUtils::ListDirectory("/proc", subdirs);
    std::regex isNumRegex("[0-9]+");
    for (const auto& subdir : subdirs) {
        if (std::regex_match(subdir, isNumRegex)) {
            ProcessData procdata = ReadProcessData("/proc/" + subdir);
            if ((procdata.ppid == pid || procdata.pgid == pid) && procdata.pid != pid) {
                childProcessIDs.push_back(procdata);
                auto grandchildProcessIDs = FindChildProcessesRecursively(procdata.pid);
                childProcessIDs.insert(childProcessIDs.end(), grandchildProcessIDs.begin(), grandchildProcessIDs.end());
            }
        }
    }
#endif
    return childProcessIDs;
}

bool KillProcess(ProcessData& proc)
{
    bool ok = false;
#if _WIN32
    if (proc.handle) {
        ok = ::TerminateProcess(proc.handle, 0);
        ::CloseHandle(proc.handle);
        proc.handle = nullptr;
    }
#else
    if (::kill(proc.pid, SIGKILL) == 0)
        ok = true;
#endif
    return ok;
}

void CleanupProcessData(ProcessData& proc)
{
#if _WIN32
    if (proc.handle) {
        ::CloseHandle(proc.handle);
        proc.handle = nullptr;
    }
#endif
}

} // namespace ProcessList
