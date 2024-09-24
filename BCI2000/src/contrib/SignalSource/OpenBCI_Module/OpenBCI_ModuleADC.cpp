////////////////////////////////////////////////////////////////////////////////
// Authors: Akshay Vyas, National Center for Adaptive Neurotechnologies, HRI, Albany
// Description: OpenBCI_ModuleADC implementation
////////////////////////////////////////////////////////////////////////////////

#include "OpenBCI_ModuleADC.h"
#include "BCIStream.h"
#include "BCIEvent.h"
#include <cstring>
#include <string>
#include <windows.h>
#include <winbase.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <tchar.h>
#include <strsafe.h>
#include <math.h>
#include "Clock.h"
#include <time.h>
#include "GenericSignal.h"
#include <thread>

using namespace std;

// Make the source filter known to the framework.
RegisterFilter( OpenBCI_ModuleADC, 1 ); // ADC filters must be registered at location "1" in order to work.

/* 
	Constructor 
*/
OpenBCI_ModuleADC::OpenBCI_ModuleADC()
: mFileDescriptor(INVALID_HANDLE_VALUE),// Each plain data member should appear in the initializer list.
  mIsStreaming(false),
  mDollarBill(0),
  mNumberOfBytesAdded(0),
  mCount(0),
  mFirstTimeStream(true),
  mOpenPort(false),
  mCurrentPort(0),
  mNumberOfChannels(EEG_CHANNELS_8),
  mSampleBlockSize(SAMPLEBLOCKSIZE),
  mSetConfig(false),
  mUse8Channels(true)
{
	//Initializing the gains for all 8/16 channels with value = 1
	for (int channel = 0; channel < mNumberOfChannels; channel++)
	{
		mChannelGain[channel] = 1;
	}
}

/*
	Destructor
*/
OpenBCI_ModuleADC::~OpenBCI_ModuleADC()
{
	// The destructor makes sure that all acquired resources are released.
	//delete[] mParseBuffer;
	StopStreaming(mFileDescriptor);
	bool close_h = CloseHandle(mFileDescriptor);	// Closing connection to device.
	bciout << "Handle Closed i.e. File Descriptor freed." << close_h << endl;
}

void
OpenBCI_ModuleADC::OnPublish()
{
  // Declare any parameters that the filter needs....

 BEGIN_PARAMETER_DEFINITIONS

	 "Source:Signal%20Properties string PortName= auto"
	 "// This software can automatically detect connection",

	 "Source:Signal%20Properties int SourceCh= auto auto 1 %"
	 "// number of digitized and stored channels",

	 "Source:Signal%20Properties int SampleBlockSize= auto"
	 " // number of samples transmitted at a time",

	 "Source:Signal%20Properties float SamplingRate= auto"
	 "// sample rate",

	 "Source:Signal%20Properties list SourceChGain= 1 auto"
	 " // physical units per raw A/D unit",

	 "Source:Signal%20Properties list SourceChOffset= 0 auto "
	 " // raw A/D offset to subtract, typically 0",

	 "Source:Signal%20Properties list ChannelNames= 1 auto "
	 " // names of amplifier channels",

	 "Source:Signal%20Properties int MyAmpSetting= 3"
	 " // some user setting specific to your amp",

	 "Source:Signal%20Properties list ChannelsGain= 1 auto 24 1"
	 " // The channel gain can be 1 2 4 6 8 12 24(Default)",

	 "Source:Signal%20Properties list SourceChList= 1 auto "
	 " // The default source channel list is 1 2 3 4 5 6 7 8",


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
    "OpenBCI_ModuleADCSyncTriggers 8 0", // <name> <bit width> <initial value>
  END_STREAM_DEFINITIONS

  BEGIN_EVENT_DEFINITIONS
    "OpenBCI_ModuleADCAsyncTriggers 8 0", // <name> <bit width> <initial value>
  END_EVENT_DEFINITIONS
}

// For asynchronous trigger notification, register this callback with the amplifier API.
static void
TriggerCallback( void* pData, int trigger )
{
  reinterpret_cast<OpenBCI_ModuleADC*>( pData )->OnTrigger( trigger );
}

void
OpenBCI_ModuleADC::OnTrigger( int trigger )
{
  // The following line will create a time-stamped entry into an event queue.
  // Once the next block of data arrives, the queued trigger value will be applied
  // to the OpenBCI_ModuleADCAsyncTriggers state variable at the sample location that
  // corresponds to the time stamp.
  bcievent << "OpenBCI_ModuleADCAsyncTriggers " << trigger;
}


