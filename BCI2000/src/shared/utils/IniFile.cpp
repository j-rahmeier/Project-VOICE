////////////////////////////////////////////////////////////////////////////////
// $Id: IniFile.cpp 7888 2024-03-03 16:36:08Z mellinger $
// Author: mellinger@neurotechcenter.com
// Description: A simple INI file implementation.
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
#include "IniFile.h"

#include <sstream>
#include <fstream>
#include <limits>
#include <cmath>

static IniFile::GroupType sNullGroup;
static IniFile::ValueType sNullValue;

IniFile::IniFile(const std::string& filename)
  : mFilename(filename)
{
  std::ifstream file(mFilename, std::ios_base::binary | std::ios_base::in);
  Unserialize(file);
}

IniFile::~IniFile()
{
  std::ofstream file(mFilename, std::ios_base::binary | std::ios_base::out);
  Serialize(file);
}

IniFile::ValueType::ValueType()
{
}

IniFile::ValueType::ValueType(const std::string& s)
  : mStorage(s)
{
}

IniFile::ValueType::ValueType(double d)
{
  if (std::isnan(d))
    mStorage = "";
  else {
    std::ostringstream oss;
    oss.imbue(std::locale("C"));
    oss << d;
    mStorage = oss.str();
  }
}

const std::string& IniFile::ValueType::ToString() const
{
  return mStorage;
}

IniFile::ValueType::operator const std::string& () const
{
  return ToString();
}

double IniFile::ValueType::ToDouble() const
{
  std::istringstream iss(mStorage);
  iss.imbue(std::locale("C"));
  double d;
  if (iss >> d)
    return d;
  return std::numeric_limits<double>::quiet_NaN();
}

IniFile::ValueType::operator double() const
{
  return ToDouble();
}

bool IniFile::ValueType::Empty() const
{
  return mStorage.empty();
}

bool IniFile::GroupType::Empty() const
{
  return mStorage.empty();
}

const IniFile::ValueType& IniFile::GroupType::Value(const std::string& s) const
{
  auto i = mStorage.find(s);
  if (i == mStorage.end())
    return sNullValue;
  return i->second;
}

IniFile::ValueType& IniFile::GroupType::Value(const std::string& s)
{
  auto i = mStorage.find(s);
  if (i == mStorage.end())
    i = mStorage.insert(std::make_pair(s, IniFile::ValueType())).first;
  return i->second;
}

const IniFile::ValueType& IniFile::GroupType::operator[](const std::string& s) const
{
  return Value(s);
}

IniFile::ValueType& IniFile::GroupType::operator[](const std::string& s)
{
  return Value(s);
}

std::ostream& IniFile::GroupType::Serialize(std::ostream& os) const
{
  for (const auto& pair : mStorage)
    os << pair.first << "=" << pair.second.ToString() << "\n";
  return os;
}

std::istream& IniFile::GroupType::Unserialize(std::istream& is)
{
  mStorage.clear();
  while (is && is.peek() != '[') {
    std::string key, value;
    std::getline(is, key, '=');
    std::getline(is, value, '\n');
    if (!key.empty())
      mStorage.insert(std::make_pair(key, value));
  }
  return is;
}

const IniFile::GroupType& IniFile::Group(const std::string& s) const
{
  auto i = mStorage.find(s);
  if (i == mStorage.end())
    return sNullGroup;
  return i->second;
}

IniFile::GroupType& IniFile::Group(const std::string& s)
{
  auto i = mStorage.find(s);
  if (i == mStorage.end())
    i = mStorage.insert(std::make_pair(s, IniFile::GroupType())).first;
  return i->second;
}

const IniFile::GroupType& IniFile::operator[](const std::string& s) const
{
  return Group(s);
}

IniFile::GroupType& IniFile::operator[](const std::string& s)
{
  return Group(s);
}

std::ostream& IniFile::Serialize(std::ostream& os) const
{
  for (const auto& pair : mStorage) {
    os << "[" << pair.first << "]\n";
    pair.second.Serialize(os);
  }
  return os;
}

std::istream& IniFile::Unserialize(std::istream& is)
{
  mStorage.clear();
  while(is) {
    std::string key, ignore;
    std::getline(is, ignore, '[');
    std::getline(is, key, ']');
    std::getline(is, ignore, '\n');
    if (!key.empty()) {
      GroupType group;
      group.Unserialize(is);
      mStorage.insert(std::make_pair(key, group));
    }
  }
  return is;
}

