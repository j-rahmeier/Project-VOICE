/////////////////////////////////////////////////////////////////////////////
// $Id: XsensMTwLogger.cpp 7993 2024-04-04 20:44:05Z mellinger $
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
#include "XsensMTwLogger.h"
#include "BCIEvent.h"

#define bcidbgOn

Extension(XsensMTwLogger);

// ======= CONSTANTS =======
const int c_update_rate   = 80; //Hz
const int c_radio_channel = 19; 
// most efficient prescaler is 11930464, but
// using 10000000 for simplicity for analysis
const int c_prescaler = 10000000;
// =========================


/*! \brief Stream insertion operator overload for XsPortInfo */
std::ostream& operator << (std::ostream& out, XsPortInfo const& p)
{
	out << "Port: " << std::right << p.portNumber() << " (" << p.portName().toStdString() << ") @ "
		  << p.baudrate() << " Bd" << ", " << "ID: " << p.deviceId().toString().toStdString();
	return out;
}


/*! \brief Stream insertion operator overload for XsDevice */
std::ostream& operator << (std::ostream& out, XsDevice const& d)
{
	out << "ID: " << d.deviceId().toString().toStdString() << " (" << d.productCode().toStdString() << ")";
	return out;
}


/*! \brief Given a list of update rates and a desired update rate, returns the closest update rate to the desired one */
int findClosestUpdateRate(const XsIntArray& supportedUpdateRates, const int desiredUpdateRate)
{
	if (supportedUpdateRates.empty())
	{
		return 0;
	}

	if (supportedUpdateRates.size() == 1)
	{
		return supportedUpdateRates[0];
	}

	int uRateDist = -1;
	int closestUpdateRate = -1;
	for (XsIntArray::const_iterator itUpRate = supportedUpdateRates.begin(); itUpRate != supportedUpdateRates.end(); ++itUpRate)
	{
		const int currDist = std::abs(*itUpRate - desiredUpdateRate);

		if ((uRateDist == -1) || (currDist < uRateDist))
		{
			uRateDist = currDist;
			closestUpdateRate = *itUpRate;
		}
	}
	return closestUpdateRate;
}


//----------------------------------------------------------------------
// Callback handler for wireless master
//----------------------------------------------------------------------
XsDeviceSet 
WirelessMasterCallback::getWirelessMTWs() const
{
	XsMutexLocker lock(m_mutex);
	return m_connectedMTWs;
}


void 
WirelessMasterCallback::onConnectivityChanged(XsDevice* dev, XsConnectivityState newState)
{
	XsMutexLocker lock(m_mutex);
	switch (newState)
	{
	case XCS_Disconnected:		/*!< Device has disconnected, only limited informational functionality is available. */
		bcidbg << "EVENT: MTW Disconnected -> " << *dev << std::endl;
		m_connectedMTWs.erase(dev);
		break;
	case XCS_Rejected:			/*!< Device has been rejected and is disconnected, only limited informational functionality is available. */
		bcidbg << "EVENT: MTW Rejected -> " << *dev << std::endl;
		m_connectedMTWs.erase(dev);
		break;
	case XCS_PluggedIn:			/*!< Device is connected through a cable. */
		bcidbg << "EVENT: MTW PluggedIn -> " << *dev << std::endl;
		m_connectedMTWs.erase(dev);
		break;
	case XCS_Wireless:			/*!< Device is connected wirelessly. */
		bcidbg << "EVENT: MTW Connected -> " << *dev << std::endl;
		m_connectedMTWs.insert(dev);
		break;
	case XCS_File:				/*!< Device is reading from a file. */
		bcidbg << "EVENT: MTW File -> " << *dev << std::endl;
		m_connectedMTWs.erase(dev);
		break;
	case XCS_Unknown:			/*!< Device is in an unknown state. */
		bcidbg << "EVENT: MTW Unknown -> " << *dev << std::endl;
		m_connectedMTWs.erase(dev);
		break;
	default:
		bcidbg << "EVENT: MTW Error -> " << *dev << std::endl;
		m_connectedMTWs.erase(dev);
		break;
	}
}


//----------------------------------------------------------------------
// Callback handler for MTw
// Handles onDataAvailable callbacks for MTW devices
//----------------------------------------------------------------------
MtwCallback::MtwCallback(int mtwIndex, XsDevice* device):
	m_mtwIndex(mtwIndex), 
	m_device(device),
	m_parent(nullptr)
{

}


