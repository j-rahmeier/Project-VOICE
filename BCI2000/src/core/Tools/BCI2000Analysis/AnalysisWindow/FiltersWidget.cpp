////////////////////////////////////////////////////////////////////////////////
// $Id: FiltersWidget.cpp 8215 2024-06-21 13:36:37Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A Widget displaying a filter graph.
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
#include "FiltersWidget.h"

#include "ConnectorGraphicsItem.h"
#include "CreateFilterGraph.h"
#include "DllFilterNode.h"
#include "FileDialog.h"
#include "FilterGraphNode.h"
#include "FilterGraphicsItem.h"
#include "GraphVisualizer.h"
#include "SplitterNode.h"
#include "StaticNodes.h"
#include "ApplicationNodes.h"
#include "SubchainGraphicsItem.h"
#include "PlaceholderNode.h"

#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>

struct FiltersWidget::Private : QGraphicsScene
{
    FilterGraph::Graph mGraph;
    bool mEditable;
    FiltersWidget *mpSelf;

    Private(FiltersWidget *parent) : QGraphicsScene(parent), mpSelf(parent), mEditable(false)
    {
    }

    ~Private()
    {
    }

    void computeLayout()
    {
        QGraphicsScene::clear();
        FilterGraph::Visualizer(this).visualize(mGraph);
        QGraphicsScene::setSceneRect(QGraphicsScene::itemsBoundingRect());
    }

    void onCustomContextMenuRequested(const QPoint &where)
    {
        if (!mEditable)
            return;
        auto pItem = mpSelf->itemAt(where);
        auto pFilterItem = dynamic_cast<FilterGraphicsItem *>(pItem);
        auto pSubchainGraphicsItem = dynamic_cast<SubchainGraphicsItem *>(pItem);
        auto pConnectorItem = dynamic_cast<ConnectorGraphicsItem *>(pItem);
        QMenu menu;
        QAction* pAction = nullptr;
        bool isStatic = true, isApplication = false;
        if (pFilterItem)
        {
            FilterGraph::Graph::iterator i = pFilterItem->filterPos();
            if (i && i != i.graph->end()) {
                isStatic = dynamic_cast<FilterGraph::StaticNode*>(*i);
                isApplication = dynamic_cast<FilterGraph::BasicApplicationNode*>(*i);
            }
        }
        if (!isApplication) {
            pAction = menu.addAction("Delete", this, [pFilterItem, this] { onDelete(pFilterItem); });
            pAction->setEnabled(!isStatic);
            pAction = menu.addAction("Rename...", this, [pFilterItem, this] { onRename(pFilterItem); });
            pAction->setEnabled(!isStatic || pSubchainGraphicsItem);
            pAction = menu.addAction("Insert filter...", this, [pConnectorItem, this] { onInsertFilter(pConnectorItem); });
            pAction->setEnabled(pConnectorItem);
            pAction = menu.addAction("Insert splitter", this, [pConnectorItem, this] { onInsertSplitter(pConnectorItem); });
            pAction->setEnabled(pConnectorItem);
        }
        else {
            pAction = menu.addAction("Replace with generic Application node", this, [pFilterItem, this]{ onReplace<FilterGraph::ApplicationNode>(pFilterItem); });
            pAction = menu.addAction("Replace with SMR Application node", this, [pFilterItem, this] { onReplace<FilterGraph::SMRApplicationNode>(pFilterItem); });
            pAction = menu.addAction("Replace with ERP Application node", this, [pFilterItem, this] { onReplace<FilterGraph::ERPApplicationNode>(pFilterItem); });
        }
        menu.exec(mpSelf->mapToGlobal(where));
    }

    void onDelete(FilterGraphicsItem *pItem)
    {
        FilterGraph::Graph::iterator i = pItem->filterPos();
        auto pSplitter = dynamic_cast<FilterGraph::SplitterNode *>(*i);
        bool ok = true;
        if (pSplitter)
        {
            QString name = QString::fromUtf8(pSplitter->name().c_str()),
                    sub1 = QString::fromUtf8(pSplitter->graph1().name().c_str()),
                    sub2 = QString::fromUtf8(pSplitter->graph2().name().c_str());
            QString text = QString("Really delete splitter \"%1\" and its subchains \"%2\" and \"%3\"?")
                               .arg(name)
                               .arg(sub1)
                               .arg(sub2);
            int button = QMessageBox::question(mpSelf, "Delete splitter...", text);
            ok = (button == QMessageBox::Yes);
        }
        if (ok)
        {
            i.graph->erase(i.index);
            computeLayout();
            emit mpSelf->edited();
        }
    }

