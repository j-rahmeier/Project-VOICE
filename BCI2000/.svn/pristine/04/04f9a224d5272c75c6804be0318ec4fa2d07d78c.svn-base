////////////////////////////////////////////////////////////////////////////////
// Authors: Akshay Vyas, National Center for Adaptive Neurotechnologies, HRI, Albany
// Description: OpenBCI_ModuleADC header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_OPENBCI_MODULEADC_H  // makes sure this header is not included more than once
#define INCLUDED_OPENBCI_MODULEADC_H

#include "BufferedADC.h"
#include <set>
#include "Win32Defs.h"
#include "Clock.h"
#include <deque>
#include "SynchronizedQueue.h"


/* Macros Definitions (Constants)*/
/*-------------------------------*/

#define BAUD_RATE CBR_115200				//Baud Rate for OpenBCI (115200bps)
#define EEG_CHANNELS_8 8					//Use 8 channels
#define EEG_CHANNELS_16 16					//Use 16 channels 
#define ADS1299_VREF 4.5					//Reference voltage  
#define BUFFERSIZE 33						//Buffer Size 
#define SAMPLEBLOCKSIZE 125					//Sample Block Size  
#define THREE_DOLLAR_SIGN 3					//Count Three Dollar Dign
#define READ_BUFFER_SIZE 5000				//read buffer size 
#define SAMPLING_RATE 250					//Default Sampling Rate 
#define DATA_BLOCK_SIZE 255					//Data Block Size 
#define CONVERSION_FACTOR 0.000000061362	//Conversion Factor for Gain 
#define SIZE_OF_POSSIBLE_GAINS 7			//7 number for possible gains 
#define VALID_HANDSHAKE_TIME 200			//Valid Handshake Time (Iterations)
#define CHANNEL_SETTINGS 9					//Total Number of Channel Settings 

// All Possible Gains 
#define GAIN_1 1
#define GAIN_2 2
#define GAIN_4 4
#define GAIN_6 6
#define GAIN_8 8
#define GAIN_12 12

// ENABLE-DISABLE CHANNEL SETTINGS ACCESS 
#define ENABLE_CHANNEL_SETTINGS_ACCESS ((char*)"x")
#define DISABLE_CHANNEL_SETTINGS_ACCESS ((char*)"X")

// POWER DOWN BITS
#define POWER_ON ((char*)"0") 
#define POWER_OFF ((char*) "1")


#define DEFAULT_CHANNEL_GAIN 1				//Default Channel Gain
#define NUM_COM_PORTS 21					//Total Number of COM Ports 

// All Channel Commands (send to the device) for turning the channels on
#define CHANNEL_1_ON ((char*)"!")
#define CHANNEL_2_ON ((char*)"@")
#define CHANNEL_3_ON ((char*)"#")
#define CHANNEL_4_ON ((char*)"$")
#define CHANNEL_5_ON ((char*)"%")
#define CHANNEL_6_ON ((char*)"^")
#define CHANNEL_7_ON ((char*)"&")
#define CHANNEL_8_ON ((char*)"*")
#define CHANNEL_9_ON ((char*)"Q")
#define CHANNEL_10_ON ((char*)"W")
#define CHANNEL_11_ON ((char*)"E")
#define CHANNEL_12_ON ((char*)"R")
#define CHANNEL_13_ON ((char*)"T")
#define CHANNEL_14_ON ((char*)"Y")
#define CHANNEL_15_ON ((char*)"U")
#define CHANNEL_16_ON ((char*)"I")

// INPUT_TYPE_SET (Select the ADC Channel input source)
#define ADSINPUT_NORMAL ((char*)"0") // default
#define ADSINPUT_SHORTED ((char*)"1")
#define ADSINPUT_BIAS_MEAS ((char*)"2")
#define ADSINPUT_MVDD ((char*)"3")
#define ADSINPUT_TEMP ((char*)"4")
#define ADSINPUT_TESTSIG ((char*)"5")
#define ADSINPUT_BIAS_DRP ((char*)"6")
#define ADSINPUT_BIAS_DRN ((char*)"7")

// BIAS_SET (Select to include the channel input in BIAS generation)
#define REMOVE_FORM_BIAS ((char*)"0") //default
#define INCLUDE_IN_BIAS ((char*)"1")

