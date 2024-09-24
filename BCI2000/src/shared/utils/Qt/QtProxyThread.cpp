////////////////////////////////////////////////////////////////////////////////
// $Id: QtProxyThread.cpp 7728 2023-11-22 19:02:27Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A QThread descendant integrated with Tiny::Thread.
//   Some Qt code requires to run within a QThread. Rather than making all
//   threads QThreads, we run the respective pieces of code inside an instance
//   of QtProxyThread.
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
#include "QtProxyThread.h"
#include "Debugging.h"
#include "ExceptionCatcher.h"
#include "SynchronizedObject.h"
#include "ThreadUtils.h"

#include <QThread>

#include <mutex>
#include <condition_variable>

struct QtProxyThread::Private : QThread
{
    std::mutex mMutex;
    std::condition_variable mRun, mDone;
    enum { idle = 1, runCalled = 2, terminate = 3 } mState = idle;
    Runnable* mpRunnable = nullptr;

    Private(QtProxyThread *p) : QThread(nullptr)
    {
        QThread::start();
    }
    ~Private()
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mState = terminate;
        mRun.notify_all();
        lock.unlock();
        QThread::wait();
    }
    bool Run(Runnable *pRunnable)
    {
        Assert(pRunnable);
        std::unique_lock<std::mutex> lock(mMutex);
        mDone.wait(lock, [this]{ return mState == idle; });
        mState = runCalled;
        mpRunnable = pRunnable;
        mRun.notify_all();
        mDone.wait(lock, [this]{ return mState == idle; });
        return true;
    }
    void run() override
    {
        MemberCall<void(QtProxyThread::Private *)> threadFunc(&QtProxyThread::Private::ThreadFunc, this);
        ExceptionCatcher().Run(threadFunc);
    }
    void ThreadFunc()
    {
        WithThreadPriority(ThreadUtils::Priority::Maximum)
        {
            while (true)
            {
                std::unique_lock<std::mutex> lock(mMutex);
                mRun.wait(lock, [this]{ return mState != idle; });
                Runnable* p = mpRunnable;
                lock.unlock();
                if (mState == terminate)
                    return;
                if (mState == runCalled)
                {
                    if (p)
                        p->Run();
                    std::unique_lock<std::mutex> lock(mMutex);
                    mState = idle;
                    lock.unlock();
                    mDone.notify_all();
                }
            }
        }
    }
};

QtProxyThread::QtProxyThread() : p(new Private(this))
{
}

QtProxyThread::~QtProxyThread()
{
}

bool QtProxyThread::Run(Runnable *pRunnable)
{
    return p->Run(pRunnable);
}
