//////////////////////////////////////////////////////////////////////
// $Id: main.cpp 8185 2024-06-11 19:53:41Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: The Operator module's main() function.
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
#include "Debugging.h"
#include "MainWindow.h"
#include "ProcessUtils.h"
#include "QtMain.h"
#include "ExceptionCatcher.h"
#include "BCIStream.h"

int main2(int argc, char **argv)
{
    MemoryDebugging::Init init;
    MemoryDebugging::SetPrompt(false);
    if (!ProcessUtils::AssertSingleInstance(argc, argv)) {
        bcierr__ << "Another instance of the Operator module is running.\n"
                 << "Terminate that instance first, or use the --AllowMultipleInstances command line switch.";
        return 0;
    }
    ProcessUtils::InitializeDesktopIntegration("org.bci2000.OnlineModules");
    ProcessUtils::InitializeShellIntegration("BCI2000ShellExtension");

    QtApplication app(argc, argv);
    QWidget *pWindow = new MainWindow;
    pWindow->setAttribute(Qt::WA_DeleteOnClose);
    return app.Run();
}

int main(int argc, char** argv)
{
    FunctionCall<int(int, char**)> callMain2(&main2, argc, argv);
    struct : ExceptionCatcher
    {
        void OnReportException(const std::string& msg) override
        {
            bcierr__ << "Uncaught exception: " << msg;
        }
    } catchExceptions;
    if (!catchExceptions.Run(callMain2)) {
        return -1;
    }
    return callMain2.Result();
}
