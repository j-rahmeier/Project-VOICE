//////////////////////////////////////////////////////////////////////
// $Id: StringUtilsUnitTest.cpp 7633 2023-10-02 13:21:01Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Unit tests for Tiny::StringUtils.
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
#include "StringUtils.h"

#include "UnitTest.h"
#include <ctime>

UnitTest(Base64Test)
{
    unsigned int seed = static_cast<unsigned int>(::time(nullptr));
    ::srand(seed);
    for (int i = 0; i < 100 * 1000; ++i)
    {
        std::string s;
        while (rand() >= RAND_MAX / 100)
            s += static_cast<char>(rand() % 256);
        std::stringstream stream;
        StringUtils::WriteAsBase64(stream, s);
        std::string s2;
        StringUtils::ReadAsBase64(stream, s2);
        TestFail_if(s != s2, "seed: " << seed << ", iteration: " << i);
    }
    ::srand(1);
}
