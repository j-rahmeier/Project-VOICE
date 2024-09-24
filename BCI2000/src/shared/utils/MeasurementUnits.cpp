/////////////////////////////////////////////////////////////////////////////
// $Id: MeasurementUnits.cpp 7195 2023-02-07 18:31:03Z mellinger $
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "BCIStream.h"
#include "MeasurementUnits.h"
#include <limits>

namespace
{
MeasurementUnits *sStaticInstance()
{
    static MeasurementUnits instance;
    return &instance;
}
MeasurementUnits *spInstance = sStaticInstance();
} // namespace

double MeasurementUnits::TimeInSampleBlocks(const std::string &value)
{
    return spInstance->TimeInSampleBlocks_(value);
}

double MeasurementUnits::TimeInSeconds(const std::string &value)
{
    return spInstance->TimeInSeconds_(value);
}

double MeasurementUnits::TimeInMilliseconds(const std::string &value)
{
    return spInstance->TimeInMilliseconds_(value);
}

std::string MeasurementUnits::TimeUnit()
{
    return spInstance->TimeUnit_();
}

double MeasurementUnits::SamplingRate()
{
    return spInstance->SamplingRate_();
}

int MeasurementUnits::SampleBlockSize()
{
    return spInstance->SampleBlockSize_();
}

double MeasurementUnits::SampleBlockDuration()
{
    return spInstance->SampleBlockDuration_();
}

double MeasurementUnits::ValueIn(const std::string &unitsOf, const std::string &value)
{
    return spInstance->ValueIn_(unitsOf, value);
}

void MeasurementUnits::Initialize(ParamList &p)
{
    spInstance->Initialize_(p);
}

void MeasurementUnits::AddInitializeCallback(const std::shared_ptr<Runnable> &p)
{
    spInstance->AddInitializeCallback_(p);
}

void MeasurementUnits::SetInstance(MeasurementUnits *pInstance)
{
    if (pInstance)
        spInstance = pInstance;
    else
        spInstance = sStaticInstance();
}

MeasurementUnits *MeasurementUnits::Instance()
{
    return spInstance;
}

MeasurementUnits::MeasurementUnits() : mSamplingRate(1.0), mSampleBlockSize(1.0)
{
}

double MeasurementUnits::TimeInSeconds_(const std::string &value)
{
    return TimeInSampleBlocks(value) * mSampleBlockSize / mSamplingRate;
}

double MeasurementUnits::TimeInMilliseconds_(const std::string &value)
{
    return TimeInSeconds(value) * 1e3;
}

std::string MeasurementUnits::TimeUnit_()
{
    return mTimeUnit.RawToPhysical(1);
}

double MeasurementUnits::SamplingRate_()
{
    return mSamplingRate;
}

int MeasurementUnits::SampleBlockSize_()
{
    return static_cast<int>(mSampleBlockSize);
}

double MeasurementUnits::SampleBlockDuration_()
{
    return mSampleBlockSize / mSamplingRate;
}

double MeasurementUnits::TimeInSampleBlocks_(const std::string &value)
{
    if (mTimeUnit.IsPhysical(value))
        return mTimeUnit.PhysicalToRaw(value);
    return value.empty() ? 0 : mIdUnit.PhysicalToRaw(value);
}

double MeasurementUnits::ValueIn_(const std::string &unitsOf, const std::string &value)
{
    std::string unit = "1" + unitsOf;
    if (mTimeUnit.IsPhysical(unit))
        return TimeInSeconds(value) / TimeInSeconds(unit);
    PhysicalUnit u;
    u.SetGainWithSymbol(unit);
    if (u.IsPhysical(value))
        return u.PhysicalToRaw(value);
    return value.empty() ? 0 : mIdUnit.PhysicalToRaw(value) / u.Gain();
}

void MeasurementUnits::Initialize_(ParamList &inParams)
{
    if (inParams.Exists("/SamplingRate"))
    {
        Param &p = inParams.ByPath("/SamplingRate");
        PhysicalUnit u;
        u.SetSymbol("Hz");
        if (!u.IsPhysical(p.Value()))
            p.Value() = std::string(p.Value()) + "Hz";
        mSamplingRate = u.PhysicalToRaw(p.Value());
        if (mSamplingRate <= 0.0)
            bcierr << "Parameter SamplingRate needs to be greater zero";
    }
    if (inParams.Exists("/SampleBlockSize"))
    {
        std::string value = inParams.ByPath("/SampleBlockSize").Value();
        if (PhysicalUnit().SetSymbol("s").IsPhysical(value))
            bcierr << "Parameter SampleBlockSize must be given in samples, not in seconds";
        else
        {
            mSampleBlockSize = PhysicalUnit().SetSymbol("").PhysicalToRaw(value);
            if (mSampleBlockSize < 1)
                bcierr << "Parameter SampleBlockSize needs to be >= 1";
        }
    }
    // Set the unit for raw numbers representing time to multiples of sample block duration.
    mTimeUnit.SetOffset(0).SetGain(mSampleBlockSize / mSamplingRate).SetSymbol("s");

    for (CallbackList::iterator i = mCallbacks.begin(); i != mCallbacks.end(); ++i)
        (*i)->Run();
}

void MeasurementUnits::AddInitializeCallback_(const std::shared_ptr<Runnable> &r)
{
    mCallbacks.push_back(r);
}
