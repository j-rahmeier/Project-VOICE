////////////////////////////////////////////////////////////////////////////////
// Authors: Robbie@DESKTOP-U5JGQCU
// Description: LabJackADC implementation
////////////////////////////////////////////////////////////////////////////////
#include "LabJackADC.h"
#include <string>
#include "BCIStream.h"
#include "BCIEvent.h"
#include "LabJackM.h"
#include "LJM_StreamUtilities.h"

RegisterFilter(LabJackADC, 1); // ADC filters must be registered at location "1" in order to work.

LabJackADC::LabJackADC()
    : 
    mHandle(0),
    mNumberOfSignalChannels(0),
    mSignalChannelNames{0},
    mData(0),
    mScanList(0)
{
}

LabJackADC::~LabJackADC()
{
    delete[] mData;
    CloseOrDie(mHandle); 
}

void
LabJackADC::OnPublish()
{

    BEGIN_PARAMETER_DEFINITIONS

        "Source:Signal%20Properties int SourceCh= 1 "
        "auto 1 % // number of digitized and stored channels",

        "Source:Signal%20Properties int SampleBlockSize= 100 "
        "auto 1 % // number of samples transmitted at a time",

        "Source:Signal%20Properties float SamplingRate= 1000 "
        "auto 0.0 % // sample rate",

        "Source:Signal%20Properties list ChannelNames= 1 AIN0 "
        " // names of amplifier channels",

        "Source:Signal%20Properties int Resolution= 0 0 0 8 "
        " // for higher sampling rates, keep at 0",

        "Source:Signal%20Properties double Range= 1 1 0 10 "
        " // maximum expected input voltage (V)",

        "Source:Signal%20Properties list SourceChGain= 1 auto "
        " // physical units per raw A/D unit",

        "Source:Signal%20Properties list SourceChOffset= 1 auto "
        " // raw A/D offset to subtract, typically 0",

        "Source:Signal%20Properties int Connection= 0 0 0 2 "
        " // 0=Any 1=USB 2=Ethernet",

        END_PARAMETER_DEFINITIONS

        BEGIN_STREAM_DEFINITIONS
        "DigitalStream 8 0", // <name> <bit width> <initial value>

        END_STREAM_DEFINITIONS

        //BEGIN_EVENT_DEFINITIONS
       //"LabJackADCAsyncTriggers 8 0", // <name> <bit width> <initial value>
       // END_EVENT_DEFINITIONS
}

void
LabJackADC::OnAutoConfig()
{
    Parameter("SamplingRate") = 1000;
    Parameter("SourceCh") = 4;
    Parameter("SampleBlockSize") = 100;

    int numofChannels = ActualParameter("SourceCh");

    Parameter("ChannelNames")->SetNumValues(numofChannels);
    Parameter("SourceChGain")->SetNumValues(numofChannels);
    Parameter("SourceChOffset")->SetNumValues(numofChannels);


    if (ActualParameter("ChannelNames")->NumValues() < numofChannels)
        numofChannels = ActualParameter("ChannelNames")->NumValues();

    if (ActualParameter("SourceChGain")->NumValues() < numofChannels)
        numofChannels = ActualParameter("SourceChGain")->NumValues();

    if (ActualParameter("SourceChOffset")->NumValues() < numofChannels)
        numofChannels = ActualParameter("SourceChOffset")->NumValues();
       


    Parameter("SourceCh") = numofChannels;
    Parameter("ChannelNames")->SetNumValues(numofChannels);
    Parameter("SourceChGain")->SetNumValues(numofChannels);
    Parameter("SourceChOffset")->SetNumValues(numofChannels);


    for (int i = 0; i < numofChannels; ++i) {
        Parameter("SourceChGain")(i) = "1muV";
        Parameter("SourceChOffset")(i) = 0;
        Parameter("ChannelNames")(i) = masterList[i];
    }
    Parameter("Range") = 1;
    Parameter("Resolution") = 0;
 
}
void
LabJackADC::OnPreflight(SignalProperties& Output) const
{
    
    for (int i = 0; i < Parameter("SourceCh"); ++i) {
        if (FindChannelIndex(Parameter("ChannelNames")(i)) == -1)
        {
            bcierr << Parameter("ChannelNames")(i) << " is invalid. Enter channel names in the form \"AIN0\", \"FIO0\", \"MIO0\", \"EIO0\", or  \"CIO0\"";
        }
    }
    if (Parameter("SamplingRate") > 100000) { //this is the absolute fastest LabJack can sample up to, however the validity of the scan rate is checked upon initializing the stream so if this is called while using an invalid scan rate the program will still terminate
        bcierr << "LabJack maxmimum sampling rate is 100,000Hz";
    }

    State("DigitalStream"); //Data recieved by the first instance of a digital input channel (FIO) in ChannelNames parameter will be stored in this stream

  
    if (!Parameter("SampleBlockSize")) 
    {
        bcierr << "Enter a nonzero block size";
    }
    if (Parameter("Range") > 10 || Parameter("Range") < 0) {
        bcierr << "Range must be between 0V and 10V";
    }
    if (Parameter("Resolution") > 8 || Parameter("Resolution") < 0) {
        bcierr << "Resolution must be between 0 and 8";
    }
    if (Parameter("Connection") < 0 || Parameter("Connection") > 2) {
        bcierr << "Input for connection must be between 0 and 2";
    }
    int samplesPerBlock = Parameter("SampleBlockSize");
    int numberOfSignalChannels = Parameter("SourceCh");
    SignalType sigType = SignalType::float32;
    int numberOfSyncStates = 1;
    Output = SignalProperties(numberOfSignalChannels + numberOfSyncStates, samplesPerBlock, sigType);
    Output.ChannelLabels()[numberOfSignalChannels + 0] = "@DigitalStream";
  
}

