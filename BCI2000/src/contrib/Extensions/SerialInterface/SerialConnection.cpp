// $Id: SerialConnection.cpp 7537 2023-08-19 03:52:08Z jhill $
// A configurable cross-platform serial-port class.
// 
// Copyright (c) 2023- Jeremy Hill
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materials provided with the
//    distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef   __INCLUDE_SERIALCONNECTION_CPP__
#define   __INCLUDE_SERIALCONNECTION_CPP__

#include "SerialConnection.h"
#include <string.h>
#include <sstream>
#include <chrono> // CPLUSPLUS11

#define STRINGSUBCLASS( x ) class x : public std::string { public: \
	x( std::string s ) { std::string & r = *this; r = s; } \
	x & operator=( std::string & s ) { std::string & r = *this; r = s; return *this; } }
#define RAISE( x ) { std::stringstream _ss; _ss << x; throw _ss.str(); }
#define RAISE_SUBCLASS( class, x ) { std::stringstream _ss; _ss << x; throw class( _ss.str() ); }

//#define DBSTREAM std::cerr
#ifdef DBSTREAM
#	include <iostream>
#	include <iomanip>
#	define DBREPORT( X )  DBSTREAM << #X << " = "   << X <<   "\n"
#	define DBREPORTQ( X ) DBSTREAM << #X << " = \"" << X << "\"\n"
#	define DBREPORTS( X ) DBREPORTQ( X )
#else
#	define DBREPORT( X )
#	define DBREPORTQ( X )
#	define DBREPORTS( X )
#endif

const double SerialConnection::WAIT_FOREVER = -1.0;
const double SerialConnection::NO_CHANGE    = -2.0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// timer

typedef std::chrono::time_point< std::chrono::steady_clock > TimerOrigin; // CPLUSPLUS11
TimerOrigin gTimerOrigin;
bool gTimerInitialized = false;

double
Seconds( TimerOrigin * origin )
{
	std::chrono::time_point< std::chrono::steady_clock > t = std::chrono::steady_clock::now(); // CPLUSPLUS11
	if( !gTimerInitialized ) { gTimerOrigin = t; gTimerInitialized = true; }
	std::chrono::duration< double > delta = origin ? t - *origin : t.time_since_epoch(); // CPLUSPLUS11
	return delta.count();
}

