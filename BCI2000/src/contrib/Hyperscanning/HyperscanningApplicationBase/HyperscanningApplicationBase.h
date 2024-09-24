#ifndef NETWORK_LOGGER_H
#define NETWORK_LOGGER_H

#include "ApplicationBase.h"
#include "Thread.h"
#include "Sockets.h"
#include <atomic>
#include <mutex>


class HyperscanningApplicationBase : public ApplicationBase, public Thread {
	public:
		HyperscanningApplicationBase();
		~HyperscanningApplicationBase();

		void Publish() override;
		void AutoConfig( const SignalProperties& Input ) override;
		void Preflight( const SignalProperties& Input, SignalProperties& Output ) const override;
		void Initialize( const SignalProperties& Input, const SignalProperties& Output ) override;
		void StartRun() override;
		void Process( const GenericSignal& Input, GenericSignal& Output ) override;
		void StopRun() override;
		void Halt() override;
		void Resting() override;

		virtual void SharedPublish();
		virtual void SharedAutoConfig( const SignalProperties& Input );
		virtual void SharedPreflight( const SignalProperties& Input, SignalProperties& Output ) const;
		virtual void SharedInitialize( const SignalProperties& Input, const SignalProperties& Output );
		virtual void SharedStartRun();
		virtual void SharedProcess( const GenericSignal& Input, GenericSignal& Output );
		virtual void SharedResting();
		virtual void SharedStopRun();
		virtual void SharedHalt();


		int OnExecute() override;
		void Setup();
		void UpdateStates();
		void UpdateMessage();

		void Interpret( char* );

		size_t GetServerMessageSize();
		void GetServerMessage( char*, size_t );

	private:

		//int on_create() override;
//		int on_open() override {
//		    return 1;
//		}

		bool downloadedParms = false;

		std::vector<std::string> mSharedStates;
		std::vector<uint64_t> mStateValues;
		std::vector<bool> mHasUpdated;
		std::mutex mStateValuesMutex;
		std::string mMessage;
		std::mutex mMessageMutex;

		ClientTCPSocket mSocket;
/*		int sockfd;
		int clientfd; */
		char* mBuffer;

		std::string mAddress;
		int mPort;

		char mClientNumber;

		std::string version = "1.0";
};

#endif
