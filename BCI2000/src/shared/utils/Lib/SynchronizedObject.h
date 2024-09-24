//////////////////////////////////////////////////////////////////////
// $Id: SynchronizedObject.h 7464 2023-06-30 15:04:08Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A template for variables to be synchronized between
//   threads. Only supports int32_t, bool, and pointer types.
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
#ifndef TINY_SYNCHRONIZED_OBJECT_H
#define TINY_SYNCHRONIZED_OBJECT_H

#include <atomic>
#include <algorithm>
#include <limits>

#if __APPLE__
// no support for shared_mutex in older versions of macOS,
// simulate a shared_mutex with a (worse-performing) mutex
#include <mutex>
namespace std
{
  struct shared_mutex : mutex
  {
    void lock_shared() { lock(); }
    void unlock_shared() { unlock(); }
  };
}
#else
#include <shared_mutex>
#endif

namespace Tiny
{

enum
{
    Full,
    AfterWriting,
    BeforeReading,
    AfterWrite = AfterWriting,
    BeforeRead = BeforeReading,
};

template <int Kind = Full> struct SyncMemory
{
    SyncMemory()
    {
        std::atomic_thread_fence(std::memory_order_seq_cst);
    }
};


#define WithLocked(x)                                                                                                  \
    for (bool b_ = true; b_;)                                                                                          \
        for (auto x; b_; b_ = false)

namespace internal_
{
template <class V> struct RWLock
{
    void ReaderLock() const
    {
        mLock.lock();
    }
    void ReaderUnlock() const
    {
        mLock.unlock();
    }
    void WriterLock() const
    {
        mLock.lock();
    }
    void WriterUnlock() const
    {
        mLock.unlock();
    }
    mutable V mLock;
};
template <> struct RWLock<std::shared_mutex>
{
    void ReaderLock() const
    {
        mLock.lock_shared();
    }
    void ReaderUnlock() const
    {
        mLock.unlock_shared();
    }
    void WriterLock() const
    {
        mLock.lock();
    }
    void WriterUnlock() const
    {
        mLock.unlock();
    }
    mutable std::shared_mutex mLock;
};
} // namespace internal_

template <class T, class L = std::shared_mutex> class SynchronizedObject
{
    struct ConstRef;
    struct MutableRef;

  public:
    SynchronizedObject()
    {
    }
    explicit SynchronizedObject(const T &t) : mValue(t)
    {
    }
    SynchronizedObject &SwapWith(T &t)
    {
        WriterLock();
        std::swap(mValue, t);
        WriterUnlock();
        return *this;
    }

    class ConstPtr
    {
      public:
        explicit ConstPtr(const SynchronizedObject *p) : p(p)
        {
            p->ReaderLock();
        }
        ConstPtr(ConstPtr &&r) : p(r.p)
        {
            r.p = 0;
        }
        ConstPtr &operator=(ConstPtr &r)
        {
            if (&r != this)
                Unlock();
            p = r.p;
            r.p = 0;
            return *this;
        }
        ~ConstPtr()
        {
            Unlock();
        }
        operator bool() const
        {
            return p;
        }
        const T &Ref() const
        {
            return *this->Get();
        }
        const T *Ptr() const
        {
            return this->Get();
        }
        const T &operator*() const
        {
            return Ref();
        }
        const T *operator->() const
        {
            return Ptr();
        }

      private:
        const T *Get() const
        {
            return &p->mValue;
        }
        void Unlock()
        {
            if (p)
                p->ReaderUnlock();
        }
        const SynchronizedObject *p;
    };
    ConstPtr Const() const
    {
        return ConstPtr(this);
    }
    ConstPtr operator->() const
    {
        return ConstPtr(this);
    }
    ConstRef operator*() const
    {
        return ConstRef(this);
    }

    struct MutablePtr
    {
        explicit MutablePtr(SynchronizedObject *p) : p(p)
        {
            p->WriterLock();
        }
        MutablePtr(MutablePtr &&r) : p(r.p)
        {
            r.p = 0;
        }
        MutablePtr &operator=(MutablePtr &r)
        {
            if (&r != this)
                Unlock();
            p = r.p;
            r.p = 0;
            return *this;
        }
        ~MutablePtr()
        {
            Unlock();
        }
        operator bool() const
        {
            return p;
        }
        T &Ref()
        {
            return *this->Get();
        }
        T *Ptr()
        {
            return this->Get();
        }
        T &operator*()
        {
            return Ref();
        }
        T *operator->()
        {
            return Ptr();
        }

      protected:
        T *Get() const
        {
            return &p->mValue;
        }

      private:
        void Unlock()
        {
            if (p)
                p->WriterUnlock();
        }
        SynchronizedObject *p;
    };
    MutablePtr Mutable()
    {
        return MutablePtr(this);
    }

  private:
    void ReaderLock() const
    {
        mLock.ReaderLock();
    }
    void ReaderUnlock() const
    {
        mLock.ReaderUnlock();
    }
    void WriterLock() const
    {
        mLock.WriterLock();
    }
    void WriterUnlock() const
    {
        mLock.WriterUnlock();
    }

  private:
    SynchronizedObject(const SynchronizedObject &);
    SynchronizedObject &operator=(const SynchronizedObject &);

    struct ConstRef
    {
        explicit ConstRef(const SynchronizedObject *p) : ptr(p)
        {
        }
        operator const T &() const
        {
            return ptr.operator*();
        }

      private:
        ConstPtr ptr;
    };
    struct MutableRef
    {
        explicit MutableRef(SynchronizedObject *p) : ptr(p)
        {
        }
        operator T &()
        {
            return ptr.operator*();
        }
        MutableRef &operator=(const T &t)
        {
            ptr.operator*() = t;
            return *this;
        }

      private:
        MutablePtr ptr;
    };

    internal_::RWLock<L> mLock;
    T mValue;
};

} // namespace Tiny
using Tiny::AfterWrite;
using Tiny::BeforeRead;
using Tiny::SynchronizedObject;
using Tiny::SyncMemory;

#endif // TINY_SYNCHRONIZED_OBJECT_H
