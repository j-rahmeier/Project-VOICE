//////////////////////////////////////////////////////////////////////
// $Id: StopWatch.h 7464 2023-06-30 15:04:08Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An object that allows to measure the timing of function
//   calls at ms precision.
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
#ifndef TINY_STOP_WATCH_H
#define TINY_STOP_WATCH_H

#include "PrecisionTime.h"

namespace Tiny
{

class StopWatch
{
  public:
    StopWatch()
    {
        Reset();
    }
    void Start()
    {
        mStart = PrecisionTime::Now();
    }
    double Stop()
    {
        double lap = Lapse();
        mTotal += lap;
        Start();
        return lap;
    }
    double Lapse() const // ms
    {
        return PrecisionTime::UnsignedDiff(PrecisionTime::Now(), mStart);
    }
    double Total() const // ms
    {
        return mTotal;
    }
    void Reset()
    {
        mTotal = 0;
        Start();
    }

  private:
    PrecisionTime::NumType mStart;
    double mTotal;
};

} // namespace Tiny

using Tiny::StopWatch;

#endif // TINY_STOP_WATCH_H
