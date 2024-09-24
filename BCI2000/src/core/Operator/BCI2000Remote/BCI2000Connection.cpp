//////////////////////////////////////////////////////////////////////
// $Id: BCI2000Connection.cpp 8254 2024-07-16 14:48:21Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A wrapper class representing a telnet connection to
//   BCI2000.
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
#include "BCI2000Connection.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <sstream>

#if _WIN32
#include <Windows.h>
#define stricmp _stricmp
#endif // _WIN32

#if __APPLE__
#include <sys/stat.h>
#endif

const std::string ReadlineTag = "\\AwaitingInput:";
const std::string AckTag = "\\AcknowledgedInput";
const std::string ExitCodeTag = "\\ExitCode";
const std::string TerminationTag = "\\Terminating";
const std::string Prompt = ">";

#if _WIN32
namespace {
    bool IsLocalAddress(const std::string&);
    bool IsOperatorRunningLocally();
}
#endif

BCI2000Connection &BCI2000Connection::WindowVisible(int inVisible)
{
    mWindowVisible = inVisible;
    if (mSocket.connected() && mWindowVisible != dontChange)
    {
        if (mWindowVisible == 0)
            Execute("hide window", 0);
        else
            Execute("show window", 0);
    }
    return *this;
}

BCI2000Connection &BCI2000Connection::WindowTitle(const std::string &inTitle)
{
    mWindowTitle = inTitle;
    if (mSocket.connected())
        Execute("set title \"" + inTitle + "\"", 0);
    return *this;
}

bool BCI2000Connection::Connect()
{
    Disconnect();
    mResult.clear();
    if (mTelnetAddress.empty())
        mTelnetAddress = DefaultTelnetAddress();
    bool success = true;
#if _WIN32
    // Windows has a long TCP connection timeout (>1s) even for local sockets.
    // By checking whether the Operator module is running at all, we can save
    // a failed connection attempt, and speed up BCI2000Connection::Connect()
    // by about 2 seconds.
    if (IsLocalAddress(mTelnetAddress) && !IsOperatorRunningLocally())
        success = false;
#endif
    if (success)
        mSocket.open(mTelnetAddress);
    success = mSocket.connected();
    bool operatorRunning = success;
    if (!operatorRunning && !mOperatorPath.empty())
    {
        operatorRunning = Run(mOperatorPath, mOperatorOptions);
        mTerminateOperator = operatorRunning;
        if (operatorRunning)
        {
            mSocket.open(mTelnetAddress);
            success = mSocket.connected();
        }
    }
    if (success)
        success = mSocket.wait_for_read(static_cast<int>(1e3 * mTimeout));
    if (!success && operatorRunning)
    {
        std::ostringstream oss;
        if (mSocket.last_error())
            oss << mSocket.error_message();
        else
            oss << "connection attempt timed out after " << mTimeout << "s";
        mResult = "Could not connect to Operator module at " + mTelnetAddress + ": " + oss.str();
    }
    else
    {
        WaitForPrompt();
        WindowVisible(mWindowVisible);
        WindowTitle(mWindowTitle);
    }
    return success;
}

bool BCI2000Connection::Connect(const BCI2000Connection &inRemote)
{
    mWindowVisible = inRemote.mWindowVisible;
    mWindowTitle = inRemote.mWindowTitle;

    mResult.clear();
    bool success = inRemote.mSocket.is_open();
    if (success)
        success = this->OperatorPath("").TelnetAddress(inRemote.TelnetAddress()).Connect();
    return success;
}

bool BCI2000Connection::Disconnect(bool neverQuit)
{
    mResult.clear();
    if (mSocket.connected() && mTerminateOperator && !neverQuit)
        Quit();
    mTerminateOperator = false;
    mSocket.close();
    mConnection.clear();
    return true;
}

bool BCI2000Connection::Connected()
{
    return mSocket.connected();
}

bool BCI2000Connection::Execute(const std::string &inCommand, int *outpExitCode, std::string *outpExitString)
{
    mResult.clear();
    if (!mSocket.connected())
    {
        mResult = "Not connected (call BCI2000Connection::Connect() to establish a connection)";
        return false;
    }
    if (inCommand.find_first_of("\r\n") != std::string::npos)
    {
        mResult = "Multiple lines in command";
        return false;
    }
    mConnection << inCommand << "\r\n" << std::flush;
    static const int cReactionTimeMs = 100;
    while (!mSocket.wait_for_read(cReactionTimeMs))
    {
        if (!mSocket.connected())
        {
            mResult = "Lost connection to Operator module";
            return false;
        }
    }
    return ProcessResponse(outpExitCode, outpExitString);
}

