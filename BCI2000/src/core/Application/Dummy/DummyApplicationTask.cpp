////////////////////////////////////////////////////////////////////////////////
// $Id: DummyApplicationTask.cpp 7775 2023-12-20 15:30:38Z mellinger $
// Authors:mellinger@neurotechcenter.org
// Description: A minimal task filter to avoid sending large amounts of data
//   to the Operator module
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
#include "DummyApplicationTask.h"

RegisterFilter(DummyApplicationTask, 3);

static const int cSignalMaxSizeKilobytes = 64;

static bool SignalIsLarge(const SignalProperties& s)
{
    return s.Channels() * s.Elements() > (cSignalMaxSizeKilobytes*1024) / sizeof(float);
}

DummyApplicationTask::DummyApplicationTask()
: mCopySignal(false)
{
}

DummyApplicationTask::~DummyApplicationTask()
{
}

void DummyApplicationTask::Publish()
{
    BEGIN_PARAMETER_DEFINITIONS
      "Application int AlwaysTransmitSignal= 0 0 0 1 "
      "// always transmit signal to Operator module, even if it is large (boolean)",
    END_PARAMETER_DEFINITIONS
}

void DummyApplicationTask::Preflight(const SignalProperties &Input, SignalProperties &Output) const
{
    Output = Input;
    bool alwaysTransmitSignal = Parameter("AlwaysTransmitSignal");
    if (SignalIsLarge(Input) && !alwaysTransmitSignal)
    {
        bciwarn << "The input signal to DummyApplication is quite large. "
                << "DummyApplication will not transmit its input signal to the Operator module "
                << "to avoid performance issues.\n"
                << "Set the AlwaysTransmitSignal parameter to \"true\" to send the signal "
                << "nevertheless.";
        Output.SetChannels(0).SetElements(0);
    }
}

void DummyApplicationTask::Initialize(const SignalProperties &Input, const SignalProperties &)
{
    bool alwaysTransmitSignal = Parameter("AlwaysTransmitSignal");
    mCopySignal = !SignalIsLarge(Input) || alwaysTransmitSignal;
}

void DummyApplicationTask::Process(const GenericSignal& Input, GenericSignal& Output)
{
    if (mCopySignal)
        Output = Input;
}

