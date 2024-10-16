////////////////////////////////////////////////////////////////////////////////
// $Id: ClassifierChannelPlotWindow.cpp 8268 2024-07-23 16:38:37Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Classifier channel plot window for BCI2000 Analysis.
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
#include "ClassifierChannelPlotWindow.h"
#include "EditMenu.h"
#include "GenericSignal.h"
#include "HelpMenu.h"
#include "SignalDisplay.h"
#include "SignalWidget.h"
#include "WindowMenu.h"

#include <QBoxLayout>
#include <QGroupBox>
#include <QMenu>
#include <QMenuBar>
#include <QInputDialog>
#include <QMessageBox>

ClassifierChannelPlotWindow::ClassifierChannelPlotWindow(
     const GenericSignal &inWeights, int inCh, QWidget *parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);

    QString title = parent->windowTitle() + " - Weights at channel ";
    QString channelName = QString::fromUtf8(inWeights.Properties().ChannelLabel(inCh).c_str());
    title += channelName;
    setWindowTitle(title);

    auto pMenu = QMainWindow::menuBar()->addMenu("&File");
    auto pAction = pMenu->addAction("&Close Window");
    pAction->setShortcut(Qt::CTRL | Qt::Key_W);
    connect(pAction, &QAction::triggered, this, &QMainWindow::close);

    EditMenu::addToMenuBar(menuBar());
    WindowMenu::addWindow(this);
    HelpMenu::addToMenuBar(menuBar());

    auto pWidget = new EditMenu::CopyWidget(this);
    auto pLayout = new QVBoxLayout;
    pLayout->setContentsMargins(2, 2, 2, 2);
    pWidget->setLayout(pLayout);
    SignalProperties prop = inWeights.Properties();
    prop.SetChannels(1);
    GenericSignal weights(prop);
    for (int el = 0; el < prop.Elements(); ++el)
        weights(0, el) = inWeights(inCh, el);
    auto pSignalWidget = new SignalWidget(weights, this);
    pSignalWidget->display().SetDisplayMode(SignalDisplay::polyline);
    pSignalWidget->display().SetChannelGroupSize(1);
    pSignalWidget->display().SetDisplayGroups(1);
    ColorList colors;
    colors.push_back(RGBColor::White);
    pSignalWidget->display().SetChannelColors(colors);
    pSignalWidget->display().SetValueUnitVisible(true);
    pWidget->layout()->addWidget(pSignalWidget);

    setCentralWidget(pWidget);

    resize(400, 160);
}

ClassifierChannelPlotWindow::~ClassifierChannelPlotWindow()
{
    WindowMenu::removeWindow(this);
}

int ClassifierChannelPlotWindow::askForChannel(const SignalProperties* inProp)
{
    if (!inProp)
        return -1;
    bool ok = false;
    QString answer =
        QInputDialog::getText(nullptr, "Show Weights at...", "Enter channel name or number:", QLineEdit::Normal,
            QString(), &ok, Qt::MSWindowsFixedSizeDialogHint);
    if (!ok)
        return -1;
    std::string address = answer.toUtf8().constData();
    int ch = inProp->ChannelIndex(address);
    if (ch < 0)
        QMessageBox::critical(nullptr, "Error", QString("\"%1\" is not a valid channel specification.").arg(answer));
    return ch;
}
