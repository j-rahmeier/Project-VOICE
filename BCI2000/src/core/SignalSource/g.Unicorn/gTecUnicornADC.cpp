////////////////////////////////////////////////////////////////////////////////
// Authors: madamek@neurotechcenter.org
// Description: gTecUnicornADC implementation
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

#include "gTecUnicornADC.h"
#include "BCIEvent.h"
#include "BCIStream.h"

// In order to help you write a source module, exchange of information
// between amplifier and the BCI2000 source module is indicated by the use of
// macros.
// Once you are done with writing the source module, each occurrence of
// GET_FROM_AMP_API(), CALL_AMP_API(), and AMP_API_SYNC_GET_DATA() should
// have been replaced with actual calls to the amplifier API, or constants.
// By removing or disabling those macros' definitions below, you can then
// make sure that the compiler will notify you of any oversights.

// Make the source filter known to the framework.
RegisterFilter(gTecUnicornADC, 1); // ADC filters must be registered at location "1" in order to work.

gTecUnicornADC::gTecUnicornADC()
    : mDeviceHandle(0), // Each plain data member should appear in the initializer list.
      mpBuffer(0)       // Make sure we can call delete[] without crashing even if we never called new[].
{
}

gTecUnicornADC::~gTecUnicornADC()
{
    // The destructor is where to make sure that all acquired resources are released.
    // Memory deallocation (calling delete[] on a NULL pointer is OK).
    delete[] mpBuffer;
    // Closing connection to device.
    // The amplifier's API might choke on an invalid device handle.
    if ((mDeviceHandle != UNICORN_ERROR_INVALID_HANDLE) && (mDeviceHandle != 0))
        if (UNICORN_CloseDevice(&mDeviceHandle) != UNICORN_ERROR_SUCCESS)
            bcierr << "Could not disconnect from device!";
}

void gTecUnicornADC::OnPublish()
{
    // Declare any parameters that the filter needs....

  BEGIN_PARAMETER_DEFINITIONS

    "Source:Signal%20Properties int SourceCh= auto "
        "auto 1 % // number of digitized and stored channels",

    "Source:Signal%20Properties list SourceChList= 1 auto "
        " // Indizes of digitized channels (must correspond to SourceCh)",

    "Source:Signal%20Properties int SampleBlockSize= auto "
        "auto 1 % // number of samples transmitted at a time",

    "Source:Signal%20Properties float SamplingRate= 250 "
        "250 0.0 % // sample rate (noedit)",

    "Source:Signal%20Properties list SourceChGain= 1 auto "
        " // physical units per raw A/D unit",

    "Source:Signal%20Properties list SourceChOffset= 1 auto "
        " // raw A/D offset to subtract, typically 0",

    "Source:Signal%20Properties list ChannelNames= 1 auto "
        " // names of amplifier channels",

    "Source:Signal%20Properties int TestMode= 0 0 0 1 "
        " // Start Device in Test Mode (boolean)",

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
            //     useful if physically distinct amplifier input sockets correspond to States, and single bits to
            //     certain lines or values communicated through such a socket.
            //   + If the amplifier API asynchronously reports trigger information through a callback mechanism, you
            //     should register a callback that uses the "bcievent" interface to set states asynchronously.
            //     This example provides a "MyAsyncTriggers" "event state", and a sample callback function.
            //   + If the amplifier API sends trigger information in the same way as it does for signal data, you should
            //     use a "State channel" to represent it in your source module. This example provides a "MySyncTriggers"
            //     state, and writes to it when acquiring data.
  BEGIN_STREAM_DEFINITIONS
    "Counter 32 0",
    "AccelerationX 32 0",
    "AccelerationY 32 0",
    "AccelerationZ 32 0",
    "GyroscopeX 32 0",
    "GyroscopeY 32 0",
    "GyroscopeZ 32 0",
    "Battery 32 0",
    "Validation 32 0",
  END_STREAM_DEFINITIONS
}

