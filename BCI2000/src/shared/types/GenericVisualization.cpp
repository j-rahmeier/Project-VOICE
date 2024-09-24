////////////////////////////////////////////////////////////////////////////////
// $Id: GenericVisualization.cpp 8039 2024-04-17 18:12:01Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: Classes that represent BCI2000 visualization messages.
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
#include "GenericVisualization.h"

#include "BCIStream.h"
#include "HierarchicalLabel.h"
#include "Label.h"
#include "MessageChannel.h"
#include "CoreConnection.h"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// Common to all visualization messages.
std::istream &VisBase::Unserialize(std::istream &is)
{
    int visID = static_cast<unsigned char>(is.get());
    if (visID == VisID::ExtendedFormat)
    {
        std::string s;
        std::getline(is, s, '\0');
        std::istringstream iss(s);
        if (!(iss >> mVisID))
            mVisID.Clear();
    }
    else
    {
        std::ostringstream oss;
        oss << visID;
        mVisID = oss.str();
    }
    OnUnserialize(is);
    return is;
}

std::ostream &VisBase::Serialize(std::ostream &os) const
{
    // We use the traditional message format if the visID can be represented
    // as a single byte number.
    bool oldFormat = false;
    int visID = ::atoi(mVisID.c_str());
    if (visID == VisID::ExtendedFormat)
        throw std_runtime_error << "Illegal VisID: " << mVisID;

    if (visID >= VisID::min && visID < VisID::ExtendedFormat)
    {
        std::ostringstream oss;
        oss << visID;
        if (oss.str() == mVisID)
            oldFormat = true;
    }
    if (oldFormat)
    {
        os.put(visID & 0xff);
    }
    else
    {
        os.put(static_cast<unsigned char>(VisID::ExtendedFormat));
        if (!mVisID.Empty()) // Maintain compatibility with older modules which don't expect an EncodedString here.
            os << mVisID;
        os.put('\0');
    }
    OnSerialize(os);
    return os;
}

// Config message.
void VisCfg::OnUnserialize(std::istream &is)
{
    mCfgID = is.get();
    std::getline(is, mCfgValue, '\0');
}

void VisCfg::OnSerialize(std::ostream &os) const
{
    os.put(mCfgID);
    os.write(mCfgValue.data(), mCfgValue.length());
    os.put('\0');
}

// Memo message.
void VisMemo::OnUnserialize(std::istream &is)
{
    std::getline(is, mMemo, '\0');
}

void VisMemo::OnSerialize(std::ostream &os) const
{
    os.write(mMemo.data(), mMemo.length());
    os.put('\0');
}

// Signal message.
void VisSignalConst::OnUnserialize(std::istream &is)
{
    is.setstate(std::ios::failbit);
}

void VisSignalConst::OnSerialize(std::ostream &os) const
{
    mrSignal.Serialize(os);
}

void VisSignal::OnUnserialize(std::istream &is)
{
    mSignal.Unserialize(is);
}

// Signal properties message.
void VisSignalProperties::OnUnserialize(std::istream &is)
{
    mSignalProperties.ExtractFrom(is);
}

void VisSignalProperties::OnSerialize(std::ostream &os) const
{
    mSignalProperties.InsertInto(os);
}

