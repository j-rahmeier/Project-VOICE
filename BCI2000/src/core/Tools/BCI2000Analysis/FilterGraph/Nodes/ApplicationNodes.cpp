////////////////////////////////////////////////////////////////////////////////
// $Id: ApplicationNodes.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Nodes representing application modules.
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
#include "ApplicationNodes.h"


namespace FilterGraph
{

std::string ApplicationNode::sType = Node::registerType("ApplicationNode", &ApplicationNode::create);

ApplicationNode::ApplicationNode() : FilterNode<ObserverWrapper<Application>, BasicApplicationNode>("Application")
{
}

Node *ApplicationNode::clone() const
{
    return create();
}

Node *ApplicationNode::create()
{
    return new ApplicationNode;
}

const std::string &ApplicationNode::type() const
{
    return sType;
}

std::string SMRApplicationNode::sType = Node::registerType("SMRApplicationNode", &SMRApplicationNode::create);

SMRApplicationNode::SMRApplicationNode() : FilterNode<ObserverWrapper<SMRApplication>, BasicApplicationNode>("SMRApplication")
{
}

Node* SMRApplicationNode::clone() const
{
    return create();
}

Node* SMRApplicationNode::create()
{
    return new SMRApplicationNode;
}

const std::string& SMRApplicationNode::type() const
{
    return sType;
}

std::string ERPApplicationNode::sType = Node::registerType("ERPApplicationNode", &ERPApplicationNode::create);

ERPApplicationNode::ERPApplicationNode() : FilterNode<ObserverWrapper<ERPApplication>, BasicApplicationNode>("ERPApplication")
{
}

Node* ERPApplicationNode::clone() const
{
    return create();
}

Node* ERPApplicationNode::create()
{
    return new ERPApplicationNode;
}

const std::string& ERPApplicationNode::type() const
{
    return sType;
}

} // namespace FilterGraph

