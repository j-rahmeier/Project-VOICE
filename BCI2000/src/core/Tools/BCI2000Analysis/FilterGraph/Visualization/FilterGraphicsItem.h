////////////////////////////////////////////////////////////////////////////////
// $Id: FilterGraphicsItem.h 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A QGraphicsItem representing a filter.
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
#ifndef FILTER_GRAPHICS_ITEM_H
#define FILTER_GRAPHICS_ITEM_H

#include "FilterGraph.h"
#include <QFont>
#include <QGraphicsItem>

class FilterGraphicsItem : public QGraphicsItem
{
  public:
    FilterGraphicsItem();
    FilterGraphicsItem(FilterGraph::Graph::iterator);

    FilterGraph::Graph::iterator filterPos() const;

    QColor color() const;
    void setColor(QColor);

    int frameWidth() const;
    void setFrameWidth(int);

    bool marked() const;
    void setMarked(bool);

    virtual QString text() const;
    virtual void setText(const QString &);

    QFont &font();
    const QFont &font() const;

    QRectF boundingRect() const override;

  protected:
    void init();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override;

  private:
    FilterGraph::Graph::iterator mFilterPos;
    QSizeF mExtent;
    QPointF mPos;
    QFont mFont;
    QColor mColor;
    int mFrameWidth;
    bool mMarked;
};

#endif // FILTER_GRAPHICS_ITEM_H
