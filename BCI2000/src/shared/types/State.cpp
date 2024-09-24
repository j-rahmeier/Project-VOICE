////////////////////////////////////////////////////////////////////////////////
// $Id: State.cpp 7989 2024-04-03 19:54:43Z mellinger $
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
#include "State.h"

#include "BCIException.h"
#include "Debugging.h"
#include "StateVector.h"

#include <map>
#include <sstream>

// **************************************************************************
// Function:   State::NameCmp::operator()
// Purpose:    comparison function for state names
// Parameters: name string references a, b
// Returns:    true when a comes before b, false otherwise
// **************************************************************************
bool State::NameCmp::operator()(const std::string &a, const std::string &b) const
{
    return ::stricmp(a.c_str(), b.c_str()) < 0;
}

// **************************************************************************
// Function:   State
// Purpose:    the constructor for the State object
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
State::State() : mValue(0), mLocation(0), mLength(0), mKind(StateKind), mModified(false), mLocationWasAssigned(false)
{
}

// **************************************************************************
// Function:   operator==
// Purpose:    Compare to another State object.
// Parameters: Reference to second State.
// Returns:    True if equal, false otherwise.
// **************************************************************************
bool State::operator==(const State &s) const
{
    return mLocationWasAssigned && this->Name() == s.Name() && this->Location() == s.Location() &&
           this->Length() == s.Length() && this->Value() == s.Value() && this->Kind() == s.Kind();
}

bool State::FromDefinition(const std::string &inDefinition)
{
    bool ok = true;
    std::istringstream iss(inDefinition);
    if (!ExtractFrom(iss))
    {
        iss.clear();
        iss.str(inDefinition + " 0 0");
        iss.seekg(0);
        ok = !!ExtractFrom(iss);
    }
    mLocationWasAssigned = false;
    return ok;
}

// **************************************************************************
// Function:   ExtractFrom
// Purpose:    Member function for formatted stream input of a single
//             state.
//             All formatted input functions are, for consistency's sake,
//             supposed to use this function.
// Parameters: Input stream to read from.
// Returns:    Input stream read from.
// **************************************************************************
std::istream &State::ExtractFrom(std::istream &is)
{
    *this = State();
    mModified = true;
    int byteLocation = 0, bitLocation = 0;
    is >> mName >> mLength >> mValue >> byteLocation >> bitLocation;
    if (mLength > 8 * sizeof(ValueType))
        throw std_range_error << "State " << mName << ": length of " << mLength << " exceeds size of State::ValueType";
    if (bitLocation < 0)
    {
        mKind = -(bitLocation + 1);
        mLocationWasAssigned = false;
        mLocation = 0;
    }
    else
    {
        mKind = StateKind;
        mLocationWasAssigned = true;
        mLocation = byteLocation * 8 + bitLocation;
    }
    return is;
}

State &State::SetKind(int k)
{
    mKind = k;
    return *this;
}

State &State::SetLocation(size_t location)
{
    mLocation = location;
    mLocationWasAssigned = true;
    return *this;
}

// **************************************************************************
// Function:   InsertInto
// Purpose:    Member function for formatted stream output of a single
//             state.
//             All formatted output functions are, for consistency's sake,
//             supposed to use this function.
// Parameters: Output stream to write into.
// Returns:    Output stream written into.
// **************************************************************************
std::ostream &State::InsertInto(std::ostream &os) const
{
    size_t byteLocation = mLocation / 8, bitLocation = mLocation % 8;
    os << mName << " " << mLength << " " << mValue << " ";
    if (mLocationWasAssigned)
        os << byteLocation << " " << bitLocation;
    else
        os << "0 " << -(mKind + 1);
    return os;
}


// **************************************************************************
// Function:   Unserialize
// Purpose:    Member function for input of a single
//             state from a binary stream, as in a state message.
// Parameters: Input stream to read from.
// Returns:    N/A
// **************************************************************************
std::istream &State::Unserialize(std::istream &is)
{
    std::string line;
    if (std::getline(is, line, '\n'))
    {
        std::istringstream linestream(line);
        ExtractFrom(linestream);
        if (!linestream)
            is.setstate(std::ios::failbit);
    }
    return is;
}

