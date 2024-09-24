////////////////////////////////////////////////////////////////////////////////
// $Id: ObjectPool.h 7464 2023-06-30 15:04:08Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A template for pools consisting of objects which are created
//   on demand.
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
#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include "Lockable.h"
#include <list>
#include <memory>

namespace Tiny
{

namespace
{
struct True
{
    template <class T> bool operator()(T)
    {
        return true;
    }
};
} // namespace

template <class T, class Available = True>
class ObjectPool : private Lockable<std::mutex>, private std::list<std::shared_ptr<T>>
{
  public:
    explicit ObjectPool(size_t initialSize = 0)
    {
        for (size_t i = 0; i < initialSize; ++i)
            Create();
    }
    int Size() const
    {
        ScopedLock(this);
        return static_cast<int>(this->size());
    }
    int Shrink()
    {
        {
            ScopedLock(this);
            auto i = this->begin();
            while (i != this->end())
                if (!i->IsShared())
                    i = this->erase(i);
                else
                    ++i;
        }
        return Size();
    }
    std::shared_ptr<T> Get()
    {
        WithLock(this) for (auto i = this->begin(); i != this->end();
                            ++i) if (!(i->use_count() > 1) && Available()(&**i)) return *i;
        return Create();
    }
    template <class U> bool Broadcast(bool (*f)(T *, U), U u)
    {
        bool result = true;
        WithLock(this) for (auto i = this->begin(); i != this->end(); ++i) result &= f(&**i, u);
        return result;
    }

  private:
    std::shared_ptr<T> Create()
    {
        ScopedLock(this);
        this->push_back(std::shared_ptr<T>(new T));
        return this->back();
    }
};

} // namespace Tiny

using Tiny::ObjectPool;

#endif // OBJECT_POOL_H
