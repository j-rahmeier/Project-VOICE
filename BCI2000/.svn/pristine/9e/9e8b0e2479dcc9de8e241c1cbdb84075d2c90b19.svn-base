/******************************************************************************
 * Program: NIDAQmx class (formerly NIADC class)                              *
 * Module:  NIDAQmxADC.cpp  (formerly NIADC.cpp)                              *
 * Comment: Support for National Instrument boards using DAQmx Driver         *
 * Copyright (C) 2008 Giulio Pasquariello (g.pasqua@inwind.it) & Gaetano      *
 *                    Gargiulo, DIET Biomedical unit - University of          *
 *                    Naples "Federico II"                                    *
 *                                                                            *
 * Version History:                                                           *
 *                                                                            *
 * V2.00 - 2021-04-03 - jhill renamed class & file, and cleaned up the code   *
 * V1.05 - 07/01/2008 - Added documentation of each function                  *
 * V1.00 - 02/01/2008 - First start and also first functioning BETA version   *
 ******************************************************************************/
 
/* $BEGIN_BCI2000_LICENSE$
 * 
 * This file is part of BCI2000, a platform for real-time bio-signal research.
 * [ Copyright (C) 2000-2023: BCI2000 team and many external contributors ]
 * 
 * BCI2000 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * BCI2000 is distributed in the hope that it will be useful, but
 *                         WITHOUT ANY WARRANTY
 * - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * $END_BCI2000_LICENSE$
 */
 
#include <stdio.h>
#include <chrono> // C++11

#include "NIDAQmxADC.h"

using namespace std;

RegisterFilter( NIDAQmxADC, 1 );

// **************************************************************************
// Function:   NIDAQmxADC
// Purpose:    The constructor for the NIDAQmxADC
//             it fills the provided list of parameters and states using
//             the parameters and states it requests from the operator
// Parameters: plist - the list of parameters
//             slist - the list of states
// Returns:    N/A
// **************************************************************************
NIDAQmxADC::NIDAQmxADC() :
  mDeviceNumber( 0 ),
  mSamplesPerSecond( 0 ),
  mSamplesPerBlock( 0 ),
  mSecondsPerBlock( 0.0 ),
  mNumberOfChannels( 0 ),
  mResultCode( 0 ),
  mTaskHandle( 0 ),
  mNumberOfBuffersQueued( 0 )
{
  if( !Dylib::nicaiu_Loaded() ) bcierr << "National Instruments dynamic library nicaiu.dll was not loaded" << endl;

  for( int iBuffer = 0; iBuffer < NIDAQ_MAX_BUFFERS; iBuffer++ )
    mHalfBuffers[ iBuffer ] = NULL;
}

// **************************************************************************
// Function:   ~NIDAQmxADC
// Purpose:    The destructor for the NIDAQmxADC
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
NIDAQmxADC::~NIDAQmxADC()
{  
  // delete all buffers
  for( int buf = 0; buf < NIDAQ_MAX_BUFFERS; buf++ )
  {
    delete [] mHalfBuffers[ buf ];
    mHalfBuffers[ buf ] = NULL;
  }
}

#define CHECK_DAQMX_FAILED( FUNCTION_CALL ) \
( \
  DAQmxFailed( mResultCode = ( FUNCTION_CALL ) )  \
  && ( sprintf( mErrorMessageBuffer, "some DAQmx function returned error code %d", mResultCode ), true )  /* fallback in case DAQmxGetExtendedErrorInfo() fails */  \
  && ( DAQmxGetExtendedErrorInfo( mErrorMessageBuffer, sizeof( mErrorMessageBuffer ) ), true )  \
  && ( bcierr << mErrorMessageBuffer << endl, true ) \
)
#define RETURN_IF_FAILED( FUNCTION_CALL )   if( CHECK_DAQMX_FAILED( FUNCTION_CALL ) ) return mResultCode;

void NIDAQmxADC::Publish()
{
  BEGIN_PARAMETER_DEFINITIONS
    "Source float SamplingRate= 256 256 1 200000 "
       "// The signal's sampling rate in Hz",
    "Source int   SampleBlockSize= 16 16 2 2048 "
       "// The number of samples in one block",
    "Source int   SourceCh= 8 8 1 16 "
       "// The number of channels",
    "Source int   BoardNumber= 1 1 1 16 "
       "// The NI-ADC board's device number",
    "Source list SourceChGain= 1 auto //",
    "Source list SourceChOffset= 1 auto //",
  END_PARAMETER_DEFINITIONS
}

