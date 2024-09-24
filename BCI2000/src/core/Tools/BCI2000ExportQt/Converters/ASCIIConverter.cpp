//////////////////////////////////////////////////////////////////////////////
// $Id: ASCIIConverter.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A BCIReader class for data file output in ASCII format.
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
#include "ASCIIConverter.h"
#include "BCIStream.h"
#include "GenericSignal.h"
#include "State.h"

#include <iomanip>
#include <string>

ASCIIConverter::ASCIIConverter(int precision) : mPrecision(precision)
{
}

ASCIIConverter::~ASCIIConverter()
{
    ExitOutput();
}

void ASCIIConverter::InitOutput(OutputInfo &inInfo)
{
    const std::string bciExtension = ".dat";
    std::string dataFileName = ".ascii", baseName(inInfo.name), lowerBaseName(baseName);
    for (std::string::iterator i = lowerBaseName.begin(); i != lowerBaseName.end(); ++i)
        *i = tolower(*i);
    int lengthDiff = static_cast<int>(baseName.length() - bciExtension.length());
    if ((lengthDiff > 0) && (lowerBaseName.substr(lengthDiff) == bciExtension))
        baseName = baseName.substr(0, lengthDiff);

    dataFileName = baseName + dataFileName;

    {
        std::ifstream input(dataFileName);
        if (input.is_open())
        {
            bcierr << "Data file \"" << dataFileName << "\" exists, will not be touched.\n\n"
                   << "Aborting conversion.";
            return;
        }
    }

    mDataFile.open(dataFileName, std::ios_base::out | std::ios_base::binary);
    if (!mDataFile.is_open())
    {
        bcierr << "Could not open \"" << dataFileName << "\" for writing.\n\n"
               << "Make sure you have write access to the folder containing "
               << "the input files.";
        return;
    }

    for (unsigned long channel = 0; channel < inInfo.numChannels; ++channel)
    {
        if ((*inInfo.channelNames)[channel].empty())
            mDataFile << " Ch" << channel + 1;
        else
            mDataFile << ' ' << (*inInfo.channelNames)[channel];
    }

    mStateValues.resize(inInfo.stateNames->size());
    for (unsigned long state = 0; state < inInfo.stateNames->size(); ++state)
    {
        mDataFile << ' ' << (*inInfo.stateNames)[state];
        mStateIndices[(*inInfo.stateNames)[state]] = state;
        mStateValues[state] = 0;
    }
    mDataFile << std::scientific << std::endl;
    if (mPrecision != defaultPrecision)
        mDataFile << std::setprecision(mPrecision);

    if (!mDataFile)
        bcierr << "Error writing to \"" << dataFileName << "\"";
}

void ASCIIConverter::ExitOutput()
{
    mDataFile.close();
}

void ASCIIConverter::OutputSignal(const GenericSignal &inSignal, long long /*inSamplePos*/)
{
    for (int sample = 0; sample < inSignal.Elements(); ++sample)
    {
        for (int channel = 0; channel < inSignal.Channels(); ++channel)
        {
            double value = inSignal(channel, sample);
            mDataFile << ' ' << value;
        }
        for (size_t state = 0; state < mStateValues.size(); ++state)
            mDataFile << ' ' << mStateValues[state];

        mDataFile << '\n';
    }
    if (!mDataFile)
        bcierr << "Error writing data file";
}

void ASCIIConverter::OutputStateChange(const State &inState, State::ValueType inValue, long long /*inSamplePos*/)
{
    mStateValues[mStateIndices[inState.Name()]] = inValue;
}
