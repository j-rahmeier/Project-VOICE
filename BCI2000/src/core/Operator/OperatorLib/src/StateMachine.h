////////////////////////////////////////////////////////////////////////////////
// $Id: StateMachine.h 7985 2024-04-03 11:07:24Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A class that represents the operator module's state machine.
//   The state machine is responsible for
//   - maintaining a global BCI2000 system state (state of operation),
//   - maintaining global parameter and state lists,
//   - handling core module connections,
//   - maintaining visualization object properties,
//   - processing state change requests,
//   - triggering event callbacks to display text messages, or to visualize data.
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
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "CallbackBase.h"
#include "CoreClient.h"
#include "EventLink.h"
#include "GenericSignal.h"
#include "Lockable.h"
#include "ParamList.h"
#include "ProtocolVersion.h"
#include "ScriptEvents.h"
#include "Sockets.h"
#include "StateList.h"
#include "StateVector.h"
#include "Thread.h"
#include "VersionInfo.h"
#include "VisTable.h"
#include "Watches.h"

#include <cstddef>
#include <fstream>
#include <set>
#include <mutex>

class CommandInterpreter;

class StateMachine : public CallbackBase, public Lockable<>, private Thread, CoreConnection::Client
{
  public:
    enum
    {
        ConnectionTimeout = 500, // ms
    };

    // Methods
  public:
    StateMachine();
    virtual ~StateMachine();

  private:
    StateMachine(const StateMachine &) = delete;
    StateMachine &operator=(const StateMachine &) = delete;

  public:
    // State manipulation.
    //  As an argument to the Startup() function, provide a list of module names
    //  and ports as in "SignalSource:4000 SignalProcessing:4001 Application:4002".
    //  In case of an empty argument, a single list entry "Core Module:4000" is
    //  assumed.
    bool Startup(const char * = NULL);
    bool SetConfig();
    bool StartRun();
    bool StopRun();
    bool Shutdown();
    bool Reset();

    // Properties

    //  System state.
    enum SysState_
    {
        Idle = 0,
        WaitingForConnection,
        Publishing,
        Information,
        Initialization,
        SetConfigIssued,
        Resting,
        RunningInitiated,
        Running,
        SuspendInitiated,
        Suspended,
        Fatal,
        Transition,

        NumStates,

        ParamsModified = 1 << 8,
        StateFlags = ParamsModified,
    };
    typedef int SysState;

    int SystemState() const
    {
        return mSystemState;
    }

    int NumConnections() const
    {
        return mConnections.Size();
    }

    //  Locking.
    typedef ::Lock_<StateMachine> DataLock;

    //  Watches.
    void CheckWatches(bool discont);

    // File part.
    void InitializeFilePart();
    void DeinitializeFilePart();
    void CheckFilePart();

    //  Parameter list.
    ParamList &Parameters()
    {
        return mParameters;
    }
    const ParamList &Parameters() const
    {
        return mParameters;
    }
    ParamList &AutoConfiguredParameters()
    {
        return mAutoParameters;
    }
    const ParamList &AutoConfiguredParameters() const
    {
        return mAutoParameters;
    }
    void ParameterChange();

    // State list.
    StateList &States()
    {
        return mStates;
    }
    const StateList &States() const
    {
        return mStates;
    }
    bool SetStateValue(const char *name, State::ValueType value);
    State::ValueType GetStateValue(const char *name, int = 0) const;

    // Event list.
    bool SetEvent(const char *name, State::ValueType value, int duration = -1);
    bool PulseEvent(const char *name, State::ValueType value)
    {
        return SetEvent(name, value, 0);
    }

    // Control signal.
    const GenericSignal &ControlSignal() const
    {
        return mControlSignal;
    }
    const ::StateVector &StateVector() const
    {
        return mStateVector;
    }

    // Table of visualization properties.
    VisTable &Visualizations()
    {
        return mVisualizations;
    }
    const VisTable &Visualizations() const
    {
        return mVisualizations;
    }

