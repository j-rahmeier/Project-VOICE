////////////////////////////////////////////////////////////////////////////////
// $Id: ProcessUtils.cpp 8236 2024-07-03 13:14:37Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: Utility functions for executing processes.
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
#include "ProcessUtils.h"

#include <iostream>
#include <string>
#include <mutex>

#if _WIN32
// clang-format off
#include <WS2tcpip.h>
#include <Windows.h>
#include <TlHelp32.h>
// clang-format on
#include <csignal>
#include <direct.h>
#include <dwmapi.h>
#include <fcntl.h>
#include <io.h>
#include <shellapi.h>
#else // _WIN32
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <spawn.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <vector>
#if __APPLE__
#include <crt_externs.h>
#define environ (*_NSGetEnviron())
#else  // __APPLE__
extern char **environ;
#endif // __APPLE__
#include "TimeUtils.h"
#endif // _WIN32

#include "ChildProcess.h"
#include "FileUtils.h"
#include "ProcessUtils.h"
#include "ThreadUtils.h"
#include "Streambuf.h"

namespace
{

bool GetProcessWDInternal(std::string &wd)
{
    int bufSize = 512;
    char *buf = nullptr;
    const char *cwd = nullptr;
    do
    {
        delete[] buf;
        buf = new char[bufSize];
        cwd = ::getcwd(buf, bufSize);
        bufSize += bufSize;
    } while (!cwd && (errno == ERANGE || errno == ENOMEM));
    if (cwd)
        wd = cwd;
    delete[] buf;
    return cwd;
}
bool SetProcessWDInternal(const std::string &wd)
{
    return !::chdir(wd.c_str());
}

std::recursive_mutex& WorkingDirAccess()
{
    static std::recursive_mutex instance;
    return instance;
}

struct WD_ : std::string
{
    WD_()
    {
        std::unique_lock<std::recursive_mutex> lock(WorkingDirAccess());
        GetProcessWDInternal(*this);
    }
};
std::string& InitialWD()
{
    static WD_ instance;
    return instance;
}
} // namespace

std::string ProcessUtils::InitialProcessWD()
{
    return InitialWD();
}

std::string ProcessUtils::ProcessWD()
{
    std::string wd;
    std::unique_lock<std::recursive_mutex> lock(WorkingDirAccess());
    if (!GetProcessWDInternal(wd))
        throw std_runtime_error << "Could not get process working directory";
    return wd;
}

void ProcessUtils::SetProcessWD(const std::string &wd)
{
    bool ok = false;
    std::unique_lock<std::recursive_mutex> lock(WorkingDirAccess());
    if (!SetProcessWDInternal(wd))
        throw std_runtime_error << "Could not change process working directory";
}

ProcessUtils::TemporaryProcessWD::TemporaryProcessWD(const std::string &s)
{
    WorkingDirAccess().lock();
    if (!s.empty())
    {
        if (!::GetProcessWDInternal(mWD))
            throw std_runtime_error << "Could not get process working directory";
        if (!::SetProcessWDInternal(s))
            throw std_runtime_error << "Could not change process working directory";
    }
}

ProcessUtils::TemporaryProcessWD::~TemporaryProcessWD() noexcept(false)
{
    if (!mWD.empty() && !::SetProcessWDInternal(mWD))
        throw std_runtime_error << "Could not change process working directory";
    WorkingDirAccess().unlock();
}

std::string ProcessUtils::UserName()
{
    const char *username = ::getenv("USERNAME");
    if (!username)
        username = ::getenv("USER");
    if (!username)
        username = "<unknown user>";
    return username;
}

std::string ProcessUtils::HostName()
{
    std::string hostname;
    int err = 0;
#if _WIN32
    WSAData wsaData = {0};
    err = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    if (!err)
    {
        char buf[NI_MAXHOST + 1];
        ::gethostname(buf, sizeof(buf) - 1);
        struct addrinfo *info = nullptr;
        err = ::getaddrinfo(buf, nullptr, nullptr, &info);
        if (!err)
        {
            char node[NI_MAXHOST];
            err = ::getnameinfo(info->ai_addr, info->ai_addrlen, node, sizeof(node), nullptr, 0, 0);
            if (!err)
                hostname = node;
            ::freeaddrinfo(info);
        }
#if _WIN32
        ::WSACleanup();
#endif
    }
    if (hostname.empty())
    {
        const char* pHostname = ::getenv("COMPUTERNAME");
        if (!pHostname)
            pHostname = ::getenv("HOSTNAME");
        if (pHostname)
            hostname = pHostname;
    }
    if (hostname.empty())
        hostname = "<unknown host>";
    return hostname;
}

#if _WIN32
namespace
{

class WinStdio
{
  public:
    WinStdio(int argc, char **argv);
    ~WinStdio();

  private:
    void SetupStdio();

