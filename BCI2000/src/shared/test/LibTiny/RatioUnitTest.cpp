//////////////////////////////////////////////////////////////////////
// $Id: RatioUnitTest.cpp 8219 2024-06-26 16:54:53Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Unit tests for Tiny::Ratio.
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
///////////////////////////////////////////////////////////////////////
#include "Ratio.h"

#include "Numeric.h"
#include "UnitTest.h"
#include <ctime>

UnitTest(Tiny_Ratio)
{
    unsigned int seed = static_cast<unsigned int>(::time(nullptr));
    ::srand(seed);
    for (int n = 0; n < 1000 * 1000; ++n)
    {
        Random<int> num, den;
        if (num != 0 && den != 0)
        {
            Ratio ratio(num, den);
            TestRequire(Floor(ratio) == Floor(ratio.To<double>()));
            TestRequire(Floor(-ratio) == Floor((-ratio).To<double>()));
            TestRequire(Ceil(ratio) == Ceil(ratio.To<double>()));
            TestRequire(Ceil(-ratio) == Ceil((-ratio).To<double>()));
            TestRequire(Round(ratio) == Round(ratio.To<double>()));
            TestRequire(Round(-ratio) == Round((-ratio).To<double>()));
            TestRequire(ratio / ratio == Ratio::From<int>(1));
            TestRequire(ratio * ratio / ratio / ratio == Ratio::From<int>(1));
            TestRequire(1 / ratio * ratio == Ratio::From<int>(1));
            TestRequire(ratio.Numerator() % ratio.GreatestCommonDivisor() == 0);
            TestRequire(ratio.Denominator() % ratio.GreatestCommonDivisor() == 0);
            Random<int> num2, den2;
            if (num2 != 0 && den2 != 0)
            {
                Ratio ratio2(num2, den2);
                TestRequire(Round(ratio + ratio2) == Round(ratio.To<double>() + ratio2.To<double>()));
                TestRequire(Round(ratio - ratio2) == Round(ratio.To<double>() - ratio2.To<double>()));
            }
        }
    }
    ::srand(1);
    TestRequire(Ratio::From<double>(1.2345e-3).To<double>() == 1.2345e-3);
    TestRequire(Ratio::From<double>(1.2345e3).To<double>() == 1.2345e3);
}

