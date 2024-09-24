#include "EventQueue.h"
//////////////////////////////////////////////////////////////////////
// $Id: EventQueue.cpp 7986 2024-04-03 11:12:44Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A thread-safe event queue.
//   An event is defined by a string description, and a time stamp.
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
#include "EventQueue.h"

#include "Exception.h"
#include "StateVector.h"
#include "StateList.h"
#include "BCIStream.h"

bool EventQueue::Compare::operator()(const Element& a, const Element& b) const
{
    /*
    From https://en.cppreference.com/w/cpp/container/priority_queue:
    Note that the Compare parameter is defined such that it returns true if its 
    first argument comes before its second argument in a weak ordering.
    But because the priority queue outputs largest elements first, the elements 
    that "come before" are actually output last.That is, the front of the queue 
    contains the "last" element according to the weak ordering imposed by Compare.
    */
    return PrecisionTime::SignedDiff(a.second, b.second) > 0;
}

void EventQueue::PushBack(const std::string& inDescriptor, PrecisionTime inTimeStamp)
{
  if (!mEventsAllowed)
    throw std_runtime_error << "No events allowed when receiving \"" << inDescriptor
    << "\" event "
    "-- trying to record events outside the \"running\" state?";
  std::lock_guard<std::mutex> lock(mMutex);
  if (mQueue.size() == mMaxCount - 1)
    throw std_runtime_error << "Event queue length reached safety maximum of " << mMaxCount;
  if (mQueue.size() < mMaxCount - 1)
    mQueue.push(make_pair(inDescriptor, inTimeStamp));
}

void EventQueue::PopFront()
{
  std::lock_guard<std::mutex> lock(mMutex);
  mQueue.pop();
}

bool EventQueue::IsEmpty()
{
  std::lock_guard<std::mutex> lock(mMutex);
  return mQueue.empty();
}

const std::string& EventQueue::FrontDescriptor() const
{
  std::lock_guard<std::mutex> lock(mMutex);
  return mQueue.top().first;
}

PrecisionTime EventQueue::FrontTimeStamp() const
{
  std::lock_guard<std::mutex> lock(mMutex);
  return mQueue.top().second;
}

void EventQueue::ProcessEvents(StateVector* inpStatevector, double inBlockDuration)
{
    // When translating event time stamps into sample positions, we assume a block's
    // source time stamp to match the subsequent block's first sample.
    PrecisionTime sourceTime = static_cast<PrecisionTime::NumType>(inpStatevector->StateValue("SourceTime"));
    int sampleBlockSize = inpStatevector->Samples() - 1;
    Assert(sampleBlockSize > 0);
    const StateList& states = inpStatevector->StateList();

    std::vector<std::pair<std::string, PrecisionTime>> repost;

    while (!IsEmpty() && PrecisionTime::SignedDiff(FrontTimeStamp(), sourceTime) < 0)
    {
        int offset = std::round(
            ((inBlockDuration - PrecisionTime::UnsignedDiff(sourceTime, FrontTimeStamp())) *
                sampleBlockSize) / inBlockDuration);
        std::istringstream iss(FrontDescriptor());
        std::string name;
        iss >> name;
        State::ValueType value;
        iss >> value;
        int duration;
        if (!(iss >> duration))
            duration = -1;

        const class State& state = states.ByName(name);
        int kind = state.Kind();
        if (kind != State::EventKind)
            throw std_invalid_argument << "Trying to set state \"" << name
            << "\" from an event. "
            "This state was not defined as an event state. "
            "Use BEGIN_EVENT_DEFINITIONS/END_EVENT_DEFINITIONS to define states "
            "as event states.";

        offset = std::max(offset, 0);
        if (duration < 0)
        { // No duration given -- set the state at the current and following positions.
            inpStatevector->SetStateValue(state.Location(), state.Length(), offset, value);
        }
        else if (duration == 0)
        { // Set the state at a single position only.
            // For zero duration events, avoid overwriting a previous event by
            // moving the current one if possible, and reposting if not.
            State::ValueType val = inpStatevector->CarryoverValue(state.Location(), state.Length());
            while (offset < sampleBlockSize &&
                inpStatevector->StateValue(state.Location(), state.Length(), offset) != val)
                ++offset;
            if (offset == sampleBlockSize) // Re-post the event to be processed in the next block
                repost.push_back(std::make_pair(FrontDescriptor(), FrontTimeStamp()));
            else
                inpStatevector->SetSampleValue(state.Location(), state.Length(), offset, value);
        }
        else
        {
            bcierr__ << "Event durations > 0 are currently unsupported "
                << "(" << iss.str() << ")";
        }
        PopFront();
    }
    for (const auto& r : repost)
        PushBack(r.first, r.second);
}

