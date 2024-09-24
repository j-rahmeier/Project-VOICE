////////////////////////////////////////////////////////////////////////////////
// $Id: NeuroOmegaADC.cpp 7809 2024-01-30 18:59:04Z wengelhardt $
// Authors: Alexander Belsten (belsten@neurotechcenter.org)
// Description: NeuroOmegaADC implementation
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

#include "NeuroOmegaADC.h"
#include "BCIStream.h"
#include "BCIEvent.h"
#include "ThreadUtils.h"
#include "StringUtils.h"

using namespace std;

bool              isValidMacAddress(const char* mac, MAC_ADDR& MACaddr);
const std::string GetSystemSDKError();

RegisterFilter(NeuroOmegaADC, 1);

 // #define TESTING_WITHOUT_DEVICE
 // #define BLOCKS

template <class T>
vector<T> BCI2000ListToVector(ParamRef ch_parm) //, bool subtract_one)
{
  vector<T> ch_vector = {};
  for (int ch = 0; ch < ch_parm->NumValues(); ch++)
  {
    ch_vector.push_back((T)ch_parm(ch));
  }
  return ch_vector;
}
//to print TriggeredStim object
std::ostream& operator<<(std::ostream& os, const NeuroOmegaADC::TriggeredStim& s)
{
  os << "id: "        << s.trigID + 1 << " "
    << "\ndelay1: "   << s.delay1 << " \tms"
    << "\namp1: "     << s.amp1 << " \tmA"
    << "\ndur1: "     << s.dur1 << " \tms"
    << "\ndelay2: "   << s.delay2 << " \tms"
    << "\namp2: "     << s.amp2 << " \tmA"
    << "\ndur2: "     << s.dur2 << " \tms"
    << "\nfreq: "     << s.freq << " \tHz"
    << "\nburstDur: " << s.burstDur << " \ts"
    << "\ntrainDur: " << s.trainDur / 1000 << " \ts"
    << "\ntrainFreq: " << s.trainFreq << " "
    << "\nchs: ";
    for (auto ch = s.chs.begin(); ch != s.chs.end(); ++ch)
      os << *ch << " ";
    os << "\nreturnCh: " << s.retCh << " ";
  return os;
}

StimulationGroupNO::StimulationGroupNO(Expression _exp, vector<Expression> _channels, Expression _retCh, int _id)
{
  m_exp = _exp;
  m_channels = _channels;
  m_return_channel = _retCh;
  m_id = _id;
  m_triggered = false;
}

StimulationGroupNO::~StimulationGroupNO()
{

}

NeuroOmegaADC::NeuroOmegaADC() :
  m_block_size_ms(0),
  m_array_ch(nullptr),
  m_array_stim(nullptr),
  m_p_data(nullptr),
  m_ts_begin(0),
  m_kill_stim_thread(true),
  m_block_size_samples(0),
  m_n_channels(0),
  m_running(false),
  m_buffer_size(20000),
  M_DELAY(0.022),
  mElRemain(0),
  mChRemain(0)
{
  // start the thread that checks if we are connected to the hardware
  if (!LoadLibrary("NeuroOmega_x64.dll") && !LoadLibrary("NeuroOmega.dll"))
  {
    bcierr << "COULD NOT LOAD LIBRARY. You must install the NeuroOmega SDK from AlphaOmega." << endl;
  }
  m_connection_status = eAO_DISCONNECTED;
  m_kill_connection_thread = false;
  //m_connection_status_thread = thread(&NeuroOmegaADC::MonitorConnectionStatus, this);
  m_stim_lock = CreateEvent(NULL, false, false, NULL);
}

NeuroOmegaADC::~NeuroOmegaADC()
{
  m_kill_connection_thread = true;
  //m_connection_status_thread.join();
  delete[] m_array_ch;
  m_array_ch = NULL;
  delete[] m_array_stim;
  m_array_stim = NULL;
  delete[] m_p_data;
  m_p_data = NULL;
  CloseHandle(m_stim_lock);
  this->Halt();
  if (this->IsConnectedToAO())
  {
    this->DisconnectFromAO();
  }
}

void
NeuroOmegaADC::OnPublish()
{
  //bciout << "Entered publish" << endl;
#ifdef TESTING_WITHOUT_DEVICE
  bciwarn << "NeuroOmegaADC: Dummy module compiled. To actually acquire data, remove "
    << "\"#define TESTING_WITHOUT_DEVICE\" from NeuroOmegaADC.cpp" << endl;
#endif

  BEGIN_PARAMETER_DEFINITIONS
    "Source:Hardware%20Addresses string NeuroOmegaMAC= A8:1B:6A:23:9A:AA"
    " // NeuroOmega MAC address",

    "Source:Signal%20Properties list RecordingChIDs= 2 10016 10017 "
    " // NeuroOmega ID of recording channels",

    "Source:Signal%20Properties int SourceCh= auto auto % %"
    " // number of digitized and stored channels (noedit)",

    "Source:Signal%20Properties int SampleBlockSize= auto auto 1 %"
    " // number of samples transmitted at a time",

    "Source:Signal%20Properties float SamplingRate= auto auto 0.0 %"
    " // sample rate",

    "Source:Signal%20Properties list SourceChGain= 1 auto "
    " // physical units per raw A/D unit",

    "Source:Signal%20Properties list SourceChOffset= 1 auto "
    " // raw A/D offset to subtract, typically 0",

    "Source:Signal%20Properties list ChannelNames= 1 auto "
    " // names of amplifier channels",
#ifdef BLOCKS
    "Source:Signal%20Properties int BufferSize= auto auto 200 %"
    " // Advanced - Size of data buffer",
#endif
    "Source:Save%20mpx%20file int SaveMPX= 0 0 0 1 "
    " // Record on Neuro Omega system (boolean) ",

    "Source:Save%20mpx%20file string FilePath= C:\\Surgeries_Data 0 0 % "
    " // File path on Neuro Omega ",

    "Source:Save%20mpx%20file string FileName= testFile 0 0 30 "
    " // File name ",

    "Source:Digital%20Ports int EnablePortInputs= 0 0 0 1 "
    " // enable 16-bit digital input ports (boolean) ",

    "Source:Device%20Information matrix ChannelInfo= 1 1 auto "
    " // populated with channel info from NeuroOmega - data can only be viewed in offline analysis (readonly)(noedit)",

    "Source:Drive int ReadDriveDepth= 0 0 0 1 "
    " // Read Driver Depth Information from device (boolean)",

    //////////////////////////////////////////////////////////////////////
    //stimulation parameters
    //////////////////////////////////////////////////////////////////////
    "Application:NeuroOmegaStimulation int EnableNeuroOmegaStim= 0 0 0 1 "
    " // enable stimulation of NeuroOmega (boolean)",
    "Application:NeuroOmegaStimulation int DynamicStimulation= 0 0 0 1 "
    " // dynamically stimulate, do not check stimulation conditions (boolean)",

    "Application:NeuroOmegaStimulation matrix StimulationConfigurations= "
    "{ Phase%201%20Delay%20[ms] Phase%201%20Amplitude%20[mA] Phase%201%20Width%20[ms] Phase%202%20Delay%20[ms] Phase%202%20Amplitude%20[mA] Phase%202%20Width%20[ms] Frequency%20[Hz] Total%20Duration%20[s] Train%20Duration%20[s] Train%20Frequency%20[Hz]} " // row labels
    "{ Configuration1 Configuration2 } "						 // Column labels
    "1.1   1.1 "	 			  // Phase 1 Delay      (ms)    
    "-3.5  3.5 "					// Phase 1 Amplitude  (mA)                Max total simultaneous stimulation: 100 mA
    "0.5   0.5 "					// Phase 1 Width      (ms)                Min-max: 0.02 - 0.5 (ms)
    "1.5   1.5 "					// Phase 2 Delay      (ms)
    "1.5   -1.5 "				  // Phase 2 Amplitude  (mA)                Max: 15mA (macro) or 0.1mA (micro)
    "0.2   0.2 "					// Phase 2 Width      (ms)
    "10    10 "					  // Frequency          (Hz)                Max: 300 Hz
    "0.1   0.1 "				  // Total duration     (s)                 Max: 1 million samples
    "2     2  "           // Train duration     (s)
    "8     8  ",          // Train frequency    (Hz)

    "Application:NeuroOmegaStimulation matrix StimulationTriggers= "
    "{ Expression Configuration%20ID Channel(s) Return%20Channel} "    // Row labels
    "{ 1 2 } "									              // Column labels
    "StimulusCode==1 StimulusCode==2  "          // Expressions that when true, cause channels to be stimulated
    "{ list 1 1 } { list 2 1 2 }  "			        // Configuration IDs (corresponds to column index in StimulationConfiguration)
    "{ list 1 10016 } { list 2 10017 10018 } "   // Channel(s)
    "-1    -1 ",		                              // Return Channel (-1 for global return)
    END_PARAMETER_DEFINITIONS

    BEGIN_STATE_DEFINITIONS
    "DriveDepth          16 0",
    "AOConnectionPercentage 16 0"
    END_STATE_DEFINITIONS

#ifndef BLOCKS
    BEGIN_STREAM_DEFINITIONS
    "NeuroOmegaTimeStamp 32 0"
    END_STREAM_DEFINITIONS
#else
    //"NeuroOmegaInvalidData 16 0",
    for (int ch = 0; ch < 16; ch++)
    {
      String chTS;
      chTS << "AOChannel" << ch + 1 << "TS 32 0";
      BEGIN_STREAM_DEFINITIONS
        chTS.c_str()
      END_STREAM_DEFINITIONS
    }
#endif
    
        
    BEGIN_EVENT_DEFINITIONS
    "StimMarker1      32 0 0 0",
    "StimMarker1Time  32 0 0 0",
    "Port1            16 0 0 0",
    "Port1Time        32 0 0 0",
    "Port2            16 0 0 0",
    "Port2Time        32 0 0 0",
    END_EVENT_DEFINITIONS

}


