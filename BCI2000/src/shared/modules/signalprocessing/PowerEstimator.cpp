////////////////////////////////////////////////////////////////////////////////
//  $Id: PowerEstimator.cpp 7464 2023-06-30 15:04:08Z mellinger $
//  Author:      juergen.mellinger@uni-tuebingen.de
//  Description: A (working) tutorial low pass filter demonstrating
//               parameter access, visualization, and unit conversion.
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
#include "PowerEstimator.h"

RegisterFilter(PowerEstimator, 2.B2);

PowerEstimator::PowerEstimator()
{
}

PowerEstimator::~PowerEstimator()
{
}

void PowerEstimator::Preflight(const SignalProperties &Input, SignalProperties &Output) const
{
    Output = Input;
    Output.SetElements(1);
    Output.ValueUnit().Clear();
}

void PowerEstimator::Initialize(const SignalProperties &Input, const SignalProperties & /*Output*/)
{
}

void PowerEstimator::Process(const GenericSignal &Input, GenericSignal &Output)
{
    for (int channel = 0; channel < Input.Channels(); ++channel)
    {
        double power = 0.0;
        for (int sample = 0; sample < Input.Elements(); ++sample)
            power += Input(channel, sample) * Input(channel, sample);
        Output(channel, 0) = power / Input.Elements();
    }
}
