////////////////////////////////////////////////////////////////////////////////
// $Id: ChildProcess.cpp 7644 2023-10-05 17:36:22Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A class that represents an external process with I/O redirected.
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
#include <iostream>
#include <string>

#if _WIN32
#include "SelfPipe.h"
#include <Windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#endif // _WIN32

#include "ChildProcess.h"
#include "FdObject.h"
#include "FileUtils.h"
#include "ProcessUtils.h"
#include "ThreadUtils.h"
#include "Runnable.h"
#include "Streambuf.h"
#include "Thread.h"
#include "SynchronizedObject.h"

namespace
{
#if _WIN32

class ProcThreadAttributeList
{
  public:
    explicit ProcThreadAttributeList(int length)
    {
        SIZE_T requiredSize = 0;
        ::InitializeProcThreadAttributeList(nullptr, length, 0, &requiredSize);
        mpData.b = new char[requiredSize];
        if (!::InitializeProcThreadAttributeList(mpData.t, length, 0, &requiredSize))
            throw std_runtime_error << SysError().Message();
    }
    ~ProcThreadAttributeList()
    {
        ::DeleteProcThreadAttributeList(mpData.t);
        delete[] mpData.b;
    }

    ProcThreadAttributeList(const ProcThreadAttributeList&) = delete;
    ProcThreadAttributeList& operator=(const ProcThreadAttributeList&) = delete;

    void SetAttribute(DWORD attribute, void* data, DWORD size)
    {
        if (!::UpdateProcThreadAttribute(mpData.t, 0, attribute, data, size, nullptr, nullptr))
            throw std_runtime_error << SysError().Message();
    }

    const LPPROC_THREAD_ATTRIBUTE_LIST Ptr() const
    {
        return mpData.t;
    }

  private:
    union { char* b; LPPROC_THREAD_ATTRIBUTE_LIST t; } mpData = { nullptr };
};

bool CreatePipeInheritable(HANDLE *outpRead, bool readInherit, HANDLE *outpWrite, bool writeInherit)
{
    BOOL result = TRUE;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = 0;

    HANDLE hReadTmp, hWriteTmp;
    result = ::CreatePipe(&hReadTmp, &hWriteTmp, &sa, 0);
    result = result && ::DuplicateHandle(::GetCurrentProcess(), hWriteTmp, ::GetCurrentProcess(), outpWrite, 0,
                                         writeInherit, DUPLICATE_SAME_ACCESS);
    result = result && ::DuplicateHandle(::GetCurrentProcess(), hReadTmp, ::GetCurrentProcess(), outpRead, 0,
                                         readInherit, DUPLICATE_SAME_ACCESS);
    ::CloseHandle(hWriteTmp);
    ::CloseHandle(hReadTmp);
    return result;
}

// translate a Win32 file handle into a waitable socket
class RedirFdObj : public Tiny::FdObject, Runnable
{
    static const int bufsize = 512;

  public:
    RedirFdObj() : mhFile(INVALID_HANDLE_VALUE), mPipe(bufsize), mThread(this, "ChildProcess IO Redirection")
    {
        FdObject::Open(&mPipe);
    }
    ~RedirFdObj()
    {
        FdObject::Close();
    }
    enum
    {
        read,
        write
    };
    bool Attach(HANDLE hFile, int dir)
    {
        if (mThread.Running())
            return false;
        mhFile = hFile;
        mDirection = dir;
        mThread.Start();
        return mThread.Running();
    }
    void Detach()
    {
        if (mDirection == read)
            ::CancelIo(mhFile);
        mThread.Terminate();
    }

  private:
    int64_t OnAvailable() override
    { // fdio::selfpipe should inherit from sockio::socket but currently doesn't
        u_long count = 0;
        ::ioctlsocket(mPipe.fd(), FIONREAD, &count);
        if (count < 1 && mPipe.can_read())
            count = 1;
        return count;
    }
    void OnRun() override
    {
        switch (mDirection)
        {
        case read:
            doRead();
            break;
        case write:
            doWrite();
            break;
        }
        SyncMemory<>();
        mhFile = INVALID_HANDLE_VALUE;
    }
    void doRead()
    {
        char buffer[bufsize];
        DWORD bytesToWrite = 0;
        while (::ReadFile(mhFile, buffer, sizeof(buffer), &bytesToWrite, 0) && bytesToWrite > 0)
        {
            const char *p = buffer;
            int bytesWritten = 0;
            while (bytesToWrite > 0 && (bytesWritten = mPipe.write(p, bytesToWrite)) > 0)
            {
                p += bytesWritten;
                bytesToWrite -= bytesWritten;
            }
        }
    }
    void doWrite()
    {
        while (Input().Wait())
        {
            char buffer[bufsize];
            int bytesToWrite = mPipe.read(buffer, sizeof(buffer));
            const char *p = buffer;
            DWORD bytesWritten = 0;
            while (bytesToWrite > 0 && ::WriteFile(mhFile, p, bytesToWrite, &bytesWritten, 0))
            {
                p += bytesWritten;
                bytesToWrite -= bytesWritten;
            }
        }
    }