/*
Function to Create a new handle and Set Up the Port
*/
HANDLE
OpenBCI_ModuleADC::OpenPort(const char* PortName)
{
	HANDLE FileDescriptor = NULL;
	DCB dcb;

	//Create the file if doesn't exist and Open the port
	FileDescriptor = CreateFile(PortName, GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);


	if (FileDescriptor == INVALID_HANDLE_VALUE)
	{
		bciout << "Failed to Open Stream1 (Port Not Found!! i.e. File Descriptor value = NULL)" << endl;
		return (INVALID_HANDLE_VALUE);
	}


	if (!SetCommMask(FileDescriptor, EV_RXCHAR))
	{
		bciout << "Failed to Open Stream2 (i.e. Failure in setting the mask!!)" << endl;
		return (INVALID_HANDLE_VALUE);
	}

	//Set the size of read and write buffer 
	SetupComm(FileDescriptor, BUFFERSIZE*mSampleBlockSize * 10, BUFFERSIZE*mSampleBlockSize * 10);

	//Set the Length of DCB
	dcb.DCBlength = sizeof(DCB);

	//Acquire the Comm State
	GetCommState(FileDescriptor, &dcb);

	//Setting the common state for the handle
	dcb.BaudRate = BAUD_RATE;   // Initializing the Default Baud Rate = 115200bmps , 8 N 1
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fInX = FALSE;
	dcb.fOutX = FALSE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;   // no hardware handshake
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.fBinary = TRUE;
	dcb.fParity = FALSE;

	if (!SetCommState(FileDescriptor, &dcb))
	{
		bciout << "Failed to Open Stream3 (i.e. Failure in Setting the Comm State!!)" << endl;
		return (INVALID_HANDLE_VALUE);
	}

	//Send a byte to RESET the OpenBCI Cyton/Daisy board 
	ResetBoard(FileDescriptor);
	bciout << "OPEN PORT Finished!!" << endl;

	//Read the data from the file, output the OpenBCI firmware version details
	/*
	unsigned char buffer[50];
	DWORD length = 20;
	bool readFile = ReadFile(FileDescriptor, (LPSTR)buffer, length, &length, NULL);
	bciout << buffer << endl;
	*/


	return FileDescriptor;
}



/*
Function to Read Maximum Bytes from the Serail Port.
*Returns the number of bytes read or -1 in case of Error
*/
int
OpenBCI_ModuleADC::ReadSerial(HANDLE FileDescriptor, unsigned char buffer[], int size)
{
	BOOL readFile;
	COMSTAT comStat;
	DWORD errorFlags;
	DWORD len;

	//require the information about the setting of the buffer 
	ClearCommError(FileDescriptor, &errorFlags, &comStat);
	len = comStat.cbInQue;

	//If the number of bytes that the serial provides is larger
	//than the number of bytes Required, then Set the length equal to the Size
	if (len > (DWORD)size) len = size;

	//If something is read
	if (len > 0)
	{
		//Read the file into the buffer 
		readFile = ReadFile(FileDescriptor, (LPSTR)buffer, len, &len, NULL);

		//Check if Failed to Read the file into the buffer
		if (!readFile)
		{
			len = 0;		// RESET the length to zero

							//Check the Error Flag 
			ClearCommError(FileDescriptor, &errorFlags, &comStat);
		}

		//Check if there is an error
		if (errorFlags > 0)
		{
			ClearCommError(FileDescriptor, &errorFlags, &comStat);
			return -1;
		}
	}
	return len;
}



/*
Function to indicate OpenBCI to Start Streaming
*/
bool
OpenBCI_ModuleADC::StartStreaming(HANDLE FileDescriptor)
{
	mIsStreaming = true;
	char *buffer = "b";
	DWORD len = 1;

	bool fileWritten = WriteFile(FileDescriptor, (LPSTR)buffer, len, &len, NULL);

	if (fileWritten)
	{
		bciout << "Streaming Started" << endl;
	}
	return fileWritten;
}



/*
Function to indicate OpenBCI to Stop Streaming
*/
bool
OpenBCI_ModuleADC::StopStreaming(HANDLE FileDescriptor)
{
	mIsStreaming = false;
	char *buffer = "s";
	DWORD len = 1;

	bool fileWritten = WriteFile(FileDescriptor, (LPSTR)buffer, len, &len, NULL);

	if (fileWritten)
	{
		bciout << "Streaming Stopped" << endl;
	}
	return fileWritten;
}



/*
Function to handle input data from the hardware device and store into the buffer.

*Return 1 : If reads $$$ and ready to start
*Return 2 : If already streaming
*Return 3 : If fails to receive $$$
*/
int
OpenBCI_ModuleADC::BufferHandler(unsigned char buffer[])
{
	//Check if the Streaming Started
	if (mIsStreaming == false)
	{
		if (buffer[0] == '$')
		{
			mDollarBill++;
		}
		else if (buffer[0] != '$' && mDollarBill > 0) mDollarBill = 0;

		//Check if received 3 $
		if (mDollarBill == THREE_DOLLAR_SIGN)
		{
			mDollarBill = 0;
			return 1;  // Handshake done, ready to start streaming
		}
	}
	//Check if already Streaming
	else if (mIsStreaming == true)
	{
		//Add the new data into the buffer 
		mParseBuffer[mNumberOfBytesAdded] = buffer[0];


		//Increament the # of bytes in the buffer 
		mNumberOfBytesAdded++;

		return 2;	// already streaming
	}

	return 3; //Failed to Start Streaming
}



