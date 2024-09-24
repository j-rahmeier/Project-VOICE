///////////////////////////////////////////////////////////////////////////////////
// $Id: RDAgetparams.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A command line utility to read RDAClient source related parameters
//   from the BrainVisionRecorder, and write them out in BCI2000 parameter format.
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
///////////////////////////////////////////////////////////////////////////////////
#include "Param.h"
#include "ParamList.h"
#include "RDAProtocol.h"
#include <iostream>
#include <set>
#include <sstream>

#define PROGRAM "RDAgetparams"

const char *usage = PROGRAM " is a command line utility that allows\n"
                            "to obtain appropriate source module parameters\n"
                            "from a host running BrainAmp's VisionRecorder.\n"
                            "On the target host, start the VisionRecorder\n"
                            "program, check that RDA is enabled under\n"
                            "Configuration->Preferences, and click the monitor (eye)\n"
                            "button before running " PROGRAM " with the host's\n"
                            "IP address as the only parameter (when omitted, this\n"
                            "defaults to \"localhost\").\n"
                            "To direct the output into a file that can later be loaded\n"
                            "into the operator module's configuration dialog, append\n"
                            "e.g. \">rda.prm\" to the command.";

template <typename T> std::string str(T t)
{
    std::ostringstream oss;
    oss << t;
    return oss.str();
}

enum
{
    noError = 0,
    generalError = -1,
};

int main(int argc, char **argv)
{
    const char *hostname = "localhost";
    if (argc > 1)
        hostname = argv[1];

    if (hostname[0] == '-')
    {
        std::cout << usage << std::endl;
        return noError;
    }

    RDA::Connection connection;

    if (!connection.Open(hostname))
    {
        std::cerr << "Could not open connection to host \"" << hostname << "\".\n\n"
                  << "Hint: " << usage << std::endl;
        return generalError;
    }

    const RDA::Info &info = connection.Info();
    ParamList paramlist;
    const char *params[] = {
        "RDA string HostName= % ",          "RDA int SourceCh= 0 ",           "RDA stringlist ChannelNames= 0 ",
        "RDA floatlist SourceChOffset= 0 ", "RDA floatlist SourceChGain= 0 ", "RDA float SamplingRate= 1 ",
        "RDA int SampleBlockSize= 1 ",      "RDA intlist TransmitChList= 0 ", "RDA matrix SpatialFilter= 0 1 ",
        "RDA int SpatialFilterType= 0 ",    "RDA float SourceMax= 300muV ",   "RDA float SourceMin= -300muV ",
    };
    for (size_t i = 0; i < sizeof(params) / sizeof(*params); ++i)
        paramlist.Add((std::string(params[i]) + " // getparams " + hostname).c_str());

    paramlist.ByPath("HostName").Value() = hostname;
    size_t numInputChannels = info.numChannels + 1;
    paramlist.ByPath("SourceCh").Value() = str(numInputChannels);
    paramlist.ByPath("ChannelNames").SetNumValues(numInputChannels);
    paramlist.ByPath("SourceChOffset").SetNumValues(numInputChannels);
    paramlist.ByPath("SourceChGain").SetNumValues(numInputChannels);
    paramlist.ByPath("TransmitChList").SetNumValues(numInputChannels - 1);
    paramlist.ByPath("SpatialFilter").SetDimensions(numInputChannels - 1, numInputChannels - 1);

    for (size_t i = 0; i < numInputChannels - 1; ++i)
    {
        paramlist.ByPath("ChannelNames").Value(i) = info.channelNames[i];
        paramlist.ByPath("SourceChOffset").Value(i) = "0";
        paramlist.ByPath("SourceChGain").Value(i) = str(info.channelResolutions[i]);
        paramlist.ByPath("TransmitChList").Value(i) = str(i + 1);
        paramlist.ByPath("SpatialFilter").Value(i, i) = "1";
    }
    paramlist.ByPath("ChannelNames").Value(numInputChannels - 1) = "T";
    paramlist.ByPath("SourceChOffset").Value(numInputChannels - 1) = "0";
    paramlist.ByPath("SourceChGain").Value(numInputChannels - 1) = "1";
    paramlist.ByPath("SamplingRate").Value() = str(1e6 / info.samplingInterval);
    paramlist.ByPath("SampleBlockSize").Value() = str(info.blockDuration / info.samplingInterval);

    std::cout << paramlist;

    return noError;
}