void NIDAQmxADC::AutoConfig( const SignalProperties& )
{
  MutableParamRef sourceChGain   = Parameter( "SourceChGain" );
  MutableParamRef sourceChOffset = Parameter( "SourceChOffset" );
  int nChannels = Parameter( "SourceCh" );
  sourceChGain->SetNumValues( nChannels );
  sourceChOffset->SetNumValues( nChannels );
  for( int iChannel = 0; iChannel < nChannels; iChannel++ )
  {
    sourceChGain( iChannel ) = 1;
    sourceChOffset( iChannel ) = 0;
  }
}

// **************************************************************************
// Function:   Preflight
// Purpose:    Checks parameters for availability and consistence with
//             input signal properties; requests minimally needed properties for
//             the output signal; checks whether resources are available.
// Parameters: Input and output signal properties pointers.
// Returns:    N/A
// **************************************************************************
void NIDAQmxADC::Preflight( const SignalProperties&, SignalProperties& outSignalProperties ) const
{
  unsigned long int serial;
  char devName[ 100 ];

  // Parameter consistency checks: Existence/Ranges and mutual Ranges.
  //    if( (Parameter("SampleBlockSize")%2) != 0)
  //      bcierr << "Sample block Size must be a power of 2" << endl; // if the error message is to be taken at face value this is an incorrect use of %
  
  // if( (Parameter("SourceCh") > 32) )
  //   bcierr << "Number of channels requested, " << Parameter("SourceCh") << " exceeds maximum value." << endl;

  // Resource availability checks.
  int boardNum = Parameter( "BoardNumber" );
  sprintf( devName, "Dev%i", boardNum );
  int error = DAQmxGetDevSerialNum( devName, &serial );
  if( !serial )  bcierr <<"Board number " << Parameter("BoardNumber") << " absent or does not support DAQmx driver ver 8.0" << endl;

  //   Input signal checks.
  Parameter( "SourceChOffset" );
  Parameter( "SourceChGain" );
  Parameter("SamplingRate").InHertz();
  // Requested output signal properties.
  outSignalProperties = SignalProperties(
    Parameter( "SourceCh" ),
    Parameter( "SampleBlockSize" ),
    SignalType::int16
  );
}

// **************************************************************************
// Function:   Initialize
// Purpose:    (1) Assigns validated parameters to variables
//             (2) Stops any running Task
//             (5) Inits the halfbuffer structure
//             (3) Sets the necessary parameter for a NI-DAQ card
//             (4) Starts the Task.
//             It is called each time the operator first starts,
//             or suspends and then resumes, the system
//             (i.e., each time the system goes into the main
//             data acquisition loop (fMain->MainDataAcqLoop())
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void NIDAQmxADC::Initialize( const SignalProperties&, const SignalProperties& )
{
  mNumberOfChannels = Parameter( "SourceCh" );
  mSamplesPerSecond = static_cast<int>( Parameter( "SamplingRate" ).InHertz() );
  mSamplesPerBlock = Parameter( "SampleBlockSize" );
  mSecondsPerBlock = ( double )mSamplesPerBlock / ( double )mSamplesPerSecond;
  mDeviceNumber = Parameter( "BoardNumber" );

  sprintf( mDeviceName, "Dev%i", mDeviceNumber );

  // stop the data acquisition board; this call is required
  // in case mDeviceNumber is not initialized when Halt function is called
  Stop();

  // delete all current buffers and re-allocate them
  mNumberOfBuffersQueued = 0;
  for( int iBuffer = 0; iBuffer < NIDAQ_MAX_BUFFERS; iBuffer++ )
  {
    delete [] mHalfBuffers[ iBuffer ];
    mHalfBuffers[ iBuffer ] = new int16[ mNumberOfChannels * mSamplesPerBlock ];
  }

  // re-configure the board and start it again
  Start();
}


