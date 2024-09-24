//////////////////////////////////////////////////////////////////////
// $Id: ReusableThread.cpp 8250 2024-07-12 11:17:34Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that wraps a thread, and allows clients to
//   run code inside that thread. Unlike OSThread, which starts a new
//   thread each time its Start() function is called, an instance of
//   ReusableThread is bound to a single thread during its lifetime, and
//   re-uses that thread for each call to Run(). After calling Run()
//   for a Runnable, call Wait() to wait until execution of the
//   Runnable has finished.
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
#include "ReusableThread.h"

#include "Exception.h"
#include "Runnable.h"
#include "Thread.h"
#include "WaitableEvent.h"

#include <atomic>
#include <condition_variable>

struct ReusableThread::Private : Thread
{
    ~Private()
    {
        Thread::TerminateAndWait();
    }
    int OnExecute();

    WaitableEvent mStartEvent, mFinishedEvent;
    // mFinishedCondition duplicates the functionality of mFinishedEvent but its wait will not be triggered
    // by Thread::Terminate().
    std::condition_variable mFinishedCondition;
    std::mutex mFinishedConditionMutex;

    std::atomic<bool> mAlive = false;
    std::atomic<Runnable *> mpRunnable = nullptr;
};

ReusableThread::ReusableThread() : p(new Private)
{
    p->mAlive = true;
    p->mFinishedEvent.Set();
    Waitable::AssociateWith(p->mFinishedEvent);
    p->Thread::Start();
}

ReusableThread::~ReusableThread()
{
    p->mStartEvent.AbortWaiting();
    delete p;
}

bool Tiny::ReusableThread::Run(Runnable& inRunnable, bool inSynchronously)
{
    if (!p->mAlive)
        throw std_runtime_error << "Thread is no longer available for execution";
    Runnable* pValue = nullptr;
    if (!p->mpRunnable.compare_exchange_strong(pValue, &inRunnable))
        return false;
    p->mFinishedEvent.Reset();
    p->mStartEvent.Set();
    if (inSynchronously) {
        std::unique_lock<std::mutex> lock(p->mFinishedConditionMutex);
        p->mFinishedCondition.wait(lock, [this] {return !p->mpRunnable;});
    }
    return true;
}

bool ReusableThread::Alive() const
{
    return p->mAlive;
}

int ReusableThread::Private::OnExecute()
{
    while (mStartEvent.Wait())
    {
        mStartEvent.Reset();
        try
        {
            mpRunnable.load()->Run();
        }
        catch (...)
        {
            mAlive = false;
            mFinishedEvent.Set();
            throw;
        }
        std::unique_lock<std::mutex> lock(mFinishedConditionMutex);
        mpRunnable = 0;
        mFinishedEvent.Set();
        lock.unlock();
        mFinishedCondition.notify_one();
    }
    mFinishedEvent.AbortWaiting();
    mAlive = false;
    return 0;
}
