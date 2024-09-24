////////////////////////////////////////////////////////////////////////////////
// $Id: TopoplotWindow.cpp 8268 2024-07-23 16:38:37Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Topoplot window for BCI2000 Analysis.
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
#include "TopoplotWindow.h"
#include "EditMenu.h"
#include "GenericSignal.h"
#include "HelpMenu.h"
#include "TopoplotWidget.h"
#include "WindowMenu.h"

#include <QBoxLayout>
#include <QGroupBox>
#include <QMenu>
#include <QMenuBar>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>

struct TopoplotWindow::Private : QObject
{
    Private(TopoplotWindow *parent) : QObject(parent)
    {
    }
};

TopoplotWindow::TopoplotWindow(const GenericSignal &inData, int inEl, QWidget *parent)
: p(new Private(this))
{
    setAttribute(Qt::WA_DeleteOnClose, true);

    QString title = parent->windowTitle() + " - Topoplot at ";
    std::string s = inData.Properties().ElementUnit().RawToPhysical(inEl);
    QString elName = QString::fromUtf8(s.c_str());
    title += elName;
    setWindowTitle(title);

    auto pMenu = QMainWindow::menuBar()->addMenu("&File");
    auto pAction = pMenu->addAction("&Close Window");
    pAction->setShortcut(Qt::CTRL | Qt::Key_W);
    connect(pAction, &QAction::triggered, this, &QMainWindow::close);

    EditMenu::addToMenuBar(menuBar());
    WindowMenu::addWindow(this);
    HelpMenu::addToMenuBar(menuBar());

    auto pCopyWidget = new EditMenu::CopyWidget(this);
    auto pLayout = new QHBoxLayout;
    pLayout->setContentsMargins(0, 0, 0, 0);
    pCopyWidget->setLayout(pLayout);
    auto pWidget = new TopoplotWidget(this);
    pWidget->setData(inData, inEl);
    pCopyWidget->layout()->addWidget(pWidget);
    setCentralWidget(pCopyWidget);

    resize(400, 320);
}

TopoplotWindow::~TopoplotWindow()
{
    WindowMenu::removeWindow(this);
}

int TopoplotWindow::askForElement(const SignalProperties* inProp)
{
    if (!inProp)
        return -1;
    QString text;
    if (inProp->ElementUnit().Symbol() == "s")
        text = "Enter time point to display distribution for, with unit: ";
    else if (inProp->ElementUnit().Symbol() == "Hz")
        text = "Enter frequency to display distribution for, with unit: ";
    else
        text = "Enter point on the x axis to display distribution for: ";
    bool ok = false;
    QString answer = QInputDialog::getText(nullptr, "Show Distribution at...", text, QLineEdit::Normal, QString(), &ok,
        Qt::MSWindowsFixedSizeDialogHint);
    if (!ok)
        return -1;
    std::string address = answer.toUtf8().constData();
    int el = inProp->ElementIndex(address);
    if (el < 0)
        QMessageBox::critical(nullptr, "Error", QString("\"%1\" is not a valid element specification.").arg(answer));
    return el;
}