/*
Function to do tasks before streaming the data from the OpenBCI board
*/
bool
OpenBCI_ModuleADC::PreStreaming(HANDLE FileDescriptor)
{
	int length;
	unsigned char buffer[1];
	int BufferValue;

	//LOOP until handshake is successfully completed
	while (1)
	{
		length = ReadSerial(FileDescriptor, buffer, 1);

		if (mFirstTimeStream)
		{
			Sleep(20);
			mCount++;
		}

		//Check if there is no handshake within VALID HANDSHAKE TIME (i.e. 4 seconds in our case)
		if (mCount>VALID_HANDSHAKE_TIME)
		{
			mCount = 0;
			return 0;
		}

		if (length > 0)	// Check if something was read
		{
			//In order to check if read $$$, get the buffer value
			BufferValue = BufferHandler(buffer);

			//If receive $$$,  the communication between computer and bluetooth has been established 
			if (BufferValue == 1)
			{
				bciout << "Communication Initialized " << mCount << endl;
				mCount = 0;
				return 1;
			}

		}
	}
}



/*
Function to parse the data into the buffer
*/
void
OpenBCI_ModuleADC::ByteParser()
{
	int channelNumber = 0;
	int ByteCount = 0;
	int temp = 0;

	//LOOP to Process the data in the buffer and convert into INTEGER
	for (int block = 0; block < mSampleBlockSize; block++)
	{
		channelNumber = 0;

		//Check if the first elelment of every block does not equal to 0xA0
		//If no then this block of data is INVALID
		if (mParseBuffer[BUFFERSIZE*block] != 0xA0)
		{
			bciout << "BLOCK INVALID" << endl;
		}

		//Store the Sample Number
		mPacket.BlockNum[block] = int(mParseBuffer[1 + BUFFERSIZE*block]);

		//Store data in to mPacket (i.e. Storing the Byte3 to Byte 26 = 24 Bytes for all 8 channels: (24bit / 3 Bytes) for each channel)
		for (int index = 2; index < 26; index++)
		{
			temp |= ((0xFF & mParseBuffer[index + BUFFERSIZE*block]) << (EEG_CHANNELS_16 - (ByteCount*EEG_CHANNELS_8)));
			ByteCount++;

			if (ByteCount == 3)
			{
				//Check if the Integer is negative 
				if ((temp & 0x00800000) > 0)
				{
					temp |= 0xFF000000;
				}
				//Else if the Integer is positive  
				else
				{
					temp &= 0x00FFFFFF;
				}

				//Read it into the output packet
				mPacket.OutPut[channelNumber][block] = temp;
				channelNumber++;

				//Finish read all 8 channels
				ByteCount = 0;
				temp = 0;
			}
		}
	}
}



/*
	Function to actually stream the data from the device continuously
	(Gets called again and again)
*/
void
OpenBCI_ModuleADC::Streaming(HANDLE FileDescriptor)
{
	unsigned char buffer[BUFFERSIZE*SAMPLEBLOCKSIZE];
	int readSerial = 0;
	int totalData = BUFFERSIZE*mSampleBlockSize;
	int lastindex = 0;

	//LOOP for continuously querying the device 
	while (1)
	{
		//Read from the Buffer 
		readSerial = ReadSerial(FileDescriptor, buffer, readSerial);

		//Check if data is left in the buffer
		if (readSerial > 0)
		{
			//Read enough bytes into buffer 
			for (int index = 0; index < readSerial; index++)
			{
				//Read the data into a buffer
				mParseBuffer[index + lastindex] = buffer[index];
			}
			lastindex += readSerial;
		}

		//Calculate the data left and re-initialize the value of total data
		readSerial = totalData - readSerial;
		totalData = readSerial;

		//Check if the whole data is read
		if (readSerial == 0)
		{
			//Call the function ByteParser to parse the data in the buffer 
			ByteParser();
			break;
		}
	}
}

/*
//Function to turn the channels ON

void
OpenBCI_2ADC::TurnChannelsOn(HANDLE FileDescriptor)
{
char * buffer;
DWORD len = 1;
int channels;
if (mUse8Channels)
channels = EEG_CHANNELS_8;
else
channels = EEG_CHANNELS_16;

for (int ch = 1; ch <= channels; ch++)
{
if (ch <= 8)
{
if (ch == 1)
buffer = CHANNEL_1_ON;
if (ch == 2)
buffer = CHANNEL_2_ON;
if (ch == 3)
buffer = CHANNEL_3_ON;
if (ch == 4)
buffer = CHANNEL_4_ON;
if (ch == 5)
buffer = CHANNEL_5_ON;
if (ch == 6)
buffer = CHANNEL_6_ON;
if (ch == 7)
buffer = CHANNEL_7_ON;
if (ch == 8)
buffer = CHANNEL_8_ON;

}
else
{
if (ch == 9)
buffer = CHANNEL_9_ON;
if (ch == 10)
buffer = CHANNEL_10_ON;
if (ch == 11)
buffer = CHANNEL_11_ON;
if (ch == 12)
buffer = CHANNEL_12_ON;
if (ch == 13)
buffer = CHANNEL_13_ON;
if (ch == 14)
buffer = CHANNEL_14_ON;
if (ch == 15)
buffer = CHANNEL_15_ON;
if (ch == 16)
buffer = CHANNEL_16_ON;
}
bool fileWritten = WriteFile(FileDescriptor, (LPSTR)buffer, len, &len, NULL);

//Board reaction time
Sleep(100);

if (fileWritten)
bciout << "Channel " << ch << " Turned ON" << endl;
}
}
*/