// **************************************************************************
// Function:   ADReadDataBlock
// Purpose:    This function is called within fMain->MainDataAcqLoop()
//             it fills the already initialized array RawEEG with values
//             and DOES NOT RETURN, UNTIL ALL DATA IS ACQUIRED
// Parameters: N/A
// Returns:    0 ... on error
//             1 ... no error
// **************************************************************************
void NIDAQmxADC::Process( const GenericSignal&, GenericSignal& signal )
{
  // wait until we are notified that data is there
  // let's wait five times longer than what we are supposed to wait
  int timeoutInMilliseconds = ( int )( 0.5 + 1000.0 * mSecondsPerBlock * 5 );

  std::unique_lock< std::mutex > lock( mDataMutex );
  if( mConditionVariable.wait_for(
    lock,
    std::chrono::milliseconds( timeoutInMilliseconds ),
    [ this ](){ return mNumberOfBuffersQueued > 0; }
  ) ) // after a successful wait_for(), we own the lock, so we'll use these braces to
  {   // effectively mark the mutex critical section (but they are purely cosmetic - we still need to unlock manually, so do not `return`!)
    int16 * buffer = mHalfBuffers[ 0 ]; // the "oldest" buffer in the queue
    if( buffer )
    {
      for( int iChannel = 0; iChannel < mNumberOfChannels; iChannel++ )
        for( int iSample = 0; iSample < mSamplesPerBlock; iSample++ )
          signal( iChannel, iSample ) = *buffer++;
  
      // recycle the "oldest" buffer (previous implementations would delete [] it here, and allocate a new one again in GetData)
      int16 * recycledBuffer = mHalfBuffers[ 0 ];
      // if we had more than one buffer, "move" the newer ones one step down
      for( int iBuffer = 1; iBuffer < mNumberOfBuffersQueued; iBuffer++ )
        mHalfBuffers[ iBuffer - 1 ] = mHalfBuffers[ iBuffer ];
      mHalfBuffers[ mNumberOfBuffersQueued - 1 ] = recycledBuffer;
    
      mNumberOfBuffersQueued--;
      if( mNumberOfBuffersQueued ) bciout << "buffers remaining in queue: " << mNumberOfBuffersQueued << endl;
    }
    else bcierr << "buffer is NULL" << endl;
    lock.unlock();
  } // end mutex critical section
  else
  {
    bcierr << "timed out waiting for the DAQmx callback to post data after " << timeoutInMilliseconds << "ms" << endl;
  }
}

// **************************************************************************
// Function:   Halt
// Purpose:    This routine shuts down data acquisition board
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void NIDAQmxADC::Halt()
{
   Stop();
}
// GenericADC functions up to here


// **************************************************************************
// Function:   Callback
// Purpose:    This function is called (by the device driver) every time a
//             buffer of mSamplesPerBlock * mNumberOfChannels is ready. In this case the function
//             GetData should not report an error (the halfbuffer for each
//             channel is ready to be read)
//             cur_adc is a pointer to the current instantiation of NIDAQmxADC;
//             The callback code is adapted from CallBack in the NIADC module
//             by Gerwin Schalk
// Parameters: see NIDAQmx documentation
// Returns:    DAQmxSuccess (0) ... no error
// **************************************************************************
long int NIDAQmxADC::Callback( TaskHandle mTaskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData )
{
  NIDAQmxADC * pointerToADC = ( NIDAQmxADC * )callbackData;
  if( pointerToADC )
  {
    try { pointerToADC->GetData(); } // we don't want a callback function to throw an exception
    catch( std::exception const & exc ) { bcierr <<  "Callback() threw a std::exception: " << exc.what() << endl; }
    catch( ... ) { bcierr << "Callback() threw an exception of unknown type" << endl; }
  }
  else { bcierr << "Callback() received NULL pointer to ADC" << endl; }
  return DAQmxSuccess; // always? well, OK...
}


// **************************************************************************
// Function:   GetData
// Purpose:    This function is called by the callback function and copies
//             the data buffer into our own buffer
// Parameters: N/A
// Returns:    error (contains DAQmx error code)
// Notes: The buffer type and the DAQmx function used to read data depends
//        on the SignalType::int16 statement in the Preflight function
// **************************************************************************
int NIDAQmxADC::GetData()
{  
  { // start mutex critical section
    std::lock_guard< std::mutex > lock( mDataMutex );
    int32 samplesDeliveredPerChannel = 0;
   
    if( mNumberOfBuffersQueued >= NIDAQ_MAX_BUFFERS )
    {
      bcierr << "buffer queue overflowed" << endl; // do not add a buffer if we ran out of buffers
    }
    else
    {
      CHECK_DAQMX_FAILED( DAQmxReadBinaryI16(
        mTaskHandle,
        mSamplesPerBlock,
        mSecondsPerBlock * 5.0, // timeout in seconds
        DAQmx_Val_GroupByChannel,
        mHalfBuffers[ mNumberOfBuffersQueued ],
        mNumberOfChannels * mSamplesPerBlock,
        &samplesDeliveredPerChannel,
        NULL
      ) );
      if( samplesDeliveredPerChannel == mSamplesPerBlock ) mNumberOfBuffersQueued++;
      else bcierr << "DAQmx returned an incomplete sample block (expected " << mSamplesPerBlock << ", got " << samplesDeliveredPerChannel << ")" << endl;
    }
  } // end mutex critical section
  mConditionVariable.notify_one();
  return mResultCode;
}


