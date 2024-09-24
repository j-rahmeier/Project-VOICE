////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: A node for placeholders (useful when creating PipeDefinition.cpp
//   with filters that are not compiled as DLLs).
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
#include "PlaceholderNode.h"
#include "FilterGraphNode.h"

namespace FilterGraph
{

static const char *sRegisteredName = "PlaceholderNode";
std::string PlaceholderNode::sType = Node::registerType(sRegisteredName, &PlaceholderNode::create);

PlaceholderNode::PlaceholderNode() : Node(sRegisteredName)
{
}

PlaceholderNode::~PlaceholderNode()
{
}

Node *PlaceholderNode::clone() const
{
    PlaceholderNode *pNode = new PlaceholderNode;
    pNode->copyFrom(this);
    return pNode;
}

Node *PlaceholderNode::create()
{
    return new PlaceholderNode;
}

const std::string &PlaceholderNode::type() const
{
    return sType;
}

GenericFilter::Registrar *PlaceholderNode::getRegistrar(Directory::Node *) const
{
    return nullptr;
}

} // namespace FilterGraph
