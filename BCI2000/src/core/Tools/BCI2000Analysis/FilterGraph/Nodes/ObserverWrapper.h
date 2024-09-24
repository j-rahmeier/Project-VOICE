////////////////////////////////////////////////////////////////////////////////
// $Id: ObserverWrapper.h 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A wrapper filter that observes its input signal, and 
//   saves it for later classifier training.
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
#ifndef OBSERVER_WRAPPER_H
#define OBSERVER_WRAPPER_H

#include "GenericFilter.h"
#include <vector>

class ObserverBase
{
  public:
    ObserverBase();
    virtual ~ObserverBase();

    void setObserve(bool);
    void setCondition1(const std::string &);
    void setCondition2(const std::string &);
    void resetStatistics();
    const std::vector<GenericSignal> &data1() const;
    const std::vector<GenericSignal> &data2() const;
    const std::vector<std::vector<int>> &trials1() const;
    const std::vector<std::vector<int>> &trials2() const;

  protected:
    void onPreflight(const SignalProperties&, SignalProperties&) const;
    void onInitialize(const SignalProperties&, const SignalProperties&);
    void onProcess(const GenericSignal&, GenericSignal&);

  private:
    struct Private;
    Private *p;
};

template<class T> class ObserverWrapper : public ObserverBase, public T
{
protected:
    void Preflight(const SignalProperties& Input, SignalProperties& Output) const override
    {
        onPreflight(Input, Output);
        T::Preflight(Input, Output);
    }
    void Initialize(const SignalProperties& Input, const SignalProperties& Output) override
    {
        onInitialize(Input, Output);
        T::Initialize(Input, Output);
    }
    void Process(const GenericSignal& Input, GenericSignal& Output) override
    {
        onProcess(Input, Output);
        T::Process(Input, Output);
    }
};

#endif // OBSERVER_WRAPPER_H