void gTecUnicornADC::OnAutoConfig()
{
    // The user has pressed "Set Config" and some parameters may be set to "auto",
    // indicating that they should be set from the current amplifier configuration.
    // In this handler, we behave as if any parameter were set to "auto", and the
    // framework will transparently make sure to preserve user-defined values.

    // Device availability (or connection parameters) may have changed, so close
    // and reopen the connection to the device before proceeding.

    Parameter("SamplingRate") = UNICORN_SAMPLING_RATE;
    Parameter("SourceCh") = UNICORN_EEG_CHANNELS_COUNT;
    Parameter("SampleBlockSize") = 10;
    Parameter("TestMode") = 0;
    int numofChannels = ActualParameter("SourceCh");

    Parameter("ChannelNames")->SetNumValues(numofChannels);
    Parameter("SourceChGain")->SetNumValues(numofChannels);
    Parameter("SourceChOffset")->SetNumValues(numofChannels);
    Parameter("SourceChList")->SetNumValues(numofChannels);

    if (ActualParameter("ChannelNames")->NumValues() < numofChannels)
        numofChannels = ActualParameter("ChannelNames")->NumValues();

    if (ActualParameter("SourceChGain")->NumValues() < numofChannels)
        numofChannels = ActualParameter("SourceChGain")->NumValues();

    if (ActualParameter("SourceChOffset")->NumValues() < numofChannels)
        numofChannels = ActualParameter("SourceChOffset")->NumValues();

    if (ActualParameter("SourceChList")->NumValues() < numofChannels)
        numofChannels = ActualParameter("SourceChList")->NumValues();

    Parameter("SourceCh") = numofChannels;
    Parameter("ChannelNames")->SetNumValues(numofChannels);
    Parameter("SourceChGain")->SetNumValues(numofChannels);
    Parameter("SourceChOffset")->SetNumValues(numofChannels);
    Parameter("SourceChList")->SetNumValues(numofChannels);

    for (int i = 0; i < numofChannels; ++i)
    {
        Parameter("SourceChList")(i) = i;
        Parameter("SourceChGain")(i) = "1muV";
        Parameter("SourceChOffset")(i) = 0;
    }

    for (int i = 0; i < numofChannels; ++i)
    {
        int ch = ActualParameter("SourceChList")(i);
        if (ch < UNICORN_EEG_CHANNELS_COUNT) // make sure that wrong indices do not cause problems
        {
            Parameter("ChannelNames")(i) << ChannelNames[ch];
        }
    }
}

void gTecUnicornADC::OnPreflight(SignalProperties &Output) const
{
    State("AccelerationX");
    State("AccelerationY");
    State("AccelerationZ");
    State("Counter");
    State("GyroscopeX");
    State("GyroscopeY");
    State("GyroscopeZ");
    State("Battery");
    State("Validation");
    // Also check that the values of any parameters are sane:

    if ((Parameter("SamplingRate").InHertz() != UNICORN_SAMPLING_RATE))
        bcierr << "Amplifier does not support a sampling rate of " << Parameter("SamplingRate");
    if (Parameter("SourceCh") > UNICORN_EEG_CHANNELS_COUNT)
        bcierr << "Amplifier has a maximum of " << std::to_string(UNICORN_EEG_CHANNELS_COUNT) << "available channels!";

    if (Parameter("SourceChList")->NumValues() != Parameter("SourceCh"))
        bcierr << "Number of items in SourceChList must be equal to SourceCh";

    int prev_channel = -1;
    for (int i = 0; i < Parameter("SourceChList")->NumValues(); ++i)
    {
        if (Parameter("SourceChList")(i) > 7)
        {
            bcierr << "Only channels 0 - 7 are allowed!";
            break;
        }
        if ((Parameter("SourceChList")(i) - prev_channel) <= 0)
        {
            bcierr << "Channels must be unique and in accending order";
            break;
        }
        else
            prev_channel = Parameter("SourceChList")(i);
    }
    if (Parameter("ChannelNames")->NumValues() != Parameter("SourceCh"))
        bcierr << "A Channel name must be defined for each Channel!";

    if (Parameter("SourceChGain")->NumValues() != Parameter("SourceCh"))
        bcierr << "A Channel gain must be defined for each Channel!";

    if (Parameter("SourceChOffset")->NumValues() != Parameter("SourceCh"))
        bcierr << "A Channel offset must be defined for each Channel!";
    // A channel name starting with @ indicates a trigger channel.
    // Note that the gTecUnicornADC instance itself, and its members, are read-only during the
    // Preflight phase---note the "const" at the end of the OnPreflight prototype above.
    // Any methods called by OnPreflight must also be declared as "const" in the same way.
    int samplesPerBlock = Parameter("SampleBlockSize");
    int numberOfSignalChannels = Parameter("SourceCh");
    SignalType sigType = SignalType::float32;
    Output = SignalProperties(numberOfSignalChannels + 9, samplesPerBlock, sigType);
    // for(int i=0; i <  numberOfSignalChannels  ; ++i)
    //	Output.ChannelUnit().

    Output.ChannelLabels()[numberOfSignalChannels + 0] = "@AccelerationX";
    Output.ChannelLabels()[numberOfSignalChannels + 1] = "@AccelerationY";
    Output.ChannelLabels()[numberOfSignalChannels + 2] = "@AccelerationZ";
    Output.ChannelLabels()[numberOfSignalChannels + 3] = "@GyroscopeX";
    Output.ChannelLabels()[numberOfSignalChannels + 4] = "@GyroscopeY";
    Output.ChannelLabels()[numberOfSignalChannels + 5] = "@GyroscopeZ";
    Output.ChannelLabels()[numberOfSignalChannels + 6] = "@Battery";
    Output.ChannelLabels()[numberOfSignalChannels + 7] = "@Counter";
    Output.ChannelLabels()[numberOfSignalChannels + 8] = "@Validation";
}

