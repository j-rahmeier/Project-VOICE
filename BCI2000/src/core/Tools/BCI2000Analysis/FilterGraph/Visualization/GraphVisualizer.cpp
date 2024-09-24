////////////////////////////////////////////////////////////////////////////////
// $Id: GraphVisualizer.cpp 8215 2024-06-21 13:36:37Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A class that visualizes a FilterGraph using Qt.
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
#include "GraphVisualizer.h"

#include "ConnectorGraphicsItem.h"
#include "DllFilterNode.h"
#include "PlaceholderNode.h"
#include "FilterGraphNode.h"
#include "FilterGraphicsItem.h"
#include "LinearClassifierNode.h"
#include "CSPNode.h"
#include "SplitterNode.h"
#include "StaticNodes.h"
#include "ApplicationNodes.h"
#include "SubchainGraphicsItem.h"
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>

namespace FilterGraph
{

namespace
{
int sSpacing = 20;

QRectF boundingRect(const std::vector<FilterGraphicsItem *> &items)
{
    if (items.empty())
        return QRectF();
    QRectF b = items[0]->mapToScene(items[0]->boundingRect()).boundingRect();
    for (auto pItem : items)
    {
        QRectF r = pItem->mapToScene(pItem->boundingRect()).boundingRect();
        if (r.left() < b.left())
            b.setLeft(r.left());
        if (r.right() > b.right())
            b.setRight(r.right());
        if (r.top() < b.top())
            b.setTop(r.top());
        if (r.bottom() > b.bottom())
            b.setBottom(r.bottom());
    }
    return b;
}
} // namespace

typedef std::pair<std::vector<FilterGraphicsItem *>, std::vector<ConnectorGraphicsItem *>> Vis;

struct Visualizer::Private
{
    QGraphicsScene *mpScene;

    static Vis visualizeGraph(Graph *, Graph::iterator);

