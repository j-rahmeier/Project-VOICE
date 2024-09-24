/////////////////////////////////////////////////////////////////////////////
// $Id: SerialInterface.h 7618 2023-09-25 05:17:08Z jhill $
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
#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#include "Environment.h"
#include "Expression.h"
#include "WaitableEvent.h"
#include "ThreadUtils.h"

#include "SerialConnection.h"

#include "Windows.h"

// Message values for MonitorThread
#define DO_NOTHING     0
#define SEND_COMMAND   1
#define SEND_TRIGGERS  2
#define EXIT_THREAD   -1

class SerialInterface;

#ifdef WIDGET_ADC
#	include "GenericADC.h"
#	define BASE_CLASS GenericADC
#else
#	define BASE_CLASS EnvironmentExtension
#endif
class SerialInterface : public BASE_CLASS
{
	public:
		         SerialInterface();
		virtual ~SerialInterface();
		void Publish( void ) override;
		void Halt( void ) override;
		void StartRun( void ) override;
		void StopRun( void ) override;
#	ifdef WIDGET_ADC
		void AutoConfig( const SignalProperties & ) override;
		void Preflight( const SignalProperties & inputProperties, SignalProperties & outputProperties ) const override;
		void Initialize( const SignalProperties & inputProperties, const SignalProperties & outputProperties ) override;
		void Process( const GenericSignal & inputSignal, GenericSignal & outputSignal ) override;
#	else
		void Preflight() const override;
		void Initialize() override;
		void Process() override;
#	endif
		void MonitorThread();

	private:
		class Condition
		{
			public:
				Condition( const std::string & triggerExpression, const std::string & outgoingMessage );
				~Condition();
				bool        Evaluate( void );
				int64_t     Send( SerialConnection & port );
			private:
				bool        mPreviousValue;
				uint32_t    mBytesToSend;
				Expression  mTriggerExpression;
				std::string mOutgoingMessage;
		};
		typedef std::vector< Condition > ConditionVector;
		typedef std::vector< std::string > StringVector;
		
		void             ParseOutputs( ConditionVector & conditions ) const;
		void             PassMessageToMonitorThread( int messageValue );
		void             PassMessageToMonitorThread( const std::string & serialCommand );
		int              SendCommand( const std::string & serialCommand, const std::string & description="" );
		std::string      Decode( const std::string & s ) const;

	
		bool             mBlocked;
		bool             mEnabled;
		bool             mGood;
		bool             mRecordEvents;
		std::string      mPortSpec;
		SerialConnection mPort;
		std::thread      mMonitorThread;
		bool             mMonitorThreadIsRunning;
		bool             mKillThreads;
		int              mThreadMessage;
		WaitableEvent    mThreadLock;
		ThreadUtils::TimeSlice mTimeSlice;
		ConditionVector  mConditions;
		int              mElseIf;
		StringVector     mNumericParameters;
		StringVector     mStringParameters;
		std::string      mStartCommand;
		std::string      mStopCommand;
		std::string      mPendingCommand;
#		ifdef WIDGET_ADC
			char *        mSignalBuffer;
			GenericSignal mIncoming;
			WaitableEvent mBlockReceived;
#		endif
};



#endif // SERIALINTERFACE_H
