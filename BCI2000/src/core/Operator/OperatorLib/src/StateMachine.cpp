////////////////////////////////////////////////////////////////////////////////
// $Id: StateMachine.cpp 8248 2024-07-11 15:14:54Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A class that represents the operator module's state machine.
//   The state machine is responsible for
//   - maintaining a global BCI2000 system state (state of operation),
//   - maintaining global parameter and state lists,
//   - handling core module connections,
//   - maintaining visualization object properties,
//   - processing state change requests,
//   - triggering callback events to display text messages, or to visualize data.
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
#include "StateMachine.h"

#include "BCIException.h"
#include "BCIStream.h"
#include "BCI_OperatorLib.h"
#include "Debugging.h"
#include "EnvVariable.h"
#include "FilePath.h"
#include "FileUtils.h"
#include "GenericVisualization.h"
#include "Label.h"
#include "ParamRef.h"
#include "ProtocolVersion.h"
#include "RunManager.h"
#include "ScriptInterpreter.h"
#include "SignalProperties.h"
#include "StateVector.h"
#include "Status.h"
#include "SysCommand.h"
#include "WildcardMatch.h"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>

StateMachine::StateMachine() : mSystemState(Idle), mEventLink(*this)
{
    Init();

    std::string path;
    EnvVariable::Get("PATH", path);
    path = FileUtils::InstallationDirectoryS() + FilePath::PathSeparator + path;
    EnvVariable::Set("PATH", path);
    EnvVariable::Set("BCI2000LAUNCHDIR", FileUtils::InstallationDirectoryS());
    EnvVariable::Set("BCI2000BINARY", FileUtils::ExecutablePath());
}

void StateMachine::Init()
{
    CloseConnections();
    DataLock lock(this);
    mParameters.Clear();
    mStates.Clear();
    mIntroducedRandomSeed = false;
    mPreviousRandomSeed.clear();
    mStateVector = StateVector();
    mControlSignal = GenericSignal();
    mVisualizations.clear();
    mLastWatchEvalDiscont = true;
    CheckWatches(true);
    mFilePart = 0;
}

bool StateMachine::Startup(const char *inArguments)
{
    if (SystemState() != Idle)
        return false;
    if (Thread::Running())
        return false;

    DataLock lock(this);
    mSystemState = Transition;

    const VersionInfo &info = VersionInfo::Current;
    mParameters.Add(
        "System:Version matrix /OperatorVersion= { Framework Revision Build Config Protocol } 1 Operator % %"
        " % % % // operator module version information (noedit)(readonly)");
    Param &p = mParameters.ByPath("/OperatorVersion");
    p.Value("Framework") = info[VersionInfo::VersionID];
    std::string revision = info[VersionInfo::Revision] + ", " + info[VersionInfo::SourceDate];
    if (revision.front() == ',')
        revision = revision.substr(2);
    p.Value("Revision") = revision;
    p.Value("Build") = info[VersionInfo::Build];
    p.Value("Config") = info[VersionInfo::Config];
    std::ostringstream oss;
    oss << ProtocolVersion::Current();
    p.Value("Protocol") = oss.str();

    mParameters.Add("System:Protocol int /AutoConfig= 1"
                    " 1 0 1 // Use AutoConfig protocol extension (boolean)");

    mParameters.Add("System:Protocol int /OperatorBackLink= 1"
                    " 1 0 1 // Send final state and signal information to Operator (boolean)");

    mParameters.Add( // added here so it will be first in list
        "System:Core%20Connections string /OperatorIP= 127.0.0.1"
        " % % % // operator module IP address (noedit)(readonly)");

    mIntroducedRandomSeed = false;
    mLocalAddress.clear();
    mpSourceModule = 0;

    std::string arguments = inArguments ? inArguments : "";
    if (arguments.empty())
        arguments = "*";
    std::istringstream argstream(arguments);
    std::string ip;
    argstream >> ip;
    std::string modules;
    std::getline(argstream, modules, '\0');

    size_t start = 0;
    while (start < modules.length() && isspace(modules[start]))
        start++;
    modules = modules.substr(start);
    std::string token = "--SystemLogFile=";
    std::string sysLogFilePath;
    if (modules.substr(0, token.length()) == token)
    {
        modules = modules.substr(token.length());
        size_t stop = 0;
        while (stop < modules.length() && !isspace(modules[stop]))
            stop++;
        sysLogFilePath = modules.substr(0, stop);
        while (stop < modules.length() && isspace(modules[stop]))
            stop++;
        modules = modules.substr(stop);
    }

    if (modules.empty())
        modules = "SignalSource:4000 SignalProcessing:4001 Application:4002";
    std::istringstream iss(modules);
    while (!iss.eof())
    {
        std::string name;
        std::getline(iss >> std::ws, name, ':');
        std::string port;
        iss >> port >> std::ws;
        std::string address = ip + ":" + port;
        CoreClient *p = new CoreClient(*this, name, address, mConnections.Size() + 1);
        p->SetUserData(p);
        mConnections.Add(p);
        if (mLocalAddress.empty())
        {
            mpSourceModule = p;
            mLocalAddress = "localhost:" + port;
            mEventLink.Open(ip, ::atoi(port.c_str()));
        }
    }
    if (sysLogFilePath.length())
    {
        std::string parentDir = FileUtils::ExtractDirectory(sysLogFilePath);
        FileUtils::MakeDirectory(parentDir, true);
        mSystemLog.close();
        mSystemLog.clear();
        mSystemLog.open(sysLogFilePath, std::ios_base::out | std::ios_base::app);
        if (!mSystemLog.is_open())
            bciwarn << "Cannot write system log: " << sysLogFilePath;
    }
    else
        mSystemLog.close();

    mSystemState = Idle;
    TriggerEvent(BCI_OnStartup);
    if (bcierr__.Empty() && IsConsistentState(WaitingForConnection))
        EnterState(WaitingForConnection);
    bool success = bcierr__.Empty() && SystemState() == WaitingForConnection;
    if (success)
        Thread::Start();
    else
        CloseConnections();
    bcierr__.Clear();
    return success;
}