bool BCI2000Connection::ProcessResponse(int *outpExitCode, std::string* outpExitString)
{
    int ignore, &outExitCode = outpExitCode ? *outpExitCode : ignore;
    outExitCode = 0;
    std::string exitString, line;
    while (mConnection && ((line != Prompt) || mConnection.rdbuf()->in_avail()))
    {
        int c = mConnection.get();
        if (c == '\n' || c == EOF)
        {
            if (line == ReadlineTag)
            {
                std::string input;
                if (OnInput(input))
                {
                    mConnection << input << "\r\n" << std::flush;
                    if (!mSocket.wait_for_read(static_cast<int>(1e3 * mTimeout)) || !getline(mConnection, line) ||
                        line.find(AckTag) != 0)
                    {
                        mResult = "Did not receive input acknowledgement";
                        return false;
                    }
                }
                else
                {
                    mResult = "Could not handle request for input (override BCI2000Connection::OnInput())";
                    return false;
                }
            }
            else if (line.find(ExitCodeTag) == 0)
            {
                exitString += "\n" + line.substr(ExitCodeTag.length());
                std::istringstream(line.substr(ExitCodeTag.length())) >> outExitCode;
            }
            else if (line == TerminationTag)
            {
                mSocket.close();
                mConnection.clear();
                mTerminateOperator = false;
                return true;
            }
            else
            {
                if (!OnOutput(line))
                {
                    if (!mResult.empty())
                        mResult += '\n';
                    mResult += line;
                }
                double value;
                exitString += "\n" + line;
                std::istringstream iss(line);
                // Map output to 0 for "true" and 1 for "false".
                // This is _intentional_ to be consistent with exit codes of
                // command line executables.
                if ((iss >> std::ws >> value >> std::ws).eof())
                    outExitCode = (value != 0) ? 0 : 1;
                else if (!::stricmp(line.c_str(), "true"))
                    outExitCode = 0;
                else if (!::stricmp(line.c_str(), "false"))
                    outExitCode = 1;
                else if (line.empty())
                    outExitCode = 0;
                else
                    outExitCode = -1;
            }
            line.clear();
        }
        else if (c != '\r')
        {
            line += c;
        }
    }
    if (outpExitString) {
        outpExitString->clear();
        if (!exitString.empty())
            *outpExitString = exitString.substr(1);
    }
    return true;
}

void BCI2000Connection::WaitForPrompt()
{
    std::string line;
    while (mConnection && (line != Prompt || mConnection.rdbuf()->in_avail() > 0))
    {
        int c = mConnection.get();
        switch (c)
        {
        case '\n':
            line.clear();
            break;
        default:
            line += c;
        }
    }
}

bool BCI2000Connection::Run(const std::string &inOperatorPath, const std::string &inOptions)
{
    std::string options = inOptions;
    options += " --Telnet \"" + mTelnetAddress + "\"";
    options += " --StartupIdle";
    if (!mWindowTitle.empty())
        options += " --Title \"" + mWindowTitle + "\"";
    if (mWindowVisible != visible)
        options += " --Hide";
    bool success = StartExecutable(inOperatorPath, options);
    if (success)
    {
        const int cReactionTime = 50; // ms
        client_tcpsocket testSocket;
        time_t start = ::time(NULL);
        while (::difftime(::time(NULL), start) < mTimeout && !testSocket.is_open())
        {
            testSocket.open(mTelnetAddress.c_str());
            testSocket.wait_for_read(cReactionTime);
        }
        success = testSocket.is_open();
        if (!success)
            mResult = "Operator module does not listen on " + mTelnetAddress;
    }
    return success;
}

bool BCI2000Connection::Quit()
{
    Execute("quit", 0);
    return Result().empty();
}

