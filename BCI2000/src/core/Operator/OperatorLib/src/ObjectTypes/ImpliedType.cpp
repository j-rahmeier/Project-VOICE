////////////////////////////////////////////////////////////////////////////////
// $Id: ImpliedType.cpp 8076 2024-04-25 16:17:14Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A script interpreter type that handles global commands.
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
#include "ImpliedType.h"

#include "ConditionType.h"
#include "EventTypes.h"
#include "ExpressionType.h"
#include "FileSystemTypes.h"
#include "LineType.h"
#include "MessageTypes.h"
#include "ParameterTypes.h"
#include "ProcessType.h"
#include "SignalTypes.h"
#include "StateTypes.h"
#include "SystemTypes.h"
#include "VariableTypes.h"
#include "WatchTypes.h"

#include "BCIException.h"
#include "BCI_OperatorLib.h"
#include "CommandInterpreter.h"
#include "StateMachine.h"

#include "EnvVariable.h"
#include "FileUtils.h"
#include "ParserToken.h"
#include "ProcessUtils.h"

namespace Interpreter
{

ImpliedType ImpliedType::sInstance;
const ObjectType::MethodEntry ImpliedType::sMethodTable[] = {
    METHOD(Get),      METHOD(Set),        METHOD(Print),     {"Decode", &Print}, METHOD(Encode),
    METHOD(Wait),     METHOD(Sleep),      METHOD(SetConfig), METHOD(Start),      {"Resume", &Start},
    METHOD(Stop),     {"Suspend", &Stop}, METHOD(Startup),   METHOD(Shutdown),   METHOD(Reset),
    METHOD(Quit),     {"Exit", &Quit},    METHOD(Version),   METHOD(Abort),      METHOD(Log),
    METHOD(Warn),     METHOD(Error),      METHOD(System),    METHOD(Echo),       METHOD(Ls),
    {"Dir", &Ls},     METHOD(Cd),         METHOD(Pwd),       METHOD(MkDir),      METHOD(Realpath),
    METHOD(Basename), METHOD(Dirname),    {"[", &Square},    METHOD(Watch),      END};

void ImpliedType::OnHelp(CommandInterpreter &inInterpreter, bool all) const
{
    inInterpreter.Out() << "Control statements: if, elseif, else, end, while, do, until\n";
    inInterpreter.Out() << "Global commands: ";
    this->ListMethods(inInterpreter.Out(), all);

    const ObjectType *pType = ObjectType::Next(this);
    if (pType && pType != this)
        inInterpreter.Out() << "\nCommands for objects of type";
    while (pType && pType != this)
    {
        std::ostringstream oss;
        pType->ListMethods(oss, all);
        if (!oss.str().empty())
            inInterpreter.Out() << "\n " << pType->Name() << ": " << oss.str();
        pType = ObjectType::Next(pType);
    }
    const char *pHelpString = NULL;
    inInterpreter.StateMachine().ExecuteCallback(BCI_OnScriptHelp, &pHelpString);
    if (pHelpString)
        inInterpreter.Out() << "\nApplication-defined commands: " << pHelpString;
}

bool ImpliedType::Get(CommandInterpreter &inInterpreter)
{
    if (inInterpreter.MatchTokens("Signal(*)"))
        return SignalType::Get(inInterpreter);
    try
    {
        ExpressionType::Evaluate(inInterpreter);
        return true;
    }
    catch (const std::exception &)
    {
    }
    catch (const bci::Exception&)
    {
    }
    inInterpreter.Unget();
    std::string tokens = inInterpreter.GetRemainingTokens();
    if (!::stricmp(tokens.c_str(), "current run file"))
    {
        const auto& s = inInterpreter.StateMachine();
        if (s.SystemState() == StateMachine::Running)
            inInterpreter.Out() << s.CurrentRunFileName();
        else
            inInterpreter.Out() << "";
        return true;
    }
    inInterpreter.Unget();
    std::string object = inInterpreter.GetToken();
    inInterpreter.Unget();
    if (inInterpreter.LocalVariables().Exists(object))
        return VariableType::Get(inInterpreter);
    if (EnvVariable::Get(object, object))
        return EnvironmentType::Get(inInterpreter);
    if (ConditionType::EvaluateExpression(inInterpreter))
        return true;
    return false;
}

bool ImpliedType::Set(CommandInterpreter &inInterpreter)
{
    std::string object = inInterpreter.GetToken();
    inInterpreter.Unget();
    CommandInterpreter::ArgumentList args;
    inInterpreter.ParseArguments(object, args);
    if (inInterpreter.LocalVariables().Exists(object))
        return VariableType::Set(inInterpreter);
    if (EnvVariable::Get(object, object))
        return EnvironmentType::Set(inInterpreter);
    return VariableType::Set(inInterpreter);
}

bool ImpliedType::Print(CommandInterpreter &inInterpreter)
{
    return !!(inInterpreter.Out() << inInterpreter.GetRemainingTokens());
}

bool ImpliedType::Encode(CommandInterpreter &inInterpreter)
{
    std::string line = inInterpreter.GetRemainder();
    std::istringstream iss(line);
    ParserToken token;
    if (iss >> std::ws >> token)
        inInterpreter.Out() << token;
    while (iss >> std::ws >> token)
        inInterpreter.Out() << " " << token;
    return !!inInterpreter.Out();
}

bool ImpliedType::Wait(CommandInterpreter &inInterpreter)
{
    std::string token = inInterpreter.GetToken();
    if (!::stricmp(token.c_str(), "for"))
    {
        std::string object = inInterpreter.GetToken();
        if (!::stricmp(object.c_str(), "process"))
            return Interpreter::ProcessType::WaitFor(inInterpreter);
        inInterpreter.Unget();
        return Interpreter::SystemType::WaitFor(inInterpreter);
    }
    inInterpreter.Unget();
    return Interpreter::SystemType::Sleep(inInterpreter);
}

bool ImpliedType::Sleep(CommandInterpreter &inInterpreter)
{
    return Interpreter::SystemType::Sleep(inInterpreter);
}

bool ImpliedType::SetConfig(CommandInterpreter &inInterpreter)
{
    return SystemType::SetConfig(inInterpreter);
}

bool ImpliedType::Start(CommandInterpreter &inInterpreter)
{
    return SystemType::Start(inInterpreter);
}

bool ImpliedType::Stop(CommandInterpreter &inInterpreter)
{
    return SystemType::Stop(inInterpreter);
}

bool ImpliedType::Startup(CommandInterpreter &inInterpreter)
{
    return SystemType::Startup(inInterpreter);
}

bool ImpliedType::Shutdown(CommandInterpreter &inInterpreter)
{
    return SystemType::Shutdown(inInterpreter);
}

bool ImpliedType::Reset(CommandInterpreter &inInterpreter)
{
    return SystemType::Reset(inInterpreter);
}

bool ImpliedType::Quit(CommandInterpreter &inInterpreter)
{
    return SystemType::Quit(inInterpreter);
}

bool ImpliedType::Version(CommandInterpreter &inInterpreter)
{
    return SystemType::GetVersion(inInterpreter);
}

bool ImpliedType::Abort(CommandInterpreter &inInterpreter)
{
    std::string message = "Execution aborted", args = inInterpreter.GetRemainingTokens();
    if (!args.empty())
    {
        message += ": ";
        message += args;
    }
    throw bciexception << message;
    return true;
}

bool ImpliedType::Log(CommandInterpreter &inInterpreter)
{
    return MessageType::Log(inInterpreter);
}

bool ImpliedType::Warn(CommandInterpreter &inInterpreter)
{
    return WarningType::Issue(inInterpreter);
}

bool ImpliedType::Error(CommandInterpreter &inInterpreter)
{
    return ErrorType::Report(inInterpreter);
}

bool ImpliedType::Echo(CommandInterpreter &inInterpreter)
{
    return LineType::Write(inInterpreter);
}

bool ImpliedType::System(CommandInterpreter &inInterpreter)
{
    std::string command = ProcessType::ToShellArguments(inInterpreter.GetRemainder());
    int exitCode = 0;
    if (!ProcessUtils::SystemExecuteSynchronously(command, inInterpreter.Out(), exitCode))
        throw bciexception << "Could not run \"" << command.c_str() << "\"";
    if (exitCode != 0)
        inInterpreter.Out() << inInterpreter.ExitCodeTag() << exitCode;
    inInterpreter.Log() << "Executed \"" << command.c_str() << "\"";
    return true;
}

bool ImpliedType::Ls(CommandInterpreter &inInterpreter)
{
    return DirectoryType::List(inInterpreter);
}

bool ImpliedType::Cd(CommandInterpreter &inInterpreter)
{
    std::string newWd = inInterpreter.GetToken();
    inInterpreter.Unget();
    if (newWd.empty())
        return Pwd(inInterpreter);
    return DirectoryType::Change(inInterpreter);
}

bool ImpliedType::Pwd(CommandInterpreter &inInterpreter)
{
    return DirectoryType::Current(inInterpreter);
}

bool ImpliedType::MkDir(CommandInterpreter &inInterpreter)
{
    return DirectoryType::Make(inInterpreter);
}

bool ImpliedType::Realpath(CommandInterpreter &inInterpreter)
{
    return PathType::Real(inInterpreter);
}

bool ImpliedType::Basename(CommandInterpreter &inInterpreter)
{
    std::string path = inInterpreter.GetToken(), matchext = inInterpreter.GetOptionalToken(),
                base = FileUtils::ExtractBase(path), ext = FileUtils::ExtractExtension(path);
    if (matchext == ext)
        inInterpreter.Out() << base;
    else
        inInterpreter.Out() << base << ext;
    return true;
}

bool ImpliedType::Dirname(CommandInterpreter &inInterpreter)
{
    std::string path = inInterpreter.GetToken(), dir = FileUtils::ExtractDirectoryS(path);
    if (dir.empty())
        inInterpreter.Out() << ".";
    else
        inInterpreter.Out() << dir;
    return true;
}

bool ImpliedType::Square(CommandInterpreter &inInterpreter)
{
    return ConditionType::Evaluate(inInterpreter);
}

bool ImpliedType::Watch(CommandInterpreter &inInterpreter)
{
    return WatchType::New(inInterpreter);
}

} // namespace Interpreter
