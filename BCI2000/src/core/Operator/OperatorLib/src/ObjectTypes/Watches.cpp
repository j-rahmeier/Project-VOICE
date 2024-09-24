////////////////////////////////////////////////////////////////////////////////
// $Id: Watches.cpp 8163 2024-05-25 17:33:44Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A watch object, and a container for watches. A watch consists
//   of a number of expressions which send their values to a UDP port whenever
//   any of them changes.
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
#include "Watches.h"

#include "BCIException.h"
#include "StateMachine.h"
#include "SystemStates.h"
#include "WildcardMatch.h"

#include <algorithm>
#include <iomanip>
#include <limits>
#include <sstream>

static const double cAutoCutoffTimeIntervalDefault = 1e-3; // cutoff auto decimation watches at 1kHz

// Watch
Watch::Watch(CommandInterpreter &inInterpreter, const std::string &inAddress, long inID)
    : mInterpreter(inInterpreter.StateMachine()), mID(inID), mrList(inInterpreter.StateMachine().Watches()),
      mSendMessages(&Watch::SendMessages, this), mThread(&mSendMessages, "Watch Send Messages"),
      mDecimationCarry(0), mDecimation(1), mAutoDecimationTimeIntervalSeconds(cAutoCutoffTimeIntervalDefault)
{
    mThread.Start();
    ScopedLock(mrList);
    if (mID == BCI_None)
    {
        if (inAddress.empty())
        {
            std::string addr, var = inInterpreter.RemoteHostName();
            if (inInterpreter.LocalVariables().Exists(var))
                addr = inInterpreter.LocalVariables()[var];
            addr = inInterpreter.StateMachine().SuggestUDPAddress(addr);
            mSocket.Open(addr);
            if (mSocket.IsOpen())
                mAddress = addr;
        }
        else
        {
            mSocket.Open(inAddress);
            if (mSocket.IsOpen())
            {
                std::string s = mSocket.LocalAddress();
                if (inInterpreter.StateMachine().SuggestUDPAddress(s) == s)
                    mAddress = s;
                else
                    mSocket.Close();
            }
        }
    }
    else
        while (mrList.FindByID(mID))
            ++mID;
    mrList.Add(this);
}

void Watch::AboutToDelete()
{
    mrList.Remove(this);
    mThread.Terminate();
}

bool Watch::CheckAndTrigger()
{
    bool triggered = false;
    int i = mDecimationCarry, max = OnLoopMax();
    int decimation = std::max(1, mDecimation);
    for (; i <= max; i += decimation)
    {
        if (OnCheck(i))
        {
            triggered = true;
            OnTrigger();
        }
    }
    mDecimationCarry = i % (max + 1);
    mCheckDone.Set();
    return triggered;
}

void Watch::QueueMessage(double inTime, const std::string &inState)
{
    mBuf = inState;
    Message msg = { inTime, inState, };
    QueueMessage(msg);
}

void Watch::QueueMessage(double inTime, const std::vector<double>& inValues)
{
    mBuf = MarshalValues(inValues);

    Message msg = { inTime, "", inValues };
    QueueMessage(msg);
}

void Watch::QueueMessage(const Message& inMessage)
{
    std::lock_guard<std::mutex> lock(mQueueMutex);
    mQueue.push(inMessage);
}

std::string Watch::MarshalMessage(const Message& inMessage)
{
    constexpr int n = 5; // number of digits required after the decimal point for 100kHz
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(n + 1) << inMessage.timestamp << '\t';
    std::string s = oss.str();
    if (!inMessage.str.empty())
        s += inMessage.str;
    else
        s += MarshalValues(inMessage.val);
    s += "\r\n";
    return s;
}

std::string Watch::MarshalValues(const std::vector<double>& inValues)
{
    std::ostringstream oss;
    oss << std::setprecision(16);
    if (!inValues.empty())
        oss << Pretty(inValues[0]);
    for (size_t idx = 1; idx < inValues.size(); ++idx)
        oss << '\t' << Pretty(inValues[idx]);
    return oss.str();
}

void Watch::SendMessages()
{
    while (mCheckDone.Wait())
    {
        mCheckDone.Reset();
        std::vector<std::string> values;
        std::unique_lock<std::mutex> lock(mQueueMutex);
        while (!mQueue.empty())
        {
            const std::string s = MarshalMessage(mQueue.front());
            if (mSocket.IsOpen())
                mSocket.Write(s.c_str(), s.length() + 1);
            if (mID != BCI_None)
                values.push_back(s);
            mQueue.pop();
        }
        lock.unlock();

        std::vector<const char*> pointers;
        for (const auto& s : values)
            pointers.push_back(s.c_str());
        if (!pointers.empty())
        {
            Interpreter().StateMachine().ExecuteCallback<int, const char* const*>(mID, pointers.size(), pointers.data());
        }
    }
}

