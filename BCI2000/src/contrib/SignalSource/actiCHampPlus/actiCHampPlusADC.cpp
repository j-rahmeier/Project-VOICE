////////////////////////////////////////////////////////////////////
// $Id: actiCHampPlusADC.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Authors: Alex Belsten belsten@neurotechcenter.org
// Description: Source module for the actiCHampPlusADC
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

#include "actiCHampPlusADC.h"
#include "RawDataHandlerExample.h"
#include "BCIStream.h"
#include "BCIEvent.h"

bool OpenAmplifier (CAmplifier& amp, int ampID);

using namespace std;

RegisterFilter( actiCHampPlusADC, 1 ); 

actiCHampPlusADC::actiCHampPlusADC () :
  m_actiCHampplus (),
  mVis ("ADCFILT"),
  m_block_size (0),
  m_sample_rate (0),
  m_device_number (0),
  m_hardware_sample_rate (0),
  m_doAquire (false),
  m_acqThread (NULL),
  m_device_open (false)
{
}

actiCHampPlusADC::~actiCHampPlusADC()
{
    m_actiCHampplus.Close ();
    m_device_open = false;
    this->OnHalt ();
}

void
actiCHampPlusADC::OnPublish ()
{
    BEGIN_PARAMETER_DEFINITIONS

        "Source:Signal%20Properties int SourceCh= auto "
        "auto 1 % // number of digitized and stored channels ",

        "Source:Signal%20Properties list ChannelNames= 1 auto "
        " // names of amplifier channels",

        "Source:Signal%20Properties int SampleBlockSize= auto "
        "auto 1 % // number of samples transmitted at a time",

        "Source:Signal%20Properties float SamplingRate= auto "
        "auto 0.0 % // sample rate, defined by hardware sample rate and decimation factor",

        "Source:actiCHampPlus int actiCHampAmplifierID= 0 "
        "0 0 10 // Amplifier device to use",

        "Source:actiCHampPlus intlist AUXChList= 8 "
        "1 2 3 4 5 6 7 8 0 1 % // list of AUX channel indices to use",

        "Source:actiCHampPlus int TotalEEGChannels= 32 32 0 % "
        "// number of EEG channels to record from",

        "Source:actiCHampPlus int AcquisitionMode= 1 1 1 2 // mode to initialize the amplifier: "
        "1: normal signal acquisition, 2: impedance measurement(enumeration)",

        "Source:actiCHampPlus int ImpedencePauseTime= 5s "
        "5s 0 % // Amount of time to illuminate LEDS on active electrodes during impedence check",
         
        "Source:actiCHampPlus int SubSampleDivisor= 0 0 0 6 // divisor to trancate data by: "
        "0 1%, 1 2%, 2 5%, 3 10%, 4 20%, 5 50%, 6 100% (enumeration)", 
        // Using <int>% allows the <int> to be shown in GUI

        "Source:actiCHampPlus int HardwareSamplingRate= 1 1 1 3 "
        "// Rate to run the amplifier before sub sampling: 1 10kHz, 2 50kHz, 3 100kHz (enumeration)",

    END_PARAMETER_DEFINITIONS

    /*
    BEGIN_STREAM_DEFINITIONS
        
        "actiChampTrig0 4 0",

        "actiChampTrig1 4 0",
        
    END_STREAM_DEFINITIONS
    */
}

