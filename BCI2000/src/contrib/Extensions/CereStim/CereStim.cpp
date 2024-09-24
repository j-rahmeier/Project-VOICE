////////////////////////////////////////////////////////////////////////////////
// Authors: Alexander Belsten (belsten@neurotechcenter.org)
// Description: BlackRock CereStim implementation
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
//s
// $END_BCI2000_LICENSE$
// 
// Notes:
// - If a new expression evaluates to true, it won't start stimulating until
//   an ID list has completed
////////////////////////////////////////////////////////////////////////////////

#include "CereStim.h"
#include "BCIStream.h"
#include "BCIEvent.h"
#include "ThreadUtils.h"
#include "StringUtils.h"

#define bcidbgOn
//#define NO_DEVICE

using namespace std;

RegisterFilter( CereStim, 3.D );


template <class T>
std::set<T> BCI2000ListToSet(ParamRef ch_parm, bool subtract_one) 
{
  std::set<T> ch_set = {};
  for (int ch = 0; ch < ch_parm->NumValues(); ch++)
  {
    if (subtract_one)
      ch_set.insert((T)ch_parm(ch) - 1);
    else
      ch_set.insert((T)ch_parm(ch));
  }
  return ch_set;
}

template <class T>
std::vector<T> BCI2000ListToVector(ParamRef ch_parm) //, bool subtract_one)
{
  std::vector<T> ch_vector = {};
  for (int ch = 0; ch < ch_parm->NumValues(); ch++)
  {
      ch_vector.push_back((T)ch_parm(ch)); 
  }
  return ch_vector;
}


template <class T>
std::string hexStr(T* data, int len)
{
  std::stringstream ss;
  ss << std::hex;
  ss << std::setw(2) << std::setfill('0') << (int)data[0];
  for (int i(1); i < len; ++i)
    ss << ":" << std::setw(2) << std::setfill('0') << (int)data[i];
  return ss.str();
}

//----------------------------------------------------------------------
// Core BCI2000 API logger implementation
//----------------------------------------------------------------------
CereStim::CereStim() :
  m_enable(false),
  m_last_error(""),
  //thread
  m_sleepThread(true),
  mVis("CereStimWindow"),
  m_kill_status_thread(true),
  mStimIsActive(false),
  mPreload(true),
  mStartAfterUpload(true),
  defaultWait(0),
  defaultReps(1),
  defaultDur(0),
  mUploadState(0)
{
    PublishEnabler("EnableCereStim");
}


CereStim::~CereStim()
{
  m_kill_status_thread = true;
  if (m_status_thread.joinable())
      m_status_thread.join();
  this->Halt();
}


void 
CereStim::Halt()
{
  // disconnect from the device completely. Make sure that this function
  // can be called in any state, even if there is no connection to the device
  if (this->IsConnected())
    this->CloseConnection();

  if (!m_sleepThread)
  {
    m_sleepThread = true;
    CloseHandle(m_sync_lock);
    m_acquire_thread.join();
    if (!mPreload)
    {
      CloseHandle(m_upload_lock);
      m_upload_thread.join();
    }
  }
}


void
CereStim::Publish()
{
  m_enable = OptionalParameter("EnableCereStim", false);
  if (!m_enable)
    return;

#ifdef BCIDEBUG
  bciwarn << "CereStim Warning: This module has been built in Debug mode. The CereStim API is known to be unreliable when used in Debug mode.\n"
    << "MAY NOT WORK AS EXPECTED!" << endl;
#endif

  BEGIN_PARAMETER_DEFINITIONS
    "CereStim:Enable int EnableCereStim= 0 0 0 1 "
    "// (boolean)",

    "CereStim:Enable int ImpedanceMeasurement= 0 0 0 1 "
    "// Measure electrode impedance during initialization (boolean)",

    "CereStim:Enable int DynamicConfiguration= 0 0 0 1 "
    "// Configure stimulation parameters during experiment (boolean)",
  END_PARAMETER_DEFINITIONS

  /* MAX VALUES */
  BEGIN_PARAMETER_DEFINITIONS
   "CereStim:Max%20values int AppliedVoltage= 8 0 0 8 "
     "// compliance voltage level: 0 %204.7V: 1 %205.3V: 2 %205.9V: 3 %206.5V: 4 %207.1V: 5 %207.7V: 6 %208.3V: 7 %208.9V: 8 %209.5V (enumeration)",
   
   "CereStim:Max%20values float AppliedCurrent= auto 0 0 % "
    "// maximum applied current in microamps",
   
   "CereStim:Max%20values float AppliedPhaseCharge= auto 0 0 % "
    "// maximum charge per phase (charge = amplitude*width)",

   "CereStim:Max%20values int AppliedFrequency= auto 0 0 % "
     "// maximum frequency of stimulations",
  END_PARAMETER_DEFINITIONS

  /* STIMULATION WAVEFORMS */
  BEGIN_PARAMETER_DEFINITIONS
   "CereStim:Waveform%20definition matrix StimulationConfigurations= "
     "{ Cathode%20first Number%20of%20pulses Phase%201%20amplitude%20[uA] Phase%202%20amplitude%20[uA] Phase%201%20duration%20[us] "
     "Phase%202%20duration%20[us] Frequency%20[Hz] Interphase%20duration%20[us] Train%20Duration%20[s] Train%20Frequency%20[Hz]} " // row labels
     "{ Configuration1 Configuration2 Configuration3 Configuration4 } "						 // column labels
     "1 0 1 1 "                 // cathode first
     "1 1 1 1 "	   						  // number of pulses
     "100 100 150 200 "					// phase 1 amplitude (uA)
     "100 100 150 200 "					// phase 2 amplitude (uA)
     "100 100 150 200 "					// phase 1 duration  (us)
     "100 100 150 200 "					// phase 2 duration  (us)
     "100 100 50 25 "					// frequency 
     "100 100 150 175 "		    // interphase duration 
     "2 2 0 0 "        // train duration    (s)
     "8 8 0 0 ",        // train frequency   (Hz)

   "CereStim:Waveform%20definition matrix StimulationTriggers= "
     "{ Expression Configuration%20ID(s) Electrode(s) } "                  // row labels
     "{ 1 2 3 4 } "									                                    // column labels
     "StimulusCode==1 StimulusCode==1 StimulusCode==2 StimulusCode==3 " // expression
     "1 2 3 4 "					// configuration ID
    "{ list 2 1 2 } { list 2 3 4 } { list 2 3 4 } { list 1 5 } ",		    // electrodes

    "CereStim:Waveform%20definition string StartExpression= % % % %  "
      "// Expression to trigger stimulation start (only used in DynamicConfiguration mode)",


  END_PARAMETER_DEFINITIONS

  /* READONLY PARMS */
  BEGIN_PARAMETER_DEFINITIONS
    "CereStim:Device%20Info matrix DeviceInfo= 1 1 auto "
      "// Device information (auto populated) (readonly)(noedit)",
  END_PARAMETER_DEFINITIONS


 BEGIN_STATE_DEFINITIONS
   "CereStimStimulation 1 0 0 0",
   "CereStimStimulationSoftware 1 0 0 0",
   "CereStimUploadStatus 16 0 0 0"
 END_STATE_DEFINITIONS
}


