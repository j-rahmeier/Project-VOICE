////////////////////////////////////////////////////////////////////////////////
// Authors:
// Description: LSLADC implementation
////////////////////////////////////////////////////////////////////////////////

#include "LSLADC.h"
#include "BCIStream.h"

// Make the source filter known to the framework.
// ADC filters must be registered at location "1" in order to work.
RegisterFilter( LSLADC, 1 );

static double cTimeout = 2.0;

//constructor
LSLADC::LSLADC() : mcursample(nullptr), minfo(nullptr)
{
}

//destructor
LSLADC::~LSLADC()
{
	delete [] mcursample;
}

//declare all the parameters
void LSLADC::OnPublish()
{
  // Declare any parameters that the filter needs....

 BEGIN_PARAMETER_DEFINITIONS

    "Source:Signal%20Properties int SourceCh= auto "
       "auto 1 % // number of digitized and stored channels",

    "Source:Signal%20Properties int SampleBlockSize= 16 16 1 % "
       "// number of samples transmitted at a time",

    "Source:Signal%20Properties float SamplingRate= auto "
       "auto 0.0 % // sample rate",

    "Source:Signal%20Properties list SourceChGain= 1 auto "
       " // physical units per raw A/D unit",

    "Source:Signal%20Properties list SourceChOffset= 1 auto "
       " // raw A/D offset to subtract, typically 0",

    "Source:Signal%20Properties list ChannelNames= 1 auto "
       " // names of amplifier channels",

    "Source:LSLStream string StreamSelectionProperty= type "
       " // property to select stream by, e.g. 'name', 'type', 'source_id'",

     "Source:LSLStream string StreamSelectionPropertyValue= EEG "
     " // property value to select stream by",

 END_PARAMETER_DEFINITIONS

  
}

// For asynchronous trigger notification, register this callback with the amplifier API.
static void TriggerCallback( void* pData, int trigger )
{
}

void LSLADC::OnTrigger( int trigger )
{
  
}

//get information from the API
void LSLADC::OnAutoConfig()
{
  int channels = 0;
  bciout<<"Now waiting for an EEG stream...";
  //resolve the stream info
  std::string property = Parameter("StreamSelectionProperty"),
              value = Parameter("StreamSelectionPropertyValue");
  int result = lsl_resolve_byprop(&minfo, 1, property.c_str(), value.c_str(), 1, cTimeout);
  if (result < 0)
    throw bcierr << "lsl_resolve_byprop(" << property << ", " << value << "): " << result;
  else if (result == 0)
    throw bcierr << "No streams found with property " << property << " = " << value;
  channels = lsl_get_channel_count(minfo);
  //set the number of channels 
  Parameter( "SourceCh" ) = channels; 
  Parameter( "ChannelNames" )->SetNumValues( channels );
  Parameter( "SourceChGain" )->SetNumValues( channels );
  Parameter( "SourceChOffset" )->SetNumValues( channels );
  //set the name and gain of channels
  for( int ch = 0; ch < channels; ++ch )
  {
    // For EEG amplifiers, channel names should use 10-20 naming if possible.
    const std::string name = "Ch "+std::to_string(ch);
    Parameter( "ChannelNames" )( ch ) << name; // Omit "<< i" if channel names are unique.
    double gainFactor = 1.0;
    Parameter( "SourceChGain" )( ch ) << gainFactor << "muV"; // Always provide correct physical unit for documentation and display purposes.
    // For most amplifiers, offset removal is done within the amp hardware or driver. Otherwise, you may set this to a nonzero value.
    Parameter( "SourceChOffset" )( ch ) = 0;
  }
  double samplingRate = 1.0;
  //set the sampling rate 
  samplingRate=lsl_get_nominal_srate(minfo);
  Parameter( "SamplingRate" ) << samplingRate << "Hz";
  
}

//set the property of the output 
void LSLADC::OnPreflight( SignalProperties& Output ) const
{
  SignalType sigType = SignalType::float32;
  int samplesPerBlock = Parameter( "SampleBlockSize" );
  int numberOfSignalChannels = Parameter( "SourceCh" );

  Output = SignalProperties( numberOfSignalChannels , samplesPerBlock, sigType );
}


void LSLADC::OnInitialize( const SignalProperties& Output )
{
	int numberOfSignalChannels = Parameter("SourceCh");
	delete [] mcursample;
	mcursample = new float[numberOfSignalChannels];
}


void LSLADC::OnStartAcquisition()
{
  //get the stream inlet
  minlet = lsl_create_inlet(minfo, 300, LSL_NO_PREFERENCE, 1);
  //open the stream
  lsl_open_stream(minlet, cTimeout, &merrcode);
  //print out the information of the stream
  bciout<<"Name of the Sig "<<lsl_get_name(minfo)<<std::endl
  <<"Type of the Sig "<<lsl_get_type(minfo)<<std::endl
  <<"Sampling Rate "<<lsl_get_nominal_srate(minfo)<<" Hz"<<std::endl
  <<"Number of Channels "<<lsl_get_channel_count(minfo)<<std::endl
  <<"Sample block size "<<Parameter("SampleBlockSize")<<std::endl;
  if (merrcode != 0)
	  throw bcierr<<"there is an error ("<<merrcode<<")";
  bciout<<"Displaying data...";
}

void
LSLADC::DoAcquire( GenericSignal& Output )
{
	for (int el = 0; el < Output.Elements(); el++)
	{
		//pull samples for the input stream 
		mtimestamp = lsl_pull_sample_f(minlet,mcursample,Output.Channels(),LSL_FOREVER,&merrcode);
		for (int ch=0; ch<Output.Channels(); ++ch)
		{
			//copy data to output 
			Output( ch, el ) = mcursample[ch];
			
		}
	}
}

void
LSLADC::OnStopAcquisition()
{
  
}