// SystemStateWatch
SystemStateWatch::SystemStateWatch(CommandInterpreter &inInterpreter, const std::string &inAddress, long inID)
    : Watch(inInterpreter, inAddress, inID), mState(BCI_None)
{
}

bool SystemStateWatch::OnCheck(int)
{
    int state = BCI_GetStateOfOperation();
    bool result = (state != mState);
    mState = state;
    return result;
}

void SystemStateWatch::OnTrigger()
{
    std::string name = SystemStates::Name(mState);
    if (name.empty())
        name = "<unknown>";
    double timestamp = List().SampleTime(Interpreter().StateMachine().StateVector().Samples());
    QueueMessage(timestamp, name);
}

// ExpressionWatch
ExpressionWatch::ExpressionWatch(CommandInterpreter &inInterpreter, const std::string &inAddress, long inID)
    : Watch(inInterpreter, inAddress, inID), mSample(-1), mOmittedValues(0), mLastQueueTimestamp(NaN<double>())
{
}

void ExpressionWatch::SetExpressions(const std::vector<std::string> &inExpressions)
{
    Disable();
    mExpressions.clear();
    for (size_t i = 0; i < inExpressions.size(); ++i)
    {
        mExpressions.push_back(InterpreterExpression(Interpreter(), inExpressions[i]));
        mExpressions.back().ForbidAssignment().Evaluate();
    }
    mValues.clear();
    mValues.resize(mExpressions.size(), std::numeric_limits<double>::quiet_NaN());
    mMinValues.resize(mValues.size());
    mMaxValues.resize(mValues.size());
    mLastQueueTimestamp = NaN<double>();
    mOmittedValues = 0;
    Enable();
}

int ExpressionWatch::OnLoopMax()
{
    int loopMax = 0;
    for (const auto &expr : mExpressions)
        if (expr.RefersStates())
            loopMax = Interpreter().StateMachine().StateVector().Samples() - 2;
    return std::max(loopMax, 0);
}

bool ExpressionWatch::OnCheck(int inSample)
{
    bool callTrigger = (Decimation() < 0); // always call Trigger() if auto decimation
    size_t idx = 0;
    for (auto& expression : mExpressions)
    {
        double result = expression.Evaluate(inSample);
        if (result != mValues[idx])
        {
            callTrigger = true;
            mValues[idx] = result;
        }
        ++idx;
    }
    if (callTrigger)
      mSample = inSample;
    return callTrigger;
}

void ExpressionWatch::ResetMinMax()
{
    mMinValues = mValues;
    mMaxValues = mValues;
}

void ExpressionWatch::UpdateMinMax()
{
    for (int i = 0; i < mValues.size(); ++i)
    {
        mMinValues[i] = std::min(mMinValues[i], mValues[i]);
        mMaxValues[i] = std::max(mMaxValues[i], mValues[i]);
    }
}

void ExpressionWatch::OnTrigger()
{
    double timestamp = List().SampleTime(mSample);
    if (Decimation() < 0) // auto decimation
    {
        double deltaT = timestamp - mLastQueueTimestamp;
        if (deltaT > 0 && deltaT <= AutoDecimationTimeIntervalSeconds())
        { // omit data point because it is too close in time to the previous one
            UpdateMinMax();
            ++mOmittedValues;
        }
        else
        {
            if (mOmittedValues > 0)
            {   // queue min/max information from previously omitted values
                if (mMinValues == mMaxValues)
                {
                    QueueMessage(mLastQueueTimestamp + AutoDecimationTimeIntervalSeconds(), mMinValues);
                }
                else
                {
                    QueueMessage(mLastQueueTimestamp + AutoDecimationTimeIntervalSeconds() / 3., mMaxValues);
                    QueueMessage(mLastQueueTimestamp + AutoDecimationTimeIntervalSeconds() * 2./ 3., mMinValues);
                    QueueMessage(mLastQueueTimestamp + AutoDecimationTimeIntervalSeconds(), mValues);
                }
            }
            else
            {   // immediately queue single data point
                QueueMessage(timestamp, mValues);
            }
            ResetMinMax();
            mLastQueueTimestamp = timestamp;
            mOmittedValues = 0;
        }
    }
    else
    {   // no auto decimation: immediately queue data point
        QueueMessage(timestamp, mValues);
        mLastQueueTimestamp = timestamp;
        mOmittedValues = 0;
    }
}