void
actiCHampPlusADC::OnPreflight (SignalProperties& Output) const
{
    int _deviceNum                 = Parameter ("actiCHampAmplifierID");
    int _mode                      = Parameter ("AcquisitionMode");
    auto _auxList                  = Parameter ("AUXChList");
    int _hardwareSampleRateMode    = Parameter ("HardwareSamplingRate");
    int _eegCount                  = Parameter ("TotalEEGChannels");
    int _totalCh                   = Parameter ("SourceCh");
    int _blockSize                 = Parameter ("SampleBlockSize");
    int _impedPause                = Parameter ("ImpedencePauseTime");
    float _subSampleDivisorVals[7] = { 1.0, 2.0, 5.0, 10.0 ,20.0, 50.0, 100.0 }; // defined by hardware
    float _subSampleDivisor        = _subSampleDivisorVals[(int)Parameter ("SubSampleDivisor")];

    int nRes;
    int hardwareBlockSize;
    float sampleRate;
    bool success;

    // check for valid number of channels
    int nEEGChannels  = 0;
    int nTrigChannels = 0;
    int nAUXChannels  = 0;
    int nDIGChannels  = 0;

    std::vector<int> vnChannelTypes;  // array that stores channel types defined in t_ChannelType

    
    for (int i = 0; i < m_available_channels; i++)
    {
        if (m_channel_types[i] == CT_EEG)
            nEEGChannels += 1;
        else if (m_channel_types[i] == CT_TRG)
            nTrigChannels += 1;
        else if (m_channel_types[i] == CT_AUX)
            nAUXChannels += 1;
        else if (m_channel_types[i] == CT_DIG)
            nDIGChannels += 1;
        else {
            bciwarn << "Amp has channels that have not been handled (Channel Type: " << m_channel_types[i] << ")" << endl;
        }
    }

    // Check aux channels for duplicates or too many columns
    if (_eegCount > nEEGChannels)
    {
        bcierr << "Parameter \"TotalEEGChannels\" exceeds " << nEEGChannels
            << " (number of EEG channels available from amp).";
        return;
    }
    if (_auxList->NumValues () > nAUXChannels)
    {
        bcierr << "Parameter \"AuxChList\" size exceeds " << nAUXChannels
            << " (number of auxilary channels available from amp)." << endl;
        return;
    }   

    std::map<int, int> AuxChCount;      // map to check for duplicate channels
    for (int i = 0; i < _auxList->NumValues (); i++)
    {
        if (_auxList (i) > nAUXChannels)
        {
            bcierr << "AuxChList index " << i + 1 << " exceeds the number of available Aux channels" << endl;
            return;
        }  
        else if (AuxChCount.find (_auxList (i)) != AuxChCount.end ())
        {
            bcierr << "Duplicate Aux channel in \"AuxChList\"" << endl;
            return;
        }
        else
            AuxChCount[_auxList (i)] = 1;
    }

    //check that there are 2 trigger channels
    if (nTrigChannels != 2)
        bciwarn << "No trigger channels available. State \"actiChampTrig\" will not be updated" << endl;

    // Check that the correct numbeer of channels will be digitized    
    if (_totalCh != _eegCount + _auxList->NumValues ())
    {
        bcierr << "\"SourceCh\" parameter incorrect. \"SourceCh\" must be equal to the number of"
            << " EEG channels being digitized plus the number of auxilary channels being digitized."
            << " Set \"SourceCh\" to " << _eegCount + _auxList->NumValues ()
            << " or set it to \"auto\"" << endl;
        return;
    }

    Output = SignalProperties (_eegCount + _auxList->NumValues (), _blockSize, SignalType::float32);
}

void 
actiCHampPlusADC::OnAutoConfig (void)
{
    // The user has pressed "Set Config" and some parameters may be set to "auto",
    // indicating that they should be set from the current amplifier configuration.
    // In this handler, we behave as if any parameter were set to "auto", and the
    // framework will transparently make sure to preserve user-defined values.
    auto _auxList                  = Parameter ("AUXChList");
    int _eegCount                  = Parameter ("TotalEEGChannels");
    int _hardwareSampleRateMode    = Parameter ("HardwareSamplingRate").InHertz ();
    float _subSampleDivisorVals[7] = { 1.0, 2.0, 5.0, 10.0 ,20.0, 50.0, 100.0 }; // defined by hardware
    float _subSampleDivisor        = _subSampleDivisorVals[(int)Parameter ("SubSampleDivisor")];
    int nCh                        = _eegCount + _auxList->NumValues ();

    float _hardwareSampleRate;
    if (_hardwareSampleRateMode == 1)
        _hardwareSampleRate = SAMPLING_RATE_10KHZ;
    else if (_hardwareSampleRateMode == 2)
        _hardwareSampleRate = SAMPLING_RATE_50KHZ;
    else if (_hardwareSampleRateMode == 3)
        _hardwareSampleRate = SAMPLING_RATE_100KHZ;

    Parameter ("SamplingRate") = _hardwareSampleRate / _subSampleDivisor;
    Parameter ("SourceCh")     = nCh;
    Parameter ("ChannelNames")->SetNumValues (nCh);
    Parameter ("ImpedencePauseTime") = "5s";

    for (int i = 0; i < _eegCount; ++i)
        Parameter ("ChannelNames")(i) << "Ch" << i + 1;
    for (int i = 0; i < _auxList->NumValues (); i++)
        Parameter ("ChannelNames")(_eegCount + i) << "Aux" << _auxList (i);  

    // open amp
    if (!m_device_open)
    {
        bool success = OpenAmplifier (m_actiCHampplus, m_device_number);
        if (!success) return;
        m_rdh = RawDataHandler (m_actiCHampplus);
        m_device_open = true;
    }
    m_actiCHampplus.GetProperty (m_available_channels, DPROP_I32_AvailableChannels);
    int nChannelType;
    m_channel_types.clear ();
    for (int i = 0; i < m_available_channels; i++)
    {
      m_actiCHampplus.GetProperty (nChannelType, i, CPROP_I32_Type);
      m_channel_types.push_back (nChannelType);
    }

    // Check battery Voltage
    float batVoltage;
    m_actiCHampplus.GetProperty (batVoltage, DPROP_F32_BatteryVoltage);

    bciout << "\nDevice battery at " << batVoltage << " [V]" << endl;
} 

