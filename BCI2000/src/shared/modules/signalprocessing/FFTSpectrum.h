////////////////////////////////////////////////////////////////////////////////
// $Id: FFTSpectrum.h 7268 2023-03-22 19:12:22Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: The FFTSpectrum filter applies a Fast Fourier Transform to
//   its input data. Its output can be configured to be
//   - raw FFT coefficients,
//   - the FFT amplitude spectrum,
//   - the FFT intensity (power) spectrum.
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
#ifndef FFT_SPECTRUM_H
#define FFT_SPECTRUM_H

#include "FFTLibWrap.h"
#include "Spectrum.h"

#include <valarray>

class FFTSpectrum : public Spectrum
{
    typedef Spectrum Super;

  public:
    FFTSpectrum() : mNormalizationFactor(1), mOutputType(0)
    {
    }

  protected:
    void Publish() override;
    void Preflight(const SignalProperties &, SignalProperties &) const override;
    void Initialize(const SignalProperties &, const SignalProperties &) override;
    void Process(const GenericSignal &, GenericSignal &) override;

  private:
    typedef FFTLibWrapper::Complex Complex;
    typedef FFTLibWrapper::Real Real;
    typedef std::valarray<Complex> ComplexVector;

    ComplexVector mShiftCarrier;
    std::vector<ComplexFFT> mFFTs;
    Real mNormalizationFactor;
    int mOutputType;
};

#endif // FFT_SPECTRUM_H
