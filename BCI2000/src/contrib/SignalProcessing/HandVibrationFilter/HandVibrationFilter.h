////////////////////////////////////////////////////////////////////////////////
// Authors: Alex Belsten belsten@neurotechcenter.org
// Description: HandVibrationFilter header
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2021: BCI2000 team and many external contributors ]
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

#ifndef INCLUDED_HANDVIBRATIONFILTER_H  // makes sure this header is not included more than once
#define INCLUDED_HANDVIBRATIONFILTER_H

#include "GenericFilter.h"
#include "Expression/Expression.h"
#include "HandVibration.h"

typedef std::pair<Expression, std::pair<int, float>> Configuration;
typedef std::list<Configuration> ConfigurationList;


class HandVibrationFilter : public GenericFilter
{
 public:
   HandVibrationFilter ();
  ~HandVibrationFilter ();
  void Publish    (                                                              )       override;
  void AutoConfig (const SignalProperties& Input                                 )       override;
  void Preflight  (const SignalProperties& Input,       SignalProperties& Output ) const override;
  void Initialize (const SignalProperties& Input, const SignalProperties& Output )       override;
  void StartRun   (                                                              )       override;
  void Process    (const GenericSignal&    Input,          GenericSignal& Output )       override;
  void StopRun    (                                                              )       override;

 private:
  bool EvaluateConfigurations (const GenericSignal& Input);

  bool                        mEnable,
                              mDoModulateState,
                              mVibrating;
  std::string                 mModulationState;
  int                         m_block_count, 
                              m_decimation;
  HandVibration               mDevice;
  ConfigurationList           mConfigList;
  ConfigurationList::iterator mCurrentConfiguration;
};

#endif // INCLUDED_HANDVIBRATIONFILTER_H