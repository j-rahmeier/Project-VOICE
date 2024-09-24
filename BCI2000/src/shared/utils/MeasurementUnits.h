/////////////////////////////////////////////////////////////////////////////
// $Id: MeasurementUnits.h 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Utilities for handling measurement units and conversions.
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
//////////////////////////////////////////////////////////////////////////////
#ifndef MEASUREMENT_UNITS_H
#define MEASUREMENT_UNITS_H

#include "ParamList.h"
#include "PhysicalUnit.h"
#include "Runnable.h"
#include "SignalProperties.h"
#include <string>
#include <memory>

// This class converts strings such as "123.3" or "12ms" to plain numbers that
// represent values in global units.
class MeasurementUnits
{
  public:
    // Use these functions to convert values forth and back into "natural" BCI2000 units:
    static double TimeInSampleBlocks(const std::string &);
    static double TimeInSeconds(const std::string &value);
    static double TimeInMilliseconds(const std::string &value);

    static std::string TimeUnit();

    static double SamplingRate();
    static int SampleBlockSize();
    static double SampleBlockDuration();

    static double ValueIn(const std::string &unitsOf, const std::string &);

    static void Initialize(ParamList &);
    static void AddInitializeCallback(const std::shared_ptr<Runnable> &);

    static void SetInstance(MeasurementUnits *);
    static MeasurementUnits *Instance();

  public:
    MeasurementUnits();
    double TimeInSampleBlocks_(const std::string &);
    double TimeInSeconds_(const std::string &value);
    double TimeInMilliseconds_(const std::string &value);

    std::string TimeUnit_();

    double SamplingRate_();
    int SampleBlockSize_();
    double SampleBlockDuration_();

    double ValueIn_(const std::string &unitsOf, const std::string &);

    void Initialize_(ParamList &);
    void AddInitializeCallback_(const std::shared_ptr<Runnable> &);

  private:
    double mSamplingRate;
    double mSampleBlockSize;
    PhysicalUnit mTimeUnit, mIdUnit;

    typedef std::vector<std::shared_ptr<Runnable>> CallbackList;
    CallbackList mCallbacks;
};

#endif // MEASUREMENT_UNITS_H
