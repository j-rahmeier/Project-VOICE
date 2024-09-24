////////////////////////////////////////////////////////////////////////////////
// $Id: Settings.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A descendant of the Qt QSettings class with a default
//   constructor that reads/writes from/to an ini file in the application
//   directory.
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
#include "Settings.h"
#include "FileUtils.h"
#include <QApplication>

namespace
{

QString FilePath(const QString& prefix = "")
{
    // If the executable is stored at a writable location,
    // put the ini file right besides the executable.
    std::string app = FileUtils::ExtractBase(FileUtils::ExecutablePath());
    std::string file = app + ".ini";
    std::string dir = FileUtils::InstallationDirectory();
    QString qfile = prefix + QString::fromLocal8Bit(file.c_str());
    if (FileUtils::IsWritableDirectory(dir))
        return QString::fromLocal8Bit(dir.c_str()) + qfile;
    // Otherwise, use the default location for ini files.
    std::string defaultFile = QSettings(
        QSettings::IniFormat, QSettings::UserScope,
        QApplication::organizationName(), QString::fromLocal8Bit(app.c_str())
    ).fileName().toLocal8Bit().constData();
    dir = FileUtils::ExtractDirectory(defaultFile);
    return QString::fromLocal8Bit(dir.c_str()) + qfile;
}

} // namespace

Settings::Settings(const QString& prefix) : QSettings(FilePath(prefix), QSettings::IniFormat)
{
}