// **************************************************************************
// Function:   Start
// Purpose:    Configure the board using the provided parameters and start the task
// Parameters: N/A
// Returns:    error (contains DAQmx error code)
// **************************************************************************
int NIDAQmxADC::Start()
{
  // Initialize all the class variables
  mResultCode = 0;

  // Temporary variables needed in this function
  const char taskName[] = "myTask";
  char chanName[ 100 ];
  
  // Variables not needed but useful to change channel properties
  double max = 5.0, min = -5.0;
  long int terminalConfig = DAQmx_Val_RSE; // setting this to RSE ensures compatibility with other devices
  // Our hardware can be set either Not Referenced Single Ended or Differential
  /* Choises are DAQmx_Val_RSE           Referenced single-ended mode
                 DAQmx_Val_NRSE          Nonreferenced single-ended mode
                 DAQmx_Val_Diff          Differential mode
                 DAQmx_Val_PseudoDiff    Pseudodifferential mode
  See DAQmx Help */

  // total number of samples to acquire in buffer
  // since we have a half-buffer, we'll get have that
  // in one double buffer operation
  uInt32 ulCount = mSamplesPerBlock * mNumberOfChannels * 2;

  // This Reset the device
  RETURN_IF_FAILED( DAQmxResetDevice( mDeviceName ) );
  // create the task (assign  value to mTaskHandle)
  RETURN_IF_FAILED( DAQmxCreateTask( taskName, &mTaskHandle ) );

  // create the channels according to the setting above
  for( int iChannel = 0; iChannel < mNumberOfChannels; iChannel++ )
  {
    sprintf( chanName, "%s/ai%i", mDeviceName, iChannel );
    RETURN_IF_FAILED( DAQmxCreateAIVoltageChan(
      mTaskHandle,
      chanName,
      "",
      terminalConfig,
      min,
      max,
      DAQmx_Val_Volts,
      NULL
    ) );
  }

  // configure the device sampling rate
  // Note Hardware buffer MUST be a power of 2 for the next call to work
  // properly. Considering a max sample rate of 10000 Hz, next higher
  // power of 2 is 16384
  RETURN_IF_FAILED( DAQmxCfgSampClkTiming(
    mTaskHandle,
    "",
    mSamplesPerSecond,
    DAQmx_Val_Rising,
    DAQmx_Val_ContSamps,
    16384
  ) );

  // configure the Event that activate our Callback function
  RETURN_IF_FAILED( DAQmxRegisterEveryNSamplesEvent(
    mTaskHandle,
    DAQmx_Val_Acquired_Into_Buffer,
    ulCount / ( 2 * mNumberOfChannels ),
    0,
    ( DAQmxEveryNSamplesEventCallbackPtr )&Callback,
    this
  ) );
  
  /*
  // Here can take place the table for gain and offset
  // define the scanning and gain table
  int16 chanVector[NIDAQ_MAX_CHANNELS];
  int16 gainVector[NIDAQ_MAX_CHANNELS];
  for( int iChannel = 0; iChannel < NIDAQ_MAX_CHANNELS; iChannel++ )
  {
    chanVector[ iChannel ] = iChannel;
    gainVector[ iChannel ] = 1;
  }
  SCAN_Setup( mDeviceNumber, mNumberOfChannels, chanVector, gainVector ); // ??
  */
  RETURN_IF_FAILED( DAQmxStartTask( mTaskHandle ) );

  return mResultCode;
}

// **************************************************************************
// Function:   Stop
// Purpose:    Stops the board
// Parameters: N/A
// Returns:    error (contains DAQmx error code)
// **************************************************************************
int NIDAQmxADC::Stop()
{
  // if a Task is currently running, stop the Task and releases any resources
  if( mTaskHandle )
    RETURN_IF_FAILED( DAQmxClearTask( mTaskHandle ) );
  mTaskHandle = 0;

  // this function may be called by Halt when mDeviceNumber and mDeviceName are not initialized
  // in that case DAQmxResetDevice causes an error
  if( mDeviceNumber > 0 )
    RETURN_IF_FAILED( DAQmxResetDevice( mDeviceName ) );

  return mResultCode;
}
