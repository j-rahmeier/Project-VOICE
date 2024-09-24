//////////////////////////////////////////////////////////////////////
// $Id: StringUtils.h 7563 2023-09-01 12:40:45Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: String-conversion related utility functions.
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
#ifndef TINY_STRING_UTILS_H
#define TINY_STRING_UTILS_H

#include <list>
#include <sstream>
#include <string>
#include <vector>

namespace Tiny
{

namespace StringUtils
{
class String
{
public:
    static const String Null;

    explicit String(bool multiline = false) : mMulti(multiline) {}
    String(const std::string &s) : mData(s), mMulti(false) {}
    String(const char *s) : mData(s ? s : ""), mMulti(false) {}

    virtual ~String() {}

    String(const String& s) : mData(s.mData), mMulti(s.mMulti) {}
    String &operator=(const String& s) { mData = s.mData; mMulti = s.mMulti; return *this; }
    String &operator=(const char* s) { mData = (s ? s : ""); mMulti = false; return *this; }

    bool operator==(const String& s) const { return mData == s.mData; }
    bool operator!=(const String& s) const { return mData != s.mData; }

    bool operator<(const String& s) const { return mData < s.mData; }
    bool operator<=(const String& s) const { return mData <= s.mData; }
    bool operator>(const String& s) const { return mData > s.mData; }
    bool operator>=(const String& s) const { return mData >= s.mData; }

    String operator+(const String& s) const { return String(mData + s.mData); }
    String &operator+=(const String& s) { mData += s.mData; return *this; }
    String operator+(char c) const { return String(mData + c); }
    String &operator+=(char c) { mData += c; return *this; }

    operator const std::string&() const { return mData; }
    operator std::string&() { return mData; }

    char At(size_t i) const { return mData.at(i); }
    char& At(size_t i) { return mData.at(i); }

    String Substr(size_t i, size_t j = std::string::npos) const { return mData.substr(i, j); }

    size_t Size() const { return mData.size(); }
    size_t Length() const { return mData.length(); }
    bool Empty() const { return mData.empty(); }
    void Clear() { mData.clear(); }

    char Back() const { return mData.back(); }
    char Front() const { return mData.front(); }
    void PushBack(char c) { mData.push_back(c); }
    String& Append(const String& s) { mData.append(s); return *this; }

    size_t Find(const std::string& s) const { return mData.find(s); }
    size_t Find(char c) const { return mData.find(c); }
    size_t FindFirstOf(const std::string& s) const { return mData.find_first_of(s); }
    size_t FindLastOf(const std::string& s) const { return mData.find_last_of(s); }

    const char* c_str() const { return mData.c_str(); }

    String Trimmed() const;
    std::vector<String> Split(const String&) const;
    String ToLower() const;
    String ToUpper() const;

private:
    class IStream
    {
        std::istringstream *iss;
        IStream &operator=(const IStream &);

      public:
        IStream(const String *s) : iss(s ? new std::istringstream(*s) : 0) {}
        IStream(const IStream &other) : iss(other.iss) { const_cast<IStream &>(other).iss = 0; }
        ~IStream() { delete iss; }
        explicit operator bool() const { return iss ? !!*iss : false; }
        template <class T> IStream &operator>>(T &t)
        {
            if (iss)
                *iss >> t;
            return *this;
        }
        IStream &operator>>(std::istream &f(std::istream &))
        {
            if (iss)
                *iss >> f;
            return *this;
        }
    };
    class OStream
    {
        String *str;
        std::ostringstream *oss;
        OStream &operator=(const OStream &);

