////////////////////////////////////////////////////////////////////////////////
// $Id: EditMenu.cpp 8268 2024-07-23 16:38:37Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A menu displaying standard edit commands.
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
#include "EditMenu.h"

#include <QAction>
#include <QApplication>
#include <QBoxLayout>
#include <QClipboard>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QPainter>
#include <QTextEdit>
#include <QTextBrowser>
#include <QGraphicsView>

namespace
{
struct Handler : QObject
{
    QAction mUndo, mRedo, mCut, mCopy, mPaste, mDelete, mSelectAll;
    QWidget *mpFocusWidget;
    Handler()
        : mUndo("&Undo"), mRedo("&Redo"), mCut("Cu&t"), mCopy("&Copy"), mPaste("&Paste"),
          mDelete("&Delete"), mSelectAll("Select &All"), mpFocusWidget(nullptr)
    {
        mUndo.setShortcut(Qt::CTRL | Qt::Key_Z);
        mRedo.setShortcut(Qt::CTRL | Qt::Key_Y);
        mCut.setShortcut(Qt::CTRL | Qt::Key_X);
        mCopy.setShortcut(Qt::CTRL | Qt::Key_C);
        mPaste.setShortcut(Qt::CTRL | Qt::Key_V);
        mDelete.setShortcut(Qt::Key_Delete);
        mSelectAll.setShortcut(Qt::CTRL | Qt::Key_A);
        connect(&mUndo, &QAction::triggered, this, &Handler::onUndo);
        connect(&mRedo, &QAction::triggered, this, &Handler::onRedo);
        connect(&mCut, &QAction::triggered, this, &Handler::onCut);
        connect(&mCopy, &QAction::triggered, this, &Handler::onCopy);
        connect(&mPaste, &QAction::triggered, this, &Handler::onPaste);
        connect(&mDelete, &QAction::triggered, this, &Handler::onDelete);
        connect(&mSelectAll, &QAction::triggered, this, &Handler::onSelectAll);
        connect(qApp, &QApplication::focusChanged, this, &Handler::onFocusChanged);
    }
    void onMenuAboutToShow()
    {
        mUndo.setEnabled(false);
        mRedo.setEnabled(false);
        mCut.setEnabled(false);
        mCopy.setEnabled(false);
        mPaste.setEnabled(false);
        mDelete.setEnabled(false);
        mSelectAll.setEnabled(false);
        auto pTextBrowser = dynamic_cast<QTextBrowser*>(mpFocusWidget);
        if (pTextBrowser)
        {
            if (pTextBrowser->textCursor().hasSelection())
                mCopy.setEnabled(true);
            mSelectAll.setEnabled(true);
            return;
        }

        auto pTextEdit = dynamic_cast<QTextEdit*>(mpFocusWidget);
        if (pTextEdit)
        {
            if (pTextEdit->isUndoRedoEnabled())
            {
                mUndo.setEnabled(true);
                mRedo.setEnabled(true);
            }
            if (pTextEdit->canPaste())
                mPaste.setEnabled(true);
            if (pTextEdit->textCursor().hasSelection())
            {
                mCopy.setEnabled(true);
                if (!pTextEdit->isReadOnly())
                {
                    mCut.setEnabled(true);
                    mDelete.setEnabled(true);
                }
            }
            mSelectAll.setEnabled(true);
            return;
        }
        auto pLineEdit = dynamic_cast<QLineEdit*>(mpFocusWidget);
        if (pLineEdit)
        {
            if (pLineEdit->isUndoAvailable())
                mUndo.setEnabled(true);
            if (pLineEdit->isRedoAvailable())
                mRedo.setEnabled(true);
            if (!QApplication::clipboard()->text().isEmpty() && !pLineEdit->isReadOnly())
                mPaste.setEnabled(true);
            if (!pLineEdit->selectedText().isEmpty())
            {
                mCopy.setEnabled(true);
                if (!pLineEdit->isReadOnly())
                {
                    mCut.setEnabled(true);
                    mDelete.setEnabled(true);
                }
            }
            mSelectAll.setEnabled(true);
            return;
        }
        auto pCopyWidget = dynamic_cast<EditMenu::CopyWidget*>(mpFocusWidget);
        if (pCopyWidget)
        {
            mCopy.setEnabled(true);
        }
        auto pGraphicsView = dynamic_cast<QGraphicsView*>(mpFocusWidget);
        if (pGraphicsView)
        {
            mCopy.setEnabled(true);
        }
    }
    void onFocusChanged(QWidget *old, QWidget *now)
    {
        if (dynamic_cast<QMenu *>(now) || dynamic_cast<QMenuBar *>(now))
            return;

        mpFocusWidget = now;
    }
    void onUndo()
    {
        auto pTextEdit = dynamic_cast<QTextEdit *>(mpFocusWidget);
        if (pTextEdit)
        {
            pTextEdit->undo();
            return;
        }
        auto pLineEdit = dynamic_cast<QLineEdit *>(mpFocusWidget);
        if (pLineEdit)
        {
            pLineEdit->undo();
            return;
        }
    }
    void onRedo()
    {
        auto pTextEdit = dynamic_cast<QTextEdit *>(mpFocusWidget);
        if (pTextEdit)
        {
            pTextEdit->redo();
            return;
        }
        auto pLineEdit = dynamic_cast<QLineEdit *>(mpFocusWidget);
        if (pLineEdit)
        {
            pLineEdit->redo();
            return;
        }
    }
    void onCut()
    {
        auto pTextEdit = dynamic_cast<QTextEdit *>(mpFocusWidget);
        if (pTextEdit)
        {
            pTextEdit->cut();
            return;
        }
        auto pLineEdit = dynamic_cast<QLineEdit *>(mpFocusWidget);
        if (pLineEdit)
        {
            pLineEdit->cut();
            return;
        }
    }
    void onCopy()
    {
        auto pTextBrowser = dynamic_cast<QTextBrowser*>(mpFocusWidget);
        if (pTextBrowser)
        {
            pTextBrowser->copy();
            return;
        }
        auto pTextEdit = dynamic_cast<QTextEdit *>(mpFocusWidget);
        if (pTextEdit)
        {
            pTextEdit->copy();
            return;
        }
        auto pLineEdit = dynamic_cast<QLineEdit *>(mpFocusWidget);
        if (pLineEdit)
        {
            pLineEdit->copy();
            return;
        }
        auto pCopyWidget = dynamic_cast<EditMenu::CopyWidget *>(mpFocusWidget);
        if (pCopyWidget)
        {
            pCopyWidget->copy();
        }
        auto pGraphicsView = dynamic_cast<QGraphicsView*>(mpFocusWidget);
        if (pGraphicsView)
        {
            EditMenu::CopyWidget::doCopy(pGraphicsView);
        }
    }
    void onPaste()
    {
        auto pTextEdit = dynamic_cast<QTextEdit *>(mpFocusWidget);
        if (pTextEdit)
        {
            pTextEdit->paste();
            return;
        }
        auto pLineEdit = dynamic_cast<QLineEdit *>(mpFocusWidget);
        if (pLineEdit)
        {
            pLineEdit->paste();
            return;
        }
    }
    void onDelete()
    {
        auto pTextEdit = dynamic_cast<QTextEdit *>(mpFocusWidget);
        if (pTextEdit)
        {
            pTextEdit->textCursor().removeSelectedText();
            return;
        }
        auto pLineEdit = dynamic_cast<QLineEdit *>(mpFocusWidget);
        if (pLineEdit)
        {
            pLineEdit->backspace();
            return;
        }
    }
    void onSelectAll()
    {
        auto pTextBrowser = dynamic_cast<QTextBrowser *>(mpFocusWidget);
        if (pTextBrowser)
        {
            pTextBrowser->selectAll();
            return;
        }
        auto pTextEdit = dynamic_cast<QTextEdit *>(mpFocusWidget);
        if (pTextEdit)
        {
            pTextEdit->textCursor().select(QTextCursor::Document);
            return;
        }
        auto pLineEdit = dynamic_cast<QLineEdit *>(mpFocusWidget);
        if (pLineEdit)
        {
            pLineEdit->selectAll();
            return;
        }
    }
};
Handler *sHandler()
{
    static Handler instance;
    return &instance;
};
} // namespace

