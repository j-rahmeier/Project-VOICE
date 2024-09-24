////////////////////////////////////////////////////////////////////////////////
// Authors: Alexander Belsten (belsten@neurotechcenter.org)
// Description: BlackRock CereStim header
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

#ifndef INCLUDED_CERESTIM_H  
#define INCLUDED_CERESTIM_H

#include "GenericFilter.h"
#include "Expression/Expression.h"
#include "imports/BStimulator.h"
#include "Win32Defs.h"
#include <iomanip>
#include <set>
#include <thread>
#include <map>
#include <concurrent_queue.h>
#include <condition_variable>
#include <chrono>
#include "GenericVisualization.h"

class CereStim : public GenericFilter
{
 public:
   CereStim();
  ~CereStim();
  void Publish    (                                                              )       override;
  void AutoConfig (const SignalProperties& Input                                 )       override;
  void Preflight  (const SignalProperties& Input,       SignalProperties& Output ) const override;
  void Initialize (const SignalProperties& Input, const SignalProperties& Output )       override;
  void StartRun   (                                                              )       override;
  void Process    (const GenericSignal&    Input,          GenericSignal& Output )       override;
  void StopRun    (                                                              )       override;
  void Halt       (                                                              )       override;

 private:
  bool OpenConnection();
  void CloseConnection();
  bool IsConnected();
  void SequenceRunning();
  bool ErrorOccurred(BResult _res);
  void CereStimThread();//Start the CereStim stimulation thread
  void ConfigurationUploadThread();//Start the CereStim stimulation upload thread
  void ClearActiveConfiguration();
  bool AlreadyTriggered(uint16_t&, uint16_t&);

  std::string                           m_last_error;
  bool                                  m_enable;
  BStimulator                           m_cerestim;

  //custom thread vars
  std::thread                           m_acquire_thread;
  std::thread                           m_status_thread;
  std::thread                           m_upload_thread;
  std::atomic_bool                      m_kill_status_thread;
  std::atomic_bool                      m_sleepThread;

  HANDLE    m_sync_lock;
  HANDLE    m_upload_lock;
  HANDLE m_stim_timer;

  GenericVisualization mVis;

  //for configuring stimulation as we go
  bool mPreload;
  struct DynamicPattern
  {
    BWFType cath_first;
    UINT8   n_pulses;
    UINT16  amp1;
    UINT16  amp2;
    UINT16  phase1;
    UINT16  phase2;
    UINT32  frequency;
    UINT16  inter_phase;
  };

  //for making code readable
  enum tID {
    exp = 0,
    id  = 1,
    el  = 2,
    tLen
  };
  enum cID {
    cath = 0,
    n    = 1,
    amp1 = 2,
    amp2 = 3,
    dur1 = 4,
    dur2 = 5,
    f    = 6,
    iDur = 7,
    tDur = 8,
    tF   = 9,
    cLen
  };
  struct StimPattern
  {
    BConfig     moduleId;
    //train vars
    UINT16      wait_ms;
    UINT16      trainReps;
    double totalDuration;
  };

  struct StimConfiguration
  {
    Expression  stimTrigger;
    //electrodes
    std::set<UINT8> electrodes;
    //patterns
    StimPattern pattern;
    DynamicPattern dynamicPattern;
    UINT8 configNum;
    bool uploaded;
  };
  std::list<StimConfiguration> mConfigurations;
  std::list<StimConfiguration*> mUploadConfigurations;
  void SetConfiguration(StimConfiguration*);
  bool EvaluateExpression(Expression*, const GenericSignal*);

  struct TriggeredStimulation
  {
    std::map<UINT8, BConfig> electrodePattern;
    //train vars
    UINT16*      wait_ms;
    UINT16*      trainReps;
    double* totalDuration;
  };
  TriggeredStimulation  mActiveStimulation;
  std::atomic_bool      mStimIsActive;
  std::atomic_bool      mStimState;
  uint16_t              mAlreadyTriggered;
  uint16_t              mAlreadyUploaded;
  Expression            mStartExpression;
  bool                  mStartAfterUpload;
  UINT8                 mUploadState;

  //values to reset triggered stim
  UINT16 defaultWait;
  UINT16 defaultReps;
  double defaultDur;
};

#endif // INCLUDED_CERESTIM_H
