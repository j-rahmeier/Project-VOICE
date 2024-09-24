////////////////////////////////////////////////////////////////////////////////
// $Id $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A message channel that is a /dev/null-like sink.
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
#ifndef BCI_NULL_MESSAGE_CHANNEL_H
#define BCI_NULL_MESSAGE_CHANNEL_H

#include "MessageChannel.h"

namespace bci
{
class NullMessageChannel : public MessageChannel
{
  public:
    NullMessageChannel() : MessageChannel(mNullStream)
    {
        mNullStream.SetOutput(&mNullOutput);
    }
    ~NullMessageChannel()
    {
        mNullStream.SetOutput(nullptr);
    }

  private:
    struct : Tiny::OutputObject
    {
        int64_t OnWrite(const char *, int64_t n) override
        {
            return n;
        }
        int64_t OnSeekTo(int64_t p, int) override
        {
            return p;
        }
        bool OnLock() const override
        {
            return true;
        }
        bool OnUnlock() const override
        {
            return true;
        }
        bool OnTryLock() const override
        {
            return true;
        }
    } mNullOutput;
    Tiny::UnbufferedIO mNullStream;
};

} // namespace bci

using bci::NullMessageChannel;

#endif // BCI_NULL_MESSAGE_CHANNEL_H
