////////////////////////////////////////////////////////////////////////////////
// $Id: IIRFilter_ML.h 7463 2023-06-30 14:59:17Z mellinger $
// Authors: juergen.mellinger@neurotechcenter.org
// Description: An IIR filter that behaves like Matlab's filter() function.
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
#ifndef IIR_FILTER_ML_H
#define IIR_FILTER_ML_H

#include "GenericFilter.h"

class IIRFilter_ML : public GenericFilter
{
 public:
  IIRFilter_ML();
  ~IIRFilter_ML();
  void Publish() override;
  void Preflight(const SignalProperties&, SignalProperties&) const override;
  void Initialize( const SignalProperties&, const SignalProperties& ) override;
  void Process( const GenericSignal&, GenericSignal& ) override;
  void StopRun() override;

 private:
  struct Private;
  Private* p;
};

#endif // IIR_FILTER_ML_H
