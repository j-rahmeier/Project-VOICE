#ifndef __NEURACLESDK_H__
#define __NEURACLESDK_H__

extern "C"
{

	enum NeuracleResultCode {
		NeuracleResult_Succeed = 0,
		NeuracleResult_Information_SyncSucceeded		= 0x01000001,
		NeuracleResult_Information_TriggerArrived		= 0x01000002,
		NeuracleResult_Information_ReconnectSucceeded	= 0x01000003,
		NeuracleResult_Information_ValueNoChanging		= 0x01000004,

		NeuracleResult_Warning_NeedMoreSpace		= 0x04000001,
		NeuracleResult_Warning_NoEnoughData			= 0x04000002,
		NeuracleResult_Warning_DoggleLost			= 0x04000003,
		NeuracleResult_Warning_DeviceDisconnected	= 0x04000004,

		NeuracleResult_Error_InvalidHandle			= 0x08000001,
		NeuracleResult_Error_InvalidParameter		= 0x08000002,
		NeuracleResult_Error_DoggleVerifyFailed		= 0x08000003,
		NeuracleResult_Error_StartAmplifyFailed		= 0x08000004,
		NeuracleResult_Error_StartTriggerFailed		= 0x08000005,

		NeuracleResult_System_Error_NoEnoughMemory	= 0x0C000001,
	};


	typedef struct {
		char DeviceType[20];
		char SerialNumber[20];
		char IPAddress[20];
		bool IsTriggerBox;
	}DeviceInformation;

	typedef struct DataBlock {
		float* Datas;
		int* Markers;
		int Timestamp;
		int ChannelCount;
		int DataCountPerChannel;
	}DataBlock;

	typedef struct NeuracleDeviceStatus {
		int BatteryLevel;
		bool SyncStatus;
		bool DeviceConnectStatus;
	};

	struct NeuracleController;
	struct NeuracleDiscoveryController;

	__declspec(dllexport) NeuracleResultCode NeuracleDeviceDiscovery(NeuracleDiscoveryController** ppController);
	__declspec(dllexport) NeuracleResultCode NeuracleStartDeviceDiscovery(NeuracleDiscoveryController* controller);
	__declspec(dllexport) NeuracleResultCode NeuracleGetFoundedDevices(NeuracleDiscoveryController* controller, DeviceInformation* devices, int* deviceCount);
	__declspec(dllexport) void NeuracleStopDeviceDiscovery(NeuracleDiscoveryController* controller);
	

	__declspec(dllexport)  NeuracleResultCode NeuracleControllerInitialize(NeuracleController** controller, char* deviceAddress, int channelCount, int sampleRate, int gain, int maxDelayMilliSeconds, bool autoReconnect);
	__declspec(dllexport)  NeuracleResultCode NeuracleControllerInitializeWithTriggerBox(NeuracleController** controller, char* deviceAddress, char* triggerBoxAddress, int channelCount, int sampleRate, int gain, int maxDelayMilliSeconds, bool autoReconnect);
	__declspec(dllexport)  NeuracleResultCode NeuracleControllerReadData(NeuracleController* controller, DataBlock** dataBlock);
	__declspec(dllexport)  NeuracleResultCode NeuracleControllerGetStatus(NeuracleController* controller, NeuracleDeviceStatus *deviceStatus);
	__declspec(dllexport)  NeuracleResultCode NeuracleControllerReadImpedance(NeuracleController* controller, float* impedance, int* count);
	__declspec(dllexport)  NeuracleResultCode NeuracleControllerStartImpedance(NeuracleController* controller);
	__declspec(dllexport)  NeuracleResultCode NeuracleControllerStart(NeuracleController* controller);
	__declspec(dllexport)  NeuracleResultCode NeuracleControllerStop(NeuracleController* controller);
	__declspec(dllexport)  void NeuracleControllerFinialize(NeuracleController* controller);
	__declspec(dllexport)  void NeuracleFreeDataBlock(DataBlock* dataBlock);

}

#endif
