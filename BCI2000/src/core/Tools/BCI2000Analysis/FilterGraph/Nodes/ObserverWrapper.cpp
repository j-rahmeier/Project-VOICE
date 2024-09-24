////////////////////////////////////////////////////////////////////////////////
// $Id: ObserverWrapper.cpp 7720 2023-11-20 18:50:11Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A wrapper filter that observes its input signal, and 
//   saves it for later classifier training.
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
#include "ObserverWrapper.h"
#include "Expression.h"
#include "Environment.h"
#include <vector>

struct ObserverBase::Private : Environment
{
    bool mObserve = false;
    Expression mCondition1, mCondition2;
    bool mPrevCondition1 = false, mPrevCondition2 = false;
    int mIdx1 = 0, mIdx2 = 0;
    std::vector<GenericSignal> mData1, mData2;
    std::vector<std::vector<int>> mTrials1, mTrials2;
    std::vector<int>* mpTrial1 = nullptr, *mpTrial2 = nullptr;

    Private();
    void reset();
    void initialize(const SignalProperties &);
    void observe(const GenericSignal &);
};

ObserverBase::Private::Private()
{
}

void ObserverBase::Private::reset()
{
    mPrevCondition1 = false;
    mIdx1 = 0;
    mpTrial1 = nullptr;
    mData1.clear();
    mTrials1.clear();
    mPrevCondition2 = false;
    mIdx2 = 0;
    mpTrial2 = nullptr;
    mData2.clear();
    mTrials2.clear();
}

void ObserverBase::Private::initialize(const SignalProperties &Input)
{
    reset();
}

void ObserverBase::Private::observe(const GenericSignal &Input)
{
    bool condition1 = mCondition1.Evaluate(&Input),
         condition2 = mCondition2.Evaluate(&Input);
    if (condition1 && !mPrevCondition1)
    {
        mTrials1.push_back(std::vector<int>());
        mpTrial1 = &mTrials1.back();
    }
    else if (mPrevCondition1 && !condition1)
        mpTrial1 = nullptr;
    mPrevCondition1 = condition1;
    if (condition1)
    {
        mData1.push_back(Input);
        mpTrial1->push_back(mIdx1++);
    }

    if (condition2 && !mPrevCondition2)
    {
        mTrials2.push_back(std::vector<int>());
        mpTrial2 = &mTrials2.back();
    }
    else if (mPrevCondition2 && !condition2)
        mpTrial2 = nullptr;
    mPrevCondition2 = condition2;
    if (condition2)
    {
        mData2.push_back(Input);
        mpTrial2->push_back(mIdx2++);
    }
}

ObserverBase::ObserverBase() : p(new Private)
{
}

ObserverBase::~ObserverBase()
{
    delete p;
}

void ObserverBase::setObserve(bool b)
{
    p->mObserve = b;
}

void ObserverBase::setCondition1(const std::string &s)
{
    p->mCondition1 = s;
}

void ObserverBase::setCondition2(const std::string &s)
{
    p->mCondition2 = s;
}

void ObserverBase::resetStatistics()
{
    p->reset();
}

const std::vector<GenericSignal> &ObserverBase::data1() const
{
    return p->mData1;
}

const std::vector<GenericSignal> &ObserverBase::data2() const
{
    return p->mData2;
}

const std::vector<std::vector<int>>& ObserverBase::trials1() const
{
    return p->mTrials1;
}

const std::vector<std::vector<int>>& ObserverBase::trials2() const
{
    return p->mTrials2;
}

void ObserverBase::onPreflight(const SignalProperties &Input, SignalProperties &Output) const
{
    GenericSignal preflightSignal(Input);
    p->mCondition1.Evaluate(&preflightSignal);
    p->mCondition2.Evaluate(&preflightSignal);
}

void ObserverBase::onInitialize(const SignalProperties &Input, const SignalProperties &Output)
{
    p->initialize(Input);
}

void ObserverBase::onProcess(const GenericSignal &Input, GenericSignal &Output)
{
    if (p->mObserve)
        p->observe(Input);
}
