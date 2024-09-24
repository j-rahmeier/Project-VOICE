////////////////////////////////////////////////////////////////////////////////
// $Id: StateTypes.cpp 8179 2024-06-05 11:26:36Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: State-related object types for the script interpreter.
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
#include "StateTypes.h"

#include "BCIException.h"
#include "BCI_OperatorLib.h"
#include "CommandInterpreter.h"
#include "Lockable.h"
#include "State.h"
#include "StateMachine.h"
#include "WildcardMatch.h"

#include <cstdlib>

namespace {

    const State& GetState(const std::string& name, const StateMachine& statemachine)
    {
        if (name.empty())
            throw bciexception << "Expected a state name";
        const State* pState = nullptr;
        if (statemachine.States().Exists(name))
        {
            pState = &statemachine.States().ByName(name);
            if (pState->Kind() != State::StateKind && pState->Kind() != State::StreamKind)
                pState = nullptr;
        }
        if (!pState)
            throw bciexception << "State " << name << " does not exist";
        return *pState;
    }

    const State& GetState(CommandInterpreter& interpreter)
    {
        return GetState(interpreter.GetToken(), interpreter.StateMachine());
    }

}

namespace Interpreter
{

//// StateType
StateType StateType::sInstance;
const ObjectType::MethodEntry StateType::sMethodTable[] = {METHOD(Set),      METHOD(Get),     METHOD(Insert),
                                                           {"Add", &Insert}, METHOD(List),    {"Show", &List},
                                                           METHOD(Exists),   {"Is", &Exists}, END};

bool StateType::Set(CommandInterpreter &inInterpreter)
{
    std::string name;
    State::ValueType value = 0;
    std::ostringstream oss;
    WithLock(inInterpreter.StateMachine())
    {
        const State &state = GetState(inInterpreter);
        name = state.Name();
        value = ::atoi(inInterpreter.GetToken().c_str());
        if (!inInterpreter.StateMachine().SetStateValue(name.c_str(), value))
            throw bciexception << "Could not set state " << name << " to " << value;
        oss << state;
    }
    inInterpreter.StateMachine().ExecuteCallback(BCI_OnState, oss.str().c_str());
    inInterpreter.Log() << "Set state " << name << " to " << value;
    return true;
}

bool StateType::Get(CommandInterpreter &inInterpreter)
{
    auto pStatevector = static_cast<StateVector*>(inInterpreter.GetAuxData("FrozenStatevector"));
    WithLock(inInterpreter.StateMachine())
    {
        std::string name = inInterpreter.GetToken();
        if (name.empty())
            throw bciexception << "Expected a state name";
        CommandInterpreter::ArgumentList args;
        inInterpreter.ParseArguments(name, args);
        if (args.size() > 1 || (!args.empty() && args.front().size() > 1))
            throw bciexception << "Too many indices given";
        const State &state = GetState(name, inInterpreter.StateMachine()); // GetState() tests for existence of the state.
        int sample = 1; // NB: user-visible sample index is 1-based
        if (!args.empty() && !args.front().empty())
            sample = ::atoi(args.front().front().c_str());
        int numSamples = inInterpreter.StateMachine().StateVector().Samples();
        if (numSamples > 1)
            --numSamples; // don't expose carryover sample to user
        if (sample < 1 || sample > numSamples)
            throw bciexception << "Sample value " << sample << " out of range for samples in state vector (" << numSamples << ")";
        if (pStatevector)
            inInterpreter.Out() << pStatevector->StateValue(state.Name(), sample - 1);
        else
            inInterpreter.Out() << inInterpreter.StateMachine().GetStateValue(state.Name().c_str(), sample - 1);
    }
    return true;
}

bool StateType::Insert(CommandInterpreter &inInterpreter)
{
    std::string name = inInterpreter.GetToken();
    std::string line = inInterpreter.GetRemainder(), stateline = name + " " + line + " 0 0";
    State state;
    std::istringstream iss(stateline);
    if (!(iss >> state))
        throw bciexception << "Invalid state definition";
    WithLock(inInterpreter.StateMachine())
    {
        switch (inInterpreter.StateMachine().SystemState())
        {
        case StateMachine::Idle:
        case StateMachine::WaitingForConnection:
        case StateMachine::Publishing:
        case StateMachine::Information:
            break;
        default:
            throw bciexception << "Could not add state " << name << " to list after information phase";
        }
        inInterpreter.StateMachine().States().Add(state);
    }
    inInterpreter.StateMachine().ExecuteCallback(BCI_OnState, stateline.c_str());
    inInterpreter.Log() << "Added state " << name << " to list";
    return true;
}

bool StateType::List(CommandInterpreter &inInterpreter)
{
    WithLock(inInterpreter.StateMachine()) inInterpreter.Out() << GetState(inInterpreter);
    return true;
}

bool StateType::Exists(CommandInterpreter &inInterpreter)
{
    WithLock(inInterpreter.StateMachine())
    {
        std::string name = inInterpreter.GetToken();
        bool exists = inInterpreter.StateMachine().States().Exists(name);
        if (exists)
        {
            auto kind = inInterpreter.StateMachine().States().ByName(name).Kind();
            exists = (kind == State::StateKind || kind == State::StreamKind);
        }
        inInterpreter.Out() << (exists ? "true" : "false");
    }
    return true;
}

//// StatesType
StatesType StatesType::sInstance;
const ObjectType::MethodEntry StatesType::sMethodTable[] = {
    METHOD(Set),
    METHOD(List), {"Show", &List},
    METHOD(Clear), {"Drop", &Clear},
    METHOD(Freeze), METHOD(Thaw), {"Unfreeze", &Thaw},
    END};

bool StatesType::Set(CommandInterpreter& inInterpreter)
{
    std::vector<std::string> statedefs, logmessages;
    WithLock(inInterpreter.StateMachine())
    {
        while (true)
        {
            std::string name = inInterpreter.GetOptionalToken();
            if (name.empty() && statedefs.empty())
                throw bciexception << "At least one state must be given";
            else if (name.empty())
                break;
            const State& state = GetState(name, inInterpreter.StateMachine());
            State::ValueType value = ::atoi(inInterpreter.GetToken().c_str());
            if (!inInterpreter.StateMachine().SetStateValue(name.c_str(), value))
                throw bciexception << "Could not set state " << name << " to " << value;
            std::ostringstream oss;
            oss << state;
            statedefs.push_back(oss.str());
            logmessages.push_back("Set state " + name + " to " + std::to_string(value));
        }
    }
    for (int i = 0; i < statedefs.size(); ++i)
    {
        inInterpreter.StateMachine().ExecuteCallback(BCI_OnState, statedefs[i].c_str());
        inInterpreter.Log() << logmessages[i];
    }
    return true;
}

bool StatesType::List(CommandInterpreter &inInterpreter)
{
    WithLock(inInterpreter.StateMachine())
    {
        std::string pattern = inInterpreter.GetRemainingTokens();
        if (pattern.empty())
            pattern = "*";
        const StateList &states = inInterpreter.StateMachine().States();
        for (const auto& state : states)
        {
            bool display = (state.Kind() == State::StateKind || state.Kind() == State::StreamKind);
            display = display && WildcardMatch(pattern, state.Name(), false);
            if (display)
                inInterpreter.Out() << state << '\n';
        }
    }
    return true;
}

bool StatesType::Clear(CommandInterpreter &inInterpreter)
{
    WithLock(inInterpreter.StateMachine())
    {
        if (inInterpreter.StateMachine().SystemState() != StateMachine::Idle)
            throw bciexception << "Must be in idle state to clear states";
        inInterpreter.StateMachine().States().Clear();
    }
    return true;
}

bool StatesType::Freeze(CommandInterpreter& inInterpreter)
{
    void* data = inInterpreter.GetAuxData("FrozenStatevector");
    if (data)
        throw bciexception << "States are frozen already, must be thawed first";
    WithLock(inInterpreter.StateMachine())
        data = new StateVector(inInterpreter.StateMachine().StateVector());
    inInterpreter.SetAuxData("FrozenStatevector", data,
        [](void* d) {delete static_cast<StateVector*>(d); });
    return true;
}

bool StatesType::Thaw(CommandInterpreter& inInterpreter)
{
    void* data = inInterpreter.GetAuxData("FrozenStatevector");
    if (!data)
        throw bciexception << "States have not been frozen";
    inInterpreter.ClearAuxData("FrozenStatevector");
    return true;
}

//// StateVectorType
StateVectorType StateVectorType::sInstance;
const ObjectType::MethodEntry StateVectorType::sMethodTable[] = {
    METHOD(Get),
    END };

bool StateVectorType::Get(CommandInterpreter& inInterpreter)
{
    std::string property = inInterpreter.GetToken();
    if (property.empty())
        throw bciexception << "No property given";
    bool ok = false;
    for (const char* pProperty : {"Samples", "SampleCount", "NumSamples"})
        if (!::stricmp(property.c_str(), pProperty))
                ok = true;
    if (!ok)
        throw bciexception << "StateVector has no property " << property;
    int numSamples = inInterpreter.StateMachine().StateVector().Samples();
    if (numSamples > 1)
        --numSamples; // hide carryover sample from user
    inInterpreter.Out() << numSamples;
    return true;
}

} // namespace Interpreter
