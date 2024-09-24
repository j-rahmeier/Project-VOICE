////////////////////////////////////////////////////////////////////////////////
// $Id: EDFOutputFormat.cpp 8074 2024-04-25 11:53:33Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Output into an EDF data file.
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
#include "EDFOutputFormat.h"

#include "BCIStream.h"
#include "GDF.h"

using namespace GDF;

EDFOutputFormat::EDFOutputFormat()
{
}

EDFOutputFormat::~EDFOutputFormat()
{
}

void EDFOutputFormat::Publish() const
{
    EDFOutputBase::Publish();

    BEGIN_PARAMETER_DEFINITIONS
    "Storage string LabID= % % % % "
    "// laboratory identification",
        "Storage string TechnicianID= % % % % "
        "// technician identification",
        END_PARAMETER_DEFINITIONS
}

void EDFOutputFormat::Preflight(const SignalProperties &inProperties, const StateVector &inStatevector) const
{
    EDFOutputBase::Preflight(inProperties, inStatevector);
    if (inProperties.Type() != SignalType::int16)
        bcierr << "Signal data type must be int16 for EDF files";

    Parameter("/SampleBlockSize");
    Parameter("/SamplingRate");
    Parameter("SubjectYearOfBirth");
    Parameter("EquipmentID");
    Parameter("TechnicianID");
    Parameter("LabID");
    Parameter("SubjectName");
}

void EDFOutputFormat::Initialize(const SignalProperties &inProperties, const StateVector &inStatevector)
{
    EDFOutputBase::Initialize(inProperties, inStatevector);
    // Adapt marker channels to EDF conventions.
    for (size_t i = inProperties.Channels(); i < Channels().size(); ++i)
    {
        Channels()[i].DigitalMinimum += 1;
        Channels()[i].DigitalMaximum -= 1;
        Channels()[i].PhysicalMinimum = Channels()[i].DigitalMinimum;
        Channels()[i].PhysicalMaximum = Channels()[i].DigitalMaximum;
    }
}

void EDFOutputFormat::BeginFile(std::ostream &os, const ParamList &params, const StateList &states)
{
    EDFOutputBase::BeginFile(os, params, states);

    EDFHeader h;
    h.Version = "0";

    time_t now;
    struct tm *time;
    AcquireTime(now, time);

    std::ostringstream year;
    year << std::setw(4) << std::setfill('0') << time->tm_year + 1900;
    std::ostringstream startdate;
    startdate << std::setw(2) << std::setfill('0') << time->tm_mday << '.' << std::setw(2) << std::setfill('0')
              << time->tm_mon + 1 << '.' << year.str().substr(2) << std::setw(2) << std::setfill('0') << time->tm_hour
              << '.' << std::setw(2) << std::setfill('0') << time->tm_min << '.' << std::setw(2) << std::setfill('0')
              << time->tm_sec;
    h.Startdate = startdate.str();

    std::ostringstream patient;
    patient << GDF::EncodedString(params("SubjectName")) << " X ";
    switch (int(params("SubjectSex")))
    {
    case GDF::unspecified:
        patient << "X";
        break;
    case GDF::male:
        patient << "M";
        break;
    case GDF::female:
        patient << "F";
        break;
    }
    patient << " XX-XXX-" << GDF::EncodedString(params("SubjectYearOfBirth"));
    h.Patient = patient.str();

    std::ostringstream recording;
    recording << "Startdate " << GDF::DateTimeToString(now) << ' ' << GDF::EncodedString(params("LabID")) << ' '
              << GDF::EncodedString(params("TechnicianID")) << ' ' << GDF::EncodedString(params("EquipmentID"));
    h.Recording = recording.str();

    h.Channels = Channels();
    h.BlockDuration = params("SampleBlockSize") / params("SamplingRate");
    h.Serialize(os);
}

void EDFOutputFormat::EndFile(std::ostream &os)
{
    const int NumRecFieldPos = 236;
    if (os.seekp(NumRecFieldPos))
        PutField<Str<8>>(os, NumRecords());
    EDFOutputBase::EndFile(os);
}

void EDFOutputFormat::Write(std::ostream &os, const GenericSignal &inSignal, const StateVector &inStatevector)
{
    EDFOutputBase::Write(os, inSignal, inStatevector);
}