std::vector<VisCfg> VisSignalProperties::ToVisCfg() const
{
    // Translation of a VisSignalProperties message into a set of VisCfg messages.
    std::vector<VisCfg> result;
    const class SignalProperties &s = SignalProperties();

    if (!s.Name().empty())
        result.push_back(VisCfg(VisID(), CfgID::WindowTitle, s.Name()));

    std::string channelUnit = s.ChannelUnit().RawToPhysical(s.ChannelUnit().Offset() + 1);
    result.push_back(VisCfg(VisID(), CfgID::ChannelUnit, channelUnit));

    int numSamples = static_cast<int>(s.ElementUnit().RawMax() - s.ElementUnit().RawMin() + 1);
    if (s.ElementUnit().RawMin() < 0)
        numSamples = s.Elements();
    result.push_back(VisCfg(VisID(), CfgID::NumSamples, numSamples));
    if (numSamples > 0)
    {
        result.push_back(
            VisCfg(VisID(), CfgID::SampleUnit, s.ElementUnit().RawToPhysical(s.ElementUnit().Offset() + 1)));
        result.push_back(VisCfg(VisID(), CfgID::SampleOffset, s.ElementUnit().Offset()));
    }

#if 1
    // We will now send all units to the visualization end, but for backward compatibility, 
    // we send a single value for all channels as well.
    std::string valueUnit = s.ValueUnit().RawToPhysical(s.ValueUnit().Offset() + 1);
    result.push_back(VisCfg(VisID(), CfgID::ValueUnit, valueUnit));

    double rangeMin = s.ValueUnit().RawMin(), rangeMax = s.ValueUnit().RawMax();
    if (rangeMin == rangeMax)
    {
        result.push_back(VisCfg(VisID(), CfgID::MinValue, "none"));
        result.push_back(VisCfg(VisID(), CfgID::MaxValue, "none"));
    }
    else
    {
        result.push_back(VisCfg(VisID(), CfgID::MinValue, rangeMin));
        result.push_back(VisCfg(VisID(), CfgID::MaxValue, rangeMax));
    }
#endif
    
    ValueList<std::string> valueUnits, rangeMins, rangeMaxes;
    for (int ch = 0; ch < s.Channels(); ++ch)
    {
        std::string valueUnit = s.ValueUnit(ch).RawToPhysical(s.ValueUnit(ch).Offset() + 1);
        valueUnits.push_back(valueUnit);

        double rangeMin = s.ValueUnit().RawMin(), rangeMax = s.ValueUnit().RawMax();
        if (rangeMin == rangeMax)
        {
            rangeMins.push_back("none");
            rangeMaxes.push_back("none");
        }
        else
        {
            rangeMins.push_back(std::to_string(rangeMin));
            rangeMaxes.push_back(std::to_string(rangeMax));
        }
    }

    result.push_back(VisCfg(VisID(), CfgID::ValueUnits, valueUnits));
    result.push_back(VisCfg(VisID(), CfgID::MinValues, rangeMins));
    result.push_back(VisCfg(VisID(), CfgID::MaxValues, rangeMaxes));

    LabelList groupLabels, channelLabels;
    int channelGroupSize = 1;
    for (int i = 0; i < s.ChannelLabels().Size(); ++i)
    {
        std::istringstream iss(s.ChannelLabels()[i]);
        HierarchicalLabel label;
        iss >> label;
        if (label.size() == 2)
        {
            if (groupLabels.empty())
            {
                groupLabels.push_back(Label(0, label[0]));
            }
            else
            {
                if (label[0] == groupLabels.begin()->Text())
                    ++channelGroupSize;
                if (label[0] != groupLabels.rbegin()->Text())
                    groupLabels.push_back(Label(static_cast<int>(groupLabels.size()), label[0]));
            }
            channelLabels.push_back(Label(static_cast<int>(channelLabels.size()), label[1]));
        }
        else
        {
            channelLabels.push_back(Label(i, s.ChannelLabels()[i]));
        }
    }
    result.push_back(VisCfg(VisID(), CfgID::ChannelGroupSize, channelGroupSize));
    result.push_back(VisCfg(VisID(), CfgID::ChannelLabels, channelLabels));
    result.push_back(VisCfg(VisID(), CfgID::GroupLabels, groupLabels));

    return result;
}

// Bitmap message.
void VisBitmap::OnUnserialize(std::istream &is)
{
    mBitmap.Unserialize(is);
}

void VisBitmap::OnSerialize(std::ostream &os) const
{
    mBitmap.Serialize(os);
}

// VideoFrame message.
void VisVideoFrame::OnUnserialize(std::istream& is)
{
    mFrame.Unserialize(is);
}

void VisVideoFrame::OnSerialize(std::ostream& os) const
{
    mFrame.Serialize(os);
}

// GenericVisualization
static bci::MessageChannel *spOutputChannel = 0;

void GenericVisualization::SetOutputChannel(bci::MessageChannel *pChannel)
{
    spOutputChannel = pChannel;
}

bool GenericVisualization::OutputChannelIsLocal()
{
    CoreConnection* pConnection = dynamic_cast<CoreConnection*>(spOutputChannel);
    if (pConnection) {
        return pConnection->IsLocal();
    }
    return false;
}

GenericVisualization &GenericVisualization::SendCfgString(CfgID inCfgID, const std::string &inCfgString)
{
    return SendObject(VisCfg(mVisID, inCfgID, inCfgString));
}

GenericVisualization &GenericVisualization::Send(CfgID cfgID, const std::string &cfgValue)
{
    return SendCfgString(cfgID, cfgValue);
}

GenericVisualization &GenericVisualization::Send(const std::string &s)
{
    return SendObject(VisMemo(mVisID, s));
}

GenericVisualization &GenericVisualization::Send(const GenericSignal &s)
{
    return SendObject(VisSignalConst(mVisID, s));
}

GenericVisualization &GenericVisualization::Send(const SignalProperties &s)
{
    return SendObject(VisSignalProperties(mVisID, s));
}

GenericVisualization &GenericVisualization::Send(const BitmapImage &b)
{
    return SendObject(VisBitmap(mVisID, b));
}

GenericVisualization& GenericVisualization::Send(const VideoFrame& f)
{
    return SendObject(VisVideoFrame(mVisID, f));
}

template <typename T> GenericVisualization &GenericVisualization::SendObject(const T &inObject)
{
    if (spOutputChannel)
    {
        if (!spOutputChannel->Send(inObject))
            this->setstate(ios_base::badbit);
    }
    else
        bcierr << "No output stream specified.";
    return *this;
}

int GenericVisualization::VisStringbuf::sync()
{
    int result = std::stringbuf::sync();
    if (!mpParent->Send(str()))
        mpParent->setstate(std::ios::failbit);
    str("");
    return result;
}

void BitmapVisualization::SendReferenceFrame(const BitmapImage &b)
{
    // An empty image indicates that the next frame is a reference frame.
    Send(BitmapImage(0, 0));
    mImageBuffer = b;
    Send(mImageBuffer);
}

void BitmapVisualization::SendDifferenceFrame(const BitmapImage &b)
{
    if (b.Width() != mImageBuffer.Width() || b.Height() != mImageBuffer.Height())
    {
        SendReferenceFrame(b);
    }
    else
    {
        BitmapImage curImage = b;
        Send(curImage - mImageBuffer);
        mImageBuffer = curImage;
    }
}
