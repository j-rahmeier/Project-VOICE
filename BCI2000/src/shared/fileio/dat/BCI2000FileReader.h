////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000FileReader.h 7195 2023-02-07 18:31:03Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: Class that provides an interface to the data stored in a
//              BCI2000 data file.
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
#ifndef BCI2000_FILE_READER_H
#define BCI2000_FILE_READER_H

#include "GenericSignal.h"
#include "Param.h"
#include "ParamList.h"
#include "ParamRef.h"
#include "State.h"
#include "StateList.h"
#include "StateRef.h"
#include "StateVector.h"

#include <fstream>
#include <string>
#include <vector>

class BCI2000FileReader
{
  public:
    static const int cDefaultBufSize = 50 * 1024;

    enum
    {
        NoError = 0,
        FileOpenError,
        MalformedHeader,

        NumErrors
    };

  public:
    BCI2000FileReader();
    explicit BCI2000FileReader(const char *fileName);
    explicit BCI2000FileReader(const std::string& fileName);
    explicit BCI2000FileReader(int fd);
    virtual ~BCI2000FileReader();

    BCI2000FileReader(const BCI2000FileReader &) = delete;
    BCI2000FileReader &operator=(const BCI2000FileReader &) = delete;

    // State
    virtual int ErrorState() const;
    virtual bool IsOpen() const;

    // File access
    BCI2000FileReader &Open(const std::string& fileName, int bufferSize = cDefaultBufSize, const std::string& prm = "");
    BCI2000FileReader& Open(int fd, int bufferSize, const std::string& prm);
    BCI2000FileReader& Close();

    BCI2000FileReader &Open(const char *fileName, int bufferSize = cDefaultBufSize, const char *prm = nullptr);
    BCI2000FileReader &Open(int fd, int bufferSize = cDefaultBufSize, const char* prm = nullptr);

    virtual int64_t NumSamples() const;
    double SamplingRate() const;
    const class SignalProperties &SignalProperties() const;
    const std::string &FileFormatVersion() const;

    // Parameter/State access
    //  Accessor functions consistent with the Environment class interface.
    const ParamList *Parameters() const;
    const StateList *States() const;
    const class StateVector *StateVector() const;
    ParamRef Parameter(const std::string &name) const;
    const StateRef State(const std::string &name) const;

    int HeaderLength() const;
    int StateVectorLength() const;
    int SourceCh() const;
    SignalType DataFormat() const;

    // Data access
    virtual GenericSignal::ValueType RawValue(int channel, int64_t sample);
    GenericSignal::ValueType CalibratedValue(int channel, int64_t sample);
    virtual BCI2000FileReader &ReadStateVector(int64_t sample);

  protected:
    void Reset();

  private:
    struct Private;
    Private *p;
};

#endif // BCI2000_FILE_READER_H
