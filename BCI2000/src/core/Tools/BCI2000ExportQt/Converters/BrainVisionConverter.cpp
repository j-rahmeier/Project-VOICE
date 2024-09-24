//////////////////////////////////////////////////////////////////////////////
// $Id: BrainVisionConverter.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A BCIReader class for data file output in
//   BrainVision GDR format.
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
#include "BrainVisionConverter.h"

#include "BCIStream.h"
#include "FileUtils.h"
#include "GenericSignal.h"
#include "ProcessUtils.h"
#include "State.h"

#include <cstdlib>
#include <ctime>
#include <string>

std::string ExtractFileName(const std::string &inPath)
{
    size_t namePos = inPath.find_last_of("/\\:");
    if (namePos == std::string::npos)
        namePos = 0;
    else
        ++namePos;
    return inPath.substr(namePos);
}

BrainVisionGDRConverter::BrainVisionGDRConverter() : mCurMarker(0)
{
}

BrainVisionGDRConverter::~BrainVisionGDRConverter()
{
    ExitOutput();
}

void BrainVisionGDRConverter::InitOutput(OutputInfo &inInfo)
{
    mCurMarker = 0;

    const std::string bciExtension = ".dat";
    std::string headerFileName = ".vhdr", dataFileName = ".vraw", markerFileName = ".vmrk", baseName(inInfo.name),
                lowerBaseName(baseName);
    for (std::string::iterator i = lowerBaseName.begin(); i != lowerBaseName.end(); ++i)
        *i = tolower(*i);
    int lengthDiff = static_cast<int>(baseName.length() - bciExtension.length());
    if ((lengthDiff > 0) && (lowerBaseName.substr(lengthDiff) == bciExtension))
        baseName = baseName.substr(0, lengthDiff);

    headerFileName = baseName + headerFileName;
    dataFileName = baseName + dataFileName;
    markerFileName = baseName + markerFileName;

    mHeaderFile.open(headerFileName);
    mMarkerFile.open(markerFileName);
    mDataFile.open(dataFileName, std::ios_base::out | std::ios_base::binary);
    bool success = mHeaderFile.is_open() && mMarkerFile.is_open() && mDataFile.is_open();
    if (!success)
    {
        bcierr << "Could not open " << baseName << " for writing.\n\n"
               << "Make sure you have write access to the folder containing "
               << "the BCI2000 files.";
        return;
    }

    time_t now;
    time(&now);
    std::string userinfo = ProcessUtils::UserName() + "@" + ProcessUtils::HostName();

    mHeaderFile << "Brain Vision Data Exchange Header File Version 1.0" << std::endl
                << "; Created " << ctime(&now) << "; by " << userinfo << std::endl
                << "; from " << inInfo.name << std::endl
                << "; using " << FileUtils::ApplicationTitle() << std::endl
                << std::endl
                << "[Common Infos]" << std::endl
                << "DataFile=" << ExtractFileName(dataFileName) << std::endl
                << "MarkerFile=" << ExtractFileName(markerFileName) << std::endl
                << "DataFormat=BINARY" << std::endl
                << "DataOrientation=MULTIPLEXED" << std::endl
                << "DataType=TIMEDOMAIN" << std::endl
                << "NumberOfChannels=" << inInfo.numChannels << std::endl
                << "SamplingInterval=" << 1e6 / inInfo.samplingRate << std::endl
                << std::endl
                << "[Binary Infos]" << std::endl
                << "BinaryFormat=IEEE_FLOAT_32" << std::endl
                << std::endl
                << "[Channel Infos]" << std::endl;

    for (unsigned long channel = 0; channel < inInfo.numChannels; ++channel)
    {
        mHeaderFile << "Ch" << channel + 1 << "=";
        mHeaderFile << (*inInfo.channelNames)[channel];
        mHeaderFile << "," /* reference Channel name */
                    << ",1" << std::endl;
    }

    mMarkerFile << "Brain Vision Data Exchange Marker File Version 1.0" << std::endl
                << "; Created " << ctime(&now) << "; by " << userinfo << std::endl
                << "; from " << inInfo.name << std::endl
                << "; using " << FileUtils::ApplicationTitle() << std::endl
                << std::endl
                << "[Common Infos]" << std::endl
                << "DataFile=" << ExtractFileName(dataFileName) << std::endl
                << std::endl
                << "[Marker Infos]" << std::endl;

    if (!mHeaderFile)
        bcierr << "Error writing " << headerFileName;
    if (!mMarkerFile)
        bcierr << "Error writing " << markerFileName;
}

void BrainVisionGDRConverter::ExitOutput()
{
    mDataFile.close();
    mHeaderFile.close();
    mMarkerFile.close();
}

void BrainVisionGDRConverter::OutputSignal(const GenericSignal &inSignal, long long /*inSamplePos*/)
{
    for (int sample = 0; sample < inSignal.Elements(); ++sample)
        for (int channel = 0; channel < inSignal.Channels(); ++channel)
        {
            float value = static_cast<float>(inSignal(channel, sample));
            mDataFile.write(reinterpret_cast<const char *>(&value), sizeof(value));
        }

    if (!mDataFile)
        bcierr << "Error writing data file";
}

void BrainVisionGDRConverter::OutputStateChange(const State & /*inState*/, State::ValueType /*inValue*/,
                                                long long /*inSamplePos*/)
{
#if 0
  ++mCurMarker;
  mMarkerFile << "Mk" << mCurMarker << "="
              << "BCIStateChange,";
  mMarkerFile << inState.Name()
              << " " << inValue << ",";
  mMarkerFile << inSamplePos << ","
              << 1 << ","
              << 0 << '\n';
#endif
}

void BrainVisionGDRConverter::OutputStateRange(const State &inState, State::ValueType inValue, long long inBeginPos,
                                               long long inEndPos)
{
    if ((inEndPos < inBeginPos) || (inBeginPos < 0))
    {
        bcierr << "Bad file format";
        return;
    }

    ++mCurMarker;
    mMarkerFile << "Mk" << mCurMarker << "="
                << "BCIState,";

    if (inState.Length() > 1)
        mMarkerFile << inState.Name() << " " << inValue << ",";
    else
        mMarkerFile << inState.Name() << ",";

    mMarkerFile << inBeginPos << "," << inEndPos - inBeginPos << "," << 0 << '\n';
}
