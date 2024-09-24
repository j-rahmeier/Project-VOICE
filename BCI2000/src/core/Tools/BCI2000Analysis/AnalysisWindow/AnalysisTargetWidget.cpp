////////////////////////////////////////////////////////////////////////////////
// $Id: AnalysisTargetWidget.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A widget to select an analysis target.
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
#include "AnalysisTargetWidget.h"

#include "FiltersWidget.h"
#include "GraphSearch.h"
#include "LinearClassifierNode.h"
#include "CSPNode.h"
#include "ApplicationNodes.h"

#include <QBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QRadioButton>
#include <QButtonGroup>

Q_DECLARE_METATYPE(FilterGraph::Node*);

struct AnalysisTargetWidget::Private : QObject
{
    QListWidget* mpListWidget = nullptr;
    QButtonGroup* mpButtonGroup = nullptr;
    FiltersWidget* mpFiltersWidget = nullptr;

    Private(QObject *parent) : QObject(parent)
    {
    }

    void onGraphChanged()
    {
        mpListWidget->clear();
        if (!mpFiltersWidget->graph().empty())
        {
            auto pItem = new QListWidgetItem(mpListWidget);
            auto pButton = new QRadioButton("Profiler");
            QRadioButton* pPreselectedButton = pButton;
            pButton->setProperty("target", QVariant::fromValue(static_cast<FilterGraph::Node*>(nullptr)));
            mpListWidget->setItemWidget(pItem, pButton);
            connect(pButton, &QRadioButton::toggled, this, &Private::onButtonToggled);

            auto cspNodes = FilterGraph::search<FilterGraph::CSPNode>(mpFiltersWidget->graph());
            for (auto cspNode : cspNodes)
            {
                std::string path = FilterGraph::nodePath(mpFiltersWidget->graph(), cspNode);
                pItem = new QListWidgetItem(mpListWidget);
                pButton = new QRadioButton(QString::fromUtf8(path.c_str()));
                pPreselectedButton = pButton;
                pButton->setProperty("target", QVariant::fromValue(static_cast<FilterGraph::Node*>(cspNode)));
                mpListWidget->setItemWidget(pItem, pButton);
                connect(pButton, &QRadioButton::toggled, this, &Private::onButtonToggled);
            }
            
            auto linearClassifiers = FilterGraph::search<FilterGraph::LinearClassifierNode>(mpFiltersWidget->graph());
            for (auto pClassifier : linearClassifiers)
            {
                std::string path = FilterGraph::nodePath(mpFiltersWidget->graph(), pClassifier);
                pItem = new QListWidgetItem(mpListWidget);
                pButton = new QRadioButton(QString::fromUtf8(path.c_str()));
                pPreselectedButton = pButton;
                pButton->setProperty("target", QVariant::fromValue(static_cast<FilterGraph::Node*>(pClassifier)));
                mpListWidget->setItemWidget(pItem, pButton);
                connect(pButton, &QRadioButton::toggled, this, &Private::onButtonToggled);
            }

            auto applications = FilterGraph::search<FilterGraph::BasicApplicationNode>(mpFiltersWidget->graph());
            for (auto pApplication : applications)
            {
                pItem = new QListWidgetItem(mpListWidget);
                pButton = new QRadioButton(QString::fromUtf8(pApplication->name().c_str()));
                pButton->setProperty("target", QVariant::fromValue(static_cast<FilterGraph::Node*>(pApplication)));
                mpListWidget->setItemWidget(pItem, pButton);
                connect(pButton, &QRadioButton::toggled, this, &Private::onButtonToggled);
            }

            pPreselectedButton->setChecked(true);
        }
    }

    void onButtonToggled(bool value)
    {
        auto pButton = dynamic_cast<QRadioButton*>(sender());
        if (pButton)
        {
            auto pNode = pButton->property("target").value<FilterGraph::Node*>();
            if (pNode)
            {
                pNode->setMarked(value);
                mpFiltersWidget->updateGraph();
            }
        }
    }
};

AnalysisTargetWidget::AnalysisTargetWidget(FiltersWidget* pFiltersWidget)
: QWidget(nullptr), p(new Private(this))
{
    auto pLayout = new QVBoxLayout;
    pLayout->setContentsMargins(2, 0, 2, 0);
    p->mpListWidget = new QListWidget;
    pLayout->addWidget(p->mpListWidget);
    setLayout(pLayout);

    p->mpListWidget->setSpacing(2);
    this->setFixedHeight(100);

    p->mpFiltersWidget = pFiltersWidget;
    connect(pFiltersWidget, &FiltersWidget::graphReplaced, p, &Private::onGraphChanged);
    connect(pFiltersWidget, &FiltersWidget::edited, p, &Private::onGraphChanged);
}

AnalysisTargetWidget::~AnalysisTargetWidget()
{
}

FilterGraph::Node* AnalysisTargetWidget::target() const
{
    for (int row = 0; row < p->mpListWidget->count(); ++row)
    {
        auto pWidget = p->mpListWidget->itemWidget(p->mpListWidget->item(row));
        Assert(pWidget);
        auto pRadioButton = dynamic_cast<QRadioButton*>(pWidget);
        Assert(pRadioButton);
        if (pRadioButton->isChecked())
            return pRadioButton->property("target").value<FilterGraph::Node*>();
    }
    return nullptr;
}

