////////////////////////////////////////////////////////////////////////////////
// $Id: SourceDecimationFilter.cpp 8119 2024-05-08 14:54:55Z mellinger $
// Authors: Jeremy Hill <jezhill@gmail.com>, juergen.mellinger@uni-tuebingen.de
// Description: A generic IIR bandpass filter that also provides a notch.
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
#include "FilterDesign.h"
#include "SourceDecimationFilter.h"
#include "BCIStream.h"

using namespace std;

RegisterFilter(SourceDecimationFilter, 1.C1);

void SourceDecimationFilter::Publish()
{
    Super::Publish();
    BEGIN_PARAMETER_DEFINITIONS
        "Source:Decimation float LowPassCorner=   auto   0Hz  % % // Low pass corner frequency",
        "Source:Decimation int   LowPassOrder=    auto    4    0 % // Low pass order",
        "Source:Decimation int   Downsample=      1      1    1 % // Decimation order",
    END_PARAMETER_DEFINITIONS
}

void SourceDecimationFilter::Initialize(const SignalProperties &Input, const SignalProperties &Output)
{
    if (Parameter("Downsample").ToString() == "auto")
    {
        Parameter("Downsample") = 1;
    }
    if (Parameter("LowPassCorner").ToString() == "auto")
    {
        Parameter("LowPassCorner") = (Input.SamplingRate() / ((int)Parameter("Downsample"))) * 4;
    }
    if (Parameter("LowPassOrder").ToString() == "auto")
    {
        Parameter("LowPassOrder") = 4;
    }
    mDownsampling = Parameter("Downsample");
    if (mDownsampling == 1)
    {
        bciout << "Downsampling is set to 1, no Downsampling or filtering will be performed" << std::endl;
    }
    Super::Initialize(Input, Output);
}

void SourceDecimationFilter::Process(const GenericSignal &Input, GenericSignal &Output)
{
    if (mDownsampling > 1)
        Super::Process(Input, Output);
    else
        Output = Input;
}

void SourceDecimationFilter::DesignFilter(SignalProperties &Signal, Real &outGain, ComplexVector &outZeros,
                                          ComplexVector &outPoles) const
{
    typedef Ratpoly<FilterDesign::Complex> TransferFunction;
    TransferFunction tf(1.0);
    Real gain = 1.0;

    // Low pass
    Real lpCorner = Parameter("LowPassCorner").InHertz() / Signal.SamplingRate();
    if (lpCorner > 0.5)
        bcierr << "LowPassCorner parameter exceeds Nyquist frequency" << endl;
    if (lpCorner < 0)
        bcierr << "LowPassCorner must be >= 0" << endl;
    int lpOrder = Parameter("LowPassOrder");
    if (lpCorner > 0 && lpCorner < 0.5 && lpOrder)
    {
        TransferFunction lp = FilterDesign::Butterworth().Order(lpOrder).Lowpass(lpCorner).TransferFunction();
        tf = lp;
        gain *= abs(lp.Evaluate(1.0)); // LF gain

        int f = Parameter("Downsample");

        if (f > 1)
        {
            Signal.SetElements(Signal.Elements() / f);
            PhysicalUnit &u = Signal.ElementUnit();
            u.SetGain(u.Gain() * f);
            u.SetRawMax(u.RawMax() / f);
        }
    }

    // User gain
    outGain = Parameter("FilterGain") / gain;
    outZeros = tf.Numerator().Roots();
    outPoles = tf.Denominator().Roots();
}