void StateMachine::CloseConnections()
{
    mLocalAddress = "";
    mpSourceModule = 0;
    mConnections.Clear();
    mEventLink.Close();
}

StateMachine::~StateMachine()
{
    // The StateMachine destructor must be called from the same (main) thread that
    // called its constructor.
    WithLock(mListeners)
    {
        for (auto pListener : mListeners)
        {
            pListener->Abort();
            pListener->RequestTermination();
        }
    }
    WithLock(mWatches)
    {
        while (!mWatches.Empty())
            delete mWatches.First();
    }
    mConnections.AbortWaiting();
    while (CheckPendingCallback())
        ThreadUtils::SleepForMs(1);
    while (Thread::Running())
    {
        ThreadUtils::SleepForMs(10);
        while (CheckPendingCallback())
            ThreadUtils::SleepForMs(1);
    }
    bool empty = false;
    while (!empty)
    {
        WithLock(mListeners)
            empty = mListeners.empty();
        if (!empty)
            ThreadUtils::SleepForMs(1);
    }
    Thread::TerminateAndWait();
}

// Send a state message containing a certain state value to the SignalSource module.
// This function is public because it is called from the CommandInterpreter class.
bool StateMachine::SetStateValue(const char *inName, State::ValueType inValue)
{
    // We call EnterState() from here to have a consistent behavior if
    // UpdateState() is called for "Running" from a script or a button.
    if (!::stricmp(inName, "Running"))
    {
        switch (mSystemState & ~StateFlags)
        {
        case Resting:
        case Suspended:
            if (inValue)
                EnterState(RunningInitiated);
            break;
        case Running:
            if (!inValue)
                EnterState(SuspendInitiated);
            break;
        default:
            return false;
        }
    }

    DataLock lock(this);
    if (!mStates.Exists(inName))
        return false;
    else
    {
        mStates.SetStateValue(inName, inValue);
        CheckWatches(true);
        if (!mpSourceModule || !mpSourceModule->Send(mStates.ByName(inName)))
            return false;
    }
    return true;
}

State::ValueType StateMachine::GetStateValue(const char *inName, int inSample) const
{
    DataLock lock(this);
    if (!mStates.Exists(inName))
        return 0;
    return mStateVector.StateValue(inName, inSample);
}

bool StateMachine::SetEvent(const char *inName, State::ValueType inValue, int inDuration)
{
    if (!mStates.Exists(inName))
        return false;
    if (mStates.ByName(inName).Kind() != State::EventKind)
        return false;
    if (mSystemState != StateMachine::Running)
        return false;
    ::Lock_<EventLink> lock(mEventLink);
    if (!mEventLink.Connected())
        return false;
    mEventLink << inName << ' ' << inValue << ' ' << inDuration << std::endl;
    return true;
}

void StateMachine::MaintainDebugLog()
{
    // Enable debug logging if requested.
    bool writeLog = false;
    if (mParameters.Exists("/DebugLevel") && ::atoi(mParameters.ByPath("/DebugLevel").Value().c_str()) != 0)
        writeLog = true;
    if (writeLog && mParameters.Exists("/DebugLog"))
    {
        std::string value = mParameters.ByPath("/DebugLog").Value();
        if (!value.empty() && ::atoi(value.c_str()) == 0)
            writeLog = false;
    }
    if (writeLog)
    {
        std::string filePath = RunManager::CurrentSession(mParameters) + ".dbg",
                    dir = FileUtils::ExtractDirectory(filePath);
        FileUtils::MakeDirectory(dir, true);
        mDebugLog.close();
        mDebugLog.clear();
        mDebugLog.open(filePath, std::ios_base::out | std::ios_base::app);
        if (!mDebugLog.is_open())
            bcierr << "Cannot write debug log: " << filePath;
    }
    else
        mDebugLog.close();
}

