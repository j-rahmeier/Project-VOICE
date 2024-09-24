/////////////////////////////////////////////////////////////////////////////
// $Id: XsensMTwLogger.h 7463 2023-06-30 14:59:17Z mellinger $
// Authors: Alexander Belsten (belsten@neurotechcenter.org)
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

#ifndef XSENS_MTW_LOGGER_H
#define XSENS_MTW_LOGGER_H

#include "Environment.h"
#include "Lockable.h"
#include "Thread.h"
#include <thread>
#include <atomic>
#include <iostream>
#include <mutex>

#include "include/xsensdeviceapi.h"
#include "xsmutex.h"

typedef std::set<XsDevice*> XsDeviceSet;
class XsensMTwLogger;

//----------------------------------------------------------------------
// Callback handler for wireless master definition
//----------------------------------------------------------------------
class WirelessMasterCallback : public XsCallback
{
  public:
	  XsDeviceSet  getWirelessMTWs() const;
    
  protected:
	  virtual void onConnectivityChanged(XsDevice* dev, XsConnectivityState newState);
    
  private:
	  mutable XsMutex m_mutex;
	  XsDeviceSet     m_connectedMTWs;
};


//----------------------------------------------------------------------
// Callback handler for MTw definition
//----------------------------------------------------------------------
class MtwCallback : public XsCallback
{
  public:
	  MtwCallback(int mtwIndex, XsDevice* device);
    void                setParent(XsensMTwLogger* _parent);
	  bool                dataAvailable() const;
	  XsDataPacket const* getOldestPacket() const;
	  void                deleteOldestPacket();
	  int                 getMtwIndex() const;
    XsDevice const&     device() const;
    
  protected:
	  virtual void        onLiveDataAvailable(XsDevice*, const XsDataPacket* packet);
    
  private:
	  mutable XsMutex         m_mutex;
	  std::list<XsDataPacket> m_packetBuffer;
	  int                     m_mtwIndex;
	  XsDevice*               m_device;
    XsensMTwLogger*         m_parent;
};


//----------------------------------------------------------------------
// BCI2000 API definition
//----------------------------------------------------------------------
class XsensMTwLogger : public EnvironmentExtension, public Thread
{
  friend class MtwCallback;

public:
  //Constructors and virtual interface
   XsensMTwLogger();
  ~XsensMTwLogger();
  void Publish() override;
  void AutoConfig() override;
  void Preflight() const override;
  void Initialize() override;
  void StartRun() override;
  void StopRun() override;
  void Halt() override;

private:
  bool ConnectToMaster();
  bool IsConnectedToControl() const { return(mp_control != nullptr); };
  bool IsConnectedToMaster() const { return(m_wireless_master_device != 0); };
  void Disconnect();

  bool   ConfigureMaster();
  size_t CountMTws();
  bool   StartMTwRecording();
  void   StopMTwRecording();
  void   AcquireData(int _thread);
  uint32_t SignMagnitude(double x);

  bool      m_enable;
  int       m_desired_update_rate;
  int       m_actual_update_rate;
  int       m_desired_radio_channel;
  int       m_prescaler;
  size_t    m_desired_mtw_count;
  size_t    m_actual_mtw_count;

  XsControl*                mp_control;
  XsDevicePtr               m_wireless_master_device;
  WirelessMasterCallback    m_wireless_master_callback; // Callback for wireless master
  std::vector<MtwCallback*> m_mtw_callbacks;            // Callbacks for MTw devices
  std::vector<XsEuler>      m_euler_data;

  std::vector<HANDLE>       m_sync_locks;
  std::vector<HANDLE>       m_record_locks;
  std::atomic_bool          m_kill_threads;
  std::atomic_bool          m_stop_recording;
  std::vector<std::thread>  m_threads;
};

#endif // XSENS_MTW_LOGGER_H
