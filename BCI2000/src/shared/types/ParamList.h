////////////////////////////////////////////////////////////////////////////////
// $Id: ParamList.h 8291 2024-07-30 16:17:34Z mellinger $
// Authors: gschalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: ParamList is a BCI2000 type that represents a collection of
//   parameters.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
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
#ifndef PARAM_LIST_H
#define PARAM_LIST_H

#include "EncodedString.h"
#include "Param.h"
#include <map>
#include <string>
#include <vector>

class ParamRef;
class MutableParamRef;

class ParamList
{
  public:
    ParamList();
    ~ParamList();
    ParamList(const ParamList &);
    ParamList &operator=(const ParamList &);

#if 0
  const Param&  operator[]( const std::string& name ) const
    { return ByName( name ); }
  Param&  operator[]( const std::string& name )
    { return ByName( name ); }
  const Param&  operator[]( size_t index ) const
    { return ByIndex( index ); }
  Param&  operator[]( size_t index )
    { return ByIndex( index ); }
#endif

    MutableParamRef operator()(const std::string &name);
    ParamRef operator()(const std::string &name) const;

    int Size() const;
    bool Empty() const;
    void Clear();

    bool Exists(const std::string &path) const;

    const Param &ByPath(const std::string &path) const;
    Param &ByPath(const std::string &path);
    const Param &ByIndex(size_t index) const;
    Param &ByIndex(size_t index);

    std::string OriginalPath(const Param *) const;

    void Add(const Param &p, float sortingHint = 0.0);
    void Add(const Param &p, int sortingHint)
    {
        Add(p, static_cast<float>(sortingHint));
    }
    bool Add(const std::string &paramDefinition);
    bool Delete(const std::string &name);

    bool Save(const std::string &filename) const;
    bool Load(const std::string &filename, bool importNonexisting = true);

    bool CopyFrom(const ParamList&, bool importNonexisting = true);
    void Join(const ParamList&);

    void Sort();
    void Prune();
    void Unchanged();

    // These contain all formatted I/O functionality.
    std::ostream &InsertInto(std::ostream &) const;
    std::istream &ExtractFrom(std::istream &);

    // These define binary I/O.
    std::ostream &Serialize(std::ostream &) const;
    std::istream &Unserialize(std::istream &);

  private:
    // Range-based for loop support
    struct iterator
    {
      iterator(ParamList* p, int i) : p(p), i(i)
      {
      }
      void operator++()
      {
        ++i;
      }
      Param& operator*() const
      {
        return p->ByIndex(i);
      }
      bool operator!=(iterator other)
      {
        return i != other.i;
      }
      ParamList* const p;
      int i;
    };

    struct const_iterator
    {
        const_iterator(const ParamList *p, int i) : p(p), i(i)
        {
        }
        void operator++()
        {
            ++i;
        }
        const Param &operator*() const
        {
            return p->ByIndex(i);
        }
        bool operator!=(const_iterator other)
        {
            return i != other.i;
        }
        const ParamList *const p;
        int i;
    };

  public:
    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

  private:
    struct Private;
    Private *p;
};

inline std::ostream &operator<<(std::ostream &s, const ParamList &p)
{
    return p.InsertInto(s);
}

inline std::istream &operator>>(std::istream &s, ParamList &p)
{
    return p.ExtractFrom(s);
}

#endif // PARAM_LIST_H