// ===========================================================================
// Buffered ADC functions for recording
// ===========================================================================
void
NeuroOmegaADC::OnAutoConfig()
{
  m_enable_stim = OptionalParameter("EnableNeuroOmegaStim", false);
  Parameter("NeuroOmegaMAC") = "A8:1B:6A:23:9A:AA";
  string aoMAC = ActualParameter("NeuroOmegaMAC");
  if (!isValidMacAddress(aoMAC.c_str(), m_ao_mac))
  {
    bcierr << "NeuroOmegaADC Error: Invalid NeuroOmega MAC address" << endl;
    return;
  }

#ifndef TESTING_WITHOUT_DEVICE
  //have to register that we are disconnected
  Sleep(200);
  // establish connection to the device
  int maxTries = 3;
  for (int n = 0; n < maxTries; n++)
  {
    if (!IsConnectedToAO())
    {
      //bciout << "Not connected" << endl;
      if (ConnectToAO(m_ao_mac))
        break; //connected
      //not connected, try to fully disconnect
      if (n == maxTries - 1)
      {
        bciwarn << "Poor connection quality after " << maxTries
          << " attempts. Continuing... \n"
          << "Wait, then press Set Config again to try to get"
          << " a better signal" << endl;
      }
      else
      {
        this->DisconnectFromAO();
        bciwarn << "Poor connection quality, trying again..." << endl;
      }
    }
    else
      break; //connected
  }
#endif
  Sleep(500);

  m_n_channels = Parameter("RecordingChIDs")->NumValues();

  Parameter("SourceCh") = m_n_channels;
  Parameter("SourceChGain")->SetNumValues(m_n_channels);
  Parameter("SourceChOffset")->SetNumValues(m_n_channels);
  Parameter("ChannelNames")->SetNumValues(m_n_channels);
#ifndef TESTING_WITHOUT_DEVICE
  // get channel info
  do {
    //wait to be fully connected
    Sleep(200);
    if (this->IsConnectedToAO())
    {
      if (m_channel_info_map.empty())
      {
        //Has to have all the channels we need to pass, or 10 tries
        //To reconnect is the only way change it
        for (int tries = 0; tries < 5; tries++)
        {
          if (GetChannelInfo())
          {
            if (SetChannelNames())
              break; //success!
          }
          bciwarn << "NeuroOmegaADC: Connection attempt " << tries + 1 << endl;
          if (tries == 2)
            bciwarn << "Will stop at 5 attempts." << endl;
          this->DisconnectFromAO();
          Sleep(200);
          ConnectToAO(m_ao_mac);
          Sleep(200);
        }
      }
      else
        SetChannelNames();
    }
  } while (this->IsConnecting());

  if (Parameter("SaveMPX"))
  {
    //wait to be fully connected
    if (!this->IsConnectedToAO())
    {
      Sleep(200);
    }
    //check file paths are ok
    char* charPath;
    string fPath = Parameter("FilePath").ToString();
    int pathLength = fPath.length();
    string str_obj(fPath);
    charPath = &str_obj[0];
    int res = SetSavePath(charPath, pathLength);
    if (res != eAO_OK)
    {
      bciout << "NeuroOmegaADC Error: Could not set Neuro Omega save path. " <<
        "Error reported from device : " << endl;
      bciout << GetSystemSDKError() << endl;
    }

    char* charName;
    fPath = Parameter("FileName").ToString();
    pathLength = fPath.length();
    string str_obj1(fPath);
    charName = &str_obj1[0];

    if (SetSaveFileName(charName, pathLength) != eAO_OK)
    {
      bciout << "NeuroOmegaADC Error: Could not set Neuro Omega file name. " <<
        "Error reported from device : " << endl;
      bciout << GetSystemSDKError() << endl;
    }
  }
#else
  if (m_channel_info_map.empty())
  {
    GetChannelInfo_DUMMY();
  }
  SetChannelNames();
#endif

  // set whatever parameters that may be set to auto
  Parameter("ChannelInfo")->SetNumRows(m_channel_info_map.size());
  Parameter("ChannelInfo")->SetNumColumns(2 /*ChID & ChNames*/);

  int ch = 0;
  for (auto itr = m_channel_info_map.begin(); itr != m_channel_info_map.end(); itr++)
  {
    Parameter("ChannelInfo")(ch, 0) = itr->first;
    Parameter("ChannelInfo")(ch, 1) = itr->second;
    ch++;
  }

  //must match the sampling rate of the NeuroOmega
  Parameter("SamplingRate") = 1375;
  //block size should be keeping BCI2000 below 50Hz
  Parameter("SampleBlockSize") = 50;
#ifdef BLOCKS
  Parameter("BufferSize") = 20000;
#endif

  
  /*uint32 c;
  GetChannelsCount(&c);
  bciout << "Channels count: " << c << endl;*/
  //remove all data types but 'd' (Data Block)
  char blockTypes[] = { 'h', 'c', 'C', 'L', 'W', '1', 'A', '2', 'E',
                       't', '3', 'T', 's', 'S', 'm', 'M', 'D', 'P'};
  for (int i = 0; i < 18; i++)
  {
    RemoveDataType(blockTypes[i], 1);
  }
  // start stimulation thread
  //bciout << "Before stim thread" << endl;
  if (m_kill_stim_thread)
  {
    m_kill_stim_thread = false;
    m_stim_thread = thread(&NeuroOmegaADC::StimulateAOThread, this);
    m_stim_marker_thread = thread(&NeuroOmegaADC::ReadStimMarkerThread, this);
  }
}

void
NeuroOmegaADC::OnPreflight(SignalProperties& Output) const
{
#ifndef TESTING_WITHOUT_DEVICE
  /*if (!this->IsConnectedToAO())
  {
    WaitForConnection();
    bcierr << "NeuroOmegaADC Error: Unable to connect to NeuroOmega" << endl;
    return;
  }*/

#endif
  // check that all parameters are valid
  Parameter("SamplingRate");
  Parameter("EnablePortInputs");
  State("DriveDepth");
  Parameter("ReadDriveDepth");
  int source_ch = Parameter("SourceCh");
  int recordingChNum = Parameter("RecordingChIDs")->NumValues();
  if (source_ch != recordingChNum)
  {
    bciwarn << "The number of channels specified in RecordingChIDs (" << recordingChNum << ") does not match the number specified in SourceCh (" << source_ch << ")" << endl;
  }

  // verify that mac addresses are of correct length
  string aoMAC = Parameter("NeuroOmegaMAC");
  MAC_ADDR not_used;
  if (!isValidMacAddress(aoMAC.c_str(), not_used))
    bcierr << "NeuroOmegaADC Error: NeuroOmegaMAC (" << aoMAC << ") is not a valid MAC address" << endl;

  // define output properties
  int samplesPerBlock = Parameter("SampleBlockSize");
  int numberOfSignalChannels = Parameter("RecordingChIDs")->NumValues();

#ifndef BLOCKS
  Output = SignalProperties(numberOfSignalChannels + 1, samplesPerBlock, SignalType::int16);
  Output.ChannelLabels()[numberOfSignalChannels] = "@NeuroOmegaTimeStamp";
#else
  Output = SignalProperties(numberOfSignalChannels + m_n_channels, samplesPerBlock, SignalType::int16);
  for (int ch = 0; ch < m_n_channels; ch++)
  {
    Output.ChannelLabels()[numberOfSignalChannels + ch] = "@AOChannel" + to_string(ch + 1) + "TS";
  }
#endif
  //Output.ChannelLabels()[numberOfSignalChannels + m_n_channels] = "@AOConnectionPercentage";
  //Output.ChannelLabels()[numberOfSignalChannels] = "@NeuroOmegaTimeStamp";
  //Output.ChannelLabels()[numberOfSignalChannels+1] = "@NeuroOmegaInvalidData";

}
void
NeuroOmegaADC::OnInitialize(const SignalProperties& Output)
{
#ifndef TESTING_WITHOUT_DEVICE
  if (!this->IsConnectedToAO())
  {
    WaitForConnection();
    if (!this->IsConnectedToAO())
    {
      bcierr << "NeuroOmegaADC Error: Unable to connect to NeuroOmega" << endl;
      return;
    }
  }
#endif
  m_block_size_ms = (((float)Parameter("SampleBlockSize") * 1000) / Parameter("SamplingRate").InHertz());
  m_block_size_samples = Parameter("SampleBlockSize");

  //set size of array for data
  mDataArray.clear();
  mChannelSeen.clear();
  mDataLastTS.clear();
  mChIDS = new int32[m_n_channels];
  for (int ch = 0; ch < m_n_channels; ch++)
  {
    queue<pair<UINT32, int16>> q;
    mChIDS[ch] = (int32)Parameter("RecordingChIDs") (ch);
    mDataArray.insert({ mChIDS[ch] , q});
    mDataLastTS.insert({ mChIDS[ch], 1 });
  }
  mStateArray.clear();
  //stim marker
  if (m_enable_stim)
  {
    if (m_array_stim != nullptr)
    {
      delete[] m_array_stim;
      m_array_stim = nullptr;
    }

    int n_stimMarkers = 1;
    m_array_stim = new int[n_stimMarkers];
    for (int ch = 0; ch < n_stimMarkers; ch++)
    {
      string stimName = "StimMarker " + to_string(ch + 1);
      if (ChannelExists(stimName, &m_array_stim[ch]))
      {
        queue<int16> q;
        mStateArray.insert({ m_array_stim[ch] , q });

#ifndef BLOCKS
        SaveAndBufferChannel(m_array_stim[ch]);
#else
        m_buffer_size = Parameter("BufferSize");
#endif
      }
    }
  }
  // Set the ports as recording
  if ((bool)Parameter("EnablePortInputs"))
  {
    int n_ports = 2;
    int* portArray = new int[n_ports];
    for (int ch = 0; ch < n_ports; ch++)
    {
      string portName = "PORT- " + to_string(ch + 1);
      if (ChannelExists(portName, &portArray[ch]))
      {
        queue<int16> q;
        mStateArray.insert({ portArray[ch] , q });
      }
    }
    delete[] portArray;
  }

#ifndef BLOCKS
  if (!SetRecordingChannels())
    return;
#endif

  //kinda pointless and leads to infrequent errors
  /*if (!TestRecordingChannels())
    return;*/

    // start the acquire thread that checks if we are connected to the hardware
  
  //m_buffer_size = Output.Channels() * Output.Elements() * 8.4;//5.2 ; //conversion factor that seems to work
  //m_buffer_size = 98512;
  ClearBuffers();

  //wait until connection is aight
  /*int q;
  real32 p;
  int n = 0;
  do {
    CheckQualityConnection(&q, &p);
    n++;
    Sleep(100);
  } while (p < 80 && n < 50);*/
}