void gTecUnicornADC::OnInitialize(const SignalProperties &Output)
{
    unsigned int available_devices = 0;
    bciout << "Searching for paired devices";
    printErrorString(UNICORN_GetAvailableDevices(NULL, &available_devices, true)); // get number of devices

    Sleep(100);
    if (available_devices == 0)
    {
        bciwarn << "No paired devices found, searching for unpaired devices!";
        printErrorString(UNICORN_GetAvailableDevices(NULL, &available_devices, false)); // get number of devices
    }
    Sleep(100);
    if (available_devices == 0)
    {
        bcierr << "No Unicorn devices found";
        return;
    }

    UNICORN_DEVICE_SERIAL *info = new UNICORN_DEVICE_SERIAL[available_devices]; // if set to null API allocates memory
    printErrorString(UNICORN_GetAvailableDevices(info, &available_devices, true)); // get number of devices

    bciout << "Pairing with first available device!";

    printErrorString(UNICORN_OpenDevice(info[0], &mDeviceHandle));
    if (mDeviceHandle == NULL)
        bcierr << "Couldn't get valid Device Handle";
    delete info;

    UNICORN_AMPLIFIER_CONFIGURATION amp_config;
    printErrorString(UNICORN_GetConfiguration(mDeviceHandle, &amp_config));

    auto channelparams = Parameter("SourceChList");
    auto channelNames = Parameter("ChannelNames");

    for (int i = 0; i < UNICORN_TOTAL_CHANNELS_COUNT; ++i) // activate all channels, including gyroscope and so on
    {
        // automatic configuration; use all channels
        if (i < UNICORN_ACCELEROMETER_CONFIG_INDEX)
            amp_config.Channels[i].enabled = false;
        else
            amp_config.Channels[i].enabled = true;

        for (int ii = 0; ii < channelparams->NumValues(); ++ii)
        {
            if (channelparams(ii) == i)
            {
                amp_config.Channels[i].enabled = true;
                continue;
            }
        }
    }

    mSamplingRate = Parameter("SamplingRate");

    if (UNICORN_SetConfiguration(mDeviceHandle, &amp_config) != UNICORN_ERROR_SUCCESS)
        bcierr << "Could not set Amplifier Settings! ";
    // The user has pressed "Set Config" and all Preflight checks have passed.
    // The system will now transition into an "Initialized" state.

    // The signal properties can no longer be modified, but the const limitation has gone, so
    // the gTecUnicornADC instance itself can be modified. Allocate any memory you need
    // store any information you need in private member variables.
    int numberOfSignalChannels = Parameter("SourceCh");
    UNICORN_GetNumberOfAcquiredChannels(mDeviceHandle, &mNumberOfSignalChannels);
    mNumberOfSignalChannels = mNumberOfSignalChannels + 9; // add the status channels
    int bufferSize = mNumberOfSignalChannels * Output.Elements();
    mNumberOfSamples = mNumberOfSignalChannels * Output.Elements();
    delete[] mpBuffer;
    mpBuffer = new float[bufferSize];
    mbufferSize = mNumberOfSamples;
    ::memset(mpBuffer, 0, mNumberOfSamples);
    // Buffer allocation may happen in OnStartAcquisition as well, if more appropriate.
}

