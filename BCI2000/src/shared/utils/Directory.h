////////////////////////////////////////////////////////////////////////////////
// $Id: Directory.h 7464 2023-06-30 15:04:08Z mellinger $
// Description: A mix-in class for objects that are part of a directory, such
//   as filters.
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
#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <list>
#include <string>

namespace Directory
{
class Node
{
  public:
    typedef std::list<Node *> Container;

    Node() : mpParent(0), mNameSet(false)
    {
    }
    virtual ~Node();
    Node *Parent() const
    {
        return mpParent;
    }
    void SetParent(Node *);
    const Container &Children() const
    {
        return mChildren;
    }
    Container &Children()
    {
        return mChildren;
    }

    void SetName(const std::string &);
    const std::string &Name() const;
    std::string Path() const;
    template <class T> std::string SimplifiedPath() const;

  protected:
    virtual std::string OnName() const;
    virtual void OnSetParent(Node *)
    {
    }

  private:
    Node *mpParent;
    Container mChildren;
    mutable std::string mName;
    mutable bool mNameSet;
};

class RootNode : public Node
{
    std::string OnName() const
    {
        return "";
    }
};

template <class F> bool Traverse(const Node *pNode, F &f)
{
    if (!pNode->Name().empty() && !f(pNode))
        return false;
    for (Node::Container::const_iterator i = pNode->Children().begin(); i != pNode->Children().end(); ++i)
        if (!Traverse(*i, f))
            return false;
    return true;
}

template <class F> bool Traverse(Node *pNode, F &f)
{
    if (!pNode->Name().empty() && !f(pNode))
        return false;
    for (Node::Container::iterator i = pNode->Children().begin(); i != pNode->Children().end(); ++i)
        if (!Traverse(*i, f))
            return false;
    return true;
}

template <class T> std::string Node::SimplifiedPath() const
{
    std::string path;
    for (const Node *pNode = this; pNode && pNode->Parent() != pNode; pNode = pNode->Parent())
        if (dynamic_cast<const T *>(pNode) && !pNode->Name().empty())
            path = pNode->Name() + '/' + path;
    if (!path.empty())
        path.pop_back();
    return path;
}

} // namespace Directory

#endif // DIRECTORY_H
