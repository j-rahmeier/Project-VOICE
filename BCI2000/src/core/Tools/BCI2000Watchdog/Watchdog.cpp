//////////////////////////////////////////////////////////////////////
// $Id$
// Authors: mellinger@neurotechcenter.org
// Description: A class for watching a BCI2000 instance.
//   We don't know details about BCI2000 modules and their possible child processes, so we
//   take a snapshot of the process tree when BCI2000 goes to "Running" state.
//   Whenever the watchdog is triggered, it will terminate BCI2000 by sending an "exit" command
//   first, and then terminating any remaining processes from the tree.
//   This is necessary to avoid piling up of defunct process instances, and dead modules which
//   block proper restarting of the BCI2000 system.
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
#include "Watchdog.h"

#include "ProcessList.h"
#include "StringUtils.h"
#include "FileUtils.h"
#include "Thread.h"
#include "Runnable.h"
#include "Sockets.h"
#include "BCI2000Connection.h"

#include <thread>
#include <chrono>

#ifndef DEBUG_TRANSITIONS
#define DEBUG_TRANSITIONS 0
#endif

using namespace std::literals::chrono_literals;

namespace {

    std::string ConstructOperatorPath(const std::string& batchfile)
    {
        std::string batchdir = FileUtils::ParentDirectory(batchfile);
        std::string progdir = FileUtils::ParentDirectory(batchdir) + "prog/";
        if (!FileUtils::IsDirectory(progdir))
            progdir = FileUtils::InstallationDirectory();
        return progdir + "Operator";
    }

    const int cUDPMaxLen = 64 * 1024;
}

struct Watchdog::Private
{
    class Watchdog::Listener* mpListener = nullptr;

    std::atomic<StateEnum> mState = Watchdog::Idle;

    std::string mBatchfile;
    std::vector<Watch> mWatches;
    double mRunningTimeout = 20; // seconds
    double mProcessesTimeout = 2; // seconds

    void Notification(Watchdog::MessageType, const std::string&);

    void StartupBci();
    void TerminateBci();

    struct BciStartupThread : Thread
    {
        BciStartupThread(Watchdog::Private*);
        MemberCall<void(Watchdog::Private*)> mCall;
    };
    void BciStartupThreadFunc();

    struct WaitForRunningThread : Thread
    {
        WaitForRunningThread(Watchdog::Private*, BCI2000Connection&);
        MemberCall<void(Watchdog::Private*, BCI2000Connection&)> mCall;
    };
    void WaitForRunningThreadFunc(BCI2000Connection&);

    struct WatchObserverThread : Thread
    {
        WatchObserverThread(Watchdog::Private*, const Watch&);
        MemberCall<void(Watchdog::Private*, const Watch&)> mCall;
    };
    void WatchObserverThreadFunc(const Watch&);

    std::mutex mWatchObserverThreadsMutex;
    std::vector<WatchObserverThread*> mWatchObserverThreads;
    void ClearWatchObserverThreads();
    void StartWatchObserverThreads();

    struct BciTerminationThread : Thread
    {
        BciTerminationThread(Watchdog::Private*);
        MemberCall<void(Watchdog::Private*)> mCall;
    };
    void BciTerminationThreadFunc();

    std::mutex mChildProcessesMutex;
    std::vector<ProcessList::ProcessData> mChildProcesses;
    void ClearProcessData();
    void FindChildProcesses();
    void TerminateChildProcesses();

    bool Transition(StateEnum from, StateEnum to);
    bool EnterState(StateEnum);
    std::string StateString(StateEnum) const;
    StateEnum State() const;

    Private();
    ~Private();
};

// Watchdog::Private class
Watchdog::Private::Private()
{
}

Watchdog::Private::~Private()
{
    ClearWatchObserverThreads();
}

void Watchdog::Private::ClearWatchObserverThreads()
{
    std::lock_guard<std::mutex> lock(mWatchObserverThreadsMutex);
    for (auto pThread : mWatchObserverThreads)
        delete pThread;
    mWatchObserverThreads.clear();
}

void Watchdog::Private::StartWatchObserverThreads()
{
    std::lock_guard<std::mutex> lock(mWatchObserverThreadsMutex);
    for (const auto& watch : mWatches)
    {
        mWatchObserverThreads.push_back(new WatchObserverThread(this, watch));
        mWatchObserverThreads.back()->Start();
    }
}

