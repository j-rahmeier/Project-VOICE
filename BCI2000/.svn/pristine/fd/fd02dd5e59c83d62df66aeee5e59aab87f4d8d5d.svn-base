////////////////////////////////////////////////////////////////////////////////
// Author: t.moore
// Date:   20 Oct 23
// Description: NC_flexXADC implementation
////////////////////////////////////////////////////////////////////////////////
#include "NC_flexXADC.h"
#include "lib/flexX.imports.h"

#include "BCIStream.h"
#include "BCIEvent.h"

#include "ThreadUtils.h"
#include "EnvVariable.h"

RegisterFilter( NC_flexXADC, 1 ); // ADC filters must be registered at location "1" in order to work.

NC_flexXADC::NC_flexXADC()
: mpBuffer( 0 ), // Make sure we can call delete[] without crashing even if we never called new[].
  mNumberOfSignalChannels(8),
  mConnected(false),
  mSamplingRate(0),
  mDCfilter(0),
  mResolutionCode(0)
{
}

NC_flexXADC::~NC_flexXADC()
{
  delete[] mpBuffer;
}

void
NC_flexXADC::OnPublish()
{
  std::string unspecified = " ";
  std::string port = OptionalParameter("PortName", unspecified);
  if (port == unspecified) EnvVariable::Get("FlexEEGPort", port);
  if (port == unspecified) port = "";
  if (port == "") port = "COM9";

  std::string portNameParameter = "Source:Signal%20Source string PortName= " + port + " // PORT to connect to";
  BEGIN_PARAMETER_DEFINITIONS
    portNameParameter.c_str(), 

    "Source:Signal%20Source int AcquisitionMode= 0 "
    "0 0 2 // data acquisition mode: "
    " 0: analog signal acquisition,"
    " 1: test signal,"
    " 2: input shorted (for offset or noise measurement),"
    " (enumeration)",

    "Source:Signal%20Source int DCfilter= 0 "
    "0 0 1 // Turn DC filter on (boolean)",

    "Source:Signal%20Source int Resolution= 0 "
    "0 0 1 //  Resolution: 0: 24-bit, 1: 16-bit (enumeration)",

    "Source:Signal%20Properties float SamplingRate= 250 "
      "250 125 250 // Sample rate can be 125 or 250",

    "Source:Signal%20Properties int SourceCh= 8 "
      "8 1 8 // number of digitized and stored channels",

    "Source:Signal%20Properties int SampleBlockSize= 32 "
      "32 24 % // number of samples transmitted at a time (must be at least 24)",

    "Source:Signal%20Properties list ChannelNames= 1 auto "
      " // names of amplifier channels",  

    "Source:Signal%20Properties list SourceChGain= 1 auto "
      " // physical units per raw A/D unit",

    "Source:Signal%20Properties list SourceChOffset= 1 auto "
      " // offset for channels in A/D units",

  END_PARAMETER_DEFINITIONS


  BEGIN_STREAM_DEFINITIONS
    "Trigger 8 0", // <name>   <bit width>   <initial value>
  END_STREAM_DEFINITIONS

  //BEGIN_STATE_DEFINITIONS
  //  "Battery 7 0", // TODO: this is intended to reflect the device's current % battery level - need code for periodically querying the device and updating this state.
  //END_STATE_DEFINITIONS

}

void
NC_flexXADC::OnAutoConfig()
{
  
}

void
NC_flexXADC::OnPreflight( SignalProperties& Output ) const
{
  State( "Trigger" );
    
    
  int k = Parameter("SourceCh"); 
  if ((k < 1 )  || (k > 8 ))
  {
     bcierr << "Number of channels must be between 1 and 8 ";
  }
  
  k =  Parameter( "SampleBlockSize" );
  if (k < 24 )
  {
    bcierr << "SampleBlockSize must be at least 24 for FlexEEG" <<k;
  }
  
  float fl = Parameter("SamplingRate"); 
  if (!((fl == 125) || (fl == 250 ) ))
  {
      bcierr << "SamplingRate must be either 125 or 250 for FlexEEG";
  }

 // check com port

  char* temp = new char[strlen(Parameter("PortName").c_str())];

  std::strcpy(temp, Parameter("PortName").c_str());

  if (strncmp(temp, "COM", 3))
  {
      bcierr << "need to specify a COM port";
  }

  k = Parameter("SourceCh");
  for (int i = 0; i < k; i++)
  {
     double gainFactor = 1.0;
     if (Parameter("SourceChGain")(i) != gainFactor)
     {
         bcierr << "gain factor needs to be 1";
     }
     if( Parameter("SourceChOffset")(i) != 0)
     {
         bcierr << "channel offset need to be 0";
     }
  }

  SignalType sigType = SignalType::float32;

  int samplesPerBlock = Output.Elements();
  int numberOfSignalChannels = Output.Channels();
  int numberOfSyncStates = 1;
  Output = SignalProperties(( (uint64_t)numberOfSignalChannels +  (uint64_t)numberOfSyncStates), samplesPerBlock, sigType );
  Output.ChannelLabels()[numberOfSignalChannels] = "@Trigger";
}

