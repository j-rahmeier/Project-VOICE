////////////////////////////////////////////////////////////////////////////////
// $Id: FrameStatistics.h 7463 2023-06-30 14:59:17Z mellinger $
// Author: juergen.mellinger@neurotechcenter.org
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
#ifndef ANALYSIS_DATA_OBJECTS_FRAME_STATISTICS_H
#define ANALYSIS_DATA_OBJECTS_FRAME_STATISTICS_H

#include "Statistics.h"

namespace AnalysisDataObjects
{

class FrameStatistics : public JointStatistics
{
  public:
    static AnalysisDataObject *create(AnalysisDataObject::Context *, const ExpressionParser::NodeList &);
    FrameStatistics(Context *, const StatisticsObject *, double frameDuration);
    double frameDuration() const;
    double total() const;
    double lost() const;
    double relativeLoss() const;

  protected:
    JointStatistics *onCreateJointStatistics(Context *p) const override;
    ExpressionParser::Node *onMemberFunction(Context *p, const std::string &function,
                                             const ExpressionParser::NodeList &args) override;
    std::string onSummary() const override;
    bool onPostEvalSamples() override;

  private:
    double mFrameDuration, mTotal, mLost;
};

} // namespace AnalysisDataObjects

#endif // ANALYSIS_DATA_OBJECTS_FRAME_STATISTICS_H