void 
MtwCallback::setParent(XsensMTwLogger* _parent)
{
	m_parent = _parent;
}

bool 
MtwCallback::dataAvailable() const
{
	XsMutexLocker lock(m_mutex);
	return !m_packetBuffer.empty();
}


XsDataPacket const* 
MtwCallback::getOldestPacket() const
{
	XsMutexLocker lock(m_mutex);
	XsDataPacket const* packet = &m_packetBuffer.front();
	return packet;
}


void 
MtwCallback::deleteOldestPacket()
{
	XsMutexLocker lock(m_mutex);
	m_packetBuffer.pop_front();
}


int 
MtwCallback::getMtwIndex() const
{
	return m_mtwIndex;
}


XsDevice const& 
MtwCallback::device() const
{
	assert(m_device != 0);
	return *m_device;
}


void 
MtwCallback::onLiveDataAvailable(XsDevice*, const XsDataPacket* packet)
{
	XsMutexLocker lock(m_mutex);
	// NOTE: Processing of packets should not be done in this thread.

	m_packetBuffer.push_back(*packet);
	if (m_packetBuffer.size() > 300)
	{
		bciwarn << "XsensMTwLogger Warning: Buffer overflow, deleting oldest packet" << std::endl;
		deleteOldestPacket();
	}
	if (m_parent && m_parent->m_sync_locks.size() > m_mtwIndex)
		SetEvent(m_parent->m_sync_locks[m_mtwIndex]);
}


//----------------------------------------------------------------------
// Core BCI2000 API logger implementation
//----------------------------------------------------------------------
XsensMTwLogger::XsensMTwLogger(): 
	m_enable							  ( false ),
	m_desired_update_rate   ( c_update_rate ),
	m_actual_update_rate    ( c_update_rate ),
	m_desired_radio_channel ( c_radio_channel ),
	m_desired_mtw_count     ( 0 ),
	m_actual_mtw_count      ( 0 ),
	mp_control						  ( nullptr ),
	m_wireless_master_device( 0 ),
	m_kill_threads					( false )
{
    PublishEnabler("LogXsensMTw");
}


XsensMTwLogger::~XsensMTwLogger()
{
	this->Disconnect();
}


void 
XsensMTwLogger::Publish()
{
	if (!OptionalParameter("LogXsensMTw", 0))
		return;

	BEGIN_PARAMETER_DEFINITIONS
		"Source:MTw int /LogXsensMTw= 0 0 0 1"
			" // enable MTw logging (boolean)",
		"Source:MTw float MasterUpdateRate= 80Hz 80Hz 0 % "
			"// The desired update rate for the master ",
		"Source:MTw int MasterRadioChannel= 19 19 11 25 "
			"// Radio channel to use for wireless communication",
		"Source:MTw int DataPrescaler= auto auto % % "
			"// Scales the data",
	END_PARAMETER_DEFINITIONS

	std::vector<std::string> dim = { "X", "Y", "Z" };
	m_desired_mtw_count = OptionalParameter("NumberOfMTws", 1);
	for (int i = 0; i < m_desired_mtw_count; i++)
	{
		for (int d = 0; d < dim.size(); d++)
		{
			StringUtils::String eventDef;
			eventDef << "MTw" << i + 1 << dim[d] << " 32 0 0 0 ";

			BEGIN_EVENT_DEFINITIONS
				eventDef.c_str()
			END_EVENT_DEFINITIONS
		}
	}
}


void 
XsensMTwLogger::AutoConfig()
{
	bool Enable = ((int)OptionalParameter("LogXsensMTw", 0) != 0);
	if (!Enable) return;

	// set and get parameters
	Parameter("MasterUpdateRate")		= c_update_rate;
	Parameter("MasterRadioChannel") = c_radio_channel;
	Parameter("DataPrescaler")			= c_prescaler;

	m_desired_update_rate		= ActualParameter("MasterUpdateRate").InHertz();
	m_desired_radio_channel = ActualParameter("MasterRadioChannel");
	
	// connect to master
	if (!this->IsConnectedToMaster())
		if (!this->ConnectToMaster())
			return;

	// configure master
	if (!this->ConfigureMaster())
		return;
}