void
CereStim::AutoConfig (const SignalProperties& Input)
{
  if (!m_enable)
    return;

  // Set parameters that may be set to auto
  Parameter("AppliedCurrent")     = 65535; 
  Parameter("AppliedPhaseCharge") = 4294967295;
  Parameter("AppliedFrequency")   = 4294967295;
  State("CereStimStimulation");
  State("CereStimStimulationSoftware");
  State("CereStimUploadStatus");

  // open initial connection to cerestim
#ifndef NO_DEVICE
  if (!this->IsConnected())
  {
    if (!this->OpenConnection())
    {
      return;
    }
  }

  // set readonly parms
  BDeviceInfo cerestiminfo;
  if (ErrorOccurred(m_cerestim.readDeviceInfo(&cerestiminfo)))
  {
    bcierr << "CereStim Error: Unable to acquire device information" << endl;
    bciout << "CereStim Response: " << m_last_error << endl;
  }
  BVersion apiVersion;
  if (ErrorOccurred(m_cerestim.libVersion(&apiVersion)))
  {
      bcierr << "CereStim Error: Unable to acquire API version" << endl;
      bciout << "CereStim Response: " << m_last_error << endl;
  }
  string prefix = "0";
  if (size(to_string(apiVersion.minor)) > 1)
      prefix = "";
  string apiVersionFormatted = to_string(apiVersion.major) + "." + prefix + to_string(apiVersion.minor);

  Parameter("DeviceInfo")->SetDimensions(6, 2);
  Parameter("DeviceInfo")(0, 0) = "Main Board Info";
  Parameter("DeviceInfo")(1, 0) = "Module Version Info";
  Parameter("DeviceInfo")(2, 0) = "Module Status Info";
  Parameter("DeviceInfo")(3, 0) = "Protocol Version Info";
  Parameter("DeviceInfo")(4, 0) = "Serial Number";
  Parameter("DeviceInfo")(5, 0) = "API version";
  Parameter("DeviceInfo")(0, 1) = cerestiminfo.mainboardVersion;
  Parameter("DeviceInfo")(1, 1) = hexStr(cerestiminfo.moduleVersion, 16);
  Parameter("DeviceInfo")(2, 1) = hexStr(cerestiminfo.moduleStatus,  16);
  Parameter("DeviceInfo")(3, 1) = cerestiminfo.protocolVersion;
  Parameter("DeviceInfo")(4, 1) = to_string(cerestiminfo.serialNo);
  Parameter("DeviceInfo")(5, 1) = apiVersionFormatted;
#endif
}


