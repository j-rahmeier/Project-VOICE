#include "HyperscanningApplicationBase.h"
#include "BCIStream.h"
#include "Thread.h"

#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <mutex>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <regex>

#if _WIN32
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <in6addr.h>

#undef errno
#define errno WSAGetLastError()
#else
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

//Extension( HyperscanningApplicationBase );



HyperscanningApplicationBase::HyperscanningApplicationBase()
: mBuffer(nullptr) {
}



HyperscanningApplicationBase::~HyperscanningApplicationBase() {
	::free(mBuffer);
	Halt();
}



//
// Get the size of the message from the server
//

size_t HyperscanningApplicationBase::GetServerMessageSize() {
	size_t size = 0;

	struct timeval time;
	time.tv_sec = 0;
	time.tv_usec = 5000;

	fd_set readfds;
	FD_ZERO( &readfds );
	FD_SET( mSocket.Fd(), &readfds );

	if ( select( mSocket.Fd() + 1, &readfds, NULL, NULL, &time ) ) {
		if ( mSocket.Wait() ) {
            // !! sizeof(size_t) differs between 32 and 64 bit systems !!
            mBuffer = ( char* ) calloc( sizeof( size_t ), 1 );

			int result;
			if ( ( result = ::recv(mSocket.Fd(), mBuffer, sizeof( size_t ), 0) ) < 0 ) {  // read one packet only
				bciwarn << "Error reading: " << errno;
			} else if ( result == 0 ) {
				bcierr << "Server disconnected";
			}

			memcpy( &size, mBuffer, sizeof( size_t ) );

			free( mBuffer );
			mBuffer = NULL;
		}
	}

	return size;
}



//
// Get the message from the server
//

void HyperscanningApplicationBase::GetServerMessage( char* buff, size_t size ) {
	//for ( int i = 0; i < size; i++ ) {
	//	if ( mSocket.Wait() ) {
	int result = 0;
	int total = 0;
	while ( total < size ) {
		if ( mSocket.Wait() && ( result = ::recv(mSocket.Fd(), buff + total, size - total, 0) ) < 0 ) {  // read one packet only
			bciwarn << "Error reading: " << errno;
			return;
		} else if ( result == 0 ) {
			bcierr << "Server disconnected";
			return;
		} else {
			// bciout << "Result" << result;
		}
		total += result;
	}
	//	}
	//}
}



void HyperscanningApplicationBase::Publish() {

	//
	// Initialize Parameters and local States
	//
	//

	BEGIN_PARAMETER_DEFINITIONS
		"Application:Hyperscanning string IPAddress= 10.138.1.104 % % %"
		" // IPv4 address of server",
		"Application:Hyperscanning int Port= 1234 % % %"
		" // server port",
		"Application:Hyperscanning string ParameterPath= ../parms/CommunicationTask/HyperScanningParameters.prm % % %"
		" // path to donwload parameters from the server to",
		"Application:Hyperscanning string SharedStates= % % % %"
		" // States to share with other clients",
		"Application:Hyperscanning string PreDefinedSharedStates= % % % %"
		" // States to share with other clients that have already been defined by another module",
		"Application:Hyperscanning string ClientID= % % % %"
		" // ID for this client",
		"Application:Hyperscanning string SessionID= % % % %"
		" // ID for this game session. Must be the same between clients for them to play the same game. If the game ends, and the same session id is used the game will resume from where it left off.",
	END_PARAMETER_DEFINITIONS

	BEGIN_STATE_DEFINITIONS
		"ClientNumber 8 0 0 0"
	END_STATE_DEFINITIONS

	//
	// Initialize the states shared with the server and other client
	//

	std::string states( Parameter( "SharedStates" ) ); // get the list of shared states

	std::vector<std::string> sharedStates; // Save the names of each of the states

	std::istringstream f( ( states ) );
	std::string name;
	std::string size;

	while ( getline( f, name, ',' ) ) {

		if ( !getline( f, size, '&' ) )
			bcierr << "Every Shared State must have a size";

		// bciout << "Shared State: " << name << ", " << size;

		BEGIN_STATE_DEFINITIONS
			name + " " + size + " 0 0 0"
		END_STATE_DEFINITIONS

		sharedStates.push_back( name );
	}

	std::string predefStates( Parameter( "PreDefinedSharedStates" ) );
	f = std::istringstream( predefStates );
	while ( getline( f, name, '&' ) ) {
		sharedStates.push_back( name );
	}

	mSharedStates = sharedStates;
	mStateValues = std::vector<uint64_t>( mSharedStates.size(), 0 );
	mHasUpdated = std::vector<bool>( mSharedStates.size(), true );

	remove( ( ( std::string ) Parameter( "ParameterPath" ) ).c_str() ); //"../parms/CommunicationTask/HyperScanningParameters.prm" );

	SharedPublish();
}



