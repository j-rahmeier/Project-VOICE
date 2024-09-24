////////////////////////////////////////////////////////////////////////////////
// $Id: State.h 7985 2024-04-03 11:07:24Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: BCI2000 type for system states (event markers).
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
#ifndef STATE_H
#define STATE_H

#include <cstdint>
#include <iostream>
#include <string>

class StateVector;
class StateList;
class CoreModule;

class State
{
    friend class StateList;
    friend class StateVector;

  public:
    typedef uint32_t ValueType; // must be an unsigned type

    enum
    {
        Padding = 0,
        StateKind,  // part of system state
        EventKind,  // event recording
        StreamKind, // data recording, e.g. digital inputs

        firstKind = StateKind,
        lastKind = StreamKind
    };

  public:
    State();
    ~State()
    {
    }

    bool operator==(const State &) const;
    bool operator!=(const State &s) const
    {
        return !operator==(s);
    }

    bool FromDefinition(const std::string &);

    const std::string &Name() const
    {
        return mName;
    }
    int Kind() const
    {
        return mKind;
    }
    State &SetKind(int k);
    int Location() const
    {
        return static_cast<int>(mLocation);
    }
    int Length() const
    {
        return static_cast<int>(mLength);
    }

    ValueType Value() const
    {
        return mValue;
    }
    State &SetValue(ValueType);

    std::ostream &InsertInto(std::ostream &) const;
    std::istream &ExtractFrom(std::istream &);
    std::ostream &Serialize(std::ostream &) const;
    std::istream &Unserialize(std::istream &);

    class NameCmp
    {
      public:
        bool operator()(const std::string &, const std::string &) const;
    };

  protected:
    State &SetName(const std::string &s)
    {
        mName = s;
        return *this;
    }
    State &SetLocation(size_t location);
    State &SetLength(size_t length)
    {
        mLength = length;
        return *this;
    }
    State& SetModified(bool b)
    {
        mModified = true;
        return *this;
    }
    State& SetLocationWasAssigned(bool b)
    {
        mLocationWasAssigned = b;
        return *this;
    }
    bool LocationWasAssigned() const
    {
        return mLocationWasAssigned;
    }

    void Commit(StateVector *) const;

  private:
    std::string mName;
    ValueType mValue;
    size_t mLocation, mLength;
    int mKind;
    bool mLocationWasAssigned;
    mutable bool mModified;
};

// State2 is a helper class that provides extended state information when it serializes itself.
// Otherwise, a State2 object behaves like a State object.
class State2 : public State
{
public:
    State2();
    State2(const State& s);
    State2& operator=(const State&);

    // Avoid accidentally creating a pointer that might be converted to State* (slicing).
    State2* operator&() = delete;
    const State2* operator&() const = delete;

    std::ostream& InsertInto(std::ostream&) const;
    std::istream& ExtractFrom(std::istream&);
    std::ostream& Serialize(std::ostream&) const;
    std::istream& Unserialize(std::istream&);
};

inline std::ostream &operator<<(std::ostream &os, const State &s)
{
    return s.InsertInto(os);
}

inline std::istream &operator>>(std::istream &is, State &s)
{
    return s.ExtractFrom(is);
}

#endif // STATE_H
