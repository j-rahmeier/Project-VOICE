////////////////////////////////////////////////////////////////////
// $Id: bci_stream2prm.cpp 7195 2023-02-07 18:31:03Z mellinger $
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
#include "GenericVisualization.h"
#include "MessageChannel.h"
#include "Param.h"
#include "ParamList.h"
#include "State.h"
#include "StateList.h"
#include "StateVector.h"
#include "Status.h"
#include "SysCommand.h"
#include "bci_tool.h"

#include <iostream>
#include <string>

std::string ToolInfo[] = {"bci_stream2prm", "Convert a BCI2000 message stream into a BCI2000 parameter file.",
                          "Reads a BCI2000 message stream from standard input and extracts "
                          "parameter messages to the standard output as a BCI2000 parameter "
                          "(.prm) file.",
                          "binary", ""};

class StreamToPrm : public MessageChannel
{
  public:
    StreamToPrm(Streambuf &in, Streambuf &out) : MessageChannel(in), mOutput(out)
    {
    }

  protected:
    bool OnParam(std::istream &) override;
    bool OnProtocolVersion(std::istream &is) override
    {
        return Unserialize<ProtocolVersion>(is);
    }
    bool OnStatus(std::istream &is) override
    {
        return Unserialize<Status>(is);
    }
    bool OnState(std::istream &is) override
    {
        return Unserialize<State>(is);
    }
    bool OnVisSignalProperties(std::istream &is) override
    {
        return Unserialize<VisSignalProperties>(is);
    }
    bool OnVisSignal(std::istream &is) override
    {
        return Unserialize<VisSignal>(is);
    }
    bool OnStateVector(std::istream &is) override
    {
        return Unserialize<StateVector>(is);
    }
    bool OnSysCommand(std::istream &is) override
    {
        return Unserialize<SysCommand>(is);
    }

  private:
    template <typename T> bool Unserialize(std::istream &is)
    {
        T t;
        return !!t.Unserialize(is);
    }

    Tiny::OStream mOutput;
};

ToolResult ToolInit()
{
    return noError;
}

ToolResult ToolMain(Streambuf &in, Streambuf &out, OptionSet &arOptions)
{
    if (arOptions.size() > 0)
        return illegalOption;
    StreamToPrm converter(in, out);
    while (converter.HandleMessage())
        ;
    if (in.Input()->Failed())
        return illegalInput;
    return noError;
}

bool StreamToPrm::OnParam(std::istream &is)
{
    Param p;
    if (p.Unserialize(is))
        mOutput << p << "\r\n";
    return true;
}
