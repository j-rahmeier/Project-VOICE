////////////////////////////////////////////////////////////////////////////////
// $Id: GraphSearch.h 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A function that searches a FilterGraph for a given type of nodes.
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
#ifndef GRAPH_SEARCH_H
#define GRAPH_SEARCH_H

#include <vector>
#include "SplitterNode.h"

namespace FilterGraph
{

struct Graph;

template <class T> std::vector<T*> search(const Graph&);
template <class T> void search(const Graph&, std::vector<T*>&);

std::string nodePath(const Graph&, const Node*);

template <class T> std::vector<T*> search(const Graph& graph)
{
    std::vector<T*> result;
    search(graph, result);
    return result;
}

template <class T> void search(const Graph& graph, std::vector<T*>& result)
{
    for (auto node : graph)
    {
        auto mnode = const_cast<Node*>(node);
        auto tnode = dynamic_cast<T*>(mnode);
        if (tnode)
            result.push_back(tnode);
        auto spnode = dynamic_cast<const SplitterNode*>(node);
        if (spnode)
        {
            search(spnode->graph1(), result);
            search(spnode->graph2(), result);
        }
    }
}

} // namespace FilterGraph

#endif // GRAPH_SEARCH_H
