////////////////////////////////////////////////////////////////////
// $Id: bci_dat2stream.cpp 7541 2023-08-21 15:01:42Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: See the ToolInfo definition below.
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
////////////////////////////////////////////////////////////////////
#include "bci_tool.h"

#include "BCI2000FileReader.h"
#include "FdObject.h"
#include "GenericSignal.h"
#include "MeasurementUnits.h"
#include "MessageChannel.h"
#include "Param.h"
#include "ParamList.h"
#include "State.h"
#include "StateList.h"
#include "StateVector.h"

#include <cassert>
#include <fstream>

std::string ToolInfo[] = {"bci_dat2stream",
                          "Convert a BCI2000 data file into a BCI2000 stream.",
                          "Reads a BCI2000 data file (*.dat) compliant stream from "
                          "standard input and writes it to the standard output as a BCI2000 "
                          "compliant binary stream.",
                          "binary",
                          "-t,       --transmit={spd}      Select States, Parameters, and/or",
                          "                                Data for transmission",
                          "-r,       --raw                 Transmit uncalibrated data",
                          "-p<file>, --parameters=<file>   Incorporate parameters from named file",
                          "-s<time>, --start=<time>        Start at a given offset within the file",
                          "-d<time>, --duration=<time>     Transmit only a limited amount of signal",
                          " ",
                          "Amounts of <time> are expressed in SampleBlocks or (if the unit is",
                          "explicitly appended) as a number of seconds or milliseconds that",
                          "corresponds to a whole number of SampleBlocks.",
                          ""};

ToolResult ToolInit()
{
    return noError;
}

ToolResult ToolMain(Streambuf &inbuf, Streambuf &outbuf, OptionSet &options)
{
    ToolResult result = noError;
    std::string transmissionList = options.getopt("-t|-T|--transmit", "spd");
    bool transmitStates = (transmissionList.find_first_of("sS") != std::string::npos),
         transmitParameters = (transmissionList.find_first_of("pP") != std::string::npos),
         transmitData = (transmissionList.find_first_of("dD") != std::string::npos),
         calibrateData = !options.findopt("-r|-R|--raw");
    std::string paramFileName = options.getopt("-p|-P|--parameters", "");
    std::string offsetString = options.getopt("-s|-S|--start", "");
    std::string durationString = options.getopt("-d|-D|--duration", "");

    FdObject* fdobj = dynamic_cast<FdObject*>(inbuf.Input());
    Assert(fdobj);
    BCI2000FileReader file(fdobj->Fd());

    int sourceCh = file.SourceCh();
    StateList states = *(file.States());
    MessageChannel output(outbuf);
    if (transmitStates)
    { // Transmit states ordered by name, i.e. independently of their order in the file.
        std::vector<std::string> stateNames;
        for (const auto &state : states)
            stateNames.push_back(state.Name());
        std::sort(stateNames.begin(), stateNames.end(), State::NameCmp());
        for (const auto &name : stateNames)
            output.Send(states.ByName(name));
    }

    ParamList parameters = *(file.Parameters());
    if (transmitParameters)
    {
        for (const Param &param : parameters)
            output.Send(param);
    }

    if (!paramFileName.empty())
    {
        std::ifstream is(paramFileName, std::ifstream::in);
        std::string line;
        while (is.good() && std::getline(is, line) && line.length() > 1)
        {
            std::istringstream iss(line);
            Param param;
            if (iss >> param)
                parameters.ByPath(param.Name()) = param;
            if (transmitParameters)
                output.Send(param);
        }
    }

    MeasurementUnits::Initialize(parameters);
    double offset = (offsetString.size() ? MeasurementUnits::TimeInSampleBlocks(offsetString) : 0.0);
    double duration = (durationString.size() ? MeasurementUnits::TimeInSampleBlocks(durationString) : -1.0);

    SignalProperties inputProperties = file.SignalProperties();
    int sampleBlockSize = inputProperties.Elements();
    if (transmitData || transmitStates)
    {
        StateVector statevector(states, sampleBlockSize + 1);
        if (statevector.Length() != file.StateVectorLength())
        {
            std::cerr << "Statevector's length (" << statevector.Length() << ")"
                      << " differs from StateVectorLen field (" << file.StateVectorLength()
                      << ")" << std::endl;
            return illegalInput;
        }
        if (calibrateData && parameters.Exists("SamplingRate"))
        {
            double samplingRate = PhysicalUnit().SetOffset(0.0).SetGain(1.0).SetSymbol("Hz").PhysicalToRaw(
                parameters.ByPath("SamplingRate").Value());
            inputProperties.ElementUnit().SetOffset(0).SetGain(1.0 / samplingRate).SetSymbol("s");
        }
        if (parameters.Exists("ChannelNames") && parameters.ByPath("ChannelNames").NumValues() > 0)
        {
            LabelIndex &outputLabels = inputProperties.ChannelLabels();
            for (int i = 0; i < std::min(inputProperties.Channels(), parameters.ByPath("ChannelNames").NumValues());
                    ++i)
                outputLabels[i] = parameters.ByPath("ChannelNames").Value(i).c_str();
        }
        SignalProperties outputProperties(inputProperties);
        if (transmitData)
        {
            if (calibrateData)
            {
                PhysicalUnit unit;
                unit.SetGainWithSymbol("1muV");
                for (int i = 0; i < outputProperties.Channels(); ++i)
                    outputProperties.ValueUnit(i) = unit;
                outputProperties.SetType(SignalType::float32);
            }
            output.Send(outputProperties);
        }

        int curSample = 0;
        int nBlocksRead = 0, nBlocksTransmitted = 0;
        GenericSignal outputSignal(outputProperties);

        int sampleCount = 0;
        while (sampleCount < file.NumSamples() && (duration < 0.0 || nBlocksTransmitted < duration))
        {
            if (calibrateData)
            {
                for (int i = 0; i < sourceCh; ++i)
                    outputSignal.SetValue(i, curSample, file.CalibratedValue(i, sampleCount));

            }
            else
            {
                for (int i = 0; i < sourceCh; ++i)
                    outputSignal.SetValue(i, curSample, file.RawValue(i, sampleCount));
            }

            if (transmitStates)
            {
                file.ReadStateVector(sampleCount);
                ::memcpy(statevector.Data(curSample), file.StateVector()->Data(), file.StateVectorLength());
            }

            if (++curSample == sampleBlockSize)
            {
                curSample = 0;
                if (++nBlocksRead > offset)
                {
                    if (transmitStates)
                    {
                        // Fixup carryover sample before sending statevector:
                        Assert(statevector.Samples() > 1);
                        int lastSample = statevector.Samples() - 2,
                            carrySample = lastSample + 1;
                        for (const auto& state : statevector.StateList())
                        {
                            auto value = statevector.StateValue(state.Location(), state.Length(), lastSample);
                            statevector.SetSampleValue(state.Location(), state.Length(), carrySample, value);
                        }
                        output.Send(statevector);
                    }
                    if (transmitData)
                    {
                        output.Send(outputSignal);
                    }
                    nBlocksTransmitted++;
                }
            }
            ++sampleCount;
        }

        if (curSample != 0)
        {
            std::cerr << "Non-integer number of data blocks in input" << std::endl;
            result = illegalInput;
        }
    }
    return result;
}
