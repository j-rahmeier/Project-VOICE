////////////////////////////////////////////////////////////////////////////////
// $Id: SpatialFilter.h 7195 2023-02-07 18:31:03Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de,
//          Adam Wilson
// Description: The SpatialFilter computes a linear transformation of its
//   input signal, given by a matrix-valued parameter.
//   In this matrix, input channels correspond to columns, and output channels
//   to rows.
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
#ifndef SPATIAL_FILTER_H
#define SPATIAL_FILTER_H

#include "GenericFilter.h"

class SpatialFilter : public GenericFilter
{
  public:
    SpatialFilter();
    ~SpatialFilter();

  protected:
    void Publish() override;
    void Preflight(const SignalProperties &, SignalProperties &) const override;
    void Initialize(const SignalProperties &, const SignalProperties &) override;
    void Process(const GenericSignal &Input, GenericSignal &Output) override;

  private:
    enum
    {
        none,
        fullMatrix,
        sparseMatrix,
        commonAverage
    };

    void DoPreflightFull(const SignalProperties &, SignalProperties &) const;
    void DoPreflightSparse(const SignalProperties &, SignalProperties &) const;
    void DoPreflightCAR(const SignalProperties &, SignalProperties &) const;

    void DoInitializeFull(const SignalProperties &, const SignalProperties &);
    void DoInitializeSparse(const SignalProperties &, const SignalProperties &);
    void DoInitializeCAR(const SignalProperties &, const SignalProperties &);

    void DoProcessUnoptimized(const GenericSignal &, GenericSignal &);
    void DoProcessUnoptimizedFull(const GenericSignal &, GenericSignal &);
    void DoProcessUnoptimizedSparse(const GenericSignal &, GenericSignal &);
    void DoProcessUnoptimizedCAR(const GenericSignal &, GenericSignal &);

    struct Private;
    Private *p;
};

#endif // SPATIAL_FILTER_H
