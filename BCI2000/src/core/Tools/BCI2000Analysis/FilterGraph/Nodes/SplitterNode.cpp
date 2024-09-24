////////////////////////////////////////////////////////////////////////////////
// $Id: SplitterNode.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A node type that represents a "Splitter" with two subgraphs.
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
#include "SplitterNode.h"
#include "IndentingOStreambuf.h"
#include "ParallelFilterCombination.h"

namespace FilterGraph
{

struct SplitterNode::Registrar : GenericFilter::FilterRegistrar<ParallelFilterCombination>
{
    const SplitterNode *mpNode;
    Registrar(const SplitterNode *pNode, Directory::Node *pDirNode)
        : FilterRegistrar<ParallelFilterCombination>(pDirNode), mpNode(pNode)
    {
    }
    GenericFilter *NewInstance() const
    {
        GenericFilter *pFilter = FilterRegistrar<ParallelFilterCombination>::NewInstance();
        auto pParallel = dynamic_cast<ParallelFilterCombination *>(pFilter);
        Assert(pParallel);
        pParallel->SetName(mpNode->name());
        mpNode->mpFilter = pFilter;
        pParallel->Add(mpNode->mGraph1.getRegistrar(pParallel));
        pParallel->Add(mpNode->mGraph2.getRegistrar(pParallel));
        return pParallel;
    }
};

std::string SplitterNode::sType = Node::registerType("Splitter", &SplitterNode::create);

SplitterNode::SplitterNode() : Node("Splitter")
{
}

SplitterNode::SplitterNode(const std::string &name, const Graph &graph1, const Graph &graph2)
    : Node(name), mGraph1(graph1), mGraph2(graph2)
{
}

Graph &SplitterNode::graph1()
{
    return mGraph1;
}

const Graph &SplitterNode::graph1() const
{
    return mGraph1;
}

Graph &SplitterNode::graph2()
{
    return mGraph2;
}

const Graph &SplitterNode::graph2() const
{
    return mGraph2;
}

Node *SplitterNode::clone() const
{
    SplitterNode *pNode = new SplitterNode(name(), mGraph1, mGraph2);
    pNode->copyFrom(this);
    return pNode;
}

Node *SplitterNode::create()
{
    return new SplitterNode;
}

const std::string &SplitterNode::type() const
{
    return sType;
}

GenericFilter::Registrar *SplitterNode::getRegistrar(Directory::Node *pNode) const
{
    return new Registrar(this, pNode);
}

void SplitterNode::gatherPerformanceData()
{
    Node::gatherPerformanceData();
    mGraph1.gatherPerformanceData();
    mGraph2.gatherPerformanceData();
}

void SplitterNode::clearFilterInstances()
{
    Node::clearFilterInstances();
    mGraph1.clearFilterInstances();
    mGraph2.clearFilterInstances();
}

void SplitterNode::onSerialize(std::ostream &os) const
{
    Node::onSerialize(os);
    os << "{\n";
    {
        IndentingOStreambuf indent(os, 2);
        mGraph1.serialize(os);
    }
    os << "}\n{\n";
    {
        IndentingOStreambuf indent(os, 2);
        mGraph2.serialize(os);
    }
    os << "}\n";
}

void SplitterNode::onUnserialize(std::istream &is)
{
    is >> std::ws;
    if (is.peek() != '{')
        is.setstate(std::ios::failbit);
    is.ignore();
    mGraph1 = Graph::unserialize(is);
    is >> std::ws;
    if (is.peek() != '}')
        is.setstate(std::ios::failbit);
    is.ignore();
    is >> std::ws;
    if (is.peek() != '{')
        is.setstate(std::ios::failbit);
    is.ignore();
    mGraph2 = Graph::unserialize(is);
    is >> std::ws;
    if (is.peek() != '}')
        is.setstate(std::ios::failbit);
    is.ignore();
}

} // namespace FilterGraph