void 
XsensMTwLogger::Preflight() const
{	
	bool Enable = ( ( int )OptionalParameter( "LogXsensMTw", 0) != 0 );
	if (!Enable) return;

	// check that all parameters are valid
	Parameter("MasterUpdateRate");
	Parameter("MasterRadioChannel");
	Parameter("DataPrescaler");
}


void 
XsensMTwLogger::Initialize()
{
	m_enable = ( ( int )OptionalParameter( "LogXsensMTw", 0) != 0 );
	if (!m_enable) return;

	if (!this->IsConnectedToMaster())
		bcierr << "XsensMTwLogger Error: Unable to connect to master device" << std::endl;

	m_actual_mtw_count = this->CountMTws();

	m_prescaler = Parameter("DataPrescaler");

  m_kill_threads = false;
  // do this before we start the threads so things aren't being appended while structure
  //  is accessed from a different threads
  if (!m_record_locks.empty())
  {
    for (int i = 0; i < m_threads.size(); i++)
    {
      CloseHandle(m_record_locks[i]);
    }
  }
  m_record_locks.clear();
  for (int i = 0; i < m_wireless_master_callback.getWirelessMTWs().size(); i++)
  {
    m_record_locks.push_back(CreateEvent(NULL, false, false, NULL));
  }
  //create locks for recording/stopping
  if (!m_sync_locks.empty())
  {
    for (int i = 0; i < m_sync_locks.size(); i++)
    {
      CloseHandle(m_sync_locks[i]);
    }
  }
  m_sync_locks.clear();
  for (int i = 0; i < m_wireless_master_callback.getWirelessMTWs().size(); i++)
  {
    m_sync_locks.push_back(CreateEvent(NULL, false, false, NULL));
  }

  // start the threads
  if (!m_threads.empty())
  {
    m_kill_threads = true;
    for (int i = 0; i < m_threads.size(); i++)
    {
      m_threads[i].join();
    }
  }
  m_threads.clear();
  m_kill_threads = false;
  for (int i = 0; i < m_wireless_master_callback.getWirelessMTWs().size(); i++)
  {
    m_threads.push_back(std::thread(&XsensMTwLogger::AcquireData, this, i));
  }

  if (!this->StartMTwRecording())
  {
    bcierr << "Could not start recording";
  }
}


void 
XsensMTwLogger::StartRun()
{
	if (!m_enable) return;

  m_stop_recording = false;
  for (int i = 0; i < m_wireless_master_callback.getWirelessMTWs().size(); i++)
  {
    SetEvent(m_record_locks[i]);
  }
  //go to recording mode
  if (!m_wireless_master_device->gotoMeasurement())
  {
    std::ostringstream error;
    error << "Failed to go to measurement mode: " << *m_wireless_master_device;
    throw std::runtime_error(error.str());
  }
}


void 
XsensMTwLogger::AcquireData(int _thread)
{
  while (!this->m_kill_threads)
  {
    //wait for start of run
    DWORD res1 = WaitForSingleObject(m_record_locks[_thread], 1000);
    if (res1 == WAIT_OBJECT_0)
    {
      while (!this->m_stop_recording)
      {
        //wait for data to be collected
        DWORD res = WaitForSingleObject(m_sync_locks[_thread], 1000);
        if (res == WAIT_OBJECT_0)
        {
          XsDataPacket const* packet = m_mtw_callbacks[_thread]->getOldestPacket();
          m_euler_data[_thread] = packet->orientationEuler();
          m_mtw_callbacks[_thread]->deleteOldestPacket();
          if (!m_stop_recording)
          {
            bcievent << "MTw" << _thread + 1 << "X " << (uint32_t)(SignMagnitude(m_euler_data[_thread].x()));
            bcievent << "MTw" << _thread + 1 << "Y " << (uint32_t)(SignMagnitude(m_euler_data[_thread].y()));
            bcievent << "MTw" << _thread + 1 << "Z " << (uint32_t)(SignMagnitude(m_euler_data[_thread].z()));
          }
        }
        else
          bcidbg << "Not enough data collected" << std::endl;
      }
    }
  }
}


void 
XsensMTwLogger::StopRun()
{
	if (!m_enable) return;
  m_stop_recording = true;

  //stop recording
  this->StopMTwRecording();
}