void HyperscanningApplicationBase::Preflight( const SignalProperties& Input, SignalProperties& Output ) const {
	Parameter( "SharedStates" );
	Parameter( "PreDefinedSharedStates" );
	Parameter( "IPAddress" );
	Parameter( "Port" );
	Parameter( "ParameterPath" );
	if ( std::string( Parameter( "ClientID" ) ).size() == 0 ) {
		bciwarn << "ClientID not given. Using IP Address";
	}
	if ( std::string( Parameter( "SessionID" ) ).size() == 0 ) {
		bcierr << "Please supply a Session ID";
	}
	//if ( !OptionalParameter( "SharedStates" ) )
	//	bcierr << "You must have at least one shared state and a name and size for each";
	//if ( !OptionalParameter( "IPAddress" ) )
	//	bcierr << "Must give server address";
	//if ( !OptionalParameter( "Port" ) )
	//	bcierr << "Must specify port";
	//if (((std::string)Parameter("ParameterPath")).empty()) {
	//	bcierr << "Must give parameter path";
	//}
	State( "ClientNumber" );

	SharedPreflight( Input, Output );

}



void HyperscanningApplicationBase::Initialize(const SignalProperties& Input, const SignalProperties& Output) {
	// bciout << "Client Number: " << ( int ) mClientNumber;
	State( "ClientNumber" ) = mClientNumber;

	SharedInitialize( Input, Output );
}



void HyperscanningApplicationBase::AutoConfig(const SignalProperties& Input) {
	Setup();

	bool result = Parameters->Load( ( std::string ) Parameter( "ParameterPath" ) );
	if (!result) bcierr << "Loading the parameter file from the server failed!" << std::endl;

	SharedAutoConfig( Input );
}



void HyperscanningApplicationBase::StartRun() {
	State( "ClientNumber" ) = mClientNumber;
	Start();

	SharedStartRun();
}


void HyperscanningApplicationBase::UpdateStates() {
	// Ensure exclusive access to vectors
	const std::lock_guard<std::mutex> lock( mMessageMutex );
	const std::lock_guard<std::mutex> lock2( mStateValuesMutex );


	for ( int i = 0; i < mSharedStates.size(); i++ ) {

		//
		// Update the local state if the server state has changed
		//

		if ( !mHasUpdated[ i ] ) {

			// bciout << "Updated " << mSharedStates[ i ] << " to " << mStateValues[ i ] << " from the server";
			// bciout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch() ).count();
			State( mSharedStates[ i ] ) = mStateValues[ i ];
			mHasUpdated[ i ] = true;

		}
	}
}

void HyperscanningApplicationBase::UpdateMessage() {
	const std::lock_guard<std::mutex> lock( mMessageMutex );
	const std::lock_guard<std::mutex> lock2( mStateValuesMutex );

	for ( int i = 0; i < mSharedStates.size(); i++ ) {

		//
		// Update the local state if the server state has changed
		//

		if ( !mHasUpdated[ i ] ) {

			bciout << "Updated " << mSharedStates[ i ] << " to " << mStateValues[ i ] << " from the server";
			// bciout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch() ).count();
			State( mSharedStates[ i ] ) = mStateValues[ i ];
			mHasUpdated[ i ] = true;

		}
	}

	for ( int i = 0; i < mSharedStates.size(); i++ ) {
		//
		// Update the message for the server with the states that have changed locally
		//

		StateRef s = State( mSharedStates[ i ] );
		uint64_t val = s;
		if ( val != mStateValues[ i ] ) {
			bciout << mSharedStates[ i ] << " locally is  " << val;

			std::string message( mSharedStates[ i ] ); // Add name of state
			message.push_back( '\0' ); // Followed by a NULL character for termination

			message.push_back( ( char ) s->Length() / 8 ); // Add size in bytes
			message += std::string( ( char* )( &val ), s->Length() / 8 ); // Add value

			mMessage += message; // Add this segment of the message to the master message
			
			mStateValues[ i ] = val;
			// bciout << "Updated Message";
			// bciout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch() ).count();
		}
	}
}



