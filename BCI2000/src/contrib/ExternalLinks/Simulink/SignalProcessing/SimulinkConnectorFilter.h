////////////////////////////////////////////////////////////////////////////////
// $Id: SimulinkConnectorFilter.h 7657 2023-10-13 15:15:52Z mellinger $
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
#ifndef SIMULINK_CONNECTOR_FILTER
#define SIMULINK_CONNECTOR_FILTER

#include "GenericFilter.h"

class SimulinkConnectorFilter : public GenericFilter
{
  public:
    SimulinkConnectorFilter();
    ~SimulinkConnectorFilter();
    void Publish() override;
    void Preflight(const SignalProperties &, SignalProperties &) const override;
    void Initialize(const SignalProperties &, const SignalProperties &) override;
    void Halt() override;
    void StartRun() override;
    void StopRun() override;
    void Process(const GenericSignal &, GenericSignal &) override;

  private:
    struct Private;
    Private *p;
};

#endif // SIMULINK_CONNECTOR_FILTER
