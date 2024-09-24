////////////////////////////////////////////////////////////////////////////////
// $Id: EDFOutputBase.h 7437 2023-06-22 17:09:12Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A base class for EDF/GDF type output formats.
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
#ifndef EDF_OUTPUT_BASE_H
#define EDF_OUTPUT_BASE_H

#include "EDFHeader.h"
#include "GenericOutputFormat.h"

class EDFOutputBase : public GenericOutputFormat
{
  protected: // No instantiation outside derived classes.
    EDFOutputBase();

  public:
    virtual ~EDFOutputBase();

    void Publish() const override;
    void Preflight(const SignalProperties &, const StateVector &) const override;
    void Initialize(const SignalProperties &, const StateVector &) override;
    void BeginFile(std::ostream&, const ParamList&, const StateList&) override;
    void EndFile(std::ostream &) override;
    void Write(std::ostream &, const GenericSignal &, const StateVector &) override;

    const char *DataFileExtension() const override = 0;

  protected:
    typedef EDFHeader::ChannelInfo ChannelInfo;
    typedef EDFHeader::ChannelList ChannelList;
    ChannelList &Channels()
    {
        return mChannels;
    }
    unsigned int NumRecords() const
    {
        return mNumRecords;
    }

  private:
    template <typename T> void PutBlock(std::ostream &, const GenericSignal &, const StateVector &);

    ChannelList mChannels;
    unsigned int mNumRecords;
    std::vector<std::string> mStateNames;
};

#endif // EDF_OUTPUT_BASE_H
