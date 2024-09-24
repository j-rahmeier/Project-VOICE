/////////////////////////////////////////////////////////////////////////////
// $Id: SerialInterface.cpp 7618 2023-09-25 05:17:08Z jhill $
// Authors: jezhill@gmail.com
// Description: An extension that allows flexible communication to and from
// serial-port devices such as programmable microcontrollers (e.g. Arduino,
// Teensy, Pico, etc.)
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
/////////////////////////////////////////////////////////////////////////////
#include "SerialInterface.h"
#include "BCIEvent.h"
#include "ProcessUtils.h"

#include <stdio.h>
#include <iomanip>

#define REPORT(X)  #X<<"="<<(X)<<"; "
#define REPORTS(X) #X<<"="<<EncodeStringLiteral(X)<<"; "
#define EXPRESSION_IS_EMPTY( E )  ( E.AsString().length() == 0 )

#ifdef DBSTREAM
#	define DB(X)        DBSTREAM<<X
#	define DBREPORT(X)  DBSTREAM<<REPORT(X)
#	define DBREPORTS(X) DBSTREAM<<REPORTS(X)
#else
#	define DB(X)       
#	define DBREPORT(X) 
#	define DBREPORTS(X)
#endif

#ifdef WIDGET_ADC // WIDGET_ADC will be left undefined if we are compiling this as an Extension (Logger) for another source module, but it will be defined if we are compiling the SerialWidget source module
	//RegisterFilter( SerialInterface, 1 );
	// Uncommenting the line above causes a linker error *even when* WIDGET_ADC is not defined
	// Under that circumstance, this code should not be seen by the compiler at all!
	// Is something somewhere grepping this line out of the file and using it??
#else
	Extension( SerialInterface );
#endif

SerialInterface::SerialInterface() :
	mBlocked( false ),
	mEnabled( false ),
	mGood( true ),
	mRecordEvents( false ),
	mMonitorThreadIsRunning( false ),
	mKillThreads( false ),
	mThreadMessage( DO_NOTHING ),
	mTimeSlice( Time::Seconds( 0.001 ) ),
#	ifdef WIDGET_ADC
		mBlockReceived( true ),
#	endif
	mThreadLock( true )
{
#	define SEMAPHORE_NAME "BCI2000SerialInterface"
#	ifdef WIDGET_ADC
		mSignalBuffer = NULL;
	
//		ProcessUtils::AssertSingleInstance( 0, NULL, SEMAPHORE_NAME, 0 ); // TODO: this fails to communicate, so we'll do it by hand in Windows-only fashion:
		HANDLE semaphore = ::CreateSemaphore( NULL, 1, 2, TEXT( SEMAPHORE_NAME ) );
		DWORD result = ::WaitForSingleObject( semaphore, 0 );
#	endif
}

SerialInterface::~SerialInterface()
{
	mKillThreads = true;
	PassMessageToMonitorThread( EXIT_THREAD );
	if (mMonitorThread.joinable())
		mMonitorThread.join();	
	mPort.Close();
}

void
SerialInterface::PassMessageToMonitorThread( int messageValue )
{
	mThreadMessage = messageValue;
	mThreadLock.Set();
}

void
SerialInterface::PassMessageToMonitorThread( const std::string & serialCommand )
{
	mThreadMessage = SEND_COMMAND;
	mPendingCommand = serialCommand;
	mThreadLock.Set();
}

int
SerialInterface::SendCommand( const std::string & serialCommand, const std::string & description )
{
	bciout << "Sending " << description << ( description.length() ? " " : "" ) << EncodeStringLiteral( serialCommand, "\"" );
	return ( int )mPort.Send( serialCommand );
}

