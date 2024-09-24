////////////////////////////////////////////////////////////////////////////////
// $Id$
// Description: A GenericFilter::Chain::FilterHook that implements
//   signal sharing through shared memory.
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
#include "SignalSharingHook.h"

#include "Directory.h"
#include "SignalProperties.h"
#include "GenericSignal.h"
#include "MessageChannel.h"

namespace {
    std::string SharingParamName(const GenericFilter* pFilter)
    {
        return "Share" + pFilter->Name();
    }
}

// Connection
SignalSharingHook::Connection::Connection()
    : mSenderThreadCall(&Connection::SenderThreadFunc, this),
    mSenderThread(&mSenderThreadCall, "Signal Sharing Sender"), mSignalAvailable(true)
{
    mSocket.SetTCPNodelay(true);
    mSocket.SetFlushAfterWrite(true);
}

SignalSharingHook::Connection::~Connection()
{
    mSenderThread.Terminate();
    mSocket.Close();
}

void SignalSharingHook::Connection::Initialize(const std::string& address, const SignalProperties& Output)
{
    mSenderThread.Terminate();
    mSocket.Close();
    mSocket.Input().ClearIOState();
    mSocket.Output().ClearIOState();
    if (!address.empty())
    { // connect to the client side
        mSocket.Open(address);
        // initialize shared signal from input signal properties
        mSharedSignal = GenericSignal(Output);
        if (!mSocket.Connected())
        {
            bciwarn << "Cannot connect to " << address;
        }
        else if (mSocket.Connected() == Socket::local) // only use shared memory when connected locally
        {
            mSharedSignal.ShareAcrossModules();
            bciout << "Locally connected to " << address << ", using shared memory";
        }
        else // otherwise, full data will be transmitted over network (slower)
        {
            bciwarn << "Remotely connected to " << address << ", transmitting data through network";
        }
    }
    mSenderThread.Start();
}

void SignalSharingHook::Connection::Update(const GenericSignal& Output)
{
    mSharedSignal.AssignValues(Output);
    mSignalAvailable.Set();
}

void SignalSharingHook::Connection::SenderThreadFunc()
{
    if (mSocket.Connected())
    {
        UnbufferedIO buf;
        buf.SetOutput(&mSocket.Output());
        // A message channel wraps up BCI2000 objects into BCI2000 messages.
        MessageChannel messageChannel(buf);
        messageChannel.Send(mSharedSignal.Properties()); // send signal properties first
        while (mSignalAvailable.Wait()) // will return false when Thread::Terminate() has been called
        {
            if (!messageChannel.Send(mSharedSignal)) // will transmit memory reference, or signal, depending on
                // whether ShareAcrossModules() has been called
            {
                bciwarn << "Could not send signal, giving up";
                mSenderThread.Terminate();
            }
        }
        messageChannel.Send(GenericSignal()); // empty signal to indicate end of data
    }
}

// SignalSharingHook
void SignalSharingHook::OnPublish(GenericFilter* pFilter)
{
    std::string paramDefinition = "SignalSharing:Processing%20Stages";
    paramDefinition +=
        " string " + SharingParamName(pFilter) + "= % % % % // Share " + pFilter->Name() + " output at address ip:port";
    pFilter->AddParameters(&paramDefinition, 1);
    mConnections[pFilter] = new Connection;
}

void SignalSharingHook::OnDispose(GenericFilter* pFilter)
{
    delete mConnections[pFilter];
    mConnections[pFilter] = nullptr;
}

void SignalSharingHook::OnPreflight(GenericFilter* pFilter, const SignalProperties& Input, const SignalProperties& Output)
{
    // Access the sharing parameter so the system does not complain at Initialize().
    pFilter->OptionalParameter(SharingParamName(pFilter));
}

void SignalSharingHook::OnInitialize(GenericFilter* pFilter, const SignalProperties& Input, const SignalProperties& Output)
{
    std::string address = pFilter->Parameter(SharingParamName(pFilter));
    mConnections[pFilter]->Initialize(address, Output);
}

void SignalSharingHook::OnProcess(GenericFilter* pFilter, const GenericSignal& Input, const GenericSignal& Output)
{
    mConnections[pFilter]->Update(Output);
}