    void onRename(FilterGraphicsItem *pItem)
    {
        auto pSubchainItem = dynamic_cast<SubchainGraphicsItem *>(pItem);
        QString title = pSubchainItem ? "Rename subchain..." : "Rename filter...";
        QString label = pSubchainItem ? "New subchain name:" : "New filter name:";
        bool ok = false;
        QString newName = QInputDialog::getText(mpSelf, title, label, QLineEdit::Normal, pItem->text(), &ok,
                                                Qt::MSWindowsFixedSizeDialogHint);
        newName = newName.trimmed();
        newName = newName.replace(" ", "_");
        if (ok && !newName.isEmpty())
        {
            pItem->setText(newName);
            computeLayout();
            emit mpSelf->edited();
        }
    }

    template<class T> void onReplace(FilterGraphicsItem* pItem)
    {
        FilterGraph::Graph::iterator i = pItem->filterPos();
        i.graph->erase(i.index);
        i.graph->insert(i.index, new T);
        computeLayout();
        emit mpSelf->edited();
    }

    void onInsertFilter(ConnectorGraphicsItem *pItem)
    {
        auto filterNames = FilterGraph::filterNames();
        QStringList names;
        for (const auto &name : filterNames)
            names.push_back(QString::fromUtf8(name.c_str()));
        names.push_back("Other filter...");
        names.push_back("Filter from DLL...");
        bool ok = false;
        auto name = QInputDialog::getItem(mpSelf, "Insert Filter...", "Filter type:", names, 0, false, &ok,
                                          Qt::MSWindowsFixedSizeDialogHint);
        if (ok)
        {
            FilterGraph::Node *pNode = nullptr;
            if (names.indexOf(name) == names.count() - 2)
            {   // Placeholder
                name = QInputDialog::getText(mpSelf, "Set filter name...", "Name:");
                if (!name.isEmpty())
                {
                    pNode = FilterGraph::PlaceholderNode::create();
                    pNode->setName(name.toUtf8().constData());
                }
            }
            else if (names.indexOf(name) == names.count() - 1)
            {   // Filter from DLL
                QString fileName =
                    FileDialog::getOpenFileName(mpSelf, "Choose a BCI2000 filter DLL", "Shared libraries (*.dll)");
                if (!fileName.isEmpty())
                {
                    auto pDllFilterNode = new FilterGraph::DllFilterNode;
                    if (pDllFilterNode->setFilterPath(fileName.toLocal8Bit().constData()))
                    {
                        pNode = pDllFilterNode;
                    }
                    else
                    {
                        QString err = QString::fromUtf8(pDllFilterNode->error().c_str());
                        QMessageBox::critical(mpSelf, "Filter DLL error", err);
                        delete pDllFilterNode;
                    }
                }
            }
            else
            {
                pNode = FilterGraph::newFilterNode(name.toUtf8().constData());
            }
            if (pNode)
            {
                auto i = pItem->toPos();
                i.graph->insert(i.index, pNode);
                computeLayout();
                emit mpSelf->edited();
            }
        }
    }

    void onInsertSplitter(ConnectorGraphicsItem *pItem)
    {
        auto i = pItem->toPos();
        i.graph->insert(i.index, new FilterGraph::SplitterNode("Splitter", FilterGraph::Graph("Chain1"),
                                                               FilterGraph::Graph("Chain2")));
        computeLayout();
        emit mpSelf->edited();
    }
};

FiltersWidget::FiltersWidget(QWidget *parent) : QGraphicsView(parent), p(new Private(this))
{
    viewport()->setAttribute(Qt::WA_AcceptTouchEvents, false); // work around warning messages on macOS
    setScene(p);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QGraphicsView::customContextMenuRequested, p, &Private::onCustomContextMenuRequested);
}

FiltersWidget::~FiltersWidget()
{
}

void FiltersWidget::clearGraph()
{
    setGraph(FilterGraph::Graph());
}

void FiltersWidget::updateGraph()
{
    p->computeLayout();
}

void FiltersWidget::setGraph(const FilterGraph::Graph &graph)
{
    if (p->mGraph != graph)
    {
        p->mGraph = graph;
        p->computeLayout();
        emit graphReplaced();
    }
}

const FilterGraph::Graph &FiltersWidget::graph() const
{
    return p->mGraph;
}

void FiltersWidget::setEditable(bool editable)
{
    p->mEditable = editable;
}

bool FiltersWidget::editable() const
{
    return p->mEditable;
}
