////////////////////////////////////////////////////////////////////////////////
// $Id: IndentingOStreambuf.h 7464 2023-06-30 15:04:08Z mellinger $
// Author: James Kanze, published on StackOverflow.com
// Description: A filtering streambuf that adds indent to a std::ostream.
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
#ifndef INDENTING_OSTREAMBUF
#define INDENTING_OSTREAMBUF

namespace Tiny
{

class IndentingOStreambuf : public std::streambuf
{
    std::streambuf *myDest;
    bool myIsAtStartOfLine;
    std::string myIndent;
    std::ostream *myOwner;

  protected:
    int overflow(int ch) override
    {
        if (myIsAtStartOfLine && ch != '\n')
        {
            myDest->sputn(myIndent.data(), myIndent.size());
        }
        myIsAtStartOfLine = ch == '\n';
        return myDest->sputc(ch);
    }

  public:
    explicit IndentingOStreambuf(std::streambuf *dest, int indent = 4)
        : myDest(dest), myIsAtStartOfLine(true), myIndent(indent, ' '), myOwner(NULL)
    {
    }
    explicit IndentingOStreambuf(std::ostream &dest, int indent = 4)
        : myDest(dest.rdbuf()), myIsAtStartOfLine(true), myIndent(indent, ' '), myOwner(&dest)
    {
        myOwner->rdbuf(this);
    }
    virtual ~IndentingOStreambuf()
    {
        if (myOwner != NULL)
        {
            myOwner->rdbuf(myDest);
        }
    }
};
} // namespace Tiny

using Tiny::IndentingOStreambuf;

#endif // INDENTING_OSTREAMBUF_H
