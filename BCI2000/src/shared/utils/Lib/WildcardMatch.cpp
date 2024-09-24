////////////////////////////////////////////////////////////////////////////////
// $Id: WildcardMatch.cpp 7633 2023-10-02 13:21:01Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
//  Description: A function that matches strings against glob-like patterns.
//    In patterns, the following special characters are recognized:
//    "*" matches zero or more arbitrary characters,
//    "?" matches a single arbitrary character,
//    "[abc]" matches any of the characters "abc",
//    "[a-c]" matches any character from the range between "a" and "c",
//    "[^abc]" and "[^a-c]" both match any character not in "abc".
//    "\<" matches the beginning of a word,
//    "\>" matches the end of a word,
//    "\b" matches either word boundary,
//    "\" is used as an escape character; write "\\" to match a single backslash.
//    Note that you must duplicate each backslash within a C string literal, so
//    to express a literal backslash within a C string, you will need to write
//    four backslashes: "\\\\".
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

#include "Debugging.h"

#include <stack>
#include <vector>

namespace {

class Matcher
{
  public:
    explicit Matcher(bool inCS) : cs(inCS), s0(0)
    {
    }
    const std::string &Error() const
    {
        return mError;
    }

    bool operator()(const char *pat, const char *str, Tiny::Matches * = 0);

  private:
    bool Equal(char, char) const;
    bool ApplyActions(Tiny::Matches *);

    bool Match(const char *p, const char *s);
    bool CharClassMatch(const char *p, const char *s);
    bool BoundaryMatch(const char *p, const char *s);
    bool SubMatch(const char *p, const char *s);

    static bool IsSpecialChar(char);
    static bool IsWordChar(char);
    static bool IsBoundary(char);
    static bool IsSubMatch(char);

    const bool cs;
    const char *s0;

