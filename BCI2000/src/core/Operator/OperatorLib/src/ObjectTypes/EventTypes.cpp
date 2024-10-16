////////////////////////////////////////////////////////////////////////////////
// $Id: EventTypes.cpp 8217 2024-06-22 10:06:42Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: Event-related object types for the script interpreter.
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
#include "EventTypes.h"

#include "BCIException.h"
#include "CommandInterpreter.h"
#include "Lockable.h"
#include "StateMachine.h"
#include "StateTypes.h"
#include "WildcardMatch.h"

#include <cstdlib>

namespace Interpreter
{

//// EventType
EventType EventType::sInstance;
const ObjectType::MethodEntry EventType::sMethodTable[] = {
    METHOD(Set),  METHOD(Get),     METHOD(Pulse),  METHOD(Insert),  {"Add", &Insert},
    METHOD(List), {"Show", &List}, METHOD(Exists), {"Is", &Exists}, END};

bool EventType::Set(CommandInterpreter &inInterpreter)
{
    return SetEvent(inInterpreter, -1);
}

bool EventType::Pulse(CommandInterpreter &inInterpreter)
{
    return SetEvent(inInterpreter, 0);
}

bool EventType::Get(CommandInterpreter &inInterpreter)
{
    WithLock(inInterpreter.StateMachine())
    {
        std::string name = inInterpreter.GetToken();
        if (name.empty())
            throw bciexception << "Expected an event name";
        CommandInterpreter::ArgumentList args;
        inInterpreter.ParseArguments(name, args);
        if (args.size() > 1 || (!args.empty() && args.front().size() > 1))
            throw bciexception << "Too many indices given";
        const State& state = GetEvent(name, inInterpreter.StateMachine()); // GetEvent() tests for existence of the state.
        int sample = 1; // NB: user-visible sample index is 1-based
        if (!args.empty() && !args.front().empty())
            sample = ::atoi(args.front().front().c_str());
        int numSamples = inInterpreter.StateMachine().StateVector().Samples();
        if (numSamples > 1)
            --numSamples; // don't expose carryover sample to user
        if (sample < 1 || sample > numSamples)
            throw bciexception << "Sample value " << sample << " out of range for samples in state vector (" << numSamples << ")";
        // All events appear as states in the state vector, and have their values stored there.
        inInterpreter.Out() << inInterpreter.StateMachine().GetStateValue(state.Name().c_str(), sample - 1);
    }
    return true;
}

bool EventType::Insert(CommandInterpreter &inInterpreter)
{
    std::string name = inInterpreter.GetToken();
    std::string line = inInterpreter.GetRemainingTokens(), eventline = name + " " + line + " 0 0";
    State event;
    std::istringstream iss(eventline);
    if (!(iss >> event))
        throw bciexception << "Invalid event definition: " << eventline;
    event.SetKind(State::EventKind);
    WithLock(inInterpreter.StateMachine())
    {
        if (inInterpreter.StateMachine().SystemState() != StateMachine::Idle)
            throw bciexception << "Could not add event " << name << " to list outside idle state";
        inInterpreter.StateMachine().States().Add(event);
    }
    return true;
}

bool EventType::List(CommandInterpreter &inInterpreter)
{
    WithLock(inInterpreter.StateMachine()) inInterpreter.Out() << GetEvent(inInterpreter);
    return true;
}

bool EventType::Exists(CommandInterpreter &inInterpreter)
{
    WithLock(inInterpreter.StateMachine())
    {
        std::string name = inInterpreter.GetToken();
        bool exists = inInterpreter.StateMachine().States().Exists(name);
        if (!exists)
        {
            exists = inInterpreter.StateMachine().States().Exists(name);
            exists = exists && inInterpreter.StateMachine().States().ByName(name).Kind() == State::EventKind;
        }
        inInterpreter.Out() << (exists ? "true" : "false");
    }
    return true;
}

const State &EventType::GetEvent(CommandInterpreter &inInterpreter)
{
    std::string name = inInterpreter.GetToken();
    if (name.empty())
        throw bciexception << "Expected an event name";
    return GetEvent(name, inInterpreter.StateMachine());
}

const State &EventType::GetEvent(const std::string& name, StateMachine& stateMachine)
{
    const State* pEvent = nullptr;
    if (stateMachine.States().Exists(name))
    {
        pEvent = &stateMachine.States().ByName(name);
        if (pEvent->Kind() != State::EventKind)
            pEvent = nullptr;
    }
    if (!pEvent)
        throw bciexception << "Event " << name << " does not exist";
    return *pEvent;
}

bool EventType::SetEvent(CommandInterpreter &inInterpreter, int inDuration)
{
    std::string name;
    State::ValueType value = 0;
    WithLock(inInterpreter.StateMachine())
    {
        const State &event = GetEvent(inInterpreter);
        name = event.Name();
        value = ::atoi(inInterpreter.GetToken().c_str());
        if ((value & ~((1LL << event.Length()) - 1LL)) != 0)
            throw bciexception << "Event value " << value << " out of range";
        if (!inInterpreter.StateMachine().SetEvent(name.c_str(), value, inDuration))
            throw bciexception << "Could not set event " << name << " to " << value;
    }
    std::string duration;
    if (inDuration < 0)
        duration = "indefinite";
    else if (inDuration == 0)
        duration = "single-sample";
    else
        duration = "unsupported";
    inInterpreter.Log(2) << "Set event " << name << " to " << value << " with " << duration << " duration";
    return true;
}

//// EventsType
EventsType EventsType::sInstance;
const ObjectType::MethodEntry EventsType::sMethodTable[] = {
    METHOD(List), {"Show", &List}, METHOD(Clear), {"Drop", &Clear}, END};

bool EventsType::List(CommandInterpreter &inInterpreter)
{
    WithLock(inInterpreter.StateMachine())
    {
        std::string pattern = inInterpreter.GetRemainingTokens();
        if (pattern.empty())
            pattern = "*";
        const StateList &states = inInterpreter.StateMachine().States();
        for (const auto &state : states)
            if (state.Kind() == State::EventKind && WildcardMatch(pattern, state.Name(), false))
                inInterpreter.Out() << state << '\n';
    }
    return true;
}

bool EventsType::Clear(CommandInterpreter &inInterpreter)
{
    WithLock(inInterpreter.StateMachine())
    {
        if (inInterpreter.StateMachine().SystemState() != StateMachine::Idle)
            throw bciexception << "Must be in idle state to clear events";

        inInterpreter.StateMachine().States().DeleteKind(State::EventKind);
    }
    return true;
}

} // namespace Interpreter
