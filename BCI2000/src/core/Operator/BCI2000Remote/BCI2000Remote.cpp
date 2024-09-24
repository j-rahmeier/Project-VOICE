/////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000Remote.cpp 7298 2023-04-04 15:06:36Z mellinger $
// Authors juergen.mellinger@uni-tuebingen.de
// Description: A class that allows remote control of BCI2000.
//   Does not depend on the BCI2000 framework except for
//   src/shared/utils/Lib/sockstream and src/shared/utils/Lib/selfpipe.
//   On error, a function returns "false", and provides an error
//   message in the Result() property inherited from BCI2000Connection.
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
/////////////////////////////////////////////////////////////////////////////
#include "BCI2000Remote.h"
#include <fstream>
#include <sstream>
#include <thread>

#if _WIN32
#define stricmp _stricmp
#endif

BCI2000Remote &BCI2000Remote::SubjectID(const std::string &inSubjectID)
{
    mSubjectID = inSubjectID;
    if (!inSubjectID.empty())
        SimpleCommand("set parameter SubjectName \"" + EscapeSpecialChars(mSubjectID) + "\"");
    return *this;
}

BCI2000Remote &BCI2000Remote::SessionID(const std::string &inSessionID)
{
    mSessionID = inSessionID;
    if (!inSessionID.empty())
        SimpleCommand("set parameter SubjectSession \"" + EscapeSpecialChars(mSessionID) + "\"");
    return *this;
}

BCI2000Remote &BCI2000Remote::DataDirectory(const std::string &inDataDirectory)
{
    mDataDirectory = inDataDirectory;
    if (!inDataDirectory.empty())
        SimpleCommand("set parameter DataDirectory \"" + EscapeSpecialChars(mDataDirectory) + "\"");
    return *this;
}

bool BCI2000Remote::StartupModules(const std::vector<std::string> &inModules)
{
    Execute("shutdown system", 0);
    bool success = WaitForSystemState("Idle");
    if (success)
    {
        std::ostringstream startupCommand;
        int port = 4000;
        startupCommand << "startup system localhost ";
        for (size_t i = 0; i < inModules.size(); ++i)
            startupCommand << "*:" << port++ << " ";
        Execute(startupCommand.str(), 0);
        success = (Result().find("not") == std::string::npos);
    }
    if (success)
    {
        std::ostringstream errors;
        for (size_t i = 0; i < inModules.size(); ++i)
        {
            int code = 0;
            Execute("start executable " + inModules[i] + " --local", &code);
            if (code)
                errors << "\n" << inModules[i] << " returned " << code;
            else if (!Result().empty())
                errors << "\n" << Result();
        }
        success = errors.str().empty();
        if (!success)
            mResult = "Could not start modules: " + errors.str();
    }
    if (success)
        success = WaitForSystemState("Initialization");
    Execute("WARN \"Using Signal Source:\" " + inModules[0], 0);
    return success;
}

bool BCI2000Remote::SetConfig()
{
    SubjectID(mSubjectID);
    SessionID(mSessionID);
    DataDirectory(mDataDirectory);
    Execute("capture messages none warnings errors", 0);
    std::string tempResult;
    if (SimpleCommand("set config"))
        WaitForSystemState("Resting|Initialization");
    else
        tempResult = Result();
    Execute("capture messages none", 0);
    Execute("get system state", 0);
    bool success = !::stricmp("Resting", Result().c_str());
    Execute("flush messages", 0);
    if (!tempResult.empty())
        mResult = tempResult + '\n' + mResult;
    return success;
}

bool BCI2000Remote::Start()
{
    bool success = true;
    Execute("get system state", 0);
    std::string state = Result();
    if (!::stricmp(state.c_str(), "Running"))
    {
        mResult = "System is already in running state";
        success = false;
    }
    else if (::stricmp(state.c_str(), "Resting") && ::stricmp(state.c_str(), "Suspended"))
    {
        success = SetConfig();
    }
    if (success)
        success = SimpleCommand("start system");
    return success;
}

bool BCI2000Remote::Stop()
{
    Execute("get system state", 0);
    bool success = !::stricmp(Result().c_str(), "Running");
    if (!success)
        mResult = "System is not in running state";
    if (success)
        success = SimpleCommand("stop system");
    return success;
}

bool BCI2000Remote::SetParameter(const std::string &inName, const std::string &inValue)
{
    return SimpleCommand("set parameter \"" + inName + "\" \"" + EscapeSpecialChars(inValue) + "\"");
}

bool BCI2000Remote::GetParameter(const std::string &inName, std::string &outValue)
{
    int exitCode = 1;
    Execute("is parameter \"" + inName + "\"", &exitCode);
    bool success = (exitCode == 0);
    if (success)
    {
        Execute("get parameter \"" + inName + "\"", 0);
        outValue = Result();
    }
    return success;
}

bool BCI2000Remote::LoadParametersLocal(const std::string &inFileName)
{
    std::ifstream file(inFileName.c_str());
    bool success = file.is_open();
    if (!success)
        mResult = "Could not open file \"" + inFileName + "\" for input.";
    if (success)
    {
        std::string line;
        while (std::getline(file, line))
            Execute("set parameter " + EscapeSpecialChars(line), 0);
    }
    return success;
}

bool BCI2000Remote::LoadParametersRemote(const std::string &inFileName)
{
    return SimpleCommand("load parameters \"" + inFileName + "\"");
}

bool BCI2000Remote::AddStateVariable(const std::string &inName, unsigned int inBitWidth, double inInitialValue)
{
    std::ostringstream oss;
    oss << "add state \"" << inName << "\" " << inBitWidth << " " << inInitialValue;
    return SimpleCommand(oss.str());
}

