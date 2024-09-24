////////////////////////////////////////////////////////////////////////////////
// $Id: Directory.cpp 7464 2023-06-30 15:04:08Z mellinger $
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
#include "Directory.h"
#include "ClassName.h"
#include <sstream>

Directory::Node::~Node()
{
    SetParent(0);
    for (Container::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
        (*i)->SetParent(0);
}

void Directory::Node::SetParent(Node *p)
{
    OnSetParent(p);
    if (mpParent)
        mpParent->Children().remove(this);
    mpParent = p;
    if (p)
        p->Children().push_front(this);
}

std::string Directory::Node::OnName() const
{
    return ClassName(typeid(*this));
}

void Directory::Node::SetName(const std::string &inName)
{
    mName = inName;
    mNameSet = true;
}

const std::string &Directory::Node::Name() const
{
    if (!mNameSet)
    {
        mNameSet = true;
        int idx = 1;
        mName = OnName();
        Node *pParent = mpParent;
        while (pParent)
        {
            bool found = false;
            Container::const_iterator i = pParent->Children().begin();
            while (!found && i != pParent->Children().end())
            {
                if (*i != this)
                    found = (mName == (*i)->Name());
                ++i;
            }
            if (found)
            {
                std::ostringstream oss;
                oss << OnName() << '_' << ++idx;
                mName = oss.str();
            }
            else
                pParent = 0;
        }
    }
    return mName;
}

std::string Directory::Node::Path() const
{
    const char sep = '/';
    std::string path;
    if (mpParent)
        path = mpParent->Path();
    if (path.empty() || *path.rbegin() != sep)
        path += sep;
    return path + Name();
}