#define TRY( X )   try{ X } catch( std::string const & error ) { bcierr << error; }
void
SerialInterface::Publish( void )
{
#	ifdef WIDGET_ADC
		// In the ADC, the semaphore was already created & acquired in the constructor.
#	else
		// By contrast. the Extension tries to acquire the semaphore later, here in Publish(), and it will
		// fail if the ADC has already been constructed, in which case the Extension will be blocked.
	
		// mBlocked = !ProcessUtils::AssertSingleInstance( 0, NULL, SEMAPHORE_NAME, 0 ); // TODO: this fails to communicate, so we'll do it by hand in Windows-only fashion:
		HANDLE semaphore = ::CreateSemaphore( NULL, 1, 2, TEXT( SEMAPHORE_NAME ) );
		if( !semaphore ) { bcierr << "CreateSemaphore() failed"; return; }
		DWORD result = ::WaitForSingleObject( semaphore, 0 );
		if( result == WAIT_FAILED ) { bcierr << "WaitForSingleObject() failed"; return; }
		mBlocked = ( result == WAIT_TIMEOUT );
		
		if( mBlocked ) bciout << "Extension blocked, because an ADC incorporating the same functionality will be using the same serial device and BCI2000 parameters.";
#	endif
	
	std::string portSpec = OptionalParameter( "SerialPort" );
#	ifdef WIDGET_ADC
		BEGIN_PARAMETER_DEFINITIONS
		
			"Source:Signal%20Properties int       SamplingRate=     1000Hz 1000Hz 0 % // the signal sampling rate",
			"Source:Signal%20Properties int       SampleBlockSize=    40     40   1 % // number of samples per block",
			"Source:Signal%20Properties int       SourceCh=         auto      1   1 % // number of digitized channels total",
			"Source:Signal%20Properties floatlist SourceChGain=   1 auto      %   % % // conversion factors from AD units into microvolts",
			"Source:Signal%20Properties floatlist SourceChOffset= 1 auto      %   % % // AD offsets for recorded channels",
			"Source:Signal%20Properties list      ChannelNames=   1 auto      %   % % // channel names",
			// TODO: are the last two/three needed? or are they handled by DataIOFilter?
		
			"Source:Signal%20Source     intlist   SourceChPins=   1    0      0   % % // microcontroller pin index corresponding to each source channel",
		
		END_PARAMETER_DEFINITIONS
		// In ADC mode, do NOT abort if SerialPort is empty - fall through to define all the parameters regardless.
#	else
		if( mBlocked || !portSpec.length() ) return;
#	endif
	
	BEGIN_PARAMETER_DEFINITIONS
		"Source:Serial%20Port string SerialPort=          %    %  % % // serial port address (optionally, append colon followed by comma-separated MODE options like dtr=on etc.)(noedit)",
		"Source:Serial%20Port string PublishCommand=      %    %  % % // if defined, send these bytes to the serial device and expect newline-delimited event definitions, followed by a blank line, in return  (escape strings \\t \\r \\n \\0 \\\\ and \\xNN will be decoded)(noedit)",
		"Source:Serial%20Port string StartCommand=        %    %  % % // if defined, send these bytes to the serial device to indicate it is OK to send event information (escape strings \\t \\r \\n \\0 \\\\ and \\xNN will be decoded)",
		"Source:Serial%20Port string StopCommand=         %    %  % % // if defined, send these bytes to the serial device to indicate that event information should no longer be sent (escape strings \\t \\r \\n \\0 \\\\ and \\xNN will be decoded)",
		"Source:Serial%20Port matrix SerialOutputs= 1 { When%20this%20Expression%20becomes%20true, send%20these%20bytes: } % %    % % % // table of contingences and byte strings (escape strings \\t \\r \\n \\0 \\\\ and \\xNN will be decoded)",
		"Source:Serial%20Port int    ElseIf=              0    0  0 1 // how to process the rows of SerialOutputs: 0: process all rows, 1: stop at the first match (enumeration)",
	END_PARAMETER_DEFINITIONS
	
	mStartCommand = DecodeStringLiteral( OptionalParameter( "StartCommand" ) );
	mStopCommand  = DecodeStringLiteral( OptionalParameter( "StopCommand"  ) );
	std::string publishCommand = DecodeStringLiteral( OptionalParameter( "PublishCommand" ) );
	if( !portSpec.length() )
	{
		if( publishCommand.length() ) bcierr << "cannot set --PublishCommand on the command-line without also setting --SerialPort";
		return;
	}
	std::vector< std::string > definitions;
	TRY(
		bciout << "Connecting to " << portSpec;
		mPort.Open( portSpec );
		if( mStopCommand.length() ) SendCommand( mStopCommand, "stop command" );
		if( publishCommand.length() )
		{
			SendCommand( publishCommand, "publish command" );
			bciout << "Waiting for replies followed by a blank line";
			std::string definition;
			while( true )
			{
				int64_t nBytes = mPort.ReceiveUntil( definition, '\n' );
				if( nBytes >= 2 && definition[ nBytes - 2 ] == '\r' ) definition.resize( nBytes - 2 ); // remove possible trailing CRLF
				else if( nBytes ) definition.resize( nBytes - 1 ); // or maybe just trailing LF
				if( !definition.length() ) break;
				bciout << "Received " << ( definition.find( '=' ) == std::string::npos ? "event" : "parameter" ) << " definition " << EncodeStringLiteral( definition, "\"" );
				definitions.push_back( definition );
			}
		}
	)
	for( int i = 0; i < definitions.size(); i++ )
	{
		std::string & definition = definitions[ i ];
		if( definition.find( '=' ) != std::string::npos )
		{
			std::stringstream ss( definition );
			std::string section, type, name;
			ss >> section >> type >> name;
			name.resize( name.length() - 1 ); // removes trailing `=` from parameter name
			size_t positionOfLastComment = definition.rfind( "//" );
			size_t positionOfReadOnlyFlag = definition.rfind( "(readonly)" );
			size_t positionOfNoeditFlag = definition.rfind( "(noedit)" );
			if( positionOfLastComment != std::string::npos && positionOfReadOnlyFlag != std::string::npos && positionOfReadOnlyFlag > positionOfLastComment )
				{ if( positionOfNoeditFlag == std::string::npos || positionOfNoeditFlag < positionOfLastComment ) definition += "(noedit)"; }
			else if( type == "int" || type == "float" ) mNumericParameters.push_back( name );
			else if( type == "string" ) mStringParameters.push_back( name );
			else bcierr << "parameter definitions received via the serial port (if not readonly) must be of type int, float or string, whereas " << name << " was defined as " << type;
			BEGIN_PARAMETER_DEFINITIONS
				definition.c_str(),
			END_PARAMETER_DEFINITIONS
		}
		else
		{
			BEGIN_EVENT_DEFINITIONS
				definition.c_str(),
			END_EVENT_DEFINITIONS
		}
	}
}