    static Vis visualizeNode(Graph::iterator);
    // Visualization depends on node type and is dispatched
    // using dynamic_cast<>(). Using the Visitor pattern would
    // have complicated things even more, so we stick to this solution.
    static Vis visualizeNode(Node *, Graph::iterator);
    static Vis visualizeSourceNode(SourceNode *, Graph::iterator);
    static Vis visualizeCSPNode(CSPNode*, Graph::iterator);
    static Vis visualizeLinearClassifierNode(LinearClassifierNode *, Graph::iterator);
    static Vis visualizeApplicationNode(BasicApplicationNode *, Graph::iterator);
    static Vis visualizeSplitterNode(SplitterNode *, Graph::iterator);
    static Vis visualizePlaceholderNode(PlaceholderNode*, Graph::iterator);
    static Vis visualizeDllFilterNode(DllFilterNode *, Graph::iterator);
};

Vis Visualizer::Private::visualizeGraph(Graph *p, Graph::iterator i)
{
    if (i == p->end())
        return Vis();
    auto items = visualizeNode(i);
    assert(!items.first.empty());
    auto r = boundingRect(items.first);
    if (++i != p->end())
    {
        auto existingItems = visualizeGraph(p, i);
        assert(!existingItems.first.empty());
        auto from = items.first.back();
        auto to = existingItems.first.front();
        items.second.push_back(new ConnectorGraphicsItem(from, to));
        for (auto pItem : existingItems.first)
        {
            pItem->setY(pItem->y() + r.height() + sSpacing);
            items.first.push_back(pItem);
        }
        for (auto pItem : existingItems.second)
        {
            items.second.push_back(pItem);
        }
    }
    return items;
}

Vis Visualizer::Private::visualizeNode(Graph::iterator i)
{
    auto pSourceNode = dynamic_cast<SourceNode *>(*i);
    if (pSourceNode)
        return visualizeSourceNode(pSourceNode, i);
    auto pApplicationNode = dynamic_cast<BasicApplicationNode *>(*i);
    if (pApplicationNode)
        return visualizeApplicationNode(pApplicationNode, i);
    auto pSplitterNode = dynamic_cast<SplitterNode *>(*i);
    if (pSplitterNode)
        return visualizeSplitterNode(pSplitterNode, i);
    auto pCSPNode = dynamic_cast<CSPNode*>(*i);
    if (pCSPNode)
        return visualizeCSPNode(pCSPNode, i);
    auto pLinearClassifierNode = dynamic_cast<LinearClassifierNode *>(*i);
    if (pLinearClassifierNode)
        return visualizeLinearClassifierNode(pLinearClassifierNode, i);
    auto pPlaceholderNode = dynamic_cast<PlaceholderNode *>(*i);
    if (pPlaceholderNode)
        return visualizePlaceholderNode(pPlaceholderNode, i);
    auto pDllFilterNode = dynamic_cast<DllFilterNode *>(*i);
    if (pDllFilterNode)
        return visualizeDllFilterNode(pDllFilterNode, i);
    if (*i)
        return visualizeNode(*i, i);
    return Vis();
}

Vis Visualizer::Private::visualizeNode(Node * pNode, Graph::iterator i)
{
    Vis items;
    auto pItem = new FilterGraphicsItem(i);
    if (pNode->framed())
        pItem->setFrameWidth(3);
    pItem->setMarked(pNode->marked());
    items.first.push_back(pItem);
    return items;
}

Vis Visualizer::Private::visualizeSourceNode(SourceNode *p, Graph::iterator i)
{
    auto vis = visualizeNode(static_cast<Node *>(p), i);
    Assert(!vis.first.empty());
    vis.first.front()->setColor(Qt::green);
    return vis;
}

Vis Visualizer::Private::visualizeCSPNode(CSPNode* p, Graph::iterator i)
{
    auto vis = visualizeNode(static_cast<Node*>(p), i);
    Assert(!vis.first.empty());
    vis.first.front()->setColor(QColor(Qt::red).lighter());
    return vis;
}

Vis Visualizer::Private::visualizeLinearClassifierNode(LinearClassifierNode *p, Graph::iterator i)
{
    auto vis = visualizeNode(static_cast<Node *>(p), i);
    Assert(!vis.first.empty());
    vis.first.front()->setColor(QColor(Qt::red).lighter());
    return vis;
}

Vis Visualizer::Private::visualizeApplicationNode(BasicApplicationNode *p, Graph::iterator i)
{
    auto vis = visualizeNode(static_cast<Node *>(p), i);
    Assert(!vis.first.empty());
    vis.first.front()->setColor(Qt::green);
    return vis;
}

Vis Visualizer::Private::visualizeSplitterNode(SplitterNode *p, Graph::iterator i)
{
    auto items = visualizeNode(static_cast<Node *>(p), i);
    QRectF r = boundingRect(items.first);

    auto pSubchainItem1 = new SubchainGraphicsItem(&p->graph1());
    QRectF r1 = pSubchainItem1->boundingRect();
    pSubchainItem1->setX(r.left() + r.width() / 2 - r1.width() / 2);
    pSubchainItem1->setY(r.bottom() + r.height());
    items.first.push_back(pSubchainItem1);

    auto pSubchainItem2 = new SubchainGraphicsItem(&p->graph2());
    QRectF r2 = pSubchainItem2->boundingRect();
    pSubchainItem2->setX(r.left() + r.width() / 2 + r2.width() / 2);
    pSubchainItem2->setY(r.bottom() + r.height());
    items.first.push_back(pSubchainItem2);

    r = boundingRect(items.first);

    auto items1 = visualizeGraph(&p->graph1(), p->graph1().begin());
    if (items1.first.empty())
        items1.first.push_back(new FilterGraphicsItem(p->graph1().begin()));
    r1 = boundingRect(items1.first);
    for (auto pItem : items1.first)
    {
        pItem->setX(pItem->x() - r1.width() / 2 - sSpacing / 2);
        pItem->setY(pItem->y() + r.height() + sSpacing);
        items.first.push_back(pItem);
    }
    for (auto pItem : items1.second)
        items.second.push_back(pItem);
    items.second.push_back(new ConnectorGraphicsItem(pSubchainItem1, items1.first.front()));
    auto items2 = visualizeGraph(&p->graph2(), p->graph2().begin());
    if (items2.first.empty())
        items2.first.push_back(new FilterGraphicsItem(p->graph2().begin()));
    r2 = boundingRect(items2.first);
    for (auto pItem : items2.first)
    {
        pItem->setX(pItem->x() + r2.width() / 2 + sSpacing / 2);
        pItem->setY(pItem->y() + r.height() + sSpacing);
        items.first.push_back(pItem);
    }
    for (auto pItem : items2.second)
        items.second.push_back(pItem);
    items.second.push_back(new ConnectorGraphicsItem(pSubchainItem2, items2.first.front()));
    r = boundingRect(items.first);
    auto pItem = new FilterGraphicsItem(Graph::iterator());
    pItem->setY(pItem->y() + r.height() + sSpacing);
    items.first.push_back(pItem);
    items.second.push_back(new ConnectorGraphicsItem(items1.first.back(), pItem, p->graph1().end()));
    items.second.push_back(new ConnectorGraphicsItem(items2.first.back(), pItem, p->graph2().end()));
    return items;
}

Vis Visualizer::Private::visualizePlaceholderNode(PlaceholderNode* p, Graph::iterator i)
{
    auto vis = visualizeNode(static_cast<Node*>(p), i);
    Assert(!vis.first.empty());
    vis.first.front()->setColor(Qt::lightGray);
    vis.first.front()->setToolTip(
        "This is a placeholder node for an undefined filter.\n"
        "You cannot run an analysis with a graph containing placeholder nodes."
    );
    return vis;
}

Vis Visualizer::Private::visualizeDllFilterNode(DllFilterNode *p, Graph::iterator i)
{
    auto vis = visualizeNode(static_cast<Node *>(p), i);
    Assert(!vis.first.empty());
    if (!p->error().empty())
    {
        vis.first.front()->setColor(QColor(Qt::red));
        vis.first.front()->setToolTip(QString::fromUtf8(p->error().c_str()));
    }
    else
    {
        vis.first.front()->setColor(QColor(Qt::blue).lighter(175));
        vis.first.front()->setToolTip(QString::fromUtf8(p->filterPath().c_str()));
    }
    return vis;
}

Visualizer::Visualizer(QGraphicsScene *pScene) : p(new Private)
{
    p->mpScene = pScene;
}

Visualizer::~Visualizer()
{
    delete p;
}

void Visualizer::visualize(Graph &graph)
{
    auto items = Private::visualizeGraph(&graph, graph.begin());
    if (p->mpScene)
    {
        for (auto pItem : items.first)
            p->mpScene->addItem(pItem);
        for (auto pItem : items.second)
            p->mpScene->addItem(pItem);
    }
}

} // namespace FilterGraph