void
CereStim::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  Output = Input;
  if (!m_enable)
    return;
  GenericSignal preflightSignals(Input);

  // verify that all parameters are valid
 
  // first verify the dimensionality of StimulationConfigurations and StimulationTriggers
  PreflightCondition(Parameter("StimulationConfigurations")->NumRows() <= cID::cLen);
  if (Parameter("DynamicConfiguration") == 0)
    PreflightCondition(Parameter("StimulationConfigurations")->NumColumns() < MAXMODULES);
  PreflightCondition(Parameter("StimulationTriggers")->NumRows() == tID::tLen);

  // check the StimulationConfigurations matrix for validity
  int n_configurations = Parameter("StimulationConfigurations")->NumColumns();
  int n_parameters_per_configuration = Parameter("StimulationConfigurations")->NumRows();
  for (int c = 0; c < n_configurations; c++)
  {
    // check that cathode first ==1 || == 0
    if (((int)Parameter("StimulationConfigurations")(cID::cath, c) != 0) && ((int)Parameter("StimulationConfigurations")(cID::cath, c) != 1))
    {
      bcierr << "CereStim Error: \"Cathode First\" row in \"StimulationConfigurations\" parameter must only contain 0 or 1" << endl;
    }

    int phs1 = Parameter("StimulationConfigurations")(cID::amp1, c);
    int phs2 = Parameter("StimulationConfigurations")(cID::amp2, c);
    if (phs1 % 100 != 0)
      bcierr << "CereStim Error: Phase 1 amplitude must be in 100 uA increments. " << phs1 << "uA in " << Parameter("StimulationConfigurations")->ColumnLabels()[c] << " is incompatable.";
    if (phs2 % 100 != 0)
      bcierr << "CereStim Error: Phase 2 amplitude must be in 100 uA increments. " << phs2 << "uA in " << Parameter("StimulationConfigurations")->ColumnLabels()[c] << " is incompatable.";

    // check that configuration parameters are > 0
    for (int i = 1; i < cID::cLen - 2; i++)
    {
      if ((double)Parameter("StimulationConfigurations")(i, c) <= 0)
      {
        bcierr << "CereStim Error: Row " << i + 1 << " in \"StimulationConfigurations\" parameter must be greater than 0" << endl;
      }
    }
    //check the train freqs and duration are valid
    if (n_parameters_per_configuration > cID::cLen - 2)
    {
      //train duration but no train freq
      if (n_parameters_per_configuration < cID::cLen)
      {
        bcierr << "CereStim Error: \"Train Duration\" is specified but \"Train Frequency\" is not specified, both in \"StimulationConfigurations\" parameter" << endl;
      }
      double tF = Parameter("StimulationConfigurations")(cID::tF, c);
      double tDur = Parameter("StimulationConfigurations")(cID::tDur, c);
      if (tF < 0)
        bcierr << "CereStim Error: \"Train Frequency\" in Configuration" << c + 1 << " must be greater or equal to 0" << endl;
      if (tDur < 0)
        bcierr << "CereStim Error: \"Train Duration\" in Configuration" << c + 1 << " must be greater or equal to 0" << endl;
      if (tF == 0 && tDur > 0)
      {
        //gotta have both 0 or train freq cant be 0
        bcierr << "CereStim Error: Row \"Train Frequency\" in \"StimulationConfigurations\" parameter must be greater than 0"
        << " when Row \"Train Duration\" is specified" << endl;
      }
      if (tDur == 0 && tF > 0)
      {
        //warn if train freq is specified (indefinite)
        bciwarn << "CereStim warning: Stimulation Configuration " << c + 1 << " will stimulate indefinitely until the run is manually stopped." << endl;
      }
      
      //burst duration = num of pulses / pulse freq (s) + pulse duration
      double burstDuration = (Parameter("StimulationConfigurations")(cID::n, c) - 1) / Parameter("StimulationConfigurations")(cID::f, c) * 1e6 //us
                          + Parameter("StimulationConfigurations")(cID::dur1, c) + Parameter("StimulationConfigurations")(cID::dur2)
                          + Parameter("StimulationConfigurations")(cID::iDur);

      double pulseDuration = 1.0 / tF * 1e6; //us
      if (pulseDuration < burstDuration)
      {
        bcierr   << "CereStim Error: Configuration" << c+1 << "- Burst takes " << burstDuration / 1e6 << " s and is triggered every " << pulseDuration / 1e6
                << " s according to the \"Train Frequency\". These settings are incompatable." << endl;
      }
    }
  }

  // check the StimulationTriggers matrix for validity
  int n_total_triggers = Parameter("StimulationTriggers")->NumColumns();
  for (int t = 0; t < n_total_triggers; t++)
  {
    // make sure the expression is valid
    Expression trigger_expression = Parameter("StimulationTriggers")(tID::exp, t).ToString();
    trigger_expression.Compile();
    trigger_expression.Evaluate(&preflightSignals);

    //check that ID list are valid
    if (Parameter("StimulationTriggers")(tID::id, t)->NumValues() > 1)
      bcierr << "CereStim Error: Only 1 ID can be specified for a given trigger in \"StimulationTriggers\". Conver to a single value" << endl;
    UINT8 id = Parameter("StimulationTriggers")(tID::id, t);
    if ((id < 1) || id > n_configurations)
      bcierr << "CereStim Error: Column " << t + 1 << " in \"StimulationTriggers\" parameter has invalid configuration ID. Valid configuration IDs are 1-" << n_configurations << endl;

   
    // make sure the electrode list is valid
    std::set<UINT8> el_list = BCI2000ListToSet<UINT8>(Parameter("StimulationTriggers")(tID::el, t), false);
    if (el_list.size() != Parameter("StimulationTriggers")(tID::el, t)->NumValues())
      bciwarn << "CereStim Error: Column " << t + 1 << " in \"StimulationTriggers\" has duplicate electrodes in it's list. Ignoring duplicates" << endl;
    // check that there are electrodes
    if (el_list.size() == 0)
      bcierr << "CereStim Error: Column " << t + 1 << " in \"StimulationTriggers\" has empty electrode list" << endl;
    if (el_list.size() > 15)
      bcierr << "CereStim Error: Column " << t + 1 << " in \"StimulationTriggers\" has more than 15 electrodes in list. Maximum number of concurrently stimulated electrodes is 15" << endl;
    // check that electrodes are valid
    for (auto itr = el_list.begin(); itr != el_list.end(); itr++)
    {
      if ( (*itr < 1) || (*itr > 96) )
        bcierr << "CereStim Error: Column " << t + 1 << " in \"StimulationTriggers\" contains invalid electrode" << endl;
    }
  }
  //validate start expression
  if (Parameter("DynamicConfiguration") == 1)
  {
    Expression startExp = Parameter("StartExpression").ToString();
    startExp.Compile();
    startExp.Evaluate(&preflightSignals);
  }

  Parameter("SampleBlockSize");
  Parameter("SamplingRate").InHertz();
  Parameter("ImpedanceMeasurement");
}


