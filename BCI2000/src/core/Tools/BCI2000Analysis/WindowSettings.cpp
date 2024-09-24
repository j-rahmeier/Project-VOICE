////////////////////////////////////////////////////////////////////////////////
// $Id: WindowSettings.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Save and retrieve window settings.
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
#include "WindowSettings.h"

#include <QMainWindow>
#include <QDockWidget>
#include <QToolBar>

namespace
{
    void setObjectNames(const QWidget* pWidget)
    {
        // saveState()/restoreState() requires object names
        auto dockWidgets = pWidget->findChildren<QDockWidget*>();
        for (auto pWidget : dockWidgets)
            pWidget->setObjectName(pWidget->windowTitle());
        auto toolBars = pWidget->findChildren<QToolBar*>();
        for (auto pToolBar : toolBars)
            pToolBar->setObjectName(pToolBar->windowTitle());
    }
}

namespace WindowSettings
{

bool read(QMainWindow* pWindow)
{
    Settings settings;
    settings.beginGroup(pWindow->windowTitle());
    QVariant geometry = settings.value("Geometry");
    if (!geometry.isValid())
        return false;
    pWindow->restoreGeometry(geometry.toByteArray());
    QVariant state = settings.value("State");
    if (!state.isValid())
        return false;
    setObjectNames(pWindow);
    pWindow->restoreState(state.toByteArray());
    return true;
}

void write(const QMainWindow* pWindow)
{
    Settings settings;
    settings.beginGroup(pWindow->windowTitle());
    settings.setValue("Geometry", pWindow->saveGeometry());
    setObjectNames(pWindow);
    settings.setValue("State", pWindow->saveState());
}

}

