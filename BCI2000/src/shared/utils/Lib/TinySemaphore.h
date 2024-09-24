//////////////////////////////////////////////////////////////////////
// $Id: TinySemaphore.h 7645 2023-10-05 17:37:31Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A semaphore that acts as a Waitable.
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
#ifndef TINY_SEMAPHORE_H
#define TINY_SEMAPHORE_H

#include "Constants.h"
#include "Waitable.h"
#include "Uncopyable.h"

namespace Tiny
{

class Semaphore : public Waitable, Uncopyable
{
  public:
    Semaphore(int initialValue = 0);
    ~Semaphore();

    bool Increase();
    bool Decrease(const Time::Interval & = Time::Forever());

  private:
    struct Private;
    Private *p;
};

} // namespace Tiny

using Tiny::Semaphore;

#endif // TINY_SEMAPHORE_H
