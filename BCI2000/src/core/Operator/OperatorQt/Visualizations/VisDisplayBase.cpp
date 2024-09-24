////////////////////////////////////////////////////////////////////////////////
// $Id: VisDisplayBase.cpp 7876 2024-02-29 19:23:31Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A base class for visualization displays.
//   Also handles message dispatching and storage of visualization properties.
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
#include "VisDisplayBase.h"

#include "CfgID.h"
#include "GenericSignal.h"
#include "Settings.h"
#include "VisDisplayWindow.h"
#include "Preferences.h"

#include <set>

VisDisplayBase::VisContainer VisDisplayBase::sVisuals;
VisDisplayBase::ConfigContainer VisDisplayBase::sVisconfigs;

////////////////////////////////////////////////////////////////////////////////
void VisDisplayBase::ConfigContainer::Save()
{
    Settings settings(Preferences::Instance()->mInstancePrefix);
    settings.beginGroup("Visualization");
    for (auto i = begin(); i != end(); ++i)
    {
        settings.beginGroup(i->first.c_str());
        std::set<CfgID> userDefinedCfgIDs;
        for (ConfigSettings::iterator j = i->second.begin(); j != i->second.end(); ++j)
            if (i->second.State(j->first) == UserDefined)
                userDefinedCfgIDs.insert(j->first);

        if (!userDefinedCfgIDs.empty())
        {
            // We add a Title entry to make it easier for the user to understand entries in the ini file.
            // Note that this entry is not named WindowTitle, so it is never restored from the ini file but
            // always determined by current BCI2000 module code.
            settings.setValue("Title", i->second[CfgID::WindowTitle].c_str());
            for (std::set<CfgID>::const_iterator j = userDefinedCfgIDs.begin(); j != userDefinedCfgIDs.end(); ++j)
                settings.setValue(std::string(*j).c_str(), i->second[*j].c_str());
        }
        settings.endGroup();
    }
}

void VisDisplayBase::ConfigContainer::Restore()
{
    Settings settings(Preferences::Instance()->mInstancePrefix);
    settings.beginGroup("Visualization");
    QStringList visIDs = settings.childGroups();
    for (const auto &visID : qAsConst(visIDs))
    {
        ConfigSettings &c = (*this)[visID.toUtf8().constData()];
        WithLock(c)
        {
            settings.beginGroup(visID);
            QStringList cfgIDs = settings.childKeys();
            for (const auto &cfgIDstring : qAsConst(cfgIDs))
            {
                CfgID cfgID(cfgIDstring.toUtf8().constData());
                if (cfgID != CfgID::None)
                {
                    std::string value = settings.value(cfgIDstring).toString().toUtf8().constData();
                    c.Put(cfgID, value, OnceUserDefined);
                }
            }
        }
        settings.endGroup();
    }
}

////////////////////////////////////////////////////////////////////////////////
void VisDisplayBase::Initialize()
{
    sVisconfigs.Restore();
}

void VisDisplayBase::Clear()
{
    sVisconfigs.Save();
    sVisuals.Clear();
}

VisDisplayBase::VisContainer &VisDisplayBase::Visuals()
{
    return sVisuals;
}

VisDisplayBase::ConfigContainer &VisDisplayBase::Visconfigs()
{
    return sVisconfigs;
}

VisDisplayBase::VisDisplayBase(const VisID &inVisID) : QWidget(NULL), mVisID(inVisID)
{
    if (inVisID.IsLayer())
    {
        if (!VisDisplayBase::Visuals()[inVisID.WindowID()])
            Create<VisDisplayWindow>(inVisID.WindowID().c_str());
        this->setParent(VisDisplayBase::Visuals()[inVisID.WindowID()]);
    }
    setAttribute(Qt::WA_QuitOnClose, false);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ContextMenu(QPoint)));
}

VisDisplayBase::~VisDisplayBase()
{
    sVisuals.erase(mVisID);
}

