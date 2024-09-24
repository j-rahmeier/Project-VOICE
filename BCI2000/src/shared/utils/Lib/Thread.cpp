//////////////////////////////////////////////////////////////////////
// $Id: Thread.cpp 8275 2024-07-25 17:21:36Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A thread class similar to the VCL's TThread.
//   To implement your own thread, create a class that inherits from
//   Thread, and put your own functionality into its
//   OnExecute() function.
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
#include "Thread.h"

#include "ClassName.h"
#include "Debugging.h"
#include "Exception.h"
#include "ExceptionCatcher.h"
#include "Lockable.h"
#include "ThreadUtils.h"
#include "WaitableEvent.h"
#include "Waitable.h"
#include "WaitableImplementation.h"
#include "SockStream.h"

#if _WIN32
// clang-format off
#include <Windows.h>
#include <Process.h>
#include <WinNT.h>
// clang-format on
#else
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#endif // _WIN32

#include <algorithm>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <list>
#include <vector>
#include <atomic>

#define USE_STD_THREAD 1

#if USE_STD_THREAD
#include <thread>
#endif

namespace
{

template <class T> struct UniqueList : std::vector<T>, Lockable<std::mutex>
{
    bool Add(const T &p)
    {
        ScopedLock(this);
        if (find(this->begin(), this->end(), p) == this->end())
        {
            this->push_back(p);
            return true;
        }
        return false;
    }
    bool Remove(const T &p)
    {
        ScopedLock(this);
        typename std::vector<T>::iterator i = std::find(this->begin(), this->end(), p);
        if (i != this->end())
        {
            this->erase(i);
            return true;
        }
        return false;
    }
};

struct NotificationList : UniqueList<Runnable *>
{
    bool Execute()
    {
        UniqueList<Runnable *> copy;
        WithLock(this) copy = *this;
        for (iterator i = copy.begin(); i != copy.end(); ++i)
            (*i)->Run();
        return !copy.empty();
    }
};

struct Notifications
{
    bool Add(int id, Runnable *p)
    {
        return mLists[id - 1].Add(p);
    }
    bool Remove(Runnable *p)
    {
        bool done = false;
        for (size_t i = 0; i < sizeof(mLists) / sizeof(*mLists); ++i)
            done |= mLists[i].Remove(p);
        return done;
    }
    bool Execute(int id)
    {
        return mLists[id - 1].Execute();
    }
    void Clear()
    {
        for (size_t i = 0; i < Thread::NumberOfEvents - 1; ++i)
            mLists[i].clear();
    }
    NotificationList mLists[Thread::NumberOfEvents - 1];
};
static thread_local Thread* stpCurrent;
static thread_local Waitable::Condition* stpAllow;
static UniqueList<Thread *> &sInstances()
{
    static UniqueList<Thread*> instance;
    return instance;
}

#if 0
void TerminateAllAndWait()
{
    Assert(Tiny::ThreadUtils::InMainThread());

    int numRunning = 1;
    while (numRunning > 0)
    {
        numRunning = 0;
        for (UniqueList<Thread *>::const_iterator i = sInstances().begin(); i != sInstances().end(); ++i)
        {
            if ((*i)->Running())
            {
                ++numRunning;
                (*i)->Terminate(Time::Interval(0));
            }
        }
        Tiny::ThreadUtils::Idle();
    }
    Assert(numRunning == 0);
}
#endif

} // namespace

namespace Tiny
{

struct Thread::Starter
{
#if USE_STD_THREAD
    static void* StartThread(void* inData)
    {
        Thread* obj = reinterpret_cast<Thread*>(inData);
        std::thread* p = new std::thread([obj]() { obj->RunThread(); });
        p->detach();
        return p;
    }
    static void DisposeThreadHandle(void* handle)
    {
        std::thread* obj = reinterpret_cast<std::thread*>(handle);
        delete obj;
    }
#elif _WIN32
    static unsigned int WINAPI ThreadProc(void *inData)
    {
        return reinterpret_cast<Thread *>(inData)->RunThread();
    }
    static void *StartThread(void *data)
    {
        uintptr_t handle = ::_beginthreadex(0, 0, &Starter::ThreadProc, data, 0, 0);
        return HANDLE(handle);
    }
    static void DisposeThreadHandle(void *handle)
    {
        if (handle)
            ::CloseHandle(handle);
    }
#else  // _WIN32
    static void *ThreadProc(void *inData)
    {
        // Set the cancel state to asynchronous, so pthread_cancel() will
        // immediately cancel thread execution.
        ::pthread_setcancelstate(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
        return reinterpret_cast<void *>(reinterpret_cast<Thread *>(inData)->RunThread());
    }
    static void *StartThread(void *data)
    {
        pthread_t *thread = new pthread_t;
        ::pthread_attr_t attributes;
        errno = ::pthread_attr_init(&attributes);
        if (!errno)
        {
            // Create a thread in detached mode, i.e. thread resources are
            // auto-released when the thread terminates.
            ::pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_DETACHED);
            errno = ::pthread_create(thread, &attributes, &Starter::ThreadProc, data);
            ::pthread_attr_destroy(&attributes);
        }
        if (errno)
        {
            delete thread;
            thread = nullptr;
        }
        return thread;
    }
    static void DisposeThreadHandle(void *handle)
    {
        delete static_cast<pthread_t *>(handle);
    }
#endif // !_WIN32
};
} // namespace Tiny

