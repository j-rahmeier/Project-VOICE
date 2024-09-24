//////////////////////////////////////////////////////////////////////
// $Id: Exception.h 7753 2023-12-11 18:26:45Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Classes for convenient throwing and catching of
//   expected and unexpected exceptions.
//
//   #include "Exception.h"
//   ...
//   throw std_invalid_argument << "Illegal value of n: " << n;
//
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
#ifndef TINY_EXCEPTION_H
#define TINY_EXCEPTION_H

#include "Compiler.h"
#include "StringUtils.h"
#include <exception>
#include <stdexcept>
#include <typeinfo>

#if defined(_MSC_VER) && _MSC_VER < 1800
#define noexcept
#endif

#define STR_(x) STR__(x)
#define STR__(x) #x

#define FILE_CONTEXT_ "\nFile: " << __FILE__ << "\nLine: " << __LINE__

#ifdef FUNCTION_
#define EXCEPTION_CONTEXT_ "\nFunction: " << FUNCTION_ << "()" << FILE_CONTEXT_
#else
#define EXCEPTION_CONTEXT_ FILE_CONTEXT_
#endif

#define EXCEPTION_CONTEXT_STRING_ Tiny::StringUtils::String() << EXCEPTION_CONTEXT_
#define stdexception(type) (Tiny::Exception_<std::type>("", EXCEPTION_CONTEXT_STRING_))
#define std_logic_error stdexception(logic_error)
#define std_domain_error stdexception(domain_error)
#define std_invalid_argument stdexception(invalid_argument)
#define std_length_error stdexception(length_error)
#define std_runtime_error stdexception(runtime_error)
#define std_out_of_range stdexception(out_of_range)
#define std_range_error stdexception(range_error)
#define std_overflow_error stdexception(overflow_error)
#define std_underflow_error stdexception(underflow_error)
#define std_bad_alloc stdexception(bad_alloc)
#define std_exception stdexception(exception)

namespace Tiny
{

class Exception
{
  public:
    explicit Exception(const String &inWhat = 0, const String &inWhere = 0,
                       const std::type_info &inType = typeid(Exception));
    Exception(const Exception &);
    virtual ~Exception()
    {
    }
    virtual void Raise() const
    {
        throw *this;
    }

    const String &Where() const
    {
        return mWhere;
    }
    String What() const
    {
        return mWhat.str();
    }
    bool AlreadyShown() const
    {
        return mAlreadyShown;
    }
    template <class U> Exception& operator<<(const U& u)
    {
        mWhat << u;
        return *this;
    }

  protected:
    std::ostringstream mWhat;
    mutable std::string mBuffer;
    String mWhere;

  private:
    mutable bool mAlreadyShown;
    const std::type_info &mType;
};

template <typename T> struct Exception_ : Exception, T
{
    explicit Exception_(const String &what = 0, const String &where = 0) : Exception(what, where, typeid(T)), T("")
    {
    }
    ~Exception_()
    {
    }
    template <class U> Exception_& operator<<(const U& u)
    {
        mWhat << u;
        return *this;
    }
    const char* what() const noexcept override
    {
        try {
            mBuffer = mWhat.str();
            return mBuffer.c_str();
        }
        catch (...) {}
        return "n/a";
    }
};

template <> struct Exception_<std::exception> : Exception, std::exception
{
    explicit Exception_(const String &what = 0, const String &where = 0) : Exception(what, where)
    {
    }
    ~Exception_()
    {
    }
    template <class U> Exception& operator<<(const U& u)
    {
        mWhat << u;
        return *this;
    }
    const char* what() const noexcept override
    {
        try {
            mBuffer = mWhat.str();
            return mBuffer.c_str();
        }
        catch (...) {}
        return "n/a";
    }
};

template <> struct Exception_<std::bad_alloc> : Exception, std::bad_alloc
{
    explicit Exception_(const String &what = 0, const String &where = 0) : Exception(what, where)
    {
    }
    ~Exception_()
    {
    }
    template <class U> Exception_ operator<<(const U& u)
    {
        mWhat << u;
        return *this;
    }
    const char *what() const noexcept override
    {
        try {
            mBuffer = mWhat.str();
            return mBuffer.c_str();
        }
        catch (...) {}
        return "n/a";
    }
};

} // namespace Tiny

using Tiny::Exception;

#endif // TINY_EXCEPTION_H