/*
	Function to SET GAIN for every Channel
*/
void
OpenBCI_ModuleADC::SetChannels(HANDLE FileDescriptor)
{
	char* buffer = "v";	//Indicates that the command will alter Settings for Each Channel
	DWORD len = 1;

	int numberOfChannels = EEG_CHANNELS_8;

	//Check whether using 8 channels or 16 channels 
	if (!mUse8Channels)
	{
		numberOfChannels = EEG_CHANNELS_16;
	}

	//LOOP to Parse every Channel
	for (int channel = 0; channel < numberOfChannels; channel++)
	{
		//LOOP through Channel Settings
		for (int setting = 0; setting < CHANNEL_SETTINGS; setting++)
		{
			if (setting == 0)
				buffer = ENABLE_CHANNEL_SETTINGS_ACCESS;
			//Second bit indicates the channel to be set 
			if (setting == 1)
			{
				//Checking if the channel number is less than than maximum channels available 
				if (channel <= EEG_CHANNELS_8)
				{
					string channelNumber = to_string(channel + 1);
					buffer = new char[channelNumber.length() + 1];
					strcpy(buffer, channelNumber.c_str());
				}
				else
				{
					if (channel == 8)
						buffer = CHANNEL_9_ON;
					if (channel == 9)
						buffer = CHANNEL_10_ON;
					if (channel == 10)
						buffer = CHANNEL_11_ON;
					if (channel == 11)
						buffer = CHANNEL_12_ON;
					if (channel == 12)
						buffer = CHANNEL_13_ON;
					if (channel == 13)
						buffer = CHANNEL_14_ON;
					if (channel == 14)
						buffer = CHANNEL_15_ON;
					if (channel == 15)
						buffer = CHANNEL_16_ON;
				}
			}

			//Third bit to check if the Channel is turned ON.
			if (setting == 2) //Power Down ON (Default)
			{
				buffer = POWER_ON;
			}

			//Set Channel Gain
			if (setting == 3)
			{
				int gain_factor = 6;
				if (mChannelGain[channel] == GAIN_1)
					gain_factor = 0;
				if (mChannelGain[channel] == GAIN_2)
					gain_factor = 1;
				if (mChannelGain[channel] == GAIN_4)
					gain_factor = 2;
				if (mChannelGain[channel] == GAIN_6)
					gain_factor = 3;
				if (mChannelGain[channel] == GAIN_8)
					gain_factor = 4;
				if (mChannelGain[channel] == GAIN_12)
					gain_factor = 5;
				if (mChannelGain[channel] == DEFAULT_CHANNEL_GAIN)
					gain_factor = 6;

				string channelGain = to_string(gain_factor);
				buffer = new char[channelGain.length() + 1];
				strcpy(buffer, channelGain.c_str());
			}

			//SET Bias and Impedance for each channel 
			if (setting == 4)
				buffer = ADSINPUT_NORMAL; //input type ADSINPUT_NORMAL (default)
			if (setting == 5)
				buffer = INCLUDE_IN_BIAS; //bias set
			if (setting == 6)
				buffer = CONNECT_INPUT_TO_SRB2; //srb2 set
			if (setting == 7)
				buffer = DISCONNECT_ALL_N_INPUTS_FROM_SRB1; //srb1 set
			if (setting == 8)
				buffer = DISABLE_CHANNEL_SETTINGS_ACCESS;	//Sending byte to Disable Channel Settings Access

			bool fileWritten = WriteFile(FileDescriptor, (LPSTR)buffer, len, &len, NULL);

			//board reaction time 
			Sleep(100);

		}
		bciout << "Setting for Channel " << channel + 1 << " out of " << numberOfChannels << " has been completed." << endl;
	}
}



/*
Function to read the contents with in the buffer
*/
void
OpenBCI_ModuleADC::ReadBuffer(HANDLE FileDescriptor, int numberOfBytes)
{
	unsigned char buffer[READ_BUFFER_SIZE];
	DWORD size = numberOfBytes;
	ReadFile(FileDescriptor, (LPSTR)buffer, size, &size, NULL);
}



