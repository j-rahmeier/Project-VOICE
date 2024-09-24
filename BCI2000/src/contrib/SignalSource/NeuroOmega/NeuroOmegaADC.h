////////////////////////////////////////////////////////////////////////////////
// $Id: NeuroOmegaADC.h 7809 2024-01-30 18:59:04Z wengelhardt $
// Authors: Alexander Belsten (belsten@neurotechcenter.org)
// Description: NeuroOmegaADC header
//   
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
#ifndef INCLUDED_NEURO_OMEGA_ADC_H
#define INCLUDED_NEURO_OMEGA_ADC_H

#include "BufferedADC.h"
#include "Clock.h"
#include "include/NeuroOmega.imports.h"
#include "Expression/Expression.h"
#include <thread>
#include <map>
#include <concurrent_queue.h>
#include <condition_variable>
#include <chrono>
#include <queue>
#include "StreamFormat.h"
#include <ostream>

//NO = NeuroOmega
class StimulationGroupNO
{
public:
  StimulationGroupNO(Expression _exp, std::vector<Expression> _channels, Expression _retCh, int _id);
  ~StimulationGroupNO();

  const Expression&               GetExpression()     const { return this->m_exp; }
  const std::vector<Expression>&  GetChannels()       const { return this->m_channels; }
  const Expression&               GetReturnChannel()  const { return this->m_return_channel; }
  const int&                      GetID()             const { return this->m_id; }
  const int                       AlreadyTriggered()  const { return this->m_triggered; }
  void                            ResetTrigger()            { this->m_triggered = false; }
  void                            SetTrigger()              { this->m_triggered = true; }

private:
  Expression              m_exp;
  std::vector<double>     m_seqs;
  std::vector<Expression> m_channels;
  Expression              m_return_channel;
  int                     m_id;
  bool                    m_triggered;
};

typedef std::vector<StimulationGroupNO> StimulationGroupNOList;

class NeuroOmegaADC : public BufferedADC
{
 public:
    NeuroOmegaADC();
   ~NeuroOmegaADC();

  //buffered classes (recording)
  void OnPublish() override;
  void OnAutoConfig() override;
  void OnPreflight(SignalProperties&) const override;
  void OnInitialize(const SignalProperties& Output) override;
  void OnStartAcquisition() override;
  void DoAcquire(GenericSignal& Output) override;
  void OnStopAcquisition() override;
  //generic classes (stimulation)
  void Preflight(const SignalProperties&, SignalProperties&) const override;
  void Initialize(const SignalProperties&, const SignalProperties&) override;
  void StartRun() override;
  void Process(const GenericSignal&, GenericSignal&) override;
  void StopRun() override;
  void Halt() override;


 private:
	 // device specific functions
   
   //stimulation
   void StimulateAOThread();
   void ReadStimMarkerThread();
   void ConvertTimeStamp(int16, int16, UINT32&);
   //connection
   void WaitForConnection();
	 bool IsConnectedToAO() const { return ((EAOConnection)isConnected() == eAO_CONNECTED); };
   bool IsConnecting() const { return ((EAOConnection)isConnected() == eAO_CONNECTING); };
   bool IsDisconnectedToAO() const { return ((EAOConnection)isConnected() == eAO_DISCONNECTED); };
	 bool ConnectToAO(MAC_ADDR _aoMAC);
	 void DisconnectFromAO();
	 void MonitorConnectionStatus();
   //recording
   bool SaveAndBufferChannel(int);
	 bool SetRecordingChannels();
   bool TestRecordingChannels();
	 bool GetChannelInfo();
   void GetChannelInfo_DUMMY(); // used for validation when not testing with device
	 bool SetChannelNames();
   bool ChannelExists(std::string chName, int* m_arr);


   //initialization
   std::map<uint32, std::string> m_channel_info_map;
   int                           m_n_channels;
   
   //block data acquisition (new, unreliable)
   int m_buffer_size;
   std::map<int32, std::queue<std::pair<UINT32, int16>>> mDataArray;
   std::map<int32, UINT32> mDataLastTS;
   std::multimap<UINT32, int32> mChannelSeen;
   std::map<int32, std::queue<int16>> mStateArray;
   int32* mChIDS;
   bool m_running;
   const int m_HEADER_SIZE = 7;
   const int m_SAMPLE_SIZE = 64;
   const int m_DELTA_T = 0x10000;
   const int m_BLOCK_DELAY = 8 * m_DELTA_T;
   int mElRemain, mChRemain;
   int64 mTimeOffset;
   UINT32 mGlobalTime;

   //test
   int lastNum = 10286;
   std::queue<int> chs;
   UINT32 lastTS = 0;
   std::queue < std::pair<int, UINT32> > mTimeStamp;
   bool firstTime = true;

   // connection status thread
	 EAOConnection			m_connection_status;
	 std::thread				m_connection_status_thread;
	 std::atomic_bool   m_kill_connection_thread;

   // mac addrs
   MAC_ADDR  m_ao_mac;

   // block size
	 float		 m_block_size_ms;
	 int			 m_block_size_samples;

   // get aligned data (old, reliable method)
	 const int mc_array_size = 10000; // size of buffer
	 int 			 m_actual_data;
	 int*			 m_array_ch;            // contains AO ChannelIDs
	 int16*		 m_p_data;              // buffer sent to AO for data
	 ULONG		 m_ts_begin;            // time-stamp returned from AO
	 std::queue<int16*> m_data_queue;
   std::queue<UINT16> m_timeStamp_queue;

   //drive depth
	 bool m_showDriveErr, m_record_driveDepth;

	 //stimulation
	 int                                  m_enable_stim;
	 StimulationGroupNOList               m_stim_groups;
   std::vector<std::vector<Expression>> m_all_configurations;
	 double                               M_DELAY;

	 //stimulation thread
	 std::thread                            m_stim_thread;
	 std::atomic_bool                       m_kill_stim_thread;
	 std::atomic_bool                       m_doStim;
	 HANDLE                                 m_stim_lock;
	 bool                                   m_newTrain;
   std::chrono::steady_clock::time_point  m_train_start;
   Clock                                  mClock;

   struct TriggeredStim
   {
     int trigID;
     real32 delay1;
     real32 amp1;
     real32 dur1;
     real32 delay2;
     real32 amp2;
     real32 dur2;
     int freq;
     real32 burstDur;
     double trainDur;
     double trainFreq;
     std::vector<int> chs;
     int retCh;
   };

   friend std::ostream& operator<<(std::ostream& os, const TriggeredStim& s);
   std::queue<TriggeredStim> m_triggeredList;

   //stim marker thread
   std::thread m_stim_marker_thread;
   int* m_array_stim;
};

#endif // INCLUDED_NEURO_OMEGA_ADC_H