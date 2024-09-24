////////////////////////////////////////////////////////////////////////////////
// $Id: FilterGraphNode.h 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Base class for nodes in a BCI2000 filter graph.
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
#ifndef FILTER_GRAPH_NODE_H
#define FILTER_GRAPH_NODE_H

#include "GenericFilter.h"
#include <iostream>
#include <string>

namespace FilterGraph
{

class Node
{
  public:
    Node(const std::string &name = "");
    virtual ~Node();

    void setName(const std::string &);
    const std::string &name() const;

    void setFramed(bool);
    bool framed() const;

    void setMarked(bool);
    bool marked() const;

    virtual Node *clone() const = 0;
    virtual GenericFilter::Registrar *getRegistrar(Directory::Node *) const;
    GenericFilter *filterInstance() const;
    const struct GenericFilter::PerformanceData &performanceData() const;
    virtual void gatherPerformanceData();
    virtual void clearFilterInstances();

  public:
    void serialize(std::ostream &) const;
    static Node *unserialize(std::istream &);

  protected:
    virtual void copyFrom(const Node *);
    virtual void onSerialize(std::ostream &) const;
    virtual void onUnserialize(std::istream &) {}

    virtual const std::string &type() const = 0;
    typedef Node *(*Creator)();
    static std::map<std::string, Creator> &sCreators();
    static const std::string &registerType(const std::string &type, Creator);

  protected:
    mutable GenericFilter *mpFilter;
    mutable struct GenericFilter::PerformanceData mPerformanceData;

  private:
    std::string mName;
    bool mFramed, mMarked;
};

} // namespace FilterGraph

#endif // FILTER_GRAPH_NODE_H