/*
Function to Read the data using ReadFile until 3 Dollars are read
*/
/*
void
OpenBCI_2ADC::ReadUntilThreeDollars(HANDLE FileDescriptor)
{
int count = THREE_DOLLAR_SIGN;
DWORD len = 1;
char *buffer = new char[1];
while (count > 0)
{
ReadFile(FileDescriptor, (LPSTR)buffer, len, &len, NULL);
if (buffer[0] == '$')
count--;
bciout << "Buffer character read"<< endl;
}
}
*/

/*
Function to read the Channel Settings and query the OpenBCI board
*/
void
OpenBCI_ModuleADC::ReadChannelSetting(HANDLE FileDescriptor)
{
	char* buffer = "?";
	DWORD len = 1;

	bool fileWritten = WriteFile(FileDescriptor, (LPSTR)buffer, len, &len, NULL);

	Sleep(100);

	if (fileWritten)
	{
		bciout << "Query the board" << endl;
	}
}



/*
Function to RESET the OpenBCI board
*/
void
OpenBCI_ModuleADC::ResetBoard(HANDLE FileDescriptor)
{
	char * buffer = "v";
	DWORD len = 1;

	bool fileWritten = WriteFile(FileDescriptor, (LPSTR)buffer, len, &len, NULL);

	//Board Reaction Time
	Sleep(100);

	if (fileWritten)
	{
		bciout << "Board RESET" << endl;
	}
	return;
}



/*
Function to SET the Channel Mode to 8 or 16
*/
void
OpenBCI_ModuleADC::SetChannelMode(HANDLE FileDescriptor)
{
	//Check if using 8 channel mode 
	if (mUse8Channels)
	{
		//Send the command to board to specify using 8 channel mode 
		char *buffer = "c";
		DWORD length = 1;

		//write one bit into the board 
		bool fileWritten = WriteFile(FileDescriptor, (LPSTR)buffer, length, &length, NULL);

		//Board Reaction time 
		Sleep(100);

		//Check if Successfully Written 
		if (fileWritten)
		{
			bciout << "Using 8 Channel Mode" << endl;
		}
	}
	//Check if using 16 Channel mode
	else if (!mUse8Channels)
	{
		//Send command to board to specify using 16 Channel mode 
		char *buffer = "C";
		DWORD length = 1;

		//Write one bit into the board 
		bool fileWritten = WriteFile(FileDescriptor, (LPSTR)buffer, length, &length, NULL);

		//Board reaction time 
		Sleep(100);

		if (fileWritten)
		{
			bciout << "Using 16 channel mode" << endl;
		}
	}
}



/*
Function to SET the Default Channel Settings
*/
void
OpenBCI_ModuleADC::SetDefaultSettings(HANDLE FileDescriptor)
{
	char *buffer = "d";
	DWORD length = 1;

	//Write one bit into the board 
	bool fileWritten = WriteFile(FileDescriptor, (LPSTR)buffer, length, &length, NULL);

	//Board Reaction time 
	Sleep(100);

	//Check if Successfully Written 
	if (fileWritten)
	{
		bciout << "SET the board to Default Channel settings" << endl;
	}
}