void
SerialInterface::ParseOutputs( ConditionVector & conditions ) const
{
	ParamRef param = Parameter( "SerialOutputs" );
	int expectedNumberOfColumns = 2;
	if( param->NumColumns() != expectedNumberOfColumns && !( param->NumColumns() == 0 && param->NumRows() == 0 ) )
		{ bcierr << param->Name() << " parameter must have " << expectedNumberOfColumns << " columns"; return; }
	conditions.clear();
	for( int iRow = 0; iRow < param->NumRows(); iRow++ )
	{
		std::string expression = param( iRow, 0 );
		std::string byteString = DecodeStringLiteral( param( iRow, 1 ) );
		if( expression.length() ) conditions.emplace_back( expression, byteString ); // SerialInterface::Condition() constructor args
		// it's OK to have an empty byte string on a particular row if the expression is meaningful and ElseIf mode is set - could translate as  if(row1BecameTrue) SendNothing(); else if(row2BecameTrue) SendSomething(); ...
	}
}

#ifdef WIDGET_ADC
	void SerialInterface::AutoConfig( const SignalProperties & props )
	{
		ParamRef
			SourceChPins   = Parameter( "SourceChPins" );
		MutableParamRef
			SourceCh       = Parameter( "SourceCh" ),
			SourceChGain   = Parameter( "SourceChGain" ),
		    SourceChOffset = Parameter( "SourceChOffset" ),
			ChannelNames   = Parameter( "ChannelNames" );
		int nChannels = SourceChPins->NumValues();
		SourceCh = nChannels;
		SourceChGain->SetNumValues( nChannels );
		SourceChOffset->SetNumValues( nChannels );
		ChannelNames->SetNumValues( nChannels );
		for( int i = 0; i < nChannels; i++ )
		{
			SourceChGain( i ) = 1;
			SourceChOffset( i ) = 0;
			std::stringstream ss;
			ss << "Pin" << ( int )SourceChPins( i );
			ChannelNames( i ) = ss.str();
		}
	}
