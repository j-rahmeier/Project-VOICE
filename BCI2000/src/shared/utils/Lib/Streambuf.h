////////////////////////////////////////////////////////////////////////////////
// $Id: Streambuf.h 8120 2024-05-08 16:37:53Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Classes for buffered and unbuffered stream IO.
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
#ifndef TINY_STREAMBUF_H
#define TINY_STREAMBUF_H

#include "IOObjects.h"
#include "SockStream.h"

namespace Tiny
{

class Streambuf : public fdio::streambuf, public Lockable<std::recursive_mutex>
{
  public:
    Streambuf(char **getbuf, size_t *getbufSize, char **putbuf, size_t *putbufSize);
    ~Streambuf();

    Streambuf &Reset();

    Streambuf &PredictInputCount(std::streamsize);
    Streambuf &ClearInputPrediction();

    Streambuf &SetInput(InputObject *);
    Streambuf &SetOutput(OutputObject *);
    Streambuf &SetIO(IOObject *);
    InputObject *Input() const
    {
        return mpInput;
    }
    OutputObject *Output() const
    {
        return mpOutput;
    }

  protected:
    std::streamsize on_read(char *, std::streamsize) override;
    std::streamsize on_write(const char *, std::streamsize) override;
    std::streampos on_seek_to(std::streampos, int way) override;
    std::streamsize showmanyc() override;

  private:
    using fdio::streambuf::reset;

    InputObject *mpInput;
    OutputObject *mpOutput;
    std::streamsize mBytesToExpect;
};

class UnbufferedIO : public Streambuf
{
  public:
    UnbufferedIO()
        : Streambuf(&mpInbuf, &mInbufSize, &mpOutbuf, &mOutbufSize), mpInbuf(&mInbuf), mpOutbuf(0), mInbufSize(1),
          mOutbufSize(0)
    {
    }

  private:
    char mInbuf, *mpInbuf, *mpOutbuf;
    size_t mInbufSize, mOutbufSize;
};

class BufferedIO : public Streambuf
{
  public:
    static const size_t defSize = 4096;
    BufferedIO(size_t inbufSize = defSize, size_t outbufSize = defSize);
    ~BufferedIO();

  private:
    char *mpInbuf, *mpOutbuf;
    size_t mInbufSize, mOutbufSize;
};

Streambuf &DefaultInput();
Streambuf &DefaultOutput();

template <class T> struct Stream : public T
{
    Stream(Streambuf &s) : T(&s)
    {
    }
};
typedef Stream<std::iostream> IOStream;
typedef Stream<std::istream> IStream;
typedef Stream<std::ostream> OStream;

template <class T> struct LockingStream : public T, LockableObject::LockObject
{
    LockingStream(Streambuf &s) : T(&s), LockableObject::LockObject(&s)
    {
    }
};
typedef LockingStream<std::iostream> LockingIOStream;
typedef LockingStream<std::istream> LockingIStream;
typedef LockingStream<std::ostream> LockingOStream;

} // namespace Tiny

using Tiny::BufferedIO;
using Tiny::Streambuf;
using Tiny::UnbufferedIO;

#if 0 // name clash with Windows header
using Tiny::IOStream;
using Tiny::IStream;
using Tiny::LockingIOStream;
using Tiny::LockingIStream;
using Tiny::LockingOStream;
using Tiny::OStream;
#endif

using Tiny::DefaultInput;
using Tiny::DefaultOutput;

#endif // TINY_STREAM_IO_H