    void SetSignalHandlers();
    static void OnSignal(int);
    static BOOL WINAPI OnConsoleCtrl(DWORD);
};

WinStdio::WinStdio(int argc, char **argv)
{
    if (std::string(argv[argc - 1]) == "&")
        return;
    if (::AttachConsole(ATTACH_PARENT_PROCESS))
        SetupStdio();
    SetSignalHandlers();
}

WinStdio::~WinStdio()
{
    ::SetConsoleCtrlHandler(&OnConsoleCtrl, FALSE);
}

void WinStdio::SetupStdio()
{
    if (_fileno(stdin) < 0)
    {
        ::freopen("CONIN$", "r", stdin);
        ::dup2(_fileno(stdin), 0);
    }
    if (_fileno(stdout) < 0)
    {
        ::freopen("CONOUT$", "w", stdout);
        ::dup2(_fileno(stdout), 1);
    }
    if (_fileno(stderr) < 0)
    {
        ::freopen("CONOUT$", "w", stderr);
        ::dup2(_fileno(stderr), 1);
    }
    std::ios::sync_with_stdio();
}

void WinStdio::SetSignalHandlers()
{
    int sig[] = {
        SIGINT, SIGILL, SIGFPE, SIGSEGV, SIGTERM, SIGBREAK, SIGABRT,
    };
    for (int i = 0; i < sizeof(sig) / sizeof(*sig); ++i)
        ::signal(sig[i], &OnSignal);
    ::SetConsoleCtrlHandler(&OnConsoleCtrl, TRUE);
}

void WinStdio::OnSignal(int inSignal)
{
    ::exit(128 + inSignal);
}

BOOL WINAPI WinStdio::OnConsoleCtrl(DWORD inEvent)
{
    int sig = 0;
    switch (inEvent)
    {
    case CTRL_C_EVENT:
        sig = SIGINT;
        break;
    case CTRL_BREAK_EVENT:
        sig = SIGBREAK;
        break;
    case CTRL_CLOSE_EVENT:
        sig = SIGTERM;
        break;
    default:;
    }
    if (sig)
    {
        bool done = ThreadUtils::RaiseSignal(sig, ThreadUtils::ID::Main());
        Assert(done);
        return TRUE;
    }
    return FALSE;
}

} // namespace
#endif // _WIN32

int ProcessUtils::WindowsEnableStdio(int argc, char **argv)
{
#if _WIN32
    static WinStdio w(argc, argv);
#endif // _WIN32
    return argc;
}

bool ProcessUtils::IsInteractive(int fd)
{
    return ::isatty(fd);
}

bool ProcessUtils::ExecuteSynchronously(const std::string &inExecutable, const std::string &inArguments,
                                        std::ostream &output, int &outExitCode)
{
    return ExecuteSynchronously(inExecutable, inArguments, output, output, outExitCode);
}

bool ProcessUtils::ExecuteSynchronously(const std::string &inExecutable, const std::string &inArguments,
                                        std::ostream &stdout_, std::ostream &stderr_, int &outExitCode)
{
    ChildProcess process(inExecutable, inArguments);
    if (!process.run())
        return false;
    auto out = process.standardIO().Input(), err = process.standardError().Input();
    Waitables waitfor;
    waitfor.Add(process).Add(out).Add(err);
    const Waitable *pTrigger = nullptr;
    while ((pTrigger = waitfor.Wait()) && pTrigger != &process)
    {
        std::vector<char> buf(out->Available());
        out->Read(const_cast<char *>(buf.data()), buf.size());
        stdout_.write(buf.data(), buf.size());
        buf.resize(err->Available());
        err->Read(const_cast<char *>(buf.data()), buf.size());
        stderr_.write(buf.data(), buf.size());
    }
    while (out->Available() > 0 || err->Available() > 0)
    {
        std::vector<char> buf(out->Available());
        out->Read(const_cast<char *>(buf.data()), buf.size());
        stdout_.write(buf.data(), buf.size());
        buf.resize(err->Available());
        err->Read(const_cast<char *>(buf.data()), buf.size());
        stderr_.write(buf.data(), buf.size());
    }
    return process.result() == 0;
}