#endif

#ifdef WIDGET_ADC
	void SerialInterface::Preflight( const SignalProperties & inputProperties, SignalProperties & outputProperties ) const
#else
	void SerialInterface::Preflight() const
#endif
{
	std::string portSpec = OptionalParameter( "SerialPort" );
#	ifdef WIDGET_ADC
		if( !portSpec.length() ) bcierr << "SerialPort parameter cannot be empty";
#	endif
	if( mBlocked || !portSpec.length() ) return;
	DecodeStringLiteral( Parameter( "StartCommand" ) );
	DecodeStringLiteral( Parameter( "StopCommand"  ) );

	ConditionVector conditions;
	ParseOutputs( conditions );
	Parameter( "ElseIf" );
	for( int i = 0; i < mNumericParameters.size(); i++ ) Parameter( mNumericParameters[ i ] );
	for( int i = 0; i < mStringParameters.size();  i++ ) Parameter( mStringParameters[  i ] );
#	ifdef WIDGET_ADC
		double samplesPerSecond = Parameter( "SamplingRate" ).InHertz();
		int samplesPerBlock = Parameter( "SampleBlockSize" );
		int numberOfChannels = Parameter( "SourceCh" );
		if( Parameter( "SourceChPins" )->NumValues()   != numberOfChannels ) bcierr << "number of elements in SourceChPins must be "   << numberOfChannels << " as specified by SourceCh";
		if( Parameter( "ChannelNames" )->NumValues()   != numberOfChannels ) bcierr << "number of elements in ChannelNames must be "   << numberOfChannels << " as specified by SourceCh";
		if( Parameter( "SourceChGain" )->NumValues()   != numberOfChannels ) bcierr << "number of elements in SourceChGain must be "   << numberOfChannels << " as specified by SourceCh";
		if( Parameter( "SourceChOffset" )->NumValues() != numberOfChannels ) bcierr << "number of elements in SourceChOffset must be " << numberOfChannels << " as specified by SourceCh";
		// TODO: are the last two/three needed? or are they handled by DataIOFilter?
		outputProperties.SetType( SignalType::float32 ); // it will be assumed that the widget delivers signals in this format; in principle if we changed this line to match the sketch's behavior, the rest of this file should automatically handle float32, int32 or int16 (but not float24)
		outputProperties.SetChannels( numberOfChannels );
		outputProperties.SetElements( samplesPerBlock );
		for( int i = 0; i < numberOfChannels; i++ )
		{
			outputProperties.ChannelLabels()[ i ] = Parameter( "ChannelNames" )( i );
			for( int j = i + 1; j < numberOfChannels; j++ )
				if( ( int )Parameter( "SourceChPins" )( i ) == ( int )Parameter( "SourceChPins" )( j ) )
					bcierr << "Pin number " << ( int )Parameter( "SourceChPins" )( i ) << " is repeated in SourceChPins";
		}

#	endif
}

#ifdef WIDGET_ADC
	void SerialInterface::Initialize( const SignalProperties & inputProperties, const SignalProperties & outputProperties )
#else
	void SerialInterface::Initialize()
