//////////////////////////////////////////////////////////////////////
// $Id$
// Authors: mellinger@neurotechcenter.org
// Description: A log widget for the BCI2000 Watchdog program.
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
///////////////////////////////////////////////////////////////////////
#include "LogWidget.h"

#include <QTextEdit>
#include <QHBoxLayout>
#include <QDateTime>

LogWidget::LogWidget(QWidget* parent)
: QWidget(parent),
  mpTextEdit(nullptr)
{
    mpTextEdit = new QTextEdit;
    mpTextEdit->setReadOnly(true);
    auto* pLayout = new QHBoxLayout;
    pLayout->addWidget(mpTextEdit);
    pLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(pLayout);
}

LogWidget::~LogWidget()
{
}

void LogWidget::Clear()
{
    mpTextEdit->clear();
}

void LogWidget::AddEntry(QString text, QColor color)
{
    mpTextEdit->setTextColor(color);
    QString format = "MMM dd yy hh:mm:ss";
    QString line = QDateTime::currentDateTime().toString(format) + ": " + text;
    mpTextEdit->append(line);
    mpTextEdit->repaint();
}