void Watchdog::Private::BciStartupThreadFunc()
{
    bool ok = FileUtils::IsFile(mBatchfile);
    if (!ok)
        Notification(Watchdog::ErrorMessage, "Invalid batch script file");

    if (ok) {
        if (!Transition(Idle, UserStartup) && !Transition(TriggeredTerminating, TriggeredStartup))
            return;

        Notification(Watchdog::LogMessage, "Starting up BCI2000...");
        std::string operatorPath = ConstructOperatorPath(mBatchfile);
        BCI2000Connection connection;
        connection.OperatorPath(operatorPath);
        bool ok = connection.Connect();
        if (!ok) {
            Notification(Watchdog::ErrorMessage, "Failed to connect to Operator module");
        }
        else {
            Notification(Watchdog::LogMessage, "Connected to Operator at " + operatorPath);
            WaitForRunningThread waitForRunningThread(this, connection);
            waitForRunningThread.Start();
            std::string command = "Execute script \"" + mBatchfile + "\"";
            int exitCode = 0;
            ok = connection.Execute(command, &exitCode);
            if (ok)
                Notification(Watchdog::LogMessage, "Successfully executed batch script");
            else if (connection.Connected()) // will be disconnected by waitForRunningThread on timeout
                Notification(Watchdog::ErrorMessage, "Failed to execute batch script");
            waitForRunningThread.Wait();
        }
        connection.Disconnect(true);

        if (State() == Triggered) {
            TerminateBci();
            return;
        }

        if (!Transition(UserStartup, Watching) && !Transition(TriggeredStartup, Watching))
            return;

        FindChildProcesses();
        ClearWatchObserverThreads();
        StartWatchObserverThreads();
        Notification(Watchdog::LogMessage, "Entered watching state");
    }
}

void Watchdog::Private::BciTerminationThreadFunc()
{
    ClearWatchObserverThreads();

    if (Transition(Triggered, TriggeredTerminating)) {
        Notification(Watchdog::LogMessage, "Watchdog triggered, restarting BCI2000...");
    }
    else {
        EnterState(UserTerminating);
    }

    Notification(Watchdog::LogMessage, "Sending exit command to BCI2000...");
    int exitCode = 0;
    BCI2000Connection connection;
    connection.Connect();
    connection.Quit();
    connection.Disconnect(true);

    std::unique_lock<std::mutex> lock(mChildProcessesMutex);
    bool empty = mChildProcesses.empty();
    lock.unlock();

    if (empty) {
        FindChildProcesses();
        lock.lock();
        empty = mChildProcesses.empty();
        lock.unlock();
    }

    if (!empty) {
        std::ostringstream oss;
        oss << mProcessesTimeout;
        Notification(Watchdog::LogMessage, "Waiting for " + oss.str() + "s before terminating any remaining subprocesses...");
        std::this_thread::sleep_for(1s * mProcessesTimeout);

        TerminateChildProcesses();
        ClearProcessData();
    }
    if (State() == TriggeredTerminating) {
        StartupBci();
    }
    else {
        EnterState(Idle);
        Notification(Watchdog::LogMessage, "Entered idle state");
    }
}

void Watchdog::Private::WaitForRunningThreadFunc(BCI2000Connection& mainConnection)
{
    BCI2000Connection connection;
    if (!connection.Connect())
    {
        Notification(Watchdog::ErrorMessage, "Could not connect to Operator");
        return;
    }
    
    std::ostringstream timeout;
    timeout << mRunningTimeout;
    int exitCode = 0;
    bool ok = connection.Execute("WAIT FOR Running " + timeout.str(), &exitCode);
    bool connected = connection.Connected();
    connection.Disconnect(true);
    if (!ok || exitCode != 0 || !connected) {
        if (Transition(UserStartup, Triggered) || Transition(TriggeredStartup, Triggered)) {
            Notification(Watchdog::TriggeredMessage, "BCI2000 did not reach Running state within " + timeout.str() + "s");
        }
        mainConnection.Disconnect(true);
        return;
    }
    Notification(Watchdog::LogMessage, "BCI2000 is now in Running state");
}

