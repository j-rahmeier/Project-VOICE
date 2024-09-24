////////////////////////////////////////////////////////////////////////////////
// $Id: HistogramObserver.h 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A statistical observer that stores distribution information
//   in form of aging histograms. For information how to use observers in general,
//   see the ObserverBase class declared in ObserverBase.h.
//
//   While maintaining histograms, QuantileAccuracy() is used as a threshold
//   to determine when neighboring data points should be collapsed into a single
//   one.
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
#ifndef HISTOGRAM_OBSERVER_H
#define HISTOGRAM_OBSERVER_H

#include "Histogram.h"
#include "ObserverBase.h"
#include <vector>

namespace StatisticalObserver
{

class HistogramObserver : public virtual ObserverBase
{
    // This class implements the ObserverBase interface. For documentation, see ObserverBase.h.
  public:
    enum
    {
        Supports = StatisticalObserver::Variance | StatisticalObserver::Quantile | StatisticalObserver::CentralMoment
    };
    HistogramObserver(int config = Supports);

  protected:
    void DoChange() override;
    void DoAgeBy(unsigned int count) override;
    void DoObserve(const Vector &, Number weight) override;
    void DoClear() override;

  public:
    Number PowerSum0(MemPool &) const override
    {
        return mPowerSum0;
    }
    VectorPtr PowerSum1(MemPool &ioPool) const override
    {
        return PowerSumDiag(1, ioPool);
    }
    VectorPtr PowerSum2Diag(MemPool &ioPool) const override
    {
        return PowerSumDiag(2, ioPool);
    }
    VectorPtr PowerSumDiag(unsigned int i, MemPool &ioPool) const override;
    VectorPtr CDF(Number, MemPool &ioPool) const override;
    VectorPtr InverseCDF(Number, MemPool &ioPool) const override;

  private:
    Number
        mPowerSum0; // This matches the sum of weights in every histogram, and is redundantly maintained for efficiency.
    std::vector<class Histogram> mHistograms;
};

} // namespace StatisticalObserver

#endif // HISTOGRAM_OBSERVER_H