void 
NeuroOmegaADC::DoAcquire(GenericSignal& Output)
{
#ifdef BLOCKS
  //use GetNextBlock()
  int realSize = 0;
  UINT32 timeStamp = 0;
  while (true)
  {
    //check size
    bool allFull = true;
    for (int ch = 0; ch < m_n_channels; ch++)
    {
      //continue if one of them still needs more data
      if (mDataArray[mChIDS[ch]].size() < m_block_size_samples)
      {
        allFull = false;
        break;
      }
    }
    if (allFull)
      break;
    int16* blockData = new int16[m_buffer_size];
#ifndef TESTING_WITHOUT_DEVICE
    //auto time_start = chrono::steady_clock::now();
    GetNextBlock(blockData, m_buffer_size, &realSize);
    //auto time_end = chrono::steady_clock::now();
    //auto timeDiff = chrono::duration_cast<chrono::milliseconds>(time_end - time_start).count();

#else
    Sleep(m_block_size_ms * (1.0 - 1.0 / 6));
    //Sleep(m_block_size_ms);
    for (int el = 0; el < Output.Elements(); el++)
    {
      for (int ch = 0; ch < m_n_channels; ch++)
      {
        blockData[el * m_n_channels + ch] = (int16)(100 * sin((((float)el * 2.0 * 3.14159) / Output.Elements())));
      }
    }
#endif
    //bciout << "Block time: " << timeDiff << ", actual num of data: " << realSize << endl;
    int i = 0;
    while (i < realSize)
    {
      int blockSize = blockData[i];
      if (mDataArray.count(blockData[i+2]))
      {
        //get each sample
        int chNum = blockData[i+2];
        if (blockSize != 71)
          bciwarn << "Block size: " << blockSize << endl;
        //get time every cycle of channels
        //if ((mChRemain % m_n_channels) == 0)
        //{
          //if (chNum != lastNum)
          //{
          //  //bciout << mChRemain << endl;
          //  lastNum = chNum;
          //}
        UINT32 ts;
        ConvertTimeStamp(blockData[i + 4], blockData[i + 5], ts);
          
        mTimeStamp.push(make_pair(chNum, ts - lastTS));
        if ((ts - lastTS) > m_BLOCK_DELAY && lastTS != 0)
        {
          mTimeOffset += (int64)(((int64)ts - (int64)lastTS) - m_BLOCK_DELAY);// / m_DELTA_T;
          //bciwarn << "Time diff: " << (int64)(((int64)ts - (int64)lastTS) - m_BLOCK_DELAY) /(m_BLOCK_DELAY) << endl;
        }
        lastTS = ts;
        //}
        //mChRemain++;
        if (mDataArray[chNum].empty())
        {
          //map organizes by key lowest to highest
          mChannelSeen.insert({ ts, chNum });
        }
        
        //start at 7 to get past the header
        UINT32 actualTS;
        for (int n = m_HEADER_SIZE; n < blockSize; n++)
        {
          actualTS = ts + m_DELTA_T * (n - m_HEADER_SIZE);
          mDataArray[chNum].push(make_pair(actualTS, blockData[i + n]));
        }
      }
      else if (m_running && mStateArray.count(blockData[i + 2]))
      {
        int chNum = blockData[i + 2];
        string chName = m_channel_info_map[chNum];
        //remove spaces
        std::string::iterator end_pos = std::remove(chName.begin(), chName.end(), ' ');
        chName.erase(end_pos, chName.end());
        //remove '-'
        end_pos = std::remove(chName.begin(), chName.end(), '-');
        chName.erase(end_pos, chName.end());

        UINT32 t;
        ConvertTimeStamp(blockData[i + 4], blockData[i + 5], t);

        bcievent << chName << "Time " << t << " 0" << endl;
        bcievent << chName << " " << chNum << " 0" << endl;
      }
      //go to next block
      i += blockSize;
    }
    delete[] blockData;

    //align data points first time around
    if (firstTime && !mChannelSeen.empty())
    {
      mGlobalTime = (*mChannelSeen.rbegin()).first;
      //bciout << "Last channel: " << (int32)(*mChannelSeen.rbegin()).second << endl;
      for (auto it = mChannelSeen.begin(); it != mChannelSeen.end(); it++)
      {
        std::queue<std::pair<UINT32, int16>>& q = mDataArray[it->second];
        //align time stamps
        while (q.front().first != mGlobalTime)
        {
          q.pop();
        }
        q.pop();
      }
      firstTime = false;
    }
  }

  //just fill output with separate time stamps
  for (int ch = 0; ch < m_n_channels; ch++)
  {
    std::queue<std::pair<UINT32, int16>>& thisQ = mDataArray[mChIDS[ch]];
    for (int el = 0; el < Output.Elements(); el++)
    {
      //timestamp
      Output(m_n_channels + ch, el) = thisQ.front().first;
      //data
      Output(ch, el) = thisQ.front().second;
      thisQ.pop();
    }
  }

  //fill output 
  /*for (int el = 0; el < Output.Elements(); el++)
  {
    bool allDelayed = true;
    int64 timeDiff;
    uint16 invalidChs = 0;
    for (int ch = 0; ch < m_n_channels; ch++)
    {
      auto &p = mDataArray[mChIDS[ch]];
      timeDiff = (int64)((int64)p.front().first - (int64)mGlobalTime - (int64)m_DELTA_T);
      if (timeDiff == 0)
      {
        //good data point
        p.pop();
        Output(ch, el) = p.front().second;
        allDelayed = false;
      }
      else if (timeDiff < 0)
      {
        //good data point
        p.pop();
        Output(ch, el) = p.front().second;
        allDelayed = false;
      }
      //all delayed, so far
      //else if (allDelayed)
      //{

      //}
      //else
      {
        Output(ch, el) = p.front().second;
        invalidChs += 1 << ch;
      }
    }
    if (allDelayed)
    {
      //need to update global time and elements
      UINT32 minTime = INFINITE;
      int validCh;
      for (int ch = 0; ch < m_n_channels; ch++)
      {
        auto& p = mDataArray[mChIDS[ch]];
        p.pop();
        if (p.front().first < minTime)
        {
          validCh = ch;
          minTime = p.front().first;
        }
        minTime = min(minTime, p.front().first);
        Output(ch, el) = p.front().second;
      }
      //mDataArray[mChIDS[validCh]].pop();
      //Output(validCh, el) = mDataArray[mChIDS[validCh]].front().second;
      //invalidChs -= 1 << validCh;
      mGlobalTime = minTime;
      //bciout << "El " << el << ": " << minTime / m_DELTA_T << endl;
    }
    Output(Output.Channels() - 2, el) = mGlobalTime;
    Output(Output.Channels() - 1, el) = invalidChs;
    mGlobalTime += m_DELTA_T;
  }*/

  //for (int ch = 0; ch < m_n_channels; ch++)
  //{
  //  std::queue<std::pair<UINT32, int16>> &thisQ = mDataArray[mChIDS[ch]];
  //  std::pair<UINT32, int16> lastPopped = thisQ.front();
  //  Output(ch, 0) = thisQ.front().second;
  //  if (ch == 0)
  //  {
  //    Output(Output.Channels() - 2, 0) = thisQ.front().first;
  //  }
  //  thisQ.pop();
  //  bool sameTimeStamp = true;
  //  int el = 1;
  //  while (el < Output.Elements())
  //  {
  //    //make sure time stamps are continuous, otherwise fill missing data
  //    //with previous data point
  //    if (((int64)thisQ.front().first - (int64)(lastPopped.first + m_DELTA_T + mTimeOffset)) > 1)
  //    //if (false)
  //    {
  //      //next 64 samples will be missing as well
  //      float pckgsLost = (float)(thisQ.front().first - (lastPopped.first + m_DELTA_T)) / (float)(m_DELTA_T * m_SAMPLE_SIZE);
  //      /*if (pckgsLost > 5)
  //      {
  //        bciwarn << "Channel packages lost: " << pckgsLost << endl;
  //      }*/
  //      //for (int d = 0; d < m_SAMPLE_SIZE; d++)
  //      //{
  //        Output(ch, el) = lastPopped.second;
  //        invalidChs[el] += 1 << ch;
  //      //}
  //      //el += m_SAMPLE_SIZE;
  //        el++;
  //      //mDataLastTS[mChIDS[ch]]++;
  //    }
  //    else
  //    {
  //      //timestamp
  //      Output(Output.Channels() - 2, el) = thisQ.front().first;
  //      Output(ch, el) = thisQ.front().second;
  //      lastPopped = thisQ.front();
  //      thisQ.pop();
  //      //mDataLastTS[mChIDS[ch]] = 1;
  //      el++;
  //    }

  //    /*if (el == Output.Elements() - 1)
  //      bciout << ch << ": " << thisQ.front().first << endl;*/
  //  }

  //}
  //for (int el = 1; el < Output.Elements(); el++)
  //{
  //  Output(Output.Channels() - 1, el) = invalidChs[el];
  //}
  //for (int el = 0; el < Output.Elements(); el++)
  //{
  //  /*if ((el + mElRemain) % m_SAMPLE_SIZE == 0)
  //  {
  //    ts = mTimeStamp.front();
  //    
  //  }*/
  //  ts = mTimeStamp.front();
  //  mTimeStamp.pop();
  //  Output(Output.Channels() - 1, el) = ts.second;
  //}
  
  /*mElRemain = (Output.Elements() + mElRemain) % m_SAMPLE_SIZE;
  mChRemain = mChRemain % m_n_channels;
  */

#else
//use GetAlignedData()
#ifndef TESTING_WITHOUT_DEVICE
  while (m_data_queue.size() < m_block_size_samples)
  {
    if (this->IsConnectedToAO())
    {
      int status = eAO_MEM_EMPTY;
      while (status == eAO_MEM_EMPTY || m_actual_data == 0)
      {
        status = GetAlignedData(m_p_data, mc_array_size, &m_actual_data, m_array_ch, m_n_channels, &m_ts_begin);
      }
    }
    else
    {
      bciwarn << "NeuroOmegaADC: Disconnected unexpectedly. Will continue to gather data in hopes of reconnection." << endl;
      Sleep(10);
      continue;
    }
    // add the returned data to the queue such that an element in the queue is an array of all the channels samples
    // at a particular time point 
    int n_samples_per_ch = m_actual_data / m_n_channels;
    //bciout << "Time (ms): " << timeDiff << ", actual num of data: " << n_samples_per_ch << endl;

    for (int el = 0; el < n_samples_per_ch; el++)
    {
      int16* temp = new int16[mc_array_size];

      for (int ch = 0; ch < m_n_channels; ch++)
      {
        temp[ch] = m_p_data[ch * (n_samples_per_ch)+el]; // m_p_data => [ch0-el0, ch0-el1, ..., ch1-el0, ch1-el1, ...]
      }
      m_data_queue.push(temp);
      m_timeStamp_queue.push((UINT16)m_ts_begin);
    }
    Sleep(1);
  }
#else
    Sleep(m_block_size_ms* (1.0 - 1.0 / 6));
    //Sleep(m_block_size_ms);
    for (int el = 0; el < Output.Elements(); el++)
    {
      int16* temp = new int16[mc_array_size];
      for (int ch = 0; ch < Output.Channels()-1; ch++)
      {
        temp[ch] = (int16)(100 * sin((((float)el * 2.0 * 3.14159) / Output.Elements())));
      }
      m_data_queue.push(temp);
      m_timeStamp_queue.push((UINT16)el);
    }
#endif


  for (int el = 0; el < Output.Elements(); el++)
  {
    int16* ch_array = m_data_queue.front();
    for (int ch = 0; ch < Output.Channels() - 1; ch++)
    {
      Output(ch, el) = ch_array[ch];
    }
    delete[] ch_array;
    m_data_queue.pop();

    //timestamp stream
    Output(Output.Channels() - 1, el) = m_timeStamp_queue.front();
    m_timeStamp_queue.pop();
  }

#endif
}

