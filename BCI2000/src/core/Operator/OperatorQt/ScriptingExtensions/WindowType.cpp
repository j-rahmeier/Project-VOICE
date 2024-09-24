//////////////////////////////////////////////////////////////////////
// $Id$
// Authors: mellinger@neurotechcenter.de
// Description: Scripting extension dealing with Operator windows.
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
#include "WindowType.h"

#include "MainWindow.h"
#include "SysLog.h"
#include "NotesWindow.h"
#include "ConfigWindow.h"
#include "WatchesWindow.h"

#include "ParserToken.h"
#include <sstream>
#include <vector>
#include <string>

#include <QWidget>
#include <QScreen>

namespace {
    ScriptingExtensions::WindowType sInstance;
    const char* sHelp = "Show Window <name>, Hide Window <name> "
                        "(names are: Main, Syslog, Notes, Configuration, Watches), "
                        "Move Window <name> <x> <y>, Resize Window <name> <w> <h>, "
                        "Arrange Window <name> <rows> <cols> <row> <col> [<rowspan> <colspan>]";
}

namespace ScriptingExtensions {
    const ObjectType::MethodEntry WindowType::sMethodTable[] = {
      {"Show", &WindowType::Show },
      {"Hide", &WindowType::Hide },
      {"Move", &WindowType::Move },
      {"Resize", &WindowType::Resize },
      {"Arrange", &WindowType::Arrange },
      { 0 }
    };

    const char* WindowType::Name() const
    {
        return "Window";
    }

    const char* WindowType::Help() const
    {
        return sHelp;
    }

    const ObjectType::MethodEntry* WindowType::MethodTable() const
    {
        return sMethodTable;
    }

    bool WindowType::Show(std::istream& is, std::ostream& os)
    {
        ParserToken name;
        is >> name;
        return SetWindowVisible(name.c_str(), true, os);
    }

    bool WindowType::Hide(std::istream& is, std::ostream& os)
    {
        ParserToken name;
        is >> name;
        return SetWindowVisible(name.c_str(), false, os);
    }

    QRect WindowType::GetGridRect(std::istream& is, std::ostream& os)
    {
        ParserToken rows, cols, row, col, rowspan, colspan;
        if (!(is >> rows >> cols >> row >> col)) {
            os << "Expected rows, cols, row, and col";
            return QRect(0, 0, -1, -1);
        }
        is >> rowspan >> colspan;
        if (rowspan.empty())
            rowspan = "1";
        if (colspan.empty())
            colspan = "1";
        int irows = ::atoi(rows.c_str()), icols = ::atoi(cols.c_str()),
            irow = ::atoi(row.c_str()), icol = ::atoi(col.c_str()),
            irowspan = ::atoi(rowspan.c_str()), icolspan = ::atoi(colspan.c_str());
        if (irows < 1 || icols < 1 || irow < 1 || icol < 1) {
            os << "rows, cols, row and col must be > 0";
            return QRect(0, 0, -1, -1);
        }
        if (irowspan < 0)
            irowspan = 0;
        if (icolspan < 0)
            icolspan = 0;

        QRect screenRect = MainWindow::Instance()->screen()->availableGeometry();
        float rowHeight = screenRect.height() * 1.0 / irows,
              colWidth = screenRect.width() * 1.0 / icols;
        QRect rect;
        rect.setLeft(screenRect.left() + colWidth * (icol - 1));
        rect.setTop(screenRect.top() + rowHeight * (irow - 1));
        rect.setWidth(colWidth * icolspan);
        rect.setHeight(rowHeight * irowspan);
        return rect;
    }