#if _WIN32
bool BCI2000Connection::StartExecutable(const std::string &inExecutable, const std::string &inOptions)
{
    HINSTANCE lib = ::LoadLibraryA("shlwapi");
    typedef BOOL(WINAPI * PathRemoveFileSpecA_)(char *);
    PathRemoveFileSpecA_ PathRemoveFileSpecA =
        reinterpret_cast<PathRemoveFileSpecA_>(::GetProcAddress(lib, "PathRemoveFileSpecA"));
    typedef char *(WINAPI * PathFindExtensionA_)(const char *);
    PathFindExtensionA_ PathFindExtensionA =
        reinterpret_cast<PathFindExtensionA_>(::GetProcAddress(lib, "PathFindExtensionA"));
    if (!lib || !PathRemoveFileSpecA || !PathFindExtensionA)
    {
        mResult = "Could not load shlwapi functions";
        return false;
    }

    mResult.clear();
    std::string commandLine = "\"" + inExecutable + "\" " + inOptions;

    char *pWorkingDir = new char[inExecutable.length() + 1];
    ::memcpy(pWorkingDir, inExecutable.c_str(), inExecutable.length() + 1);
    PathRemoveFileSpecA(pWorkingDir);
    if (*pWorkingDir == '\0')
    {
        delete[] pWorkingDir;
        pWorkingDir = NULL;
    }

    const char *pExt = PathFindExtensionA(inExecutable.c_str());
    bool isScript = (0 == ::stricmp(pExt, ".cmd") || 0 == ::stricmp(pExt, ".bat"));
    if (isScript)
        commandLine = "cmd /c call " + commandLine;
    else
        commandLine += " &";

    STARTUPINFOA si = {0};
    si.cb = sizeof(si);
#if 0
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOWNOACTIVATE;
#endif
    PROCESS_INFORMATION pi = {0};
    bool success = (TRUE == ::CreateProcessA(NULL, const_cast<char *>(commandLine.c_str()), NULL, NULL, FALSE, 0, NULL,
                                             pWorkingDir, &si, &pi));
    if (!success)
    {
        std::ostringstream oss;
        oss << "Could not execute: " << commandLine << "\n"
            << "in working directory: " << pWorkingDir << "\n"
            << "Error code: 0x" << std::hex << ::GetLastError();
        mResult = oss.str();
    }
    delete[] pWorkingDir;
    if (success)
    {
        DWORD exitCode = 0;
        ::GetExitCodeProcess(pi.hProcess, &exitCode);
        if (exitCode == STILL_ACTIVE)
        {
            ::WaitForInputIdle(pi.hProcess, static_cast<DWORD>(mTimeout * 1e3));
            ::GetExitCodeProcess(pi.hProcess, &exitCode);
        }
        if (isScript)
        {
            success = (exitCode == 0);
            if (!success)
                mResult = "Execution of script \"" + inExecutable + "\" failed";
        }
        else if (exitCode != STILL_ACTIVE)
        {
            success = (exitCode == 0);
            if (!success)
                mResult = "Application initialization failed";
        }
        ::CloseHandle(pi.hProcess);
        ::CloseHandle(pi.hThread);
    }
    return success;
}
#else  // _WIN32
bool BCI2000Connection::StartExecutable(const std::string &inExecutable, const std::string &inOptions)
{
    mResult.clear();
    std::string executable = inExecutable;
#if __APPLE__
    struct stat s;
    if (::stat(executable.c_str(), &s) && errno == ENOENT) // executable does not exist
    {
        // Extend the path to point into a presumed bundle, and check whether it exists.
        std::string pathIntoBundle = executable + ".app/Contents/MacOS/";
        size_t pos = executable.find_last_of("/");
        if (pos == std::string::npos)
            pathIntoBundle += executable;
        else
            pathIntoBundle += executable.substr(pos + 1);
        if (!::stat(pathIntoBundle.c_str(), &s))
            executable = pathIntoBundle;
    }
#endif // __APPLE__
    std::string commandLine = "\"" + executable + "\" " + inOptions + " &";
    size_t pos = executable.find_last_of("/");
    if (pos != std::string::npos)
    {
        std::string workingDir = executable.substr(0, pos + 1);
        commandLine = "cd \"" + workingDir + "\" && " + commandLine;
    }
    bool success = (0 == ::system(commandLine.c_str()));
    if (!success)
        mResult = "Could not execute: " + commandLine;
    return success;
}
#endif // _WIN32

#if _WIN32
namespace {

bool IsLocalAddress(const std::string& address)
{
    return address.find("localhost:") == 0 || address.find("127.0.0.1:") == 0;
}

bool IsOperatorRunningLocally()
{
    HANDLE mutex = ::OpenMutexA(SYNCHRONIZE, FALSE, "Operator");
    if (mutex != NULL)
        ::CloseHandle(mutex);
    return mutex != NULL;
}

} // namespace

#endif