double
Seconds( bool relativeToFirstCall=true )
{
	return Seconds( relativeToFirstCall ? &gTimerOrigin : NULL );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Helper functions for encoding and decoding backslash-escaped string literals

std::string
EncodeStringLiteral( const std::string & s, const char * quote )
{
	return EncodeStringLiteral( s.c_str(), s.length(), quote );
}

std::string
EncodeStringLiteral( const char * s, const char * quote )
{
	return EncodeStringLiteral( s, s ? strlen( s ) : 0, quote );
}


std::string
EncodeStringLiteral( const char * s, size_t length, const char * quote )
{
	if( s == NULL ) return "NULL";
	std::stringstream ss;
	if( quote ) ss << quote;
	for( size_t i = 0; i < length; i++ )
	{
		char c = s[ i ];
		if(      c == '\0' ) ss << "\\0";
		else if( c == '\n' ) ss << "\\n";
		else if( c == '\r' ) ss << "\\r";
		else if( c == '\t' ) ss << "\\t";
		else if( c == '\\' ) ss << "\\\\";
		else if( quote && c == quote[ 0 ] ) ss << "\\" << c;
		else if( c < 32 || c > 127 )
		{
			// so tired of trying to make setw/setfill/etc work reliably, so:
			char hex[ 3 ]; sprintf( hex, "%02X", ( unsigned char )c ); // calm down, casting as unsigned char means it can never take up more than two chars, plus null terminator
			ss << "\\x" << hex;
		}
		else ss << c;
	}
	if( quote ) ss << quote;
	return ss.str();
}

std::string
DecodeStringLiteral( const std::string & s )
{
	std::string decoded;
	bool backslashed = false;
	int hexDigitsToExpect = 0;
	char hexChar = '\0';
	for( std::string::const_iterator it = s.begin(); it != s.end(); it++ )
	{
		char c = *it;
		bool escape = ( c == '\\' && !backslashed );
		if( hexDigitsToExpect == 2 )
		{
			if(      c >= 'A' && c <= 'F' ) { hexChar = c - 'A' + 10; hexDigitsToExpect = 1; continue; }
			else if( c >= 'a' && c <= 'f' ) { hexChar = c - 'a' + 10; hexDigitsToExpect = 1; continue; }
			else if( c >= '0' && c <= '9' ) { hexChar = c - '0'     ; hexDigitsToExpect = 1; continue; }
			else hexDigitsToExpect = 0;
		}
		if( hexDigitsToExpect == 1 )
		{
			if(      c >= 'A' && c <= 'F' ) { c = c - 'A' + 10 + hexChar * 16; }
			else if( c >= 'a' && c <= 'f' ) { c = c - 'a' + 10 + hexChar * 16; }
			else if( c >= '0' && c <= '9' ) { c = c - '0'      + hexChar * 16; }
			else decoded += hexChar; // it was a one-digit \xN only - we'll allow it, decode what we already got, and take the current character as literal
			hexDigitsToExpect = 0;
			hexChar  = 0;
		}
		if( backslashed )
		{ // we're only going to support the most common abbreviations \t\r\n\0 - anything else can use \xNN
			if(      c == 'n' ) c = '\n';
			else if( c == 'r' ) c = '\r';
			else if( c == 't' ) c = '\t';
			else if( c == '0' ) c = '\0';
			else if( c == 'x' ) { backslashed = false; hexDigitsToExpect = 2; continue; }
			else decoded += '\\';
			//  (\xNN is limited to two digits at a time only, like Python rather than C)
		}
		if( !escape ) decoded += c;
		backslashed = escape;
	}
	return decoded;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OS-specific Sleep implementation

#ifdef _WIN32
#	include <windows.h>
#else
#	include <sys/select.h>
#endif

void
SleepSeconds( double seconds ) // only used to make "fake" serial ports wait until their timeout without busy-waiting
{
	if( seconds < 0.0 ) return; // Windows represents milliseconds as a DWORD, i.e. unsigned 32-bit int, and you don't want to be sleeping for 50 days
	long msec = ( long )( 0.5 + seconds * 1000.0 );
#	if _WIN32 // Begin Windows implementation
		::Sleep( msec ); // NB: very often has poor granularity
#	else      // Begin Posix implementation
		struct timeval t;
		t.tv_sec = msec / 1000L;
		t.tv_usec = ( msec % 1000L ) * 1000L;
		select( 0, 0, 0, 0, &t );
#	endif     // End OS-specific implementations
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OS-specific serial port implementation

#ifdef _WIN32
#	include <windows.h>
#else
#	include <termios.h>
#	include <unistd.h>
#	include <fcntl.h>
#	include <errno.h>
#	include <sys/ioctl.h>
#	include <sys/stat.h>
#	include <sys/types.h>
#endif


SerialConnection::SerialConnection()
{
	Init();
}

SerialConnection::SerialConnection( std::string name )
{
	Init();
	Open( name );
}

SerialConnection::~SerialConnection()
{
	CleanUp();
}

std::string
SerialConnection::Name( void ) const
{
	return mName;
}
		
void
SerialConnection::SetFakeSource( std::deque<char> * fake, double t0 )
{
	mFake = fake;
	if( t0 ) mTimeZero = t0;
}

void
SerialConnection::FeedFakeData( const char * buffer, size_t nBytes )
{
	while( mFake && buffer && nBytes-- )
		mFake->push_back( *buffer++ );
}

bool
SerialConnection::IsFake( void ) const
{
	return mFake != NULL;
}

void
SerialConnection::Init( void )
{
	mHandle = 0;
	mTimeZero = 0.0;
	mFake = NULL;
	mTimeoutSeconds = WAIT_FOREVER;
#	ifdef _WIN32
#	else
		mpPreviousSettings = ( void * )new struct termios;
		mpCurrentSettings  = ( void * )new struct termios;
#	endif
}

void
SerialConnection::CleanUp( void )
{
	Close();
	mFake = NULL;
	mTimeZero = 0.0;
#	ifdef _WIN32
#	else
		delete ( struct termios * )mpPreviousSettings;
		mpPreviousSettings = NULL;
		delete ( struct termios * )mpCurrentSettings;
		mpCurrentSettings = NULL;
#	endif
}

double
SerialConnection::Elapsed( void ) const
{
	return Seconds() - mTimeZero;
}

void
SerialConnection::Open( std::string name )
{
	std::stringstream ss( name );
	std::getline( ss, name, ':' );
	std::string opts; // e.g. "baud=9600,data=8,parity=N,stop=1,dtr=on,rts=off"  (commas may also be spaces)
	while( true )
	{
		std::string opt;
		std::getline( ss, opt, ',' );
		if( ss.fail() ) break;
		if( opts.length() ) opts += " ";
		opts += opt;
	}
	// COMx[:][baud={19200}][parity={n|e|o|m|s}][data={5|6|7|8}][stop={1|1.5|2}][dtr={on|off|hs}][rts={on|off|hs|tg}]
	//        [to={on|off}][xon={on|off}][odsr={on|off}][octs={on|off}][idsr={on|off}]

	if( mHandle ) RAISE( "Serial port " << mName << " must be closed before it can be re-opened." );
	mName = name;
	if( !name.length() ) return;
	
	if( mFake )
	{
		if( !mTimeZero ) mTimeZero = Seconds();
		return;
	}
	
#	ifdef _WIN32 // Begin Windows implementation
		
		name = "\\\\.\\" + name;
		mHandle = CreateFileA( name.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ); // changed from 0 to FILE_ATTRIBUTE_NORMAL following manashmandal/SerialPort
		if( mHandle == INVALID_HANDLE_VALUE )
			{ mHandle = NULL; RAISE( "Failed to open COM port \"" << mName << "\"" ); }
		DCB previousSettings;
		if( !GetCommState( mHandle, &previousSettings ) )
			{ CloseHandle( mHandle ); mHandle = NULL; RAISE( "Failed to query settings for COM port \"" << mName << "\"" ); }	
		DCB settings = previousSettings;	
		if( !BuildCommDCBA( opts.c_str(), &settings ) )
			{ CloseHandle( mHandle ); mHandle = NULL; RAISE( "Failed to build struct for settings \"" << opts << "\"" ); }
#		ifdef DBSTREAM
#			define RSI( X )  ( DBSTREAM << " " << std::setw(20) << #X << ":  " <<                         previousSettings.X,  ( settings.X != previousSettings.X ) && DBSTREAM << " -> " <<                         settings.X, DBSTREAM << "\n" )
#			define RSC( X )  ( DBSTREAM << " " << std::setw(20) << #X << ":  " << ( int )( unsigned char )previousSettings.X,  ( settings.X != previousSettings.X ) && DBSTREAM << " -> " << ( int )( unsigned char )settings.X, DBSTREAM << "\n" )
			RSI(DCBlength);RSI(BaudRate);RSI(fBinary);RSI(fParity);RSI(fOutxCtsFlow);RSI(fOutxDsrFlow);RSI(fDtrControl);RSI(fDsrSensitivity);RSI(fTXContinueOnXoff);RSI(fOutX);RSI(fInX);RSI(fErrorChar);RSI(fNull);RSI(fRtsControl);RSI(fAbortOnError);RSI(fDummy2);RSI(wReserved);RSI(XonLim);RSI(XoffLim);RSC(ByteSize);RSC(Parity);RSC(StopBits);RSC(XonChar);RSC(XoffChar);RSC(ErrorChar);RSC(EofChar);RSC(EvtChar);RSI(wReserved1);
#		endif
		if( !SetCommState( mHandle, &settings ) )
			{ CloseHandle( mHandle ); mHandle = NULL; RAISE( "Failed to update settings for COM port \"" << mName << "\"" ); }
		if( !PurgeComm( mHandle, PURGE_RXCLEAR | PURGE_RXCLEAR | PURGE_TXCLEAR ) )
			{ CloseHandle( mHandle ); mHandle = NULL; RAISE( "Failed to purge COM port \"" << mName << "\"" ); }
		double timeout = mTimeoutSeconds;
		if( timeout == NO_CHANGE ) timeout = WAIT_FOREVER;
		mTimeoutSeconds = NO_CHANGE; // illegal value, just ensures that SetTimeout will indeed change it when called
		if( !SetTimeout( timeout ) )
			{ CloseHandle( mHandle ); mHandle = NULL; RAISE( "Failed to update time-out settings for COM port \"" << mName << "\"" ); }
		
#	else      // Begin Posix implementation

		std::stringstream parser( opts );
		std::string optionKey, optionValue;
		bool dtr = false, rts = false;
		tcflag_t baud = B9600;
		tcflag_t bitsPerByte = CS8;
		tcflag_t parityFlag = 0; // means even parity
		tcflag_t stopFlag = 0;   // means 1 stop bit
		while( true )
		{
			parser >> optionKey;
			if( parser.fail() ) break;
			size_t equalPos = optionKey.find( '=' );
			if( equalPos == std::string::npos || equalPos == 0 || equalPos == optionKey.length() - 1 )
				RAISE( "could not interpret option \"" << optionKey << "\"" );
			optionValue = optionKey.substr( equalPos + 1 );
			optionKey.resize( equalPos );
			bool knownOpt = false;
#			define OPTVAL( OPTKEY, OPTVALUE, ASSIGNMENT ) \
				if( optionKey == OPTKEY ) { knownOpt = true; if( optionValue == OPTVALUE ) { ASSIGNMENT; continue; } }
			// COMx[:][baud={19200}][parity={n|e|o|m|s}][data={5|6|7|8}][stop={1|1.5|2}][dtr={on|off|hs}][rts={on|off|hs|tg}]
			//        [to={on|off}][xon={on|off}][odsr={on|off}][octs={on|off}][idsr={on|off}]
			const bool boolOpt_on = true, boolOpt_off = false;
			OPTVAL( "dtr",   "on", dtr=true );        OPTVAL( "dtr",  "off", dtr=false );
			OPTVAL( "rts",   "on", dtr=true );        OPTVAL( "rts",  "off", rts=false );
			OPTVAL( "data",   "8", bitsPerByte=CS8 ); OPTVAL( "data",   "7", bitsPerByte=CS7 ); OPTVAL( "data",   "6", bitsPerByte=CS6 );   OPTVAL( "data",   "5", bitsPerByte=CS5 );
			OPTVAL( "parity", "n", parityFlag=0 );    OPTVAL( "parity", "e", parityFlag=0 );    OPTVAL( "parity", "o", parityFlag=PARODD ); 
			OPTVAL( "stop",   "1", stopFlag=0 );      OPTVAL( "stop",   "2", stopFlag=CSTOPB ); 
#			define BAUDOPTVAL( HUMAN_READABLE_VALUE, FLAG ) OPTVAL( "baud", #HUMAN_READABLE_VALUE, baud=FLAG )
			// we're going to use the octal literals for the baud-rate flags because some library versions will have headers that define all of these rates and some will not (e.g. the macro B921600 is not defined everywhere, but the value 0010007 will still work)
			BAUDOPTVAL(      50, 0000001 ); BAUDOPTVAL(      75, 0000002 ); BAUDOPTVAL(     110, 0000003 ); BAUDOPTVAL(     134, 0000004 );
			BAUDOPTVAL(     150, 0000005 ); BAUDOPTVAL(     200, 0000006 ); BAUDOPTVAL(     300, 0000007 ); BAUDOPTVAL(     600, 0000010 );
			BAUDOPTVAL(    1200, 0000011 ); BAUDOPTVAL(    1800, 0000012 ); BAUDOPTVAL(    2400, 0000013 ); BAUDOPTVAL(    4800, 0000014 );
			BAUDOPTVAL(    9600, 0000015 ); BAUDOPTVAL(   19200, 0000016 ); BAUDOPTVAL(   38400, 0000017 ); 
			BAUDOPTVAL(   57600, 0010001 ); BAUDOPTVAL(  115200, 0010002 ); BAUDOPTVAL(  230400, 0010003 ); BAUDOPTVAL(  460800, 0010004 );
			BAUDOPTVAL(  500000, 0010005 ); BAUDOPTVAL(  576000, 0010006 ); BAUDOPTVAL(  921600, 0010007 ); BAUDOPTVAL( 1000000, 0010010 );
			BAUDOPTVAL( 1152000, 0010011 ); BAUDOPTVAL( 1500000, 0010012 ); BAUDOPTVAL( 2000000, 0010013 ); BAUDOPTVAL( 2500000, 0010014 );
			BAUDOPTVAL( 3000000, 0010015 ); BAUDOPTVAL( 3500000, 0010016 ); BAUDOPTVAL( 4000000, 0010017 ); 
			// For other baud rates: some library headers  `#define BOTHER 0010000`  so you're supposed to use that and then if you are using a `struct termios2` from `#include <asm/termios.h>` you can set `c_ispeed` and `c_ospeed` to the custom value you want
			if( knownOpt ) { RAISE( "unsupported value \"" << optionValue << "\" for option \"" << optionKey << "\"" ); }
			else           { RAISE( "unsupported option \"" << optionKey << "\"" ); }
		}
		DBREPORT(baud);DBREPORT(bitsPerByte);DBREPORT(parityFlag);DBREPORT(stopFlag);DBREPORT(dtr);DBREPORT(rts);


		if( !mpPreviousSettings ) Init();
		mHandle = open( name.c_str(), O_RDWR | O_NOCTTY | O_NDELAY );
		if( mHandle <= 0 )
			{ mHandle = 0; RAISE( "Failed to open serial port \"" << mName << "\"" ); }

		struct termios & previousSettings = *( struct termios * )mpPreviousSettings;
		struct termios & currentSettings  = *( struct termios * )mpCurrentSettings;
		if( tcgetattr( mHandle, &previousSettings ) < 0 )
			{ close( mHandle ); mHandle = 0; RAISE( "Failed to read settings from serial port \"" << mName << "\"" ); }
		memcpy( &currentSettings, &previousSettings, sizeof( currentSettings ) );
		currentSettings.c_cflag = baud | bitsPerByte | parityFlag | stopFlag | CLOCAL | CREAD;
		currentSettings.c_iflag = IGNPAR;
		currentSettings.c_oflag = 0;
		currentSettings.c_lflag = 0;
		currentSettings.c_cc[ VMIN ] = 0;  // blocking based on number of bytes until required bytes are received
		currentSettings.c_cc[ VTIME ] = 0; // blocking based on timer (in units of 10^-5 sec)
		if( tcsetattr( mHandle, TCSANOW, &currentSettings ) < 0 )
			{ close( mHandle ); mHandle = 0; RAISE( "Failed to update settings for serial port \"" << mName << "\"" ); }
		int status;
		if( ioctl( mHandle, TIOCMGET, &status ) < 0 )
			{ close( mHandle ); mHandle = 0; RAISE( "Failed to query status from serial port \"" << mName << "\"" ); }
		if( dtr ) status |= TIOCM_DTR;  // enable DTR
		else      status &= ~TIOCM_DTR; // disable DTR
		if( rts ) status |= TIOCM_RTS;  // enable RTS
		else      status &= ~TIOCM_RTS; // disable RTS
		if( ioctl( mHandle, TIOCMSET, &status ) < 0 )
			{ close( mHandle ); mHandle = 0; RAISE( "Failed to update status for serial port \"" << mName << "\"" ); }

#	endif     // End OS-specific implementations

	mTimeZero = Seconds();
}

bool
SerialConnection::IsOpen( void ) const
{
	if( mFake ) return true;
	if( !mHandle ) return false;
	// TODO: A better name for this would be HasBeenOpened() since we are not checking whether the connection is still alive.
	//       Under Windows, manashmandal/SerialPort flags the connection as broken if ClearCommError( mHandle, &errors, &status ) returns zero.
	return true;
}

void
SerialConnection::Close( void )
{
	if( !mHandle ) return;
#	if _WIN32 // Begin Windows implementation
		CloseHandle( mHandle );
#	else      // Begin Posix implementation
		int status;
		if( ioctl( mHandle, TIOCMGET, &status ) < 0 ) status = 0; // if ioctl returns < 0: failed to get (and hence cannot revert) port settings
		else
		{
			status &= ~TIOCM_DTR & ~TIOCM_RTS; // disable RTS and DTR
			if( ioctl( mHandle, TIOCMSET, &status ) < 0 ) status = 0; // if ioctl returns < 0: failed to revert port settings
		}
		close( mHandle );
		tcsetattr( mHandle, TCSANOW, ( struct termios * )mpPreviousSettings );
#	endif     // End OS-specific implementations
	mHandle = 0;
}

int64_t
SerialConnection::Send( const std::string & s )
{
	return Send( ( void * )s.c_str(), ( uint32_t )s.length() );
}

int64_t
SerialConnection::Send( const void * buffer, uint32_t nBytes )
{
	if( !mHandle ) return 0;
	if( mFake ) return ( int64_t )nBytes;
#	if _WIN32 // Begin Windows implementation
		DWORD nWritten; // NB: DWORD = uint32_t
		if( WriteFile( mHandle, buffer, nBytes, &nWritten, NULL ) )
		{
			if( !FlushFileBuffers( mHandle ) ) RAISE( "failed to flush buffers after writing to serial port" );
			return ( int64_t )nWritten;
		}
		DWORD errors;
		COMSTAT status;
		ClearCommError( mHandle, &errors, &status );
		return -1;
#	else      // Begin Posix implementation
		int64_t nWritten = write( mHandle, buffer, nBytes ); 
		if( nWritten < 0 ) nWritten = -errno;
		if( nWritten > 0 ) fsync( mHandle ); // TODO: needed? If so, should it be fsync( mHandle ) or fdatasync( mHandle ) ? Or maybe we should disable buffering in the open() call instead
		return nWritten;
#	endif     // End OS-specific implementations
}

uint32_t
SerialConnection::Available( void )
{
	if( !mHandle && !mFake ) return 0;
	if( mFake ) return ( uint32_t )mFake->size();
	
#	if _WIN32 // Begin Windows implementation
		DWORD errors;
		COMSTAT status;
		if( !ClearCommError( mHandle, &errors, &status ) ) return 0;  // error codes (in status) can be a bitwise combination of CE_BREAK | CE_FRAME | CE_OVERRUN | CE_RXOVER | CE_RXPARITY
		return status.cbInQue; // NB: this is a DWORD which means uint32_t
#	else      // Begin Posix implementation
		int bytesAvailable;
		if( ioctl( mHandle, FIONREAD, &bytesAvailable ) < 0 ) return 0; // TODO: pyserial seems to think TIOCINQ is the right thing to use instead of FIONREAD
		if( bytesAvailable < 0 ) return 0;
		return ( uint32_t )bytesAvailable;	
#	endif     // End OS-specific implementations
}

bool
SerialConnection::SetTimeout( double seconds )
{
	if( seconds == NO_CHANGE || seconds == mTimeoutSeconds ) return true;
	mTimeoutSeconds = seconds;
	if( mFake ) return true;
	if( !mHandle ) return false;
#	if _WIN32 // Begin Windows implementation
		bool waitForever = ( mTimeoutSeconds < 0.0 );
		DWORD msec = waitForever ? MAXDWORD : ( DWORD )( 0.5 + mTimeoutSeconds * 1000.0 ); 
		COMMTIMEOUTS timeouts;
		timeouts.ReadIntervalTimeout         = msec ? msec : MAXDWORD; // must have MAXDWORD here and 0 elsewhere if you want instantaneous timeout
		timeouts.ReadTotalTimeoutMultiplier  = waitForever ? MAXDWORD : 0;
		timeouts.ReadTotalTimeoutConstant    = waitForever ? MAXDWORD : msec;
		timeouts.WriteTotalTimeoutMultiplier = 0;
		timeouts.WriteTotalTimeoutConstant   = 0;
		return SetCommTimeouts( mHandle, &timeouts );
#	else      // Begin Posix implementation
		return true; // timeouts are implemented at Receive() time using select()
#	endif     // End OS-specific implementations
}

int64_t
SerialConnection::Receive( void * buffer, uint32_t maxBytes, double timeoutSeconds ) // the kind of Receive() where you wait until the required buffer is filled
{
	SetTimeout( timeoutSeconds );
	if( !mHandle && !mFake ) return 0;
	uint32_t nRead = 0; // same as Windows DWORD
	
	if( mFake )
	{
		double deadline = Elapsed() + mTimeoutSeconds;
		uint32_t totalRead = 0; // same as Windows DWORD
		while( true )
		{
			unsigned char *start = ( ( unsigned char * )buffer ) + totalRead;
			uint32_t nToRead = ( int )( maxBytes - totalRead );
			for( nRead = 0; nRead < nToRead && !mFake->empty(); ) { start[ nRead++ ] = mFake->front(); mFake->pop_front(); }
			totalRead += nRead;
			if( totalRead >= maxBytes ) break;
			if( mTimeoutSeconds == 0 && nRead == 0 ) break;
			if( mTimeoutSeconds == 0 && nRead != 0 ) continue;
			if( mTimeoutSeconds > 0 )
			{
				if( nRead ) deadline = Elapsed() + mTimeoutSeconds;
				else if( Elapsed() >= deadline ) break;
			}
			// remember mTimeoutSeconds could be negative (means wait forever)
			if( nRead == 0 ) SleepSeconds( 0.001 );
		}
		return ( int64_t )totalRead;
	}
#	if _WIN32 // Begin Windows implementation
		DWORD errors;
		COMSTAT status;
		if( !ClearCommError( mHandle, &errors, &status ) ) return -1;
		if( !maxBytes ) return 0;
		ReadFile( mHandle, buffer, maxBytes, ( LPDWORD )( ( void * )&nRead ), NULL );
#	else      // Begin Posix implementation
		if( !maxBytes ) return 0;
		uint32_t bytesAvailable = Available();
		if( bytesAvailable < maxBytes && mTimeoutSeconds != 0.0 )
		{
			bool waitForever = ( mTimeoutSeconds < 0.0 );
			double deadline = waitForever ? 0.0 : Elapsed() + mTimeoutSeconds;
			double remainingTimeoutSeconds = waitForever ? 1.0 : mTimeoutSeconds;
			while( remainingTimeoutSeconds > 0.0 )
			{
				timeval timeout;
				timeout.tv_sec  = ( time_t )remainingTimeoutSeconds;
				timeout.tv_usec = ( suseconds_t )( 1e6 * ( remainingTimeoutSeconds - ( double )timeout.tv_sec ) );
				if( timeout.tv_sec > 0 || timeout.tv_usec > 0 )
				{
					fd_set rdset;
					FD_ZERO( &rdset );
					FD_SET( mHandle, &rdset );				
					int result = select( mHandle + 1, &rdset, NULL, NULL, &timeout );
					if( result < 0 ) return result;  // negative means an error occurred
					if( result == 0 && !waitForever ) break; // 0 means the timeout expired
				}
				bytesAvailable = Available();
				if( bytesAvailable >= maxBytes ) break;
				if( !timeout.tv_sec && !timeout.tv_usec ) break;
				if( !waitForever ) remainingTimeoutSeconds = deadline - Elapsed();
				// at this point, we know bytes have arrived before timeout, but not enough, which is why we're in a loop
			}
		}
		if( bytesAvailable < maxBytes ) maxBytes = bytesAvailable;
		if( !maxBytes ) return 0;
		nRead = read( mHandle, buffer, maxBytes );
#	endif     // End OS-specific implementations
		
	return ( int64_t )nRead;
}

int64_t
SerialConnection::ReceiveAvailable( void * buffer, uint32_t maxBytes ) // the kind of Receive() where you take whatever you can get without waiting
{
	
	char * cbuffer = ( char * )buffer;
	uint32_t totalBytesRead = 0, bytesAvailable;
	while( totalBytesRead < maxBytes && ( bytesAvailable = Available() ) > 0 )
	{
		uint32_t toRead = maxBytes - totalBytesRead;
		if( bytesAvailable < toRead ) toRead = bytesAvailable;
		if( !toRead ) break;
		int64_t bytesRead = Receive( ( void * )cbuffer, toRead );
		if( bytesRead <= 0 ) break;
		cbuffer += bytesRead;
		totalBytesRead += ( uint32_t )bytesRead;
	}
	return totalBytesRead;
}

int64_t
SerialConnection::ReceiveUntil( std::string & line, char delimiter, double timeoutSeconds ) // the kind of Receive() where you read until a specified character is encountered
{
	int64_t totalBytesRead = 0;
	SetTimeout( timeoutSeconds );
	double deadline = Elapsed() + mTimeoutSeconds;
	line = "";
	char c;
	do
	{
		int64_t bytesRead = Receive( &c, 1 );
		if( bytesRead <= 0 ) break;
		totalBytesRead++;
		line += c;
	} while( c != delimiter && ( mTimeoutSeconds == WAIT_FOREVER || Elapsed() < deadline || Available() ) );
	return totalBytesRead;
}

#endif // __INCLUDE_SERIALCONNECTION_CPP__