void
NC_flexXADC::OnInitialize( const SignalProperties& Output )
{
  // =================== check com port =====================


  if (mConnected)
  {
     flexX_ClosePort();
     bciout << "closed serial connection";
     mConnected = false;
     ThreadUtils::SleepForMs(2000);
     // TODO: if you close and reopen the port (e.g. press Set Config for a second time in a session)
     //       the API appears to freeze (never returns from flexX_OpenPort) even with this 2-second delay
  }
  std::string portName = Parameter("PortName");
  bciout << "opening serial connection";
  if (flexX_OpenPort(portName.c_str(), 115200))
  {
     mConnected = true;
     bciout << "connected to "<< portName;
     ThreadUtils::SleepForMs(500);
     // TODO: if, after the API has frozen once, you try to open again WITHOUT physically power-cycling
     //       the device, it may claim success here at flexX_OpenPort, but then fail at all its subsequent
     //       attempts to write configuration settings and register values
  }
  else
  {
     throw bcierr << "Could not connect to FlexEEG device on " << portName;
  }
  
  // Set number of channels
  mNumberOfSignalChannels = Parameter("SourceCh");

  if (!flexX_SetEnabledChannels(mNumberOfSignalChannels))
  {
    bciout << "flexX_SetEnabledChannels(" << (int)mNumberOfSignalChannels << ") failed";
  }
 
  // Parameter("SourceCh") = channels; // Set SourceCh in case of "auto"
  bciout << "Enabled Channels: " << (int)mNumberOfSignalChannels;
  
  // Set Sampling Rate
  mSamplingRate = Parameter("SamplingRate");
  if (flexX_SetSampling( (mSamplingRate == 125) ? 1 : 0 ))
  {
    bciout << "Set sampling rate to " << (int)mSamplingRate << "Hz";
  }
  else
  {
    bcierr << "flexX_SetSampling(" << ( (mSamplingRate == 125) ? 1 : 0 ) << ") failed";
  }
 
 // Set Resolution 
 
  mResolutionCode = Parameter("Resolution");
  if (flexX_SetResolution(mResolutionCode))
  {
    bciout << "Resolution set to " << ( mResolutionCode ? 16 : 24 ) << " bit";
  }
  else
  {
    bcierr << "flexX_SetResolution(" << (int)mResolutionCode << ") failed";
  }
   
  mDCfilter = Parameter("DCfilter");
  if (flexX_SetFilter(mDCfilter))
  {
    bciout << "DC filter " << (mDCfilter ? "enabled" : "disabled");
  }
  else
  {
    bcierr << "flexX_SetFilter(" << (int)mDCfilter << ") failed";
  }

  // set up names and other stuff 
  uint8_t channelRegisterValue = 0;
  uint8_t acquisitionMode =  Parameter("AcquisitionMode") ;
     
  switch (acquisitionMode)
  {
    case 1:
      channelRegisterValue = 0x65;
      break;

    case 2:
      channelRegisterValue = 0x61;    // input shorted
      break;

    case 0:
    default:
      channelRegisterValue = 0x60;
      break;

  }

  for (int i = 0; i < mNumberOfSignalChannels; ++i)
  {
    // write the channels setting
    if (!flexX_SetRegister(0x5 + i, channelRegisterValue))
    {
      bcierr << "flexX_SetRegister(0x" << std::hex << (0x5 + i) << ", 0x" << std::hex << (unsigned int)channelRegisterValue << ") failed";
    }
  } 
    
  int bufferBytes = Output.Channels() * Output.Elements() * sizeof( float );
  delete[] mpBuffer;
  mpBuffer = new char[bufferBytes];
  ::memset( mpBuffer, 0, bufferBytes );
}

void
NC_flexXADC::OnStartAcquisition()
{
  flexX_Start();
  bciout << "Started getting data";

}

void
NC_flexXADC::DoAcquire( GenericSignal& Output )
{
  float OurBuff[640];     // read in floats from headset 32 * 9 floats 
  float* buffer_ptr = &OurBuff[0];  // gets data
  int i, j;
  uint8_t noofsamples = 0;
  uint16_t recno = 0;
  int samplesPerBlock = Output.Elements();
 
  // get some data
  while (noofsamples < samplesPerBlock)
  {
    ThreadUtils::SleepForMs(2);
    if (flexX_RecNo(&recno))
    {
      // recno has number of samples available
      // what happens if over 32?   just take 32
      
      if (recno >= samplesPerBlock)
      {
        // just read whatever is available once its above samplesPerBlock
        // 121023 only read required blocks

        for (i = 0; i < samplesPerBlock /*recno*/; i++)
        {
          if (flexX_GetChannelData(buffer_ptr))
          {
            noofsamples++;
            buffer_ptr += 9;
          }
        }
      }
    }
  }

  int ch, sample; 
 
  // Copy values from raw buffer into output signal.
  buffer_ptr = &OurBuff[0];    
  for (sample = 0; sample < samplesPerBlock /* noofsamples *//*Output.Elements()*/; sample++)
  {
    for (ch  = 0; ch < mNumberOfSignalChannels; ch++)
    {
      Output(ch, sample) = *buffer_ptr++;
    }
    Output(ch, sample) = (float)*buffer_ptr++;
  }

}

void
NC_flexXADC::OnStopAcquisition()
{
  flexX_Stop();
  bciout << "Stopped getting data";
}
