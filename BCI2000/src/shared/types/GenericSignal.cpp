////////////////////////////////////////////////////////////////////////////////
// $Id: GenericSignal.cpp 8039 2024-04-17 18:12:01Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: GenericSignal is the BCI2000 type representing filter input and
//              output data.
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
#include "GenericSignal.h"

#include "BinaryData.h"
#include "LengthField.h"
#include "SharedMemoryMap.h"
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>

static SharedMemoryMap &ShmContainer()
{
    static SharedMemoryMap instance;
    return instance;
}

const GenericSignal::ValueType GenericSignal::NaN = std::numeric_limits<ValueType>::quiet_NaN();

GenericSignal::GenericSignal()
{
    SetProperties(mProperties);
}

GenericSignal::~GenericSignal()
{
}

GenericSignal::GenericSignal(size_t inChannels, size_t inElements, SignalType::Type inType)
{
    SetProperties(SignalProperties(inChannels, inElements, inType));
}

GenericSignal::GenericSignal(size_t inChannels, size_t inElements, SignalType inType)
{
    SetProperties(SignalProperties(inChannels, inElements, inType));
}

GenericSignal::GenericSignal(const SignalProperties &inProperties)
{
    SetProperties(inProperties);
}

GenericSignal::GenericSignal(const SignalProperties &inProperties, ValueType inValue)
{
    SetProperties(inProperties);
    SetAllValues(inValue);
}

GenericSignal &GenericSignal::SetProperties(const SignalProperties &inSp)
{
    if (inSp.Channels() != mProperties.Channels() || inSp.Elements() != mProperties.Elements())
    {
        size_t newSize = inSp.Channels() * inSp.Elements();
        LazyArray<ValueType> newValues;
        if (mSharedMemory)
        {
            if (mSharedMemory->Protocol() == "file://")
                throw std_runtime_error << "Cannot resize shared memory if tied to a file";
            mSharedMemory = ShmContainer().New(newSize * sizeof(ValueType));
            std::shared_ptr<LazyArray<ValueType>::Memory> p(new LazyArray<ValueType>::ShmMemory(mSharedMemory, newSize));
            newValues = LazyArray<ValueType>(p);
        }
        else
        {
            newValues = LazyArray<ValueType>(newSize);
        }

        for (int ch = 0; ch < std::min(mProperties.Channels(), inSp.Channels()); ++ch)
        {
            for (int el = 0; el < std::min(mProperties.Elements(), inSp.Elements()); ++el)
                newValues[inSp.LinearIndex(ch, el)] = mValues[mProperties.LinearIndex(ch, el)];
            for (int el = mProperties.Elements(); el < inSp.Elements(); ++el)
                newValues[inSp.LinearIndex(ch, el)] = 0;
        }
        for (int ch = mProperties.Channels(); ch < inSp.Channels(); ++ch)
            for (int el = 0; el < inSp.Elements(); ++el)
                newValues[inSp.LinearIndex(ch, el)] = 0;
        mValues = newValues;
    }
    mProperties = inSp;
    return *this;
}

GenericSignal &GenericSignal::SetAllValues(GenericSignal::ValueType value)
{
    GenericSignal::ValueType *pData = mValues.Data();
    for (int i = 0; i < mValues.Count(); ++i)
        *pData++ = value;
    return *this;
}

std::ostream &GenericSignal::InsertInto(std::ostream &os) const
{
    std::string indent(static_cast<size_t>(os.width()), ' ');

    os << '\n' << indent << "SignalProperties { ";
    mProperties.InsertInto(os);
    os << '\n' << indent << "}";
    if (mSharedMemory)
        os << '\n' << indent << "SharedMemory { " << mSharedMemory->Name() << '\n' << indent << "}";
    os << std::setprecision(7);
    for (int j = 0; j < Elements(); ++j)
    {
        os << '\n' << indent;
        for (int i = 0; i < Channels(); ++i)
        {
            os << std::setw(14) << Value(i, j) << ' ';
        }
    }
    return os;
}

std::ostream &GenericSignal::Serialize(std::ostream &os) const
{
    SignalType type = Type();
    type.SetShared(!!mSharedMemory);
    type.Serialize(os);
    LengthField<2> channelsField(Channels()), elementsField(Elements());
    channelsField.Serialize(os);
    elementsField.Serialize(os);
    if (type.Shared())
    {
        std::atomic_thread_fence(std::memory_order_seq_cst);
        os.write(mSharedMemory->Name().c_str(), mSharedMemory->Name().length() + 1);
    }
    else
        for (int i = 0; i < Channels(); ++i)
            for (int j = 0; j < Elements(); ++j)
                WriteValueBinary(os, i, j);
    return os;
}

