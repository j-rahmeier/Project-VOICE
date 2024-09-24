// $Id: test.cpp 7537 2023-08-19 03:52:08Z jhill $
// Test of the SerialConnection class.
// 
//     cl.exe /EHsc test.cpp
//     test.exe SERIALPORT[:PORT_OPTIONS] [COMMAND] [TIMEOUT] [NBYTES]
//
//     g++ -o test test.cpp
//     ./test SERIALPORT[:PORT_OPTIONS] [COMMAND] [TIMEOUT] [NBYTES]
//
// SERIALPORT defaults to COM4
// COMMAND defaults to ?
// TIMEOUT is in seconds, defaults to 0.1, and -1 means "forever"
// NBYTES is the number of bytes to wait for (if omitted, read
// until you encounter a newline, or until you time out).
// 
// A serial connection is opened on the specified SERIALPORT.
// A newline is appended to the COMMAND and the result is sent.
// We then pause for 250ms to allow the device to compose and
// send its reply. Then we read according to the specified
// combination of TIMEOUT and NBYTES arguments.

#include "SerialConnection.cpp"

#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <iomanip>
#include <string>

double Send( SerialConnection & sp, std::string msg, double pauseSeconds=0.25 )
{
	if( !msg.length() ) { printf( "Nothing to send.\n" ); return 0.0; }
	msg += "\n";
	printf( "Sending %d characters: %s\n", ( int )msg.length(), EncodeStringLiteral( msg, 0 ).c_str() );
	double t0 = sp.Elapsed();
	int nSent = ( int )sp.Send( msg );
	double t = sp.Elapsed() - t0;
	printf( "Sent %d characters in %.3fms.\n", nSent, t * 1000.0 );
	if( nSent != msg.length() ) printf( "!!! BYTE COUNT MISMATCH IN Send() RESULT\n" );
	if( pauseSeconds > 0.0 ) { printf( "Pausing for %gms.\n", pauseSeconds * 1000.0 ); SleepSeconds( pauseSeconds ); }
	return t;
}
double Receive( SerialConnection & sp, uint32_t nBytes, double timeoutSeconds=SerialConnection::WAIT_FOREVER, int lineEndChar=-1 )
{
	char delim = ( char )lineEndChar;
	char * buffer = NULL;
	int nReceived;
	std::string result;
	double t0, t;
	bool mismatch = false;
	if( lineEndChar >= 0 ) printf( "Requesting all chars until %s", EncodeStringLiteral( std::string( &delim, 1 ), "'" ).c_str() );
	else printf( "Requesting %d bytes%s", nBytes, ( timeoutSeconds == SerialConnection::WAIT_FOREVER ? "" : " (max.)") );
	if( timeoutSeconds == SerialConnection::WAIT_FOREVER ) printf( " with no timeout.\n" );
	else printf( " with timeout %gms.\n", timeoutSeconds * 1000.0 );
	if( lineEndChar >= 0 )
	{
		t0 = sp.Elapsed();
		nReceived = ( int )sp.ReceiveUntil( result, lineEndChar, timeoutSeconds );
		t = sp.Elapsed() - t0;
	}
	else
	{
		buffer = new char[ nBytes ];
		t0 = sp.Elapsed();
		nReceived = ( int )sp.Receive( buffer, nBytes, timeoutSeconds );
		t = sp.Elapsed() - t0;
		result.assign( buffer, nReceived );
		delete [] buffer;
		mismatch = ( timeoutSeconds == SerialConnection::WAIT_FOREVER && nReceived != nBytes );
	}
	printf( "Received %d bytes returning after %.3fms", nReceived, t * 1000.0 );
	if( nReceived ) printf( ": %s\n", EncodeStringLiteral( result, 0 ).c_str() );
	else printf( ".\n" );
	if( nReceived != result.length() ) printf( "!!! INTERNAL BYTE COUNT MISMATCH\n" );
	if( mismatch ) printf( "!!! RECEIVED UNEXPECTED NUMBER OF BYTES\n" );
	return t;
}
double Receive( SerialConnection & sp, char delim, double timeoutSeconds=SerialConnection::WAIT_FOREVER )
{
	return Receive( sp, 0, timeoutSeconds, ( int )( unsigned char )delim );
}

int main( int argc, const char * argv[] )
{
	try
	{
		SerialConnection sp( argc >= 2 ? argv[ 1 ] : "COM4" );
		std::string msg( argc >= 3 ? DecodeStringLiteral( argv[2] ) : "?" );
		double timeoutSeconds = 0.1;
		uint32_t maxBytes = 0;
		if( argc >= 4 )
		{
			std::string remainder;
			std::stringstream ss( argv[ 3 ] ); 
			ss >> timeoutSeconds;
			bool failed = ss.fail();
			ss >> remainder;
			if( failed || remainder.length() ) { printf( "failed to interpret %s as a timeout in seconds\n", argv[ 3 ] ); return -1; }
		}
		if( argc >= 5 )
		{
			std::string remainder;
			std::stringstream ss( argv[ 4 ] ); 
			ss >> maxBytes;
			bool failed = ss.fail();
			ss >> remainder;
			if( failed || remainder.length() ) { printf( "failed to interpret %s as a number of bytes\n", argv[ 4 ] ); return -1; }
		}
		

		double sendMsec, receiveMsec;
		sendMsec = 1000.0 * Send( sp, msg );
		if( maxBytes ) receiveMsec = 1000.0 * Receive( sp, maxBytes, timeoutSeconds );
		else receiveMsec = 1000.0 * Receive( sp, '\n', timeoutSeconds );
		
		printf( "Send time:    %8.3fms\n", sendMsec );
		printf( "Receive time: %8.3fms\n", receiveMsec );
		printf( "Total:        %8.3fms\n", sendMsec + receiveMsec );
		
		
	}
	catch( std::string const & error )
	{
		std::cerr << error << std::endl;
		return -1;
	}
	return 0;
}