// Watch::Set
Watch::Set::Set(const Watch::Set &other) : vector<Watch *>(other), mpList(other.mpList)
{
    mpList->Lock();
}

Watch::Set::Set(const class Watch::List *inList) : mpList(inList)
{
    if (mpList != this)
        mpList->Lock();
}

Watch::Set::~Set()
{
    if (mpList != this)
        mpList->Unlock();
}

Watch *Watch::Set::First() const
{
    Lock_ lock(this);
    return empty() ? 0 : *begin();
}

Watch *Watch::Set::Next(Watch *inEl) const
{
    Lock_ lock(this);
    const_iterator i = find(begin(), end(), inEl);
    if (i != end())
        ++i;
    return i == end() ? 0 : *i;
}

Watch *Watch::Set::FindByID(int inID) const
{
    if (inID == BCI_None)
        return 0;

    Lock_ lock(this);
    for (const_iterator i = begin(); i != end(); ++i)
        if ((*i)->ID() == inID)
            return *i;
    return 0;
}

Watch::Set Watch::Set::SelectByAddress(const std::string &inAddress, options inOpt) const
{
    Set result(mpList);
    struct Comp
    {
        static bool Eq(const std::string &a, const std::string &b)
        {
            return a == b;
        }
        static bool Wild(const std::string &a, const std::string &b)
        {
            return WildcardMatch(a, b);
        }
    };
    bool (*match)(const std::string &, const std::string &) = (inOpt & WildcardYes) ? &Comp::Wild : &Comp::Eq;

    Lock_ lock(this);
    for (const_iterator i = begin(); i != end(); ++i)
        if (!(*i)->Address().empty() && match(inAddress, (*i)->Address()))
            result.push_back(*i);
    return result;
}

// Watch::Set::Lock_
Watch::Set::Lock_::Lock_(const Set *p) : mp(p)
{
    mp->mpList->Lock();
}

Watch::Set::Lock_::~Lock_()
{
    mp->mpList->Unlock();
}

// Watch::List
Watch::List::List()
  : Set(this), 
    mSourceTime(-1), mPrevSourceTime(-1), mBlockDuration(-1), mSampleBlockSize(-1)
{
}

Watch::List::~List()
{
    Lock_ lock(this);
    for (auto pWatch : *this)
        delete pWatch;
}

double Watch::List::SampleTime(int inSample) const
{
  if (mBlockDuration <= 0)
    return -1; // no common time stamp between Operator and Source

  // We use the previous time stamp to determine the actual block duration, which may
  // differ considerably from average block duration.
  // This way, we avoid sample times from the end of a block to overlap with the beginning
  // of the next block.
  double blockBegin = mPrevSourceTime;
  double blockDuration = mSourceTime - mPrevSourceTime;
  return blockBegin + (blockDuration * inSample) / mSampleBlockSize;
}

void Watch::List::Add(Watch *inpWatch)
{
    Lock_ lock(this);
    if (find(begin(), end(), inpWatch) == end())
        push_back(inpWatch);
}

void Watch::List::Remove(Watch *inpWatch)
{
    Lock_ lock(this);
    iterator i = find(begin(), end(), inpWatch);
    if (i != end())
        erase(i);
}

void Watch::List::Check(int samples, double blockDuration, PrecisionTime sourceTime, bool discont)
{
    Lock_ lock(this);
    mSampleBlockSize = samples;
    mBlockDuration = blockDuration;

    // If the block duration is greater 0, source module and Operator are running on the
    // same machine, and the source time timestamp is a valid 16-bit PrecisionTime.
    // We complete this to a full 64 bit timestamp by using current time from the same source.
    uint64_t now = TimeUtils::MonotonicTime().ToMilliseconds();
    auto diff = PrecisionTime::SignedDiff(PrecisionTime(now), sourceTime);
    uint64_t fullTimeStamp = now - diff;
    double fullSourceTime = fullTimeStamp * 1e-3;

    if (discont)
    {
        mSourceTime = now * 1e-3;
        mPrevSourceTime = mSourceTime;
    }
    else if (mPrevSourceTime < 0)
    {
        mSourceTime = fullSourceTime;
        mPrevSourceTime = mSourceTime - mBlockDuration;
    }
    else if (fullSourceTime != mSourceTime)
    {
        mPrevSourceTime = mSourceTime;
        mSourceTime = fullSourceTime;
    }

    for (iterator i = begin(); i != end(); ++i)
        (*i)->Check();

    mBlockDuration = 0;
}