void 
XsensMTwLogger::Halt()
{
	if (!m_wireless_master_device)
		return;
	// if this is measuring, stop the measurement
	try {
		bcidbg << "Disabling radio channel if previously enabled..." << std::endl;
		if (m_wireless_master_device->isRadioEnabled())
		{
			if (!m_wireless_master_device->disableRadio())
			{
				std::ostringstream error;
				error << "Failed to disable radio channel: " << *m_wireless_master_device;
				throw std::runtime_error(error.str());
			}
		}
	}
	catch (std::exception const& ex)
	{
		bcierr << "XsensMTwLogger Error: Could not connect to master. " << ex.what() << std::endl;
		m_wireless_master_device = 0;
	}
	catch (...)
	{
		bcierr << "XsensMTwLogger Error: An unknown fatal error has occured. Aborting." << std::endl;
		m_wireless_master_device = 0;
	}

  m_kill_threads = true;
  for (int i = 0; i < m_threads.size(); i++)
  {
    m_threads[i].join();
  }
  m_threads.clear();
  // Terminate();
  this->StopMTwRecording();

  for (int i = 0; i < m_threads.size(); i++)
  {
    bciwarn << "Handles closed from halt" << std::endl;
    CloseHandle(m_record_locks[i]);
  }
  m_record_locks.clear();

  for (int i = 0; i < m_threads.size(); i++)
  {
    CloseHandle(m_sync_locks[i]);
  }
  m_sync_locks.clear();
}

uint32_t
XsensMTwLogger::SignMagnitude(double x)
{
	if ((abs((float)m_prescaler * x)) > (float)0x7FFFFFFF)
		bciwarn << "overflow of state occurred; consider reducing prescaler" << std::endl;
	return (uint32_t)(((float)m_prescaler * x) + (float)0x80000000);
}


//----------------------------------------------------------------------
// other device-specific BCI2000 member functions
//----------------------------------------------------------------------
bool 
XsensMTwLogger::ConnectToMaster()
{

	bcidbg << "XsensMTwLogger: Attempting to connect to master..." << std::endl;
	try
	{
		mp_control = XsControl::construct();
		if (mp_control == 0)
		{
			throw std::runtime_error("Failed to construct XsControl instance.");
		}

		bcidbg << "Scanning ports..." << std::endl;
		XsPortInfoArray detectedDevices = XsScanner::scanPorts();

		bcidbg << "Finding wireless master..." << std::endl;
		XsPortInfoArray::const_iterator wirelessMasterPort = detectedDevices.begin();
		while (wirelessMasterPort != detectedDevices.end() && !wirelessMasterPort->deviceId().isWirelessMaster())
		{
			++wirelessMasterPort;
		}
		if (wirelessMasterPort == detectedDevices.end())
		{
			throw std::runtime_error("No wireless masters found");
		}
		bcidbg << "Wireless master found @ " << *wirelessMasterPort << std::endl;

		bcidbg << "Opening port..." << std::endl;
		if (!mp_control->openPort(wirelessMasterPort->portName().toStdString(), wirelessMasterPort->baudrate()))
		{
			std::ostringstream error;
			error << "Failed to open port " << *wirelessMasterPort;
			throw std::runtime_error(error.str());
		}

		bcidbg << "Getting XsDevice instance for wireless master..." << std::endl;
		m_wireless_master_device = mp_control->device(wirelessMasterPort->deviceId());
		if (m_wireless_master_device == 0)
		{
			std::ostringstream error;
			error << "Failed to construct XsDevice instance: " << *wirelessMasterPort;
			throw std::runtime_error(error.str());
		}

		bcidbg << "XsDevice instance created @ " << *m_wireless_master_device << std::endl;

		bcidbg << "Setting config mode..." << std::endl;
		if (!m_wireless_master_device->gotoConfig())
		{
			std::ostringstream error;
			error << "Failed to go to config mode: " << *m_wireless_master_device;
			throw std::runtime_error(error.str());
		}

		bcidbg << "Attaching callback handler..." << std::endl;
		m_wireless_master_device->addCallbackHandler(&m_wireless_master_callback);
	}
	catch (std::exception const& ex)
	{
		bcierr << "XsensMTwLogger Error: Could not connect to master. " << ex.what() << std::endl;
		m_wireless_master_device = 0;
		Disconnect();
		return false;
	} 
	catch (...)
	{
		bcierr << "XsensMTwLogger Error: An unknown fatal error has occured. Aborting." << std::endl;
		m_wireless_master_device = 0;
		Disconnect();
		return false;
	}
	bcidbg << "XsensMTwLogger: Connected to master" << std::endl;
	return true;
}


