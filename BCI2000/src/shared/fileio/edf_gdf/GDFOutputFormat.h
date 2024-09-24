////////////////////////////////////////////////////////////////////////////////
// $Id: GDFOutputFormat.h 8072 2024-04-24 18:35:48Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Output into a GDF data file.
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
#ifndef GDF_OUTPUT_FORMAT_H
#define GDF_OUTPUT_FORMAT_H

#include "EDFOutputBase.h"
#include "Expression/Expression.h"

#include <vector>

class GDFOutputFormat : public EDFOutputBase
{
  public:
    GDFOutputFormat();
    virtual ~GDFOutputFormat();

    void Publish() const override;
    void Preflight(const SignalProperties &, const StateVector &) const override;
    void Initialize(const SignalProperties &, const StateVector &) override;
    void BeginFile(std::ostream& os, const ParamList& params, const StateList& states) override;
    void EndFile(std::ostream &) override;
    void Write(std::ostream &, const GenericSignal &, const StateVector &) override;

    const char *DataFileExtension() const override
    {
        return ".gdf";
    }

  private:
    struct EventInfo
    {
        unsigned int SamplePosition;
        unsigned short Code;
    };
    std::vector<EventInfo> mEvents;
    // cannot use std::vector<Expression> because Expressions must remain in original memory position
    std::list<Expression> mEventConditions;
    std::vector<unsigned short> mEventCodes;
    std::vector<bool> mPreviousConditionValues;
    mutable std::mutex mMutex;
};

#endif // GDF_OUTPUT_FORMAT_H
