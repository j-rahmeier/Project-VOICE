////////////////////////////////////////////////////////////////////////////////
// $Id: CoreModule.h 8296 2024-08-01 16:12:02Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A class that represents functionality common to all BCI2000
//          core modules.
//
//          For a core module that does not use any GUI elements,
//          CoreModule::Run() takes care of all framework functionality, such
//          that a module's minimal main() function reads:
//
//          int main( int argc, char** argv )
//          {
//            bool success = CoreModule().Run( argc, argv );
//            return ( success ? 0 : -1 );
//          }
//
//          For core modules that use GUI elements, the GUI's message loop must
//          be replaced by the message loop implemented within the CoreModule
//          class. To process GUI messages, override CoreModule::OnProcessGUIMessages()
//          from a derived class:
//
//          class CoreModuleGUI : public CoreModule
//          {
//            public:
//              CoreModuleGUI() {}
//              void OnProcessGUIMessages() override
//              {
//                // Example applying to Borland VCL
//                Application->ProcessMessages();
//                ::Sleep( 0 );
//              }
//              bool OnGUIMessagesPending() override
//              {
//                return ::GetQueueStatus( QS_ALLINPUT );
//              }
//           };
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
#ifndef CORE_MODULE_H
#define CORE_MODULE_H

#include "CoreConnection.h"
#include "Environment.h"
#include "GenericSignal.h"
#include "GenericVisualization.h"
#include "ParamList.h"
#include "ProtocolVersion.h"
#include "Sockets.h"
#include "StateList.h"
#include "StateVector.h"
#include "Uncopyable.h"
#include "EventQueue.h"
#include "VisualizationHook.h"
#include "SignalSharingHook.h"

#if MODTYPE

#define SIGSRC 1
#define SIGSRC_NAME "SignalSource"
#define SIGPROC 2
#define SIGPROC_NAME "SignalProcessing"
#define APP 3
#define APP_NAME "Application"

#if (MODTYPE == SIGSRC)
#define THISMODULE SIGSRC_NAME
#define THISOPPORT "4000"
#define PREVMODULE APP_NAME
#define NEXTMODULE SIGPROC_NAME
#elif (MODTYPE == SIGPROC)
#define PREVMODULE SIGSRC_NAME
#define THISOPPORT "4001"
#define THISMODULE SIGPROC_NAME
#define NEXTMODULE APP_NAME
#elif (MODTYPE == APP)
#define PREVMODULE SIGPROC_NAME
#define THISOPPORT "4002"
#define THISMODULE APP_NAME
#define NEXTMODULE SIGSRC_NAME
#else
#error Unknown MODTYPE value
#endif

#else // MODTYPE

#define MODTYPE 1
#define THISMODULE "Module"
#define THISOPPORT "4000"
#define PREVMODULE "previous"
#define NEXTMODULE "next"

#endif // MODTYPE

class CoreModule : CoreConnection::Client, Uncopyable
{
    static const int cInitialConnectionTimeout = 20000; // ms

  public:
    CoreModule();
    virtual ~CoreModule();
    bool Run(int &argc, char **argv); // must take int reference for Qt's sake
    void Terminate();

  protected:
    // Override to integrate with a GUI library.
    virtual void OnInitialize(int &argc, char **argv)
    {
    }
    virtual void OnProcessGUIMessages()
    {
    }
    virtual bool OnGUIMessagesPending()
    {
        return false;
    }

  private:
    void DoRun(int &argc, char **argv);
    bool Initialize(int &argc, char **argv);
    void PrintExtensions();
    void MainMessageLoop();
    void ProcessBCIAndGUIMessages();

    bool IsFirstModule() const;
    bool IsLastModule() const;

    void InitializeOperatorConnection(const std::string &operatorAddress);
    void InitializeCoreConnections();
    void ShutdownSystem();
    void InitializeStatevector();
    void ResetStatevector();
    void AppendFilterDirectory(Param &) const;

    void InitializeFilterChain(const class SignalProperties &);
    void InitializeInputSignal(const class SignalProperties &);
    void AutoConfigFilters();
    void InitializeFilters();
    void StartRunFilters();
    void StopRunFilters();
    void BroadcastParameterChanges();
    void ProcessFilters();
    void SendOutput();

    // BCI message handling functions.
    bool OnStateVector(CoreConnection &, std::istream &) override;

    void OnReceive(CoreConnection &, const Param &) override;
    void OnReceive(CoreConnection &, const State &) override;
    void OnReceive(CoreConnection &, const State2 &) override;
    void OnReceive(CoreConnection &, const VisSignal &) override;
    void OnReceive(CoreConnection &, const VisSignalProperties &) override;
    void OnReceive(CoreConnection &, const SysCommand &) override;
    void OnReceive(CoreConnection &, const ProtocolVersion &) override;

    bool OnSend(CoreConnection &, const VisSignal &) override;

  private:
    ParamList mParamlist, mNextModuleInfo;
    StateList mStatelist;
    StateVector mStatevector, mInitialStatevector;
    GenericSignal mInputSignal, mOutputSignal;
    ClientTCPSocket mOperatorSocket, mNextModuleSocket;
    TCPSocket mPreviousModuleSocket;
    ServerTCPSocket mServerSocket;
    CoreConnection mOperator, mNextModule, mPreviousModule;
    EventQueue mBCIEvents;
    VisualizationHook mVisualizationHook;
    SignalSharingHook mSignalSharingHook;

    bool mTerminating, mRunning, mFiltersInitialized, mNeedStopRun,
        mReceivingNextModuleInfo;
    void *mGlobalID;
    bool mOperatorBackLink, mAutoConfig;
    bool mActiveResting;
    std::map<const GenericSignal *, int> mLargeSignals;
    Environment::Context &mEnvironment;
};

#endif // CORE_MODULE_H