void HyperscanningApplicationBase::StopRun() {
	TerminateAndWait();
	SharedStopRun();
}



void HyperscanningApplicationBase::Halt() {
	TerminateAndWait();
	SharedHalt();
}



//
// Set up the connection with the server and set initial values
//

void HyperscanningApplicationBase::Setup() {

	//
	// Avoid Connecting Twice
	//

	if ( mSocket.IsOpen() ) return;

	//
	// Establish connection to server
	//

	mAddress = ( std::string )OptionalParameter( "IPAddress" );

	// Convert domain name to IP Address
	unsigned char buf[ sizeof( struct in6_addr ) ];

	if ( inet_pton( AF_INET, mAddress.c_str(), buf ) <= 0 ) {
		bciout << "Attempting to convert hostname to ip address";
		struct hostent* he;
		he = gethostbyname( mAddress.c_str() );
		if ( he == NULL )
			bcierr << "Invalid IP Address or Domain Name";
		mAddress = inet_ntoa( *( struct in_addr* )he->h_addr );
		bciout << "Using IP Address: " << mAddress;
	}

	mPort = OptionalParameter( "Port" );

	bciout << "Connecting to " << mAddress << ":" << ( unsigned short )mPort;

	mSocket.Open( mAddress, ( unsigned short )mPort ); // Opening socket

	bciout << "Is open: " << mSocket.IsOpen();
	bciout << "Address: " << mSocket.RemoteAddress();
	bciout << "Connected: " << mSocket.Connected();

	// Ensure connection
	if ( !mSocket.Connected() ) {
		bcierr << "Not connected to server. Try again.";
		return;
	} else {
		bciout << "Connected to server.";
	}

	// Send version information
	bciout << "Hyperscanning Application Base Version: " << version;
	size_t size = version.size();
    send( mSocket.Fd(), ( const char* ) &size, sizeof( size_t ), 0 );
    send( mSocket.Fd(), version.c_str(), version.size(), 0 );

	// Send ID
	std::string id = Parameter( "ClientID" );
	bciout << "client id: " << id;
	size = id.size();
    send( mSocket.Fd(), ( const char* ) &size, sizeof( size_t ), 0 );
	if ( size )
        send( mSocket.Fd(), id.c_str(), id.size(), 0 );

	//Send Session ID
	std::string sid = Parameter( "SessionID" );
	bciout << "session id: " << sid;
	size = sid.size();
    send( mSocket.Fd(), ( const char* ) &size, sizeof( size_t ), 0 );
    send( mSocket.Fd(), sid.c_str(), sid.size(), 0 );

	//
	// Download Paramater File
	//

	bciout << "Awaiting server parameters...";

	if ( mSocket.Wait() ) {

		// Get the size of the parameter file from the server
		size_t size = GetServerMessageSize();

		if ( size > 0 ) {
			mBuffer = ( char* )malloc( size );

			// Get the parameter file from the server
			GetServerMessage( mBuffer, size );

			// Save the parameter file
			std::string param_file = std::string( mBuffer, size - 1 );
			std::ofstream outfile ( ( std::string ) Parameter( "ParameterPath" ) );//"../parms/CommunicationTask/HyperScanningParameters.prm" );
			outfile << param_file;

			free( mBuffer );
			mBuffer = NULL;

		}
	}

	bciout << "Recieved server parameters.";

	//
	// Get Client Number
	//

	// Get client number message
	if ( mSocket.Wait() ) {
		mBuffer = ( char* ) calloc( 1025, 1 );
		*mBuffer = 'q';

		int result;
		if ( ( result = ::recv(mSocket.Fd(), mBuffer, 1025, 0 ) ) < 0 ) // read one packet only
			bciwarn << "reading socket: " << errno;
		else if ( result == 0 )
			bcierr << "Server disconnected";

		char* buffer = mBuffer;
		std::string name( buffer );
		buffer += name.size() + 1;
		char size = *buffer++;

		uint64_t val = 0;
		memcpy( &val, buffer, size );

		bciout << "ClientNumber: " << ( int )val;

		mClientNumber = val;

		free( mBuffer );
		mBuffer = NULL;
	}

	//
	// Send Shared States List
	//

	std::string sharedstates_buffer = ( std::string ) OptionalParameter( "SharedStates" ) + ( std::string ) OptionalParameter( "PreDefinedSharedStates" );
	bciout << "Sending shared states";
	if ( ::send( mSocket.Fd(), sharedstates_buffer.c_str(), sharedstates_buffer.size(), 0 ) < 0 )
		bciwarn << "Error sending to socket: " << errno;
	bciout << "Shared states sent";

	//
	// Shared States Viability Check
	// 0: First Client, set the standard
	// 1: Successive Client, viable states
	// 2: Successive Client, incorrect states
	//

	bciout << "Waiting for server to send";
	if ( mSocket.Wait() ) {
		mBuffer = ( char* ) calloc( 1, 1 );

		int result;
		if ( ( result = ::recv( mSocket.Fd(), mBuffer, 1, 0 ) ) < 0 )
			bciwarn << "Error sending to socket: " << errno;
		else if ( result == 0 )
			bcierr << "Server disconnected";


		if ( *mBuffer == 0 )
			bciout << "You are the first client, all other clients will need the same states as you";
		if ( *mBuffer == 1 )
			bciout << "Viable shared states";
		if ( *mBuffer == 2 )
			bcierr << "Shared states do not match";

		free( mBuffer );
		mBuffer = NULL;
	}

}

