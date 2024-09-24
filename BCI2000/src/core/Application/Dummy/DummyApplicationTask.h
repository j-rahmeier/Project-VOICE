////////////////////////////////////////////////////////////////////////////////
// $Id: DummyApplicationTask.h 7775 2023-12-20 15:30:38Z mellinger $
// Authors:mellinger@neurotechcenter.org
// Description: A minimal task filter to avoid sending large amounts of data
//   to the Operator module
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
#ifndef DUMMY_APPLICATION_TASK_H
#define DUMMY_APPLICATION_TASK_H

#include "GenericFilter.h"

class DummyApplicationTask : public GenericFilter
{
  public:
    DummyApplicationTask();
    ~DummyApplicationTask();

  protected:
    void Publish() override;
    void Preflight(const SignalProperties &Input, SignalProperties &Output) const override;
    void Initialize(const SignalProperties &Input, const SignalProperties &Output) override;
    void Process(const GenericSignal&, GenericSignal&) override;

  private:
    bool mCopySignal;
};

#endif // DUMMY_APPLICATION_TASK_H
