/////////////////////////////////////////////////////////////////////////////
// $Id: TimeUtils.cpp 8269 2024-07-23 17:15:26Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: Utility functions dealing with time measurement and time
//   format conversion.
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
/////////////////////////////////////////////////////////////////////////////
#include "TimeUtils.h"

#if _WIN32
#include "Lockable.h"
#include <Windows.h>
#include <ctime>
#elif __APPLE__
#include <mach/mach_time.h>
#include <sys/time.h>
#else
#include <time.h>
#endif
#include <limits>
#include <algorithm>
#include <chrono>

#define MICRO_PER_1 (1000000LL)
#define NANO_PER_1 (1000LL * MICRO_PER_1)

namespace Tiny
{
namespace TimeUtils
{

const TimeInterval TimeInterval::OneSecond() { return FromRawInt(1LL << FractionBits); }
const Time Time::Never() { return FromRawUInt(std::numeric_limits<uint64_t>::max()); }
const TimeInterval TimeInterval::Forever() { return FromRawInt(std::numeric_limits<int64_t>::max()); }

static int64_t ToIntClamped(double d)
{
    if (std::isnan(d))
        return 0;

    int exp = 0;
    std::frexp(d, &exp);
    if (std::isfinite(d) && exp <= 63) // representable as int64_t
        return d;

    return std::signbit(d) ? std::numeric_limits<int64_t>::min() : std::numeric_limits<int64_t>::max();
}

TimeInterval TimeInterval::operator*(double d) const
{
    return (mValue == Forever().mValue && d) ? *this : FromRawInt(ToIntClamped(mValue * d));
}

TimeInterval TimeInterval::operator/(double d) const
{
    return d == 0 ? Forever() : FromRawInt(ToIntClamped(mValue / d));
}

Ratio TimeInterval::operator/(const TimeInterval &t) const
{
    if (mValue == Forever().mValue)
    {
        if (t.mValue == Forever().mValue)
            return Ratio(1, 1);
        if (t.mValue >= 0)
            return Ratio(1, 0);
        if (t.mValue < 0)
            return Ratio(-1, 0);
    }
    if (t.mValue == Forever().mValue)
        return Ratio(0, 1);
    return Ratio(mValue, t.mValue);
}

TimeInterval TimeInterval::operator*(const Ratio &r) const
{
    if (IsNaN(r))
        return mValue ? Forever() : 0;
    if (mValue == Forever().mValue)
        return Forever();
    if (!r.Denominator())
        return Forever();
    return FromRawInt(Round(Ratio(mValue, 1) * r));
}

double TimeInterval::Seconds() const
{
    if (mValue == Forever().mValue)
        return Infinity<double>();
    return double(mValue) / OneSecond().mValue;
}

TimeInterval Time::operator-(const Time &t) const
{
    if (RawUInt() == Never().RawUInt())
        return Time::Forever();
    if (t.RawUInt() == Never().RawUInt())
        return Time::Forever();
    return FromRawInt(RawUInt() - t.RawUInt());
}

namespace
{
class SystemClocks
{
  public:
    SystemClocks()
    {
        Init();
    }
    Time MonotonicTime() const;
    Time SystemTimeUTC() const;

  private:
    void Init();
};
const SystemClocks& Clocks()
{
    static const SystemClocks instance;
    return instance;
}

#if _WIN32

static uint64_t sFreq;
static int sShift;

void SystemClocks::Init()
{
    LARGE_INTEGER f;
    Syscall(::QueryPerformanceFrequency(&f));
    sFreq = f.QuadPart;
    sShift = TimeInterval::FractionBits;
    while ((sFreq & 1) && sShift > 0)
        sFreq >>= 1, --sShift;
}

Time TimeFromWin32PerformanceCounter(uint64_t c)
{
    uint64_t value = c;
    value <<= sShift;
    value /= sFreq;
    value &= ~(1LL << (sizeof(value) * 8 - 1));
    return Time::FromRawUInt(value);
}

Time SystemClocks::MonotonicTime() const
{
    auto now = std::chrono::steady_clock::now();
    auto nanoseconds = std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
    long double d = Time::OneSecond().RawInt();
    d *= 1e-9;
    d *= nanoseconds.time_since_epoch().count();
    return Time::FromRawUInt(::floor(d));
}

Time SystemClocks::SystemTimeUTC() const
{
    FILETIME filetime;
    ::GetSystemTimePreciseAsFileTime(&filetime);
    ULARGE_INTEGER time;
    time.LowPart = filetime.dwLowDateTime;
    time.HighPart = filetime.dwHighDateTime;
    return Time::FromWin32Time(time.QuadPart);
}

#else // _WIN32

void SystemClocks::Init()
{
}

Time SystemClocks::MonotonicTime() const
{
    struct timespec t;
    ::clock_gettime(CLOCK_MONOTONIC, &t);
    return Time(TimeInterval::FromTimespec(t));
}

Time SystemClocks::SystemTimeUTC() const
{
    struct timespec t = {0, 0};
    ::clock_gettime(CLOCK_REALTIME, &t);
    return Time(TimeInterval::FromTimespec(t));
}

#endif // _WIN32

} // anonymous namespace

#if _WIN32

Time Time::FromWin32PerformanceCounter(int64_t c)
{
    return TimeFromWin32PerformanceCounter(c);
}

#endif // _WIN32

Time MonotonicTime()
{
    return Clocks().MonotonicTime();
}

Time SystemTimeUTC()
{
    return Clocks().SystemTimeUTC();
}

int TimeInterval::ToIntTimeout() const
{
    if (*this == Forever())
        return InfiniteTimeout;
    int ms = ToIntMilliseconds();
    if (ms >= 0)
        return ms;
    return 0;
}

TimeInterval TimeInterval::FromIntTimeout(int ms)
{
    if (ms >= 0)
        return FromMilliseconds(ms);
    if (ms == InfiniteTimeout)
        return Forever();
    return Time::Interval(0);
}

TimeInterval TimeInterval::FromTimeval(const struct timeval &t)
{
    return FromRawInt(((t.tv_sec * 1LL) << FractionBits) + OneSecond().RawInt() * t.tv_usec / MICRO_PER_1);
}

struct timeval *TimeInterval::ToTimeval(struct timeval &t) const
{
    t.tv_sec = static_cast<long>(mValue >> FractionBits);
    t.tv_usec = static_cast<long>(((mValue % OneSecond().RawInt()) * MICRO_PER_1) >> FractionBits);
    return (*this == Forever()) ? 0 : &t;
}

#ifndef _WIN32

TimeInterval TimeInterval::FromTimespec(const struct timespec &t)
{
    int64_t raw = t.tv_sec << FractionBits;
    raw += OneSecond().RawInt() * t.tv_nsec / NANO_PER_1;
    TimeInterval interval = FromRawInt(raw);
    return interval;
}

struct timespec *TimeInterval::ToTimespec(struct timespec &t) const
{
    t.tv_sec = mValue >> FractionBits;
    t.tv_nsec = ((mValue % OneSecond().RawInt()) * NANO_PER_1) >> FractionBits;
    return &t;
}

#endif // _WIN32

} // namespace TimeUtils
} // namespace Tiny