      public:
        OStream(String *s) : str(s), oss(new std::ostringstream) {}
        OStream(const OStream &other) : str(other.str), oss(other.oss) { const_cast<OStream &>(other).oss = 0; }
        ~OStream()
        {
            Str();
            delete oss;
        }
        explicit operator bool() const { return oss ? !!*oss : false;}
        template <class T> OStream &operator<<(const T &t)
        {
            if (oss)
                *oss << t;
            return *this;
        }
        OStream &operator<<(std::ostream &f(std::ostream &))
        {
            if (oss)
                *oss << f;
            return *this;
        }
        const String &Str()
        {
            if (str && oss)
                str->Append(oss);
            return str ? *str : String::Null;
        }
        operator const std::string &() { return Str(); }
        operator const String &() { return Str(); }
    };

  public:
    template <class T> IStream operator>>(T &t) const
        { return IStream(this) >> t; }
    IStream operator>>(std::istream &f(std::istream &)) const
        { return IStream(this) >> f; }
    template <class T> OStream operator<<(const T &t)
        { return OStream(this) << t; }
    OStream operator<<(std::ostream &f(std::ostream &))
        { return OStream(this) << f; }

  private:
    void Append(const std::ostringstream *);
    bool mMulti;
    std::string mData;
};

inline String operator+(const std::string& s1, const String& s2)
    { return String(s1 + static_cast<const std::string&>(s2)); }
inline bool operator==(const std::string& s1, const String& s2)
    { return s1 == static_cast<const std::string&>(s2); }
inline bool operator!=(const std::string& s1, const String& s2)
    { return s1 != static_cast<const std::string&>(s2); }
inline std::istream& operator>>(std::istream& is, String& s)
    { return is >> static_cast<std::string&>(s); }
inline std::ostream& operator<<(std::ostream& os, const String& s)
    { return os << static_cast<const std::string&>(s); }

class NameValueList
{
public:
    typedef std::list<std::pair<String, String>> Data;

    virtual ~NameValueList() {}

    size_t Size() const { return mData.size(); }
    bool Empty() const { return mData.empty(); }
    void Clear() { mData.clear(); }

    Data::iterator Find(const String & s);
    Data::const_iterator Find(const String &) const;

    String &operator[](const String &);
    const String &operator[](const String &) const;

    template <class T> T To() const
    {
        T t;
        for (const auto &p : *this)
            t.push_back(std::make_pair(p.first, p.second));
        return t;
    }