//
// Run the asynchronous server loop
//

int HyperscanningApplicationBase::OnExecute() {
	while ( !Terminating() ) {
		//
		// Write Message to Server
		//

		{
			std::lock_guard<std::mutex> messageLock(mMessageMutex);
			std::string name(mMessage.c_str());

			if ( mMessage.size() > 0 ) {

				size_t size = mMessage.size() + 1;
				char* csize = ( char* ) &size;

				mMessage = std::string( csize, sizeof( size_t ) ) + mMessage;

				bciout << "Writing: " << mMessage << ", size: " << mMessage.size();
				if (::send(mSocket.Fd(), mMessage.c_str(), size + sizeof( size_t ), 0) < 0)
				{
					bciwarn << "Error writing to socket: " << errno;
					return -1;
				}

				mMessage = std::string( "" ); // Reset message
			}
		}

		//
		// Read Message From Server
		//

		size_t size = GetServerMessageSize();

		if ( size > 0 ) {
			mBuffer = ( char* ) calloc( size + 1, 1 );
			GetServerMessage( mBuffer, size );

			Interpret(mBuffer);
			free( mBuffer );
			mBuffer = NULL;
		}

	}
	return 1;
}



//
// Interpret the buffer downloaded from the server
//

void HyperscanningApplicationBase::Interpret( char* buffer ) {
	while ( *buffer != 0 ) {
		bciwarn << "Buffer: " << buffer;

		// Get State Name and Size
		std::string name( buffer );
		buffer += name.size() + 1;
		char size = *buffer++;

		if ( size == 0 ) continue;

		// Get State Value
		std::string value( buffer, size );
		buffer += size;

		uint64_t val = 0;
		memcpy( &val, value.c_str(), size );

		bciwarn << name << ": " << val;

		// Set state to be updated to new value
		mStateValuesMutex.lock();
		auto it = find( mSharedStates.begin(), mSharedStates.end(), name );
		if ( it != mSharedStates.end() ) {
			mStateValues[ it - mSharedStates.begin() ] = val;
			mHasUpdated[ it - mSharedStates.begin() ] = false;
		} else {
			bciwarn << "State is not a part of Hyperscanning Shared States";
		}
		mStateValuesMutex.unlock();

	}
}


void HyperscanningApplicationBase::Process( const GenericSignal& Input, GenericSignal& Output ) {
	UpdateStates();

	SharedProcess( Input, Output );

	UpdateMessage();
}

void HyperscanningApplicationBase::Resting() {
	SharedResting();
}

void HyperscanningApplicationBase::SharedPublish() {}
void HyperscanningApplicationBase::SharedAutoConfig( const SignalProperties& Input ) {}
void HyperscanningApplicationBase::SharedPreflight( const SignalProperties& Input, SignalProperties& Output ) const {}
void HyperscanningApplicationBase::SharedInitialize( const SignalProperties& Input, const SignalProperties& Output ) {}
void HyperscanningApplicationBase::SharedStartRun() {}
void HyperscanningApplicationBase::SharedProcess( const GenericSignal& Input, GenericSignal& Output ) {}
void HyperscanningApplicationBase::SharedResting() {}
void HyperscanningApplicationBase::SharedStopRun() {}
void HyperscanningApplicationBase::SharedHalt() {}
