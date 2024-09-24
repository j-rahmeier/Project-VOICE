//////////////////////////////////////////////////////////////////////////////
// $Id: ASCIIConverter.h 7195 2023-02-07 18:31:03Z mellinger $
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
#ifndef ASCII_CONVERTER_H
#define ASCII_CONVERTER_H

#include "../BCIReader.h"
#include <fstream>
#include <map>
#include <string>
#include <vector>

std::string ExtractFileName(const std::string &);

class ASCIIConverter : public BCIReader
{
    enum
    {
        defaultPrecision = -1,
        highPrecision = 20,
    };

  public:
    ASCIIConverter(int precision = defaultPrecision);
    virtual ~ASCIIConverter();

    static BCIReader *CreateInstance()
    {
        return new ASCIIConverter;
    }
    static BCIReader *CreateInstanceHighPrecision()
    {
        return new ASCIIConverter(highPrecision);
    }

  protected:
    void InitOutput(OutputInfo &) override;
    void ExitOutput() override;
    void OutputSignal(const GenericSignal &, long long inSamplePos) override;
    void OutputStateChange(const State &, State::ValueType, long long inSamplePos) override;
    void OutputStateRange(const State &, State::ValueType, long long inBeginPos, long long inEndPos) override
    {
    }

  private:
    int mPrecision;
    std::ofstream mDataFile;
    std::vector<State::ValueType> mStateValues;
    std::map<std::string, size_t> mStateIndices;
};

#endif // ASCII_CONVERTER_H