/*
Function to Auto Configure the Port
*/
void
OpenBCI_ModuleADC::OnAutoConfig()
{
	if (Parameter("PortName") != "auto")
		bcierr << "Error, can't set the portname manually, it will be set automatically, Please set the value to <auto>" << endl;
	//Check if the port is already open
	if (!mOpenPort)
	{
		//check from com port 1 to com port 21 
		for (int p = 1; p < NUM_COM_PORTS; p++)
		{
			string PortName = "\\\\.\\COM" + to_string(p);
			char *CopyPortName = new char[PortName.size() + 1];
			std::strcpy(CopyPortName, PortName.c_str());

			//Now Open the port and get the handle
			mFileDescriptor = OpenPort(CopyPortName);

			if (mFileDescriptor == INVALID_HANDLE_VALUE)
			{
				bciout << "PORT COM" << p << " NO Connection!!!" << endl;
				continue;
			}

			// If reached here, that means port successfully connected
			bciout << "PORT COM" << p << " Connected Successfully!!";

			if (FlushFileBuffers(mFileDescriptor))
			{
				bciout << "BUFFER refreshed Successfully" << endl;
			}

			//DO the HANDSHAKE
			bool SuccessOpenPort = PreStreaming(mFileDescriptor);
			delete[] CopyPortName;

			//If there is no Hand Shake
			if (!SuccessOpenPort)
			{
				//Output the message and continue to next port
				bciout << "PORT COM" << p << " NO Handshake" << endl;
				continue;
			}
			mOpenPort = true;
			break;
		}
	}

	// If there is no PORT available for connection
	//i.e. All ports were tried and none gets connected, then provide a warning
	if (mOpenPort == false)
	{
		bcierr << "DEVICE NOT AVAILABLE" << endl;
		return;
	}

	//if successfully connect to device, set all the parameter 

	//If Parameter SourceCh is not explicitly specified byt the user.
	if (Parameter("SourceCh") == "auto")
	{
		bciout << "SourceCh SET automatically to 8 Channels" << endl;
		mNumberOfChannels = 8;
	}
	else
	{
		mNumberOfChannels = Parameter("SourceCh");
		bciout << "SourceCh SET to the number of channels manually specified in the Source Parameters" << endl;
	}

	Parameter("SourceCh") = mNumberOfChannels;

	// if the channels list is auto 
	if (Parameter("SourceChList") == "auto")
	{
		//the default value is the first 8 channels
		Parameter("SourceChList")->SetNumValues(mNumberOfChannels);
		for (int ch = 0; ch < mNumberOfChannels; ch++)
		{
			Parameter("SourceChList")(ch) = ch + 1;
		}
	}
	else if (Parameter("SourceChList") != "auto")
	{
		bciout << "SourceChList is SET manually, for setting it automatically, Please set the value to <auto>" << endl;
		if (Parameter("SourceChList")->NumValues() != mNumberOfChannels)
		{
			mNumberOfChannels = Parameter("SourceChList")->NumValues();
			Parameter("SourceCh") = mNumberOfChannels;
		}

	}

	if (mNumberOfChannels == 8)
	{
		mUse8Channels = true;
	}
	else
	{
		mUse8Channels = false;
	}


	//store channel index into member variable 
	for (int ch = 0; ch < mNumberOfChannels; ch++)
	{
		mSourceChannelList.push_back(Parameter("SourceChList")(ch) - 1);
	}


	if (Parameter("SamplingRate") != "auto")
	{	//if the user set the sampling rate, so its not auto
		bcierr << "Error, can't set the Sampling Rate manually, it will be set automatically to 250, Please set the value to <auto>" << endl;
		return;
	}

	Parameter("SamplingRate") = SAMPLING_RATE;


	if (Parameter("SampleBlockSize") != "auto")
	{	//if the user set the sample block size 
		//bcierr << "Error, can't set the Sample Block Size manually, it is set to 25, to change it, go to the header file and change the constant" << endl;
		mSampleBlockSize = ActualParameter("SampleBlockSize").ToNumber();
		bciout << "Sample block size SET automatically to " << mSampleBlockSize << endl;
	}
	else
	{
		Parameter("SampleBlockSize") = SAMPLEBLOCKSIZE;
		mSampleBlockSize = SAMPLEBLOCKSIZE;
	}



	if (Parameter("SourceChGain") != "auto")
	{	//if the user set the source channel gain 
		bcierr << "Error, Source channel gain will be Calculated automatically. This value can be change by changing ChannelsGain" << endl << "Please set the value to <auto>" << endl;
		return;
	}




	//SET Channel Names
	if (Parameter("ChannelNames") == "auto")
	{
		Parameter("ChannelNames")->SetNumValues(mNumberOfChannels + 1);
		for (int ch = 0; ch < mNumberOfChannels; ch++)
		{
			Parameter("ChannelNames")(ch) = "CHANNEL " + to_string(int(Parameter("SourceChList")(ch)));
		}
		//Parameter("ChannelNames")(mNumberOfChannels) = "LostSampleindicator";
	}
	else if (Parameter("ChannelNames") != "auto")
	{
		if (Parameter("ChannelNames")->NumValues() != mNumberOfChannels)
		{	//if the user set channels' name and doesn't set all channels 
			bcierr << "number of channel names must match number of source channels (" << mNumberOfChannels << ")" << endl;
			return;
		}
		bciout << "ChannelNames are set manually by the user, if you want it be set automatically, Please set the value to <auto>" << endl;
	}


	//Check if user sets the Channels Gain but doesn't specify the values 
	if (Parameter("ChannelsGain") != "auto"&&Parameter("ChannelsGain")->NumValues() != mNumberOfChannels)
	{
		bcierr << "Please specify gain for all channels" << endl;
		return;
	}

	//SPECIFY the Channels Gain
	else if (Parameter("ChannelsGain") == "auto")
	{
		Parameter("ChannelsGain")->SetNumValues(mNumberOfChannels + 1);
		for (int ch = 0; ch < mNumberOfChannels; ch++)
		{
			Parameter("ChannelsGain")(ch) = DEFAULT_CHANNEL_GAIN;
		}
	}

	Parameter("ChannelsGain")(mNumberOfChannels) = 1;
	int possiblegains[] = { GAIN_1,GAIN_2,GAIN_4,GAIN_6,GAIN_8,GAIN_12,DEFAULT_CHANNEL_GAIN };
	bool correctGains = false;

	//Check if the Channel Gains are Reasonable
	for (int ch = 0; ch < mNumberOfChannels; ch++)
	{
		correctGains = false;

		for (int i = 0; i < SIZE_OF_POSSIBLE_GAINS; i++)
		{
			//Check if all gains belong to the list of possilbe gains
			if (Parameter("ChannelsGain")(ch) == possiblegains[i])
			{
				correctGains = true;
				break;
			}
		}

		//Check if there are Impossible/Incorrect Channel Gains
		if (correctGains == false) {
			bcierr << "the gain for channel " << mSourceChannelList[ch] + 1 << " is out of range the possible value for gain is 1,2,3,6,8,12,24" << endl;
			return;
		}
	}

	//SET Source Channel Gains 
	Parameter("SourceChGain")->SetNumValues(mNumberOfChannels + 1);

	//set default channel gains
	for (int ch = 0; ch < EEG_CHANNELS_16; ch++)
	{
		mChannelGain[ch] = DEFAULT_CHANNEL_GAIN;
	}

	//read channel gain from the user 
	for (int ch = 0; ch < mNumberOfChannels; ch++)
	{
		mChannelGain[mSourceChannelList[ch]] = Parameter("ChannelsGain")(ch);
	}

	//convert the channel gains 
	for (int ch = 0; ch < mNumberOfChannels; ch++)
	{
		double gainFactor = CONVERSION_FACTOR*(24.0 / mChannelGain[mSourceChannelList[ch]]);
		Parameter("SourceChGain")(ch) << gainFactor << "V";
	}
	Parameter("SourceChGain")(mNumberOfChannels) << 1.0 << "V";

}

