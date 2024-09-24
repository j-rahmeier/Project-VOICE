////////////////////////////////////////////////////////////////////////////////
// $Id: LineType.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: I/O Line object type for the script interpreter.
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
#include "LineType.h"

#include "BCIException.h"
#include "ClassName.h"
#include "CommandInterpreter.h"
#include "ParserToken.h"

namespace Interpreter
{

LineType LineType::sInstance;
const ObjectType::MethodEntry LineType::sMethodTable[] = {METHOD(Read), {"Get", &Read}, METHOD(Write), END};

bool LineType::Read(CommandInterpreter &inInterpreter)
{
    std::string line;
    if (!inInterpreter.ReadLine(line))
        throw bciexception << "No input associated with command interpreter of type "
                           << ClassName(typeid(inInterpreter));
    inInterpreter.Out() << line;
    return true;
}

bool LineType::Write(CommandInterpreter &inInterpreter)
{
    std::string args = inInterpreter.GetRemainingTokens(), line;
    std::istringstream iss(args);
    ParserToken arg;
    if (iss >> arg)
        line = arg;
    while (iss >> arg)
    {
        line += " ";
        line += arg;
    }
    if (!inInterpreter.WriteLine(line))
        throw bciexception << "No output associated with command interpreter of type "
                           << ClassName(typeid(inInterpreter));
    return true;
}

} // namespace Interpreter