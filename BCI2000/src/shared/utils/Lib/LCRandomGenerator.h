////////////////////////////////////////////////////////////////////////////////
// $Id: LCRandomGenerator.h 7464 2023-06-30 15:04:08Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A class that encapsulates a Linear Congruential Pseudo Random
//   number generator.
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
#ifndef TINY_LC_RANDOM_GENERATOR_H
#define TINY_LC_RANDOM_GENERATOR_H

#include <cstdint>
#include <string>

namespace Tiny
{

class LCRandomGenerator
{
  public:
    typedef uint32_t SeedType;
    typedef uint32_t NumberType;

    LCRandomGenerator() : mSeed(DefaultSeed())
    {
    }
    LCRandomGenerator(SeedType s) : mSeed(s)
    {
    }
    SeedType DefaultSeed();
    // Properties
    SeedType Seed() const
    {
        return mSeed;
    }
    LCRandomGenerator &SetSeed(SeedType s)
    {
        mSeed = s;
        return *this;
    }

    static NumberType RandMax();
    // A random integer between 0 and including RandMax().
    NumberType Random();
    // A random integer between 0 and (N-1).
    NumberType Random(NumberType N)
    {
        return (Random() * N) / (RandMax() + 1);
    }

    // STL functor interface: operator() returns a random integer between 0 and (N-1).
    template <typename Int> Int operator()(Int inN)
    {
        return Random(inN);
    }

    // STL Uniform Random Bit Generator interface: operator() returns a random integer between 0 and RandMax().
    typedef NumberType result_type;
    static constexpr result_type min() { return 0; }
    static constexpr result_type max() { return ~NumberType(0); }
    result_type operator()() { return Random(); }

    struct Uniform01
    {
        static double InverseCPDF(double x)
        {
            return x;
        }
    };
    struct Normal
    {
        static double InverseCPDF(double);
    };
    template <class Dist> double RandomValue()
    {
        return Dist::InverseCPDF(Random() * 1.0 / RandMax());
    }

    std::string RandomName(size_t inLength);
    char RandomCharacter(int (*)(int) = &::isalpha);

  private:
    static NumberType NumberFromSeed(SeedType);

    SeedType mSeed;
};

} // namespace Tiny

using Tiny::LCRandomGenerator;

#endif // TINY_LC_RANDOM_GENERATOR_H