bool ProcessUtils::StartProcess(const std::string &inExecutable, const std::string &inArguments, int &outExitCode,
                                ProcessUtils::Pid &outPid)
{
    bool success = false;
    outExitCode = 0;
    outPid = 0;
    std::string executable = inExecutable, wd = FileUtils::WorkingDirectory();

#if _WIN32

    std::string extension = ".exe";
    if (executable.length() < extension.length() ||
        ::stricmp(extension.c_str(), executable.substr(executable.length() - extension.length()).c_str()))
        executable += extension;

    // Use per-thread working directory to create absolute path
    executable = FileUtils::AbsolutePath(executable);
    // Make sure executable appears in tasklist with its long name
    executable = FileUtils::NormalizedPath(executable);
    // ShellExecuteEx does not accept forward slashes
    for (auto& c : executable)
        if (c == '/')
            c = '\\';
    if (executable.empty())
    {
        ::SetLastError(ERROR_FILE_NOT_FOUND);
        return false;
    }
    wd = FileUtils::CanonicalPath(wd); // ShellExecuteEx does not accept forward slashes
    if (wd.empty())
    {
        ::SetLastError(ERROR_BAD_PATHNAME);
        return false;
    }

    // When CreateProcess() is used to start up a core module, listening sockets in the Operator module are not closed
    // properly. ShellExecute() seems not to have this problem.
    SHELLEXECUTEINFOA info = {0};
    info.cbSize = sizeof(info);
    info.fMask = SEE_MASK_FLAG_DDEWAIT | SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;
    info.hwnd = NULL;
    info.lpVerb = "open";
    info.lpFile = executable.c_str();
    info.lpParameters = inArguments.c_str();
    info.lpDirectory = wd.c_str();
    info.nShow = SW_SHOWNA;

    success = ::ShellExecuteExA(&info);
    if (success)
    {
        bool processTerminated = true;
        DWORD dwExitCode = 0;
        ::GetExitCodeProcess(info.hProcess, &dwExitCode);
        if (dwExitCode == STILL_ACTIVE)
        { // STILL_ACTIVE as a return code may mean
            // 1) process has terminated with a result code that matches the value of STILL_ACTIVE,
            // 2) process is still running.
            processTerminated = false;
            dwExitCode = 0;
            if (::WaitForSingleObject(info.hProcess, 0) == WAIT_OBJECT_0)
            {
                processTerminated = true;
                // Here we are sure that the process has terminated, but it may still have been active
                // during the first GetExitCodeProcess() call.
                // So we need to repeat that call to be sure about the exit code as well.
                ::GetExitCodeProcess(info.hProcess, &dwExitCode);
            }
        }
        if (processTerminated) // has process finished due to initialization failure?
            success = (dwExitCode == 0);
        outExitCode = dwExitCode;
        outPid = ::GetProcessId(info.hProcess);
        ::CloseHandle(info.hProcess);
    }

#else // _WIN32

    const char* wd_ = wd.c_str();
    const char* exe_ = executable.c_str();
    std::string cmdline = "\"" + executable + "\" " + inArguments;
    char** argv = CommandLineToArgvA(cmdline.c_str(), nullptr);

    pid_t pid = fork();
    if (pid < 0)
        success = false;
    if (pid > 0)
    {
        ::usleep(1e6*0.25); // give child process time to try exec()
        int status = 0;
        int result = ::waitpid(pid, &status, WNOHANG);
        if (result == pid)
        {
            success = false;
            outPid = 0;
            outExitCode = WEXITSTATUS(status);
        }
        else if (result == 0)
        {
            success = true;
            outPid = pid;
            outExitCode = 0;
        }
        else if (result < 0)
        {
            success = false;
            outPid = 0;
            outExitCode = errno;
        }
        else
        {
            success = false;
            outPid = 0;
            outExitCode = WEXITSTATUS(status);
        }
    }
    if (pid == 0)
    {
        ::chdir(wd_);
        ::setenv("PWD", wd_, 1);
        int fdlimit = ::sysconf(_SC_OPEN_MAX);
        for (int i = STDERR_FILENO + 1; i < fdlimit; ++i)
            close(i);
        ::setsid(); // create new process group to stay alive when parent process quits
        ::execv(exe_, argv);
        ::_exit(-1);
    }

#endif // _WIN32

    return success;
}

bool ProcessUtils::WaitForProcessTerminated(Pid &pid, int timeoutMs)
{
    if (pid == 0)
        return true;
#if _WIN32
    HANDLE hProcess = ::OpenProcess(SYNCHRONIZE, FALSE, pid);
    if (!hProcess)
        switch (::GetLastError())
        {
        case ERROR_ACCESS_DENIED:
            return false;
        case ERROR_INVALID_PARAMETER:
            return true;
        default:
            SuggestDebugging << "Unexpected error";
            return true;
        }
    if (hProcess && (WAIT_OBJECT_0 == ::WaitForSingleObject(hProcess, timeoutMs)))
        pid = 0;
    if (hProcess)
        ::CloseHandle(hProcess);
    return pid == 0;
#else
    int r = ::waitpid(pid, nullptr, WNOHANG);
    if (timeoutMs < 0)
    {
        while (r >= 0 && r != pid)
            r = ::waitpid(pid, nullptr, 0);
    }
    else if (timeoutMs > 0)
    {
        auto delta = Time::FromMilliseconds(timeoutMs);
        auto until = TimeUtils::MonotonicTime() + delta;
        while (delta > 0 && r >= 0 && r != pid)
        {
            delta = std::min(delta, Time::FromMilliseconds(10));
            ThreadUtils::SleepFor(delta);
            r = ::waitpid(pid, nullptr, WNOHANG);
            delta = until - TimeUtils::MonotonicTime();
        }
    }
    if (r < 0)
        return false;
    if (r == pid)
        pid = 0;
    return pid == 0;
#endif
}

