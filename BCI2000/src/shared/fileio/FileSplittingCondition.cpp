////////////////////////////////////////////////////////////////////////////////
// $Id: FileSplittingCondition.cpp 7887 2024-03-03 06:07:42Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A helper class that parses a file splitting expression and
//     evaluates it.
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
#include "FileSplittingCondition.h"
#include "StringUtils.h"

#include <algorithm>

static struct { const char* symbol; double value; } units[] =
{
    // There is quite some confusion about units of file sizes.
    // We stick to the official NIST recommendation.
    { "B", 1.0 },
    { "kB", 1000.0 }, { "KB", 1000.0 }, { "MB", 1000.0 * 1000.0 }, { "GB", 1000.0 * 1000.0 * 1000.0 }, { "TB", 1000.0 * 1000.0 * 1000.0 * 1000.0 },
    { "KiB", 1024.0 }, { "MiB", 1024.0 * 1024.0 }, { "GiB", 1024.0 * 1024.0 * 1024.0 }, { "TiB", 1024.0 * 1024.0 * 1024.0 * 1024.0 },
};

FileSplittingCondition& FileSplittingCondition::Compile(const std::string& inExpr)
{
    std::string expr = StringUtils::Trim(inExpr);
    mMaxDuration = TimeUtils::TimeInterval::Forever();
    mMaxLength = std::numeric_limits<int64_t>::max();
    if (expr.empty())
        return *this;
    if (expr.back() == 'B')
    {   // Interpret as a file size with unit
        const char* begin = expr.c_str();
        char* end = nullptr;
        double num = std::strtod(begin, &end);
        if (begin == end)
        {
            mError = "Could not parse numeric value in \"" + expr + "\"";
            return *this;
        }
        if (std::isnan(num) || num <= 0)
        {
            mError = "Numeric value out of range in \"" + expr + "\"";
            return *this;
        }
        std::string symbol = end;
        double value = 0;
        for (const auto& unit : units)
        {
            if (unit.symbol == symbol)
            {
                value = unit.value;
                break;
            }
        }
        if (value == 0)
        {
            mError = "Unknown file size unit: \"" + symbol + "\"";
            return *this;
        }
        mMaxLength = num * value;
        mMaxLength = std::max<int64_t>(mMaxLength, 50LL*1024);
    }
    else if (expr.find(':') != std::string::npos)
    {   // Interpret as hh:mm:ss or hh:mm
        auto parts = StringUtils::Split(":", expr);
        if (parts.size() > 3)
        {
            mError = "Too many ':' characters in expression";
            return *this;
        }
        for (const auto& part : parts)
        {
            for (const auto c : part)
            {
                if (!std::isdigit(c))
                {
                    mError = "Unexpected characters in expression: \"" + part + "\"";
                    return *this;
                }
            }
        }
        std::string hrs = parts[0], mins = parts[1], secs = "0";
        if (parts.size() == 3)
            secs = parts[2];
        mMaxDuration = TimeUtils::TimeInterval::Hours(::atoi(hrs.c_str()))
                     + TimeUtils::TimeInterval::Minutes(::atoi(mins.c_str()))
                     + TimeUtils::TimeInterval::Seconds(::atoi(secs.c_str()));
        mMaxDuration = std::max(mMaxDuration, TimeUtils::TimeInterval::Seconds(5));
    }
    else
    {
        mError = "Unrecognized type of expression: \"" + expr + "\"";
        return *this;
    }
    return *this;
}

bool FileSplittingCondition::Evaluate(TimeUtils::TimeInterval duration, int64_t fileLength) const
{
    if (duration >= mMaxDuration)
        return true;
    if (fileLength >= mMaxLength)
        return true;
    return false;
}

const std::string& FileSplittingCondition::Error() const
{
    return mError;
}

