////////////////////////////////////////////////////////////////////////////////
// $Id: Statistics.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Statistics functions for the BCI2000 Analysis program.
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
#include "Statistics.h"

namespace Statistics
{
GenericSignal mean(const std::vector<GenericSignal> &data)
{
    if (data.empty())
        return GenericSignal();
    SignalProperties prop = data.front().Properties();
    GenericSignal mean(prop);
    for (int ch = 0; ch < mean.Channels(); ++ch)
    {
        for (int el = 0; el < mean.Elements(); ++el)
        {
            double d = 0;
            for (const auto &s : data)
                d += s(ch, el);
            d /= data.size();
            mean(ch, el) = d;
        }
    }
    return mean;
}

GenericSignal rsq(const std::vector<GenericSignal> &data1, const std::vector<GenericSignal> &data2)
{
    if (data1.empty() || data2.empty())
        return GenericSignal();

    SignalProperties prop = data1.front().Properties();
    PhysicalUnit valueUnit;
    valueUnit.SetGain(1).SetSymbol("").SetRawMax(1).SetRawMin(0);
    prop.ValueUnit() = valueUnit;
    GenericSignal rsq(prop);
    for (int ch = 0; ch < rsq.Channels(); ++ch)
    {
        for (int el = 0; el < rsq.Elements(); ++el)
        {

            double s1 = 0, q1 = 0;
            for (const auto &data : data1)
            {
                double d = data(ch, el);
                s1 += d;
                q1 += d * d;
            }

            double s2 = 0, q2 = 0;
            for (const auto &data : data2)
            {
                double d = data(ch, el);
                s2 += d;
                q2 += d * d;
            }

            double n1 = data1.size(), n2 = data2.size();
            double G = (s1 + s2) * (s1 + s2) / (n1 + n2);
            double r2 = s1 * s1 / n1 + s2 * s2 / n2 - G;
            r2 /= q1 + q2 - G;
            rsq(ch, el) = r2;
        }
    }
    return rsq;
}
} // namespace Statistics