void 
NeuroOmegaADC::OnStartAcquisition()
{
  firstTime = true;
  mTimeOffset = 0;
}

void NeuroOmegaADC::OnStopAcquisition()
{
  while (!m_data_queue.empty())
  {
    m_data_queue.pop();
  }
  while (!m_timeStamp_queue.empty())
  {
    m_timeStamp_queue.pop();
  }
}

// ===========================================================================
// Generic ADC functions for stimulation
// ===========================================================================

void
NeuroOmegaADC::Preflight(const SignalProperties& Input, SignalProperties& Output) const
{
  BufferedADC::Preflight(Input, Output);
  if (!(bool)OptionalParameter("EnableNeuroOmegaStim", false)) return;

  PreflightCondition(Parameter("StimulationTriggers")->NumRows() == 4);
  GenericSignal preflightSignals(Input);

  //check validity of expressions
  // 
  //check number of rows is what its supposed to be
  if ((Parameter("StimulationConfigurations")->NumRows() != 10) && (Parameter("StimulationConfigurations")->NumRows() != 8))
  {
    bcierr << "NeuroOmegaADC Error: Number of rows in StimulationConfigurations must be 8 or 10" << endl;
  }

  //go through configuration columns
  int n_configurations = Parameter("StimulationConfigurations")->NumColumns();
  for (int col = 0; col < n_configurations; col++)
  {
    //dynamic, don't check conditions
    if (Parameter("DynamicStimulation") == 1)
    {
      for (int i = 0; i < Parameter("StimulationConfigurations")->NumRows(); i++)
      {
        // make sure the expression is valid if it is given as a state
        Expression exp = Parameter("StimulationConfigurations")(i, col);
        exp.Compile();
        exp.Evaluate(&preflightSignals);
      }
    }
    //check stimulation conditions
    else
    {
      //max amplitude is 15mA for macro, or 0.1 mA for micro
      for (int i = 1; i < 5; i += 3)
      {
        double amp1 = abs(Parameter("StimulationConfigurations")(i, col));
        if (amp1 > 15)
        {
          bcierr << "NeuroOmegaADC Error: Stimulation amplitude of " << amp1 << " mA surpasses the maximum stimulation amplitude of 15 mA" << endl;
        }
      }
      //pulse width must be 0.02 - 0.5 ms 
      for (int i = 2; i < 6; i += 3)
      {
        double t1 = Parameter("StimulationConfigurations")(i, col);
        if (t1 > 0.5)
        {
          bcierr << "NeuroOmegaADC Error: Pulse width of " << t1 << " ms surpasses the maximum width length of 0.5 ms" << endl;
        }
        else if (t1 < 0.02)
        {
          bcierr << "NeuroOmegaADC Error: Pulse width of " << t1 << " ms is lower than the minimum width length of 0.02 ms" << endl;
        }
      }
      //max frequency is 300Hz
      double freq = Parameter("StimulationConfigurations")(6, col).InHertz();
      if (freq > 300)
      {
        bcierr << "NeuroOmegaADC Error: Stimulation frequency of " << freq << " Hz surpasses the maximum frequency of 300 Hz" << endl;
      }
      else if (freq < 0)
      {
        bcierr << "NeuroOmegaADC Error: Stimulation frequency of " << freq << " Hz must not be negative" << endl;
      }

      //has train duration and freq
      if (Parameter("StimulationConfigurations")->NumRows() > 8)
      {
        double tFreq = Parameter("StimulationConfigurations")(9, col).InHertz();
        double tDur = Parameter("StimulationConfigurations")(8, col);
        //XOR gate
        if (((tDur == 0) || (tFreq == 0))
          && ((tDur != 0) || (tFreq != 0)))
        {
          bcierr << "NeuroOmegaADC Error: If either Train Duration or Train Frequency is non-zero, the other must be non-zero as well" << endl;
        }

        //check that train duration allows for complete stimulation
        if (fmod(tFreq * tDur, 1) != 0)
        {
          bciwarn << "NeuroOmegaADC Error: Train duration in Configuration " << col + 1 << " allows for " << double(tFreq * tDur) << " pulses. Since this is not a whole number,"
            << " the last stimulation will go over the desired duration" << endl;
        }
        if (tFreq < 0)
        {
          bcierr << "NeuroOmegaADC Error: Train frequency of " << tFreq << " Hz must not be negative" << endl;
        }
        if (tDur < 0)
        {
          bcierr << "NeuroOmegaADC Error: Train duration of " << tDur << " Hz must not be negative" << endl;
        }
      }
    }
  }

  //go through trigger columns
  int n_triggers = Parameter("StimulationTriggers")->NumColumns();
  for (int t = 0; t < n_triggers; t++)
  {
    // make sure the expression is valid
    Expression exp = Parameter("StimulationTriggers")(0, t);
    exp.Compile();
    exp.Evaluate(&preflightSignals);

    //check that ID is valid
    INT8 tID = Parameter("StimulationTriggers")(1, t);
    if (tID < 1 || tID > n_configurations)
      bcierr << "NeuroOmegaADC Error: Column " << t + 1 << " in \"StimulationTriggers\" parameter has invalid configuration ID. Valid configuration IDs are 1-" << n_configurations << endl;
  
    if (Parameter("DynamicStimulation") == 1)
    {
      // make sure channels are valid if expressions
      //ch could be list
      if (Parameter("StimulationTriggers")(2, t)->NumValues() > 1)
      {
        vector<Expression> chs = BCI2000ListToVector<Expression>(Parameter("StimulationTriggers")(2, t));
        for (auto ch = chs.begin(); ch != chs.end(); ++ch)
        {
          ch->Compile();
          ch->Evaluate(&preflightSignals);
        }
      }
      else
      {
        exp = (Expression)Parameter("StimulationTriggers")(2, t);
        exp.Compile();
        exp.Evaluate(&preflightSignals);
      }
      // check return ch is valid
      exp = (Expression)Parameter("StimulationTriggers")(3, t);
      exp.Compile();
      exp.Evaluate(&preflightSignals);
    }
  }
}