void
CereStim::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
  // all parameters are (maybe) valid, get the device ready for the upcoming run.
  if (!m_enable) return;
  //impedance measurement mode
#ifndef NO_DEVICE
  if (Parameter("ImpedanceMeasurement") == 1)
  {
    int n_triggers = Parameter("StimulationTriggers")->NumColumns();
    std::set<UINT8> stim_electrodes;
    for (int t = 0; t < n_triggers; t++)
    {
      std::set<UINT8> el_list = BCI2000ListToSet<UINT8>(Parameter("StimulationTriggers")(2, t), false);
      std::merge(el_list.begin(), el_list.end(), stim_electrodes.begin(), stim_electrodes.end(),
        std::inserter(stim_electrodes, stim_electrodes.begin()));
    }
    //get electrode impedance
    BTestElectrodes elStruct;
    m_cerestim.testElectrodes(&elStruct);
    UINT32* impedances = elStruct.impedance;
    auto itr = stim_electrodes.begin();

    //print to user for channels we are stimulating at
    mVis.Send(CfgID::WindowTitle, "CereStim Impedance Values");
    std::stringstream memostream;
    for (int el = 0; el < MAXCHANNELS; el++)
    {
      if (*itr == el)
      {
        memostream << "Electrode " << el << ": " << impedances[el] << " Ohms\r";
        itr++;
      }
      if (itr == stim_electrodes.end())
        break;
    }
    mVis.Send(memostream.str());
    mVis.Send("\r");
  }
#endif

  bool allGood = true;
#ifndef NO_DEVICE
  if (!this->IsConnected())
  {
    bcierr << "CereStim Error: Not connected to CereStim" << std::endl;
    allGood = false;
    return;
  }

  UINT8 module_status[16];
  if (ErrorOccurred(m_cerestim.getModuleStatus(module_status)))
  {
    bcierr << "CereStim Error: Unable to get module information" << endl;
    allGood = false;
    return;
  }
#endif

  mPreload = Parameter("DynamicConfiguration") == 0;
  if (!mPreload)
  {
    mStartAfterUpload = Parameter("StartExpression") == "";
    mStartExpression = (Expression)Parameter("StartExpression");
  }

  // load configurations onto device and store other parameters in data structure
  int n_configurations = Parameter("StimulationConfigurations")->NumColumns();
  std::vector<StimPattern> pats;
  std::vector<DynamicPattern> dyPats;
  for (int c = 0; c < n_configurations; c++)
  {
    BConfig configID = (BConfig)(c + 1);
    BWFType cath_first = (Parameter("StimulationConfigurations")(cID::cath, c) == 1) ? BWF_CATHODIC_FIRST : BWF_ANODIC_FIRST;
    UINT8   n_pulses = Parameter("StimulationConfigurations")(cID::n, c);
    UINT16  amp1 = Parameter("StimulationConfigurations")(cID::amp1, c);
    UINT16  amp2 = Parameter("StimulationConfigurations")(cID::amp2, c);
    UINT16  phase1 = Parameter("StimulationConfigurations")(cID::dur1, c);
    UINT16  phase2 = Parameter("StimulationConfigurations")(cID::dur2, c);
    UINT32  frequency = Parameter("StimulationConfigurations")(cID::f, c);
    UINT16  inter_phase = Parameter("StimulationConfigurations")(cID::iDur, c);

#ifndef NO_DEVICE
    if (mPreload)
    {
      if (module_status[configID] == BMODULE_DISABLED)
      {
        // enable the module
        if (ErrorOccurred(m_cerestim.enableModule(configID)))
        {
          bcierr << "CereStim Error: Unable to enable module " << configID << endl;
          allGood = false;
        }
      }
      else if (module_status[configID] == BMODULE_UNAVAILABLE)
      {
        bcierr << "CereStim Error: Module " << configID << " is unavailable" << endl;
        allGood = false;
      }
    }
#endif

    //total time config runs for, number of trains decided by train period
    double trainPeriod_ms = 0; //ms
    double numPulses = defaultReps;
    double burstDur_us = (double)n_pulses / (double)frequency * 1e6 + phase1 + phase2 + inter_phase;
    double totalDuration = burstDur_us * 1e-6; //if no train, this is the stimulation duration
    if (Parameter("StimulationConfigurations")->NumRows() >= cID::cLen - 2 &&
        Parameter("StimulationConfigurations")(cID::tF,c) != 0)
    {
      trainPeriod_ms = 1.0 / Parameter("StimulationConfigurations")(cID::tF, c).ToNumber() * 1000; //ms
      //check if we are stimulating indefinitely
      if ((double)Parameter("StimulationConfigurations")(cID::tDur, c) == 0)
      {
        numPulses = 0;
        totalDuration = defaultDur;
      }
      else
      {
        totalDuration = Parameter("StimulationConfigurations")(cID::tDur, c); //s
        numPulses = (totalDuration * 1e6 - burstDur_us) * Parameter("StimulationConfigurations")(cID::tF, c) * 1e-6;
        double simplifiedNumTrains = totalDuration * Parameter("StimulationConfigurations")(cID::tF, c);

        double intPart;
        if (numPulses <= 0)
        {
          //set stimulation duration if no train
          totalDuration = burstDur_us * 1e-6;
          numPulses = 1;
        }
        //check train duration allows for exact number of bursts
        //allow user to either account for burst duration or not
        else if (abs(round(numPulses) - numPulses) > 1e-6) //us precision
        {
          //if they didn't account for burst duration, allow it
          if (abs(round(simplifiedNumTrains) - simplifiedNumTrains) > 1e-6) //us precision
          {
            double newDuration = round(numPulses) / Parameter("StimulationConfigurations")(cID::tF, c) + burstDur_us * 1e-6;
            //double newDuration = Parameter("StimulationConfigurations")(cID::tDur, c) + 1 / Parameter("StimulationConfigurations")(cID::tF, c);
            bciwarn << "Train duration of " << Parameter("StimulationConfigurations")(cID::tDur, c) << " s will be rounded to " << newDuration
              << " to allow for " << round(numPulses) << " bursts in the train. Please adjust accordingly if not desired" << endl;

            Parameter("StimulationConfigurations")(cID::tDur, c) = newDuration;
            totalDuration = newDuration * 1e3;
            numPulses = round(numPulses);
          }
          else
          {
            //seems like user gave duration without burst duration
            numPulses = simplifiedNumTrains;
          }
        }
      }
    }
    //calculate params
    StimPattern p;
    p.moduleId = configID;
    p.wait_ms = max((int)(trainPeriod_ms - burstDur_us / 1000), 0);
    p.trainReps = ceil(numPulses);
    p.totalDuration = totalDuration * 1e3;
    pats.push_back(p);

    //save configuration
    if (mPreload)
    {
#ifndef NO_DEVICE
      if (ErrorOccurred(
        m_cerestim.configureStimulusPattern(
          configID,
          cath_first,
          n_pulses,
          amp1,
          amp2,
          phase1,
          phase2,
          frequency,
          inter_phase)))
      {
        bcierr << "CereStim Error: Configuration associated with ID " << configID << " is invalid. Double check that all stimulation parameters are valid. Error reported from device: " << m_last_error << endl;
        allGood = false;
      }
#endif
    }
    else
    {
      DynamicPattern tempPattern{ cath_first, n_pulses, amp1, amp2, phase1, phase2, frequency, inter_phase};
      dyPats.push_back(tempPattern);
    }
  }
  if (!allGood) return;