    bool WindowType::Arrange(std::istream& is, std::ostream& os)
    {
        ParserToken name;
        if (!(is >> name))
        {
            os << "Expected window name";
            return false;
        }
        QRect rect = GetGridRect(is, os);
        if (rect.width() < 0)
            return false;

        QWidget* pWindow = GetWindow(name.c_str(), os);
        if (pWindow) {
            QSize size = AdjustSize(QSize(rect.width(), rect.height()), pWindow);
            QMetaObject::invokeMethod(pWindow, "Move", 
                Qt::AutoConnection, Q_ARG(int, rect.left()), Q_ARG(int, rect.top()));
            QMetaObject::invokeMethod(pWindow, "Resize", 
                Qt::AutoConnection, Q_ARG(int, size.width()), Q_ARG(int, size.height()));
            return true;
        }
        return false;
    }

    bool WindowType::Move(std::istream& is, std::ostream& os)
    {
        ParserToken name, x, y;
        if (!(is >> name >> x >> y)) {
            os << "Expected name, x position, and y position";
            return false;
        }
        QWidget* pWindow = GetWindow(name.c_str(), os);
        if (pWindow) {
            int ix = ::atoi(x.c_str()), iy = ::atoi(y.c_str());
            QMetaObject::invokeMethod(pWindow, "Move", Qt::AutoConnection, Q_ARG(int, ix), Q_ARG(int, iy));
            return true;
        }
        return false;
    }

    bool WindowType::Resize(std::istream& is, std::ostream& os)
    {
        ParserToken name, w, h;
        if (!(is >> name >> w >> h)) {
            os << "Expected name, width, and height";
            return false;
        }
        QWidget* pWindow = GetWindow(name.c_str(), os);
        if (pWindow) {
            QSize size = AdjustSize(QSize(::atoi(w.c_str()), ::atoi(h.c_str())), pWindow);
            QMetaObject::invokeMethod(pWindow, "Resize", 
                Qt::AutoConnection, Q_ARG(int, size.width()), Q_ARG(int, size.height()));
            return true;
        }
        return false;
    }

    QWidget* WindowType::GetWindow(const char* name, std::ostream& os)
    {
        if (!*name || !::stricmp("Main", name))
            return MainWindow::Instance();
        if (!::stricmp("SysLog", name) || !::stricmp("Log", name))
            return SysLog::Instance();
        if (!::stricmp("Notes", name))
            return NotesWindow::Instance();
        if (!::stricmp("Watches", name))
            return WatchesWindow::Instance();
        if (!::stricmp("Config", name) || !::stricmp("Configuration", name)) 
            return ConfigWindow::Instance();
        os << "Don't know window named \"" << name << "\"";
        return nullptr;
    }

    bool WindowType::SetWindowVisible(const char* name, bool visible, std::ostream& os)
    {
        QWidget* pWindow = GetWindow(name, os);
        if (pWindow && pWindow == ConfigWindow::Instance()) {
            switch (bci::BCI_GetStateOfOperation())
            {
                case bci::BCI_StateConnected:
                case bci::BCI_StateResting:
                case bci::BCI_StateSuspended:
                case bci::BCI_StateParamsModified:
                    break;
                default:
                    os << "Cannot open Configuration window in current system state";
                    return false;
            }
            if (visible)
                QMetaObject::invokeMethod(MainWindow::Instance(), "on_pushButton_Config_clicked", 
                    Qt::AutoConnection);
            else
                QMetaObject::invokeMethod(pWindow, "close", Qt::AutoConnection);
            return true;
        }

        if (pWindow) {
            QMetaObject::invokeMethod(pWindow, "setVisible", Qt::AutoConnection, Q_ARG(bool, visible));
            return true;
        }
        return false;
    }

    QSize WindowType::AdjustSize(const QSize& frameSize, QWidget* pWidget)
    {
        if (!pWidget)
            pWidget = MainWindow::Instance();
        assert(pWidget);
        QRect frameGeometry = pWidget->frameGeometry(),
              geometry = pWidget->geometry();
        int reducedWidth = frameSize.width() - frameGeometry.width() + geometry.width(),
            reducedHeight = frameSize.height() - frameGeometry.height() + geometry.height();
        return QSize(reducedWidth, reducedHeight);
    }
}