void
NeuroOmegaADC::Initialize(const SignalProperties& Input, const SignalProperties& Output)
{
  BufferedADC::Initialize(Input, Output);

  //drive depth
  m_record_driveDepth = !(Parameter("ReadDriveDepth") == 0);
  m_showDriveErr = true;

  //stimulation
  if (!m_enable_stim) return;
  //reset variables
  m_stim_groups.clear();
  m_all_configurations.clear();

  //set up stimulation configurations
  //store all configurations
  int n_configurations = Parameter("StimulationConfigurations")->NumColumns();
  for (int config = 0; config < n_configurations; config++)
  {
    std::vector<Expression> configuration;
    int n_rows = Parameter("StimulationConfigurations")->NumRows();
    for (int j = 0; j < n_rows; j++)
    {
      // make sure the expression is valid if it is given as a state
      Expression exp = Parameter("StimulationConfigurations")(j, config);
      exp.Compile();
      exp.Evaluate();
      configuration.push_back(exp);
    }
    if (n_rows == 8)
    {
      configuration.push_back((Expression)0);
      configuration.push_back((Expression)0);
    }
    m_all_configurations.push_back(configuration);
  }

  //store triggers
  for (int trig = 0; trig < Parameter("StimulationTriggers")->NumColumns(); trig++)
  {
    //bciout << "Group " << trig << ": " << endl;
    int tID = Parameter("StimulationTriggers")(1, trig);
    vector<Expression> channel_list = BCI2000ListToVector<Expression>(Parameter("StimulationTriggers")(2, trig));
    Expression channel_return = Parameter("StimulationTriggers")(3, trig);

    StimulationGroupNO temp_group = StimulationGroupNO::StimulationGroupNO(
      (Expression)Parameter("StimulationTriggers")(0, trig),
      channel_list, channel_return, tID);

    m_stim_groups.push_back(temp_group);
  }
  //M_DELAY = Parameter("CustomDelay");


}

void
NeuroOmegaADC::StartRun()
{
  BufferedADC::StartRun();
#ifndef TESTING_WITHOUT_DEVICE  
  if (Parameter("SaveMPX"))
  {
    StartSave();
  }
#endif
  m_running = true;

  //prevents stimulation from last run
  if (!m_enable_stim) return;
  m_doStim = false;
  State("StimulusCode") = 0;
  for (int trig = 0; trig < m_stim_groups.size(); trig++)
  {
    m_stim_groups[trig].ResetTrigger();
    //for timing of stimulation
    //m_time_start[trig] = chrono::steady_clock::now();
  }
  m_newTrain = true;
}

void
NeuroOmegaADC::Process(const GenericSignal& Input, GenericSignal& Output)
{
  BufferedADC::Process(Input, Output);
#ifndef TESTING_WITHOUT_DEVICE  
  int q;
  real32 p;
  CheckQualityConnection(&q, &p);
  State("AOConnectionPercentage") = (uint16)p;
  //get drive depth in um
  if (m_record_driveDepth)
  {
    int32 depth;
    if (GetDriveDepth(&depth) != eAO_OK)
    {
      if (m_showDriveErr)
      {
        m_showDriveErr = false;
        bciwarn << "NeuroOmegaADC Error: Unable to acquire drive depth from NeuroOmega" << endl;
        bciwarn << "NeuroOmegaADC Error: Error reported from device: " << GetSystemSDKError() << endl;
      }
    }
    else
    {
      m_showDriveErr = true;
      State("DriveDepth") = depth;
    }
  }
#endif
    ////////////////////////////
    //stimulation
    ////////////////////////////
    if (!m_enable_stim) return;

    //bciout << State("Running") << endl;
    if (State("Running") != 0)
    {
      for (auto stim = m_stim_groups.begin(); stim != m_stim_groups.end(); ++stim)
      {
        //enough time has passed where the stimulation has ended
        //only process when expressions are true
        bool value = false;
        for (int i = 0; i < Statevector->Samples() - 1; ++i)
        {
          value = stim->GetExpression().Evaluate(&Input, i);
          if (value)
            break;
        }
        if (!value)
        {
          //expression is not true
          stim->ResetTrigger();
          continue;
        }

        //only stimulate sequence once per when expression is met
        if (stim->AlreadyTriggered())
        {
          //bciout << "Already stimulated for this expression" << endl;
          continue;
        }
        //triggered
        stim->SetTrigger();

        //preparing for stimulation
        vector<Expression> config = m_all_configurations[stim->GetID() - 1];
        vector<Expression> chs = stim->GetChannels();
        vector<int> evalChs;
        for (auto ch = chs.begin(); ch != chs.end(); ++ch)
          evalChs.push_back(ch->Evaluate());
        int evalRetCh = stim->GetReturnChannel().Evaluate();

        //group necessary information for stimulation
        TriggeredStim triggeredStim{ stim - m_stim_groups.begin(), //index of trigger table
                            config[0].Evaluate(), 
                            config[1].Evaluate(),
                            config[2].Evaluate(), 
                            config[3].Evaluate(), 
                            config[4].Evaluate(),
                            config[5].Evaluate(), 
                            config[6].Evaluate(), 
                            config[7].Evaluate(),
                            config[8].Evaluate() * 1000, //ms
                            config[9].Evaluate(),
                            evalChs,
                            evalRetCh
                          };

        m_triggeredList.push(triggeredStim);
        bciout << "Stimulation config: " << triggeredStim;
        m_doStim = true; //flag to run through stimulation thread
      }
    }
    else
    {
      Sleep(0);
    }

    if (m_doStim)
      SetEvent(m_stim_lock);

return;
}

void
NeuroOmegaADC::StopRun()
{
  BufferedADC::StopRun();
  m_running = false;
#ifndef TESTING_WITHOUT_DEVICE  
  if (Parameter("SaveMPX"))
  {
    StopSave();
  }
#endif

  // recording just stopped
  //stops stimulation on all channels
  if (m_enable_stim)
  {
    StopStimulation(-1);
    mClock.Stop();
    m_doStim = false;
    //clear queue
    std::queue<int> empty;
    while (!m_triggeredList.empty())
    {
      m_triggeredList.pop();
    }
  }
}


void
NeuroOmegaADC::Halt()
{
  BufferedADC::Halt();
  //bciout << "Halt" << endl;
  // disconnect from the device completely. Make sure that this function
  // can be called in any state, even if there is no connection to the device

  if (!m_kill_stim_thread)
  {
    m_kill_stim_thread = true;
    m_stim_thread.join();
    m_stim_marker_thread.join();
  }

#ifndef TESTING_WITHOUT_DEVICE
  if (m_enable_stim)
    //stops stimulation on all channels
    StopStimulation(-1);

  if (this->IsConnectedToAO())
  {
    StopSave();

    this->DisconnectFromAO();
  }
#endif
}