void
LabJackADC::OnInitialize(const SignalProperties& Output)
{
    int connectionIndex= Parameter("Connection");
    if (Parameter("Connection") == 2) {
        connectionIndex = 3;
    }
 
    //the following 2 lines are a modified version of LabJacks "OpenOrDie". These commands need to be run prior to executing any other labjack functions
    err = LJM_Open(LJM_dtANY, connectionIndex, "LJM_idANY", &mHandle);
    BCIErrorCheck(err, INITIAL_ERR_ADDRESS, "LJM_GetHandleInfo");
    err = LJM_GetHandleInfo(mHandle, NULL, &connectionType, NULL, NULL, NULL,
        NULL);
    BCIErrorCheck(err, INITIAL_ERR_ADDRESS, "LJM_GetHandleInfo"); //these 4 commands ensure the connection to the device is valid and establishes the handle


    mNumberOfSignalChannels = Parameter("SourceCh");
    
    for (int i = 0; i < mNumberOfSignalChannels; ++i)
    {
        mSignalChannelNames[i] = masterList[FindChannelIndex(Parameter("ChannelNames")(i))];
        bciwarn << "Initialize: Channel: " << mSignalChannelNames[i];
        if (firstDigitalInput==-1) {
            if (FindChannelIndex(Parameter("ChannelNames")(i)) > 13) {
                firstDigitalInput = i;
                bciwarn << "Will stream digital input from " << Parameter("ChannelNames")(i);
            }
        }
    }
    
    samplingRate = (double) Parameter("SamplingRate").InHertz();
    delete[] mScanList;
    mScanList = new int[(Output.Channels() - 1)];
    delete[] mData;
    mData = new double[(Output.Channels() - 1) * Output.Elements()];
    err = LJM_NamesToAddresses(mNumberOfSignalChannels, (const char**)mSignalChannelNames, mScanList, NULL);
    BCIErrorCheck(err, INITIAL_ERR_ADDRESS, "Getting positive channel addresses");
    HardcodedConfigureStream(mHandle);
    err = LJM_eStreamStart(mHandle, Output.Elements(), mNumberOfSignalChannels, mScanList,
        &samplingRate);
    BCIErrorCheck(err, INITIAL_ERR_ADDRESS, "LJM_eStreamStart");

    for (int i = 0; i < 3; ++i) { //stream tends to be inaccurate for the first few runs, running it here a few times prevents (unnecessary) warnings from showing
        err = LJM_eStreamRead(mHandle, mData, &deviceScanBacklog,
            &LJMScanBacklog);
        BCIErrorCheck(err, INITIAL_ERR_ADDRESS, "LJM_eStreamRead");
    }
    bciwarn << "Wait for a few seconds for the stream to stabilize...";
}

void
LabJackADC::OnStartAcquisition()
{
}