namespace
{
enum
{
    Stopped = 0,
    Started,
    Termination,
    Finished,
};
} // namespace

namespace Tiny
{

struct Thread::Private : Lockable<std::recursive_mutex>
{
    std::atomic<int> mResult = 0;
    ThreadUtils::ID mThreadID;
    std::atomic<void *> mHandle = nullptr;
    std::atomic<int> mState = 0;
    WaitableEvent mStartedEvent;
    Waitable::EventCondition* mpFinishedCondition;
    Waitable::ConditionPtr mFinishedConditionRef;

    std::string mName;
    std::atomic<Runnable *> mpAbortWait = nullptr;
    Notifications mNotify;
    Runnable *mpOnExecute;
    bool mAutoDelete, mAdopted;

    Private()
        : mThreadID(ThreadUtils::ID::None()), mResult(0), mState(Stopped), mpAbortWait(0), mpOnExecute(0),
          mAutoDelete(false), mAdopted(false), mpFinishedCondition(new Waitable::EventCondition(false)),
          mFinishedConditionRef(mpFinishedCondition)
    {
    }
    void Adopt()
    {
        mAdopted = true;
        mpFinishedCondition->Reset();
        mThreadID = ThreadUtils::ID::Current();
        mState = Started;
    }
    void Terminate()
    {
        int value = Started;
        if (mState.compare_exchange_strong(value, Termination))
            mNotify.Execute(WhenTerminating);
    }
    bool Wait(Time::Interval timeout)
    {
        return Waitable::Condition::Wait(&mFinishedConditionRef, 1, timeout) == 0;
    }
};

Thread::Thread(int inAutoDelete, Runnable *inOnExecute, const std::string &inName) : p(nullptr)
{
    Init(inAutoDelete, inOnExecute, inName);
}

Thread::Thread(Runnable *inOnExecute, const std::string &inName) : p(nullptr)
{
    Init(false, inOnExecute, inName);
}

Thread::Thread(const std::string &inName) : p(nullptr)
{
    Init(false, 0, inName);
}

void Thread::Init(int inAutoDelete, Runnable *inOnExecute, const std::string &inName)
{
    p.reset(new Private);
    sInstances().Add(this);
    p->mAutoDelete = inAutoDelete;
    p->mpOnExecute = inOnExecute;
    p->mName = inName;
    p->mStartedEvent.Reset();
    p->mpFinishedCondition->Set();
    mConditionRef = p->mFinishedConditionRef;
    Waitable::AssociateWith(mConditionRef);
}

Thread::~Thread()
{
    if (p->mpOnExecute)
        TerminateAndWait();
    else if (p->mAdopted)
    {
        p->mpFinishedCondition->Set();
        p->mState = Stopped;
    }
    sInstances().Remove(this);
    if (p->mState != Stopped && Current() != this)
        SuggestDebugging << "Thread \"" << Name()
                         << "\" still executing when destructed. "
                            "Call Thread::TerminateAndWait() from subclasses for a quick fix, "
                            "rewrite your code to use the Runnable argument to the constructor "
                            "for a more thorough solution.";
}

const std::string &Thread::Name() const
{
    return p->mName;
}

int Thread::Result() const
{
    return p->mResult;
}

bool Thread::Running() const
{
    switch (p->mState.load())
    {
    case Started:
    case Termination:
        return true;
    }
    return false;
}

bool Thread::Terminating() const
{
    return p->mState == Termination;
}

bool Thread::Terminate(Time::Interval inTimeout)
{
    p->Terminate();
    if (Current() == this)
        return false;
    WithLock(*p) if (p->mpAbortWait) p->mpAbortWait.load()->Run();
    if (inTimeout <= 0)
        return !Running();
    
    if (p->mAdopted && p->mState == Termination)
    {
        p->mState = Stopped;
        p->mpFinishedCondition->Set();
        return !Running();
    }

    Time expire = TimeUtils::MonotonicTime() + inTimeout;
    static const Time::Interval cSleep = Time::FromMilliseconds(100);
    stpAllow = p->mpFinishedCondition;
    bool stillRunning = true;
    while (stillRunning && TimeUtils::MonotonicTime() < expire)
    {
      WithLock(*p) if (p->mpAbortWait) p->mpAbortWait.load()->Run();
      stillRunning = p->mState != Stopped && !p->Wait(cSleep);
    }
    if (!stillRunning)
        p->mState = Stopped; // ensure consistency
    stpAllow = nullptr;
    return !stillRunning;
}

void Thread::TerminateAndWait(Time::Interval timeout)
{
    bool terminated = Terminate(timeout);
    if (Current() != this && !terminated)
        SuggestDebugging << "Thread still executing";
}

Thread *Thread::AdoptAs(const std::string &inName)
{
    Assert(!Current());
    Thread *pNew = new Thread(inName);
    pNew->p->Adopt();
    stpCurrent = pNew;
    return pNew;
}

Thread *Thread::Current()
{
    return stpCurrent;
}

bool Thread::Notify(int inWhen, Runnable *inHow)
{
    Thread *pCurrent = Current();
    if (!pCurrent)
        return false;
    if (inWhen == None)
        return pCurrent->p->mNotify.Remove(inHow);
    return pCurrent->p->mNotify.Add(inWhen, inHow);
}

int Thread::RegisterWait(const Waitable::ConditionPtr *inpData, int inCount, Runnable *inAborter)
{
    // Wait on explicitly allowed object
    Waitable::Condition *pAllow = static_cast<Waitable::Condition *>(stpAllow);
    if (pAllow)
        for (int i = 0; i < inCount; ++i)
            if (pAllow == inpData[i].get())
                return 0;

    int result = WaitAllowAbort;
    Thread *pCurrent = Current();
    if (pCurrent)
        WithLock(pCurrent->p.get())
        {
            pCurrent->p->mpAbortWait = inAborter;

            // Avoid deadlock (thread waiting on itself)
            for (int i = 0; i < inCount; ++i)
                if (pCurrent->p->mpFinishedCondition == inpData[i].get())
                    result |= WaitAbortNow;

            // Don't wait in termination phase (except for case above)
            if (pCurrent->p->mState != Started)
                result |= WaitAbortNow;

            // Only wait if all conditions are enabled
            for (int i = 0; i < inCount; ++i)
                if (!inpData[i]->Enabled())
                    result |= WaitAbortNow;
        }
    return result;
}

bool Thread::StartIfNotRunning()
{
    if (p->mName.empty())
        p->mName = ClassName(typeid(*this));
    int value = Stopped;
    if (!p->mState.compare_exchange_strong(value, Started))
    {
        p->mState = Stopped;
        Starter::DisposeThreadHandle(p->mHandle.exchange(nullptr));
    }

    p->mpFinishedCondition->Reset();
    if (!(p->mHandle = Starter::StartThread(this)))
    {
        p->mpFinishedCondition->Set();
        throw std_runtime_error << ::strerror(errno);
    }
    else
    {
        p->mStartedEvent.Set();
    }
    // Don't use any member variables below this point,
    // Thread class instance may have been auto-deleted
    // by now.
#if 0
    static bool registered = false;
    if (!registered)
        registered = !::atexit(&TerminateAllAndWait);
#endif
    return true;
}

void Thread::Start()
{
    if (!StartIfNotRunning())
        throw std_logic_error << "Thread of type " << Name() << " is already running";
}

int Thread::OnExecute()
{
    if (p->mpOnExecute)
        p->mpOnExecute->Run();
    return 0;
}

int Thread::RunThread()
{
    Debugging::SetCurrentThreadName(Name());
    // The OnFinished() handler may delete the Thread object, so we
    // maintain a shared pointer to our data until finished.
    std::shared_ptr<Private> ref = this->p;
    Waitable::ConditionPtr conditionRef = this->mConditionRef;

    MemberCall<int(Thread *)> execute(&Thread::OnExecute, this);
    ExceptionCatcher eh;
    eh.SetMessage("Canceling thread of type " + Name());

    ref->mNotify.Clear();
    ref->mThreadID = ThreadUtils::ID::Current();
    stpCurrent = this;
    ref->mStartedEvent.Wait();
    if (eh.Run(execute))
        ref->mResult = execute.Result();
    else
        ref->mResult = -1;
    ref->mState = Finished;
    ref->mNotify.Execute(WhenFinished);
    ref->mState = Stopped;
    ref->mThreadID = ThreadUtils::ID::None();
    ref->Lock();
    ref->mpFinishedCondition->Set();
    if (ref->mAutoDelete)
        delete this;
    stpCurrent = nullptr;
    Starter::DisposeThreadHandle(ref->mHandle.exchange(nullptr));
    ref->Unlock();
    Debugging::SetCurrentThreadName("");
    return ref->mResult;
}

} // namespace Tiny