#endif
{
	mGood = true;
	mPortSpec = OptionalParameter( "SerialPort" );
	mEnabled = mPortSpec.length() > 0 && !mBlocked; // mEnabled will always be true from this point on if we're in WIDGET_ADC mode, because in that mode an empty SerialPort parameter will have already caused Preflight() to fail
	if( !mEnabled ) return;
	mStartCommand = DecodeStringLiteral( Parameter( "StartCommand" ) );
	mStopCommand  = DecodeStringLiteral( Parameter( "StopCommand"  ) );
		
	if( !mMonitorThreadIsRunning )
	{
		mMonitorThread = std::thread( &SerialInterface::MonitorThread, this );
	}
	ParseOutputs( mConditions );
	mElseIf = Parameter( "ElseIf" );
	
	std::string parameterCommand;
	TRY(
		mThreadMessage = DO_NOTHING;
		mPendingCommand = "";
		if( mPort.IsOpen() && mStopCommand.length() ) SendCommand( mStopCommand, "stop command" );
		mPort.Close();
		mPort.Open( mPortSpec ); // NB: in principle the port may have changed, but we are not going to ask it to publish again
		if( mStopCommand.length() ) SendCommand( mStopCommand, "stop command" );
		for( int i = 0; i < mNumericParameters.size(); i++ )
		{
			parameterCommand = mNumericParameters[ i ] + "=" + Parameter( mNumericParameters[ i ] ) + "\n";
			SendCommand( parameterCommand );
		}
		for( int i = 0; i < mStringParameters.size();  i++ )
		{
			parameterCommand = mStringParameters[ i ] + "=" + EncodeStringLiteral( Parameter( mStringParameters[ i ] ), "\"" ) + "\n";
			SendCommand( parameterCommand );
		}
	)
#	ifdef WIDGET_ADC
		mIncoming.SetProperties( outputProperties );
		mSignalBuffer = new char[ mIncoming.Elements() * mIncoming.Channels() * mIncoming.Type().Size() ];
#		define BUILD_STRING( VARNAME, TERMS ) { std::stringstream _ss; _ss << TERMS; VARNAME = _ss.str(); }
		TRY (
			BUILD_STRING( parameterCommand, "samplesPerSecond=" << ( double )Parameter( "SamplingRate" ).InHertz() << "\n" );
			SendCommand( parameterCommand );
			BUILD_STRING( parameterCommand, "samplesPerBlock="  << ( int )Parameter( "SampleBlockSize" ) << "\n" );
			SendCommand( parameterCommand );
			std::stringstream ss;
			ss << "sourcePins=\"";
			for( int i = 0; i < Parameter( "SourceChPins" )->NumValues(); i++ ) ss << ( i ? " " : "" ) << ( int )Parameter( "SourceChPins" )( i );
			ss << "\"\n";
			parameterCommand = ss.str();
			SendCommand( parameterCommand );
			::Sleep( 250 );
		)
		if( mGood && mStartCommand.length() ) SendCommand( mStartCommand, "start command" );
#	endif
}

void
SerialInterface::Halt( void )
{
	if( !mEnabled ) return;
#	ifdef WIDGET_ADC
		delete [] mSignalBuffer;
		mSignalBuffer = NULL;
	
		if( mStopCommand.length() ) SendCommand( mStopCommand, "stop command" );
#	else
		// NB: in the Extension, flow doesn't ever seem to reach here before a regular Preflight/Initialize, only on error - do not rely on this method for regular cleanup
		mPort.Close();
#	endif
}

void
SerialInterface::StartRun( void )
{
	if( !mEnabled ) return;
#	ifdef WIDGET_ADC
		// StartCommand is used at Initialize() time rather than StartRun()
#	else
		if( mStartCommand.length() ) SendCommand( mStartCommand, "start command" );
#	endif
	mRecordEvents = true;
}

void
SerialInterface::StopRun( void )
{
	if( !mEnabled ) return;
#	ifdef WIDGET_ADC
		// StopCommand is used at Halt() time rather than StopRun()
#	else
		if( mStopCommand.length() ) SendCommand( mStopCommand, "stop command" );
#	endif
	mRecordEvents = false;
}

#ifdef WIDGET_ADC
	void SerialInterface::Process( const GenericSignal & inputSignal, GenericSignal & outputSignal )
#else
	void SerialInterface::Process()
#endif
{
	if( !mEnabled ) return;
	bool sending = false;
	for( ConditionVector::iterator itCondition = mConditions.begin(); itCondition != mConditions.end(); itCondition++ )
	{
		sending |= itCondition->Evaluate();
		if( sending && mElseIf ) break;
	}
	if( sending ) PassMessageToMonitorThread( SEND_TRIGGERS );
#	ifdef WIDGET_ADC
		WithThreadPriority( ThreadUtils::Priority::Maximum - 1 ) mBlockReceived.Wait();
		outputSignal = mIncoming;
#	endif
}

