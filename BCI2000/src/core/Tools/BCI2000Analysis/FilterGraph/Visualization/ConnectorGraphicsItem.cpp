////////////////////////////////////////////////////////////////////////////////
// $Id: ConnectorGraphicsItem.cpp 8082 2024-04-27 21:12:26Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A QGraphicsItem representing a connector between filters.
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
#include "ConnectorGraphicsItem.h"
#include "FilterGraphicsItem.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

ConnectorGraphicsItem::ConnectorGraphicsItem(FilterGraphicsItem *from, FilterGraphicsItem *to,
                                             FilterGraph::Graph::iterator toPos)
    : mpFrom(from), mpTo(to), mFromPos(from->filterPos()), mToPos(to->filterPos())
{
    if (toPos)
        mToPos = toPos;
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsSelectable, true);
}

FilterGraph::Graph::iterator ConnectorGraphicsItem::fromPos() const
{
    return mFromPos;
}

FilterGraph::Graph::iterator ConnectorGraphicsItem::toPos() const
{
    return mToPos;
}

QRectF ConnectorGraphicsItem::boundingRect() const
{
    QPointF center = (fromPoint() + toPoint()) / 2;
    QRectF r(center, center);
    r.adjust(-4, -4, 4, 4);
    QRectF r2(fromPoint(), toPoint());
    r2 = r2.normalized();
    return r.united(r2);
}

QPointF ConnectorGraphicsItem::fromPoint() const
{
    QRectF r = mpFrom->mapRectToScene(mpFrom->boundingRect());
    return QPointF(r.center().x(), r.bottom());
}

QPointF ConnectorGraphicsItem::toPoint() const
{
    QRectF r = mpTo->mapRectToScene(mpTo->boundingRect());
    return QPointF(r.center().x(), r.top());
}

void ConnectorGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(QPen(option->palette.text(), 1));
    if (option->state & (QStyle::State_Selected | QStyle::State_HasFocus))
        painter->setBrush(option->palette.highlight());
    else
        painter->setBrush(option->palette.window());
    painter->drawLine(fromPoint(), toPoint());
    QPointF center = (fromPoint() + toPoint()) / 2;
    QRectF r(center, center);
    r.adjust(-4, -4, 4, 4);
    if (option->state & (QStyle::State_Selected | QStyle::State_HasFocus))
        painter->setPen(QPen(option->palette.highlight(), 1));
    painter->drawEllipse(r);
}
