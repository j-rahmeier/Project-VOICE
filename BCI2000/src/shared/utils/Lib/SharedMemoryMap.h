//////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: A map of existing shared memory objects, indexed by
//   their shared memory names.
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
#ifndef TINY_SHARED_MEMORY_MAP_H
#define TINY_SHARED_MEMORY_MAP_H

#include <memory>
#include <map>
#include "SharedMemory.h"

namespace Tiny
{
    typedef std::shared_ptr<SharedMemory> ShmPtr;

    class SharedMemoryMap : private std::map<std::string, ShmPtr>
    {
      public:
        ShmPtr& New(size_t bytes)
        {
            SharedMemory* p = new SharedMemory(bytes);
            return (*this)[p->Name()] = ShmPtr(p);
        }
        ShmPtr& Get(const std::string& name)
        {
            ShmPtr& s = (*this)[name];
            if (!s)
                s = ShmPtr(new SharedMemory(name));
            return s;
        }
        void Clear(const std::string& name)
        {
            erase(name);
        }
    };

} // namespace Tiny

using Tiny::SharedMemoryMap;

#endif // TINY_SHARED_MEMORY_MAP_H