void StateMachine::DebugWarning()
{
    const std::string suffix = "Version", row = "Build", pattern = "*\\<debug\\>*", sep = ", ";
    std::string debugList;
    for (int i = 0; i < mParameters.Size(); ++i)
    {
        const Param &p = mParameters.ByIndex(i);
        ptrdiff_t pos = p.Name().length() - suffix.length();
        if (pos > 0 && p.Name().substr(pos) == suffix)
        {
            std::string module = p.Name().substr(0, pos);
            if (p.RowLabels().Exists(row) && WildcardMatch(pattern, p.Value(row, 0), false))
                debugList += sep + module;
        }
    }
    if (!debugList.empty())
        bciwarn__ << "The following modules were built in debug mode: " << debugList.substr(sep.length()) << ".\n"
                  << "Debug mode is useful for development, but affects performance"
                  << " -- do not use debug builds for experiments!\n";
}

bool StateMachine::SetConfig()
{
    bool result = false;
    switch (mSystemState & ~StateFlags)
    {
    case Information:
    case Initialization:
    case Resting:
    case Suspended:
        result = true;
        EnterState(SetConfigIssued);
        break;

    default:;
    }
    return result;
}

bool StateMachine::StartRun()
{
    bool result = false;
    switch (mSystemState & ~StateFlags)
    {
    case Resting:
    case Suspended:
        result = SetStateValue("Running", true);
        break;

    default:;
    }
    return result;
}

bool StateMachine::StopRun()
{
    bool result = false;
    switch (mSystemState)
    {
    case Running:
        result = SetStateValue("Running", false);
        break;

    default:;
    }
    return result;
}

// Terminate the state machine thread, which will shut down connections.
bool StateMachine::Shutdown()
{
    bool result = (mSystemState != Idle);
    if (result)
    {
        StopRun();
        Terminate(Time::Interval(0));
    }
    return result;
}

bool StateMachine::Reset()
{
    bool result = (mSystemState == Idle);
    if (result)
        Init();
    return result;
};

void StateMachine::ParameterChange()
{
    switch (mSystemState)
    {
    case Resting:
    case Suspended:
        mSystemState |= ParamsModified;
        break;
    }
}

StateMachine::ConnectionInfo StateMachine::Info(int i) const
{
    if (!Terminating())
        if (i < mConnections.Size())
            return mConnections[i]->Info();
    return ConnectionInfo();
}

void StateMachine::SendNextModuleInfo()
{
    for (int i = 0; i < mConnections.Size(); ++i)
    {
        ConnectionInfo receiver = Info(i), next = Info((i + 1) % mConnections.Size());
        if (receiver.Version.Provides(ProtocolVersion::NextModuleInfo))
        {
            ParamList p;
            p.Add("system string NextModuleAddress= " + next.ListeningIP + ":" + next.ListeningPort + " // (readonly)");
            CoreClient &c = *mConnections[i];
            c.Send(next.Version);
            c.Send(p);
            c.Send(SysCommand::EndOfParameter);
        }
    }
}

void StateMachine::PruneParameters()
{
    mParameters.Prune();
}

void StateMachine::BroadcastParameters()
{
    for (int i = 0; i < mConnections.Size(); ++i)
        if (mConnections[i]->Send(mParameters))
            ThreadUtils::Idle();
    mParameters.Unchanged();
}

void StateMachine::BroadcastEndOfParameter()
{
    for (int i = 0; i < mConnections.Size(); ++i)
        mConnections[i]->Send(SysCommand::EndOfParameter);
}

void StateMachine::BroadcastParameterChanges()
{
    ParamList changedParams;
    for (int i = 0; i < mParameters.Size(); ++i)
    {
        if (mParameters.ByIndex(i).Changed())
        {
            mParameters.ByIndex(i).Unchanged();
            changedParams.Add(mParameters.ByIndex(i));
        }
    }
    if (!changedParams.Empty())
    {
        for (int i = 0; i < mConnections.Size(); ++i)
            if (mConnections[i]->Send(changedParams))
                ThreadUtils::Idle();
    }
}

void StateMachine::BroadcastStates()
{
    for (int i = 0; i < mConnections.Size(); ++i)
        if (mConnections[i]->Send(mStates))
            ThreadUtils::Idle();
}

void StateMachine::BroadcastEndOfState()
{
    for (int i = 0; i < mConnections.Size(); ++i)
        mConnections[i]->Send(SysCommand::EndOfState);
}

void StateMachine::InitializeStateVector()
{
    mParameters.Add("System:State%20Vector"
                    " int /StateVectorLength= 0 16 % % "
                    "// length of the state vector in bytes (noedit)(readonly)");
    mStates.AssignPositions();
    mStateVector = ::StateVector(mStates);
    std::ostringstream length;
    length << mStateVector.Length();
    mParameters.ByPath("/StateVectorLength").Value() = length.str().c_str();
}

void StateMachine::InitializeModules()
{
    mAutoParameters.Clear();
    mpSourceModule->Send(SignalProperties(0, 0));
}