// ===========================================================================
// Device specific & helper functions
// ===========================================================================
void
NeuroOmegaADC::StimulateAOThread()
{
  while (!m_kill_stim_thread)
  {
    DWORD res = WaitForSingleObject(m_stim_lock, 20 * m_block_size_ms);
    if (res == WAIT_OBJECT_0)
    {
      while (m_doStim)
      {
        TriggeredStim* stim = &m_triggeredList.front();
        vector<int> channels = stim->chs;

        //train just started
        if (m_newTrain)
        {
          //bciout << "New train" << endl;
          m_newTrain = false;
          if (stim->trainFreq > 0)
          { 
            mClock.SetInterval(Time::Seconds(1.0 / stim->trainFreq));
            mClock.Start();
          }
          m_train_start = chrono::steady_clock::now();
        }
        //stimulate each channel at same time
        for (int ch = 0; ch < channels.size(); ch++)
        {
          double chDelay = M_DELAY * (channels.size() - ch - 1);
          //stimulate
#ifndef TESTING_WITHOUT_DEVICE
          if (StartDigitalStimulation(channels[ch], 
                                      stim->delay1 + chDelay,
                                      stim->amp1,
                                      stim->dur1,
                                      stim->delay2,
                                      stim->amp2,
                                      stim->dur2,
                                      stim->freq,
                                      stim->burstDur,
                                      stim->retCh) != eAO_OK)
          {
            bciwarn << "NeuroOmegaADC Error: Could not stimulate\n"
              << "Error reported from device: " << GetSystemSDKError() << endl;
          }
#endif
        }
        //if there exists a train
        if (stim->trainFreq > 0)
        {
          //wait for period time length
          WithThreadPriority(ThreadUtils::Priority::Maximum - 1) mClock.Wait();
          //mClock.Wait();
        }
        auto time_end = chrono::steady_clock::now();
        auto timeTotalDiff = chrono::duration_cast<chrono::milliseconds>(time_end - m_train_start).count();
        //bciout << "total time: " << timeTotalDiff << ", trainDur: " << stim.trainDur << endl;
        if (timeTotalDiff >= stim->trainDur)
        {
          //move to next train
          //bciout << "Move to next train" << endl;
          m_triggeredList.pop();
          m_newTrain = true;
          mClock.Stop();
        }
        if (m_triggeredList.size() < 1)
          m_doStim = false;
    }
  }
    else
      Sleep(0);
  }
}

void
NeuroOmegaADC::ReadStimMarkerThread()
{
  while (!m_kill_stim_thread)
  {
    //update stimulation events when running
    if (m_enable_stim && m_running)
    {
      int m_other_act_data = 0;
      for (int ch = 0; ch < 1; ch++)
      {
        int16* temp_p_data = new int16[mc_array_size];
#ifndef TESTING_WITHOUT_DEVICE
        int markerRes = GetChannelData(m_array_stim[ch], temp_p_data, mc_array_size, &m_other_act_data);
        if (markerRes == eAO_OK)
        {
          //from doc: byte 9-12 TimeStamp of the first sample of the block you will have to reorder them [byte10 byte9 byte12 byte11]
          int tS1 = temp_p_data[4];
          int tS2 = temp_p_data[5];

          BYTE b9 = tS1 & 0xFF;
          BYTE b10 = (tS1 >> 8) & 0xFF;
          BYTE b11 = tS2 & 0xFF;
          BYTE b12 = (tS2 >> 8) & 0xFF;
          int t = (b10 << 24) + (b9 << 16) + (b12 << 8) + b11;

          //only want to update for one event index
          bcievent << "StimMarker" << ch + 1 << "Time " << (UINT32)t << " 0";
          bcievent << "StimMarker" << ch + 1 << " " << temp_p_data[3] << " 0";
        }
        else if (markerRes != eAO_MEM_EMPTY) //ok if it is empty, means no stimulation has happened yet
        {
          bciout << "NeuroOmegaADC Error: Unable to acquire data from StimMarker " << ch + 1 << endl;
          bciout << "NeuroOmegaADC Error: Error reported from device: " << GetSystemSDKError() << endl;
        }
#else
        bcievent << "StimMarker" << ch + 1 << " " << m_doStim;
#endif
        delete[] temp_p_data;
        Sleep(1);
      }
    }
    else
      Sleep(100);
  }
}

void
NeuroOmegaADC::ConvertTimeStamp(int16 t1, int16 t2, UINT32& t)
{
  //from doc: byte 9-12 TimeStamp of the first sample of the block you will have to reorder them [byte10 byte9 byte12 byte11]
  BYTE b9 = t1 & 0xFF;
  BYTE b10 = (t1 >> 8) & 0xFF;
  BYTE b11 = t2 & 0xFF;
  BYTE b12 = (t2 >> 8) & 0xFF;
  t = (UINT32)((b10 << 24) + (b9 << 16) + (b12 << 8) + b11);
}

void 
NeuroOmegaADC::WaitForConnection()
{
  for (int i = 0; i < 10; i++)
  {
    Sleep(1000);
    if (this->IsConnectedToAO() == eAO_CONNECTED)
      break;
  }
}

bool
NeuroOmegaADC::ConnectToAO(MAC_ADDR _aoMAC)
{
  int nRes = eAO_OK;
  if (this->IsDisconnectedToAO())
  {
    nRes = DefaultStartConnection(&_aoMAC, 0);
  }
  if (nRes == eAO_OK)
  {
    this->WaitForConnection();
    if (this->IsConnectedToAO())
    {
      //check connection
      /*int q;
      real32 p;
      CheckQualityConnection(&q, &p);
      bciout << "Quality: " << q << ", percent: " << p << endl;*/
      return true;
      /*if (q == 1)
        return true;
      Sleep(200);
      return false;*/
    }
    else
    {
      bciwarn << "NeuroOmegaADC Error: Unable to connect to system ("
        << m_ao_mac << ")\n"  
        << "Connection attempt timed out." << endl;
      DisconnectFromAO();
      return ConnectToAO(_aoMAC);
    }
  }
  else
  {
    bcierr << "NeuroOmegaADC Error: Unable to connect to system ("
      << m_ao_mac << ")" << endl;
    bciout << "NeuroOmegaADC Error: StartConnection failed: " << GetSystemSDKError() << endl;
  }
  return false;
}


void
NeuroOmegaADC::DisconnectFromAO()
{
  CloseConnection();
}

void
NeuroOmegaADC::MonitorConnectionStatus()
{
  // This function runs in a separate thread and monitors the connection status with the 
  // NeuroOmega. 
  while (!m_kill_connection_thread)
  {
#ifndef TESTING_WITHOUT_DEVICE
    EAOConnection new_status = (EAOConnection)isConnected(); // their own API doesn't even return their own type...
#else
    EAOConnection new_status = eAO_DISCONNECTED;
#endif
#ifdef BCIDEBUG
    if (new_status != m_connection_status)
    {
      if (new_status == eAO_CONNECTED)
      {
        bciwarn << "NeuroOmegaADC: Connected to system" << endl;
      }
      else if (new_status == eAO_CONNECTING)
      {
        bciwarn << "NeuroOmegaADC: Connecting to system..." << endl;
      }
      else if (new_status == eAO_DISCONNECTED)
      {
        bciwarn << "NeuroOmegaADC: Disconnected from system" << endl;
      }
  }
#endif
    m_connection_status = new_status;
    // check connection every 100 ms
    Sleep(50);
}
}


bool
NeuroOmegaADC::SetRecordingChannels()
{
  BOOL saving_channel;
  const int sleep_time_ms = 10; // NeuroOmega likes to rest between calls

  if (m_array_ch != nullptr)
  {
    delete[] m_array_ch;
    m_array_ch = nullptr;
  }

  do {
    if (!this->IsConnectedToAO())
      Sleep(200);
  } while (this->IsConnecting());
  // To collect data from a particular channel, we must add it as a "Buffered Channel"
  m_array_ch = new int[m_n_channels];
  for (int ch = 0; ch < m_n_channels; ch++)
  {
    m_array_ch[ch] = (int)Parameter("RecordingChIDs")(ch);
    SaveAndBufferChannel(m_array_ch[ch]);
  }
  if (m_p_data != nullptr)
  {
    delete[] m_p_data;
    m_p_data = nullptr;
  }
  m_p_data = new int16[mc_array_size];
  return true;
}

bool
NeuroOmegaADC::SaveAndBufferChannel(int channel)
{
#ifndef TESTING_WITHOUT_DEVICE
  BOOL saving_channel = false;
  // get the port save state 
  if (GetChannelSaveState(channel, &saving_channel) != eAO_OK)
  {
    bcierr << "NeuroOmegaADC Error: Unable to get port \"" << m_channel_info_map[channel]
      << "\" save state." << GetSystemSDKError() << endl;
    return false;
  }
  // set the port save state if it is not already saving
  if (!saving_channel)
  {
    if (SetChannelSaveState(channel, TRUE) != eAO_OK)
    {
      bcierr << "NeuroOmegaADC Error: Unable to set \"" << m_channel_info_map[channel]
        << "\" save state." << GetSystemSDKError() << endl;
      return false;
    }
  }
  // add the port as a buffered
  if (AddBufferChannel(channel, mc_array_size) != eAO_OK)
  {
    bcierr << "NeuroOmegaADC Error: Unable to set \"" << m_channel_info_map[channel]
      << "\" for recording." << GetSystemSDKError() << endl;
    return false;
  }

#endif
  return true;
}

const std::string
GetSystemSDKError()
{
  char sError[1000] = { 0 };
  int  nErrorCount = 0;
  ErrorHandlingfunc(&nErrorCount, sError, 1000);
  return std::string(sError);
}