    HANDLE mhFile;
    int mDirection;
    Thread mThread;
    fdio::selfpipe mPipe;
};
#else  // _WIN32
// just a wrapper for an existing fd
class RedirFdObj : public Tiny::FdObject
{
  public:
    ~RedirFdObj()
    {
        FdObject::Close();
    }
    bool Attach(fdio::fd_type fd)
    {
        FdObject::Close();
        mFd = fdio::fd_object(fd);
        FdObject::Open(&mFd);
        return true;
    }

  private:
    fdio::fd_object mFd;
};
#endif // _WIN32
} // namespace

namespace Tiny
{

struct ChildProcess::Private : Runnable
{
    Private(ChildProcess *p, const std::string &executable, const std::string &arguments)
        : mExecutable(executable), mArguments(arguments), mResult(0), mPid(0),
          mThread(this, "External Process Execution Thread")
    {
        p->AssociateWith(mThread);
        mStandardIO.SetInput(&mStdout.Input()).SetOutput(&mStdin.Output());
        mStandardError.SetInput(&mStderr.Input()).SetOutput(nullptr);
    }
    ~Private() override
    {
        ProcessUtils::Pid pid = mPid;
        ProcessUtils::TerminateProcess(pid);
    }
    void OnRun() override
    {
        mResult = -1;
        mRunning = false;
        std::string executable = FileUtils::ExecutableCanonicalPath(mExecutable);
        if (executable.empty())
            return;

#if _WIN32
        // Make sure the process appears in tasklist with its long name.
        executable = FileUtils::NormalizedPath(executable);
        // CreateProcessA() does not accept forward slashes.
        for (auto& c : executable)
          if (c == '/')
            c = '\\';

        // MS KB Article ID 190351: How to spawn console processes with redirected standard handles.
        HANDLE stdoutRead = 0, stderrRead = 0, stdinWrite = 0;

        PROCESS_INFORMATION procInfo = {0};
        STARTUPINFOEXA startInfoEx = {0};
        STARTUPINFOA& startInfo = startInfoEx.StartupInfo;
        startInfo.cb = sizeof(startInfo);
        startInfo.wShowWindow = SW_SHOWNA;
        startInfo.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;

        BOOL ok = CreatePipeInheritable(&stdoutRead, false, &startInfo.hStdOutput, true);
        ok = ok && CreatePipeInheritable(&stderrRead, false, &startInfo.hStdError, true);
        ok = ok && CreatePipeInheritable(&startInfo.hStdInput, true, &stdinWrite, false);

        HANDLE inheritHandles[] = { startInfo.hStdOutput, startInfo.hStdError, startInfo.hStdInput };
        ProcThreadAttributeList attrib(1);
        attrib.SetAttribute(PROC_THREAD_ATTRIBUTE_HANDLE_LIST, inheritHandles, sizeof(inheritHandles));
        startInfoEx.lpAttributeList = attrib.Ptr();

        DWORD creationFlags = CREATE_NO_WINDOW | CREATE_NEW_PROCESS_GROUP | EXTENDED_STARTUPINFO_PRESENT;
        std::string command = "\"" + executable + "\" " + mArguments;
        ok = ok && ::CreateProcessA(0, const_cast<char *>(command.c_str()), 0, 0, TRUE, creationFlags, 0, mWorkingDirectory.c_str(),
                                    &startInfo, &procInfo);
        ::CloseHandle(startInfo.hStdOutput);
        ::CloseHandle(startInfo.hStdError);
        ::CloseHandle(startInfo.hStdInput);
        ok = ok && mStdin.Attach(stdinWrite, RedirFdObj::write);
        ok = ok && mStdout.Attach(stdoutRead, RedirFdObj::read);
        ok = ok && mStderr.Attach(stderrRead, RedirFdObj::read);
        if (ok)
        {
            mRunning = true;
            mPid = ::GetProcessId(procInfo.hProcess);
            ok = WAIT_OBJECT_0 == ::WaitForSingleObject(procInfo.hProcess, INFINITE);
            mRunning = false;
        }
        DWORD dwExitCode = -1;
        ok = ok && ::GetExitCodeProcess(procInfo.hProcess, &dwExitCode);
        if (ok)
            mResult = dwExitCode;

        mStdin.Detach();
        mStdout.Detach();
        mStderr.Detach();
        ::CloseHandle(stdinWrite);
        ::CloseHandle(stdoutRead);
        ::CloseHandle(stderrRead);

        ::CloseHandle(procInfo.hProcess);
        ::CloseHandle(procInfo.hThread);

#else // _WIN32

        int stdinpipe[2], stdoutpipe[2], stderrpipe[2];
        for (auto& p : {stdinpipe, stdoutpipe, stderrpipe}) {
            if (::pipe(p) < 0)
                throw std_runtime_error << "pipe() failed: " << SysError().Message();
        }
        mStdin.Attach(stdinpipe[1]);
        mStdout.Attach(stdoutpipe[0]);
        mStderr.Attach(stderrpipe[0]);

        // macOS does not like complex operations between fork() and exec().
        const char* wd_ = mWorkingDirectory.c_str();
        const char* exe_ = executable.c_str();
        std::string cmdline = "\"" + executable + "\" " + mArguments;
        auto argv = ProcessUtils::CommandLineToArgvA(cmdline.c_str(), nullptr);

        pid_t pid = ::fork();
        if (pid < 0)
            throw std_runtime_error << "fork() failed: " << SysError().Message();
        else if (pid > 0) {
            mPid = pid;
            // Execute in existing process:
            // 1. Close unused ends of pipes
            ::close(stdinpipe[0]);
            ::close(stdoutpipe[1]);
            ::close(stderrpipe[1]);
            // 2. Wait for child process to finish
            mRunning = true;
            int status = 0;
            errno = 0;
            do {
                if (::waitpid(pid, &status, 0) < 0)
                  throw std_runtime_error << "waitpid() failed: " << SysError().Message();
                if (WIFEXITED(status))
                    mResult = WEXITSTATUS(status);
                else if (WIFSIGNALED(status))
                    mResult = -WTERMSIG(status);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
            mRunning = false;
        }
        else if (pid == 0) {
            // Execute in child process:
            // 1. Map used ends of pipes
            ::dup2(stdinpipe[0], ::fileno(stdin));
            ::dup2(stdoutpipe[1], ::fileno(stdout));
            ::dup2(stderrpipe[1], ::fileno(stderr));
            // 2. Close unused file descriptors
            for (auto& p : {stdinpipe, stdoutpipe, stderrpipe}) {
                ::close(p[0]);
                ::close(p[1]);
            }
            // 3. Change working directory
            ::chdir(wd_);
            ::setenv("PWD", wd_, 1);
            // 4. Replace child process with new executable
            ::execv(exe_, argv);
            // 5. Terminate child process if exec() failed
            ::_exit(-1);
        }
#endif // _WIN32
    }
    std::string mExecutable, mArguments, mWorkingDirectory;
    std::atomic<int> mResult = 0;
    std::atomic<ProcessUtils::Pid> mPid;
    std::atomic<bool> mRunning = false;
    RedirFdObj mStdin, mStdout, mStderr;
    UnbufferedIO mStandardIO, mStandardError;
    Thread mThread;
};

ChildProcess::ChildProcess(const std::string &executable, const std::string &arguments)
    : p(new Private(this, executable, arguments))
{
    p->mWorkingDirectory = FileUtils::WorkingDirectory();
}

ChildProcess::~ChildProcess()
{
    delete p;
}

Tiny::Streambuf &ChildProcess::standardIO()
{
    return p->mStandardIO;
}

Tiny::Streambuf &ChildProcess::standardError()
{
    return p->mStandardError;
}

ChildProcess &ChildProcess::setWorkingDirectory(const std::string &wd)
{
    p->mWorkingDirectory = wd;
    return *this;
}

const std::string &ChildProcess::workingDirectory() const
{
    return p->mWorkingDirectory;
}

bool ChildProcess::run()
{
    p->mResult = -1;
    p->mThread.Start();
    while (p->mThread.Running() && p->mPid == 0)
        ThreadUtils::Idle();
    return p->mPid != 0;
}

ProcessUtils::Pid ChildProcess::pid() const
{
    if (p->mRunning)
        return p->mPid;
    return 0;
}

bool ChildProcess::running() const
{
    return p->mRunning;
}

int ChildProcess::result() const
{
    return p->mResult;
}

} // namespace Tiny