void
XsensMTwLogger::Disconnect()
{
	if (this->IsConnectedToControl())
	{
		mp_control->close();
		mp_control = nullptr;
	}
	if (this->IsConnectedToMaster())
	{
	
	}
	if (!m_mtw_callbacks.empty())
	{
		for (int i = 0; i < m_mtw_callbacks.size(); ++i)
		{
			delete m_mtw_callbacks[i];
		}
		m_mtw_callbacks.clear();
	}
}


bool
XsensMTwLogger::ConfigureMaster()
{
	try
	{
		bcidbg << "Getting the list of the supported update rates..." << std::endl;
		const XsIntArray supportedUpdateRates = m_wireless_master_device->supportedUpdateRates();

		bcidbg << "Supported update rates: ";
		for (XsIntArray::const_iterator itUpRate = supportedUpdateRates.begin(); itUpRate != supportedUpdateRates.end(); ++itUpRate)
		{
			bcidbg << *itUpRate << " ";
		}

		m_actual_update_rate = findClosestUpdateRate(supportedUpdateRates, m_desired_update_rate);

		if (m_actual_update_rate != m_desired_update_rate)
		{
			bciwarn << "XsensMTwLogger: Requested MasterUpdateRate of " << m_desired_update_rate
							<< "Hz not valid, using nearest valid value of " << m_actual_update_rate
							<< "Hz instead" << std::endl;
		}

		bcidbg << "Setting update rate to " << m_actual_update_rate << " Hz..." << std::endl;
		if (!m_wireless_master_device->setUpdateRate(m_actual_update_rate))
		{
			std::ostringstream error;
			error << "Failed to set update rate: " << *m_wireless_master_device;
			throw std::runtime_error(error.str());
		}

		bcidbg << "Disabling radio channel if previously enabled..." << std::endl;
		if (m_wireless_master_device->isRadioEnabled())
		{
			if (!m_wireless_master_device->disableRadio())
			{
				std::ostringstream error;
				error << "Failed to disable radio channel: " << *m_wireless_master_device;
				throw std::runtime_error(error.str());
			}
		}

		bcidbg << "Setting radio channel to " << m_desired_radio_channel << " and enabling radio..." << std::endl;
		if (!m_wireless_master_device->enableRadio(m_desired_radio_channel))
		{
			std::ostringstream error;
			error << "Failed to set radio channel: " << *m_wireless_master_device;
			throw std::runtime_error(error.str());
		}
	} 
	catch (std::exception const& ex)
	{
		bcierr << "XsensMTwLogger Error: Could not connect to master. " << ex.what() << std::endl;
		m_wireless_master_device = 0;
		return false;
	} 
	catch (...)
	{
		bcierr << "XsensMTwLogger Error: An unknown fatal error has occurred. Aborting." << std::endl;
		m_wireless_master_device = 0;
		return false;
	}
	return true;
}


size_t
XsensMTwLogger::CountMTws()
{
	bciwarn << "XsensMTwLogger: Waiting for MTw(s) to wirelessly connect..." << std::endl;
	int max_retry = 100; // search for 10 seconds	
	int retry = 0;
	
	size_t connectedMTWCount = m_wireless_master_callback.getWirelessMTWs().size();
	while (retry < max_retry)
	{
		Sleep(100);
		size_t nextCount = m_wireless_master_callback.getWirelessMTWs().size();
		
		if (nextCount != connectedMTWCount)
		{
			connectedMTWCount = nextCount;
			bciwarn << "XsensMTwLogger: Updated connected MTw count: " << connectedMTWCount << std::endl;
		}
		
		if (m_desired_mtw_count != 0 && connectedMTWCount == m_desired_mtw_count)	
			break; // we have found all of the devices 
				
		retry++;
		if ((retry%(max_retry/5) == 0))
			bciout << "XsensMTwLogger: Still trying to find more MTws..." << std::endl;
	}
	//validate the correct number has been counted
	if (connectedMTWCount == 0)
	{
		bcierr << "XsensMTwLogger Error: No MTws found. \n"
			<< "Make sure the MTws are actively blinking (they might need some jostling)" << std::endl;
		return -1;
	}

	if (m_desired_mtw_count == 0)
	{
		bciwarn << "XsensMTwLogger: Found " << connectedMTWCount << " MTw(s)" << std::endl;
	}
	else
	{
		if (m_desired_mtw_count != connectedMTWCount)
		{
			bcierr << "XsensMTwLogger Error: User requested to connect to " << m_desired_mtw_count
				<< " MTw(s) but " << connectedMTWCount << " were found." << std::endl;
			return -1;
		}
	}
	return connectedMTWCount;
}