bool ProcessUtils::TerminateProcess(Pid &pid)
{
    WaitForProcessTerminated(pid, 0);
    if (pid == 0)
        return true;
#if _WIN32
    HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (!hProcess)
        switch (::GetLastError())
        {
        case ERROR_ACCESS_DENIED:
            return false;
        case ERROR_INVALID_PARAMETER:
            return true;
        default:
            SuggestDebugging << "Unexpected error";
            return true;
        }
    if (::GenerateConsoleCtrlEvent(CTRL_C_EVENT, pid))
        WaitForProcessTerminated(pid, 100);
    if (pid && ::TerminateProcess(hProcess, 0))
        WaitForProcessTerminated(pid, INFINITE);
    ::CloseHandle(hProcess);
    return pid == 0;
#else
    static const struct
    {
        int signal;
        int wait_ms;
    } trysignals[] = {{SIGINT, 50}, {SIGTERM, 250}, {SIGABRT, 500}, {SIGILL, 500}};
    for (auto &s : trysignals)
    {
        if (!::kill(pid, s.signal))
        {
            ThreadUtils::SleepFor(Time::FromMilliseconds(s.wait_ms));
            int r = ::waitpid(pid, nullptr, WNOHANG);
            if (r == pid)
                return true;
            if (r < 0)
                return errno == ECHILD ? true : false;
        }
    }
    return false;
#endif
}

bool ProcessUtils::SystemExecuteSynchronously(const std::string &inCommand, std::ostream &output, int &outExitCode)
{
    return SystemExecuteSynchronously(inCommand, output, output, outExitCode);
}

bool ProcessUtils::SystemExecuteSynchronously(const std::string &command, std::ostream &out, std::ostream &err,
                                              int &exitCode)
{
    std::string shell, args;
#if _WIN32
    shell = "cmd";
    args = "/c \"" + command + "\"";
#else
    shell = "sh";
    args = "-c \"" + command + "\"";
#endif
    return ExecuteSynchronously(shell, args, out, err, exitCode);
}

#if _WIN32
namespace
{
// Find the first available actual window of a process, if any
class FindProcessWindow
{
  public:
    FindProcessWindow(DWORD processId, HWND ignore = NULL) : mResult(NULL), mIgnore(ignore), mPid(processId)
    {
        ::EnumWindows(&EnumWindowsProc, LPARAM(this));
    }
    operator HWND() const
    {
        return mResult;
    }

  private:
    static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
    {
        auto this_ = reinterpret_cast<FindProcessWindow *>(lParam);
        DWORD pid = 0;
        if (::GetWindowThreadProcessId(hwnd, &pid) && pid == this_->mPid && this_->mIgnore != hwnd)
        {
            this_->mResult = hwnd;
            return FALSE;
        }
        return TRUE;
    }
    HWND mResult, mIgnore;
    DWORD mPid;
};
class ShowProcessWindows_
{
  public:
    ShowProcessWindows_(DWORD pid, int nCmdShow) : mResult(TRUE), mPid(pid), mCmdShow(nCmdShow)
    {
        ::EnumWindows(&EnumWindowsProc, LPARAM(this));
    }
    operator bool() const
    {
        return mResult;
    }

  private:
    static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
    {
        auto this_ = reinterpret_cast<ShowProcessWindows_ *>(lParam);
        DWORD pid = 0;
        char text[2];
        if (::GetWindowTextA(hwnd, text, 2) &&
            ::GetWindowThreadProcessId(hwnd, &pid) && pid == this_->mPid)
            this_->mResult &= ::ShowWindow(hwnd, this_->mCmdShow);
        return TRUE;
    }
    DWORD mPid;
    int mCmdShow;
    BOOL mResult;
};
} // namespace
#endif

bool ProcessUtils::SendProcessToForeground(Pid pid)
{
#if _WIN32
    HWND window = FindProcessWindow(pid ? pid : ::GetCurrentProcessId());
    if (window)
        return ::SetForegroundWindow(window);
#endif
    return false;
}

bool ProcessUtils::ShowProcessWindows(Pid pid)
{
#if _WIN32
    return ShowProcessWindows_(pid ? pid : ::GetCurrentProcessId(), SW_SHOWNA);
#endif
    return false;
}

bool ProcessUtils::HideProcessWindows(Pid pid)
{
#if _WIN32
    return ShowProcessWindows_(pid ? pid : ::GetCurrentProcessId(), SW_HIDE);
#endif
    return false;
}

#if _WIN32
namespace
{

namespace Shell32
{
HRESULT(WINAPI *SetCurrentProcessExplicitAppUserModelID)(PCWSTR) = 0;
bool Win7Init()
{
    bool result = true;
    HMODULE shell32 = ::LoadLibraryW(L"Shell32");
#define IMPORT(x)                                                                                                      \
    *(void **)&x = ::GetProcAddress(shell32, #x);                                                                      \
    result = result && x;
    IMPORT(SetCurrentProcessExplicitAppUserModelID);
#undef IMPORT
    return result;
}
} // namespace Shell32

namespace Dwmapi
{
HRESULT(WINAPI *DwmSetWindowAttribute)(HWND, DWORD, LPCVOID, DWORD);
HRESULT(WINAPI *DwmSetIconicThumbnail)(HWND, HBITMAP, DWORD);
bool Win7Init()
{
    bool result = true;
    HMODULE dwmapi = ::LoadLibraryW(L"Dwmapi");
#define IMPORT(x)                                                                                                      \
    *(void **)&x = ::GetProcAddress(dwmapi, #x);                                                                       \
    result = result && x;
    IMPORT(DwmSetWindowAttribute);
    IMPORT(DwmSetIconicThumbnail);
#undef IMPORT
    return result;
}
} // namespace Dwmapi

static bool Win7Initialized()
{
    bool sStatus = Shell32::Win7Init() && Dwmapi::Win7Init();
    return sStatus;
}

class TaskbarProxyWindow
{
    // Defines a window that is handled by its own thread.
    //
    // This window is never shown, but will have an entry in the task bar, which may be used
    // to bring the main window to the front (if any).
    // The original reason for introducing this functionality was the AVG virus scanner's
    // "Identity Protection" feature which was apparently triggered by having executables
    // running under the Windows subsystem, but not owning any windows.
  public:
    TaskbarProxyWindow(const char *title);
    ~TaskbarProxyWindow();
    DWORD Error()
    {
        return mError;
    }

