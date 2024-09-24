////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000OutputFormat.cpp 7437 2023-06-22 17:09:12Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An output class for the BCI2000 dat format.
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
#include "BCI2000OutputFormat.h"

#include "BCIStream.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

void BCI2000OutputFormat::Publish() const
{
}

void BCI2000OutputFormat::Preflight(const SignalProperties &inProperties, const StateVector & inStatevector) const
{
    GenericOutputFormat::Preflight(inProperties, inStatevector);

    Parameter("/SourceCh");

    switch (inProperties.Type())
    {
    case SignalType::int16:
    case SignalType::int32:
    case SignalType::float32:
        /* These types are OK */
        break;

    default:
        bcierr << inProperties.Type().Name() << " data type unsupported for BCI2000 files";
    }
}

void BCI2000OutputFormat::Initialize(const SignalProperties &inProperties, const StateVector &inStatevector)
{
    mInputProperties = inProperties;
    mStatevectorLength = inStatevector.Length();
}

void BCI2000OutputFormat::BeginFile(std::ostream &os, const ParamList & parameters, const StateList & states)
{
    // We write 16 bit data in the old format to maintain backward compatibility.
    bool useOldFormat = (mInputProperties.Type() == SignalType::int16);

    // Write the header.
    //
    // Because the header contains its own length in ASCII format, it is a bit
    // tricky to get this right if we don't want to imply a fixed width for the
    // HeaderLen field.
    std::ostringstream header;
    header << " "
           << "SourceCh= " << mInputProperties.Channels() << " "
           << "StatevectorLen= " << mStatevectorLength;
    if (!useOldFormat)
        header << " "
               << "DataFormat= " << mInputProperties.Type().Name();
    header << "\r\n"
           << "[ State Vector Definition ] \r\n";
    states.Serialize(header);
    header << "[ Parameter Definition ] \r\n";
    parameters.Serialize(header);
    header << "\r\n";

    std::string headerBegin;
    if (!useOldFormat)
        headerBegin = "BCI2000V= 1.1 ";
    headerBegin += "HeaderLen= ";
    size_t fieldLength = 5; // Follow the old scheme
                            // (5 characters for the header length field),
                            // but allow for a longer HeaderLen field
                            // if necessary.
    std::ostringstream headerLengthField;
    do
    { // This trial-and-error scheme is invariant against changes in number
        // formatting and character set (unicode, hex, ...).
        size_t headerLength = headerBegin.length() + fieldLength + header.str().length();
        headerLengthField.str("");
        headerLengthField << headerBegin << std::setfill(' ') << std::setw(fieldLength) << headerLength;
    } while (headerLengthField && (headerLengthField.str().length() - headerBegin.length() != fieldLength++));

    os.write(headerLengthField.str().data(), headerLengthField.str().size());
    os.write(header.str().data(), header.str().size());
}

void BCI2000OutputFormat::EndFile(std::ostream &)
{
}

void BCI2000OutputFormat::Write(std::ostream &os, const GenericSignal &inSignal, const StateVector &inStatevector)
{
    switch (mInputProperties.Type())
    {
    case SignalType::int16:
    case SignalType::float32:
    case SignalType::int32:
        // Note that the order of Elements and Channels differs from the one in the
        // socket protocol.
        for (int j = 0; j < inSignal.Elements(); ++j)
        {
            for (int i = 0; i < inSignal.Channels(); ++i)
                inSignal.WriteValueBinary(os, i, j);
            os.write(inStatevector.Data(std::min(j, inStatevector.Samples() - 1)), inStatevector.Length());
        }
        break;

    default:
        bcierr << "Unsupported signal data type";
    }
}
