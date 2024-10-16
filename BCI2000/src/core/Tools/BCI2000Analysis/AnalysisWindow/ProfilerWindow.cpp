////////////////////////////////////////////////////////////////////////////////
// $Id: ProfilerWindow.cpp 8268 2024-07-23 16:38:37Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Profiler results window for BCI2000 Analysis.
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
#include "ProfilerWindow.h"

#include <fstream>

#include <QAction>
#include <QBoxLayout>
#include <QFileDialog>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QTreeWidget>

#include "EditMenu.h"
#include "FileDialog.h"
#include "HelpMenu.h"
#include "WindowMenu.h"
#include "WindowSettings.h"

#include "FilterGraphNode.h"
#include "GenericFilter.h"
#include "SplitterNode.h"
#include "StaticNodes.h"
#include "ApplicationNodes.h"

#include "IndentingOStreambuf.h"

namespace
{
double round(double d, int n)
{
    for (int i = 0; i < n; ++i)
        d *= 10;
    d = ::floor(d + 0.5);
    for (int i = 0; i < n; ++i)
        d /= 10;
    return d;
}

struct ExpandingTreeWidget : QTreeWidget
{
    // Adapt tree widget's minimum size to its content.
    QSize minimumSizeHint() const override
    {
        QSize hsize = header()->sizeHint();
        QSize vsize = viewportSizeHint();
        int width = std::max(hsize.width(), vsize.width()), height = hsize.height() + vsize.height();
        return QSize(width + 2, height + 5);
    }
};
} // namespace

struct ProfilerWindow::Private : QObject
{
    FilterGraph::Graph mGraph;
    ExpandingTreeWidget *mpTree;
    QWidget *mpSelf;

    Private(QWidget *parent) : QObject(parent), mpSelf(parent)
    {
    }
    void setGraph(const FilterGraph::Graph &);
    static void buildSubtree(const FilterGraph::Graph &, QTreeWidgetItem *);
    static void setItemData(QTreeWidgetItem *, const std::string &, const struct GenericFilter::PerformanceData &);
    void onFileSave();
    void writeToStream(std::ostream &) const;
    static void writeToStream(QTreeWidgetItem *, int, std::ostream &);
    static int getNameWidth(QTreeWidgetItem *, int);
};

void ProfilerWindow::Private::setGraph(const FilterGraph::Graph &graph)
{
    mGraph = graph;
    mpTree->clear();
    mpTree->setColumnCount(3);
    mpTree->setHeaderLabels(QStringList() << "Name"
                                          << "Average"
                                          << "Maximum");
    mpTree->header()->setStretchLastSection(false);
    buildSubtree(mGraph, mpTree->invisibleRootItem());
    for (int i = 0; i < mpTree->columnCount(); ++i)
        mpTree->resizeColumnToContents(i);
    mpTree->clearSelection();
    mpTree->setCurrentItem(nullptr);
}

void ProfilerWindow::Private::buildSubtree(const FilterGraph::Graph &graph, QTreeWidgetItem *pItem)
{
    auto pNewItem = new QTreeWidgetItem(pItem);
    std::string name = graph.name();
    if (name.empty())
        name = "Total";
    setItemData(pNewItem, name, graph.performanceData());
    pNewItem->setExpanded(true);
    pItem->addChild(pNewItem);
    for (auto p : graph)
    {
        if (dynamic_cast<const FilterGraph::StaticNode *>(p))
            continue;
        if (dynamic_cast<const FilterGraph::BasicApplicationNode*>(p))
            continue;
        auto pNewItem2 = new QTreeWidgetItem(pNewItem);
        setItemData(pNewItem2, p->name(), p->performanceData());
        pNewItem->addChild(pNewItem2);
        auto pSplitter = dynamic_cast<const FilterGraph::SplitterNode *>(p);
        if (pSplitter)
        {
            buildSubtree(pSplitter->graph1(), pNewItem2);
            buildSubtree(pSplitter->graph2(), pNewItem2);
        }
        pNewItem2->setExpanded(true);
    }
}

void ProfilerWindow::Private::setItemData(QTreeWidgetItem *pItem, const std::string &name,
                                          const struct GenericFilter::PerformanceData &data)
{
    QString text = QString::fromUtf8(name.c_str());
    pItem->setText(0, text);

    if (data.count > 0)
    {

        double avg = data.totalDuration / data.count / data.blockDuration * 100;
        avg = round(avg, 2);
        pItem->setText(1, QString("%1%").arg(QString::number(avg, 'f', 2)));
        QColor background;
        background.setHsvF(0, std::min(1.0, avg / 100.), 1);
        pItem->setBackground(1, background);
        pItem->setTextAlignment(1, Qt::AlignRight | Qt::AlignVCenter);

        double max = data.maxDuration / data.blockDuration * 100;
        max = round(max, 2);
        pItem->setText(2, QString("%2%").arg(QString::number(max, 'f', 2)));
        background.setHsvF(0, std::min(1.0, max / 100.), 1);
        pItem->setBackground(2, background);
        pItem->setTextAlignment(2, Qt::AlignRight | Qt::AlignVCenter);
    }
}

