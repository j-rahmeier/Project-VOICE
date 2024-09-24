////////////////////////////////////////////////////////////////////////////////
// $Id: VisDisplayContainer.cpp 7849 2024-02-21 18:20:24Z mellinger $
// Authors: juergen.mellinger@neurotechcenter.org
// Description: Container window class for visualization displays.
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
#include "VisDisplayContainer.h"
#include "Color.h"
#include "Debugging.h"

#include <QLabel>
#include <QPaintEvent>
#include <QPainter>

VisDisplayContainer::VisDisplayContainer(const std::string& visID)
: VisDisplayWindow(visID, false),
  mpLayout(new QGridLayout),
  mBackground(Qt::lightGray)
{
    setLayout(mpLayout);
}

VisDisplayContainer::~VisDisplayContainer()
{
}

void VisDisplayContainer::PlaceSubwindow(QWidget* pWidget, int row, int col, int rowSpan, int colSpan)
{
    QLayoutItem* pItem = mpLayout->itemAtPosition(row, col);
    QVBoxLayout* pSublayout = pItem ? dynamic_cast<QVBoxLayout*>(pItem->layout()) : nullptr;
    if (pSublayout)
    {
        int idx = mpLayout->indexOf(pItem);
        Assert(idx >= 0);
        int existingRow, existingCol, existingRowSpan, existingColSpan;
        mpLayout->getItemPosition(idx, &existingRow, &existingCol, &existingRowSpan, &existingColSpan);
        Assert(existingRow == row);
        Assert(existingCol == col);
        if (existingRowSpan != rowSpan || existingColSpan != colSpan)
        {
            mpLayout->removeItem(pItem);
            mpLayout->addLayout(pSublayout, row, col, rowSpan, colSpan);
        }
    }
    else
    {
        pSublayout = new QVBoxLayout;
        pSublayout->insertWidget(0, new QLabel);
        mpLayout->addLayout(pSublayout, row, col, rowSpan, colSpan);
    }

    pItem = pSublayout->itemAt(0);
    QLabel* pLabel = pItem ? dynamic_cast<QLabel*>(pItem->widget()) : nullptr;
    Assert(pLabel);
    pLabel->setText(pWidget->windowTitle());
    QPalette palette;
    palette.setBrush(QPalette::ColorRole::WindowText, Qt::black);
    pLabel->setPalette(palette);

    bool isFramelessWindow = pWidget->windowFlags() & Qt::FramelessWindowHint;
    pLabel->setVisible(!isFramelessWindow);

    pItem = pSublayout->itemAt(1);
    QWidget* pExistingWidget = pItem ? dynamic_cast<QWidget*>(pItem->widget()) : nullptr;
    if (pExistingWidget && pExistingWidget != pWidget)
        pSublayout->removeWidget(pExistingWidget);
    if (pExistingWidget != pWidget)
        pSublayout->insertWidget(1, pWidget);
}

void VisDisplayContainer::SetMargin(int margin)
{
    mpLayout->setSpacing(margin);
    mpLayout->setContentsMargins(margin, margin, margin, margin);
}

void VisDisplayContainer::SetBackgroundColor(QColor color)
{
    mBackground = color;
}

bool VisDisplayContainer::ContainsSubwindow(QWidget* pWidget) const
{
    auto children = findChildren<QWidget*>();
    for (auto pChild : children)
        if (pChild == pWidget)
            return true;
    return false;
}

void VisDisplayContainer::OnSetConfig(ConfigSettings& inConfig)
{
    VisDisplayWindow::OnSetConfig(inConfig);
    int margin = 0;
    if (inConfig.Get(CfgID::PlacementMargin, margin))
        SetMargin(margin);
    RGBColor c;
    if (inConfig.Get(CfgID::PlacementColor, c))
        SetBackgroundColor(c.ToQRgb());
}

void VisDisplayContainer::paintEvent(QPaintEvent* ev)
{
    QPainter painter(this);
    painter.fillRect(ev->rect(), mBackground);
    ev->accept();
}