bool BCI2000Remote::SetStateVariable(const std::string &inStateName, double inValue)
{
    std::ostringstream value;
    value << inValue;
    Execute("set state \"" + inStateName + "\" " + value.str(), 0);
    return Result().empty();
}

bool BCI2000Remote::GetStateVariable(const std::string &inStateName, double &outValue)
{
    Execute("get state \"" + inStateName + "\"", 0);
    return !!(std::istringstream(Result()) >> outValue);
}

bool BCI2000Remote::AddEventVariable(const std::string& inName, unsigned int inBitWidth, double inInitialValue)
{
    std::ostringstream oss;
    oss << "add event \"" << inName << "\" " << inBitWidth << " " << inInitialValue;
    return SimpleCommand(oss.str());
}

bool BCI2000Remote::SetEventVariable(const std::string& inStateName, double inValue)
{
    std::ostringstream value;
    value << inValue;
    Execute("set event \"" + inStateName + "\" " + value.str(), 0);
    return Result().empty();
}

bool BCI2000Remote::PulseEventVariable(const std::string& inStateName, double inValue)
{
    std::ostringstream value;
    value << inValue;
    Execute("pulse event \"" + inStateName + "\" " + value.str(), 0);
    return Result().empty();
}

bool BCI2000Remote::GetEventVariable(const std::string& inStateName, double& outValue)
{
    Execute("get event \"" + inStateName + "\"", 0);
    return !!(std::istringstream(Result()) >> outValue);
}

bool BCI2000Remote::GetSystemState(std::string &outResult)
{
    bool success = Execute("get system state", 0);
    if (success)
        outResult = Result();
    else
        outResult.clear();
    return success;
}

bool BCI2000Remote::GetControlSignal(int inChannel, int inElement, double &outValue)
{
    std::ostringstream oss;
    oss << "get signal(" << inChannel << "," << inElement << ")";
    Execute(oss.str(), 0);
    return !!(std::istringstream(Result()) >> outValue);
}

bool BCI2000Remote::SetScript(const std::string &inEvent, const std::string &inScript)
{
    return SimpleCommand("set script " + inEvent + "\"" + EscapeSpecialChars(inScript) + "\"");
}

bool BCI2000Remote::GetScript(const std::string &inEvent, std::string &outScript)
{
    Execute("get script " + inEvent, 0);
    bool success = true;
    const std::string tag = "scripting event:";
    size_t pos = Result().find(tag);
    if (pos != std::string::npos)
    {
        std::istringstream iss(Result().substr(pos + tag.length()));
        std::string event;
        iss >> std::ws >> event;
        std::string::const_iterator i = event.begin();
        std::string::iterator j = event.begin();
        while (i != event.end())
            if (*i != '\"')
                *j++ = *i++;
            else
                ++i;
        event.erase(j, event.end());
        if (inEvent.find(event) != std::string::npos)
            success = false;
    }
    if (success)
        outScript = Result();
    return success;
}

std::string BCI2000Remote::EncodeValue(const std::string &inValue)
{
    return EscapeSpecialChars(inValue, "");
}

bool BCI2000Remote::WaitForSystemState(const std::string &inState)
{
    std::ostringstream timeout;
    timeout << Timeout() - 1;
    return SimpleCommand("wait for " + inState + " " + timeout.str());
}

bool BCI2000Remote::SimpleCommand(const std::string &inCommand)
{
    Execute(inCommand, 0);
    return Result().empty() || ::atoi(Result().c_str());
}

std::string BCI2000Remote::EscapeSpecialChars(const std::string &inString, const std::string &inExcept)
{
    // Encode characters that might be special to the ScriptInterpreter shell.
    std::ostringstream oss;
    static const std::string escapeThese = "#\"${}`&|<>;\n";
    for (const char& c : inString)
    {
        uint8_t d = static_cast<uint8_t>(c);
        bool doEscape = d <= 0x20 || d >= 0x80 || escapeThese.find(c) != std::string::npos;
        doEscape = doEscape && inExcept.find(c) == std::string::npos;
        if (doEscape)
            oss << "%" << std::hex << (d >> 4) << (d & 0xf);
        else
            oss.put(c);
    }
    return oss.str();
}

BCI2000Remote::Watch* BCI2000Remote::NewWatch(const std::string& expr, CallbackFn callback, void* data)
{
    std::string addr;
    Execute("ADD WATCH " + expr, nullptr, &addr);
    if (addr.empty())
        return nullptr;
    return new Watch(this, addr, callback, data);
}

BCI2000Remote::Watch::Watch(BCI2000Remote* parent, const std::string &address, BCI2000Remote::CallbackFn fn, void* data)
: mpParent(parent), mAddress(address), mCallback(fn), mpCallbackData(data),
  mThread(&BCI2000Remote::Watch::ThreadFunc, this)
{
}

BCI2000Remote::Watch::~Watch()
{
    mAbort.write("quit", 4);
    mThread.join();
    mpParent->Execute("DELETE WATCH " + mAddress, nullptr);
}

void BCI2000Remote::Watch::ThreadFunc()
{
    receiving_udpsocket receiver(mAddress);
    const int datagramMax = 1 << 16;
    std::vector<char> recbuf(datagramMax);
    fdio::fd_object* objects[] = { &receiver, &mAbort }, * readable = nullptr;
    while (readable != &mAbort) {
        readable = fdio::fd_object::select(objects, 2, nullptr, 0, fdio::fd_object::infinite_timeout);
        if (readable == &receiver) {
            receiver.read(recbuf.data(), recbuf.size());
            if (mCallback)
                mCallback(mpCallbackData, recbuf.data());
        }
    }
}
