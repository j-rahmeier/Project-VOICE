////////////////////////////////////////////////////////////////////////////////
// $Id: SplitterNode.h 7463 2023-06-30 14:59:17Z mellinger $
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
#ifndef SPLITTER_NODE_H
#define SPLITTER_NODE_H

#include "FilterGraph.h"
#include "FilterGraphNode.h"

namespace FilterGraph
{

class SplitterNode : public Node
{
  public:
    SplitterNode();
    SplitterNode(const std::string &name, const Graph &, const Graph &);
    Graph &graph1();
    const Graph &graph1() const;
    Graph &graph2();
    const Graph &graph2() const;
    Node *clone() const override;
    const std::string &type() const override;
    GenericFilter::Registrar *getRegistrar(Directory::Node *pNode) const override;
    void gatherPerformanceData() override;
    void clearFilterInstances() override;

  protected:
    void onSerialize(std::ostream &) const override;
    void onUnserialize(std::istream &) override;

  private:
    static Node *create();

  private:
    struct Registrar;
    Graph mGraph1, mGraph2;
    static std::string sType;
};

} // namespace FilterGraph

#endif // SPLITTER_NODE_H