void
actiCHampPlusADC::OnInitialize( const SignalProperties& Output )
{
    m_device_number                = Parameter ("actiCHampAmplifierID");
    int _mode                      = Parameter ("AcquisitionMode");
    int _eeg_count                 = Parameter ("TotalEEGChannels");
    m_sample_rate                  = Parameter ("SamplingRate").InHertz ();
    int _hardwareSampleRateMode    = Parameter("HardwareSamplingRate");
    auto usedAUX                   = Parameter ("AUXChList");
    float _subSampleDivisorVals[7] = { 1.0, 2.0, 5.0, 10.0, 20.0, 50.0, 100.0 }; 
    float _subSampleDivisor        = _subSampleDivisorVals[(int)Parameter ("SubSampleDivisor")];
    m_imped_pauseMS                = Parameter ("ImpedencePauseTime").InMilliseconds();
    m_block_size                   = Output.Elements ();

    int nRes, nChannelType;
    bool success;

    // clear all old data from previous run
    m_eeg_aux_channel_idxs.clear();
    m_trig_channel_idxs.clear();
    m_fData.clear();

    if (_hardwareSampleRateMode == 1)
        m_hardware_sample_rate = SAMPLING_RATE_10KHZ;
    else if (_hardwareSampleRateMode == 2)
        m_hardware_sample_rate = SAMPLING_RATE_50KHZ;
    else if (_hardwareSampleRateMode == 3)
        m_hardware_sample_rate = SAMPLING_RATE_100KHZ;
   
   // store the desired channels index's
   int eegCount = 0;
   std::vector<int> tempAuxChIdxs;
   for (int i = 0; i < m_available_channels; i++)
   {
       nChannelType = m_channel_types[i];
       if (nChannelType == CT_EEG && eegCount < _eeg_count)
       {
           m_eeg_aux_channel_idxs.push_back (i);
           eegCount += 1;
       }
       else if (nChannelType == CT_TRG)
           m_trig_channel_idxs.push_back (i);
       else if (nChannelType == CT_AUX)
           tempAuxChIdxs.push_back (i);
   }

   for (int i = 0; i < usedAUX->NumValues (); i++)
       m_eeg_aux_channel_idxs.push_back (tempAuxChIdxs[usedAUX (i) - 1]);

   // set sampling rate
   nRes = m_actiCHampplus.SetProperty (m_hardware_sample_rate, DPROP_F32_BaseSampleRate);
    if (nRes != AMP_OK)
    {
        bcierr << "Unable to set hardware sampling rate. Try unplugging actiChampPlus and restarting BCI2000" << endl;
        return;
    }

    // set subsample divisor
    nRes = m_actiCHampplus.SetProperty (_subSampleDivisor, DPROP_F32_SubSampleDivisor);
    if (nRes != AMP_OK)
    {
        bcierr << "Unable to set sub sample divisor. Try unplugging actiChampPlus and restarting BCI2000" << endl;
        return;
    }
    if (_mode == RM_IMPEDANCE)
        CheckImpedence ();
    m_actiCHampplus.StartAcquisition (RM_NORMAL);
    ThreadUtils::SleepFor (Time::Seconds (1));
}

void
actiCHampPlusADC::OnStartAcquisition()
{
    m_doAquire = true;
    m_acqThread = new std::thread (&actiCHampPlusADC::AquisitionWorker, this);
}

void
actiCHampPlusADC::DoAcquire( GenericSignal& Output )
{
    WithThreadPriority (ThreadUtils::Priority::Maximum - 1)
    {
        int bufSize = Output.Channels ();
        std::vector<float> buffer (bufSize);
        float elapsed_secs = 0.0;
        while (m_acqQueue.unsafe_size () < m_block_size)
        {
            auto start = std::chrono::high_resolution_clock::now ();
            Sleep (0);
            auto finish = std::chrono::high_resolution_clock::now ();
            std::chrono::duration<double> elapsed = finish - start;
            elapsed_secs += elapsed.count ();

            if (elapsed_secs > 1)
            {
                bcierr << "actiChamp Plus Time-out occured" << endl;
                return;
            }
        }
        for (int el = 0; el < Output.Elements (); el++)
        {
            while (!m_acqQueue.try_pop (buffer));
            for (int ch = 0; ch < Output.Channels (); ch++)
            {
                Output (ch, el) = buffer[ch];
            }
        }
    }
}