void ProfilerWindow::Private::onFileSave()
{
    QString fileName = FileDialog::getSaveFileName(mpSelf, "Save profiler report", "Text files (*.txt)", "output",
                                                   "ProfilerReport.txt");
    if (!fileName.isEmpty())
    {
        std::ofstream file(fileName.toLocal8Bit().constData());
        writeToStream(file);
        if (!file)
            QMessageBox::critical(mpSelf, "Error", "Could not write to file\n" + fileName);
    }
}

void ProfilerWindow::Private::writeToStream(std::ostream &os) const
{
    auto pRoot = mpTree->invisibleRootItem();
    int nameWidth = getNameWidth(pRoot, 0);
    auto &data = mGraph.performanceData();
    os << "BCI2000 Analysis Profiler Report\n"
       << "Execution time in terms of Sample Block Duration\n"
       << "\n"
       << "Sample block duration = " << data.blockDuration * 1e3 << "ms\n"
       << "Sample blocks processed = " << data.count << "\n"
       << "\n";
    os.width(nameWidth);
    os << std::left << "Name";
    os << "\tAverage\tMaximum\n" << std::string(50, '=') << "\n";
    for (int i = 0; i < pRoot->childCount(); ++i)
        writeToStream(pRoot->child(i), nameWidth, os);
}

void ProfilerWindow::Private::writeToStream(QTreeWidgetItem *pItem, int nameWidth, std::ostream &os)
{
    os.width(nameWidth);
    os << std::left << pItem->text(0).toUtf8().constData() << '\t' << pItem->text(1).toUtf8().constData() << '\t'
       << pItem->text(2).toUtf8().constData() << '\n';
    IndentingOStreambuf indent(os, 2);
    for (int i = 0; i < pItem->childCount(); ++i)
        writeToStream(pItem->child(i), nameWidth - 2, os);
}

int ProfilerWindow::Private::getNameWidth(QTreeWidgetItem *pItem, int add)
{
    std::string name = pItem->text(0).toUtf8().constData();
    int nameWidth = name.length() + add;
    for (int i = 0; i < pItem->childCount(); ++i)
    {
        int width = getNameWidth(pItem->child(i), add + 2);
        nameWidth = std::max(width, nameWidth);
    }
    return nameWidth;
}

ProfilerWindow::ProfilerWindow(const FilterGraph::Graph &graph) : QMainWindow(nullptr), p(new Private(this))
{
    setAttribute(Qt::WA_DeleteOnClose);

    setWindowTitle("Profiler Results");

    QMenu *pMenu = QMainWindow::menuBar()->addMenu("&File");
    QAction *pAction = pMenu->addAction("&Save...");
    pAction->setShortcut(Qt::CTRL | Qt::Key_S);
    connect(pAction, &QAction::triggered, p, &Private::onFileSave);
    pMenu->addSeparator();
    pAction = pMenu->addAction("&Close");
    pAction->setShortcut(Qt::CTRL | Qt::Key_W);
    connect(pAction, &QAction::triggered, this, &QMainWindow::close);

    EditMenu::addToMenuBar(menuBar());
    WindowMenu::addWindow(this);
    HelpMenu::addToMenuBar(menuBar());

    auto pWidget = new QWidget;
    auto pLayout = new QVBoxLayout;
    pWidget->setLayout(pLayout);
    auto &data = graph.performanceData();
    QString heading = "<h2>Execution time in terms of Sample Block Duration</h2>"
                      "Sample block duration = %1ms<br>"
                      "Sample blocks processed = %2";
    auto pLabel = new QLabel;
    pLabel->setText(heading.arg(data.blockDuration * 1e3).arg(data.count));
    pLabel->setWordWrap(true);
    pLayout->addWidget(pLabel);
    p->mpTree = new ExpandingTreeWidget;
    pLayout->addWidget(p->mpTree);
    setCentralWidget(pWidget);

    p->setGraph(graph);

    WindowSettings::read(this);
}

ProfilerWindow::~ProfilerWindow()
{
    WindowSettings::write(this);
    WindowMenu::removeWindow(this);
}
