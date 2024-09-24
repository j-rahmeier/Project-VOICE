////////////////////////////////////////////////////////////////////////////////
// $Id: main.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: The main() function of the BCI2000Analysis tool.
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
#include "AnalysisWindow.h"
#include "ProcessUtils.h"
#include "QtMain.h"
#include <QApplication>
#include <QFileInfo>

int main(int argc, char **argv)
{
    ProcessUtils::InitializeDesktopIntegration("org.bci2000.Tools");
    ProcessUtils::InitializeShellIntegration("../../prog/BCI2000ShellExtension");
    QtApplication app(argc, argv);
    QStringList args = qApp->arguments();
    if (args.size() > 1)
    {
        QStringList datafiles, analysisfiles;
        for (int i = 1; i < args.size(); ++i)
        {
            QString suffix = QFileInfo(args[i]).suffix().toLower();
            if (suffix == "dat" || suffix == "bcidat")
                datafiles.append(args[i]);
            else if (suffix == "bcianalysis")
                analysisfiles.append(args[i]);
        }
        if (!datafiles.isEmpty())
        {
            auto pWindow = new AnalysisWindow();
            pWindow->openDataFiles(datafiles);
            pWindow->show();
        }
        for (int i = 0; i < analysisfiles.size(); ++i)
        {
            auto pWindow = new AnalysisWindow();
            pWindow->openAnalysisFile(analysisfiles[i]);
            pWindow->show();
        }
    }
    else
    {
        auto pWindow = new AnalysisWindow;
        pWindow->show();
    }
    return app.Run();
}