/*SRB2_SET
(Select to connect this channel’s P input to the SRB2 pin.
This closes a switch between P input and SRB2 for the given channel,
and allows the P input also remain connected to the ADC)
*/
#define DISCONNECT_INPUT_FROM_SRB2 ((char*)"0")
#define CONNECT_INPUT_TO_SRB2 ((char*)"1") //default

/*
SRB1_SET
(Select to connect all channels’ N inputs to SRB1. This effects all pins, and disconnects all N inputs from the ADC.)
*/
#define DISCONNECT_ALL_N_INPUTS_FROM_SRB1 ((char*)"0") //default
#define CONNECT_ALL_N_INPUTS_TO_SRB1 ((char*)"1")


class OpenBCI_ModuleADC : public BufferedADC
{
 public:
  OpenBCI_ModuleADC();
  ~OpenBCI_ModuleADC();
  
  void OnAutoConfig() override;
  void OnPublish() override;
  void OnPreflight(SignalProperties& Output) const  override;
  void OnInitialize(const SignalProperties&) override;
  void OnStartAcquisition() override;
  void OnStopAcquisition()  override;
  void Process(const GenericSignal& Input, GenericSignal& Output) override;
  // void DoAcquire( GenericSignal& Output ) override;
  void OnTrigger(int); // for asynchronous triggers only

  HANDLE OpenPort(const char* PortName);
  int ReadSerial(HANDLE FileDescriptor, unsigned char buf[], int size);
  bool StartStreaming(HANDLE FileDescriptor);
  bool StopStreaming(HANDLE FileDescriptor);
  int BufferHandler(unsigned char buffer[]);
  bool PreStreaming(HANDLE FileDescriptor);
  void ByteParser();
  void Streaming(HANDLE FileDescriptor);
  void SetChannels(HANDLE FileDescriptor);
  void ReadChannelSetting(HANDLE FileDescriptor);
  void ResetBoard(HANDLE FileDescriptor);
  void SetChannelMode(HANDLE FileDescriptor);
  void SetDefaultSettings(HANDLE FileDescriptor);
  void ReadBuffer(HANDLE FileDescriptor, int numberofBytes);
  void AddBytesToQueue();
  //void TurnChannelsOn(HANDLE FileDescriptor);
  //void ReadUntilThreeDollars(HANDLE FileDescriptor);

  //structure used to store data 
  struct Packet
  {
	  int OutPut[EEG_CHANNELS_8][SAMPLEBLOCKSIZE];
	  int BlockNum[SAMPLEBLOCKSIZE];
  }   mPacket;


 private:
  // Use this space to declare any OpenBCI_ModuleADC-specific methods and member variables you'll need
  
  /* Member Variables Declaration*/
  /*-----------------------------*/
	 HANDLE mFileDescriptor;									//File Descriptor 
	 unsigned char mParseBuffer[BUFFERSIZE * SAMPLEBLOCKSIZE];	//Buffer that stores Incoming Data
	 bool mIsStreaming;											//Flag to check if data is streaming
	 int mDollarBill;											//Indicates if receive $$$ 
	 int mNumberOfBytesAdded;									//Count the no. of bytes that have been added 
	 int mCount;												//Counter
	 bool mFirstTimeStream;										//Flag to check if it is the first time stream or not  
	 bool mOpenPort;											//Flag to check whether the port is open or not
	 int mChannelGain[EEG_CHANNELS_16];							//Array to store the Gains for all 16 channels
	 int mSampleBlockSize;										//Records the sample block size
	 std::vector<int> mSourceChannelList;						//Vector to store the Source Channels in form of a List 
	 int mCurrentPort;											//Stores the current port
	 int mNumberOfChannels;										//Stores the number of channels 
	 bool mUse8Channels;										//Flag to check whether using 8 channel mode or 16 channel mode 
	 bool mSetConfig;											//Flag to indicate if user set the configuration or not
	 int mFirst8Channels[EEG_CHANNELS_8];						//Stores the previous values for the first 8 channels 
	 int mLast8Channels[EEG_CHANNELS_8];						//Stores the previous values for the last 8 channels
	 bool mKeepAcquiringData;									//Flag to indicate the continuous acquisition of data 									
	 SynchronizedQueue <int> q;									//Queue to store data
	 Clock mClock;												//Clock variable to keep track of time
	 std::thread *th1;											//New global thread variable
	 int mPrevSampleIndex;
};

#endif // INCLUDED_OPENBCI_MODULEADC_H
