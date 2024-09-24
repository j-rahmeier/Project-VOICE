//////////////////////////////////////////////////////////////////////
// $Id: SysLog.cpp 7849 2024-02-21 18:20:24Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: The Operator module's log window.
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
#include "SysLog.h"
#include "LogEntry.h"
#include "MainWindow.h"
#include "OperatorUtils.h"
#include "Preferences.h"

#include <QCloseEvent>
#include <QDateTime>
#include <QLayout>
#include <QScrollBar>

#if __APPLE__
  const float cFontSize = 12;
#else
  const float cFontSize = 8;
#endif

static SysLog* spInstance = nullptr;

SysLog* SysLog::Instance()
{
    return spInstance;
}

SysLog::SysLog(MainWindow *inParent)
    : QDialog(inParent, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint), mpParent(inParent),
      mpLog(new QTextBrowser), mWasVisible(false), mAttachToParent(true), mEmpty(true), mState(normal), mErrorsDisplayedSoFar(true),
      mURLMatcher("://")
{
    spInstance = this;
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->setContentsMargins(0, 0, 0, 0);
    pLayout->addWidget(mpLog);
    this->setLayout(pLayout);
    this->setWindowTitle(Preferences::Instance()->mInstancePrefix + "System Log");
    mAttachToParent = !OperatorUtils::RestoreWidget(this);

    mpLog->setOpenExternalLinks(true);
    mDefaultFormat = mpLog->currentCharFormat();
    mDefaultFormat.setFontPointSize(cFontSize); // fontPointSize is 0 by default
}

SysLog::~SysLog()
{
    if (mWasVisible)
        OperatorUtils::SaveWidget(this);
    spInstance = nullptr;
}

void SysLog::Show(bool inForce)
{

    if (inForce || (parentWidget() && parentWidget()->isVisible()))
        this->show();
    if (inForce && parentWidget())
        parentWidget()->show();
}

void SysLog::AddEntry(const QString &inText, int inMode, bool inVerbatim)
{
    QStringList parts, context;
    if (inVerbatim)
    {
        parts.push_back("");
        parts.push_back("");
        parts.push_back(inText);
        parts.push_back("");
    }
    else
        LogEntry::Parse(inText, parts, context);

    QMutexLocker lock(&mCritsec);
    QTextCharFormat format1 = mDefaultFormat, format2 = format1;

    switch (inMode)
    {
    case logEntryWarning:
        format1.setFontPointSize(mDefaultFormat.fontPointSize() * 6 / 5);
        format1.setFontWeight(QFont::Bold);
        format2 = format1;
        context.push_back("[warning]");
        mErrorsDisplayedSoFar = isVisible();
        break;

    case logEntryError:
        format1.setFontPointSize(mDefaultFormat.fontPointSize() * 6 / 5);
        format1.setForeground(QBrush("darkred"));
        format1.setFontWeight(QFont::Bold);
        format2 = format1;
        format2.setForeground(mDefaultFormat.foreground());
        context.push_back("[error]");
        mErrorsDisplayedSoFar = isVisible() && !mpParent->Terminating();
        break;

    case logEntryNormal:
    default:
        break;
    }

    bool moveToEnd = true;
    QScrollBar *pScrollBar = mpLog->verticalScrollBar();
    if (pScrollBar && pScrollBar->isVisible() && pScrollBar->isEnabled())
        moveToEnd = (pScrollBar->value() + pScrollBar->singleStep() > pScrollBar->maximum());

    QString text;
    if (!inVerbatim)
        text = LogEntry::Format(parts, context);
    QTextCursor c = mpLog->textCursor();
    c.movePosition(QTextCursor::End);
    c.insertText((mEmpty ? "" : "\n") + parts[0], mDefaultFormat);
    c.insertText(parts[1], mDefaultFormat);
    c.insertText(parts[2], format1);
    c.insertText(parts[3], format2);

    int begin = 0, end = begin;
    while (MatchURL(text, begin, end))
    {
        c.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        c.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, text.length() - begin);
        c.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, end - begin);
        QTextCharFormat f;
        f.setFontUnderline(true);
        f.setForeground(Qt::blue);
        f.setAnchor(true);
        f.setAnchorHref(QUrl::fromUserInput(c.selectedText()).toString());
        c.mergeCharFormat(f);
        begin = end;
    }
    switch (inMode)
    {
    case logEntryError:
        moveToEnd = true;
        Show(true);
        break;
    case logEntryWarning:
        Show(false);
        break;
    default:
        break;
    }
    if (moveToEnd)
    {
        mpLog->moveCursor(QTextCursor::End);
        mpLog->ensureCursorVisible();
    }
    mEmpty = false;
}

bool SysLog::MatchURL(const QString &s, int &begin, int &end)
{
    begin = mURLMatcher.indexIn(s, begin);
    if (--begin < 0)
        return false;
    end = begin + mURLMatcher.pattern().length();
    while (begin > 0 && s[begin].isLetter())
        --begin;
    while (end < s.length() && !s[end].isSpace())
        ++end;
    return true;
}

void SysLog::showEvent(QShowEvent *)
{
    mWasVisible = true;
    QWidget *pParent = this->parentWidget();
    if (mAttachToParent && pParent)
    {
        QSize s = pParent->size();
        s.setHeight(2 * s.height());
        this->resize(s);
        QPoint p = pParent->frameGeometry().bottomLeft();
        this->move(p);
        mAttachToParent = true;
    }
}

void SysLog::moveEvent(QMoveEvent *)
{
    if (isVisible())
        mAttachToParent = false;
}

void SysLog::paintEvent(QPaintEvent *ev)
{
    mErrorsDisplayedSoFar = true;
    QDialog::paintEvent(ev);
}

void SysLog::closeEvent(QCloseEvent *ev)
{
    // The user must close the syslog manually if there are errors/warnings.
    if (mState == normal && HasUndisplayedErrors())
    {
        mState = closeDenied;
        ev->ignore();
        parentWidget()->hide();
    }
    else if (mState == closeDenied)
    {
        ev->accept();
        parentWidget()->close();
    }
    else
    {
        hide();
        ev->ignore();
    }
}
