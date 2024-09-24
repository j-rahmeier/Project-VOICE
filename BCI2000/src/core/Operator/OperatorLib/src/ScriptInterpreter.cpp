////////////////////////////////////////////////////////////////////////////////
// $Id: ScriptInterpreter.cpp 7331 2023-04-24 15:41:59Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A class that encapsulates interpretation of operator scripts.
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
#include "ScriptInterpreter.h"

#include "BCIException.h"
#include "BCI_OperatorLib.h"
#include "Script.h"
#include "Thread.h"

namespace
{
// A self-deleting thread that executes a script asynchronously.
class ScriptThread : private ScriptInterpreter, Thread
{
  public:
    ScriptThread(class StateMachine &);
    void Execute(const std::string &, const std::string &, const std::vector<std::string> &);

  private:
    int OnExecute();

    std::string mScript, mName;
    std::vector<std::string> mArgs;
};
} // namespace

ScriptInterpreter::ScriptInterpreter(const ScriptInterpreter &inInterpreter)
    : CommandInterpreter(inInterpreter.StateMachine())
{
}

ScriptInterpreter::ScriptInterpreter(class StateMachine &inStateMachine) : CommandInterpreter(inStateMachine)
{
}

ScriptInterpreter::~ScriptInterpreter()
{
}

bool ScriptInterpreter::Execute(const std::string& inScript, const std::string& inName)
{
    bool success = false;
    try
    {
        Script(inScript, inName).Compile().Execute(*this);
        success = true;
    }
    catch (const bci::Exception& e)
    {
        OnScriptError(e.What());
    }
    catch (const std::exception& e)
    {
        OnScriptError(e.what());
    }
    return success;
}

bool ScriptInterpreter::ExecuteAsSubroutine(const std::string &inScript, const std::string &inName, const std::vector<std::string> &inArgs)
{
    bool success = false;
    try
    {
        Script(inScript, inName).Compile().ExecuteAsSubroutine(*this, inArgs);
        success = true;
    }
    catch (const bci::Exception &e)
    {
        OnScriptError(e.What());
    }
    catch (const std::exception &e)
    {
        OnScriptError(e.what());
    }
    return success;
}

void ScriptInterpreter::ExecuteAsynchronously(const std::string &inScript, const std::string &inName, const std::vector<std::string> &inArgs)
{
    (new ScriptThread(this->StateMachine()))->Execute(inScript, inName, inArgs);
}

std::string ScriptInterpreter::Result() const
{
    return CommandInterpreter::Result();
}

void ScriptInterpreter::Abort()
{
    CommandInterpreter::Abort();
}

void ScriptInterpreter::OnScriptError(const std::string &inMessage)
{
    StateMachine().ExecuteCallback(BCI_OnScriptError, inMessage.c_str());
}

// ScriptThread definitions
ScriptThread::ScriptThread(class StateMachine &s) : Thread(true), ScriptInterpreter(s)
{
}

void ScriptThread::Execute(const std::string &inScript, const std::string &inName, const std::vector<std::string> &inArgs)
{
    mScript = inScript;
    mName = inName;
    mArgs = inArgs;
    Thread::Start();
}

int ScriptThread::OnExecute()
{
    ScriptInterpreter::ExecuteAsSubroutine(mScript, mName, mArgs);
    return 0;
}
