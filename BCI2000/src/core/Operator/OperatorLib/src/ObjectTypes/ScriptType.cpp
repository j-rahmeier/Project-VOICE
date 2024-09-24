////////////////////////////////////////////////////////////////////////////////
// $Id: ScriptType.cpp 7542 2023-08-21 18:35:08Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: Script object type for the script interpreter.
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
#include "ScriptType.h"

#include "BCIException.h"
#include "BCI_OperatorLib.h"
#include "CommandInterpreter.h"
#include "FileUtils.h"
#include "Script.h"
#include "ScriptEvents.h"
#include "StateMachine.h"

namespace Interpreter
{

ScriptType ScriptType::sInstance;
const ObjectType::MethodEntry ScriptType::sMethodTable[] = {
    METHOD(Set), METHOD(Get), {"Show", &Get}, METHOD(Clear), {"Drop", &Clear}, METHOD(Execute), END};

bool ScriptType::Set(CommandInterpreter &inInterpreter)
{
    std::string events = inInterpreter.GetToken(), script = inInterpreter.GetToken();
    SetScript(inInterpreter, events, script);
    return true;
}

bool ScriptType::Clear(CommandInterpreter &inInterpreter)
{
    std::string events = inInterpreter.GetToken();
    SetScript(inInterpreter, events, "");
    return true;
}

bool ScriptType::Get(CommandInterpreter &inInterpreter)
{
    std::string eventName = inInterpreter.GetToken();
    int eventID = EventID(eventName);
    std::string script = inInterpreter.StateMachine().EventScripts().Get(eventID);
    inInterpreter.Out() << script << std::endl;
    return true;
}

bool ScriptType::Execute(CommandInterpreter &inInterpreter)
{
    std::string token = inInterpreter.GetToken();
    int eventID = ScriptEvents::ID(token);
    std::string script, name, abs;
    if (eventID != BCI_None)
    {
        script = inInterpreter.StateMachine().EventScripts().Get(eventID);
        name = token + " script";
    }
    else
    {
        inInterpreter.Unget();
        name = inInterpreter.GetToken();
        if (!FileUtils::IsFile(name))
        {
            const char *ext[] = {".bciscript", ".cmd", ".bat"};
            bool match = false;
            for (size_t i = 0; !match && i < sizeof(ext) / sizeof(*ext); ++i)
                if (FileUtils::IsFile(name + ext[i]))
                    name += ext[i], match = true;
        }
        abs = FileUtils::AbsolutePath(name);
        std::ifstream file(abs);
        if (!file.is_open())
            throw bciexception << "Could not open script file \"" << name << "\"";
        std::getline(file, script, '\0');
    }
    std::vector<std::string> args;
    for (int idx = 1; idx < 10; ++idx)
        args.push_back(inInterpreter.GetOptionalToken());
    Script(script, abs).Compile().ExecuteAsSubroutine(inInterpreter, args);
    return true;
}

int ScriptType::EventID(const std::string &inEventName)
{
    int eventID = ScriptEvents::ID(inEventName);
    if (eventID == BCI_None)
        throw bciexception << "Unknown scripting event: " << inEventName;
    return eventID;
}

void ScriptType::SetScript(CommandInterpreter &inInterpreter, const std::string &inEvents, const std::string &inScript)
{
    std::istringstream iss(inEvents);
    std::string eventName;
    while (std::getline(iss >> std::ws, eventName, '|'))
        inInterpreter.StateMachine().EventScripts().Set(EventID(eventName), inScript);
}

} // namespace Interpreter
