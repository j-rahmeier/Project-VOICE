////////////////////////////////////////////////////////////////////////////////
// $Id: SimulinkConnectorFilter.cpp 8164 2024-05-25 18:56:51Z mellinger $
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
#include "SimulinkConnectorFilter.h"
#include "Sockets.h"
#include "Streambuf.h"
#include "Thread.h"
#include "WaitableEvent.h"
#include "Expression.h"

struct SimulinkConnectorFilter::Private
{
    // A signal to be shared.
    GenericSignal mSharedSignal;
    SignalProperties mSharedSignalProperties;
    // An event to notify about thread startup.
    WaitableEvent mThreadStartupDone;
    // An event to notify the sender thread when new signal data is available.
    WaitableEvent mSignalAvailable;
    // An object to wrap a call to SenderThreadFunc().
    MemberCall<void(Private *)> mSenderThreadCall;
    // The sender thread.
    Thread mSenderThread;
    // Listening address.
    std::string mAddress;
    // Expressions for states to share.
    std::vector<Expression> mStates;

    Private();
    void SenderThreadFunc();
};

SimulinkConnectorFilter::Private::Private()
    : mSenderThreadCall(&Private::SenderThreadFunc, this),
      mSenderThread(&mSenderThreadCall, "Simulink Connector Thread"),
      mSignalAvailable(true)
{
}

void SimulinkConnectorFilter::Private::SenderThreadFunc()
{
    while (!mSenderThread.Terminating())
    {
        // initialize shared signal from input signal properties
        mSharedSignal = GenericSignal(mSharedSignalProperties);
        mThreadStartupDone.Set();

        ServerTCPSocket socket;
        socket.Open(mAddress);
        if (!socket.Listening())
        {
            bcierr << "Cannot listen on " << mAddress;
            return;
        }

        bciwarn << "Waiting for connection on " << mAddress;
        if (!socket.Wait())
        {
            bciwarn << "Wait aborted";
            return;
        }
        if (!socket.WaitForAccept(0))
        {
            bciwarn << "Connection failed, aborting";
            return;
        }

        bciwarn << "Connected to " << socket.RemoteAddress();
        socket.SetTCPNodelay(true);
        socket.SetFlushAfterWrite(true);

        if (socket.Connected() == Socket::local) // only use shared memory when connected locally
        {
            mSharedSignal.ShareAcrossModules();
            bciwarn << "Locally connected to " << socket.RemoteAddress() << ", using shared memory";
        }
        else // otherwise, full data will be transmitted over network (slower)
        {
            bciwarn << "Remotely connected to " << socket.RemoteAddress() << ", transmitting data through network";
        }

        UnbufferedIO buf;
        buf.SetOutput(&socket.Output());
        std::ostream clientConnection(&buf);
        mSharedSignal.Properties().Serialize(clientConnection); // send signal properties first
        bool connected = true;
        while (connected && mSignalAvailable.Wait())   // will return false when Thread::Terminate() has been called
        {
            connected = !!mSharedSignal.Serialize(clientConnection); // will transmit memory reference, or signal, depending on
                                                                     // whether ShareAcrossModules() has been called
        }
        if (connected)
        {
            GenericSignal().Serialize(clientConnection); // empty signal to indicate end of data
        }
        bciwarn << "Done";
    }
}

// Main class

SimulinkConnectorFilter::SimulinkConnectorFilter() : p(new Private)
{
}

SimulinkConnectorFilter::~SimulinkConnectorFilter()
{
    delete p;
}

void SimulinkConnectorFilter::Publish()
{
    BEGIN_PARAMETER_DEFINITIONS
        "Connector string SimulinkConnectorAddress= localhost:1905 localhost:1905 % % "
        "// address connector is listening on, empty to disable",
        "Connector stringlist SimulinkConnectorStates= 0 % % "
        "// states to send to Simulink, appended to signal",
    END_PARAMETER_DEFINITIONS
}

void SimulinkConnectorFilter::Preflight(const SignalProperties &Input, SignalProperties &Output) const
{
    Parameter("SimulinkConnectorAddress");
    auto simulinkConnectorStates = Parameter("SimulinkConnectorStates");
    for (int i = 0; i < simulinkConnectorStates->NumValues(); ++i)
        Expression(simulinkConnectorStates(i)).Evaluate(); // will result in an error if invalid
    Output = Input;
}

void SimulinkConnectorFilter::Initialize(const SignalProperties &Input, const SignalProperties &Output)
{
    p->mAddress = Parameter("SimulinkConnectorAddress").ToString();
    if (p->mAddress.empty())
        return;

    p->mSharedSignalProperties = Input;

    p->mStates.clear();
    auto simulinkConnectorStates = Parameter("SimulinkConnectorStates");
    for (int i = 0; i < simulinkConnectorStates->NumValues(); ++i)
    {
        p->mStates.push_back(Expression(simulinkConnectorStates(i)));
        p->mStates.back().Compile();
    }
    if (simulinkConnectorStates->NumValues() > 0)
    {
        if (Input.Elements() < Statevector->Samples() - 1)
            bciwarn << "Signal has fewer elements than Statevector, will not transfer all state values";
        else if (Input.Elements() > Statevector->Samples() - 1)
            bciwarn << "Signal has more elements than Statevector, will fill with NaNs";
    }

    p->mSharedSignalProperties.SetChannels(Input.Channels() + p->mStates.size());

    p->mThreadStartupDone.Reset();
    p->mSenderThread.Start();
    p->mThreadStartupDone.Wait();
}

void SimulinkConnectorFilter::Halt()
{
    p->mSenderThread.Terminate();
}

void SimulinkConnectorFilter::StartRun()
{
}

void SimulinkConnectorFilter::StopRun()
{
}

void SimulinkConnectorFilter::Process(const GenericSignal &Input, GenericSignal &Output)
{
    Output = Input;
    for (int ch = 0; ch < Input.Channels(); ++ch)
        for (int el = 0; el < Input.Elements(); ++el)
            p->mSharedSignal(ch, el) = Input(ch, el);
    for (int state = 0; state < p->mStates.size(); ++state)
    {
        int ch = Input.Channels() + state;
        int samples = std::min(p->mSharedSignal.Elements(), Statevector->Samples() - 1);
        for (int sample = 0; sample < samples; ++sample)
            p->mSharedSignal(ch, sample) = p->mStates[state].Evaluate(&Input, sample);
        for (int sample = samples; sample < p->mSharedSignal.Elements(); ++sample)
            p->mSharedSignal(ch, sample) = GenericSignal::NaN;
    }
    p->mSignalAvailable.Set();
}
