////////////////////////////////////////////////////////////////////////////////
// $Id: SampleClock.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: juergen.mellinger@neurotechcenter.org
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
#include "SampleClock.h"
#include "BCI2000FileReader.h"
#include "BCIException.h"
#include <iomanip>
#include <algorithm>

namespace AnalysisDataObjects
{

static bool regSampleClock = AnalysisDataObject::registerFactory("sample_clock", &SampleClock::create);

AnalysisDataObject* SampleClock::create(Context* p, const ExpressionParser::NodeList& args)
{
    int64_t ignoreAtBegin = 1, ignoreAtEnd = 1;
    if (args.size() > 0)
        ignoreAtBegin = args[0]->Evaluate();
    if (args.size() > 1)
        ignoreAtEnd = args[1]->Evaluate();
    if (args.size() > 2)
        throw bciexception << "sample_clock() expects blocks to ignore at begin and end as its arguments";
    ignoreAtBegin = std::max<int64_t>(1, ignoreAtBegin);
    ignoreAtEnd = std::max<int64_t>(1, ignoreAtEnd);
    return new SampleClock(p, ignoreAtBegin, ignoreAtEnd);
}

SampleClock::SampleClock(Context *p, int64_t ignoreAtBegin, int64_t ignoreAtEnd)
    : TimeSeriesDataObject(p), mSampleBlockSize(0), mNumSamples(0), mFileDuration(NaN(mFileDuration)),
      mSampleDuration(NaN(mSampleDuration)), mBlockDuration(NaN(mBlockDuration)), mRsq(NaN(mRsq)),
      mIgnoreBlocksAtBegin(ignoreAtBegin), mIgnoreBlocksAtEnd(ignoreAtEnd)
{
}

void SampleClock::onPreEvalSamples()
{
    TimeSeriesDataObject::onPreEvalSamples();
    BCI2000FileReader *p = context()->reader;
    mUnit = *context()->timestampUnit;

    mNominalRate_Hz = Ratio::From(p->SamplingRate());
    mSampleBlockSize = p->SignalProperties().Elements();
    mNumSamples = p->NumSamples();
    StateRef SourceTime = p->State("SourceTime");

    int64_t i1 = mSampleBlockSize * mIgnoreBlocksAtBegin - 1;
    if (i1 >= mNumSamples - 1)
    {
        invalidate();
        return;
    }
    p->ReadStateVector(i1);
    int64_t ts1 = SourceTime.AsUnsigned();
    // i1 should be the begin of a block
    while (i1 < mNumSamples - 1 && SourceTime.AsUnsigned() == ts1)
        p->ReadStateVector(++i1);
    ts1 = SourceTime.AsUnsigned();

    int64_t i2 = mNumSamples - mSampleBlockSize * mIgnoreBlocksAtEnd;
    if (i2 < 0)
    {
        invalidate();
        return;
    }
    p->ReadStateVector(i2);
    int64_t ts2 = SourceTime.AsUnsigned();
    // i2 should be the begin of a block
    while (i2 > 0 && SourceTime.AsUnsigned() == ts2)
        p->ReadStateVector(--i2);
    ++i2;
    if (i2 < i1)
    {
        invalidate();
        return;
    }

    int numWraparounds = 0;
    ts2 = ts1;
    for (auto i = i1 + 1; i <= i2; ++i)
    {
        p->ReadStateVector(i);
        if (SourceTime.AsUnsigned() < ts2)
            ++numWraparounds;
        ts2 = SourceTime.AsUnsigned();
    }

    p->ReadStateVector(*context()->sample);

    int64_t wraparound_ms = 1 << SourceTime->Length();
    ts2 += wraparound_ms * numWraparounds;

    auto measuredRate_kHz = Ratio(i2 - i1, ts2 - ts1);
    for (int64_t i = 0; i < mNumSamples; ++i)
        mData[i] = ts1 + ((i - i1 - mSampleBlockSize) / measuredRate_kHz).To<double>();

    mMeasuredRate_Hz = measuredRate_kHz * 1000;
    mMeasuredRate_Hz.Normalize();
    mNominalRate_Hz.Normalize();
    mSampleDuration = 1 / mMeasuredRate_Hz.To<double>() / context()->timestampUnit->Gain();
    mBlockDuration = mSampleBlockSize * mSampleDuration;
    mFileDuration = mNumSamples * mSampleDuration;
    double relativeError = ::fabs((mMeasuredRate_Hz / mNominalRate_Hz).To<double>() - 1);
    mRsq = 1 - relativeError * relativeError;
    if (mRsq < 0.9)
        invalidate();
}

double SampleClock::position() const
{
    return data()[*context()->sample] - data().front();
}

double SampleClock::fileDuration() const
{
    data();
    return mFileDuration;
}

double SampleClock::sampleDuration() const
{
    data();
    return mSampleDuration;
}

double SampleClock::blockDuration() const
{
    data();
    return mBlockDuration;
}

double SampleClock::blocksIgnoredAtBegin() const
{
    return mIgnoreBlocksAtBegin;
}

double SampleClock::blocksIgnoredAtEnd() const
{
    return mIgnoreBlocksAtEnd;
}

double SampleClock::nominalRate_Hz() const
{
    data();
    return mNominalRate_Hz.To<double>();
}

double SampleClock::measuredRate_Hz() const
{
    data();
    return mMeasuredRate_Hz.To<double>();
}

double SampleClock::rsq() const
{
    data();
    return mRsq;
}

std::string SampleClock::onSummary() const
{
    std::ostringstream oss;
    oss << "f=" << Pretty(measuredRate_Hz()) << "Hz, r\xc2\xb2=" << std::setprecision(2) << Pretty(rsq());
    return oss.str();
}

ExpressionParser::Node *SampleClock::onMemberFunction(Context *p, const std::string &name,
                                                      const ExpressionParser::NodeList &args)
{
    if (!::stricmp(name.c_str(), "position") && args.size() == 0)
        return new Func0<SampleClock, &SampleClock::position>(this);
    else if (!::stricmp(name.c_str(), "fileDuration") && args.size() == 0)
        return new Func0<SampleClock, &SampleClock::fileDuration>(this);
    else if (!::stricmp(name.c_str(), "sampleDuration") && args.size() == 0)
        return new Func0<SampleClock, &SampleClock::sampleDuration>(this);
    else if (!::stricmp(name.c_str(), "blockDuration") && args.size() == 0)
        return new Func0<SampleClock, &SampleClock::blockDuration>(this);
    else if (!::stricmp(name.c_str(), "blocksIgnoredAtBegin") && args.size() == 0)
        return new Func0<SampleClock, &SampleClock::blocksIgnoredAtBegin>(this);
    else if (!::stricmp(name.c_str(), "blocksIgnoredAtEnd") && args.size() == 0)
        return new Func0<SampleClock, &SampleClock::blocksIgnoredAtEnd>(this);
    else if (!::stricmp(name.c_str(), "nominalSamplingRateHz") && args.size() == 0)
        return new Func0<SampleClock, &SampleClock::nominalRate_Hz>(this);
    else if (!::stricmp(name.c_str(), "measuredSamplingRateHz") && args.size() == 0)
        return new Func0<SampleClock, &SampleClock::measuredRate_Hz>(this);
    else if (!::stricmp(name.c_str(), "rsq") && args.size() == 0)
        return new Func0<SampleClock, &SampleClock::rsq>(this);
    return SampleDataObject::onMemberFunction(p, name, args);
}

} // namespace AnalysisDataObjects
