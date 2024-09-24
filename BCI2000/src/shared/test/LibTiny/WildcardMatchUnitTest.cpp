////////////////////////////////////////////////////////////////////////////////
// $Id: WildcardMatchUnitTest.cpp 7887 2024-03-03 06:07:42Z mellinger $
// Author: mellinger@neurotechcenter.org
//  Description: Test cases for Tiny::WildcardMatch.
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
#include "WildcardMatch.h"

#include "UnitTest.h"
#include "Exception.h"

#include <cstring>

static const size_t cLengthLimit = 2 * 1024; // limit to avoid stack overflow

namespace
{

const struct TestCase
{
    const char *pattern, *match;
} sPositiveCases[] =
    {
        {"*", ""},
        {"*String", "TestString"},
        {"Test*", "TestString"},
        {"T*tString", "TestString"},
        {"T*tString", "TtttString"},
        {"Tes?String", "TestString"},
        {"Test\\?String", "Test?String"},
        {"Test\\*String", "Test*String"},
        {"?*estString", "TestString"},
        {"*TestString", "TestString"},
        {"*TestString*", "TestString"},
        {"TestString*", "TestString"},
        {"Te[s]tString", "TestString"},
        {"*[s]tString", "TestString"},
        {"*Str*", "TestString"},
        {"*TestString*", "TestString"},
        {"T*tring", "TestString"},
        {"*?String", "TestString"},
        {"\\(*?\\)String", "TestString"},
        {"\\<*ing", "TestString"},
        {" \\<*ing", " TestString"},
        {"*ing\\>", " TestString"},
        {"*ing\\> ", " TestString "},
        {" \\<*\\> \\<*\\> ", " Test String "},
        {"\\b*ing", "TestString"},
        {" \\b*ing", " TestString"},
        {"\\(*\\)ing\\b", " TestString"},
        {"*ing\\b ", " TestString "},
        {" \\b\\(*\\b\\) \\b*\\b ", " Test String "},
},
  sNegativeCases[] =
      {
          {"TestString", ""},
          {"", "TestString"},
          {"TestString", "Test"},
          {"?*TestString", "TestString"},
          {"*[^s]tString", "TestString"},
          {"\\<*ing", " TestString"},
          {"*ing\\>", "TestString "},
          {" \\<*\\> \\<*\\> ", " TestString "},
          {"\\b*ing", " TestString"},
          {"*ing\\b", "TestString "},
          {" \\b*\\b \\b*\\b ", " TestString "},
},
  sIllegalCases[] = {
      // Note: Test strings for which pattern evaluation
      // terminates early will not allow for detection
      // of illegal patterns, e.g.:
      // { "Test]String", "TessString" },
      // will not detect the illegal pattern.
      // This is by design, and *not* a test failure.
      {"Test]String", "TestString"},   {"Test[String", "TestString"},   {"Test\\String", "TestString"},
      {"TestString\\", "TestString "}, {"TestString\\(", "TestString"}, {"Test\\)String", "TestString"},
      {"Test\\(String", "TestString"},
};

void RunTests()
{
    const TestCase *pCase = 0;
    for (size_t i = 0; i < sizeof(sPositiveCases) / sizeof(*sPositiveCases); ++i)
    {
        pCase = &sPositiveCases[i];
        if (!ExtWildcardMatch(pCase->pattern, pCase->match))
            throw std_exception << "test case failed: \"" << pCase->pattern << "\" does not match \""
                                << pCase->match << "\"";
    }
    for (size_t i = 0; i < sizeof(sNegativeCases) / sizeof(*sNegativeCases); ++i)
    {
        pCase = &sNegativeCases[i];
        if (ExtWildcardMatch(pCase->pattern, pCase->match))
            throw std_exception << "test case failed: \"" << pCase->pattern << "\" should not match \""
                                << pCase->match << "\"";
    }
    for (size_t i = 0; i < sizeof(sIllegalCases) / sizeof(*sIllegalCases); ++i)
    {
        pCase = &sIllegalCases[i];
        bool thrown = false;
        try
        {
            ExtWildcardMatch(pCase->pattern, pCase->match);
        }
        catch (const Tiny::Exception &)
        {
            thrown = true;
        }
        if (!thrown)
            throw std_exception << "test case failed: \"" << pCase->pattern
                                << "\" should be illegal, did not throw an error";
    }
    // Check whether there is enough stack space for a maxLength string.
    char largeString[cLengthLimit + 1] = "";
    ::memset(largeString, 'x', cLengthLimit);
    largeString[cLengthLimit] = 0;
    WildcardMatch(largeString, largeString);
}

} // namespace

UnitTest(WildcardTest)
{
    try
    {
        ::RunTests();
    }
    catch (const Tiny::Exception &e)
    {
        TestFail << e.What();
    }
}
