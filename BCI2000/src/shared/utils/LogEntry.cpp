//////////////////////////////////////////////////////////////////////
// $Id: LogEntry.cpp 7464 2023-06-30 15:04:08Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: Parsing and formatting of Operator log messages.
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
#include "LogEntry.h"

#include <QDateTime>
#include <QString>
#include <QStringList>

namespace LogEntry
{

void Parse(const QString &inEntry, QStringList &outParts, QStringList &outContext)
{
    outParts.clear();
    outContext.clear();

    QString text = inEntry, text2;
    int idx = -1;
    while (text.size() && text[0] == '[' && (idx = text.indexOf(']')) > 0)
    {
        outContext.push_back(text.left(idx + 1));
        text = text.mid(idx + 1);
    }

    idx = text.indexOf('\n');
    if (idx > 0)
    {
        text2 = text.mid(idx + 1);
        text = text.left(idx + 1);
    }

    QString context;
    while (text.size() && (idx = text.indexOf("::")) >= 0)
    {
        while (idx < text.size() && !text[idx].isSpace())
            ++idx;
        context += text.left(idx) + " ";
        text = text.mid(idx + 1);
    }
    if (!context.isEmpty())
        outContext.push_front(context);

    outParts.push_back(text);
    outParts.push_back(text2);
}

QString Format(QStringList &ioParts, const QStringList &inContext)
{
    QString context1, context2;
    for (int i = 0; i < inContext.size(); ++i)
    {
        const QString &s = inContext[i];
        if (!s.isEmpty() && s[0] == '[')
            context1 += s.mid(1, s.length() - 2).trimmed() + " ";
        else
            context2 += s.trimmed() + " ";
    }
    QString context = context1.trimmed();
    context2 = context2.trimmed();
    if (!context.isEmpty() && !context2.isEmpty())
        context += ", ";
    context += context2;
    context = context.trimmed();
    if (!context.isEmpty())
    {
        if (!context[0].isUpper())
            context[0] = context[0].toUpper();
        if (context[context.length() - 1].isPunct())
            context.truncate(context.length() - 1);
        context = context.trimmed() + ":\n";
    }
    ioParts.push_front(context);
    ioParts.push_front(QDateTime::currentDateTime().toString(Qt::ISODateWithMs) + " - ");

    QString &text1 = ioParts[2], &text2 = ioParts[3];
    if (text2.trimmed().isEmpty())
    {
        text2 = "";
        text1 = text1.trimmed();
        static const QString punct = ".!?";
        if (!text1.isEmpty() && punct.indexOf(text1[text1.length() - 1]) < 0)
            text1 += '.';
    }
    return ioParts.join("");
}

} // namespace LogEntry