#ifndef NO_DEVICE
  if (!mPreload)
  {
    //enable all possible modules in case we need them all
    for (int configID = 0; configID < 16; configID++)
    {
      if (module_status[configID] == BMODULE_DISABLED)
      {
        // enable the module
        if (ErrorOccurred(m_cerestim.enableModule(configID)))
        {
          bcierr << "CereStim Error: Unable to enable module " << configID << endl;
          allGood = false;
        }
      }
    }
  }
#endif
  
  int n_total_triggers = Parameter("StimulationTriggers")->NumColumns();
  mConfigurations.clear();
  for (int t = 0; t < n_total_triggers; t++)
  {
    Expression trigger_expression = Parameter("StimulationTriggers")(tID::exp, t).ToString();
    std::set<UINT8> el_list  = BCI2000ListToSet<UINT8>(Parameter("StimulationTriggers")(tID::el, t), false);

    StimPattern p = pats[Parameter("StimulationTriggers")(tID::id, t) - 1];
    StimConfiguration trigConfig { trigger_expression, el_list, p };
    if (!mPreload)
      trigConfig.dynamicPattern = dyPats[Parameter("StimulationTriggers")(tID::id, t) - 1];
    trigConfig.configNum = t+1;
    trigConfig.uploaded = false;
    mConfigurations.push_back(trigConfig);
  }

  //Initialize the CereStimThread if haven't already been initialized 
  if (m_sleepThread)
  {
    m_sleepThread = false;
    m_sync_lock = CreateEvent(NULL, false, false, NULL);
    m_acquire_thread = thread(&CereStim::CereStimThread, this);
    if (!mPreload)
    {
      m_upload_lock = CreateEvent(NULL, false, false, NULL);
      m_upload_thread = thread(&CereStim::ConfigurationUploadThread, this);
    }
  }
  //status thread
  if (m_kill_status_thread)
  {
    m_kill_status_thread = false;
    m_status_thread = thread(&CereStim::SequenceRunning, this);
  }
}


void
CereStim::StartRun()
{ 
  // The user has just pressed "Start" (or "Resume")
  if (!m_enable) return;
  bcidbg << "Start run bug" << std::endl;
  State("CereStimStimulation") = 0;
  State("CereStimStimulationSoftware") = 0;
  State("CereStimUploadStatus") = 0;
  mAlreadyTriggered = 0;
  mAlreadyUploaded = false;
  ClearActiveConfiguration();
}

