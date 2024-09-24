// $Id: SerialConnection.h 7524 2023-08-11 17:00:06Z jhill $
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


#ifndef   __INCLUDE_SERIALCONNECTION_H__
#define   __INCLUDE_SERIALCONNECTION_H__

#include <deque>
#include <string>
#include "stdint.h"

class SerialConnection
{
	public:
		SerialConnection();
		SerialConnection( std::string name );
		~SerialConnection();
		
		static const double NO_CHANGE;
		static const double WAIT_FOREVER;
	
		std::string    Name( void ) const;
		
		void           Init();
		void           SetFakeSource( std::deque<char> * fake, double t0 = 0.0 );
		void           FeedFakeData( const char * buffer, size_t nBytes );
		bool           IsFake( void ) const;
		void           Open( std::string name );
		bool           IsOpen( void ) const;
		double         Elapsed( void ) const;
		int64_t        Send( const std::string & s );
		int64_t        Send( const void * buffer, uint32_t nBytes );
		uint32_t       Available( void );
		bool           SetTimeout( double seconds );
		int64_t        ReceiveAvailable( void * buffer, uint32_t maxBytes );
		int64_t        Receive( void * buffer, uint32_t nBytes, double timeoutSeconds=SerialConnection::NO_CHANGE );
		int64_t        ReceiveUntil( std::string & line, char delimiter='\n', double timeoutSeconds=SerialConnection::NO_CHANGE );
		void           Close( void );
		void           CleanUp();
		
	private:
		std::string    mName;
		double         mTimeZero;
		double         mTimeoutSeconds;
#		ifdef _WIN32
			void *         mHandle;
#		else
			int            mHandle;
			void *         mpPreviousSettings;
			void *         mpCurrentSettings;
#		endif
		std::deque<char> * mFake;
};

std::string EncodeStringLiteral( const std::string & s,         const char * quote="\"" );
std::string EncodeStringLiteral( const char * s,                const char * quote="\"" );
std::string EncodeStringLiteral( const char * s, size_t length, const char * quote="\"" );
std::string DecodeStringLiteral( const std::string & s );


#endif // __INCLUDE_SERIALCONNECTION_H__