void Watchdog::Private::WatchObserverThreadFunc(const Watch& watch)
{
    BCI2000Connection connection;
    if (!connection.Connect()) {
        Notification(Watchdog::ErrorMessage, "Could not connect to Operator");
        return;
    }
    int exitCode = 0;
    std::string address;
    bool ok = connection.Execute("ADD WATCH " + watch.expression, &exitCode, &address);
    if (exitCode == -1 && address.find("When processing") == 0)
        ok = false;
    if (!ok || exitCode != -1) {
        Notification(Watchdog::ErrorMessage, "Could not add watch for " + watch.expression + ": " + address);
        return;
    }
    connection.Disconnect(true);

    ReceivingUDPSocket socket;
    socket.Open(address);
    if (!socket.IsOpen()) {
        Notification(Watchdog::ErrorMessage, "Could not open socket at " + address);
        return;
    }

    std::vector<char> buffer(cUDPMaxLen);

    if (watch.initialTimeoutSeconds > 0) {
        Time::Interval initial = Time::Interval::Seconds(watch.initialTimeoutSeconds);
        // See whether watch is active within the interval given by initialTimeoutSeconds.
        // First, absorb initial value (sent immediately after Start)
        if (socket.Wait(Time::Interval())) {
            int available = socket.Available();
            buffer.resize(available);
            socket.Read(buffer.data(), available);
        }
        // Now, wait for the next value to arrive within initialTimeoutSeconds
        if (!socket.Wait(initial) && !Thread::Current()->Terminating()) {
            std::ostringstream oss;
            oss << "Watch \"" << watch.expression << "\" inactive during first "
                << watch.initialTimeoutSeconds << "s, ignoring";
            Notification(Watchdog::LogMessage, oss.str());
            return;
        }
    }

    Time::Interval timeout = Time::Interval::Seconds(watch.timeoutSeconds);
    while (socket.Wait(timeout)) {
        int available = socket.Available();
        buffer.resize(available);
        socket.Read(buffer.data(), available);
    }
    if (!Thread::Current()->Terminating()) {
        std::ostringstream oss;
        oss << "Watch \"" << watch.expression << "\" timed out after " << watch.timeoutSeconds << "s";
        EnterState(Triggered);
        Notification(Watchdog::TriggeredMessage, oss.str());
        TerminateBci();
    }
}

void Watchdog::Private::ClearProcessData()
{
    std::lock_guard<std::mutex> lock(mChildProcessesMutex);
    for (auto& process : mChildProcesses)
        ProcessList::CleanupProcessData(process);
    mChildProcesses.clear();
}

void Watchdog::Private::FindChildProcesses()
{
    ClearProcessData();
    std::unique_lock<std::mutex> lock(mChildProcessesMutex);
    mChildProcesses = ProcessList::FindChildProcessesRecursively(0);
    if (!mChildProcesses.empty()) {
        std::ostringstream oss;
        oss << "Subprocesses detected:\n";
        for (const auto& proc : mChildProcesses)
            oss << "  " << proc.name << " (" << proc.pid << ")\n";
        lock.unlock();
        Notification(Watchdog::LogMessage, oss.str());
    }
}

void Watchdog::Private::TerminateChildProcesses()
{
    std::ostringstream oss;
    std::unique_lock<std::mutex> lock(mChildProcessesMutex);
    for (auto& proc : mChildProcesses) {
        // consider any additional (recent) subprocesses
        auto subprocesses = ProcessList::FindChildProcessesRecursively(proc.pid);
        if (KillProcess(proc)) {
            oss << "  Terminated " << proc.name << " (" << proc.pid << ")\n";
        }
        for (auto& subproc : subprocesses) {
            if (KillProcess(subproc)) {
                oss << "  Terminated " << subproc.name << " (" << subproc.pid << ")\n";
            }
        }
    }
    lock.unlock();
    if (oss.str().empty())
        Notification(Watchdog::LogMessage, "No subprocesses to terminate");
    else
        Notification(Watchdog::LogMessage, oss.str());
}

