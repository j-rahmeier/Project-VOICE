////////////////////////////////////////////////////////////////////////////////
// $Id: GraphSearch.cpp 7463 2023-06-30 14:59:17Z mellinger $
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
#include "GraphSearch.h"

#include <string>
#include <stack>

namespace FilterGraph
{

static bool nodePath(const Graph&, const Node*, std::stack<std::string>&);

std::string nodePath(const Graph& inGraph, const Node* inpNode)
{
    std::stack<std::string> names;
    nodePath(inGraph, inpNode, names);
    std::string result;
    while (!names.empty()) {
        if (!names.top().empty())
            result = names.top() + "/" + result;
        names.pop();
    }
    result += inpNode->name();
    return result;
}

bool nodePath(const Graph& inGraph, const Node* inpNode, std::stack<std::string>& ioNames)
{
    ioNames.push(inGraph.name());
    for (auto pNode : inGraph)
    {
        if (pNode == inpNode)
            return true;
        auto pSplitterNode = dynamic_cast<const SplitterNode*>(pNode);
        if (pSplitterNode)
        {
            ioNames.push(pSplitterNode->name());
            if (nodePath(pSplitterNode->graph1(), inpNode, ioNames))
                return true;
            if (nodePath(pSplitterNode->graph2(), inpNode, ioNames))
                return true;
            ioNames.pop();
        }
    }
    ioNames.pop();
    return false;
}

} // namespace FilterGraph
