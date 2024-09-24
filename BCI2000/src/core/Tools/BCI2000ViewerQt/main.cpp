////////////////////////////////////////////////////////////////////////////////
// $Id: main.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: The main() function of the BCI2000Viewer tool.
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
#include "BCI2000Viewer.h"
#include "ProcessUtils.h"
#include "ExceptionCatcher.h"
#include "QtMain.h"

static BCI2000Viewer* spViewer;

static void CreateViewer()
{
    spViewer = new BCI2000Viewer;
    spViewer->show();
}

int main(int argc, char **argv)
{
    ProcessUtils::WindowsEnableStdio(argc, argv);
    ProcessUtils::InitializeDesktopIntegration("org.bci2000.Tools");
    ProcessUtils::InitializeShellIntegration("../../prog/BCI2000ShellExtension");
    QtApplication app(argc, argv);
    FunctionCall<void ()> call(&CreateViewer);
    int result = -1;
    if (ExceptionCatcher().Run(call))
        result = app.Run();
    delete spViewer;
    return result;
}
