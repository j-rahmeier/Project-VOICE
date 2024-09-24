////////////////////////////////////////////////////////////////////////////////
// $Id: CombinedObserver.h 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A statistical observer that combines a PowerSumObserver with
//   a HistogramObserver.
//   It provides accurate mean, variance, and covariance, as well as support
//   for quantiles and moments.
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
#ifndef COMBINED_OBSERVER_H
#define COMBINED_OBSERVER_H

#include "HistogramObserver.h"
#include "PowerSumObserver.h"

namespace StatisticalObserver
{

class CombinedObserver : public virtual PowerSumObserver, public virtual HistogramObserver
{
    // This class implements the ObserverBase interface. For documentation, see ObserverBase.h.
  public:
    enum
    {
        Supports = StatisticalObserver::Variance | StatisticalObserver::Covariance | StatisticalObserver::Quantile |
                   StatisticalObserver::CentralMoment
    };

    CombinedObserver(int inConfig = DefaultConfig)
        : ObserverBase(inConfig, Supports), PowerSumObserver(ImpliedConfig(inConfig) & PowerSumObserver::Supports),
          HistogramObserver(ImpliedConfig(inConfig) & HistogramObserver::Supports),
          mUseHistogram(ImpliedConfig(inConfig) & (StatisticalObserver::Quantile | StatisticalObserver::CentralMoment))
    {
    }

  protected:
    void DoChange() override
    {
        PowerSumObserver::DoChange();
        if (mUseHistogram)
            HistogramObserver::DoChange();
    }
    void DoAgeBy(unsigned int count) override
    {
        PowerSumObserver::DoAgeBy(count);
        if (mUseHistogram)
            HistogramObserver::DoAgeBy(count);
    }
    void DoObserve(const Vector &v, Number w) override
    {
        PowerSumObserver::DoObserve(v, w);
        if (mUseHistogram)
            HistogramObserver::DoObserve(v, w);
    }
    void DoClear() override
    {
        PowerSumObserver::DoClear();
        if (mUseHistogram)
            HistogramObserver::DoClear();
    }

  public:
    Number PowerSum0(MemPool &ioPool) const override
    {
        return PowerSumObserver::PowerSum0(ioPool);
    }
    VectorPtr PowerSum1(MemPool &ioPool) const override
    {
        return PowerSumObserver::PowerSum1(ioPool);
    }
    VectorPtr PowerSum2Diag(MemPool &ioPool) const override
    {
        return PowerSumObserver::PowerSum2Diag(ioPool);
    }
    MatrixPtr PowerSum2Full(MemPool &ioPool) const override
    {
        return PowerSumObserver::PowerSum2Full(ioPool);
    }
    VectorPtr PowerSumDiag(unsigned int i, MemPool &ioPool) const override
    {
        return HistogramObserver::PowerSumDiag(i, ioPool);
    }
    VectorPtr CDF(Number p, MemPool &ioPool) const override
    {
        return HistogramObserver::CDF(p, ioPool);
    }
    VectorPtr InverseCDF(Number p, MemPool &ioPool) const override
    {
        return HistogramObserver::InverseCDF(p, ioPool);
    }

  private:
    bool mUseHistogram;
};

} // namespace StatisticalObserver

#endif // COMBINED_OBSERVER_H
