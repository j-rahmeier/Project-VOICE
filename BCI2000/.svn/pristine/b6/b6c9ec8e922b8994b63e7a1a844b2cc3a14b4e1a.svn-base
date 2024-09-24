////////////////////////////////////////////////////////////////////////////////
// $Id $
// Author: mellinger@neurotechcenter.org
// Description: A window to display BCI2000 filter messages in a GUI application.
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
#include "LogWindow.h"

#include "LogEntry.h"
#include "BCIStream.h"
#include "ParamList.h"

#include <QBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QIntValidator>
#include <QTextBrowser>

struct LogWindow::Private : QObject
{
    static LogWindow *spInstance;
    QTextBrowser *mpTextBrowser = nullptr;
    ParamList* mpParameters = nullptr;

    Private(QObject *parent, ParamList* pParams)
    : QObject(parent), mpParameters(pParams)
    {
    }
    void onVerbosityChanged(const QString& val)
    {
        mpParameters->Add("% int Verbosity= 0");
        mpParameters->ByPath("Verbosity").Value() = val.toUtf8();
    }
    void onDebugLevelChanged(const QString& val)
    {
        mpParameters->Add("% int DebugLevel= 0");
        mpParameters->ByPath("DebugLevel").Value() = val.toUtf8();
    }
};

LogWindow *LogWindow::Private::spInstance = nullptr;

LogWindow *LogWindow::instance()
{
    return Private::spInstance;
}

namespace {
    // remove unwanted elements from message context
    void FilterMessageContext(QStringList& context)
    {
        if (context.isEmpty())
            return;
        QString unwanted = "SubchainFilter::";
        if (!context.front().startsWith(unwanted))
            return;
        QString text = context.front().mid(unwanted.length());
        int pos = text.indexOf(": ");
        if (pos < 0)
            return;
        text = text.mid(pos + 2);
        context.front() = text;
    }
}

LogWindow::LogWindow(ParamList* pParams) : p(new Private(this, pParams))
{
    if (Private::spInstance == nullptr)
        Private::spInstance = this;

    resize(480, 320);
    setWindowTitle("Filter Log");
    auto pLayout = new QVBoxLayout;
    pLayout->setContentsMargins(2, 0, 2, 2);
    p->mpTextBrowser = new QTextBrowser;
    pLayout->addWidget(p->mpTextBrowser);

    auto pParmsLayout = new QFormLayout;
    pParmsLayout->setHorizontalSpacing(this->fontMetrics().averageCharWidth());
    pParmsLayout->setVerticalSpacing(this->fontMetrics().descent());
#if 0
    auto pVerbosity = new QLineEdit;
    pVerbosity->setValidator(new QIntValidator(-1, 10));
    pParmsLayout->addRow("Verbosity", pVerbosity);
    connect(pVerbosity, &QLineEdit::textEdited, p, &Private::onVerbosityChanged);
    pVerbosity->setText("0");
    p->onVerbosityChanged(pVerbosity->text());
#endif
    auto pDebugLevel = new QLineEdit;
    pDebugLevel->setValidator(new QIntValidator(-1, 10));
    pParmsLayout->addRow("Debug Level", pDebugLevel);
    connect(pDebugLevel, &QLineEdit::textEdited, p, &Private::onDebugLevelChanged);

#if BCIDEBUG
    pDebugLevel->setText("1");
#else
    pDebugLevel->setText("0");
#endif
    p->onDebugLevelChanged(pDebugLevel->text());

    pLayout->addItem(pParmsLayout);
    setLayout(pLayout);
}

LogWindow::~LogWindow()
{
    if (Private::spInstance == this)
        Private::spInstance = nullptr;
}

void LogWindow::addMessage(const std::string &msg, Kind kind)
{
    QString size, color;
    bool bold = false;
    switch (kind)
    {
    case plain:
        size = "3";
        break;
    case debug:
        size = "3";
        break;
    case warning:
        size = "4";
        bold = true;
        break;
    case error:
        size = "4";
        color = "red";
        bold = true;
        break;
    default:;
    }
    QString fontOpen, fontClose;
    if (!size.isEmpty() || !color.isEmpty())
    {
        fontOpen = "<font";
        fontClose = "</font>";
    }
    if (!size.isEmpty())
        fontOpen += " size='" + size + "'";
    if (!color.isEmpty())
        fontOpen += " color='" + color + "'";
    if (!fontOpen.isEmpty())
        fontOpen += ">";
    if (bold)
    {
        fontOpen += "<b>";
        fontClose = "</b>" + fontClose;
    }

    QString s = QString::fromUtf8(msg.c_str());
    QStringList parts, context;
    LogEntry::Parse(s, parts, context);
    FilterMessageContext(context);
    for (auto& part : context)
        part = part.toHtmlEscaped();
    for (auto& part : parts)
        part = part.toHtmlEscaped();
    s = context.join("") + fontOpen + parts.join("") + fontClose + "<br>";
    p->mpTextBrowser->moveCursor(QTextCursor::End);
    p->mpTextBrowser->insertHtml(s);
}

void LogWindow::clear()
{
    p->mpTextBrowser->clear();
}
