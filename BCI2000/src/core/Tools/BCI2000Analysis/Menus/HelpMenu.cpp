////////////////////////////////////////////////////////////////////////////////
// $Id: HelpMenu.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A menu containing executable help and about box items.
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
#include "HelpMenu.h"
#include "AboutBox.h"
#include "ExecutableHelp.h"
#include <QApplication>
#include <QMenu>
#include <QMenuBar>

namespace
{
struct Handler : QObject
{
    void onExecutableHelp()
    {
        ExecutableHelp().Display();
    }
    void onHelpAbout()
    {
        AboutBox().SetApplicationName(qApp->applicationName().toLocal8Bit().constData()).Display();
    }
} sHandler;
} // namespace

QMenu *HelpMenu::addToMenuBar(QMenuBar *pBar)
{
    auto pMenu = pBar->addMenu("&Help");
    auto pAction = pMenu->addAction(qApp->applicationName() + " &Help");
    pAction->setShortcut(Qt::Key_F1);
    pAction->connect(pAction, &QAction::triggered, &sHandler, &Handler::onExecutableHelp);
    pAction = pMenu->addAction("&About " + qApp->applicationName() + "...");
    pAction->connect(pAction, &QAction::triggered, &sHandler, &Handler::onHelpAbout);
    return pMenu;
}
