////////////////////////////////////////////////////////////////////////////////
// $Id: AnalysisParamWidget.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A widget for analysis parameters.
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
#include "AnalysisParamWidget.h"
#include "BCI2000FileReader.h"
#include <QBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

AnalysisParamWidget::AnalysisParams AnalysisParamWidget::AnalysisParams::createFromFile(const BCI2000FileReader *pFile)
{
    AnalysisParams params;
    if (pFile->States()->Exists("TargetCode"))
    {
        if (pFile->States()->Exists("Feedback"))
        {
            params.condition1 = "Feedback && (TargetCode==1)";
            params.condition2 = "Feedback && (TargetCode==2)";
        }
        else
        {
            params.condition1 = "TargetCode==1";
            params.condition2 = "TargetCode==2";
        }
    }
    if (pFile->States()->Exists("StimulusCodeRes") && pFile->States()->Exists("StimulusTypeRes"))
    {
        params.condition1 = "StimulusCodeRes != 0 && StimulusTypeRes == 0";
        params.condition2 = "StimulusCodeRes != 0 && StimulusTypeRes != 0";
    }
    if (pFile->States()->Exists("StimulusCode") && !pFile->States()->Exists("StimulusCodeRes"))
    {
        if (pFile->States()->Exists("Feedback"))
        {
            params.condition1 = "Feedback && (StimulusCode==1)";
            params.condition2 = "Feedback && (StimulusCode==2)";
        }
        else
        {
            params.condition1 = "StimulusCode==1";
            params.condition2 = "StimulusCode==2";
        }
    }
    return params;
}

struct AnalysisParamWidget::Private : QObject
{
    QLineEdit *mpCondition1 = nullptr, *mpCondition2 = nullptr;
    Private(QObject *parent) : QObject(parent)
    {
    }
};

AnalysisParamWidget::AnalysisParamWidget(QWidget *parent) : QWidget(parent), p(new Private(this))
{
    auto pLayout = new QVBoxLayout;
    pLayout->setContentsMargins(4, 0, 4, 0);
    setLayout(pLayout);
    auto pGroupBox = new QGroupBox("Filter Parameters", this);
    pLayout->addWidget(pGroupBox);
    auto pLayout3 = new QVBoxLayout;
    pGroupBox->setLayout(pLayout3);
    auto pButton = new QPushButton("Edit...", pGroupBox);
    pLayout3->addWidget(pButton);
    connect(pButton, &QPushButton::clicked, this, &AnalysisParamWidget::editFilterParamsClicked);
    pButton = new QPushButton("Reset...", pGroupBox);
    pLayout3->addWidget(pButton);
    connect(pButton, &QPushButton::clicked, this, &AnalysisParamWidget::resetFilterParamsClicked);
    pGroupBox = new QGroupBox("Conditions", this);
    pLayout->addWidget(pGroupBox);
    auto pLayout2 = new QFormLayout;
    p->mpCondition1 = new QLineEdit(this);
    p->mpCondition1->setMinimumWidth(10 * 20);
    pLayout2->addRow("Condition1", p->mpCondition1);
    p->mpCondition2 = new QLineEdit(this);
    p->mpCondition2->setMinimumWidth(10 * 20);
    pLayout2->addRow("Condition2", p->mpCondition2);
    pGroupBox->setLayout(pLayout2);
    connect(p->mpCondition1, &QLineEdit::textEdited, this, &AnalysisParamWidget::conditionsEdited);
    connect(p->mpCondition2, &QLineEdit::textEdited, this, &AnalysisParamWidget::conditionsEdited);
}

AnalysisParamWidget::~AnalysisParamWidget()
{
}

void AnalysisParamWidget::setParams(const AnalysisParams &params)
{
    p->mpCondition1->setText(QString::fromUtf8(params.condition1.c_str()));
    p->mpCondition2->setText(QString::fromUtf8(params.condition2.c_str()));
}

AnalysisParamWidget::AnalysisParams AnalysisParamWidget::params() const
{
    AnalysisParams params;
    params.condition1 = p->mpCondition1->text().toUtf8().constData();
    params.condition2 = p->mpCondition2->text().toUtf8().constData();
    return params;
}

void AnalysisParamWidget::AnalysisParams::serialize(std::ostream &os) const
{
    os << condition1 << '\n' << condition2 << '\n';
}

AnalysisParamWidget::AnalysisParams AnalysisParamWidget::AnalysisParams::unserialize(std::istream &is)
{
    AnalysisParams params;
    is >> std::ws;
    std::getline(is, params.condition1);
    std::getline(is, params.condition2);
    return params;
}