bool isValidMacAddress(const char* mac, MAC_ADDR& MACaddr)
{
  // check for a valid mac address string, and populate the NeuroOmega defined 
  // structure MAC_ADDR
  int i = 0;  // digit counter
  int s = 0;  // separator counter

  while (*mac)
  {
    if (isxdigit(*mac))
    {
      // we've encountered a digit, lets make sure its valid
      int idx = (int)(i / 2);

      int val = mac[0];
      // convert the ASCII char ('A'=65) to a hex int (0xA=10)
      if ((0x30 <= val) && (val <= 0x39))
      {
        // this char is in '0'-'9'
        val = val - 0x30;
      }
      else
      {
        // this char is in 'A'-'F' (or lowercase)
        val = (val & 0xDF) - 0x30;
        if (val > 9)
          val = val - 7;
        val;
      }
      // check if val \in 0, 1, ..., 15
      if ((0 > val) || (val > 15))
      {
        return false;
      }
      // write to MAC_ADDR depending on if the digit is the more significant byte
      if (i % 2 == 0)
      {
        MACaddr.addr[idx] = 0x00;
        MACaddr.addr[idx] += val << 4;
      }
      else
      {
        MACaddr.addr[idx] += val;
      }
      i++;
    }
    else if (*mac == ':' || *mac == '-')
    {
      // we've encountered a separating character
      if (i == 0 || i / 2 - 1 != s)
        break;
      ++s;
    }
    else {
      s = -1;
    }
    ++mac;
  }
  // make sure we encountered 12 digits and 5 (or 0) separating characters
  return (i == 12 && (s == 5 || s == 0));
}


ostream&
operator<<(ostream& os, const MAC_ADDR& mac)
{
  // print the NeuroOmega defined MAC_ADDR
  for (int i = 0; i < 6; i++)
  {
    if (i == 5)
      os << hex << mac.addr[i];
    else
      os << hex << mac.addr[i] << ":";
  }
  return os;
}

bool
NeuroOmegaADC::GetChannelInfo()
{
  uint32 channel_count = 0;
  SInformation* channel_info;

  //request channel names until it stablizes
  uint32 uChannelsCountPrev = 0;
  GetChannelsCount(&uChannelsCountPrev);

  int max_retry = 50;
  int stableCount = 0;
  for (int i = 0; i < max_retry; i++)
  {
    Sleep(100);
    if (GetChannelsCount(&channel_count) != eAO_OK)
    {
      stableCount = 0;
      continue;
    }

    if (channel_count == uChannelsCountPrev)
    {
      stableCount++;
      if (stableCount == 10)
        break; // No more channels
    }
    else
    {
      stableCount = 0;
      uChannelsCountPrev = channel_count;
    }

    if (i == max_retry-1)
    {
      bciwarn << "Max retry reached" << endl;
      //return false;
    }
  }
  // populate the channel info data structure
  channel_info = new SInformation[channel_count];
  //give it some time to rest
  //Sleep(200);
  if (GetAllChannels(channel_info, channel_count) != eAO_OK)
  {
    return false;
  }
  m_channel_info_map.clear();
  //Sleep(200);
  //bciout << "Updating channels" << endl;
  for (int ch = 0; ch < channel_count; ch++)
  {
    m_channel_info_map[channel_info[ch].channelID] = channel_info[ch].channelName;
  }
  delete[] channel_info;
  return true;  
}

bool NeuroOmegaADC::SetChannelNames()
{
  string missingChNames;
  for (int ch = 0; ch < m_n_channels; ch++)
  {
    Parameter("SourceChGain")(ch) = 1;
    Parameter("SourceChOffset")(ch) = 0;
    string chName = m_channel_info_map[(int32)Parameter("RecordingChIDs")(ch)];
    if (chName == "")
    {
      if (missingChNames.empty())
        missingChNames = Parameter("RecordingChIDs")(ch);
      else
        missingChNames = missingChNames + ", " + Parameter("RecordingChIDs")(ch);
      continue;
    }
    Parameter("ChannelNames")(ch) << m_channel_info_map[(int32)Parameter("RecordingChIDs")(ch)];
  }
  if (!missingChNames.empty())
  {
    m_channel_info_map.clear();
#ifndef BLOCKS
    m_channel_info_map.clear();
    bciwarn << "NeuroOmegaADC Error: Could not get names from channels " << missingChNames << endl;
    return false;
#else
    bciout << "NeuroOmegaADC Error: Could not get names from channels " << missingChNames << endl;
    //dummy channels works better than actual device,
    //but will try the device first
    bcidbg << "Getting dummy channels" << endl;
    GetChannelInfo_DUMMY();
    return SetChannelNames();
#endif
  }
  return true;
}


bool
NeuroOmegaADC::ChannelExists(std::string chName, int* m_arr)
{
  //but what if it is the last element :(
  map<uint32, string>::iterator port_1_info = m_channel_info_map.end();
  for (auto it = m_channel_info_map.begin(); it != m_channel_info_map.end(); ++it)
  {
    // find the ch based on its name
    if (it->second == chName)
      port_1_info = it;
  }
  if (port_1_info == m_channel_info_map.end())
  {
    bciwarn << "NeuroOmegaADC Warning: Unable to record from " << chName << endl;
    return false;
  }
  *m_arr = port_1_info->first;
  return true;
}