void
CereStim::Process( const GenericSignal& Input, GenericSignal& Output )
{
  Output = Input; 
  if (!m_enable) return;
  
  //can't upload or stim if stim is active
  if (!mStimIsActive)
  {
    if (mPreload)
    {
      uint16_t counter = 0;
      //check triggers for stimulation
      ClearActiveConfiguration();
      for (auto it = mConfigurations.begin(); it != mConfigurations.end(); it++)
      {
        if (EvaluateExpression(&(it->stimTrigger), &Input))
        {
          if (!AlreadyTriggered(mAlreadyTriggered, counter))
          {
            mStimIsActive = true;
            SetConfiguration(&(*it));
          }
        }
        else if ((mAlreadyTriggered >> counter) & 1)
          //triggered, but now not true
          mAlreadyTriggered = mAlreadyTriggered & ~(1 << counter); //set as not triggered

        counter++;
      }
    }
    else
    {
      //check triggers for uploading configurations
      bool upload = false;
      for (auto it = mConfigurations.begin(); it != mConfigurations.end(); it++)
      {
        if (EvaluateExpression(&(it->stimTrigger), &Input))
        {
          if (!it->uploaded)
          {
            it->uploaded = true;
            mUploadConfigurations.push_back(&(*it));
            upload = true;
          }
        }
        else if (it->uploaded)
          //uploaded, but now expression is not true
          it->uploaded = false;
      }
      if (upload)
      {
        SetEvent(m_upload_lock);
        if (mStartAfterUpload)
          mStimIsActive = true;
      }

      //check start expression for dynamic configuration
      if (!mStartAfterUpload)
      {
        if (EvaluateExpression(&mStartExpression, &Input))
        {
          if (mAlreadyTriggered == 0)
          {
            if (mUploadConfigurations.empty() && mActiveStimulation.electrodePattern.empty())
            {
              //nothing uploading or ready for stim
              bciwarn << "CereStim warning: Stimulation requested without any configuration uploaded. "
                << "Please upload the configuration first, or do not use dynamic loading mode" << endl;
            }
            else
            {
              //trigger
              mStimIsActive = true;
            }
            mAlreadyTriggered = 1;
          }
        }
        else if (mAlreadyTriggered == 1)
        {
          //reset trigger
          mAlreadyTriggered = 0;
        }
      }
    }

    //trigger stim thread
    if (mStimIsActive)
    {
      //check if we start if doing dynamic loading
      if (mPreload || !mStartAfterUpload)
      {
        SetEvent(m_sync_lock);
      }
    }
  }
  //Updates states here
  State("CereStimStimulation") = mStimState;
  State("CereStimStimulationSoftware") = mStimIsActive;;
  State("CereStimUploadStatus") = mUploadState;
}


void
CereStim::StopRun()
{
  if (!m_enable) return;
  State("CereStimStimulation") = 0;
  State("CereStimStimulationSoftware") = 0;
  if (mStimIsActive || mStimState)
    m_cerestim.stop();
  mStimIsActive = false;
}


//----------------------------------------------------------------------
// other device-specific BCI2000 member functions
//----------------------------------------------------------------------
bool CereStim::EvaluateExpression(Expression* exp, const GenericSignal* Input)
{
  for (int i = 0; i < Statevector->Samples() - 1; ++i)
  {
    if (exp->Evaluate(Input, i)) return true;
  }
  return false;
}

bool CereStim::AlreadyTriggered(uint16_t& exp, uint16_t& n)
{
  if ((exp >> n) & 1)
    //this expression has already been triggered
    return true;
  else
    exp = exp | (1 << n); //set as triggered
  return false;
}

void CereStim::ClearActiveConfiguration()
{
  if (!mActiveStimulation.electrodePattern.empty())
  {
    mActiveStimulation.electrodePattern.clear();
  }
  mActiveStimulation.wait_ms = &defaultWait;
  mActiveStimulation.trainReps = &defaultReps;
  mActiveStimulation.totalDuration = &defaultDur;
  mUploadState = 0;
}
void CereStim::SetConfiguration(StimConfiguration* config)
{
  //initialize mActiveStimulation
  for (auto el = config->electrodes.begin(); el != config->electrodes.end(); el++)
  {
    //store config for each electrode
    mActiveStimulation.electrodePattern.insert({ *el, config->pattern.moduleId });
  }
  //update wait time for train frequency
  if (mActiveStimulation.wait_ms == &defaultWait)
  {
    mActiveStimulation.wait_ms = &config->pattern.wait_ms;
  }
  else if (*mActiveStimulation.wait_ms != config->pattern.wait_ms)
  {
    //take smallest wait time to best match desired train frequency
    if (config->pattern.wait_ms < *mActiveStimulation.wait_ms)
    {
      mActiveStimulation.wait_ms = &config->pattern.wait_ms;
    }
  }
  //update train repetitions
  if (mActiveStimulation.trainReps != &defaultReps
    && *mActiveStimulation.trainReps != config->pattern.trainReps)
    bciwarn << "CereStim warning: Configurations triggered at same time must have matching train repetitions. " 
      << "Changing to " << config->pattern.trainReps << " repetitions" << endl;
  mActiveStimulation.trainReps = &config->pattern.trainReps;

  //wait for the maximum amount of stimulation time
  if (*mActiveStimulation.totalDuration < config->pattern.totalDuration)
    mActiveStimulation.totalDuration = &config->pattern.totalDuration;

  //set upload state, add if there are multiple configs being uploaded
  mUploadState += config->configNum;
}

void CereStim::ConfigurationUploadThread()
{
  while (!m_sleepThread)
  {
    DWORD res = WaitForSingleObject(m_upload_lock, 1000);
    if (res == WAIT_OBJECT_0)
    {
      ClearActiveConfiguration();
      uint16_t configIDcounter = BCONFIG_1;
      for (auto it = mUploadConfigurations.begin(); it != mUploadConfigurations.end(); it++)
      {
        //upload config and update ID
        (*it)->pattern.moduleId = (BConfig) configIDcounter;
#ifndef NO_DEVICE
        if (ErrorOccurred(
          m_cerestim.configureStimulusPattern(
            (*it)->pattern.moduleId,
            (*it)->dynamicPattern.cath_first,
            (*it)->dynamicPattern.n_pulses,
            (*it)->dynamicPattern.amp1,
            (*it)->dynamicPattern.amp2,
            (*it)->dynamicPattern.phase1,
            (*it)->dynamicPattern.phase2,
            (*it)->dynamicPattern.frequency,
            (*it)->dynamicPattern.inter_phase)))
        {
          bcierr << "CereStim Error: Could not update new configuration. Make sure we are not currently stimulating. "
            << " Error reported from device: " << m_last_error << endl;
        }
#endif
        configIDcounter++;
        //initialize active configuration
        SetConfiguration(*it);
      }
      mUploadConfigurations.clear();
      if (mStartAfterUpload)
      {
        SetEvent(m_sync_lock);
      }
    }
  }
}