    Data::iterator begin() { return mData.begin(); }
    Data::iterator end() { return mData.end(); }
    Data::const_iterator begin() const { return mData.begin(); }
    Data::const_iterator end() const { return mData.end(); }

private:
    std::list<std::pair<String, String>> mData;
};

template <typename T> std::string &ToString(std::string &s, const T &t)
{
    std::ostringstream os;
    os << t;
    s = os.str();
    return s;
}
std::string &ToString(std::string &, const std::string &);
std::string &ToString(std::string &, double);
std::string &ToString(std::string &, float);
std::string &ToString(std::string &, long double);
template <typename T> std::string ToString(const T &t)
{
    std::string s;
    return ToString(s, t);
}
template <typename T> bool FromString(T &t, const std::string &s)
{
    std::istringstream is(s);
    is >> t;
    return is && is.peek() == EOF;
}
bool FromString(std::string &s2, const std::string &s1);
template <typename T> T To(const std::string &s)
{
    T t;
    return FromString(t, s) ? t : T();
}
template <> double To<double>(const std::string &);
template <> float To<float>(const std::string &);
template <> long double To<long double>(const std::string &);

std::wstring ToWide(const char *);
inline std::wstring ToWide(const std::string &s)
{
    return ToWide(s.c_str());
}
inline std::wstring FromNarrow(const char *s)
{
    return ToWide(s);
}
inline std::wstring FromNarrow(const std::string &s)
{
    return ToWide(s.c_str());
}

std::string ToNarrow(const wchar_t *);
inline std::string ToNarrow(const std::wstring &s)
{
    return ToNarrow(s.c_str());
}
inline std::string FromWide(const wchar_t *s)
{
    return ToNarrow(s);
}
inline std::string FromWide(const std::wstring &s)
{
    return ToNarrow(s.c_str());
}

extern const std::string WhiteSpace;
std::string LStrip(const std::string &, const std::string & = WhiteSpace);
std::string RStrip(const std::string &, const std::string & = WhiteSpace);
std::string Strip(const std::string &, const std::string & = WhiteSpace);

inline std::string LTrim(const std::string &s)
{
    return LStrip(s, WhiteSpace);
}
inline std::string RTrim(const std::string &s)
{
    return RStrip(s, WhiteSpace);
}
inline std::string Trim(const std::string &s)
{
    return Strip(s, WhiteSpace);
}

template <class T = std::vector<std::string>> T Split(const std::string &separator, const std::string &s)
{
    T t;
    size_t p1 = 0, p2;
    do
    {
        p2 = s.find(separator, p1);
        t.push_back(To<typename T::value_type>(s.substr(p1, p2 - p1)));
        p1 = p2 + separator.length();
    } while (p2 != std::string::npos);
    return t;
}
std::string Join(const std::string &j, const std::string &s1, const std::string &s2);
template <class T> std::string Join(const std::string &j, const T &t)
{
    std::string s = t.empty() ? std::string() : ToString(t.front());
    for (size_t i = 1; i < t.size(); ++i)
        s += j + ToString(t[i]);
    return s;
}

std::string ReplaceAll(const std::string &, const std::string &find, const std::string &replace);

std::wstring ToUpper(const std::wstring &);
std::wstring ToLower(const std::wstring &);
inline std::string ToUpper(const std::string &s)
{
    return ToNarrow(ToUpper(ToWide(s)));
}
inline std::string ToLower(const std::string &s)
{
    return ToNarrow(ToLower(ToWide(s)));
}

bool CiLess(const std::wstring &, const std::wstring &);
bool CiLess(const std::string &, const std::string &);
template <class T> struct CiRef
{
    const T &a;
    operator const T &()
    {
        return a;
    }
    bool operator==(const T &b) const
    {
        return !CiLess(a, b) && !CiLess(b, a);
    }
    bool operator!=(const T &b) const
    {
        return CiLess(a, b) || CiLess(b, a);
    }
    bool operator<(const T &b) const
    {
        return CiLess(a, b);
    }
    bool operator>=(const T &b) const
    {
        return !CiLess(a, b);
    }
    bool operator>(const T &b) const
    {
        return CiLess(b, a);
    }
    bool operator<=(const T &b) const
    {
        return !CiLess(b, a);
    }
};
template <class T> CiRef<T> Ci(const T &t)
{
    CiRef<T> r = {t};
    return r;
}

std::ostream &WriteAsBase64(std::ostream &, const std::string &);
std::istream &ReadAsBase64(std::istream &, std::string &, int stopAtChar);
std::istream &ReadAsBase64(std::istream &, std::string &, int (*stopIf)(int) = 0);
} // namespace StringUtils

using StringUtils::String;

} // namespace Tiny

template <class T> bool operator==(const T &a, Tiny::StringUtils::CiRef<T> b)
{
    return b == a;
}
template <class T> bool operator!=(const T &a, Tiny::StringUtils::CiRef<T> b)
{
    return b != a;
}
template <class T> bool operator<(const T &a, Tiny::StringUtils::CiRef<T> b)
{
    return b >= a;
}
template <class T> bool operator<=(const T &a, Tiny::StringUtils::CiRef<T> b)
{
    return b > a;
}
template <class T> bool operator>(const T &a, Tiny::StringUtils::CiRef<T> b)
{
    return b <= a;
}
template <class T> bool operator>=(const T &a, Tiny::StringUtils::CiRef<T> b)
{
    return b < a;
}

namespace StringUtils = Tiny::StringUtils;
#if 0 // don't invade the global namespace with the name "String"
using StringUtils::Ci;
using StringUtils::String;
#endif

#endif // TINY_STRING_UTILS_H
