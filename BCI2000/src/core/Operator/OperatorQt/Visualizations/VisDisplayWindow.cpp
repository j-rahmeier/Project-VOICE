////////////////////////////////////////////////////////////////////////////////
// $Id: VisDisplayWindow.cpp 7849 2024-02-21 18:20:24Z mellinger $
// Authors: griffin.milsap@gmail.com, juergen.mellinger@uni-tuebingen.de
// Description: Root window class for visualization displays.
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
#include "VisDisplayWindow.h"
#include "VisDisplayContainer.h"
#include "Preferences.h"
#include "Debugging.h"

#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QScreen>

#include <cctype>
#include <map>

static QWidget *spParentWindow = nullptr;

typedef std::pair<VisID, VisDisplayBase *> Visual;

static size_t stripTrailingNumber(const std::string &s)
{
    size_t pos = s.length();
    while (pos > 0 && std::isdigit(s[pos - 1]))
        --pos;
    while (pos > 0 && std::isspace(s[pos - 1]))
        --pos;
    if (pos == 0 || pos == s.length())
        pos = std::string::npos;
    return pos;
}

static void addWindowAction(QMenu *pMenu, const Visual &visual)
{
    QWidget *pWindow = visual.second;
    QString title = pWindow->windowTitle();
    title += (" [" + visual.first.WindowID() + "]").c_str();
    auto action = pMenu->addAction(title);
    action->setCheckable(true);
    action->setChecked(pWindow->isVisible());
    action->connect(action, &QAction::triggered, pWindow, &QWidget::setVisible);
}

void VisDisplayWindow::buildWindowMenu(QMenu *pMenu)
{
    std::map<std::string, std::vector<Visual>> windows;
    for (const auto &v : Visuals())
    {
        if (dynamic_cast<VisDisplayWindow *>(v.second))
        {
            size_t pos = stripTrailingNumber(v.first);
            windows[v.first.Substr(0, pos)].push_back(v);
        }
    }

    pMenu->clear();
    for (const auto &w : windows)
    {
        if (w.second.size() == 1)
        {
            addWindowAction(pMenu, w.second.front());
        }
        else
        {
            std::string title = w.second.front().second->windowTitle().toLocal8Bit().constData();
            size_t pos = stripTrailingNumber(title);
            if (pos == std::string::npos)
                title.clear();
            else
                title = title.substr(0, pos) + ' ';
            title += "[" + w.first + "]";
            auto pSubmenu = pMenu->addMenu(QString::fromLocal8Bit(title.c_str()));
            for (const auto &visual : w.second)
                addWindowAction(pSubmenu, visual);
        }
    }
    if (pMenu->isEmpty())
        pMenu->addAction("(no visualizations)")->setEnabled(false);
}

void VisDisplayWindow::SetParentWindow(QWidget *p)
{
    spParentWindow = p;
}

void VisDisplayWindow::SetWindowMenu(QMenu *pMenu)
{
    pMenu->connect(pMenu, &QMenu::aboutToShow, [pMenu]() { buildWindowMenu(pMenu); });
}

VisDisplayWindow::VisDisplayWindow(const std::string &inVisID, bool createLayout)
: VisDisplayBase(inVisID), mUserIsMoving(true), mpLayout(nullptr)
{
    this->setParent(spParentWindow, Qt::Tool);
    this->setAttribute(Qt::WA_MacAlwaysShowToolWindow, true);

    if (createLayout)
    {
        mpLayout = new QStackedLayout(this);
        mpLayout->setStackingMode(QStackedLayout::StackAll);
    }

    // All VisDisplayWindows have a black background for backward compatibility
    // Note: This palette will propagate to child QWidgets.
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Window, QBrush(Qt::black));
    pal.setBrush(QPalette::WindowText, QBrush(Qt::white));
    this->setPalette(pal);
}

VisDisplayWindow::~VisDisplayWindow()
{
}

