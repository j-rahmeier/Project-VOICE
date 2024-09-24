//////////////////////////////////////////////////////////////////////
// $Id: UnitTest.cpp 7635 2023-10-02 16:14:49Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A runnable descendant that performs a test when run,
//   and reports test failure.
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
#include "UnitTest.h"
#include "ExceptionCatcher.h"

#include <cctype>
#include <cstdlib>
#include <iostream>

static bool sSelfTest = false;

namespace Tiny {

namespace Test {

static std::vector<Test*>& Tests()
{
    static std::vector<Test*> sTests;
    return sTests;
}

static void DefaultHandler(const std::string &s)
{
    std::cerr << s << std::endl;
}

static std::string MakeDesc(const std::string &inName)
{
    std::string desc = inName, lcName = inName;
    for (size_t i = 0; i < lcName.length(); ++i)
        lcName[i] = ::tolower(lcName[i]);
    if (lcName.find("test") == std::string::npos)
        desc += " test";
    return desc;
}

Test::Test(const std::string &inName) : mDesc(MakeDesc(inName))
{
    Tests().push_back(this);
}

std::ostream &Test::FailStream_(const char *inFile, int inLine)
{
    return mFailStream << mDesc << " failed in " << inFile << ", line " << inLine << ":\n\t";
}

std::ostream& Test::FailStream_()
{
    return mFailStream << mDesc << " failed:\n\t";
}

bool Test::Run_(OutputHandler_ inHandler)
{
    mFailStream.clear();
    mFailStream.str("");
    struct : ExceptionCatcher
    {
        void OnReportException(const std::string& s) override
        {
            parent->FailStream_() << s;
        }
        Test* parent;
    } exceptionCatcher;
    exceptionCatcher.parent = this;
    MemberCall<void(Test*)> onRun(&Test::OnRun_, this);
    exceptionCatcher.Run(onRun);
    mFailStream.flush();
    std::string s = mFailStream.str();
    if (!s.empty())
    {
        if (inHandler)
            inHandler(s);
        else
            DefaultHandler(s);
    }
    return s.empty();
}

Test::RunAll_::RunAll_(Test::OutputHandler_ inHandler, bool forceFailure) : failures(0)
{
    sSelfTest = forceFailure;
    OutputHandler_ handler = inHandler;
    if (!handler)
        handler = &DefaultHandler;
    for (auto pTest : Tests())
    {
        handler("Running " + pTest->mDesc);
        if (!pTest->Run_(inHandler))
        {
            ++failures;
            handler(pTest->mDesc + " failed\n");
        }
        else
        {
            handler(pTest->mDesc + " succeeded\n");
        }
    }
}

} // namespace Test
} // namespace Tiny

UnitTest(UnitTestSelfTest)
{
    TestFail_if(sSelfTest == true, "SelfTest succeeded in failing");
}