// Here we list allowed state transitions, and associated actions.
#define TRANSITION(a, b) ((int(a) << 8) | int(b))
void StateMachine::EnterState(SysState inState)
{
    bool execute = false;
    int transition = 0;
    WithLock(mTransitionLock)
    {
        int curState = mSystemState & ~StateFlags;
        if (curState != Fatal || inState == Fatal)
        {
            execute = true;
            transition = TRANSITION(curState, inState);
            mSystemState = Transition;
            PerformTransition(transition);
            switch (inState)
            {
            case Initialization:
            case SetConfigIssued:
            case RunningInitiated:
                SetConnectionState(inState);
                break;
            }
            mSystemState = inState;
        }
    }
    if (execute)
        ExecuteTransitionCallbacks(transition);
}

void StateMachine::PerformTransition(int inTransition)
{
    DataLock lock(this);
    switch (inTransition)
    {
    case TRANSITION(Idle, Idle):
    case TRANSITION(Idle, WaitingForConnection):
    case TRANSITION(WaitingForConnection, Publishing):
    case TRANSITION(Publishing, Publishing):
        break;

    case TRANSITION(Publishing, Information):
        DebugWarning();
        Randomize();
        mEventLink.ConfirmConnection();
        PruneParameters();
        break;

    case TRANSITION(Information, SetConfigIssued):
        MaintainDebugLog();
        InitializeStateVector();
        SendNextModuleInfo();
        BroadcastParameters();
        BroadcastEndOfParameter();
        BroadcastStates();
        BroadcastEndOfState();
        InitializeModules();
        break;

    case TRANSITION(Initialization, SetConfigIssued):
    case TRANSITION(Resting, SetConfigIssued):
    case TRANSITION(Suspended, SetConfigIssued):
        MaintainDebugLog();
        BroadcastParameters();
        BroadcastEndOfParameter();
        InitializeModules();
        break;

    case TRANSITION(SetConfigIssued, Resting):
    case TRANSITION(SetConfigIssued, Initialization):
        InitializeMeasurementUnits();
        break;

    case TRANSITION(Resting, RunningInitiated):
    case TRANSITION(Suspended, RunningInitiated):
        RandomizationWarning();
        MaintainDebugLog();
        break;

    case TRANSITION(RunningInitiated, Running):
    case TRANSITION(Running, SuspendInitiated):
    case TRANSITION(SuspendInitiated, SuspendInitiated):
        break;

    case TRANSITION(SuspendInitiated, Suspended):
        Randomize();
        PruneParameters();
        BroadcastParameterChanges();
        break;

    case TRANSITION(Initialization, Idle):
    case TRANSITION(SetConfigIssued, Idle):
    case TRANSITION(Resting, Idle):
    case TRANSITION(RunningInitiated, Idle):
    case TRANSITION(Running, Idle):
    case TRANSITION(SuspendInitiated, Idle):
    case TRANSITION(Suspended, Idle):
        // Send a system command 'Reset' to the SignalSource module.
        mpSourceModule->Send(SysCommand::Reset);
        /* fall through */
    case TRANSITION(WaitingForConnection, Idle):
    case TRANSITION(Publishing, Idle):
    case TRANSITION(Information, Idle):
        mDebugLog.close();
        CloseConnections();
        break;

    case TRANSITION(Idle, Fatal):
    case TRANSITION(WaitingForConnection, Fatal):
    case TRANSITION(Publishing, Fatal):
    case TRANSITION(Information, Fatal):
    case TRANSITION(Initialization, Fatal):
    case TRANSITION(SetConfigIssued, Fatal):
    case TRANSITION(Resting, Fatal):
    case TRANSITION(RunningInitiated, Fatal):
    case TRANSITION(Running, Fatal):
    case TRANSITION(SuspendInitiated, Fatal):
    case TRANSITION(Suspended, Fatal):
    case TRANSITION(Transition, Fatal):
    case TRANSITION(Fatal, Fatal):
        break;

    default:
        SuggestDebugging << "Unexpected system state transition: " << (inTransition >> 8) << " -> "
                         << (inTransition & 0xff);
    }
}