void
SerialInterface::MonitorThread()
{	
	mMonitorThreadIsRunning = true;
	bool keepGoing = true;
	std::string line;
	static char incomingMessage[ 1024 ];
	const std::string littleEndianMarker( "\x01\x00", 2 );
	const std::string    bigEndianMarker( "\x00\x01", 2 );
	while( keepGoing && !mKillThreads )
	{
		bool gotMessage;
		WithThreadPriority( ThreadUtils::Priority::Maximum - 1 )
			gotMessage = mThreadLock.Wait( Time::Seconds( 0.001 ) );
		
		if( gotMessage )
		{
			switch( mThreadMessage )
			{
				case SEND_COMMAND:
					if( mPendingCommand.length() )
					{
						SendCommand( mPendingCommand );
						mPendingCommand = "";
					}
					break;
				case SEND_TRIGGERS:
					for( ConditionVector::iterator itCondition = mConditions.begin(); itCondition != mConditions.end(); itCondition++ )
						itCondition->Send( mPort );
					break;
				
				case EXIT_THREAD:
					keepGoing = false;
					break;
			} // end of switch( mThreadMessage )
			mThreadMessage = DO_NOTHING; // message handled, so put the flag down
			continue; // next iteration of while(keepGoing) loop
		}
		
		// At this point, we must have timed out while waiting for a message from Process().
		// So, let's see if the widget has sent us anything.
		
		// TODO: do we need to start and stop the thread, or otherwise prevent the following call from happening while a
		//       second/subsequent Initialize() might concurrently be disconnecting/reconnecting/sending parameter values?
		int64_t nBytesReceived = mPort.ReceiveAvailable( incomingMessage, sizeof( incomingMessage ) );
		for( int64_t iMessageByte = 0; iMessageByte < nBytesReceived; iMessageByte++ )
		{
			if( incomingMessage[ iMessageByte ] != '\n' )
			{
				line += incomingMessage[ iMessageByte ];
				continue; // next iMessageByte
			}
			// Now we know we have received a line terminated by \n (possibly \r\n)
			if( line.length() && line[ line.length() - 1 ] == '\r' ) line.resize( line.length() - 1 );
			if( !line.length() ) continue;
			// Now we have a non-empty line, stripped of its line ending
			
#			ifdef WIDGET_ADC
				if( line == littleEndianMarker || line == bigEndianMarker )
				{
					uint16_t endianMarker = *( uint16_t * )line.c_str();
					bool endianSwapNeeded = ( endianMarker != 1 );
					size_t bytesPerSample = mIncoming.Type().Size();
					size_t nBytesNeeded = mIncoming.Elements() * mIncoming.Channels() * bytesPerSample;
					char * cursor = mSignalBuffer;
					// we expect nBytesNeeded bytes of binary data to immediately follow the "@" line
					// but some of this may already have arrived from the ReceiveAvailable() call, so
					// let's use it up first...
					iMessageByte++; // first, get past the '\n' currently under consideration in the for loop
					while( iMessageByte < nBytesReceived && nBytesNeeded ) { *cursor++ = incomingMessage[ iMessageByte++ ]; nBytesNeeded--; }
					// ...and if that wasn't enough, synchronously read the rest
					int64_t nExtraBytesReceived = nBytesNeeded ? mPort.Receive( cursor, nBytesNeeded ) : 0;
					if( nExtraBytesReceived == nBytesNeeded )
					{
						cursor = mSignalBuffer;
						switch( mIncoming.Type() )
						{
							case SignalType::float32:
								for( int iSample = 0; iSample < mIncoming.Elements(); iSample++ )
									for( int iChannel = 0; iChannel < mIncoming.Channels(); iChannel++, cursor += bytesPerSample )
									{
										if( endianSwapNeeded ) { char tmp = cursor[ 0 ]; cursor[ 0 ] = cursor[ 3 ]; cursor[ 3 ] = tmp; tmp = cursor[ 1 ]; cursor[ 1 ] = cursor[ 2 ]; cursor[ 2 ] = tmp; }
										mIncoming( iChannel, iSample ) = *( float * )cursor;
									}
								break;
								
							case SignalType::int32:
								for( int iSample = 0; iSample < mIncoming.Elements(); iSample++ )
									for( int iChannel = 0; iChannel < mIncoming.Channels(); iChannel++, cursor += bytesPerSample )
									{
										if( endianSwapNeeded ) { char tmp = cursor[ 0 ]; cursor[ 0 ] = cursor[ 3 ]; cursor[ 3 ] = tmp; tmp = cursor[ 1 ]; cursor[ 1 ] = cursor[ 2 ]; cursor[ 2 ] = tmp; }
										mIncoming( iChannel, iSample ) = *( int32_t * )cursor;
									}
								break;
								
							case SignalType::int16:
								for( int iSample = 0; iSample < mIncoming.Elements(); iSample++ )
									for( int iChannel = 0; iChannel < mIncoming.Channels(); iChannel++, cursor += bytesPerSample )
									{
										if( endianSwapNeeded ) { char tmp = cursor[ 0 ]; cursor[ 0 ] = cursor[ 1 ]; cursor[ 1 ] = tmp; }
										mIncoming( iChannel, iSample ) = *( int16_t * )cursor;
									}
								break;
						}
						mBlockReceived.Set();
					}
					else { bcierr << "expected " << nBytesNeeded << " extra bytes, but got " << nExtraBytesReceived; }
					
					line = "";
					continue; // next iMessageByte
				}
#			endif
			
			if( line[ 0 ] == '{' ) // some JSON message
			{
				if( line.find( "_ERROR_" ) != std::string::npos ) // The Keyhole library delivers error messages like {"_KEYHOLE_ERROR_TYPE": "BadKey", "_KEYHOLE_ERROR_MSG": "failed to recognize command"}
				{
					size_t pos;
					std::string first  = "{\"_KEYHOLE_ERROR_TYPE\": \""; // if these are present, finesse the error message
					std::string second = "\", \"_KEYHOLE_ERROR_MSG\": "; // if not, no harm no foul
					if( ( pos = line.find( first  ) ) != std::string::npos ) line.replace( pos, first.length(), "The device at " + mPort.Name() + " issued a " );
					if( ( pos = line.find( second ) ) != std::string::npos ) { line.replace( pos, second.length(), " error: " ); line.resize( line.length() - 1 ); }
					
					mGood = false;
					bcierr << line; 
				}
				// otherwise, just ignore it
			}
			else // if it isn't JSON, assume it must be a BCI2000 event descriptor line
			{
				if( mRecordEvents ) bcievent << line;
				// otherwise (i.e. if suspended) just ignore it
			}
			line = ""; // line processed. start a new line
			
		} // next iMessageByte
		
	} // end of main while(keepGoing) loop
	mMonitorThreadIsRunning = false;
}


