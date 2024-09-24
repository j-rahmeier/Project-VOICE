////////////////////////////////////////////////////////////////////////////////
// $Id: Rectify.h 7195 2023-02-07 18:31:03Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: Rectification functions
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
#ifndef RECTIFY_H
#define RECTIFY_H

#include <algorithm>
#include <numeric>
#include <valarray>

namespace Rectify
{
template <typename T> T HalfWaveRectify(const T &val);
template <typename T> void HalfWaveRectify(const std::valarray<T> &, std::valarray<T> &);
template <typename T> void HalfWaveRectify(const T &, T &);
template <typename T> T FullWaveRectify(const T &val);
template <typename T> void FullWaveRectify(const std::valarray<T> &, std::valarray<T> &);
template <typename T> void FullWaveRectify(const T &, T &);
} // namespace Rectify

// Implementation
template <typename T> T Rectify::HalfWaveRectify(const T &val)
{
    return (val < 0.0) ? 0.0 : val;
}

template <typename T> void Rectify::HalfWaveRectify(const std::valarray<T> &inData, std::valarray<T> &outResult)
{
    if (outResult.size() != inData.size())
        outResult.resize(inData.size(), 0.0);
    for (size_t i = 0; i < inData.size(); i++)
        outResult = inData.apply(Rectify::HalfWaveRectify);
}

template <typename T> void Rectify::HalfWaveRectify(const T &inData, T &outResult)
{
    for (int chan = 0; chan < inData.Channels(); chan++)
        for (int elem = 0; elem < inData.Elements(); elem++)
            outResult(chan, elem) = HalfWaveRectify(inData(chan, elem));
}

template <typename T> T Rectify::FullWaveRectify(const T &val)
{
    return fabs(val);
}

template <typename T> void Rectify::FullWaveRectify(const std::valarray<T> &inData, std::valarray<T> &outResult)
{
    if (outResult.size() != inData.size())
        outResult.resize(inData.size(), 0.0);
    if (inData.size() > 0)
        outResult = inData.apply(Rectify::FullWaveRectify);
}

template <typename T> void Rectify::FullWaveRectify(const T &inData, T &outResult)
{
    for (int chan = 0; chan < inData.Channels(); chan++)
        for (int elem = 0; elem < inData.Elements(); elem++)
            outResult(chan, elem) = FullWaveRectify(inData(chan, elem));
}

#endif // RECTIFY_H
