////////////////////////////////////////////////////////////////////////////////
// $Id: MappedProperty.h 7846 2024-02-20 12:08:37Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A helper class that represents names mapped to values.
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
#ifndef MAPPED_PROPERTY_H
#define MAPPED_PROPERTY_H

#include "EncodedString.h"
#include <iostream>
#include <map>

template<class T> class MappedProperty
{
    struct Comp
    {
        bool operator()(const std::string &a, const std::string &b) const
        {
            return stricmp(a.c_str(), b.c_str()) < 0;
        }
    };
    typedef std::map<EncodedString, T, Comp> MapType;

  public:
    MappedProperty() = default;
    ~MappedProperty() = default;
    MappedProperty(const MappedProperty&) = default;
    MappedProperty& operator=(const MappedProperty&) = default;

    bool operator==(const MappedProperty&) const;

    T& operator[](const std::string &);
    const T& operator[](const std::string &) const;
    bool Exists(const std::string &) const;
    int Size() const;
    MappedProperty& Clear();

    typename MapType::const_iterator begin() const;
    typename MapType::const_iterator end() const;
    typename MapType::iterator begin();
    typename MapType::iterator end();

    // Stream I/O.
    std::ostream &InsertInto(std::ostream &) const;
    std::istream &ExtractFrom(std::istream &);

  private:
    MapType mData;
    T mDefaultValue{};
};

template<class T>
inline bool MappedProperty<T>::operator==(const MappedProperty& other) const
{
    return mData == other.mData;
}

template<class T>
inline T& MappedProperty<T>::operator[](const std::string& key)
{
    EncodedString ekey(key);
    auto i = mData.find(ekey);
    if (i == mData.end())
    {
        mData[ekey] = mDefaultValue;
        return mData[ekey];
    }
    return i->second;
}

template<class T>
inline const T& MappedProperty<T>::operator[](const std::string& key) const
{
    auto i = mData.find(EncodedString(key));
    if (i == mData.end())
        return mDefaultValue;
    return i->second;
}

template<class T>
inline bool MappedProperty<T>::Exists(const std::string& key) const
{
    return mData.find(EncodedString(key)) != mData.end();
}

template<class T>
inline int MappedProperty<T>::Size() const
{
    return mData.size();
}

template<class T>
inline MappedProperty<T>& MappedProperty<T>::Clear()
{
    mData.clear();
    mDefaultValue = T{};
    return *this;
}

template<class T>
inline typename MappedProperty<T>::MapType::const_iterator MappedProperty<T>::begin() const
{
    return mData.begin();
}

template<class T>
inline typename MappedProperty<T>::MapType::const_iterator MappedProperty<T>::end() const
{
    return mData.end();
}

template<class T>
inline typename MappedProperty<T>::MapType::iterator MappedProperty<T>::begin()
{
    return mData.begin();
}

template<class T>
inline typename MappedProperty<T>::MapType::iterator MappedProperty<T>::end()
{
    return mData.end();
}

template<class T>
inline std::ostream& MappedProperty<T>::InsertInto(std::ostream& os) const
{
    os << "{";
    for (const auto& entry : mData)
        os << entry.first << " " << entry.second << " ";
    os << "}";
    return os;
}

template<class T>
inline std::istream& MappedProperty<T>::ExtractFrom(std::istream& is)
{
    Clear();
    if ((is >> std::ws).peek() != '{')
    {
        is.setstate(std::ios::failbit);
        return is;
    }
    is.ignore();
    while (is && (is >> std::ws).peek() != '}')
    {
        EncodedString key;
        is >> key;
        T value;
        is >> value;
        if (is)
        {
            mData[key] = value;
        }
        else
        {
            is.setstate(std::ios::failbit);
            return is;
        }
    }
    if ((is >> std::ws).peek() != '}')
    {
        is.setstate(std::ios::failbit);
        return is;
    }
    is.ignore();
    return is;
}

template<class T>
inline std::ostream &operator<<(std::ostream &os, const MappedProperty<T> &mp)
{
    return mp.InsertInto(os);
}
template<class T> 
inline std::istream &operator>>(std::istream &is, MappedProperty<T> &mp)
{
    return mp.ExtractFrom(is);
}

#endif // MAPPED_PROPERTY_H