void StateMachine::ExecuteTransitionCallbacks(int inTransition)
{
    switch (inTransition)
    {
    case TRANSITION(Idle, WaitingForConnection):
        LogMessage(BCI_OnLogMessage, "BCI2000 Started");
        break;

    case TRANSITION(Publishing, Information):
        TriggerEvent(BCI_OnConnect);
        break;

    case TRANSITION(Information, SetConfigIssued):
    case TRANSITION(Initialization, SetConfigIssued):
    case TRANSITION(Resting, SetConfigIssued):
    case TRANSITION(Suspended, SetConfigIssued):
        LogMessage(BCI_OnLogMessage, "Operator set configuration");
        break;

    case TRANSITION(SetConfigIssued, Resting):
        TriggerEvent(BCI_OnSetConfig);
        break;

    case TRANSITION(Resting, RunningInitiated):
        TriggerEvent(BCI_OnStart);
        LogMessage(BCI_OnLogMessage, "Operator started operation");
        break;

    case TRANSITION(Suspended, RunningInitiated):
        TriggerEvent(BCI_OnResume);
        LogMessage(BCI_OnLogMessage, "Operator resumed operation");
        break;

    case TRANSITION(RunningInitiated, Running):
        TriggerEvent(BCI_OnStartRun, CurrentRunFileName().c_str());
        InitializeFilePart();
        break;

    case TRANSITION(Running, SuspendInitiated):
        LogMessage(BCI_OnLogMessage, "Operation suspended");
        break;

    case TRANSITION(SuspendInitiated, Suspended):
        TriggerEvent(BCI_OnSuspend);
        DeinitializeFilePart();
        break;

    case TRANSITION(WaitingForConnection, Idle):
    case TRANSITION(Publishing, Idle):
    case TRANSITION(Information, Idle):
    case TRANSITION(Initialization, Idle):
    case TRANSITION(SetConfigIssued, Idle):
    case TRANSITION(Resting, Idle):
    case TRANSITION(RunningInitiated, Idle):
    case TRANSITION(Running, Idle):
    case TRANSITION(SuspendInitiated, Idle):
    case TRANSITION(Suspended, Idle):
        TriggerEvent(BCI_OnShutdown);
        LogMessage(BCI_OnLogMessage, "Operator shut down connections");
        break;
    }
    ExecuteCallback(BCI_OnSystemStateChange);
    DataLock lock(this);
    CheckWatches(true);
}

int StateMachine::OnExecute()
{
    while (mConnections.Wait(InfiniteTimeout))
    {
        mConnections.ProcessMessages();
        ExecuteCallback(BCI_OnCoreInput);
    }
    while (mConnections.Wait(0))
    {
        mConnections.ProcessMessages();
        ExecuteCallback(BCI_OnCoreInput);
    }
    EnterState(Idle);
    while (CallbacksPending())
        ThreadUtils::Idle();
    return 0;
}

std::string StateMachine::SuggestUDPAddress(const std::string &inAddressHint) const
{
    sending_udpsocket socket(LocalAddress().c_str());
    std::string result, local = socket.local_address(), local2, hint = inAddressHint.empty() ? local : inAddressHint;
    socket.open(socket.local_ip(), socket.local_port() + 1);
    local2 = socket.local_address();

    std::string ip = "";
    uint16_t port = 0;
    std::istringstream iss(hint);
    if (std::getline(iss, ip, ':') >> port)
    {
        int maxPort = port + 250;
        for (; port < maxPort && result.empty(); ++port)
        {
            socket.open(ip.c_str(), port);
            if (socket.is_open())
            {
                std::string a = socket.local_address();
                bool ok = a != local && a != local2 && Watches().SelectByAddress(a).Empty();
                if (ok)
                    result = a;
            }
        }
    }
    return result;
}

bool StateMachine::CheckInitializeVis(const std::string &inSourceID, const std::string &inKind)
{
    bool kindDiffers = (mVisualizations[inSourceID].Kind() != inKind);
    if (kindDiffers)
    {
        mVisualizations[inSourceID].Kind() = inKind;
        ExecuteCallback(BCI_OnInitializeVis, inSourceID.c_str(), inKind.c_str());
    }
    return kindDiffers;
}

