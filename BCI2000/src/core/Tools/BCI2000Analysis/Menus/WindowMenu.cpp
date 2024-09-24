////////////////////////////////////////////////////////////////////////////////
// $Id: WindowMenu.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A menu holding window titles, and activating a window when
//   clicked.
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
#include "WindowMenu.h"

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QFileInfo>

#include <map>

namespace
{
std::map<QMainWindow *, QMenu *> sMenus;
struct MenuHandler : QObject
{
    void onMenuTriggered();
    void onAboutToShow();
} sMenuHandler;

void MenuHandler::onMenuTriggered()
{
    auto pAction = dynamic_cast<QAction *>(sender());
    if (pAction)
    {
        auto pWindow = pAction->data().value<QMainWindow *>();
        if (pWindow)
        {
            pWindow->show();
            pWindow->activateWindow();
        }
    }
}

void MenuHandler::onAboutToShow()
{
    auto pMenu = dynamic_cast<QMenu *>(sender());
    if (pMenu)
    {
        QList<QAction*> actions = pMenu->actions();
        for (auto pAction : actions)
        {
            if (pAction->data().isValid())
            {
                auto pWindow = pAction->data().value<QWidget*>();
                if (pWindow)
                {
                    QString title = pWindow->windowTitle();
                    pAction->setText(title);
                    pAction->setCheckable(true);
                    if (pWindow == QApplication::activeWindow())
                        pAction->setChecked(true);
                    else
                        pAction->setChecked(false);
                }
            }
        }
    }
}

} // namespace

void WindowMenu::addWindow(QMainWindow *pWindow, const QString &title)
{
    QString menuEntry = title;
    if (menuEntry.isEmpty())
        menuEntry = pWindow->windowTitle();
    QMenu *pMenu = pWindow->menuBar()->addMenu("&Window");
    if (!sMenus.empty())
    {
        auto actions = sMenus.begin()->second->actions();
        for (auto pAction : actions)
        {
            auto pAction2 = pMenu->addAction("<N/A>");
            pAction2->setData(pAction->data());
            pAction2->connect(pAction2, &QAction::triggered, &sMenuHandler, &MenuHandler::onMenuTriggered);
        }
    }
    sMenus[pWindow] = pMenu;
    pWindow->connect(pMenu, &QMenu::aboutToShow, &sMenuHandler, &MenuHandler::onAboutToShow);
    for (auto &s : sMenus)
    {
        auto pAction = s.second->addAction(menuEntry);
        pAction->setData(QVariant::fromValue(pWindow));
        pWindow->connect(pAction, &QAction::triggered, &sMenuHandler, &MenuHandler::onMenuTriggered);
    }
}

void WindowMenu::updateWindow(QMainWindow* pWindow, const QString& title)
{
    if (sMenus.find(pWindow) == sMenus.end())
        addWindow(pWindow, title);
    QString menuEntry = title;
    if (menuEntry.isEmpty())
        menuEntry = pWindow->windowTitle();
    // update all existing menu entries in all existing window menus
    for (const auto& s : sMenus)
    {
        auto actions = s.second->actions();
        for (auto pAction : actions)
            if (pAction->data().value<QMainWindow*>() == pWindow)
                pAction->setText(menuEntry);
    }
}

void WindowMenu::removeWindow(QMainWindow *pWindow)
{
    sMenus.erase(pWindow);
    for (auto &s : sMenus)
    {
        for (auto pAction : s.second->actions())
        {
            if (pAction->data().isValid() && pAction->data().value<QWidget *>() == pWindow)
            {
                s.second->removeAction(pAction);
                break;
            }
        }
    }
}

namespace {
    QWidget* findWindowWithTitle(const QString& title)
    {
        auto widgets = QApplication::topLevelWidgets();
        for (auto p : widgets)
        {
            if (p->windowTitle() == title)
                return p;
        }
        return nullptr;
    }
}

QString WindowMenu::uniqueTitle(const QString& title)
{
    QString result = title;
    int i = 0;
    while (findWindowWithTitle(result))
        result = QString("%1 %2").arg(title).arg(++i);
    return result;
}

QString WindowMenu::windowTitleFromFile(const QString& name, const QString& file)
{
    QString title;
    if (file.isEmpty())
        title = name;
    else
        title = name + " - " + QFileInfo(file).fileName();
    return uniqueTitle(title);
}
