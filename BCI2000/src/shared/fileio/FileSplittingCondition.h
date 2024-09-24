////////////////////////////////////////////////////////////////////////////////
// $Id: FileSplittingCondition.h 7645 2023-10-05 17:37:31Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A helper class that parses a file splitting expression and
//     evaluates it.
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
#ifndef FILE_SPLITTING_CONDITION_H
#define FILE_SPLITTING_CONDITION_H

#include <string>
#include <limits>
#include "TimeUtils.h"

class FileSplittingCondition
{
  public:
    FileSplittingCondition& Compile(const std::string&);
    bool Evaluate(TimeUtils::TimeInterval, int64_t fileLength) const;
    const std::string& Error() const;

  private:
    std::string mError;
    TimeUtils::TimeInterval mMaxDuration = TimeUtils::TimeInterval::Forever();
    int64_t mMaxLength = std::numeric_limits<int64_t>::max();
};

#endif // FILE_SPLITTING_CONDITION_H

