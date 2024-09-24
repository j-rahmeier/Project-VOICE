//////////////////////////////////////////////////////////////////////
// $Id: Preferences.cpp 7746 2023-12-06 21:46:31Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A class to hold user preferences.
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
#include "Preferences.h"
#include "Debugging.h"
#include "Settings.h"
#include <QApplication>
#include <QStringList>

Preferences* Preferences::Instance()
{
    static Preferences instance;
    return &instance;
}

Preferences::Preferences() : mUserLevel(Beginner)
{
    for (int i = 0; i < Preferences::numScriptEvents; ++i)
        mCmdlineSpecified[i] = false;

    ReadFromCommandLine();
    ReadSettings();
}

Preferences::~Preferences()
{
    WriteSettings();
}

void Preferences::ReadFromCommandLine()
{
    const struct
    {
        const char *name;
        int event;
    } paramNames[] = {
#define PARAM(x) {"--" #x, x},
        PARAM(OnConnect)
        PARAM(OnExit)
        PARAM(OnSetConfig)
        PARAM(OnSuspend)
        PARAM(OnResume)
        PARAM(OnStart)
        PARAM(OnStartRun)
        PARAM(OnShutdown)
#undef PARAM
        {"--OnStopRun", OnSuspend},
    };
    int i = 1;
    auto args = qApp->arguments();
    while (i + 1 < args.size())
    {
        if (args.at(i) == "--InstancePrefix")
        {
            mInstancePrefix = args.at(++i);
            mInstancePrefix = mInstancePrefix.trimmed() + " ";
        }
        else for (size_t j = 0; j < sizeof(paramNames) / sizeof(*paramNames); ++j)
        {
            if (args.at(i).indexOf(paramNames[j].name) == 0)
            {
                mCmdlineSpecified[paramNames[j].event] = true;
                mScript[paramNames[j].event] = args.at(++i);
                break;
            }
        }
        ++i;
    }

}

void Preferences::ReadSettings()
{
    Settings settings(mInstancePrefix);

    QString dir = settings.value("Config/WorkingDirectory").toString();
    if (dir.isEmpty())
        dir = QApplication::applicationDirPath();
    mCurrentDir.setPath(dir);
    mUserLevel = settings.value("Config/Various/GlobalUserLevel", Advanced).toInt();

#define READ_SCRIPT(name)                                                                                              \
    if (!mCmdlineSpecified[name])                                                                                      \
        mScript[name] = settings.value("Config/Scripts/" #name, "").toString();
    READ_SCRIPT(AfterModulesConnected);
    READ_SCRIPT(OnExit);
    READ_SCRIPT(OnSetConfig);
    READ_SCRIPT(OnResume);
    READ_SCRIPT(OnSuspend);
    READ_SCRIPT(OnStart);
    READ_SCRIPT(OnStartRun);
    READ_SCRIPT(OnShutdown);

#define READ_BUTTON(number)                                                                                            \
    mButtons[number - 1].Name = settings.value("Config/Buttons/Button" #number "Name", "").toString();            \
    mButtons[number - 1].Cmd = settings.value("Config/Buttons/Button" #number "Cmd", "").toString();
    READ_BUTTON(1);
    READ_BUTTON(2);
    READ_BUTTON(3);
    READ_BUTTON(4);
}

void Preferences::WriteSettings()
{
    Settings settings(mInstancePrefix);

    // suppress Qt warning on empty path
    QString path;
    if (!mCurrentDir.path().isEmpty())
        path = mCurrentDir.canonicalPath();
    settings.setValue("Config/WorkingDirectory", path);
    settings.setValue("Config/Various/GlobalUserLevel", mUserLevel);

#define WRITE_SCRIPT(name)                                                                                             \
    if (!mCmdlineSpecified[name])                                                                                      \
        settings.setValue("Config/Scripts/" #name, mScript[name]);
    WRITE_SCRIPT(AfterModulesConnected);
    WRITE_SCRIPT(OnExit);
    WRITE_SCRIPT(OnSetConfig);
    WRITE_SCRIPT(OnResume);
    WRITE_SCRIPT(OnSuspend);
    WRITE_SCRIPT(OnStart);
    WRITE_SCRIPT(OnStartRun);
    WRITE_SCRIPT(OnShutdown);

#define WRITE_BUTTON(number)                                                                                           \
    settings.setValue("Config/Buttons/Button" #number "Name", mButtons[number - 1].Name);                         \
    settings.setValue("Config/Buttons/Button" #number "Cmd", mButtons[number - 1].Cmd);
    WRITE_BUTTON(1);
    WRITE_BUTTON(2);
    WRITE_BUTTON(3);
    WRITE_BUTTON(4);
}
