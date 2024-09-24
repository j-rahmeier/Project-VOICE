////////////////////////////////////////////////////////////////////////////////
// Authors: bci2000@DESKTOP-503T7ML
// Description: RippleADC implementation
////////////////////////////////////////////////////////////////////////////////
#include "RippleADC.h"

#include "BCIStream.h"
#include "BCIEvent.h"
#include <stdint.h>
#include <iostream>
#include <unistd.h>

const RippleADC::FrontEndInfo RippleADC::cFrontends[2] =
{
	{ "stim", 24, 32, 0.022e-6,(SamplingRates)(HIRES & HiFreq)},
	{ "raw", 16, 32, 0.25e-6,(SamplingRates)(LFP & HIRES & RAW)}
};



// Make the source filter known to the framework.
RegisterFilter(RippleADC, 1); // ADC filters must be registered at location "1" in order to work.

RippleADC::RippleADC() :m_databuffer(NULL)
{
	
}

RippleADC::~RippleADC()
{
	delete m_databuffer;
}

void
RippleADC::OnPublish()
{
	// Declare any parameters that the filter needs....

	BEGIN_PARAMETER_DEFINITIONS

		"Source:Signal%20Properties int SourceCh= auto "
		"auto 1 % // number of digitized and stored channels", // right now only 2khz or 30khz are supported

		"Source:Signal%20Properties int SampleBlockSize= auto "
		"auto 1 % // number of samples transmitted at a time",

		"Source:Signal%20Properties float SamplingRate= auto "
		"auto 0.0 % // sample rate",

		"Source:Signal%20Properties list SourceChGain= 1 auto "
		" // physical units per raw A/D unit",

		"Source:Signal%20Properties list SourceChOffset= 1 auto "
		" // raw A/D offset to subtract, typically 0",

		"Source:Signal%20Properties list ChannelNames= 1 auto "
		" // names of amplifier channels",

		"Source:Signal%20Properties int MyExampleAmpSetting= 3 "
		" // some example user setting specific to your amp",

		END_PARAMETER_DEFINITIONS

		// ...and likewise any state variables.

		// IMPORTANT NOTE ABOUT BUFFEREDADC AND STATES:
		// * BCI2000 States, or "state variables", are additional data channels stored alongside signal data,
		//   with a resolution of one value per source signal sample.
		// * A State is much like a source signal channel, but differs from a source signal channel in the
		//   following respects:
		//   + You choose the number of bits used to represent the State's value, up to 64.
		//   + Unlike source signals, States are transmitted through all stages of processing, and their values
		//     may be modified during processing, allowing all parts of the system to store state information in
		//     data files.
		//   + When loading a BCI2000 data file for analysis, States appear separately, with names, which is
		//     typically more useful for trigger information than being just another channel in the signal.
		//   + States may be set synchronously from inside a filter's Process() handler, or asynchronously using
		//     a "bcievent" interface.
		//   + If your amplifier provides a digital input, or another kind of "trigger" information, it makes sense
		//     to store this information in form of one or more States. From a user perspective, it is probably most
		//     useful if physically distinct amplifier input sockets correspond to States, and single bits to certain
		//     lines or values communicated through such a socket.
		//   + If the amplifier API asynchronously reports trigger information through a callback mechanism, you
		//     should register a callback that uses the "bcievent" interface to set states asynchronously.
		//     This example provides a "MyAsyncTriggers" "event state", and a sample callback function.
		//   + If the amplifier API sends trigger information in the same way as it does for signal data, you should
		//     use a "State channel" to represent it in your source module. This example provides a "MySyncTriggers"
		//     state, and writes to it when acquiring data.
		BEGIN_STREAM_DEFINITIONS

		"Timestamp 32 0"

		END_STREAM_DEFINITIONS


}

bool RippleADC::Open()
{
	int err = 0;
	if ((err = xl_open_udp()) != XL_ERR_SUCCESS) //right now use default settings
	{
		bcierr << "Error while connecting to Ripple System! Make sure your interface is within 192.168.42.*! Error code: " <<std::to_string(err) << std::endl;
		return false;
	}
	return true;

}

bool RippleADC::Close()
{

	if (xl_close() != XL_ERR_SUCCESS)
	{
		return false;
		bcierr << "Could not close connection to Ripple Device!";
	}

	return true;

}


std::string RippleADC::GetFrontEndName(uint32_t el)
{
	char chl_name[STRLEN_LABEL];
	xl_get_fe_streams(&chl_name, 1, el);
	int frontend = xl_get_fe(el);
	return std::string(chl_name);
}