void CereStim::CereStimThread()
{

  while (!m_sleepThread)
  {
    DWORD res = WaitForSingleObject(m_sync_lock, 1000);
    if (res == WAIT_OBJECT_0)
    {
#ifndef NO_DEVICE
      //begin creating stimulation sequence
      if (ErrorOccurred(m_cerestim.beginningOfSequence()))
      {
        bciwarn << "CereStim Warning: Unable to start stimulation sequence. "
          << "CereStim reported: " << m_last_error << endl;
        continue;
      }
      //group for simultaneous stimulation
      if (ErrorOccurred(m_cerestim.beginningOfGroup()))
      {
        bciwarn << "CereStim Warning: Unable to start stimulation group. "
          << "CereStim reported: " << m_last_error << endl;
        continue;
      }
      //configure all electrodes
      for (auto pat = mActiveStimulation.electrodePattern.begin(); pat != mActiveStimulation.electrodePattern.end(); pat++)
      {
        if (ErrorOccurred(m_cerestim.autoStimulus(pat->first, pat->second)))
        {
          bciwarn << "CereStim Warning: Unable to upload stimulus with ID: "
            << (int)pat->second << " at electrode " << (int)pat->first
            << ". CereStim reported: " << m_last_error << endl;
        }
      }
      //end group
      if (ErrorOccurred(m_cerestim.endOfGroup()))
      {
        bciwarn << "CereStim Warning: Unable to end stimulation group. "
          << "CereStim reported: " << m_last_error << endl;
        continue;
      }
      //add pause for train
      if (ErrorOccurred(m_cerestim.wait(*mActiveStimulation.wait_ms)))
      {
        bciwarn << "CereStim Warning: Unable to add pause in sequence. "
          << "CereStim reported: " << m_last_error << endl;
        continue;
      }
      //end sequence
      if (ErrorOccurred(m_cerestim.endOfSequence()))
      {
        bciwarn << "CereStim Warning: Unable to end stimulation sequence. "
          << "CereStim reported: " << m_last_error << endl;
        continue;
      }

      //start stim
      if (ErrorOccurred(m_cerestim.play(*mActiveStimulation.trainReps)))
      {
        bciwarn << "CereStim Warning: Unable to trigger stimulation sequence. "
          << "CereStim reported: " << m_last_error << endl;
        continue;
      }
#endif
      //don't reset mStimIsActive if stimulating indefinitely, resets when we stop run
      if (*mActiveStimulation.trainReps != 0)
      {
        //sleep until stim is done to reset variables
        Sleep(*mActiveStimulation.totalDuration);
        mStimIsActive = false;
      }
    }
  }
}

bool 
CereStim::OpenConnection()
{
  vector<UINT32> devices;
  if (ErrorOccurred(m_cerestim.scanForDevices(devices)))
  {
    bcierr << "CereStim Error: Could not scan for devices" << endl;
    return false;
  }
  if (devices.size() == 0)
  {
    bcierr << "CereStim Error: No devices found" << endl;
    return false;
  }
  // default connect to first device connected
  if (ErrorOccurred(m_cerestim.setDevice(0)))
  {
    bcierr << "CereStim Error: Unable to connect to CereStim" << endl;
    return false;
  }
  if (ErrorOccurred(m_cerestim.connect(BINTERFACE_WUSB, 0)))
  {
    bcierr << "CereStim Error: Unable to connect to CereStim" << endl;
    return false;
  }
  return true;
}


void 
CereStim::CloseConnection()
{
  if (this->IsConnected())
    m_cerestim.disconnect();
}


bool 
CereStim::IsConnected()
{
  // function isn't const so we cant const this function...
  return m_cerestim.isConnected(); 
}


void
CereStim::SequenceRunning()
{
  while (!m_kill_status_thread)
  {
    if (this->IsConnected())
    {
      BSequenceStatus status;
      if (ErrorOccurred(m_cerestim.readSequenceStatus(&status)))
      {
        bcierr << "CereStim Error: Unable to query device for sequence status" << endl;
      }
      mStimState = status.status == BSEQ_PLAYING;
    }
    Sleep(0);
  }
}


