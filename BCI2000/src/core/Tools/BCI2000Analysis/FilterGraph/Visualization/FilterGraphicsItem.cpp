////////////////////////////////////////////////////////////////////////////////
// $Id: FilterGraphicsItem.cpp 8082 2024-04-27 21:12:26Z mellinger $
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
#include "FilterGraphicsItem.h"
#include "FilterGraphNode.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

FilterGraphicsItem::FilterGraphicsItem(FilterGraph::Graph::iterator i)
    : mFilterPos(i)
    , mExtent(0, 0), mPos(0, 0)
    , mFrameWidth(1)
    , mColor(Qt::white), mFont()
    , mMarked(false)
{
    init();
}

FilterGraphicsItem::FilterGraphicsItem()
    : mFilterPos(FilterGraph::Graph::iterator())
    , mExtent(0, 0), mPos(0, 0)
    , mFrameWidth(1)
    , mColor(Qt::white), mFont()
    , mMarked(false)
{
    // init() must be called by descendant constructor
}

void FilterGraphicsItem::init()
{
    QString text_ = text();
    bool isEmpty = text_.isEmpty();
    QFontMetricsF fm(mFont);
    if (isEmpty)
        text_ = "<Empty Node>";
    mExtent = fm.size(0, text_);
    mExtent *= ::sqrt(2);
    mPos.setX(-mExtent.width() / 2);
    mPos.setY(-mExtent.height());
    if (isEmpty)
    {
        mExtent.setHeight(0);
        setVisible(false);
    }
    else
    {
#if 0
        QGraphicsItem::setFlag(QGraphicsItem::ItemIsSelectable, true);
#endif
    }
}

QColor FilterGraphicsItem::color() const
{
    return mColor;
}

void FilterGraphicsItem::setColor(QColor c)
{
    mColor = c;
}

int FilterGraphicsItem::frameWidth() const
{
    return mFrameWidth;
}

void FilterGraphicsItem::setFrameWidth(int i)
{
    if (i != mFrameWidth)
    {
        prepareGeometryChange();
        mFrameWidth = i;
    }
}

bool FilterGraphicsItem::marked() const
{
    return mMarked;
}

void FilterGraphicsItem::setMarked(bool marked)
{
    if (marked != mMarked)
    {
        prepareGeometryChange();
        mMarked = marked;
    }
}

QString FilterGraphicsItem::text() const
{
    bool isEmpty = !mFilterPos || (mFilterPos == mFilterPos.graph->end());
    QString text = "";
    if (!isEmpty)
    {
        const FilterGraph::Node *pData = *mFilterPos;
        text = QString::fromUtf8(pData->name().c_str());
    }
    return text;
}

void FilterGraphicsItem::setText(const QString &text)
{
    if (!mFilterPos)
        return;
    if (mFilterPos == mFilterPos.graph->end())
        return;
    FilterGraph::Node *pData = *mFilterPos;
    pData->setName(text.toUtf8().constData());
}

QFont &FilterGraphicsItem::font()
{
    return mFont;
}

const QFont &FilterGraphicsItem::font() const
{
    return mFont;
}

FilterGraph::Graph::iterator FilterGraphicsItem::filterPos() const
{
    return mFilterPos;
}

QRectF FilterGraphicsItem::boundingRect() const
{
    QRectF rect(mPos.x(), mPos.y(), mExtent.width(), mExtent.height());
    int d = mFrameWidth - 1;
    if (mMarked)
        d += 2 * mFrameWidth;
    rect.adjust(-d, -d, d, d);
    return rect;
}

void FilterGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    QRectF rect(mPos.x(), mPos.y(), mExtent.width(), mExtent.height());
    int d = mFrameWidth - 1;
    rect.adjust(-d, -d, d, d);
    painter->setPen(QPen(option->palette.windowText(), mFrameWidth));
    QColor color = mColor;
    if (option->state & (QStyle::State_Selected | QStyle::State_HasFocus))
        color = color.darker();
    painter->setBrush(color);
    if (mMarked)
        painter->setPen(QPen(option->palette.highlight(), 4 * mFrameWidth));
    painter->drawRoundedRect(rect, 5, 5);
    painter->setFont(mFont);
    painter->setPen(QPen(Qt::black, 1));
    painter->drawText(rect, Qt::AlignCenter, text());
}