void
RippleADC::OnAutoConfig()
{

	if (!Open()) return;
	// The user has pressed "Set Config" and some parameters may be set to "auto",
	// indicating that they should be set from the current amplifier configuration.
	// In this handler, we behave as if any parameter were set to "auto", and the
	// framework will transparently make sure to preserve user-defined values.

	// Device availability (or connection parameters) may have changed, so close
	// and reopen the connection to the device before proceeding.



	uint32_t  channels = xl_list_elec(m_chList, MAX_NUM_CHLS, "all");
	Parameter("SourceCh") = channels; // Set SourceCh in case of "auto"
	// If SourceCh != auto but e.g. SourceChGain != auto, we need to use the actual
	// rather than the auto-set value of SourceCh in order to consistently configure
	// SourceChGain.
	// For this purpose, an ActualParameter() call will retrieve a parameter's current
	// value, no matter whether auto-configured or user-configured.
	channels = ActualParameter("SourceCh");
	Parameter("ChannelNames")->SetNumValues(channels);
	Parameter("SourceChGain")->SetNumValues(channels);
	Parameter("SourceChOffset")->SetNumValues(channels);
	for (int i = 0; i < channels; ++i)
	{
		

		Parameter("ChannelNames")(i) << GetFrontEndName(m_chList[i]) << "_" << m_chList[i]; // Omit "<< "_"" << i" if channel names are unique.

		double gainFactor = 1.0;

		Parameter("SourceChGain")(i) << gainFactor << "muV"; // Always provide correct physical unit for documentation and display purposes.
		// For most amplifiers, offset removal is done within the amp hardware or driver. Otherwise, you may set this to a nonzero value.
		Parameter("SourceChOffset")(i) = 0;
	}

	double samplingRate = HIRES_SAMPLINGRATE;

	Parameter("SamplingRate") << samplingRate << "Hz";

	samplingRate = ActualParameter("SamplingRate");
	int samplesPerPacket = std::round(samplingRate/10);

	Parameter("SampleBlockSize") = samplesPerPacket;
	Close();

}

void
RippleADC::OnPreflight(SignalProperties& Output) const
{

	//
	// Errors issued in this way, during the Preflight phase, still allow the user
	// to open the Config dialog box, fix bad parameters and re-try.  By contrast,
	// errors and C++ exceptions at any other stage (outside Preflight) will make
	// the system stop, such that BCI2000 will need to be relaunched entirely.

	// Internally, signals are always represented by GenericSignal::ValueType == double.
	// Here, you choose the format in which data will be stored, which may be
	// int16, int32, or float32.
	// Typically, you will choose the format that your amplifier natively provides, in
	// order to avoid loss of precision when converting, e.g., int32 to float32.
	SignalType sigType = SignalType::float32;

	int samplesPerBlock = Output.Elements();
	int numberOfSignalChannels = Output.Channels();
	if (numberOfSignalChannels == 0)
		bcierr << "No appropriate recoding channels found!" << std::endl;
	Output = SignalProperties(numberOfSignalChannels+1, samplesPerBlock, sigType);
	Output.ChannelLabels()[numberOfSignalChannels + 0] = "@Timestamp";
	int sr = Parameter("SamplingRate");
	switch (sr)
	{
	case RAW_SAMPLINGRATE:
		break;
	case HIRES_SAMPLINGRATE:
		break;
	case HIFREQ_SAMPLINGRATE:
		break;
	case LFP_SAMPLINGRATE:
		break;
	default:
		bcierr << "Unsupported samplingrate! Supported Sampling rates are: " << std::to_string(RAW_SAMPLINGRATE) << ", " << std::to_string(HIRES_SAMPLINGRATE)
			<< ", " << std::to_string(HIFREQ_SAMPLINGRATE)
			<< ", " << std::to_string(LFP_SAMPLINGRATE) << " depending on the connected Frontend" << std::endl;

	}


	// Note that the RippleADC instance itself, and its members, are read-only during the
	// Preflight phase---note the "const" at the end of the OnPreflight prototype above.
	// Any methods called by OnPreflight must also be declared as "const" in the same way.
}

void
RippleADC::OnInitialize(const SignalProperties& Output)
{
	// The user has pressed "Set Config" and all Preflight checks have passed.
	// The system will now transition into an "Initialized" state.

	// If the user should be able to control amplifier configuration through BCI2000,
	// you will need to use the amplifier's API here in order to apply parameter settings.
	// Preflight() checks should ensure that Initialize() never fails due to misconfiguration.
	// Still, unforeseen cases or hardware conditions may lead to failure.


	// The signal properties can no longer be modified, but the const limitation has gone, so
	// the RippleADC instance itself can be modified. Allocate any memory you need
	// store any information you need in private member variables.
	State("Timestamp");
	m_buffersize = Output.Channels() * Output.Elements();
	delete m_databuffer;
	m_databuffer = new float[m_buffersize];
	m_samplingRate = Parameter("SamplingRate");
	bciout << "Sampling Rate set to: " << m_samplingRate << std::endl;
	m_blocks = Parameter("SampleBlockSize");

	//init streams
	//for (int i = 0; i < Output.Channels(); ++i)
	//{
	//	switch (m_samplingRate)
	//	{
	//	case RAW_SAMPLINGRATE:
	//		if (xl_signal_set(m_chList[i],"raw", 1) != XL_ERR_SUCCESS) bcierr << "Could not set Channel stream!" << std::endl;
	//		break;
	//	case HIRES_SAMPLINGRATE:
	//		if (xl_signal_set(m_chList[i], "raw", 0) != XL_ERR_SUCCESS) bcierr << "Could not set Channel stream!" << std::endl;
	//		if (xl_signal_set(m_chList[i], "hires", 0) != XL_ERR_SUCCESS) bcierr << "Could not set Channel stream!" << std::endl;
	//		break;
	//	case HIFREQ_SAMPLINGRATE:
	//		if (xl_signal_set(m_chList[i], "raw", 0) != XL_ERR_SUCCESS) bcierr << "Could not set Channel stream!" << std::endl;
	//		if (xl_signal_set(m_chList[i], "hifreq", 0) != XL_ERR_SUCCESS) bcierr << "Could not set Channel stream!" << std::endl;
	//		break;
	//	case LFP_SAMPLINGRATE:
	//		if (xl_signal_set(m_chList[i], "raw", 0) != XL_ERR_SUCCESS) bcierr << "Could not set Channel stream!" << std::endl;
	//		if (xl_signal_set(m_chList[i], "lfp", 0) != XL_ERR_SUCCESS) bcierr << "Could not set Channel stream!" << std::endl;
	//		break;
	//	default:
	//		break;
	//	}
	//}


	// Buffer allocation may happen in OnStartAcquisition as well, if more appropriate.
}