void
LabJackADC::DoAcquire(GenericSignal& Output)
{   
    err = LJM_eStreamRead(mHandle, mData, &deviceScanBacklog,
        &LJMScanBacklog);
    BCIErrorCheck(err, INITIAL_ERR_ADDRESS, "LJM_eStreamRead");
        
    if (connectionType != LJM_ctUSB) {
        err = LJM_GetStreamTCPReceiveBufferStatus(mHandle,
            &receiveBufferBytesSize, &receiveBufferBytesBacklog);
        BCIErrorCheck(err, INITIAL_ERR_ADDRESS, "LJM_GetStreamTCPReceiveBufferStatus");
        
    }

    for (int sample = 0; sample < Output.Elements(); ++sample)
    {

        for (int ch = 0; ch < mNumberOfSignalChannels; ++ch)
        {
            Output(ch, sample) = mData[ch + sample * mNumberOfSignalChannels];
            if (ch == firstDigitalInput) {
                Output(mNumberOfSignalChannels, sample)= mData[ch + sample * mNumberOfSignalChannels];
             }
        }

    }

}

void
LabJackADC::OnStopAcquisition()
{
    err = LJM_eStreamStop(mHandle);
    BCIErrorCheck(err, INITIAL_ERR_ADDRESS, "Stopping stream");
    firstDigitalInput=-1;
    delete[] mData;
    delete[] mScanList;
    mData = nullptr;
    mScanList = nullptr;
}

int
LabJackADC::FindChannelIndex(std::string channel) const
{


    char* channelChar = &channel[0];
    int channelLength = channel.length();
    int position=-1;
    for (int i = 0; i < *(&masterList+1)-masterList; ++i) {
        if (std::equal((channelChar), (channelChar + channelLength), (masterList[i])))
        {
            position = i;
            break;
        }
    }
    return position;
}

void 
LabJackADC::HardcodedConfigureStream(int handle)
{
    const int STREAM_TRIGGER_INDEX = 0;
    const int STREAM_CLOCK_SOURCE = 0;
    const int STREAM_RESOLUTION_INDEX = Parameter("Resolution");
    const double STREAM_SETTLING_US = 0;
    const double AIN_ALL_RANGE = Parameter("Range");
    const int AIN_ALL_NEGATIVE_CH = LJM_GND;
    BCIWriteNameOrError(handle, "STREAM_TRIGGER_INDEX", STREAM_TRIGGER_INDEX);
    if (STREAM_CLOCK_SOURCE == 0) {
    }
    BCIWriteNameOrError(handle, "STREAM_CLOCK_SOURCE", STREAM_CLOCK_SOURCE);
    // Configure the analog inputs' negative channel, range, settling time and
    // resolution.
    // Note: when streaming, negative channels and ranges can be configured for
    // individual analog inputs, but the stream has only one settling time and
    // resolution.
    BCIWriteNameOrError(handle, "STREAM_RESOLUTION_INDEX", STREAM_RESOLUTION_INDEX);
    BCIWriteNameOrError(handle, "STREAM_SETTLING_US", STREAM_SETTLING_US);
    BCIWriteNameOrError(handle, "AIN_ALL_RANGE", AIN_ALL_RANGE);
    BCIWriteNameOrError(handle, "AIN_ALL_NEGATIVE_CH", AIN_ALL_NEGATIVE_CH);
} 

void
LabJackADC::BCIErrorCheck(int err, int errAddress,const char* description)
{
    
    char errName[LJM_MAX_NAME_SIZE];
    if (err >= LJME_WARNINGS_BEGIN && err <= LJME_WARNINGS_END) {
        LJM_ErrorToString(err, errName);
        bciwarn <<description<< "Warning code: " << errName << " with value: " << err;
        if (!(errAddress < 0)) bciwarn << description<<" Error address: " << errAddress << " (" << errName << ")";

    }
    else if (err != LJME_NOERROR)
    {
        LJM_ErrorToString(err, errName);
        bciwarn << description << " Error code: " << errName << " with value: " << err << ". Closing all devices and exiting.";
        if (!(errAddress < 0)) bciwarn << description << " Error address: " << errAddress << " (" << errName << ")";
        if (1) {
            WaitForUserIfWindows();
            LJM_CloseAll();
            throw bcierr << "Devices closed, fix issues and reset config."; //"Devices closed, fix issues and reset config.";
        }
    }
}

void 
LabJackADC::BCIWriteNameOrError(int handle, const char* name, double value)
{
 int err = WriteName(handle, name, value);
 if (err) {
    WaitForUserIfWindows();
    throw bcierr << "Could not write name (" << err << ")";
 }

}