void StateMachine::LogMessage(int inCallbackID, const std::string &inMessage)
{
    std::string separator;
    switch (inCallbackID)
    {
    case BCI_OnDebugMessage:
    case BCI_OnLogMessage:
        separator = ": ";
        break;
    case BCI_OnWarningMessage:
        separator = ": Warning: ";
        break;
    case BCI_OnErrorMessage:
        separator = ": Error: ";
        break;
    default:
        throw std_logic_error << "Unknown log message callback ID: " << inCallbackID;
    }
    ExecuteCallback(inCallbackID, inMessage.c_str());
    time_t t = ::time(NULL);
    const size_t BUFFER_SIZE = 100;
    char buf[BUFFER_SIZE];
    ::strftime(buf, BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", localtime(&t));
    mDebugLog << buf << separator << inMessage << std::endl;
    mSystemLog << buf << separator << inMessage << std::endl;
    WithLock(mListeners) for (Listeners::iterator i = mListeners.begin(); i != mListeners.end(); ++i)
        (*i)->HandleLogMessage(inCallbackID, inMessage);
}

void StateMachine::TriggerEvent(int inCallbackID, const char* inArg)
{
    const char *pName = ScriptEvents::Name(inCallbackID);
    if (!pName)
        throw std_logic_error << "Unknown operator event callback ID: " << inCallbackID;

    if (inArg)
        ExecuteCallback(inCallbackID, inArg);
    else
        ExecuteCallback(inCallbackID);

    std::string script = mScriptEvents.Get(inCallbackID);
    if (!script.empty())
    {
        LogMessage(BCI_OnLogMessage, "Executing " + std::string(pName) + " script ...");
        std::vector<std::string> args;
        if (inArg)
            args.push_back(inArg);
        ScriptInterpreter(*this).ExecuteAsynchronously(script, pName, args);
    }
}

void StateMachine::Randomize()
{
    // Add a RandomSeed parameter if it's not present already.
    Param p1 = Param::fromDefinition(
        "System:Randomization"
        " int /RandomSeed= 0 0 % % "
        " // seed for the BCI2000 pseudo random number generator (readonly)"); // should be "nowrite" instead of
                                                                               // "readonly"
    if (mParameters.Exists("/RandomSeed"))
        p1.Value() = mParameters.ByPath("/RandomSeed").Value();
    else
        mIntroducedRandomSeed = true;
    mParameters.Add(p1);
    Param p2 = Param::fromDefinition("System:Randomization"
                                     " int /RandomizationWarning= 1 1 0 1 "
                                     " // warn if subsequent Runs have identical RandomSeed value (boolean)");
    if (mParameters.Exists("/RandomizationWarning"))
        p2.Value() = mParameters.ByPath("/RandomizationWarning").Value();
    mParameters.Add(p2);

    if (mIntroducedRandomSeed)
    {
        ::srand(static_cast<unsigned int>(::time(NULL)));
        for (int n = 0; n < ::rand() % 10; ++n)
            ::rand(); // MSVC 2008 initial random number tends to be proportional to seed (!)
        int number = 0;
        while (number == 0 || number == ::atoi(mPreviousRandomSeed.c_str()))
            number = ::rand();
        std::ostringstream oss;
        oss << number;
        mParameters.ByPath("/RandomSeed").Value() = oss.str();
    }
}

void StateMachine::RandomizationWarning()
{
    if (!mIntroducedRandomSeed && ::atoi(mParameters.ByPath("/RandomizationWarning").Value().c_str()) &&
        mParameters.ByPath("/RandomSeed").Value().c_str() == mPreviousRandomSeed && ::atoi(mPreviousRandomSeed.c_str()))
        bciwarn__ << "In the present configuration, the RandomSeed value does not "
                  << "automatically change between runs. "
                  << "Any 'random' behavior, such as randomization of the order of trials, "
                  << "or the generation of noise signals, will be exactly the same "
                  << "on this run as on the previous run.";

    mPreviousRandomSeed = mParameters.ByPath("/RandomSeed").Value().c_str();
}

void StateMachine::OnReceive(CoreConnection &conn, const Status &s)
{
    if (s.Content() == Status::note)
    {
        ExecuteCallback(BCI_OnNoteMessage, s.Message().c_str());
        return;
    }


    const CoreClient &c = *static_cast<CoreClient *>(conn.UserData());
    int kind = 0;
    switch (s.Content())
    {
    case Status::debug:
        kind = BCI_OnDebugMessage;
        break;
    case Status::warning:
        kind = BCI_OnWarningMessage;
        break;
    case Status::error:
        kind = BCI_OnErrorMessage;
        break;
    default:
        kind = BCI_OnLogMessage;
    }
    std::string name = c.Info().Name, msg = s.Message();
    if (kind == BCI_OnLogMessage)
    {
        if (!::stricmp(name.c_str(), msg.substr(0, name.length()).c_str()))
            msg = msg.substr(name.length());
        if (!msg.empty())
        {
            if (!::isspace(*msg.begin()))
                msg = " " + msg;
            std::string::iterator i = msg.begin();
            while (::isspace(*i) && i != msg.end())
                ++i;
            if (i != msg.end())
                *i = ::toupper(*i);
            msg = ":" + msg;
        }
        msg = name + msg;
    }
    else if (mConnections.Size() > 1)
    {
        msg = "[" + name + "] " + msg;
    }
    LogMessage(kind, msg);

    if (s.Content() == Status::error)
    {
        switch (SystemState())
        {
        case Initialization:
            break;
        case SetConfigIssued:
            EnterState(Initialization);
            break;
        default:
            EnterState(Fatal);
        }
    }
}

void StateMachine::OnReceive(CoreConnection &conn, const SysCommand &s)
{
    const CoreClient &c = *static_cast<CoreClient *>(conn.UserData());
    if (s == SysCommand::Reset)
    {
        Shutdown();
    }
    else if (s == SysCommand::Suspend)
    {
        if (SystemState() == Running)
            EnterState(SuspendInitiated);
    }
    else if (s == SysCommand::EndOfParameter)
    {
        /* do nothing */
    }
    else if (s == SysCommand::EndOfState)
    {
        ConnectionInfo info = c.Info();
        // Do major version numbers match?
        if (!info.Version.Matches(ProtocolVersion::Current()))
        {
            std::string older = info.Name, newer = "Operator";
            if (info.Version.MoreRecentThan(ProtocolVersion::Current()))
                std::swap(older, newer);

            bcierr__ << "Protocol version mismatch between Operator and " << info.Name << " module.\n"
                     << "The " << newer << " module appears to be more recent than the " << older << " module.\n\n"
                     << "Please make sure that all modules share the same"
                     << " BCI2000 major version number.\n"
                     << "The operator module's version number is "
                     << mParameters.ByPath("/OperatorVersion").Value("Framework") << ".\n\n"
                     << "BCI2000 will now shut down.";
            Shutdown();
        }
        // Do minor version numbers match such that Operator is most recent?
        else if (info.Version.MoreRecentThan(ProtocolVersion::Current()))
        {
            bcierr__ << "Protocol version mismatch between Operator and " << info.Name << " module.\n"
                     << "Please make sure that the Operator module is at least as recent as the "
                     << "most recent core module.\n"
                     << "BCI2000 will now shut down.";
            Shutdown();
        }
    }
}

void StateMachine::OnReceive(CoreConnection &conn, const Param &inParam)
{
    const CoreClient &inConnection = *static_cast<CoreClient *>(conn.UserData());
    if (SpecialParameter(inConnection, inParam))
        return;

    // Get data file shm name
    if (inParam.Name() == "DataFileShm")
        mDataFileShmName = inParam.Value().AsString();

    switch (SystemState() & ~StateFlags)
    {
    case SetConfigIssued:
        mAutoParameters.ByPath(inParam.Path()) = inParam;
        break;

    default: {
        std::ostringstream oss;
        {
            DataLock lock(this);
            mParameters.Add(inParam, static_cast<int>(inConnection.Tag()));
            mParameters.ByPath(inParam.Path()).InsertInto(oss);
            ParameterChange();
        }
        ExecuteCallback(BCI_OnParameter, oss.str().c_str());
    }
    break;
    }
}

bool StateMachine::SpecialParameter(const CoreClient &inConnection, const Param &inParam)
{
    if (inParam.Name() == "Filters")
    {
        typedef std::vector<std::string> Lines;
        Lines lines;

        bool doInit = !mParameters.Exists(inParam.Path());
        Param &p = mParameters.ByPath(inParam.Path());
        if (doInit)
        {
            p = inParam;
            p.SetNumRows(0);
            p.SetNumColumns(1);
        }
        for (int i = 0; i < p.NumRows(); ++i)
            lines.push_back(p.Value(i, 0));

        std::ostringstream oss;
        oss << "/" << inConnection.Tag();
        for (int i = 0; i < inParam.NumRows(); ++i)
            lines.push_back(oss.str() + inParam.Value(i, 0).ToString());

        p.SetNumRows(lines.size());
        for (int i = 0; i < p.NumRows(); ++i)
            p.Value(i, 0) = lines[i];

        return true;
    }
    return false;
}

void StateMachine::OnReceive(CoreConnection &, const State &inState)
{
    std::ostringstream oss;
    {
        DataLock lock(this);
        mStates.Delete(inState.Name());
        mStates.Add(inState);
        inState.InsertInto(oss);
    }
    ExecuteCallback(BCI_OnState, oss.str().c_str());
}

void StateMachine::OnReceive(CoreConnection &c, const State2 &inState)
{
    StateMachine::OnReceive(c, State(inState));
}

bool StateMachine::OnStateVector(CoreConnection &, std::istream &is)
{
    DataLock lock(this);
    bool result = !!mStateVector.Unserialize(is);
    // CheckWatches() will be called later when receiving control signal
    // Calling CheckWatches() twice for the same block will result in O(SampleBlockSize^2) 
    // behavior in watches window, so don't call it here
#if 0
    CheckWatches(false);
#endif
    CheckFilePart();
    return result;
}

void StateMachine::OnReceive(CoreConnection &, const VisSignal &inSignal)
{
    if (inSignal.VisID().Empty())
    {
        DataLock lock(this);
        mControlSignal = inSignal;
        CheckWatches(false);
    }
    else
    {
        const std::string kind = "Graph";
        CheckInitializeVis(inSignal.VisID(), kind);
        int channels = inSignal.Signal().Channels(), elements = inSignal.Signal().Elements(),
            size = channels * elements;
        Assert(size >= 0);
        float *pData = new float[channels * elements];
        for (int ch = 0; ch < channels; ++ch)
            for (int el = 0; el < elements; ++el)
                pData[ch * elements + el] = static_cast<float>(inSignal.Signal()(ch, el));
        ExecuteCallback(BCI_OnVisSignal, inSignal.VisID().c_str(), channels, elements, pData);
        delete[] pData;
    }
}

void StateMachine::OnReceive(CoreConnection &conn, const VisSignalProperties &v)
{
    const CoreClient &c = *static_cast<CoreClient *>(conn.UserData());
    DataLock lock(this);
    if (v.VisID().Empty())
    {
        int state = SystemState() & ~StateFlags;
        if (state == SetConfigIssued)
        {
            for (int i = 0; i < mConnections.Size(); ++i)
                if (mConnections[i]->Send(mAutoParameters))
                    ThreadUtils::Idle();
            int idx = c.Tag();
            if (idx < mConnections.Size())
                mConnections[idx]->Send(v);
            else
                mControlSignal.SetProperties(v);
        }
    }
    else
    {
        std::vector<VisCfg> visCfg = v.ToVisCfg();
        for (size_t i = 0; i < visCfg.size(); ++i)
            OnReceive(conn, visCfg[i]);
    }
}

void StateMachine::OnReceive(CoreConnection &, const VisMemo &v)
{
    const std::string kind = "Memo";
    CheckInitializeVis(v.VisID(), kind);
    ExecuteCallback(BCI_OnVisMemo, v.VisID().c_str(), v.MemoText().c_str());
}

void StateMachine::OnReceive(CoreConnection &, const VisBitmap &v)
{
    const std::string kind = "Bitmap";
    CheckInitializeVis(v.VisID(), kind);
    const BitmapImage &b = v.BitmapImage();
    ExecuteCallback(BCI_OnVisBitmap, v.VisID().c_str(), b.Width(), b.Height(), b.RawData());
}

void StateMachine::OnReceive(CoreConnection&, const VisVideoFrame &v)
{
    const std::string kind = "VideoFrame";
    CheckInitializeVis(v.VisID(), kind);
    const VideoFrame &f = v.VideoFrame();
    ExecuteCallback(BCI_OnVisVideoFrame, v.VisID().c_str(), f.Width(), f.Height(), f.RawData());
}

void StateMachine::OnReceive(CoreConnection &, const VisCfg &v)
{
    DataLock lock(this);
    mVisualizations[v.VisID()][v.CfgID()] = v.CfgValue();
    ExecuteCallback(BCI_OnVisPropertyMessage, v.VisID().c_str(), v.CfgID(), v.CfgValue().c_str());
}

void StateMachine::OnModuleStateChange(const CoreClient &inConnection, SysState inState)
{
    int systemState = SystemState() & ~StateFlags;
    if (systemState == Transition || systemState == inState)
        return;

    switch (inState)
    {
    case Publishing:
        EnterState(Publishing);
        break;
    case Idle:
        ExecuteCallback(BCI_OnCoreInput);
        break;
    default:
        if (IsConsistentState(inState))
            EnterState(inState);
    }
}

bool StateMachine::IsConsistentState(SysState s) const
{
    for (int i = 0; i < mConnections.Size(); ++i)
        if (mConnections[i]->State() != s)
            return false;
    return true;
}

void StateMachine::SetConnectionState(SysState s)
{
    for (int i = 0; i < mConnections.Size(); ++i)
        mConnections[i]->EnterState(s);
}

void StateMachine::InitializeMeasurementUnits()
{
    ParamList params = mParameters;
    static const char* names[] = { "/SamplingRate", "/SampleBlockSize" };
    for (auto name : names)
        if (mAutoParameters.Exists(name))
            params.ByPath(name) = mAutoParameters.ByPath(name);
    try
    {
        MeasurementUnits::Initialize(params);
    }
    catch (const std::exception&)
    {
        // If SamplingRate and/or SampleBlockSize are set to auto and we did not receive
        // actual values in mAutoParameters due to an error, we get an exception from
        // MeasurementUnits::Initialize(). There is no need to display this secondary
        // error, so we don't take any additional action from here.
    }
    catch (const Exception&)
    {
    }
}

void StateMachine::CheckWatches(bool discont)
{
    int sampleBlockSize = mStateVector.Samples() - 1;
    double blockDuration = -1.0;
    if (mConnections.Size() > 0 && mConnections[0]->IsLocal())
    {
        // If the source module is running locally, we can use
        // the local clock to complete truncated time stamps.
        blockDuration = MeasurementUnits::SampleBlockDuration();
    }
    PrecisionTime sourceTime = mStateVector.StateValue("SourceTime");
    Watches().Check(sampleBlockSize, blockDuration, sourceTime, mLastWatchEvalDiscont);
    mLastWatchEvalDiscont = discont;
}

void StateMachine::InitializeFilePart()
{
    mFilePart = 1;
}

void StateMachine::DeinitializeFilePart()
{
    mFilePart = 0;
}

void StateMachine::CheckFilePart()
{
    if (mFilePart < 1)
        return;

    int filePart = mStateVector.StateValue("FilePart", mStateVector.Samples() - 1);
    if (filePart > mFilePart)
    {
        ExecuteCallback(BCI_OnNextFilePart, filePart);
        mFilePart = filePart;
    }
}

std::string StateMachine::CurrentRunFileName() const
{
    if (!mDataFileShmName.empty())
    {
        try
        {
            SharedMemory shm(mDataFileShmName);
            char* pData = static_cast<char*>(shm.Memory());
            if (!pData)
                throw std::runtime_error("Data file shared memory not available");
            return pData;
        }
        catch (const std::runtime_error& e)
        {
            bciwarn << e.what();
        }
    }
    return "";
}

