////////////////////////////////////////////////////////////////////////////////
// $Id: AnalysisLogWidget.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A widget to display the analysis log.
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
#include "AnalysisLogWidget.h"

#include <QBoxLayout>
#include <QTextBrowser>

struct AnalysisLogWidget::Private : QObject
{
    QTextBrowser* mpTextBrowser = nullptr;

    Private(QObject *parent) : QObject(parent)
    {
    }

};

AnalysisLogWidget::AnalysisLogWidget()
: QWidget(nullptr), p(new Private(this))
{
    auto pLayout = new QVBoxLayout;
    pLayout->setContentsMargins(2, 0, 2, 2);
    p->mpTextBrowser = new QTextBrowser;
    pLayout->addWidget(p->mpTextBrowser);
    setLayout(pLayout);
    this->setMinimumHeight(60);
}

AnalysisLogWidget::~AnalysisLogWidget()
{
}

void AnalysisLogWidget::append(const QString& text)
{
    p->mpTextBrowser->append(text);
}
