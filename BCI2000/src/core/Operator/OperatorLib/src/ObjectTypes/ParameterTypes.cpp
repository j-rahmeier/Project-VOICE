////////////////////////////////////////////////////////////////////////////////
// $Id: ParameterTypes.cpp 8291 2024-07-30 16:17:34Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: Parameter-related object types for the script interpreter.
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
#include "ParameterTypes.h"

#include "BCIException.h"
#include "BCI_OperatorLib.h"
#include "CommandInterpreter.h"
#include "FileUtils.h"
#include "Lockable.h"
#include "StateMachine.h"
#include "SystemTypes.h"
#include "WildcardMatch.h"
#include "BCI2000FileReader.h"

#include <cstdlib>

namespace Interpreter
{

//// ParameterType
ParameterType ParameterType::sInstance;
const ObjectType::MethodEntry ParameterType::sMethodTable[] = {METHOD(Set),
                                                               METHOD(Get),
                                                               {"Get actual", &GetActual},
                                                               METHOD(Insert),
                                                               {"Add", &Insert},
                                                               METHOD(List),
                                                               {"Show", &List},
                                                               {"List actual", &ListActual},
                                                               {"Show actual", &ListActual},
                                                               METHOD(Exists),
                                                               {"Is", &Exists},
                                                               END};

bool ParameterType::Set(CommandInterpreter &inInterpreter)
{
    std::string line = inInterpreter.GetRemainder();
    std::istringstream iss(line);
    Param param;
    if (iss >> param)
        WithLock(inInterpreter.StateMachine())
        {
            ParamList &list = inInterpreter.StateMachine().Parameters();
            if (!list.Exists(param.Path()))
                throw bciexception << "Parameter " << param.Path() << " does not exist";
            if (list.ByPath(param.Path()).Section() == "System")
                throw bciexception << "Cannot set parameter from the \"System\" section";
            list.ByPath(param.Path()).AssignValues(param);
            inInterpreter.StateMachine().ExecuteCallback(BCI_OnParameter, line.c_str());
            inInterpreter.StateMachine().ParameterChange();
            inInterpreter.Log() << "Set parameter " << param.Path();
        }
    else
    {
        inInterpreter.Unget();
        std::string name, value, path;
        std::ostringstream oss;
        WithLock(inInterpreter.StateMachine())
        {
            MutableParamRef paramref = GetParamRef(inInterpreter, false);
            if (paramref->Section() == "System")
                throw bciexception << "Cannot set system parameter";
            name = paramref->Name();
            value = inInterpreter.GetToken();
            paramref = value;
            if (!name.empty())
            {
                paramref->InsertInto(oss);
                path = paramref->Path();
            }
        }
        if (!oss.str().empty())
        {
            inInterpreter.StateMachine().ExecuteCallback(BCI_OnParameter, oss.str().c_str());
            inInterpreter.StateMachine().ParameterChange();
            inInterpreter.Log() << "Set parameter " << path << " to " << value;
        }
    }
    return true;
}

bool ParameterType::Get(CommandInterpreter &inInterpreter)
{
    WithLock(inInterpreter.StateMachine()) inInterpreter.Out() << GetParamRef(inInterpreter, false);
    return true;
}

bool ParameterType::GetActual(CommandInterpreter &inInterpreter)
{
    WithLock(inInterpreter.StateMachine()) inInterpreter.Out() << GetParamRef(inInterpreter, true);
    return true;
}

bool ParameterType::Insert(CommandInterpreter &inInterpreter)
{
    std::string line = inInterpreter.GetRemainder();
    std::istringstream iss(line);
    Param param;
    if (!(iss >> param))
        throw bciexception << "Invalid parameter line: " << line;
    WithLock(inInterpreter.StateMachine())
    {
        if (inInterpreter.StateMachine().SystemState() != StateMachine::Idle &&
            inInterpreter.StateMachine().SystemState() != StateMachine::Publishing)
            throw bciexception << "Can not add parameter to list after publishing phase";
        inInterpreter.StateMachine().Parameters().Add(param);
    }
    inInterpreter.StateMachine().ExecuteCallback(BCI_OnParameter, line.c_str());
    inInterpreter.Log().operator<<("Added parameter to list: ") << param.Path();
    return true;
}

bool ParameterType::List(CommandInterpreter &inInterpreter)
{
    WithLock(inInterpreter.StateMachine())
    {
        ParamRef param = GetParamRef(inInterpreter, false);
        inInterpreter.Out() << *param.operator->();
    }
    return true;
}

bool ParameterType::ListActual(CommandInterpreter &inInterpreter)
{
    WithLock(inInterpreter.StateMachine())
    {
        ParamRef param = GetParamRef(inInterpreter, true);
        inInterpreter.Out() << *param.operator->();
    }
    return true;
}

bool ParameterType::Exists(CommandInterpreter &inInterpreter)
{
    WithLock(inInterpreter.StateMachine())
    {
        bool exists = inInterpreter.StateMachine().Parameters().Exists(inInterpreter.GetToken());
        inInterpreter.Out() << (exists ? "true" : "false");
    }
    return true;
}

MutableParamRef ParameterType::GetParamRef(CommandInterpreter &inInterpreter, bool inActual)
{
    std::string name = inInterpreter.GetToken();
    if (name.empty())
        throw bciexception << "Expected a parameter name or path, with optional indices in parentheses";
    CommandInterpreter::ArgumentList args;
    inInterpreter.ParseArguments(name, args);
    if (!inInterpreter.StateMachine().Parameters().Exists(name))
        throw bciexception << "Parameter " << name << " does not exist";

    Param *pParam = nullptr;
    if (inActual && inInterpreter.StateMachine().AutoConfiguredParameters().Exists(name))
        pParam = &inInterpreter.StateMachine().AutoConfiguredParameters().ByPath(name);
    else
        pParam = &inInterpreter.StateMachine().Parameters().ByPath(name);
    if (pParam->NumValues() < 1)
        throw bciexception << "Parameter " << name << " is empty";
    try
    {
        size_t maxIdx = 0, nIdx = 0, i = 0;
        while (args.size() > 0 && i < args.size() - 1)
        {
            nIdx = args[i].size();
            maxIdx = std::max(nIdx, maxIdx);
            size_t idx1 = 0, idx2 = 0;
            if (nIdx > 0)
                idx1 = GetIndex(args[i][0], pParam->RowLabels());
            if (nIdx > 1)
                idx2 = GetIndex(args[i][1], pParam->ColumnLabels());
            switch (nIdx)
            {
            case 0:
                pParam = pParam->Value().ToParam();
                break;
            case 1:
                pParam = pParam->Value(idx1).ToParam();
                break;
            case 2:
                pParam = pParam->Value(idx1, idx2).ToParam();
                break;
            }
            ++i;
        }
        size_t idx1 = ParamRef::none, idx2 = ParamRef::none;
        if (i < args.size())
        {
            nIdx = args[i].size();
            maxIdx = std::max(nIdx, maxIdx);
            if (maxIdx > 2)
                throw bciexception << "Too many (" << maxIdx << ") indices";
            if (nIdx > 0)
                idx1 = GetIndex(args[i][0], pParam->RowLabels());
            if (nIdx > 1)
                idx2 = GetIndex(args[i][1], pParam->ColumnLabels());
        }
        return MutableParamRef(pParam, idx1, idx2);
    }
    catch (const std::exception &e)
    {
        throw bciexception << name << ": " << e.what();
    }
}

size_t ParameterType::GetIndex(const std::string &inAddress, const LabelIndex &inLabels)
{
    int result = 0;
    if (inAddress.find_first_not_of("0123456789") == std::string::npos)
    {
        result = ::atoi(inAddress.c_str()) - 1;
        if (result < 0 || result >= inLabels.Size())
            throw bciexception << "Index " << inAddress << " out of range";
    }
    else if (inLabels.Exists(inAddress))
    {
        result = inLabels[inAddress];
    }
    else
    {
        throw bciexception << "Label " << inAddress << " does not exist";
    }
    return result;
}

//// ParametersType
ParametersType ParametersType::sInstance;
const ObjectType::MethodEntry ParametersType::sMethodTable[] = {
    METHOD(Load), METHOD(Apply), METHOD(List), {"Show", &List}, METHOD(Clear), {"Drop", &Clear}, END};

bool ParametersType::Load(CommandInterpreter &inInterpreter)
{
    return ParameterfileType::Load(inInterpreter);
}

bool ParametersType::List(CommandInterpreter &inInterpreter)
{
    WithLock(inInterpreter.StateMachine())
    {
        std::string pattern = inInterpreter.GetRemainingTokens();
        if (pattern.empty())
            pattern = "*";
        const ParamList &parameters = inInterpreter.StateMachine().Parameters();
        for (int i = 0; i < parameters.Size(); ++i)
            if (WildcardMatch(pattern, parameters.ByIndex(i).Name(), false))
                inInterpreter.Out() << parameters.ByIndex(i) << '\n';
    }
    return true;
}

bool ParametersType::Apply(CommandInterpreter &inInterpreter)
{
    return SystemType::SetConfig(inInterpreter);
}

bool ParametersType::Clear(CommandInterpreter &inInterpreter)
{
    WithLock(inInterpreter.StateMachine())
    {
        if (inInterpreter.StateMachine().SystemState() != StateMachine::Idle)
            throw bciexception << "Must be in idle state to clear parameters";
        inInterpreter.StateMachine().Parameters().Clear();
    }
    return true;
}

//// ParameterfileType
ParameterfileType ParameterfileType::sInstance;
const ObjectType::MethodEntry ParameterfileType::sMethodTable[] = {METHOD(Load), END};

bool ParameterfileType::Load(CommandInterpreter &inInterpreter)
{
    std::string fileName = inInterpreter.GetToken();
    if (fileName.empty())
        throw bciexception << "Must specify a file name";
    fileName = FileUtils::NormalizedPath(FileUtils::AbsolutePath(fileName));
    std::string tempfile;
    BCI2000FileReader dataFileReader(fileName);
    if (dataFileReader.IsOpen()) {
        inInterpreter.StateMachine().Parameters().CopyFrom(*dataFileReader.Parameters(), false);
        inInterpreter.Log() << "Loaded parameters from data file \"" << fileName << "\"";
    }
    else {
        bool ok = false;
        std::string error;
        try {
            ok = inInterpreter.StateMachine().Parameters().Load(fileName, false);
        }
        catch (const std::exception& e) {
            error = e.what();
        }
        if (!ok) {
            throw bciexception << "Could not load \"" << fileName << "\" as a parameter file"
                << (error.empty() ? "" : ": ") << error;
        }
        inInterpreter.Log() << "Loaded parameter file \"" << fileName << "\"";
    }

    inInterpreter.StateMachine().ParameterChange();
    return true;
}

} // namespace Interpreter