////////////////////////////////////////////////////////////////
SerialInterface::Condition::Condition( const std::string & triggerExpression, const std::string & outgoingMessage )
: mPreviousValue( false ), mTriggerExpression( triggerExpression ), mOutgoingMessage( outgoingMessage ), mBytesToSend( 0 )
{
	mTriggerExpression.Evaluate();
	mBytesToSend = 0;
}

SerialInterface::Condition::~Condition()
{
	
}

bool
SerialInterface::Condition::Evaluate( void )
{
	bool newValue = ( mTriggerExpression.Evaluate() != 0.0 );
	bool returnValue = newValue && !mPreviousValue;
	mPreviousValue = newValue;
	if( returnValue ) mBytesToSend = mOutgoingMessage.length();
	if( returnValue ) DB( "flagging " << this << " for send" );
	return returnValue;
}

int64_t
SerialInterface::Condition::Send( SerialConnection & port )
{
	if( !mBytesToSend ) return 0;
	DB( "Sending " << mBytesToSend << " bytes " << EncodeStringLiteral( mOutgoingMessage, "\"" ) << " from " << this );
	int64_t returnValue = port.Send( mOutgoingMessage.c_str(), mBytesToSend );
	if( returnValue > 0 ) mBytesToSend = 0;
	return returnValue;
}
////////////////////////////////////////////////////////////////