void VisDisplayBase::VisContainer::Clear()
{
    while (!empty())
    {
        VisDisplayBase *pVis = nullptr;
        iterator i = begin();
        while (begin() != end() && !begin()->second)
            erase(begin());
        if (begin() != end())
            pVis = begin()->second;
        delete pVis;
    }
}

void VisDisplayBase::SetConfig(ConfigSettings &inConfig)
{
    OnSetConfig(inConfig);
}

void VisDisplayBase::HandleSignal(int inChannels, const std::vector<float> &inSignal)
{
    if (inChannels > 0)
    {
        int elements = inSignal.size() / inChannels;
        GenericSignal signal(inChannels, elements);
        const float *pData = inSignal.data();
        for (int i = 0; i < inChannels; ++i)
            for (int j = 0; j < elements; ++j)
                signal(i, j) = *pData++;
        OnSignal(signal);
    }
}

void VisDisplayBase::HandleMemo(const QString &inText)
{
    OnMemo(inText);
}

void VisDisplayBase::HandleBitmap(const BitmapImage &inBitmap)
{
    OnBitmap(inBitmap);
}

void VisDisplayBase::HandleVideoFrame(const VideoFrame& inFrame)
{
    OnVideoFrame(inFrame);
}

void VisDisplayBase::HandleProperty(CfgID cfgId, const char *value, ConfigState state)
{
    OnProperty(cfgId, value, state);
}

void VisDisplayBase::ContextMenu(const QPoint &p)
{
    OnContextMenu(p);
}

void VisDisplayBase::OnSetConfig(ConfigSettings &inConfig)
{
    bool visible = true;
    inConfig.Get(CfgID::Visible, visible);
    this->setVisible(visible);
}

void VisDisplayBase::HandleSignal(const VisID &inVisID, int inChannels, const std::vector<float> &inSignal)
{
    VisDisplayBase *p = Visuals()[inVisID];
    if (p) {
        Assert(p);
        p->HandleSignal(inChannels, inSignal);
    }
}

void VisDisplayBase::HandleMemo(const VisID &inVisID, const std::string &inText)
{
    VisDisplayBase *p = Visuals()[inVisID];
    Assert(p);
    p->HandleMemo(inText.c_str());
}

void VisDisplayBase::HandleBitmap(const VisID &inVisID, const BitmapImage &inBitmap)
{
    VisDisplayBase *p = Visuals()[inVisID];
    Assert(p);
    p->HandleBitmap(inBitmap);
}

void VisDisplayBase::HandleVideoFrame(const VisID& inVisID, const VideoFrame& inVideoFrame)
{
    VisDisplayBase* p = Visuals()[inVisID];
    Assert(p);
    p->HandleVideoFrame(inVideoFrame);
}

void VisDisplayBase::HandleProperty(const VisID &inVisID, CfgID inCfgID, const std::string &inValue, int inState)
{
    ConfigSettings &c = Visconfigs()[inVisID];
    WithLock(c)
    {
        c.Put(inCfgID, inValue, ConfigState(inState));
        VisDisplayBase *p = Visuals()[inVisID];
        if (p)
            p->SetConfig(c);
    }
}

void VisDisplayBase::HandlePropertyReset(const VisID& inVisID, CfgID inCfgID)
{
    ConfigSettings& c = Visconfigs()[inVisID];
    WithLock(c)
    {
        c.Erase(inCfgID);
        VisDisplayBase* p = Visuals()[inVisID];
        if (p)
            p->SetConfig(c);
    }
}

bool VisDisplayBase::ConfigSettings::Get(CfgID id, std::string &s, ConfigState minState)
{
    const_iterator i = find(id);
    if (i == end())
        return false;
    if (State(id) < minState)
        return false;
    s = i->second;
    return true;
}

bool VisDisplayBase::ConfigSettings::Put(CfgID id, const std::string &s, ConfigState state)
{
    if (State(id) > state)
        return false;
    State(id) = state;
    (*this)[id] = s;
    return true;
}

void VisDisplayBase::ConfigSettings::Erase(CfgID id)
{
    mStates.erase(id);
    erase(id);
}
