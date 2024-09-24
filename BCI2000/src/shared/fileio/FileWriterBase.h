////////////////////////////////////////////////////////////////////////////////
// $Id: FileWriterBase.h 7437 2023-06-22 17:09:12Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A base class that implements functionality common to all
//              file writer classes that output into a file.
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
#ifndef FILE_WRITER_BASE_H
#define FILE_WRITER_BASE_H

#include "FileSplittingCondition.h"
#include "Files.h"
#include "GenericFileWriter.h"
#include "GenericOutputFormat.h"
#include "Streambuf.h"
#include "SynchronizedQueue.h"
#include "Thread.h"

#include <iostream>
#include <string>
#include <mutex>

class FileWriterBase : public GenericFileWriter, Thread
{
  protected:
    FileWriterBase(GenericOutputFormat &);

  public:
    virtual ~FileWriterBase();
    void Publish() override;
    void Preflight(const SignalProperties &Input, SignalProperties &Output) const override;
    void Initialize(const SignalProperties &Input, const SignalProperties &Output) override;
    void StartRun() override;
    void StopRun() override;
    void Halt() override;
    void Write(const GenericSignal &Signal, const StateVector &Statevector) override;

    void BeginFile();
    void EndFile();

  private:
    int OnExecute() override;

    GenericOutputFormat &mrOutputFormat;
    BufferedIO mStreambuf;
    std::ostream mOutputFile;
    typedef SynchronizedQueue<std::pair<GenericSignal, StateVector>> Queue;
    Queue mQueue;

    FileSplittingCondition mFileSplittingCondition;
    int mFilePart;

    ParamList mParameters;
    StateList mStates;
    std::mutex mMutex;
};

#endif // FILE_WRITER_BASE_H