  private:
    HWND Create(const char *);
    LRESULT OnClose()
    {
        return 0;
    } // refuse closing for now, might as well ask user whether to call ExitProcess()
    LRESULT OnQueryOpen();
    LRESULT OnQueryThumbnail(int, int);
    static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
    static DWORD CALLBACK ThreadProc(LPVOID);

    struct ThreadProcData
    {
        TaskbarProxyWindow *pInstance;
        const char *pWindowTitle;
        HANDLE event;
    };
    HWND mWnd;
    HICON mIcon;
    DWORD mError;
    enum
    {
        uninitialized,
        created,
        initialized,
    } mState;
};

TaskbarProxyWindow::TaskbarProxyWindow(const char *title) : mWnd(0), mError(0), mState(uninitialized), mIcon(0)
{
    ThreadProcData data = {
        this,
        title,
        ::CreateEventW(nullptr, true, false, nullptr),
    };
    bool ok = data.event;
    if (ok)
    {
        HANDLE h = ::CreateThread(nullptr, 1, &TaskbarProxyWindow::ThreadProc, &data, 0, nullptr);
        if (h)
            ::CloseHandle(h);
        else
            ok = false;
    }
    ok = ok && (::WaitForSingleObject(data.event, INFINITE) == WAIT_OBJECT_0);
    if (!ok && !mError)
        mError = ::GetLastError();
    if (data.event)
        ::CloseHandle(data.event);
}

TaskbarProxyWindow::~TaskbarProxyWindow()
{
}

DWORD CALLBACK TaskbarProxyWindow::ThreadProc(LPVOID pData_)
{
    ThreadProcData *pData = static_cast<ThreadProcData *>(pData_);
    HWND window = pData->pInstance->Create(pData->pWindowTitle);
    bool ok = window;
    ok = ok && ::ShowWindowAsync(window, SW_SHOWNORMAL);
    if (!ok && !pData->pInstance->mError)
        pData->pInstance->mError = ::GetLastError();
    ::SetEvent(pData->event);
    if (ok)
    {
        ::ShowWindowAsync(window, SW_MINIMIZE);
        MSG msg;
        while (::GetMessageA(&msg, window, 0, 0) > 0)
        {
            ::TranslateMessage(&msg);
            ::DispatchMessageA(&msg);
        }
        delete pData->pInstance;
    }
    return 0;
}

HWND TaskbarProxyWindow::Create(const char *title)
{
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = &TaskbarProxyWindow::WindowProc;
    wc.hInstance = ::GetModuleHandleA(nullptr);
    wc.lpszClassName = "ProcessUtils::TaskbarProxy";
    wc.cbWndExtra = sizeof(TaskbarProxyWindow *);
    wc.style = CS_NOCLOSE;
    wc.hIcon = ::LoadIconA(wc.hInstance, MAKEINTRESOURCEA(1));
    DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_NOACTIVATE, dwStyle = WS_ICONIC;
    if (::RegisterClassA(&wc) || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS)
        mWnd = ::CreateWindowExA(dwExStyle, wc.lpszClassName, title, dwStyle, 0, 0, 0, 0, 0, 0, wc.hInstance, this);
    if (mWnd && Win7Initialized())
    {
        BOOL value = TRUE;
        Dwmapi::DwmSetWindowAttribute(mWnd, DWMWA_HAS_ICONIC_BITMAP, &value, sizeof(value));
        mIcon = wc.hIcon;
        OnQueryThumbnail(MAXINT32, MAXINT32);
    }
    if (!mWnd)
        mError = ::GetLastError();
    return mWnd;
}

LRESULT
TaskbarProxyWindow::OnQueryOpen()
{
    HWND window = FindProcessWindow(::GetCurrentProcessId(), mWnd);
    if (window)
        ::SetForegroundWindow(window);
    return 0;
}

LRESULT
TaskbarProxyWindow::OnQueryThumbnail(int maxWidth, int maxHeight)
{
    int w = std::min(128, maxWidth), h = std::min(32, maxHeight);

    HDC dc = ::GetDC(NULL);
    HDC memDC = ::CreateCompatibleDC(dc);
    HBITMAP bmp = ::CreateCompatibleBitmap(dc, w, h);
    ::SelectObject(memDC, bmp);
    ::DrawIconEx(memDC, (w - h) / 2, 0, mIcon, h, h, 0, 0, DI_NORMAL);
    HBITMAP dib = (HBITMAP)::CopyImage(bmp, IMAGE_BITMAP, w, h, LR_CREATEDIBSECTION);
    HRESULT hr = Dwmapi::DwmSetIconicThumbnail(mWnd, dib, 0);
    if (FAILED(hr))
        switch (hr)
        {
        case DWM_E_COMPOSITIONDISABLED:
            break; // not a problem
        default:
            SuggestDebugging << SysError(hr).Message();
        }
    ::DeleteObject(dib);
    ::DeleteObject(bmp);
    ::DeleteObject(memDC);
    ::ReleaseDC(NULL, dc);
    return 0;
}

LRESULT CALLBACK TaskbarProxyWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE)
    {
        const CREATESTRUCTA *pData = reinterpret_cast<CREATESTRUCTA *>(lParam);
        ::SetWindowLongPtrA(hwnd, GWLP_USERDATA, LONG_PTR(pData->lpCreateParams));
    }
    TaskbarProxyWindow *this_ = reinterpret_cast<TaskbarProxyWindow *>(::GetWindowLongPtrA(hwnd, GWLP_USERDATA));
    switch (uMsg)
    {
    case WM_DWMSENDICONICTHUMBNAIL:
        return this_->OnQueryThumbnail(HIWORD(lParam), LOWORD(lParam));
    case WM_QUERYOPEN:
        return this_->OnQueryOpen();
    case WM_CLOSE:
        return this_->OnClose();
    case WM_CREATE:
        this_->mState = created;
    }
    return ::DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

} // namespace
#endif // _WIN32

bool ProcessUtils::InitializeDesktopIntegration(const std::string &inId, const std::string &inName)
{
    bool ok = true;
#if _WIN32
    if (!Win7Initialized())
        throw std_runtime_error << "This executable requires Windows 7 or newer";
    if (Shell32::SetCurrentProcessExplicitAppUserModelID(StringUtils::ToWide(inId).c_str()) < 0)
        SuggestDebugging << SysError().Message();
    if (!inName.empty())
    {
        TaskbarProxyWindow *pProxy = new TaskbarProxyWindow(inName.c_str());
        if (pProxy->Error())
        {
            ::SetLastError(pProxy->Error());
            ok = false;
            delete pProxy;
        }
    }
#endif //_WIN32
    if (!ok)
        SuggestDebugging << SysError().Message();
    return ok;
}

bool Tiny::ProcessUtils::InitializeShellIntegration(const std::string& registrar)
{
#if _WIN32
    std::string dll = registrar;
    if (!FileUtils::IsAbsolutePath(dll))
        dll = FileUtils::InstallationDirectory() + registrar;
    dll = FileUtils::ExtractDirectory(dll) + FileUtils::ExtractBase(dll);
    HMODULE hModule = ::LoadLibraryA(dll.c_str());
    if (!hModule)
        hModule = ::LoadLibraryA((dll + "32").c_str());
    if (!hModule)
        hModule = ::LoadLibraryA((dll + "64").c_str());
    if (!hModule)
        return false;
    typedef HRESULT(__stdcall *DllInstallPtr)(BOOL, const wchar_t*);
    DllInstallPtr DllInstall = reinterpret_cast<DllInstallPtr>(::GetProcAddress(hModule, "DllInstall"));
    HRESULT result = E_FAIL;
    if (DllInstall)
        result = DllInstall(TRUE, L"refresh");
    ::FreeLibrary(hModule);
    return SUCCEEDED(result);
#else
    return true;
#endif
}


bool ProcessUtils::RequestPrecisionTiming()
{
#if _WIN32
# ifndef PROCESS_POWER_THROTTLING_IGNORE_TIMER_RESOLUTION // Win11 headers
#  define PROCESS_POWER_THROTTLING_IGNORE_TIMER_RESOLUTION 0x4
# endif
    // According to MSDN, in Windows 11, explicitly opting out of "ignore timer resolution" 
    // restores behavior as it was in Windows 10.
    PROCESS_POWER_THROTTLING_STATE state = { 0 };
    state.Version = PROCESS_POWER_THROTTLING_CURRENT_VERSION;
    state.ControlMask = PROCESS_POWER_THROTTLING_IGNORE_TIMER_RESOLUTION;
    state.StateMask = 0;
    ::SetProcessInformation(::GetCurrentProcess(), ProcessPowerThrottling, &state, sizeof(state));
    // If SetProcessInformation() fails, we are on Win10 and may return true nevertheless.
    return true;
#else
    return true;
#endif
}

namespace
{

#if !_WIN32
// Normalize name for POSIX temporary file
std::string NormalizeName(const std::string& inName)
{
    std::string name = inName;
    for (auto& c : name)
        if (c == '/')
            c = '_';
    return "/" + name;
}
#endif

class GlobalID
{
  public:
    GlobalID(const std::string &name, int timeout);
    ~GlobalID();
    bool Owned() const
    {
#if _WIN32
        return mHandle != NULL;
#else
        return mLocked;
#endif
    }
    static void Cleanup(const std::string &name);