bool Watchdog::Private::Transition(StateEnum from, StateEnum to)
{
    bool result = mState.compare_exchange_strong(from, to);
#if DEBUG_TRANSITIONS
    if (result)
        Notification(Log2Message, "Transition " + StateString(from) + " -> " + StateString(to));
    else
        Notification(Log2Message, "Failed transition " + StateString(from) + " -> " + StateString(to));
#endif
    return result;
}

bool Watchdog::Private::EnterState(StateEnum to)
{
    StateEnum from = mState;
    mState = to;
#if DEBUG_TRANSITIONS
    Notification(Log2Message, "Transition " + StateString(from) + " -> " + StateString(to));
#endif
    return true;
}

Watchdog::StateEnum Watchdog::Private::State() const
{
    return mState;
}

std::string Watchdog::Private::StateString(StateEnum state) const
{
#define _(x) case x: return #x;
    switch (state) {
        _(Idle)
        _(UserStartup)
        _(Watching)
        _(Triggered)
        _(UserTerminating)
        _(TriggeredTerminating)
        _(TriggeredStartup)
        _(Error)
    }
    std::ostringstream oss;
    oss << "(" << state << ")";
    return oss.str();
#undef _
}

void Watchdog::Private::Notification(Watchdog::MessageType type, const std::string& msg)
{
    if (type == Watchdog::ErrorMessage) {
        EnterState(Error);
        TerminateBci();
    }
    if (mpListener)
        mpListener->OnLogMessage(type, msg);
}

void Watchdog::Private::StartupBci()
{
    // Self-deleting thread
    (new BciStartupThread(this))->Start();
}

void Watchdog::Private::TerminateBci()
{
    // Self-deleting thread
    (new BciTerminationThread(this))->Start();
}

// Watchdog class
Watchdog::Watchdog() : p(new Private)
{
}

Watchdog::~Watchdog()
{
    delete p;
}

Watchdog::StateEnum Watchdog::State() const
{
    return p->State();
}

void Watchdog::SetWatches(const std::vector<Watch>& watches)
{
    p->mWatches = watches;
}

const std::vector<Watchdog::Watch>& Watchdog::Watches() const
{
    return p->mWatches;
}

void Watchdog::SetBatchfile(const std::string& batchfile)
{
    p->mBatchfile = batchfile;
}

const std::string& Watchdog::Batchfile() const
{
    return p->mBatchfile;
}

void Watchdog::SetRunningTimeout(double seconds)
{
    p->mRunningTimeout = seconds;
}

double Watchdog::RunningTimeout() const
{
    return p->mRunningTimeout;
}

void Watchdog::SetProcessesTimeout(double seconds)
{
    p->mProcessesTimeout = seconds;
}

double Watchdog::ProcessesTimeout() const
{
    return p->mProcessesTimeout;
}

void Watchdog::StartBCI2000()
{
    p->StartupBci();
}

void Watchdog::TerminateBCI2000()
{
    p->TerminateBci();
}

void Watchdog::Reset()
{
    p->ClearWatchObserverThreads();
    p->TerminateChildProcesses();
    if (p->State() != Idle) {
        p->EnterState(Idle);
        p->Notification(LogMessage, "Reset watchdog to idle state");
    }
}

void Watchdog::SetListener(Watchdog::Listener* pListener)
{
    p->mpListener = pListener;
}

Watchdog::Listener* Watchdog::GetListener() const
{
    return p->mpListener;
}

Watchdog::Private::WatchObserverThread::WatchObserverThread(Watchdog::Private* p, const Watch& watch)
: Thread(&mCall, "Watch Observer (" + watch.expression + ")"),
  mCall(&Watchdog::Private::WatchObserverThreadFunc, p, watch)
{
}

Watchdog::Private::BciTerminationThread::BciTerminationThread(Watchdog::Private* p)
: Thread(true, &mCall),
  mCall(&Watchdog::Private::BciTerminationThreadFunc, p)
{
}

Watchdog::Private::BciStartupThread::BciStartupThread(Watchdog::Private* p)
: Thread(true, &mCall),
  mCall(&Watchdog::Private::BciStartupThreadFunc, p)
{
}

Watchdog::Private::WaitForRunningThread::WaitForRunningThread(Watchdog::Private* p, BCI2000Connection& c)
: Thread(false, &mCall),
  mCall(&Watchdog::Private::WaitForRunningThreadFunc, p, c)
{
}