void
NeuroOmegaADC::GetChannelInfo_DUMMY()
{
  // Populate the channel info data structure with "DUMMY" info for debugging purposes
  int channel_count = 160;
  SInformation* channel_info;
  channel_info = new SInformation[channel_count];
  channel_info[0] = SInformation{ 10000, "LFP 01" };
  channel_info[1] = SInformation{ 10001, "LFP 02" };
  channel_info[2] = SInformation{ 10002, "LFP 03" };
  channel_info[3] = SInformation{ 10003, "LFP 04" };
  channel_info[4] = SInformation{ 10004, "LFP 05" };
  channel_info[5] = SInformation{ 10005, "Macro LFP 01" };
  channel_info[6] = SInformation{ 10006, "Macro LFP 02" };
  channel_info[7] = SInformation{ 10007, "Macro LFP 03" };
  channel_info[8] = SInformation{ 10008, "Macro LFP 04" };
  channel_info[9] = SInformation{ 10009, "Macro LFP 05" };
  channel_info[10] = SInformation{ 10016, "ECOG LF 1 / 01" };
  channel_info[11] = SInformation{ 10017, "ECOG LF 1 / 02" };
  channel_info[12] = SInformation{ 10018, "ECOG LF 1 / 03" };
  channel_info[13] = SInformation{ 10019, "ECOG LF 1 / 04" };
  channel_info[14] = SInformation{ 10020, "ECOG LF 1 / 05" };
  channel_info[15] = SInformation{ 10021, "ECOG LF 1 / 06" };
  channel_info[16] = SInformation{ 10022, "ECOG LF 1 / 07" };
  channel_info[17] = SInformation{ 10023, "ECOG LF 1 / 08" };
  channel_info[18] = SInformation{ 10024, "ECOG LF 1 / 09" };
  channel_info[19] = SInformation{ 10025, "ECOG LF 1 / 10" };
  channel_info[20] = SInformation{ 10026, "ECOG LF 1 / 11" };
  channel_info[21] = SInformation{ 10027, "ECOG LF 1 / 12" };
  channel_info[22] = SInformation{ 10028, "ECOG LF 1 / 13" };
  channel_info[23] = SInformation{ 10029, "ECOG LF 1 / 14" };
  channel_info[24] = SInformation{ 10030, "ECOG LF 1 / 15" };
  channel_info[25] = SInformation{ 10031, "ECOG LF 1 / 16" };
  channel_info[26] = SInformation{ 10032, "ECOG LF 2 / 01" };
  channel_info[27] = SInformation{ 10033, "ECOG LF 2 / 02" };
  channel_info[28] = SInformation{ 10034, "ECOG LF 2 / 03" };
  channel_info[29] = SInformation{ 10035, "ECOG LF 2 / 04" };
  channel_info[30] = SInformation{ 10036, "ECOG LF 2 / 05" };
  channel_info[31] = SInformation{ 10037, "ECOG LF 2 / 06" };
  channel_info[32] = SInformation{ 10038, "ECOG LF 2 / 07" };
  channel_info[33] = SInformation{ 10039, "ECOG LF 2 / 08" };
  channel_info[34] = SInformation{ 10040, "ECOG LF 2 / 09" };
  channel_info[35] = SInformation{ 10041, "ECOG LF 2 / 10" };
  channel_info[36] = SInformation{ 10042, "ECOG LF 2 / 11" };
  channel_info[37] = SInformation{ 10043, "ECOG LF 2 / 12" };
  channel_info[38] = SInformation{ 10044, "ECOG LF 2 / 13" };
  channel_info[39] = SInformation{ 10045, "ECOG LF 2 / 14" };
  channel_info[40] = SInformation{ 10046, "ECOG LF 2 / 15" };
  channel_info[41] = SInformation{ 10047, "ECOG LF 2 / 16" };
  channel_info[42] = SInformation{ 10128, "SEG 01" };
  channel_info[43] = SInformation{ 10129, "SEG 02" };
  channel_info[44] = SInformation{ 10130, "SEG 03" };
  channel_info[45] = SInformation{ 10131, "SEG 04" };
  channel_info[46] = SInformation{ 10132, "SEG 05" };
  channel_info[47] = SInformation{ 10256, "SPK 01" };
  channel_info[48] = SInformation{ 10257, "SPK 02" };
  channel_info[49] = SInformation{ 10258, "SPK 03" };
  channel_info[50] = SInformation{ 10259, "SPK 04" };
  channel_info[51] = SInformation{ 10260, "SPK 05" };
  channel_info[52] = SInformation{ 10272, "ECOG HF 1 / 01" };
  channel_info[53] = SInformation{ 10273, "ECOG HF 1 / 02" };
  channel_info[54] = SInformation{ 10274, "ECOG HF 1 / 03" };
  channel_info[55] = SInformation{ 10275, "ECOG HF 1 / 04" };
  channel_info[56] = SInformation{ 10276, "ECOG HF 1 / 05" };
  channel_info[57] = SInformation{ 10277, "ECOG HF 1 / 06" };
  channel_info[58] = SInformation{ 10278, "ECOG HF 1 / 07" };
  channel_info[59] = SInformation{ 10279, "ECOG HF 1 / 08" };
  channel_info[60] = SInformation{ 10280, "ECOG HF 1 / 09" };
  channel_info[61] = SInformation{ 10281, "ECOG HF 1 / 10" };
  channel_info[62] = SInformation{ 10282, "ECOG HF 1 / 11" };
  channel_info[63] = SInformation{ 10283, "ECOG HF 1 / 12" };
  channel_info[64] = SInformation{ 10284, "ECOG HF 1 / 13" };
  channel_info[65] = SInformation{ 10285, "ECOG HF 1 / 14" };
  channel_info[66] = SInformation{ 10286, "ECOG HF 1 / 15" };
  channel_info[67] = SInformation{ 10287, "ECOG HF 1 / 16" };
  channel_info[68] = SInformation{ 10288, "ECOG HF 2 / 01" };
  channel_info[69] = SInformation{ 10289, "ECOG HF 2 / 02" };
  channel_info[70] = SInformation{ 10290, "ECOG HF 2 / 03" };
  channel_info[71] = SInformation{ 10291, "ECOG HF 2 / 04" };
  channel_info[72] = SInformation{ 10292, "ECOG HF 2 / 05" };
  channel_info[73] = SInformation{ 10293, "ECOG HF 2 / 06" };
  channel_info[74] = SInformation{ 10294, "ECOG HF 2 / 07" };
  channel_info[75] = SInformation{ 10295, "ECOG HF 2 / 08" };
  channel_info[76] = SInformation{ 10296, "ECOG HF 2 / 09" };
  channel_info[77] = SInformation{ 10297, "ECOG HF 2 / 10" };
  channel_info[78] = SInformation{ 10298, "ECOG HF 2 / 11" };
  channel_info[79] = SInformation{ 10299, "ECOG HF 2 / 12" };
  channel_info[80] = SInformation{ 10300, "ECOG HF 2 / 13" };
  channel_info[81] = SInformation{ 10301, "ECOG HF 2 / 14" };
  channel_info[82] = SInformation{ 10302, "ECOG HF 2 / 15" };
  channel_info[83] = SInformation{ 10303, "ECOG HF 2 / 16" };
  channel_info[84] = SInformation{ 10384, "RAW 01" };
  channel_info[85] = SInformation{ 10385, "RAW 02" };
  channel_info[86] = SInformation{ 10386, "RAW 03" };
  channel_info[87] = SInformation{ 10387, "RAW 04" };
  channel_info[88] = SInformation{ 10388, "RAW 05" };
  channel_info[89] = SInformation{ 10389, "Macro RAW 01" };
  channel_info[90] = SInformation{ 10390, "Macro RAW 02" };
  channel_info[91] = SInformation{ 10391, "Macro RAW 03" };
  channel_info[92] = SInformation{ 10392, "Macro RAW 04" };
  channel_info[93] = SInformation{ 10393, "Macro RAW 05" };
  channel_info[94] = SInformation{ 11016, "ANALOG - IN 1" }; // 8 BNC connectors
  channel_info[91] = SInformation{ 11017, "ANALOG - IN 2" };
  channel_info[92] = SInformation{ 11018, "ANALOG - IN 3" };
  channel_info[93] = SInformation{ 11019, "ANALOG - IN 4" };
  channel_info[94] = SInformation{ 11020, "ANALOG - IN 5" };
  channel_info[95] = SInformation{ 11021, "ANALOG - IN 6" };
  channel_info[95] = SInformation{ 11022, "ANALOG - IN 7" };
  channel_info[96] = SInformation{ 11023, "ANALOG - IN 8" };
  channel_info[97] = SInformation{ 11024, "ADD ANALOG - IN 1" }; // D-sub connector
  channel_info[98] = SInformation{ 11025, "ADD ANALOG - IN 2" };
  channel_info[99] = SInformation{ 11026, "ADD ANALOG - IN 3" };
  channel_info[100] = SInformation{ 11027, "ADD ANALOG - IN 4" };
  channel_info[101] = SInformation{ 11028, "ADD ANALOG - IN 5" };
  channel_info[102] = SInformation{ 11029, "ADD ANALOG - IN 6" };
  channel_info[103] = SInformation{ 11030, "ADD ANALOG - IN 7" };
  channel_info[104] = SInformation{ 11031, "ADD ANALOG - IN 8" };
  channel_info[105] = SInformation{ 11108, "ANALOG - OUT 1" };
  channel_info[106] = SInformation{ 11109, "ANALOG - OUT 2" };
  channel_info[107] = SInformation{ 11110, "ANALOG - OUT 3" };
  channel_info[108] = SInformation{ 11111, "ANALOG - OUT 4" };
  channel_info[109] = SInformation{ 11112, "ANALOG - OUT 5" };
  channel_info[110] = SInformation{ 11113, "ANALOG - OUT 6" };
  channel_info[111] = SInformation{ 11114, "ANALOG - OUT 7" };
  channel_info[112] = SInformation{ 11115, "ANALOG - OUT 8" };
  channel_info[113] = SInformation{ 11120, "Audio 1" };
  channel_info[114] = SInformation{ 11121, "Audio 2" };
  channel_info[115] = SInformation{ 11122, "Audio 3" };
  channel_info[116] = SInformation{ 11123, "Audio 4" };
  channel_info[117] = SInformation{ 11124, "Audio 5" };
  channel_info[118] = SInformation{ 11125, "Audio 6" };
  channel_info[119] = SInformation{ 11126, "Audio 7" };
  channel_info[120] = SInformation{ 11127, "Audio 8" };
  channel_info[121] = SInformation{ 11202, "PORT- 1" }; // 16 bit digital D-sub inputs
  channel_info[122] = SInformation{ 11214, "PORT- 2" };
  channel_info[123] = SInformation{ 11220, "UD InPort  1" };
  channel_info[124] = SInformation{ 11221, "StimMarker 1" };
  channel_info[125] = SInformation{ 11222, "StimMarker 2" };
  channel_info[126] = SInformation{ 11223, "StimMarker 3" };
  channel_info[127] = SInformation{ 11224, "StimMarker 4" };
  channel_info[128] = SInformation{ 11225, "Internal Detection 1" };
  channel_info[129] = SInformation{ 11230, "Internal Use 1" };
  channel_info[130] = SInformation{ 11231, "Internal Use 2" };
  channel_info[131] = SInformation{ 11232, "Internal Use 3" };
  channel_info[132] = SInformation{ 11233, "Internal Use 4" };
  channel_info[133] = SInformation{ 11234, "Internal Use 5" };
  channel_info[134] = SInformation{ 11235, "Internal Use 6" };
  channel_info[135] = SInformation{ 11236, "Internal Use 7" };
  channel_info[136] = SInformation{ 11237, "Internal Use 8" };
  channel_info[137] = SInformation{ 11238, "Internal Use 9" };
  channel_info[138] = SInformation{ 11239, "Internal Use 10" };
  channel_info[139] = SInformation{ 11240, "Internal Use 11" };
  channel_info[140] = SInformation{ 11241, "Internal Use 12" };
  channel_info[141] = SInformation{ 11348, "DIG - IN 1" }; // 4 BNC input connections
  channel_info[142] = SInformation{ 11349, "DIG - IN 2" };
  channel_info[143] = SInformation{ 11350, "DIG - IN 3" };
  channel_info[144] = SInformation{ 11351, "DIG - IN 4" };
  channel_info[145] = SInformation{ 11352, "ADD DIG - IN 1" }; // D-sub connector
  channel_info[146] = SInformation{ 11353, "ADD DIG - IN 2" };
  channel_info[147] = SInformation{ 11354, "ADD DIG - IN 3" };
  channel_info[148] = SInformation{ 11355, "ADD DIG - IN 4" };
  channel_info[149] = SInformation{ 11356, "ADD DIG - IN 5" };
  channel_info[150] = SInformation{ 11357, "ADD DIG - IN 6" };
  channel_info[151] = SInformation{ 11358, "ADD DIG - IN 7" };
  channel_info[152] = SInformation{ 11359, "ADD DIG - IN 8" };
  channel_info[153] = SInformation{ 11360, "ADD DIG - IN 9" };
  channel_info[154] = SInformation{ 11361, "ADD DIG - IN 10" };
  channel_info[155] = SInformation{ 11362, "ADD DIG - IN 11" };
  channel_info[156] = SInformation{ 11363, "ADD DIG - IN 12" };
  channel_info[157] = SInformation{ 11701, "DIG - OUT 1" };
  channel_info[158] = SInformation{ 11707, "ADD DIG - OUT 1" };
  channel_info[159] = SInformation{ 11801, "TS Sync 1" };

  m_channel_info_map.clear();
  for (int ch = 0; ch < channel_count; ch++)
  {
    m_channel_info_map[channel_info[ch].channelID] = string(channel_info[ch].channelName);
  }
  delete[] channel_info;
}
