////////////////////////////////////////////////////////////////////////////////
// $Id: FilterGraphNode.cpp 7463 2023-06-30 14:59:17Z mellinger $
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
#include "FilterGraphNode.h"
#include "FilterGraph.h"

namespace FilterGraph
{

std::map<std::string, FilterGraph::Node::Creator> &FilterGraph::Node::sCreators()
{
    static std::map<std::string, FilterGraph::Node::Creator> object;
    return object;
}

void Node::copyFrom(const Node *pOther)
{
    mName = pOther->mName;
    mpFilter = pOther->mpFilter;
    mPerformanceData = pOther->mPerformanceData;
    mFramed = pOther->mFramed;
    mMarked = pOther->mMarked;
}

GenericFilter::Registrar *Node::getRegistrar(Directory::Node *) const
{
    return nullptr;
}

GenericFilter *Node::filterInstance() const
{
    return mpFilter;
}

const struct GenericFilter::PerformanceData &Node::performanceData() const
{
    return mPerformanceData;
}

void Node::gatherPerformanceData()
{
    if (mpFilter)
        mPerformanceData = mpFilter->PerformanceData();
    else
        ::memset(&mPerformanceData, 0, sizeof(mPerformanceData));
}

void Node::clearFilterInstances()
{
    mpFilter = nullptr;
}

void Node::serialize(std::ostream &os) const
{
    onSerialize(os);
}

void Node::onSerialize(std::ostream &os) const
{
    std::string type = this->type();
    os << type;
    if (mName != type)
        os << ':' << mName;
    os << "\n";
}

Node *Node::unserialize(std::istream &is)
{
    std::string s, name;
    is >> s;
    auto pos = s.find(':');
    if (pos != std::string::npos)
    {
        name = s.substr(pos + 1);
        s = s.substr(0, pos);
    }
    Node *pNode = nullptr;
    Creator creator = nullptr;
    auto i = sCreators().find(s);
    if (i != sCreators().end())
        creator = i->second;
    if (creator)
        pNode = creator();
    if (!pNode)
        is.setstate(std::ios::failbit);
    if (pNode && !name.empty())
        pNode->mName = name;
    if (pNode)
        pNode->onUnserialize(is);
    return pNode;
}

const std::string &Node::registerType(const std::string &type, Creator creator)
{
    sCreators()[type] = creator;
    return type;
}

Node::Node(const std::string &name) : mName(name), mFramed(false), mMarked(false), mpFilter(nullptr)
{
    ::memset(&mPerformanceData, 0, sizeof(mPerformanceData));
}

Node::~Node()
{
}

void Node::setName(const std::string &name)
{
    mName = name;
}

const std::string &Node::name() const
{
    return mName;
}

void Node::setFramed(bool b)
{
    mFramed = b;
}

bool Node::framed() const
{
    return mFramed;
}

void Node::setMarked(bool b)
{
    mMarked = b;
}

bool Node::marked() const
{
    return mMarked;
}

} // namespace FilterGraph