void VisDisplayWindow::OnSetConfig(ConfigSettings &inConfig)
{
    mUserIsMoving = false;
    std::string title = inConfig[CfgID::WindowTitle];
    if (!title.empty())
        this->setWindowTitle(Preferences::Instance()->mInstancePrefix + title.c_str());
    else
        this->setWindowTitle(Preferences::Instance()->mInstancePrefix + mVisID.c_str());

    // The static variables make each new window appear a little down right
    // from the previous one.
    static int newTop = 10, newLeft = 10;
    const int delta = 10, minSize = 10, defaultSize = 100;
    int formTop = newTop, formLeft = newLeft, formHeight = defaultSize, formWidth = defaultSize;
    bool posDefault = !inConfig.Get(CfgID::Top, formTop) || !inConfig.Get(CfgID::Left, formLeft),
         sizeDefault = !inConfig.Get(CfgID::Height, formHeight) || !inConfig.Get(CfgID::Width, formWidth);
    if (posDefault)
    {
        formTop = newTop;
        newTop += delta;
        formLeft = newLeft;
        newLeft += delta;
    }
    if (formWidth <= minSize)
    {
        sizeDefault = true;
        formWidth = minSize;
    }
    if (formHeight <= minSize)
    {
        sizeDefault = true;
        formHeight = minSize;
    }
    QRect formRect(formLeft, formTop, formWidth, formHeight);
    formRect.adjust(minSize, minSize, -minSize, -minSize);
    auto screens = QGuiApplication::screens();
    bool isVisible = false;
    for (auto screen : qAsConst(screens))
        isVisible |= formRect.intersects(screen->availableGeometry());
    if (!isVisible && !screens.empty())
    {
        posDefault = true;
        formTop = newTop;
        newTop += delta;
        formLeft = newLeft;
        newLeft += delta;
    }
    this->move(formLeft, formTop);
    this->resize(formWidth, formHeight);
    if (posDefault)
    {
        Visconfigs()[mVisID].Put(CfgID::Top, formTop, Default);
        Visconfigs()[mVisID].Put(CfgID::Left, formLeft, Default);
    }
    if (sizeDefault)
    {
        Visconfigs()[mVisID].Put(CfgID::Width, formWidth, Default);
        Visconfigs()[mVisID].Put(CfgID::Height, formHeight, Default);
    }
    bool windowFrame;
    if (inConfig.Get(CfgID::WindowFrame, windowFrame))
    {
        Qt::WindowFlags flags = this->windowFlags() & ~Qt::FramelessWindowHint;
        if (!windowFrame)
            flags |= Qt::FramelessWindowHint;
        this->setWindowFlags(flags);
    }
    std::string containerVisID;
    if (inConfig.Get(CfgID::PlacementVis, containerVisID))
    {
        int row = 1, col = 1, height = 1, width = 1;
        inConfig.Get(CfgID::PlacementRow, row);
        inConfig.Get(CfgID::PlacementCol, col);
        inConfig.Get(CfgID::PlacementHeight, height);
        inConfig.Get(CfgID::PlacementWidth, width);
        auto pContainer = dynamic_cast<VisDisplayContainer*>(Visuals()[containerVisID]);
        if (!pContainer)
        {
            VisDisplayBase::Create<VisDisplayContainer>(containerVisID);
            pContainer = dynamic_cast<VisDisplayContainer*>(Visuals()[containerVisID]);
        }
        Assert(pContainer);
        auto p = dynamic_cast<VisDisplayContainer*>(this);
        if (p && p->ContainsSubwindow(pContainer))
            throw std_runtime_error << "Detected recursive placement";
        pContainer->PlaceSubwindow(this, row - 1, col - 1, height, width);
    }
    VisDisplayBase::OnSetConfig(inConfig);
    mUserIsMoving = true;
}

void VisDisplayWindow::moveEvent(QMoveEvent *iopEvent)
{
    if (mUserIsMoving)
    {
        Visconfigs()[mVisID].Put(CfgID::Top, this->pos().y(), UserDefined);
        Visconfigs()[mVisID].Put(CfgID::Left, this->pos().x(), UserDefined);
    }
    QWidget::moveEvent(iopEvent);
}

void VisDisplayWindow::resizeEvent(QResizeEvent *iopEvent)
{
    this->update();
    if (mUserIsMoving)
    {
        Visconfigs()[mVisID].Put(CfgID::Width, this->size().width(), UserDefined);
        Visconfigs()[mVisID].Put(CfgID::Height, this->size().height(), UserDefined);
    }
    QWidget::resizeEvent(iopEvent);
}
