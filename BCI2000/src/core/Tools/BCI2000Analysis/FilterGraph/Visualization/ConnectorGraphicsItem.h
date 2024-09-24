////////////////////////////////////////////////////////////////////////////////
// $Id: ConnectorGraphicsItem.h 7463 2023-06-30 14:59:17Z mellinger $
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
#ifndef CONNECTOR_GRAPHICS_ITEM_H
#define CONNECTOR_GRAPHICS_ITEM_H

#include "FilterGraph.h"
#include <QGraphicsItem>

class FilterGraphicsItem;

class ConnectorGraphicsItem : public QGraphicsItem
{
  public:
    ConnectorGraphicsItem(FilterGraphicsItem *from, FilterGraphicsItem *to,
                          FilterGraph::Graph::iterator toPos = {nullptr, 0});

    FilterGraph::Graph::iterator fromPos() const;
    FilterGraph::Graph::iterator toPos() const;

    QRectF boundingRect() const override;

  protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override;

  private:
    QPointF fromPoint() const;
    QPointF toPoint() const;

    FilterGraphicsItem *mpFrom, *mpTo;
    FilterGraph::Graph::iterator mFromPos, mToPos;
};

#endif // CONNECTOR_GRAPHICS_ITEM_H