bool
XsensMTwLogger::StartMTwRecording()
{
	try
	{
		bcidbg << "Starting measurement..." << std::endl;
		//enable first
		if (!m_wireless_master_device->isRadioEnabled())
		{
			if (!m_wireless_master_device->enableRadio(m_desired_radio_channel))
			{
				std::ostringstream error;
				error << "Failed to set radio channel: " << *m_wireless_master_device;
				throw std::runtime_error(error.str());
			}
		}

		bcidbg << "Getting XsDevice instances for all MTWs..." << std::endl;
		XsDeviceIdArray allDeviceIds = mp_control->deviceIds();
		XsDeviceIdArray mtwDeviceIds;
		int count = 1;
		std::string s;
		for (XsDeviceIdArray::const_iterator i = allDeviceIds.begin(); i != allDeviceIds.end(); ++i)
		{
			if (i->isMtw())
			{
				mtwDeviceIds.push_back(*i);
				//get serial number
				std::string fullSerial = mtwDeviceIds.last().toString().c_str();
				if (count == 1) s = "Each State's MTw Serial Number:\n";
				s += "\t" + std::string("ID ") + std::to_string(count) + "\t" + fullSerial + "\n";
				count++;
			}
		}
		bciout << s;
		if (mtwDeviceIds.size() != m_actual_mtw_count)
		{
			throw std::runtime_error("Unable to connect to all MTws");
		}
		XsDevicePtrArray mtwDevices;
		for (XsDeviceIdArray::const_iterator i = mtwDeviceIds.begin(); i != mtwDeviceIds.end(); ++i)
		{
			XsDevicePtr mtwDevice = mp_control->device(*i);
			if (mtwDevice != 0)
			{
				mtwDevices.push_back(mtwDevice);
			} else
			{
				throw std::runtime_error("Failed to create an MTW XsDevice instance");
			}
		}
		if (!m_mtw_callbacks.empty())
		{
			for (int i = 0; i < m_mtw_callbacks.size(); ++i)
			{
				delete m_mtw_callbacks[i];
			}
			m_mtw_callbacks.clear();
		}
		bcidbg << "Attaching callback handlers to MTWs..." << std::endl;
		m_mtw_callbacks.resize(mtwDevices.size());
		for (int i = 0; i < (int)mtwDevices.size(); ++i)
		{
			m_mtw_callbacks[i] = new MtwCallback(i, mtwDevices[i]);
			mtwDevices[i]->addCallbackHandler(m_mtw_callbacks[i]);

			m_mtw_callbacks[i]->setParent(this);
		}
		m_euler_data.clear();
		m_euler_data.resize(m_mtw_callbacks.size());
	}
	catch (std::exception const& ex)
	{
		bcierr << "XsensMTwLogger Error: Failed to initialize MTw for recording. " << ex.what() << std::endl;
		return false;
	} catch (...)
	{
		bcierr << "XsensMTwLogger Error: An unknown fatal error has occurred. Aborting." << std::endl;
		return false;
	}
	return true;
}


void 
XsensMTwLogger::StopMTwRecording()
{
	try
	{
		std::cout << "Setting config mode..." << std::endl;
		if (!m_wireless_master_device->gotoConfig())
		{
			std::ostringstream error;
			error << "Failed to go to config mode: " << *m_wireless_master_device;
			throw std::runtime_error(error.str());
		}
	}
	catch (std::exception const& ex)
	{
		bcierr << "XsensMTwLogger Error: Failed to stop MTw recording. " << ex.what() << std::endl;
	} catch (...)
	{
		bcierr << "XsensMTwLogger Error: An unknown fatal error has occurred. Aborting." << std::endl;
	}
}