    std::string mError;
    typedef std::pair<char, const char *> Action;
    std::vector<Action> mActions;
};

bool Matcher::operator()(const char *pat, const char *str, Tiny::Matches *outpMatches)
{
#if 0  // TINY_DEBUG // may be triggered by C++ mangled names in DylibImports
  bool specialsInPattern = false;
  for( const char* p = pat; *p; ++p )
    specialsInPattern = specialsInPattern || IsSpecialChar( *p );
  bool specialsInString = false;
  for( const char* p = str; *p; ++p )
    specialsInString = specialsInString || IsSpecialChar( *p );
  if( specialsInString && !specialsInPattern )
    SuggestDebugging(
      "WildcardMatch: Special characters in target but not in pattern -- inverted arguments?\n\n"
      << "Pattern:\t" << pat << "\nTarget:\t" << str
    );
#endif // TINY_DEBUG
    mActions.clear();
    mError.clear();
    s0 = str;
    Match(pat, s0);
    return ApplyActions(outpMatches);
}

bool Matcher::Equal(char a, char b) const
{
    return cs ? (a == b) : (::tolower(a) == ::tolower(b));
}

bool Matcher::ApplyActions(Tiny::Matches *outpMatches)
{
    bool result = !mActions.empty() && mActions.back().first == '\0';
    if (result && outpMatches)
    {
        Tiny::Matches &matches = *outpMatches;
        matches.clear();
        Tiny::Matches::value_type match = {0, static_cast<size_t>(mActions.back().second - s0)};
        matches.push_back(match);
        std::stack<size_t> s;
        for (size_t i = 0; i < mActions.size() - 1; ++i)
        {
            switch (mActions[i].first)
            {
            case '(': {
                s.push(matches.size());
                match.begin = mActions[i].second - s0;
                match.length = 0;
                matches.push_back(match);
            }
            break;
            case ')':
                if (s.empty())
                    mError = "Unbalanced \\)";
                else
                {
                    Tiny::Matches::value_type &m = matches[s.top()];
                    m.length = (mActions[i].second - s0) - m.begin;
                    s.pop();
                }
            default:;
            }
        }
        if (!s.empty())
            mError = "Unbalanced \\(";
    }
    if (!mError.empty())
        throw std_exception << "Error in wildcard pattern: " << mError;
    mActions.clear();
    return result;
}

bool Matcher::Match(const char *p, const char *s)
{
    bool result = false;
    switch (*p)
    {
    case '*':
        result = Match(p + 1, s) || ((*s != '\0') && (Match(p + 1, s + 1) || Match(p, s + 1)));
        break;

    case '?':
        result = (*s != '\0') && Match(p + 1, s + 1);
        break;

    case '[':
        result = CharClassMatch(p + 1, s);
        break;

    case ']':
        mError = "Unbalanced ]";
        break;

    case '\0':
        result = (*s == '\0');
        if (result)
            mActions.push_back(std::make_pair('\0', s));
        break;

    case '\\': {
        char code = *(p + 1);
        if (IsBoundary(code))
            result = BoundaryMatch(p + 1, s);
        else if (IsSubMatch(code))
            result = SubMatch(p + 1, s);
        else if (IsSpecialChar(code))
            result = Equal(code, *s) && Match(p + 2, s + 1);
        else
            mError = std::string("Illegal escape code: \\") + code;
    }
    break;

    default:
        result = Equal(*p, *s) && Match(p + 1, s + 1);
    }
    return result;
}

bool Matcher::CharClassMatch(const char *p, const char *s)
{
    bool result = false;

    std::string charset;
    bool negate = false;
    while (*p != '\0' && *p != ']')
    {
        switch (*p)
        {
        case '^':
            if (charset.empty())
                negate = true;
            else
                charset += *p;
            ++p;
            break;
        case '-':
            if (charset.empty())
                charset += *p;
            else
            {
                ++p;
                for (char c = *charset.rbegin(); c <= *p; ++c)
                    charset += c;
            }
            break;
        case '\\':
            if (*(p + 1) != '\0')
                ++p;
            /* no break */
        default:
            charset += *p++;
        }
        if (*p == '\0')
        {
            mError = "Missing ]";
            result = false;
        }
        else
        {
            result = (charset.find(*s) != std::string::npos);
            if (negate)
                result = !result;
        }
    }
    return result && Match(p + 1, s + 1);
}

bool Matcher::BoundaryMatch(const char *p, const char *s)
{
    bool result = false;
    bool wasWord = s > s0 && IsWordChar(*(s - 1)), isWord = IsWordChar(*s), left = !wasWord && isWord,
         right = wasWord && !isWord;
    switch (*p)
    {
    case '<':
        result = left;
        break;
    case '>':
        result = right;
        break;
    case 'b':
        result = left || right;
        break;
    default:
        Assert(false);
    }
    result = result && Match(p + 1, s);
    return result;
}

bool Matcher::SubMatch(const char *p, const char *s)
{
    size_t oldSize = mActions.size();
    mActions.push_back(std::make_pair(*p, s));
    bool result = Match(p + 1, s);
    if (!result)
        mActions.resize(oldSize);
    return result;
}

bool Matcher::IsSpecialChar(char c)
{
    static const std::string s = "*?[]\\";
    return s.find(c) != std::string::npos;
}

bool Matcher::IsWordChar(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || (c & 0x80);
}

bool Matcher::IsBoundary(char c)
{
    static const std::string codes = "<>b";
    return codes.find(c) != std::string::npos;
}

bool Matcher::IsSubMatch(char c)
{
    static const std::string codes = "()";
    return codes.find(c) != std::string::npos;
}

} // namespace

namespace Tiny
{

bool WildcardMatch(const std::string &inPattern, const std::string &inString, bool inCaseSensitive)
{
    return WildcardMatch(inPattern.c_str(), inString.c_str(), inCaseSensitive);
}

bool WildcardMatch(const char *inPattern, const char *inString, bool inCaseSensitive)
{
    return Matcher(inCaseSensitive)(inPattern, inString);
}

Matches ExtWildcardMatch(const std::string &inPattern, const std::string &inString, bool inCaseSensitive)
{
    return ExtWildcardMatch(inPattern.c_str(), inString.c_str(), inCaseSensitive);
}

Matches ExtWildcardMatch(const char *inPattern, const char *inString, bool inCaseSensitive)
{
    Matches result;
    Matcher m(inCaseSensitive);
    m(inPattern, inString, &result);
    return result;
}

} // namespace Tiny
