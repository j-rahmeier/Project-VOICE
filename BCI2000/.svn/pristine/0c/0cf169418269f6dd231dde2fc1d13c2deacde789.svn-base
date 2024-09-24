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
#ifndef SIGNAL_SHARING_HOOK_H
#define SIGNAL_SHARING_HOOK_H

#include "Uncopyable.h"
#include "GenericFilter.h"
#include "Sockets.h"
#include "WaitableEvent.h"
#include "Thread.h"

class SignalSharingHook : public GenericFilter::Chain::FilterHook, private Uncopyable
{
  protected:
    void OnPublish(GenericFilter*) override;
    void OnDispose(GenericFilter*) override;
    void OnPreflight(GenericFilter*, const SignalProperties& Input, const SignalProperties& Output) override;
    void OnInitialize(GenericFilter*, const SignalProperties& Input, const SignalProperties& Output) override;
    void OnProcess(GenericFilter*, const GenericSignal& Input, const GenericSignal& Output) override;

  private:
    struct Connection
    {
        // A socket for a connection to the client application.
        ClientTCPSocket mSocket;
        // A signal to be shared.
        GenericSignal mSharedSignal;
        // An event to notify the sender thread when new signal data is available.
        WaitableEvent mSignalAvailable;
        // An object to wrap a call to SenderThreadFunc().
        MemberCall<void(Connection*)> mSenderThreadCall;
        // The sender thread.
        Thread mSenderThread;

        Connection();
        ~Connection();
        void Initialize(const std::string& address, const SignalProperties& Output);
        void Update(const GenericSignal&);
        void SenderThreadFunc();
    };

    std::map<GenericFilter*, Connection*> mConnections;
};

#endif // SIGNAL_SHARING_HOOK_H