void
OpenBCI_ModuleADC::OnPreflight( SignalProperties& Output ) const
{
	//SETUP the Output Properties 
	SignalType sigType = SignalType::int32;
	int samplesPerBlock = ActualParameter("SampleBlockSize");
	int numberOfSignalChannels = ActualParameter("SourceCh");
	int samplingRate = ActualParameter("SamplingRate");

	Output = SignalProperties(numberOfSignalChannels, samplesPerBlock, sigType);
	bciout << "Number of Signal Channels = " << numberOfSignalChannels << endl;
	bciout << "Sample Block Size  = " << samplesPerBlock << endl;
	bciout << "Sampling Rate = " << samplingRate << endl;
}

void
OpenBCI_ModuleADC::OnInitialize( const SignalProperties& Output )
{

	ResetBoard(mFileDescriptor);
	SetDefaultSettings(mFileDescriptor); //Change to check if settings are default already
	StopStreaming(mFileDescriptor);
	Sleep(100);

	if (FlushFileBuffers(mFileDescriptor))
	{
		bciout << "Flush buffer" << endl;
	}

	SetChannelMode(mFileDescriptor);
	SetChannels(mFileDescriptor);
	ReadChannelSetting(mFileDescriptor);
	ReadBuffer(mFileDescriptor, 3000);
	PreStreaming(mFileDescriptor);
	StartStreaming(mFileDescriptor);



	//Set for previous data vectors to zero
	for (int i = 0; i < EEG_CHANNELS_8; i++)
	{
		mFirst8Channels[i] = 0;
		mLast8Channels[i] = 0;
	}

	mClock.SetInterval(Time::Seconds(MeasurementUnits::SampleBlockDuration()));
	mClock.Start();
}

/* 
	Function to Acquire data and Output data on BCI2000 UI
*/

void 
OpenBCI_ModuleADC::Process(const GenericSignal& Input,GenericSignal& Output)
{
	DWORD begin_time = 0, end_time = 0;
	WithThreadPriority(ThreadUtils::Priority::Maximum - 1)
		//mClock.Wait(Time::FromIntTimeout(MeasurementUnits::SampleBlockDuration() * 1000 - (end_time - begin_time)));
		mClock.Wait();

	begin_time = GetTickCount();
	
	for (int sample = 0; sample < mSampleBlockSize; sample++)
	{
		while (q.Empty());
		
		for (int channel = 0; channel < mNumberOfChannels; channel++)
		{			
			SynchronizedQueue<int>::Consumable c= q.Consume();
			if (c)
			{
				Output(channel, sample) = (*c);
			}
		}
	}	
	end_time = GetTickCount();
	//bciout << "Time Elapsed = " << (end_time - begin_time);
}


/*
//Actual Function that works
void
OpenBCI_2ADC::DoAcquire(GenericSignal& Output)
{
	Streaming(mFileDescriptor);

	//If Using the 8 channel mode 
	if (mUse8Channels)
	{
		for (int channel = 0; channel < mNumberOfChannels; channel++)
		{
			for (int sample = 0; sample < mSampleBlockSize; sample++)
			{
				Output(channel, sample) = mPacket.OutPut[channel][sample];
			}
		}
	}

	//If Using 16 channel mode 
	else if (!mUse8Channels)
	{
		for (int sample = 0; sample < mSampleBlockSize; sample++)
		{
			//Daisy Board
			if (mPacket.BlockNum[sample] % 2 == 0)
			{
				for (int channel = 0; channel < mNumberOfChannels; channel++)
				{
					if (mSourceChannelList[channel] >= EEG_CHANNELS_8)
					{
						Output(channel, sample) = (mPacket.OutPut[mSourceChannelList[channel] - EEG_CHANNELS_8][sample] + mLast8Channels[mSourceChannelList[channel] - EEG_CHANNELS_8]) / 2;
						mLast8Channels[mSourceChannelList[channel] - EEG_CHANNELS_8] = mPacket.OutPut[mSourceChannelList[channel] - EEG_CHANNELS_8][sample];
					}
					else
					{
						Output(channel, sample) = mFirst8Channels[mSourceChannelList[channel] - EEG_CHANNELS_8];
					}
				}
			}
			//Cyton Board
			else if (mPacket.BlockNum[sample] % 2 == 1)
			{
				for (int channel = 0; channel < mNumberOfChannels; channel++)
				{
					if (mSourceChannelList[channel] < EEG_CHANNELS_8)
					{
						Output(channel, sample) = (mPacket.OutPut[mSourceChannelList[channel]][sample] + mFirst8Channels[mSourceChannelList[channel]]) / 2;
						mFirst8Channels[mSourceChannelList[channel]] = mPacket.OutPut[mSourceChannelList[channel]][sample];
					}
					else
					{
						Output(channel, sample) = mLast8Channels[mSourceChannelList[channel]];
					}
				}
			}
		}
	}
}

*/