std::istream &GenericSignal::Unserialize(std::istream &is)
{
    SignalType type;
    LengthField<2> channels, elements;
    type.Unserialize(is);
    channels.Unserialize(is);
    elements.Unserialize(is);
    bool shared = type.Shared();
    type.SetShared(false);
    SetProperties(SignalProperties(channels, elements, type));
    if (shared)
    {
        std::string name;
        std::getline(is, name, '\0');
        AttachToSharedMemory(name);
        std::atomic_thread_fence(std::memory_order_seq_cst);
    }
    else
        for (int i = 0; i < Channels(); ++i)
            for (int j = 0; j < Elements(); ++j)
                ReadValueBinary(is, i, j);
    return is;
}

std::ostream &GenericSignal::WriteValueBinary(std::ostream &os, size_t i, size_t j) const
{
    switch (Type())
    {
    case SignalType::int16:
        BinaryData<int16_t, LittleEndian>(Value(i, j)).Put(os);
        break;

    case SignalType::float24:
        PutValue_float24(os, Value(i, j));
        break;

    case SignalType::float32:
        BinaryData<float, LittleEndian>(Value(i, j)).Put(os);
        break;

    case SignalType::int32:
        BinaryData<int32_t, LittleEndian>(Value(i, j)).Put(os);
        break;

    default:
        os.setstate(os.failbit);
    }
    return os;
}

std::istream &GenericSignal::ReadValueBinary(std::istream &is, size_t i, size_t j)
{
    switch (Type())
    {
    case SignalType::int16:
        Value(i, j) = BinaryData<int16_t, LittleEndian>(is);
        break;

    case SignalType::float24:
        Value(i, j) = GetValue_float24(is);
        break;

    case SignalType::float32:
        Value(i, j) = BinaryData<float, LittleEndian>(is);
        break;

    case SignalType::int32:
        Value(i, j) = BinaryData<int32_t, LittleEndian>(is);
        break;

    default:
        is.setstate(is.failbit);
    }
    return is;
}

void GenericSignal::PutValue_float24(std::ostream &os, ValueType value)
{
    int mantissa, exponent;
    if (value == 0.0)
    {
        mantissa = 0;
        exponent = 1;
    }
    else
    {
        exponent = static_cast<int>(::ceil(::log10(::fabs(value))));
        mantissa = static_cast<int>(value / ::pow(10.0, exponent)) * 10000;
        exponent -= 4;
    }
    os.put(mantissa & 0xff).put(mantissa >> 8);
    os.put(exponent & 0xff);
}

GenericSignal::ValueType GenericSignal::GetValue_float24(std::istream &is)
{
    signed short mantissa = is.get();
    mantissa |= is.get() << 8;
    signed char exponent = is.get();
    return mantissa * ::pow(10.0, exponent);
}

GenericSignal &GenericSignal::AssignFrom(const GenericSignal &s)
{
    SetProperties(s.Properties());
    AssignValues(s);
    return *this;
}

GenericSignal &GenericSignal::AssignValues(const GenericSignal &s)
{
    bool mismatch = s.Channels() != Channels() || s.Elements() != Elements();
    if (mismatch)
        SetProperties(SignalProperties(s.Channels(), s.Elements()));

    if (!mSharedMemory && !s.mSharedMemory)
        mValues.ShallowAssignFrom(s.mValues);
    else
        mValues.DeepAssignFrom(s.mValues);
    return *this;
}

GenericSignal::ValueType* GenericSignal::MutableData()
{
    return mValues.GetWritableData();
}

const GenericSignal::ValueType* GenericSignal::ConstData() const
{
    return mValues.Data();
}

size_t GenericSignal::LinearIndex(size_t ch, size_t el) const
{
    return mProperties.LinearIndex(ch, el);
}

void GenericSignal::EnsureDeepCopy()
{
    mValues.GetWritableData();
}

bool GenericSignal::ShareAcrossModules()
{
    if (!mSharedMemory && mValues.Count() != 0)
    {
        mSharedMemory = ShmContainer().New(mValues.Count() * sizeof(ValueType));
        std::shared_ptr<LazyArray<ValueType>::Memory>
            p(new LazyArray<ValueType>::ShmMemory(mSharedMemory, mValues.Count()));
        LazyArray<ValueType> newValues(p);
        newValues.DeepAssignFrom(mValues);
        mValues.ShallowAssignFrom(newValues);
    }
    return !!mSharedMemory;
}

void GenericSignal::AttachToSharedMemory(const std::string &inName)
{
    if (!mSharedMemory || mSharedMemory->Name() != inName)
    {
        mSharedMemory = ShmContainer().Get(inName);
        std::shared_ptr<LazyArray<ValueType>::Memory> p(new LazyArray<ValueType>::ShmMemory(mSharedMemory, mValues.Count()));
        mValues = LazyArray<ValueType>(p);
    }
}

// GenericChannel
GenericChannel &GenericChannel::operator=(const GenericChannel &inChannel)
{
    for (size_t i = 0; i < size(); i++)
        (*this)[i] = inChannel[i];
    return (*this);
}

// GenericElement
GenericElement &GenericElement::operator=(const GenericElement &inElement)
{
    for (size_t i = 0; i < size(); i++)
        (*this)[i] = inElement[i];
    return (*this);
}