void
RippleADC::OnStartAcquisition()
{
	// This method is called from the acquisition thread once the system is initialized.
	// You should use this space to start up your amplifier using its API.  Any initialization
	// here is done in the acquisition thread, so non-thread-safe APIs should work if initialized here.
	Open();
	m_timestamp = xl_time();
	m_newtimestamp = 0;
	//xl_cont_hires(&m_timestamp, m_databuffer, 1, m_chList, 1, 0); //get correct timestamp

}

void
RippleADC::DoAcquire(GenericSignal& Output)
{
	uint32_t total_pts = 0;
	uint32_t pts = 0;
	uint32_t timestamp = 0;
	//m_timestamp = xl_time();
	std::fill_n(m_databuffer, m_buffersize, 0);

	uint32_t required_timestamp = m_timestamp + m_blocks*( RAW_SAMPLINGRATE/ m_samplingRate);
	do 
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		timestamp = xl_time();
	} while ((required_timestamp- float(timestamp)) >= 0);
	
		
	//int pts=xl_cont_hires(&m_timestamp, m_databuffer, Output.Elements(), m_chList, Output.Channels(), 0);
	switch (m_samplingRate)
	{
	case RAW_SAMPLINGRATE:
			pts = xl_cont_raw(&timestamp, m_databuffer, Output.Elements(), m_chList, Output.Channels()-1, m_timestamp);
			break;
	case HIRES_SAMPLINGRATE:
			pts = xl_cont_hires(&timestamp, m_databuffer, Output.Elements(), m_chList, Output.Channels()-1, m_timestamp );
			//pts = xl_cont_emg(&new_timestamp, m_databuffer, Output.Elements(), m_chList, Output.Channels(), m_timestamp);
			break;
	case HIFREQ_SAMPLINGRATE:
			pts = xl_cont_hifreq(&timestamp, m_databuffer, Output.Elements(), m_chList, Output.Channels()-1, m_timestamp );
			break;
	case LFP_SAMPLINGRATE:
			pts = xl_cont_lfp(&timestamp, m_databuffer, Output.Elements(), m_chList, Output.Channels()-1, m_timestamp );
			break;
	default:
			bcierr << "Unsupported samplingrate! Supported Sampling rates are: " << std::to_string(RAW_SAMPLINGRATE) << ", " << std::to_string(HIRES_SAMPLINGRATE)
			<< ", " << std::to_string(HIFREQ_SAMPLINGRATE)
			<< ", " << std::to_string(LFP_SAMPLINGRATE) << std::endl;

	}

	if (pts != (Output.Elements()))
		bciwarn << "Wrong number of samples received! " << std::to_string(pts) << std::endl;
	m_timestamp = required_timestamp;
	m_newtimestamp = timestamp;

	//bciout << std::to_string(pts) << std::endl;
	for (int ch = 0; ch < Output.Channels()-1; ++ch)
	{
		for (int s = 0; s < pts; ++s)
		{
			//if (m_databuffer[s + (ch * (pts))] != -32768)
				Output(ch, s) = m_databuffer[s + (ch * (pts))];
			//else
			//{
			//	Output(ch, s) = NAN;
			//}


		}
	}
	for (int s = 0; s < pts; ++s)
		Output(Output.Channels() - 1, s) = m_newtimestamp;


	//bciout << std::to_string(pts) << ":" << std::to_string(n_nan) << std::endl;

	
}



void
RippleADC::OnStopAcquisition()
{
	Close();
	// This method is called from the acquisition thread just before it exits.  Use this method
	// to shut down the amplifier API (undoing whatever was done in OnStartAcquisition).
	// Immediately after this returns, the system will go into an un-initialized state and
	// OnHalt will be called in the main thread.

}

