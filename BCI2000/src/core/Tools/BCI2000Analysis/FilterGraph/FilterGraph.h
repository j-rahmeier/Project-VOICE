////////////////////////////////////////////////////////////////////////////////
// $Id: FilterGraph.h 8215 2024-06-21 13:36:37Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: An abstract representation for the graph formed by BCI2000
//   filters in an analysis setup.
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
#ifndef FILTER_GRAPH_H
#define FILTER_GRAPH_H

#include <iostream>
#include <string>
#include <vector>

#include "Directory.h"
#include "GenericFilter.h"

namespace FilterGraph
{

class Node;

class Graph
{
  public:
    struct iterator
    {
        Graph *graph;
        size_t index;
        iterator();
        iterator(Graph *, size_t);
        iterator(const iterator &);
        operator void *() const;
        bool operator==(const iterator &other) const;
        bool operator!=(const iterator &other) const;
        Node *operator*() const;
        iterator &operator++();
        iterator operator++(int);
    };

    struct const_iterator
    {
        const Graph *graph;
        size_t index;
        const_iterator();
        const_iterator(const Graph *, size_t);
        const_iterator(const iterator &);
        const_iterator(const const_iterator &);
        operator const void *() const;
        bool operator==(const const_iterator &other) const;
        bool operator!=(const const_iterator &other) const;
        const Node *operator*() const;
        const_iterator &operator++();
        const_iterator operator++(int);
    };

    Graph(const std::string &name = "");
    Graph(const Graph &other);
    Graph &operator=(const Graph &other);
    ~Graph();

    bool operator==(const Graph &) const;
    bool operator!=(const Graph &) const;

    void setName(const std::string &);
    const std::string &name() const;
    void cloneFrom(const Graph &other);

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    bool empty() const;
    void clear();
    void erase(size_t pos);
    void insert(size_t pos, FilterGraph::Node *pNode);
    void push_back(Node *);

    void serialize(std::ostream &) const;
    static Graph unserialize(std::istream &);

    GenericFilter::Registrar *getRegistrar(Directory::Node *pNode) const;
    GenericFilter *filterInstance() const;
    GenericFilter *instantiateAsFilter(Directory::Node * = nullptr) const;
    bool isRunnable() const;
    const struct GenericFilter::PerformanceData &performanceData() const;
    void gatherPerformanceData();
    void clearFilterInstances();

  private:
    struct Registrar;
    friend struct Registrar;
    std::vector<Node *> mNodes;
    std::string mName;
    mutable GenericFilter *mpFilter;
    mutable struct GenericFilter::PerformanceData mPerformanceData;
};

} // namespace FilterGraph

#endif // FILTER_GRAPH_H
