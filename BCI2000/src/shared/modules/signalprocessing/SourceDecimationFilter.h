////////////////////////////////////////////////////////////////////////////////
// $Id: SourceDecimationFilter.h 7464 2023-06-30 15:04:08Z mellinger $
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
#ifndef SOURCE_DEC_FILTER_H
#define SOURCE_DEC_FILTER_H

#include "IIRFilterBase.h"

class SourceDecimationFilter : public IIRFilterBase
{
    typedef IIRFilterBase Super;

  public:
    void Publish() override;
    void Initialize(const SignalProperties &, const SignalProperties &) override;
    void Process(const GenericSignal &, GenericSignal &) override;

  private:
    void DesignFilter(SignalProperties &, Real &gain, ComplexVector &zeros, ComplexVector &poles) const override;

    int mDownsampling;
};

#endif // IIR_BANDPASS_H