/*
Function to Add Bytes of Data to Queue
*/

void OpenBCI_ModuleADC::AddBytesToQueue()
{
	int temp = 0;

	while (mKeepAcquiringData)
	{
		Streaming(mFileDescriptor);

		if (mUse8Channels)
		{
			for (int sample = 0; sample < mSampleBlockSize; sample++)
			{
				for (int channel = 0; channel < mNumberOfChannels; channel++)
				{
					temp = mPacket.OutPut[channel][sample];
					q.Produce(temp);
				}
			}
		}

		//If Using 16 channel mode 
		else if (!mUse8Channels)
		{
			mPrevSampleIndex = mPacket.BlockNum[0] - 1;
			bool sampleLost;

			for (int sample = 0; sample < mSampleBlockSize; sample++)
			{
				sampleLost = false;

				if (mPacket.BlockNum[sample] != mPrevSampleIndex + 1)
				{
					sampleLost = true;
				}
				mPrevSampleIndex = mPacket.BlockNum[sample];

				if (mPacket.BlockNum[sample] == DATA_BLOCK_SIZE)
				{
					mPrevSampleIndex = -1;
				}

				//Daisy (Extension) Board
				if (mPacket.BlockNum[sample] % 2 == 0)
				{
					for (int channel = 0; channel < mNumberOfChannels; channel++)
					{
						if (mSourceChannelList[channel] >= EEG_CHANNELS_8)
						{
							temp = (mPacket.OutPut[mSourceChannelList[channel] - EEG_CHANNELS_8][sample] + mLast8Channels[mSourceChannelList[channel] - EEG_CHANNELS_8]) / 2;
							mLast8Channels[mSourceChannelList[channel] - EEG_CHANNELS_8] = mPacket.OutPut[mSourceChannelList[channel] - EEG_CHANNELS_8][sample];
						}
						else
						{
							if (sampleLost)
							{
								mFirst8Channels[mSourceChannelList[channel]] += mPacket.OutPut[mSourceChannelList[channel]][sample - 1] - mPacket.OutPut[mSourceChannelList[channel]][sample - 2];
							}
							temp = mFirst8Channels[mSourceChannelList[channel]];
						}
						q.Produce(temp);
					}
				}
				//Cyton Board
				else if (mPacket.BlockNum[sample] % 2 == 1)
				{
					for (int channel = 0; channel < mNumberOfChannels; channel++)
					{
						if (mSourceChannelList[channel] < EEG_CHANNELS_8)
						{
							temp = (mPacket.OutPut[mSourceChannelList[channel]][sample] + mFirst8Channels[mSourceChannelList[channel]]) / 2;
							mFirst8Channels[mSourceChannelList[channel]] = mPacket.OutPut[mSourceChannelList[channel]][sample];
						}
						else
						{
							if (sampleLost)
							{
								mLast8Channels[mSourceChannelList[channel]] += mPacket.OutPut[mSourceChannelList[channel]][sample - 1] - mPacket.OutPut[mSourceChannelList[channel]][sample - 2];
							}
							temp = mLast8Channels[mSourceChannelList[channel] - EEG_CHANNELS_8];
						}
						q.Produce(temp);
					}
				}
			}
		}
	}

}

void
OpenBCI_ModuleADC::OnStartAcquisition()
{
	// This method is called from the acquisition thread once the system is initialized.
	// You should use this space to start up your amplifier using its API.  Any initialization
	// here is done in the acquisition thread, so non-thread-safe APIs should work if initialized here.
	//CALL_AMP_API(StartAcquisition);
	//CALL_AMP_API(SetTriggerCallback(&TriggerCallback, this));

	mKeepAcquiringData = true;
	th1 = new std::thread(&OpenBCI_ModuleADC::AddBytesToQueue, this);
	//Sleep(500);
}


void
OpenBCI_ModuleADC::OnStopAcquisition()
{
	mKeepAcquiringData = false;
	mClock.Stop();
	th1->join();
	delete th1;
	q.Clear();
	bciout << "Process Stopped Acquiring Data!! Please check your connection." << endl << "Possible Solution: Disconnect & reconnect your device" << endl;
}

