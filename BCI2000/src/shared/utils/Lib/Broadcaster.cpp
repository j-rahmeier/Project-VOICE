//////////////////////////////////////////////////////////////////////
// $Id: Broadcaster.cpp 7464 2023-06-30 15:04:08Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A mix-in class that provides support for synchronous
//   events to be sent to a set of handlers.
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
#include "Broadcaster.h"
#include "Lockable.h"
#include "Runnable.h"

#include <algorithm>
#include <map>
#include <vector>
#include <atomic>

namespace Tiny
{

namespace
{
typedef std::vector<Runnable *> ListenerSet;
typedef std::map<int, ListenerSet> ListenerMap;

thread_local class Broadcaster::Message sMessage;
} // namespace

struct Broadcaster::Data : Lockable<std::recursive_mutex>
{
    ListenerMap mListeners;
    std::atomic<bool> mEnabled = 0;
    Data() {}
    Data(const Data& other)
    {
        *this = other;
    }
    Data& operator=(const Data& other)
    {
        if (&other != this) {
            mListeners = other.mListeners;
            mEnabled.store(other.mEnabled.load());
        }
        return *this;
    }
};

Broadcaster::Broadcaster() : d(new Data)
{
    d->mEnabled = true;
}

Broadcaster::~Broadcaster()
{
    delete d;
}

Broadcaster::Broadcaster(const Broadcaster &b) : d(new Data(*b.d))
{
}

Broadcaster &Broadcaster::operator=(const Broadcaster &b)
{
    *d = *b.d;
    return *this;
}

const class Broadcaster::Message &Broadcaster::Message()
{
    if (!sMessage.sender)
        throw std_runtime_error << "Trying to access message information outside of callback";
    return sMessage;
}

int Broadcaster::Emit(unsigned short message, void *arg)
{
    int count = 0;
    if (d->mEnabled)
    {
        sMessage.sender = this;
        sMessage.id = message;
        sMessage.arg = arg;
        WithLock(d)
        {
            ListenerMap::const_iterator i = d->mListeners.find(message);
            if (i == d->mListeners.end())
                return 0;
            const ListenerSet &listeners = i->second;
            for (ListenerSet::const_iterator i = listeners.begin(); i != listeners.end(); ++i)
                if (*i)
                {
                    (*i)->Run();
                    ++count;
                }
        }
        sMessage.sender = nullptr;
        sMessage.id = -1;
        sMessage.arg = nullptr;
    }
    return count;
}

Broadcaster &Broadcaster::SetEnabled(bool b)
{
    d->mEnabled = b;
    return *this;
}

bool Broadcaster::Enabled() const
{
    return d->mEnabled;
}

Broadcaster &Broadcaster::AddListener(int message, Runnable *p)
{
    WithLock(d)
    {
        ListenerSet &set = d->mListeners[message];
        ListenerSet::iterator i;
        while ((i = std::find(set.begin(), set.end(), nullptr)) != set.end())
            set.erase(i);
        if (std::find(set.begin(), set.end(), p) == set.end())
            set.push_back(p);
    }
    return *this;
}

Broadcaster &Broadcaster::RemoveListener(int message, Runnable *p)
{

    WithLock(d)
    {
        ListenerSet &set = d->mListeners[message];
        ListenerSet::iterator i = std::find(set.begin(), set.end(), p);
        if (i != set.end())
            *i = nullptr;
    }
    return *this;
}

} // namespace Tiny