    // Scripting events.
    ScriptEvents &EventScripts()
    {
        return mScriptEvents;
    }
    const ScriptEvents &EventScripts() const
    {
        return mScriptEvents;
    }

    // Watches.
    class Watch::List &Watches()
    {
        return mWatches;
    }
    const class Watch::List &Watches() const
    {
        return mWatches;
    }

    // Address of local connection.
    const std::string &LocalAddress() const
    {
        return mLocalAddress;
    }
    std::string SuggestUDPAddress(const std::string &) const;

    // Obtain current run file name.
    std::string CurrentRunFileName() const;

    // Issue a log message.
    void LogMessage(int messageCallbackID, const std::string &);

    // Interface to CommandInterpreter class.
  public:
    void AddListener(CommandInterpreter &listener)
    {
        WithLock(mListeners) mListeners.insert(&listener);
    }
    void RemoveListener(CommandInterpreter &listener)
    {
        WithLock(mListeners) mListeners.erase(&listener);
    }

  private:
    virtual int OnExecute();

    void Init();

    void EnterState(SysState);
    void PerformTransition(int transition);
    void ExecuteTransitionCallbacks(int transition);

    void SendNextModuleInfo();
    void PruneParameters();
    void BroadcastParameters();
    void BroadcastEndOfParameter();
    void BroadcastParameterChanges();
    void BroadcastStates();
    void BroadcastEndOfState();
    void InitializeStateVector();
    void InitializeModules();
    void InitializeMeasurementUnits();
    void MaintainDebugLog();
    void DebugWarning();
    void Randomize();
    void RandomizationWarning();

    void TriggerEvent(int eventCallbackID, const char* arg = 0);

  private:
    std::atomic<int> mSystemState = 0;
    Lockable<std::recursive_mutex> mTransitionLock;

    bool mIntroducedRandomSeed;
    std::string mPreviousRandomSeed, mDataFileShmName;
    ParamList mParameters, mAutoParameters;
    StateList mStates;
    class StateVector mStateVector;
    GenericSignal mControlSignal;
    VisTable mVisualizations;
    std::string mLocalAddress;

    std::ofstream mDebugLog;
    std::ofstream mSystemLog;

    struct Listeners : std::set<CommandInterpreter *>, Lockable<std::recursive_mutex>
    {
    } mListeners;
    ScriptEvents mScriptEvents;
    class Watch::List mWatches;
    bool mLastWatchEvalDiscont;
    int mFilePart;

  public:
    typedef struct CoreClient::Info ConnectionInfo;
    ConnectionInfo Info(int i) const;

  protected:
    bool OnStateVector(CoreConnection &, std::istream &);
    void OnReceive(CoreConnection &, const Status &);
    void OnReceive(CoreConnection &, const SysCommand &);
    void OnReceive(CoreConnection &, const Param &);
    void OnReceive(CoreConnection &, const State &);
    void OnReceive(CoreConnection &, const State2 &);
    void OnReceive(CoreConnection &, const VisSignal &);
    void OnReceive(CoreConnection &, const VisSignalProperties &);
    void OnReceive(CoreConnection &, const VisMemo &);
    void OnReceive(CoreConnection &, const VisBitmap &);
    void OnReceive(CoreConnection &, const VisVideoFrame &);
    void OnReceive(CoreConnection &, const VisCfg &);

  public:
    void OnModuleStateChange(const CoreClient &, SysState);

  private:
    bool CheckInitializeVis(const std::string &sourceID, const std::string &kind);
    bool SpecialParameter(const CoreClient &, const Param &);
    bool IsConsistentState(SysState) const;
    void SetConnectionState(SysState);
    void CloseConnections();

    friend class CoreClient;
    CoreClient::List mConnections;
    CoreClient *mpSourceModule;

    friend class EventLink;
    EventLink mEventLink;
};

#endif // STATE_MACHINE_H
