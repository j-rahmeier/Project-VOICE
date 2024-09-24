/////////////////////////////////////////////////////////////////////////////
// $Id: SecondaryBCI2000Controller.h 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
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
/////////////////////////////////////////////////////////////////////////////
#ifndef SECONDARY_BCI2000_CONTROLLER_H
#define SECONDARY_BCI2000_CONTROLLER_H

#include "Environment.h"
#include "BCI2000Remote.h"

class SecondaryBCI2000Controller : public EnvironmentExtension
{
public:
  SecondaryBCI2000Controller();
  ~SecondaryBCI2000Controller();

protected:
  void AutoConfig() override;
  void Publish() override;
  void Preflight() const override;
  void Initialize() override;
  void StartRun() override;
  void StopRun() override;
  void Process() override;
  void Halt() override;
  void Halt2() override { Halt(); }

private:
  bool mEnabled, mConnected;
  int mFilePart;
  mutable BCI2000Remote mConnection;
};

#endif // SECONDARY_BCI2000_CONTROLLER_H





