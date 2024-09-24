////////////////////////////////////////////////////////////////////////////////
// $Id: SubchainGraphicsItem.cpp 8082 2024-04-27 21:12:26Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A QGraphicsItem representing a subchain.
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
#include "SubchainGraphicsItem.h"
#include "FilterGraphicsItem.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

SubchainGraphicsItem::SubchainGraphicsItem(FilterGraph::Graph *pGraph) : mpGraph(pGraph)
{
    FilterGraphicsItem::init();
    font().setItalic(true);
}

QString SubchainGraphicsItem::text() const
{
    return mpGraph ? QString::fromUtf8(mpGraph->name().c_str()) : "";
}

void SubchainGraphicsItem::setText(const QString &text)
{
    if (!mpGraph)
        return;
    mpGraph->setName(text.toUtf8().constData());
}

QRectF SubchainGraphicsItem::boundingRect() const
{
    return FilterGraphicsItem::boundingRect();
}

void SubchainGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    QRectF rect = boundingRect();
    rect.adjust(1, 1, -1, -1);
    painter->setPen(QPen(option->palette.text(), 1));
    if (option->state & (QStyle::State_Selected | QStyle::State_HasFocus))
        painter->setBrush(option->palette.highlight());
    else
        painter->setBrush(option->palette.window());
    painter->drawRect(rect);
    painter->setFont(font());
    painter->drawText(rect, Qt::AlignCenter, text());
}