bool 
CereStim::ErrorOccurred(BResult _res)
{
  m_last_error = "";
  if (_res == BSUCCESS)
    return false;

  if (_res > -25) // software error
  {
    switch (_res)
    {
    case BRETURN:
      m_last_error = "Software Error: Early returned warning";
      break;
    case BSUCCESS:
      m_last_error = "Software Error: Successful operation";
      break;
    case BNOTIMPLEMENTED:
      m_last_error = "Software Error: Not implemented";
      break;
    case BUNKNOWN:
      m_last_error = "Software Error: Unknown error";
      break;
    case BINVALIDHANDLE:
      m_last_error = "Software Error: Invalid handle";
      break;
    case BNULLPTR:
      m_last_error = "Software Error: Null pointer";
      break;
    case BINVALIDINTERFACE:
      m_last_error = "Software Error: Invalid interface specified or interface not supported";
      break;
    case BINTERFACETIMEOUT:
      m_last_error = "Software Error: Timeout in creating the interface";
      break;
    case BDEVICEREGISTERED:
      m_last_error = "Software Error: Device with that address already connected";
      break;
    case BINVALIDPARAMS:
      m_last_error = "Software Error: Invalid parameters";
      break;
    case BDISCONNECTED:
      m_last_error = "Software Error: CereStim is disconnected, invalid operation";
      break;
    case BCONNECTED:
      m_last_error = "Software Error: CereStim is connected, invalid operation";
      break;
    case BSTIMATTACHED:
      m_last_error = "Software Error: CereStim is attached, invalid operation";
      break;
    case BDEVICENOTIFY:
      m_last_error = "Software Error: Cannot register for device change notification";
      break;
    case BINVALIDCOMMAND:
      m_last_error = "Software Error: Invalid command";
      break;
    case BINTERFACEWRITE:
      m_last_error = "Software Error: Cannot open interface for write";
      break;
    case BINTERFACEREAD:
      m_last_error = "Software Error: Cannot open interface for read";
      break;
    case BWRITEERR:
      m_last_error = "Software Error: Cannot write command to the interface";
      break;
    case BREADERR:
      m_last_error = "Software Error: Cannot read command from the interface";
      break;
    case BINVALIDMODULENUM:
      m_last_error = "Software Error: Invalid module number specified";
      break;
    case BINVALIDCALLBACKTYPE:
      m_last_error = "Software Error: Invalid callback type";
      break;
    case BCALLBACKREGFAILED:
      m_last_error = "Software Error: Callback register/unregister failed";
      break;
    case BLIBRARYFIRMWARE:
      m_last_error = "Software Error: CereStim firmware version not supported by SDK Library Version";
      break;
    case BFREQPERIODZERO:
      m_last_error = "Software Error: Frequency or period is zero and unable to be converted";
      break;
    case BNODEVICESELECTED:
      m_last_error = "Software Error: No physical device has been set. See setDevice() for help";
      break;
    default:
      m_last_error = "Software Error";
      break;
    }
    cout << m_last_error << endl;
    return true;
  }
  else
  {
    switch (_res)
    {
    case BNOK:
      m_last_error = "Hardware Error : Command result not OK";
      break;
    case BSEQUENCEERROR:
      m_last_error = "Hardware Error: Sequence error";
      break;
    case BINVALIDTRIGGER:
      m_last_error = "Hardware Error: Invalid trigger";
      break;
    case BINVALIDCHANNEL:
      m_last_error = "Hardware Error: Invalid channel";
      break;
    case BINVALIDCONFIG:
      m_last_error = "Hardware Error: Invalid configuration";
      break;
    case BINVALIDNUMBER:
      m_last_error = "Hardware Error: Invalid number";
      break;
    case BINVALIDRWR:
      m_last_error = "Hardware Error: Invalid read/write";
      break;
    case BINVALIDVOLTAGE:
      m_last_error = "Hardware Error: Invalid voltage";
      break;
    case BINVALIDAMPLITUDE:
      m_last_error = "Hardware Error: Invalid amplitude";
      break;
    case BINVALIDAFCF:
      m_last_error = "Hardware Error: Invalid AF/CF";
      break;
    case BINVALIDPULSES:
      m_last_error = "Hardware Error: Invalid pulses";
      break;
    case BINVALIDWIDTH:
      m_last_error = "Hardware Error: Invalid width";
      break;
    case BINVALIDINTERPULSE:
      m_last_error = "Hardware Error: Invalid interpulse";
      break;
    case BINVALIDINTERPHASE:
      m_last_error = "Hardware Error: Invalid interphase";
      break;
    case BINVALIDFASTDISCH:
      m_last_error = "Hardware Error: Invalid fast discharge";
      break;
    case BINVALIDMODULE:
      m_last_error = "Hardware Error: Invalid module";
      break;
    case BSTIMULIMODULES:
      m_last_error = "Hardware Error: More stimuli than modules";
      break;
    case BMODULEUNAVAILABLE:
      m_last_error = "Hardware Error: Module not available";
      break;
    case BCHANNELUSEDINGROUP:
      m_last_error = "Hardware Error: Channel already used in group";
    case BCONFIGNOTACTIVE:
      break;
      m_last_error = "Hardware Error: Configuration not active";
      break;
    case BEMPTYCONFIG:
      m_last_error = "Hardware Error: Empty config";
      break;
    case BPHASENOTBALANCED:
      m_last_error = "Hardware Error: Phases not balanced";
      break;
    case BPHASEGREATMAX:
      m_last_error = "Hardware Error: Phase charge greater than max";
      break;
    case BAMPGREATMAX:
      m_last_error = "Hardware Error: Amplitude greater than max";
      break;
    case BWIDTHGREATMAX:
      m_last_error = "Hardware Error: Width greater than max";
      break;
    case BVOLTGREATMAX:
      m_last_error = "Hardware Error: Voltage greater than max";
      break;
    case BMODULEDISABLED:
      m_last_error = "Hardware Error: Module already disabled, can't disable it";
      break;
    case BMODULEENABLED:
      m_last_error = "Hardware Error: Module already enabled, can't re-enable it";
      break;
    case BINVALIDFREQUENCY:
      m_last_error = "Hardware Error: Invalid frequency";
      break;
    case BFREQUENCYGREATMAX:
      m_last_error = "Hardware Error: The frequency is greater than the max value allowed";
      break;
    case BDEVICELOCKED:
      m_last_error = "Hardware Error: Device locked due to hardware mismatch or not being configured";
      break;
    case BECHOERROR:
      m_last_error = "Hardware Error: Command returned was not the same command sent";
      break;
    default:
      m_last_error = "Hardware Error";
      break;
    }
    return true;
  }
}
