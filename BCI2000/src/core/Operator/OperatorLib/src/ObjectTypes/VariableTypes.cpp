////////////////////////////////////////////////////////////////////////////////
// $Id: VariableTypes.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: Local and environment variable object types for the script
//   interpreter.
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
#include "VariableTypes.h"

#include "BCIException.h"
#include "CommandInterpreter.h"
#include "EnvVariable.h"

namespace Interpreter
{

// VariableType
VariableType VariableType::sInstance;
const ObjectType::MethodEntry VariableType::sMethodTable[] = {
    METHOD(Set), METHOD(Get), METHOD(Clear), {"Drop", &Clear}, END};

bool VariableType::Set(CommandInterpreter &inInterpreter)
{
    std::string name = inInterpreter.GetToken(), value = inInterpreter.GetToken();
    inInterpreter.LocalVariables()[name] = value;
    if (name == inInterpreter.ResultName())
        inInterpreter.Out() << value;
    return true;
}

bool VariableType::Get(CommandInterpreter &inInterpreter)
{
    std::string name = inInterpreter.GetToken();
    if (inInterpreter.LocalVariables().Exists(name))
        inInterpreter.Out() << inInterpreter.LocalVariables()[name];
    return true;
}

bool VariableType::Clear(CommandInterpreter &inInterpreter)
{
    std::string name = inInterpreter.GetToken();
    if (inInterpreter.LocalVariables().Exists(name))
        inInterpreter.LocalVariables().erase(name);
    return true;
}

// EnvironmentType
EnvironmentType EnvironmentType::sInstance;
const ObjectType::MethodEntry EnvironmentType::sMethodTable[] = {
    METHOD(Set), METHOD(Get), METHOD(Clear), {"Drop", &Clear}, END};

bool EnvironmentType::Set(CommandInterpreter &inInterpreter)
{
    std::string name = inInterpreter.GetToken(), value = inInterpreter.GetToken();
    if (!EnvVariable::Set(name, value))
        throw bciexception << "Could not set environment variable \"" << name << "\"";
    return true;
}

bool EnvironmentType::Get(CommandInterpreter &inInterpreter)
{
    std::string name = inInterpreter.GetToken(), value;
    if (EnvVariable::Get(name, value))
        inInterpreter.Out() << value;
    return true;
}

bool EnvironmentType::Clear(CommandInterpreter &inInterpreter)
{
    std::string name = inInterpreter.GetToken();
    if (!EnvVariable::Clear(name))
        throw bciexception << "Could not clear environment variable \"" << name << "\"";
    return true;
}

} // namespace Interpreter