// **************************************************************************
// Function:   Serialize
// Purpose:    Member function for output of a single
//             state into a binary stream, as in a state message.
// Parameters: Output stream to write into.
// Returns:    N/A
// **************************************************************************
std::ostream &State::Serialize(std::ostream &os) const
{
    InsertInto(os);
    return os.write("\r\n", 2);
}

// **************************************************************************
// Function:   SetValue
// Purpose:    Sets this state's value
// Parameters: value
// Returns:    N/A
// **************************************************************************
State &State::SetValue(State::ValueType inValue)
{
    mValue = inValue;
    mModified = true;
    return *this;
}

// **************************************************************************
// Function:   State::Commit
// Purpose:    if the write cache has been modified, write its content into
//             the given state vector and clear the "modified" flag
// Parameters: stateVector - the state vector that calls this function
// Returns:    N/A
// **************************************************************************
void State::Commit(StateVector *stateVector) const
{
    if (mModified)
    {
        Assert(stateVector != nullptr);
        stateVector->SetStateValue(mLocation, mLength, mValue);
        mModified = false;
    }
}

// State2 is a helper class that provides extended state information when it serializes itself.
// Otherwise, a State2 object behaves like a State object.
State2::State2()
{
    SetKind(0);
    SetLength(0);
    SetValue(0);
    SetLocation(0);
    SetLocationWasAssigned(false);
}

State2::State2(const State& s)
: State(s)
{
}

State2& State2::operator=(const State& s)
{
    State::operator=(s);
    return *this;
}

// **************************************************************************
// Function:   InsertInto
// Purpose:    Member function for formatted stream output of a single
//             state.
//             All formatted output functions are, for consistency's sake,
//             supposed to use this function.
// Parameters: Output stream to write into.
// Returns:    Output stream written into.
// **************************************************************************
std::ostream& State2::InsertInto(std::ostream& os) const
{
    os << Name() << " " << Kind() << " " << Length() << " " << Value() << " ";
    if (LocationWasAssigned())
        os << Location();
    else
        os << -1;
    return os;
}

// **************************************************************************
// Function:   ExtractFrom
// Purpose:    Member function for formatted stream input of a single
//             state.
//             All formatted input functions are, for consistency's sake,
//             supposed to use this function.
// Parameters: Input stream to read from.
// Returns:    Input stream read from.
// **************************************************************************
std::istream& State2::ExtractFrom(std::istream& is)
{
    *this = State2();
    int length = 0, kind = 0, location = 0;
    State::ValueType value = 0;
    std::string name;
    is >> name >> kind >> length >> value >> location;
    if (length > 8 * sizeof(ValueType))
        throw std_range_error << "State " << name << ": length of " << length << " exceeds size of State::ValueType";
    SetName(name).SetKind(kind);
    SetLength(length).SetValue(value);
    SetLocation(location);
    SetLocationWasAssigned(location >= 0);
    SetModified(true);
    return is;
}

// **************************************************************************
// Function:   Serialize
// Purpose:    Member function for output of a single
//             state into a binary stream, as in a state message.
// Parameters: Output stream to write into.
// Returns:    N/A
// **************************************************************************
std::ostream& State2::Serialize(std::ostream& os) const
{
    State2::InsertInto(os);
    return os.write("\r\n", 2);
}

// **************************************************************************
// Function:   Unserialize
// Purpose:    Member function for input of a single
//             state from a binary stream, as in a state message.
// Parameters: Input stream to read from.
// Returns:    N/A
// **************************************************************************
std::istream& State2::Unserialize(std::istream& is)
{
    std::string line;
    if (std::getline(is, line, '\n'))
    {
        std::istringstream linestream(line);
        State2::ExtractFrom(linestream);
        if (!linestream)
            is.setstate(std::ios::failbit);
    }
    return is;
}
