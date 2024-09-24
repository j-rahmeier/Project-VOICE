////////////////////////////////////////////////////////////////////////////////
// $Id: Classifier.cpp 7726 2023-11-22 18:04:27Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A base class for linear classifier training algorithms.
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
#include "Classifier.h"
#include "Exception.h"
#include <atomic>
#include <numeric>
#include <thread>

Classifier::Classifier()
: mDownsamplingFactor(1), mOutputDimension(1)
{
    mCallback.callback = nullptr;
    mCallback.data = nullptr;
#if BCIDEBUG
    std::vector<double> v(40);
    for (auto &d : v)
        d = 1.0 * ::rand() / RAND_MAX;
    mDownsamplingFactor = 11;
    GenericSignal s(4, 100);
    upsampleData(v, s);
    std::vector<double> v2;
    downsampleData(s, v2);
    Assert(v.size() == v2.size());
    for (size_t i = 0; i < v.size(); ++i)
        Assert(::fabs(v[i] - v2[i]) < 1e-5);
#endif
    mDownsamplingFactor = 1;
}

void Classifier::setTrainingCallback(Callback callback, void *data)
{
    mCallback.callback = callback;
    mCallback.data = data;
}

void Classifier::setTrainingCallback(const CallbackDef &def)
{
    mCallback = def;
}

const Classifier::CallbackDef &Classifier::trainingCallback() const
{
    return mCallback;
}

void Classifier::setDownsamplingFactor(int f)
{
    if (f < 1)
        f = 1;
    mDownsamplingFactor = f;
}

int Classifier::downsamplingFactor() const
{
    return mDownsamplingFactor;
}

void Classifier::setOutputDimension(int dim)
{
    mOutputDimension = dim;
}

int Classifier::outputDimension() const
{
    return mOutputDimension;
}

void Classifier::clear()
{
    onClear();
}

void Classifier::train(const std::vector<GenericSignal> &data1, const std::vector<GenericSignal> &data2)
{
    if (data1.empty() || data2.empty())
        throw std_exception << "Empty dataset in classifier training input";
    int count = 0;
    size_t n = data1.front().Channels() * data1.front().Elements();
    std::vector<double> buf;
    for (const auto &s : data1)
    {
        if (s.Channels() * s.Elements() != n)
            throw std_exception << "Inconsistent dataset in classifier training input";
        downsampleData(s, buf);
        if (count == 0)
            onInitialize(data1.size() + data2.size(), buf.size());
        onObserve(-1, buf);
        if (mCallback.callback)
            mCallback.callback(count, mCallback.data);
        ++count;
    }
    for (const auto &s : data2)
    {
        if (s.Channels() * s.Elements() != n)
            throw std_exception << "Inconsistent dataset in classifier training input";
        downsampleData(s, buf);
        onObserve(1, buf);
        if (mCallback.callback)
            mCallback.callback(count, mCallback.data);
        ++count;
    }
    // Run potentially lengthy computation in a background thread, while calling
    // GUI callback in the foreground.
    std::atomic<bool> done(false);
    std::thread background([&] {
        onFinalize(mWeights);
        apply(data1, data2);
        done = true;
    });
    while (!done)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        mCallback.callback(count + 1, mCallback.data);
    }
    background.join();

    auto prop = data1.front().Properties();
    prop.ValueUnit() = PhysicalUnit();
    mWeightsAsSignal = GenericSignal(prop);
    upsampleData(mWeights, mWeightsAsSignal);
}

double Classifier::classify(const GenericSignal &s) const
{
    std::vector<double> buf;
    downsampleData(s, buf);
    double value = std::inner_product(buf.begin(), buf.end(), mWeights.begin(), 0.0);
    if (value < mOffset)
        return -1;
    return 1;
}

const std::vector<double> &Classifier::weights() const
{
    return mWeights;
}

const GenericSignal &Classifier::weightsAsSignal() const
{
    return mWeightsAsSignal;
}

double Classifier::offset() const
{
    return mOffset;
}

double Classifier::meanDiff() const
{
    return mMeanDiff;
}

double Classifier::variance() const
{
    return mVariance;
}

void Classifier::normalizeVariance()
{
    double sdev = ::sqrt(mVariance);
    if (mMeanDiff < 0)
        sdev *= -1;
    for (auto& weight : mWeights)
        weight /= sdev;
    mOffset /= sdev;
    mMeanDiff /= sdev;
    mVariance = 1;
}

void Classifier::apply(const std::vector<GenericSignal> &data1, const std::vector<GenericSignal> &data2)
{
    double s1 = 0, s2 = 0;
    std::vector<double> buf, val1, val2;
    val1.reserve(data1.size());
    val2.reserve(data2.size());
    for (int i = 0; i < data1.size(); ++i)
    {
        downsampleData(data1[i], buf);
        Assert(buf.size() == mWeights.size());
        double value = std::inner_product(buf.begin(), buf.end(), mWeights.begin(), 0.0);
        s1 += value;
        val1.push_back(value);
    }
    for (int i = 0; i < data2.size(); ++i)
    {
        downsampleData(data2[i], buf);
        Assert(buf.size() == mWeights.size());
        double value = std::inner_product(buf.begin(), buf.end(), mWeights.begin(), 0.0);
        s2 += value;
        val2.push_back(value);
    }
    double n1 = data1.size(), n2 = data2.size();
    double m1 = s1 / n1, m2 = s2 / n2;
    mOffset = (m2 + m1) / 2;
    mMeanDiff = m2 - m1;

    double ssq1 = 0;
    for (auto value : val1)
    {
        value -= m1;
        ssq1 += value * value;
    }
    double ssq2 = 0;
    for (auto value : val2)
    {
        value -= m2;
        ssq2 += value * value;
    }

    mVariance = (ssq1 + ssq2) / (n1 + n2);
}

void Classifier::downsampleData(const GenericSignal &s, std::vector<double> &v) const
{
    int newElements = (s.Elements() + mDownsamplingFactor - 1) / mDownsamplingFactor;
    v.resize(s.Channels() * newElements);
    size_t k = 0;
    for (int ch = 0; ch < s.Channels(); ++ch)
    {
        for (int el = 0; el < s.Elements(); el += mDownsamplingFactor)
        {
            double val = 0;
            int i = 0;
            while (i < mDownsamplingFactor)
            {
                if (el + i >= s.Elements())
                    break;
                val += s(ch, el + i);
                ++i;
            }
            v[k++] = val / i;
        }
    }
}

void Classifier::upsampleData(const std::vector<double> &v, GenericSignal &s) const
{
    int dsElements = (s.Elements() + mDownsamplingFactor - 1) / mDownsamplingFactor;
    for (size_t i = 0; i < v.size(); ++i)
    {
        int ch = i / dsElements, el = (i % dsElements) * mDownsamplingFactor;
        for (int j = el; j < std::min(el + mDownsamplingFactor, s.Elements()); ++j)
            s(ch, j) = v[i];
    }
}
