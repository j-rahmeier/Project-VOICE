////////////////////////////////////////////////////////////////////////////////
// $Id: DllFilterNode.h 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A node for filters loaded from a shared library.
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
#ifndef DLL_FILTER_NODE_H
#define DLL_FILTER_NODE_H

#include "FilterGraphNode.h"

namespace FilterGraph
{

class DllFilterNode : public Node
{
  public:
    DllFilterNode();
    ~DllFilterNode();

    bool setFilterPath(const std::string &);
    const std::string &filterPath() const;
    const std::string &error() const;

    Node *clone() const override;
    static Node *create();

    const std::string &type() const override;
    GenericFilter::Registrar *getRegistrar(Directory::Node *) const override;

  protected:
    void onSerialize(std::ostream &) const override;
    void onUnserialize(std::istream &) override;

  private:
    struct Private;
    Private *p;

    static std::string sType;
};

} // namespace FilterGraph

#endif // DLL_FILTER_NODE_H
