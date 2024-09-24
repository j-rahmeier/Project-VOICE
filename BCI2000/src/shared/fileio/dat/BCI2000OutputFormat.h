////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000OutputFormat.h 7437 2023-06-22 17:09:12Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An output class for the BCI2000 dat format.
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
#ifndef BCI2000_OUTPUT_FORMAT_H
#define BCI2000_OUTPUT_FORMAT_H

#include "GenericOutputFormat.h"

class BCI2000OutputFormat : public GenericOutputFormat
{
  public:
    BCI2000OutputFormat()
    {
    }
    virtual ~BCI2000OutputFormat()
    {
    }

    void Publish() const override;
    void Preflight(const SignalProperties &, const StateVector &) const override;
    void Initialize(const SignalProperties &, const StateVector &) override;
    void BeginFile(std::ostream &, const ParamList &, const StateList &) override;
    void EndFile(std::ostream &) override;
    void Write(std::ostream &, const GenericSignal &, const StateVector &) override;
    void Halt() override
    {
    }

    const char *DataFileExtension() const override
    {
        return ".dat";
    }

  private:
    SignalProperties mInputProperties;
    int mStatevectorLength;
};

#endif // BCI2000_OUTPUT_FORMAT_H
