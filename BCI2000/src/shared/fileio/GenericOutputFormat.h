////////////////////////////////////////////////////////////////////////////////
// $Id: GenericOutputFormat.h 7437 2023-06-22 17:09:12Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A virtual class interface for output data formats.
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
#ifndef GENERIC_OUTPUT_FORMAT_H
#define GENERIC_OUTPUT_FORMAT_H

#include "Environment.h"
#include "GenericSignal.h"
#include <cstdio>
#include <ctime>
#include <iostream>

class GenericOutputFormat : public Environment
{
  protected: // No instantiation outside derived classes.
    GenericOutputFormat()
    {
    }

  public:
    virtual ~GenericOutputFormat()
    {
    }
    // Request parameters and states from the Publish() function.
    virtual void Publish() const = 0;
    // Test parameters and states for consistency from the Preflight() function.
    virtual void Preflight(const SignalProperties &, const StateVector &) const;
    // Apply parameters and states from Initialize().
    virtual void Initialize(const SignalProperties &, const StateVector &) = 0;
    // Write a header in BeginFile(), and a footer in EndFile().
    // Note that BeginFile(), EndFile(), and Write() are called asynchronously from
    // a writer thread, so it is not possible to use Environment::Parameters and
    // Environment::States for access.
    virtual void BeginFile(std::ostream &, const ParamList&, const StateList&)
    {
    }
    virtual void EndFile(std::ostream &)
    {
    }
    // The Write() function takes as argument the state vector
    // that existed at the time of the signal argument's time stamp.
    virtual void Write(std::ostream &, const GenericSignal &, const StateVector &) = 0;
    // Stop all asynchronous activity in Halt().
    virtual void Halt()
    {
    }
    // A descendant reports the file extension through the DataFileExtension()
    // function.
    virtual const char *DataFileExtension() const = 0;

  protected:
    void AcquireTime(time_t &t, tm *&time) const;
};

#endif // GENERIC_OUTPUT_FORMAT_H
