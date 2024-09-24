//////////////////////////////////////////////////////////////////////
// $Id: LogEntry.h 7464 2023-06-30 15:04:08Z mellinger $
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
#ifndef LOG_ENTRY_H
#define LOG_ENTRY_H

#include <QString>
#include <QStringList>

namespace LogEntry
{
void Parse(const QString &inEntry, QStringList &outParts, QStringList &outContext);
QString Format(QStringList &ioParts, const QStringList &inContext);
} // namespace LogEntry

#endif // LOG_ENTRY_H