void
actiCHampPlusADC::AquisitionWorker ()
{
    std::vector<float>buffer (m_eeg_aux_channel_idxs.size ());
    UINT8 trigValue = 0;
    float sum = 0;
   
    while (m_doAquire) 
    {
        m_rdh.ParseRawData (m_actiCHampplus, m_fData);
        for (int el = 0; el < m_fData.size(); el ++)
        {
            for (int idx = 0; idx < m_eeg_aux_channel_idxs.size (); idx++)  
                buffer[idx] = m_fData[el][m_eeg_aux_channel_idxs[idx]];
            m_acqQueue.push (buffer);
        }
    }
}

void
actiCHampPlusADC::OnStopAcquisition()
{
    m_doAquire = false;
    if (m_acqThread != NULL)
        m_acqThread->join ();
    delete m_acqThread;
    m_acqThread = NULL;
    m_acqQueue.empty ();

    m_actiCHampplus.StopAcquisition ();
    ThreadUtils::SleepFor (Time::Seconds (2));       // allow the amp time to finish acquisition
}

bool
OpenAmplifier (CAmplifier& amp, int ampID)
{
    int nRes;
    AmplifierFamily amplifier_family = eActiChampFamily;
    nRes = SetAmplifierFamily (amplifier_family);
    if (nRes != AMP_OK)
    {
        bcierr << "Error in setting family. Error: " << nRes << std::endl;
        return false;
    }
    
    // search for devices
    std::string sHWDeviceAddress = "";
    char hwi[20];
    strcpy_s (hwi, "USB");
    nRes = EnumerateDevices (hwi, sizeof (hwi), (const char*)sHWDeviceAddress.data (), 0);
    if (nRes < 1)
    {
        bcierr << "No actiCHampPlus devices found. Try unplugging actiChampPlus and restarting BCI2000" << endl;
        return false;
    }
    

    // open an amplifier using CAmplifier class from SDK.h
    // then query the device for serial number and device type
    amp.m_hAmplifier = NULL;
    nRes = amp.Open (ampID);
    if (nRes != AMP_OK)
    {
        bcierr << "Could not open device at ID number " << ampID 
               << ". Try setting configuration again. If that does not work, restart BCI2000" << endl;
        return false;
    }
    return true;
}

void
actiCHampPlusADC::CheckImpedence ()
{
    int nRes;
    std::vector<float> vfImpData;
    std::vector<float> vfImpVals;
    BOOL bChannelSupportImp;
    int32_t electrodeType = 0;
    int nImpChns = 0;
    mVis.Send (CfgID::WindowTitle, "actiCHampPlus Impedance Values");

    // get total number of channels
    int nCh = 0;
    nRes = m_actiCHampplus.GetProperty (nCh, DPROP_I32_AvailableChannels);
    if (nRes != AMP_OK)
    {
        bcierr << "Unable to get total number of channels from amplifier. Try unplugging actiChampPlus and restarting BCI2000" << endl;
        return;
    }
    for (int i = 0; i < nCh; i++)
    {
        nRes = m_actiCHampplus.GetProperty (bChannelSupportImp, i, CPROP_B32_ImpedanceMeasurement);
        if (!bChannelSupportImp || nRes < 0)
            continue;
        nImpChns++;
    }

    if (nImpChns == 0)
    {
        if (nImpChns == 0) mVis.Send ("\r");
        bciwarn << "No impedence data available. Are any electrodes attached?" << endl;
        return;
    }
    // this is how to gather impedence data:
    vfImpVals.resize (nImpChns, -1.0);
    // there are 2*nChns + 1 values of raw data to parse 
    nImpChns = 2 * (nImpChns + 1);
    vfImpData.resize (nImpChns, -1.0);
    int nCnt = 0;
    m_actiCHampplus.StartAcquisition (RM_IMPEDANCE);    
    ThreadUtils::SleepFor (Time::Seconds (m_imped_pauseMS/1000));
    nRes = -1;
    while (nRes <= AMP_OK)
        nRes = m_actiCHampplus.GetData (&vfImpData[0], vfImpData.size () * sizeof (float), vfImpData.size () * sizeof (float));
    float fVal = 0;
    stringstream memostream;
    if (nRes > AMP_OK)
    {
        fVal = vfImpData[0];
        memostream << "Gnd: " << fVal << "\n";
        for (std::vector<float>::iterator it = vfImpData.begin () + 2;
            it != vfImpData.end ();
            it += 2)
        {
            if (it == vfImpData.begin ())
                continue; // skip the reference channel
            fVal = (*(it + 1) < 0) ? *it : *it - *(it + 1);
            memostream << "Ch" << nCnt + 1 << ": " << fVal << "\n";
            nCnt++;
        }
    }
    string memostring = memostream.str ();
    mVis.Send (memostring.c_str ());
    m_actiCHampplus.StopAcquisition ();
}