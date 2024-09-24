////////////////////////////////////////////////////////////////////
// $Id: bci_decimate.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Author:  juergen.mellinger@uni-tuebingen.de
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

std::string ToolInfo[] = {"bci_decimate",
                          "Decimate data in a binary BCI2000 stream.",
                          "Reads a BCI2000 compliant binary stream from standard input, "
                          "and writes it to standard output, retaining only every "
                          "Nth statevector and signal message.",
                          "binary",
                          "-d<N>,    --decimate=<N>        Decimation factor, defaults to 1",
                          ""};

class Decimate : public MessageChannel
{
  public:
    Decimate(int factor, Streambuf &in, Streambuf &out)
        : MessageChannel(in), mOutput(out), mFactor(factor), mCount(0), mpStatevector(nullptr)
    {
    }
    ~Decimate()
    {
        delete mpStatevector;
    }
    template <typename T> bool Send(const T &) = delete;

  protected:
    bool OnProtocolVersion(std::istream &) override;
    bool OnStatus(std::istream &) override;
    bool OnParam(std::istream &) override;
    bool OnState(std::istream &) override;
    bool OnVisSignalProperties(std::istream &) override;
    bool OnVisSignal(std::istream &) override;
    bool OnStateVector(std::istream &) override;
    bool OnSysCommand(std::istream &) override;

  private:
    template <typename T> void Forward(std::istream &);
    template <typename T> void Absorb(std::istream &);

  private:
    MessageChannel mOutput;
    int mFactor, mCount;
    StateList mStatelist;
    StateVector *mpStatevector;
};

ToolResult ToolInit()
{
    return noError;
}

ToolResult ToolMain(Streambuf &in, Streambuf &out, OptionSet &arOptions)
{
    int decimation = ::atoi(arOptions.getopt("-d|-D|--decimation", "1").c_str());
    Decimate decimator(decimation, in, out);
    while (decimator.HandleMessage())
        ;
    if (in.Input()->Failed())
        return illegalInput;
    return noError;
}

template <typename T> void Decimate::Forward(std::istream &is)
{
    T t;
    t.Unserialize(is);
    mOutput.Send(t);
}

template <typename T> void Decimate::Absorb(std::istream &is)
{
    T t;
    t.Unserialize(is);
}

bool Decimate::OnProtocolVersion(std::istream &is)
{
    Forward<ProtocolVersion>(is);
    return true;
}

bool Decimate::OnStatus(std::istream &is)
{
    Forward<Status>(is);
    return true;
}

bool Decimate::OnParam(std::istream &is)
{
    Forward<Param>(is);
    return true;
}

bool Decimate::OnState(std::istream &is)
{
    State s;
    s.Unserialize(is);
    if (is)
    {
        mStatelist.Delete(s.Name());
        mStatelist.Add(s);
        if (mpStatevector)
        {
            delete mpStatevector;
            mpStatevector = new StateVector(mStatelist);
        }
        mOutput.Send(s);
    }
    return true;
}

bool Decimate::OnVisSignalProperties(std::istream &is)
{
    Forward<VisSignalProperties>(is);
    return true;
}

bool Decimate::OnVisSignal(std::istream &is)
{
    if (mCount % mFactor == 0)
        Forward<VisSignal>(is);
    else
        Absorb<VisSignal>(is);
    return true;
}

bool Decimate::OnStateVector(std::istream &is)
{
    if (!mpStatevector)
        mpStatevector = new StateVector(mStatelist);
    mpStatevector->Unserialize(is);
    // state vectors are sent first, so we increase
    // the count here rather than in the signal handler
    if ((++mCount %= mFactor) == 0)
        mOutput.Send(*mpStatevector);
    return true;
}

bool Decimate::OnSysCommand(std::istream &is)
{
    Forward<SysCommand>(is);
    return true;
}