void gTecUnicornADC::OnStartAcquisition()
{
    // This method is called from the acquisition thread once the system is initialized.
    // You should use this space to start up your amplifier using its API.  Any initialization
    // here is done in the acquisition thread, so non-thread-safe APIs should work if initialized here.
    while (UNICORN_StartAcquisition(mDeviceHandle, Parameter("TestMode")) != UNICORN_ERROR_SUCCESS)
        ;
}

void gTecUnicornADC::DoAcquire(GenericSignal &Output)
{
    // Sleep(0);

    float *pSignalData = mpBuffer;
    int res = UNICORN_GetData(mDeviceHandle, Output.Elements(), pSignalData, mbufferSize * sizeof(float));
    // UNICORN_AMPLIFIER_CONFIGURATION amp_config;

    if (res == UNICORN_ERROR_SUCCESS)
    {

        // Copy values from raw buffer into output signal.
        for (int ch = 0; ch < Output.Channels(); ch++)
        {
            for (int sample = 0; sample < Output.Elements(); sample++)
            {
                // Check the amplifier's documentation whether data are sent in
                // channel-major or sample-major format.
                Output(ch, sample) = pSignalData[ch + (sample * Output.Channels())];
                // When getting garbage, try this instead:
                // Output(ch, sample) = pSignalData[sample + ch * Output.Elements()];
            }
        }
    }
    // Sleep(0);
}

void gTecUnicornADC::OnStopAcquisition()
{
    // This method is called from the acquisition thread just before it exits.  Use this method
    // to shut down the amplifier API (undoing whatever was done in OnStartAcquisition).
    // Immediately after this returns, the system will go into an un-initialized state and
    // OnHalt will be called in the main thread.
    while (UNICORN_StopAcquisition(mDeviceHandle) != UNICORN_ERROR_SUCCESS)
        ;
}

void gTecUnicornADC::printErrorString(unsigned int err)
{
    switch (err)
    {
    case UNICORN_ERROR_BLUETOOTH_INIT_FAILED:
        bcierr << "Failed to initialize bluetooth";
        break;

    case UNICORN_ERROR_BLUETOOTH_SOCKET_FAILED:
        bcierr << "The operation could not be performed because the Bluetooth socket failed.";
        break;

    case UNICORN_ERROR_BUFFER_OVERFLOW:
        bcierr << "The acquisition buffer is full.";
        break;

    case UNICORN_ERROR_BUFFER_UNDERFLOW:
        bcierr << "The acquisition buffer is empty.";
        break;

    case UNICORN_ERROR_CONNECTION_PROBLEM:
        bcierr << "The operation could not complete because of connection problems.";
        break;

    case UNICORN_ERROR_GENERAL_ERROR:
        bcierr << "An unspecified error occurred.";
        break;

    case UNICORN_ERROR_INVALID_CONFIGURATION:
        bcierr << "The configuration is invalid.";
        break;

    case UNICORN_ERROR_INVALID_HANDLE:
        bcierr << "The specified Unicorn handle is invalid.";
        break;

    case UNICORN_ERROR_INVALID_PARAMETER:
        bcierr << "One of the specified parameters does not contain a valid value.";
        break;

    case UNICORN_ERROR_OPEN_DEVICE_FAILED:
        bcierr << "The Unicorn Brain Interface could not be opened.";
        break;

    case UNICORN_ERROR_OPERATION_NOT_ALLOWED:
        bcierr << "The operation is not allowed during acquisition or non-acquisition.";
        break;

    case UNICORN_ERROR_UNSUPPORTED_DEVICE:
        bcierr << "The Unicorn Brain Interface is not supported with this API (UNICORN_SUPPORTED_DEVICE_VERSION).";
        break;

    case UNICORN_ERROR_SUCCESS:
        break;

    default:
        bcierr << "Unknown Error return value!";
    }
}