QMenu *EditMenu::addToMenuBar(QMenuBar *pBar)
{
    auto pHandler = sHandler();
    auto pMenu = pBar->addMenu("&Edit");
    pMenu->addAction(&pHandler->mUndo);
    pMenu->addAction(&pHandler->mRedo);
    pMenu->addSeparator();
    pMenu->addAction(&pHandler->mCut);
    pMenu->addAction(&pHandler->mCopy);
    pMenu->addAction(&pHandler->mPaste);
    pMenu->addAction(&pHandler->mDelete);
    pMenu->addSeparator();
    pMenu->addAction(&pHandler->mSelectAll);
    pMenu->connect(pMenu, &QMenu::aboutToShow, pHandler, &Handler::onMenuAboutToShow);
    return pMenu;
}

// CopyWidget
EditMenu::CopyWidget::CopyWidget(QWidget *parent) : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
}

void EditMenu::CopyWidget::copy()
{
    doCopy(this);
}

void EditMenu::CopyWidget::doCopy(QWidget* pWidget)
{
    float ratio = 300.0 / 96.0; // suited for printing
    QPixmap data(pWidget->size().width() * ratio, pWidget->size().height() * ratio);
    data.setDevicePixelRatio(ratio);
    QPainter painter(&data);
    QFont font = painter.font();
    font.setHintingPreference(QFont::PreferNoHinting);
    pWidget->render(&painter);
    QImage image = data.toImage();
    float dpm = 300 * 2.54;
    image.setDotsPerMeterX(dpm);
    image.setDotsPerMeterY(dpm);
    QApplication::clipboard()->setImage(image);
}
