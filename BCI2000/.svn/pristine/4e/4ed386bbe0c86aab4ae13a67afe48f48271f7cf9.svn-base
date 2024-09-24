////////////////////////////////////////////////////////////////////////////////
// $Id: PipeDefinition.cpp 7195 2023-02-07 18:31:03Z wengelhardt $
// Description: This file defines which filters will be used, and the sequence
//   in which they are applied.
//   Each Filter() entry consists of the name of the filter and a string which,
//   by lexical comparison, defines the relative position of the filter in the
//   sequence.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "IIRBandpass.h"
#include "HilbertFilter.h"
#include "SubchainFilter.h"
#include "FilterCombination.h"
#include "SourceDecimationFilter.h"
#include "SpatialFilter.h"

struct MySubchain : SubchainFilter
{
  MySubchain()
  {
    Add<IIRBandpass>();
    Add<HilbertFilter>();
  }
};
struct Phase : MySubchain {};
struct Amplitude : MySubchain {};
//combines parallel filters back into one chain
struct MyParallelFilter : ParallelCombination<Phase, Amplitude> {};


Filter(SourceDecimationFilter, 2.A1);
Filter(SpatialFilter, 2.A2);
Filter(MyParallelFilter, 2.B);

//custom filter to compute the output of the filters
#include "PACFilter.h"
Filter(PACFilter, 2.C);

#include "LinearClassifier.h"
Filter(LinearClassifier, 2.E);

#include "Normalizer.h"
Filter(Normalizer, 2.F);