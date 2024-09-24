//////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: A widget to display notes that were entered during a recording.
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
//////////////////////////////////////////////////////////////////////////////
#include "NotesWidget.h"

#include <QBoxLayout>
#include <QListWidget>
#include <fstream>

struct NotesWidget::Private : QObject
{
    NotesWidget* mpParent;
    QListWidget* mpListWidget = nullptr;
    bool mUserChangedRow = true;

    Private(NotesWidget* pParent) : QObject(pParent), mpParent(pParent)
    {
    }

    void onCurrentRowChanged(int row)
    {
        if (mUserChangedRow && row >= 0)
            mpParent->userClickedNote(row);
    }
};

NotesWidget::NotesWidget(QWidget *inParent)
: QWidget(inParent), p(new Private(this))
{
    QHBoxLayout* pLayout = new QHBoxLayout;
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
    p->mpListWidget = new QListWidget;
    p->mpListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    p->mpListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    p->mpListWidget->setWordWrap(true);
    p->mpListWidget->setSpacing(4);
    pLayout->addWidget(p->mpListWidget);
    connect(p->mpListWidget, &QListWidget::currentRowChanged, p, &Private::onCurrentRowChanged);
}

NotesWidget::~NotesWidget()
{
  // p will be deleted by Qt
}

NotesWidget &NotesWidget::setLinePosition(int inLine)
{
    p->mUserChangedRow = false;
    if (inLine <= 0 || inLine > p->mpListWidget->count())
        p->mpListWidget->clearSelection();
    else
        p->mpListWidget->setCurrentRow(inLine - 1);
    p->mUserChangedRow = true;
    return *this;
}

NotesWidget &NotesWidget::setNotesFile(const std::string &inFile)
{
    p->mpListWidget->clear();
    if (!inFile.empty())
    {
        p->mUserChangedRow = false;
        std::ifstream file(inFile);
        std::string line;
        while (std::getline(file, line))
        {
            std::string datetime, text;
            auto pos = line.find('\t');
            if (pos != std::string::npos)
                datetime = line.substr(0, pos);
            pos = line.rfind('\t');
            if (pos != std::string::npos)
                text = line.substr(pos + 1);
            else
                text = line;
            while (!text.empty() && std::isspace(text.back()))
                text.pop_back();
            QListWidgetItem* pItem = new QListWidgetItem(QString::fromUtf8(text.c_str()));
            pItem->setToolTip(QString::fromUtf8(datetime.c_str()));
            p->mpListWidget->addItem(pItem);
        }
        p->mUserChangedRow = true;
    }
    return *this;
}

