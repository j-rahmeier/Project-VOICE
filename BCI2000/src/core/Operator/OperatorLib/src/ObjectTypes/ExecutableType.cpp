////////////////////////////////////////////////////////////////////////////////
// $Id$
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A script interpreter type that handles commands acting on
//   executables.
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
#include "ExecutableType.h"

#include "ProcessType.h"
#include "BCIException.h"
#include "CommandInterpreter.h"

#include "ProcessUtils.h"
#include "FileUtils.h"
#include "ModuleInventory.h"

namespace Interpreter
{

ExecutableType ExecutableType::sInstance;
const ObjectType::MethodEntry ExecutableType::sMethodTable[] = {
    METHOD(Start),
    METHOD(Categorize),
    END
};

bool ExecutableType::Start(CommandInterpreter &inInterpreter)
{
    std::string executable = inInterpreter.GetToken(),
                arguments = ProcessType::ToShellArguments(inInterpreter.GetOptionalRemainder());
    int exitCode = 0;
    bool ok = ProcessUtils::ExecuteAsynchronously(executable, arguments, exitCode);
    SysError err;
    if (!ok && err.Code())
        throw bciexception << "Could not run \"" << executable << "\": " << err.Message();
    if (exitCode != 0)
        inInterpreter.Out() << inInterpreter.ExitCodeTag() << exitCode;
    inInterpreter.Log() << "Started \"" << executable << "\"" << (ok ? "" : ", terminated immediately");
    return true;
}

static bci::Inventory sInventory;

bool ExecutableType::Categorize(CommandInterpreter& inInterpreter)
{
    std::string executable = inInterpreter.GetToken();
    StringUtils::String extension = FileUtils::ExtractExtension(executable);
    if (extension.Empty() || extension.ToLower() == ".exe")
        executable = FileUtils::ExtractBase(executable);
    int category = sInventory.Categorize(executable);
    inInterpreter.Out() << bci::Inventory::CategoryToString(category);
    return true;
}

} // namespace Interpreter