  private:
    bool TryCreate(const std::string &inName);
    void Destroy();
#if _WIN32
    HANDLE mHandle = NULL;
#else
    bool mLocked = false;
    int mFd = -1;
    std::string mName;
#endif
};

GlobalID::GlobalID(const std::string &inName, int inTimeout)
{
    const int cTimeResolution = 100; // ms
    int timeElapsed = 0;
    while (!TryCreate(inName) && timeElapsed < inTimeout)
    {
        ThreadUtils::SleepForMs(cTimeResolution);
        timeElapsed += cTimeResolution;
    }
}

GlobalID::~GlobalID()
{
    Destroy();
}

bool GlobalID::TryCreate(const std::string &inName)
{
#if _WIN32

    if (mHandle)
        return true;

    HANDLE mutex = ::CreateMutexA(NULL, true, inName.c_str());
    if (::GetLastError() == ERROR_ALREADY_EXISTS)
        ::CloseHandle(mutex);
    else
        mHandle = reinterpret_cast<void *>(mutex);

    return mHandle != NULL;

#else // _WIN32

    std::string dir = "/tmp"; // we need the global /tmp to have all processes refer to the same files
    if (!FileUtils::IsDirectory(dir)) {
        throw std_runtime_error << "Could not find a suitable directory for Global IDs";
    }
    dir += "/bci2000/GlobalID";
    FileUtils::MakeDirectory(dir, true);
    mName = dir + NormalizeName(inName);
    mFd = ::open(mName.c_str(), O_CREAT | O_WRONLY | O_CLOEXEC, 0666);
    if (mFd < 0) {
        mLocked = false;
        throw std_runtime_error << "Could not create run file at " << mName;
    }
    mLocked = ::flock(mFd, LOCK_EX | LOCK_NB) >= 0;
    return mLocked;
    
#endif // _WIN32
}

void GlobalID::Destroy()
{
#if _WIN32

    if (!mHandle)
        return;

    ::ReleaseMutex(mHandle);
    ::CloseHandle(mHandle);
    mHandle = NULL;
    

#else // _WIN32

    if (mFd >= 0)
    {
        ::flock(mFd, LOCK_UN);
        ::close(mFd);
        mFd = -1;
    }
    if (!mName.empty())
    {
        ::unlink(mName.c_str());
        mName.clear();
    }
    mLocked = false;

#endif // _WIN32
}

void GlobalID::Cleanup(const std::string &inName)
{
}

} // namespace

bool ProcessUtils::AssertSingleInstance(int inArgc, char **inArgv, const std::string &inID, int inTimeout)
{
    std::string name = inID.empty() ? FileUtils::ApplicationTitle() : inID;
    for (int i = 1; inArgv && i < inArgc; ++i)
        if (!::stricmp(inArgv[i], "--AllowMultipleInstances"))
        {
            GlobalID::Cleanup(name);
            return true;
        }
    static const GlobalID sInstance(name, inTimeout);
    return sInstance.Owned();
}

static int GetCurrentPriority();
static bool SetNormalPriority();
static bool SetPriority(int);

bool ProcessUtils::Priority::Set(int p)
{
    int mode = Normal;
    if (::abs(p - Minimum) < ::abs(p - Normal))
        mode = Minimum;
    else if (::abs(p - Maximum) < ::abs(p - Normal))
        mode = Maximum;
    int delta = p - mode;
    switch (mode)
    {
    case Minimum:
        while (Decrease())
            ;
        break;
    case Maximum:
        while (Increase())
            ;
        break;
    default:
        SetNormalPriority();
    }
    return ChangeBy(delta);
}

int ProcessUtils::Priority::Get()
{
    return GetCurrentPriority();
}

bool ProcessUtils::Priority::Increase(int by)
{
    return ChangeBy(by);
}

bool ProcessUtils::Priority::Decrease(int by)
{
    return ChangeBy(-by);
}

bool ProcessUtils::Priority::ChangeBy(int delta)
{
    int p = GetCurrentPriority();
    if (p < 0)
        return false;
    return SetPriority(p + delta);
}

#if _WIN32

static int sProcessPriorities[] = {IDLE_PRIORITY_CLASS,         BELOW_NORMAL_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS,
                                   ABOVE_NORMAL_PRIORITY_CLASS, HIGH_PRIORITY_CLASS,         REALTIME_PRIORITY_CLASS};

bool SetNormalPriority()
{
    return ::SetPriorityClass(::GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
}

bool SetPriority(int i)
{
    if (i >= 0 && i < sizeof(sProcessPriorities) / sizeof(*sProcessPriorities))
    {
        int value = sProcessPriorities[i];
        if (::SetPriorityClass(::GetCurrentProcess(), value))
            return ::GetPriorityClass(::GetCurrentProcess()) == value;
    }
    return false;
}

int GetCurrentPriority()
{
    int p = ::GetPriorityClass(::GetCurrentProcess());
    for (int i = 0; i < sizeof(sProcessPriorities) / sizeof(*sProcessPriorities); ++i)
        if (sProcessPriorities[i] == p)
            return i;
    SuggestDebugging << "Unexpected result of GetPriorityClass(): " << p;
    return -1;
}

#else

bool SetNormalPriority()
{
    int r = ::setpriority(PRIO_PROCESS, 0, 0);
    return r >= 0;
}

bool SetPriority(int i)
{
    // remap input range of [0..39] to [20..-19] as expected by setpriority()
    int priority = -(i - 20);
    int r = ::setpriority(PRIO_PROCESS, 0, priority);
    return r >= 0;
}

int GetCurrentPriority()
{
    errno = 0;
    int r = ::getpriority(PRIO_PROCESS, 0);
    if (r == -1 && errno != 0)
        return -1;
    return -r + 20;
}

#endif

#ifndef _WIN32
// Imitate Win32 CommandLineToArgvW() on Posix systems to assure
// identical behavior across platforms.
// This function has been adapted from ReactOS 0.4.15-dev-1647-g91fceab,
// wine/shell32_main.c
char**
ProcessUtils::CommandLineToArgvA(const char *lpCmdline, int *pArgc)
{
    /* --- First count the arguments */
    int argc=1;
    const char* s=lpCmdline;
    /* The first argument, the executable path, follows special rules */
    if (*s=='"')
    {
        /* The executable path ends at the next quote, no matter what */
        s++;
        while (*s)
            if (*s++=='"')
                break;
    }
    else
    {
        /* The executable path ends at the next space, no matter what */
        while (*s && *s!=' ' && *s!='\t')
            s++;
    }
    /* skip to the first argument, if any */
    while (*s==' ' || *s=='\t')
        s++;
    if (*s)
        argc++;

    /* Analyze the remaining arguments */
    int qcount = 0, bcount = 0;
    while (*s)
    {
        if ((*s==' ' || *s=='\t') && qcount==0)
        {
            /* skip to the next argument and count it if any */
            while (*s==' ' || *s=='\t')
                s++;
            if (*s)
                argc++;
            bcount=0;
        }
        else if (*s=='\\')
        {
            /* '\', count them */
            bcount++;
            s++;
        }
        else if (*s=='"')
        {
            /* '"' */
            if ((bcount & 1)==0)
                qcount++; /* unescaped '"' */
            s++;
            bcount=0;
            /* consecutive quotes, see comment in copying code below */
            while (*s=='"')
            {
                qcount++;
                s++;
            }
            qcount=qcount % 3;
            if (qcount==2)
                qcount=0;
        }
        else
        {
            /* a regular character */
            bcount=0;
            s++;
        }
    }

    /* Allocate in a single lump, the string array, and the strings that go
     * with it. This way the caller can make a single free() call to free
     * both, as in Win32.
     */
    char** argv = static_cast<char**>(::malloc((argc+1)*sizeof(char*)+(::strlen(lpCmdline)+1)*sizeof(char)));
    if (!argv)
        return nullptr;
    char* cmdline = reinterpret_cast<char*>(argv+argc+1);
    ::strcpy(cmdline, lpCmdline);

    /* --- Then split and copy the arguments */
    argv[0]=cmdline;
    argc=1;
    char* d = cmdline;
    /* The first argument, the executable path, follows special rules */
    if (*d=='"')
    {
        /* The executable path ends at the next quote, no matter what */
        s=d+1;
        while (*s)
        {
            if (*s=='"')
            {
                s++;
                break;
            }
            *d++=*s++;
        }
    }
    else
    {
        /* The executable path ends at the next space, no matter what */
        while (*d && *d!=' ' && *d!='\t')
            d++;
        s=d;
        if (*s)
            s++;
    }
    /* close the executable path */
    *d++=0;
    /* skip to the first argument and initialize it if any */
    while (*s==' ' || *s=='\t')
        s++;
    if (!*s)
    {
        /* There are no parameters so we are all done */
        argv[argc]=nullptr;
        if (pArgc)
            *pArgc = argc;
        return argv;
    }

    /* Split and copy the remaining arguments */
    argv[argc++]=d;
    qcount=bcount=0;
    while (*s)
    {
        if ((*s==' ' || *s=='\t') && qcount==0)
        {
            /* close the argument */
            *d++=0;
            bcount=0;

            /* skip to the next one and initialize it if any */
            do {
                s++;
            } while (*s==' ' || *s=='\t');
            if (*s)
                argv[argc++]=d;
        }
        else if (*s=='\\')
        {
            *d++=*s++;
            bcount++;
        }
        else if (*s=='"')
        {
            if ((bcount & 1)==0)
            {
                /* Preceded by an even number of '\', this is half that
                 * number of '\', plus a quote which we erase.
                 */
                d-=bcount/2;
                qcount++;
            }
            else
            {
                /* Preceded by an odd number of '\', this is half that
                 * number of '\' followed by a '"'
                 */
                d=d-bcount/2-1;
                *d++='"';
            }
            s++;
            bcount=0;
            /* Now count the number of consecutive quotes. Note that qcount
             * already takes into account the opening quote if any, as well as
             * the quote that lead us here.
             */
            while (*s=='"')
            {
                if (++qcount==3)
                {
                    *d++='"';
                    qcount=0;
                }
                s++;
            }
            if (qcount==2)
                qcount=0;
        }
        else
        {
            /* a regular character */
            *d++=*s++;
            bcount=0;
        }
    }
    *d='\0';
    argv[argc]=nullptr;
    if (pArgc)
        *pArgc = argc;
    return argv;
}

#endif // _WIN32
