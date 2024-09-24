////////////////////////////////////////////////////////////////////////////////
// Authors: lijintao@SH-lijintao
// Description: NeuSenWADC implementation
////////////////////////////////////////////////////////////////////////////////
#include "NeuSenWADC.h"

#include "BCIStream.h"
#include "BCIEvent.h"
#include <Windows.h>
#include <ctime>

RegisterFilter( NeuSenWADC, 1 );

NeuSenWADC::NeuSenWADC()
{
    m_trigger_box = false;
}

NeuSenWADC::~NeuSenWADC()
{
}

void
NeuSenWADC::OnPublish()
{
 BEGIN_PARAMETER_DEFINITIONS

    "Source:Signal%20Properties int SourceCh= 8 "
       "auto 1 % // number of digitized and stored channels",

    "Source:Signal%20Properties int SampleBlockSize= 40 "
       "auto 1 % // number of samples transmitted at a time",

    "Source:Signal%20Properties float SamplingRate= 1000 "
       "auto 0.0 % // sample rate",

    "Source:Signal%20Properties list SourceChGain= 1 auto "
       " // physical units per raw A/D unit",

    "Source:Signal%20Properties list SourceChOffset= 1 auto "
       " // raw A/D offset to subtract, typically 0",

    "Source:Signal%20Properties list ChannelNames= 1 auto "
       " // names of amplifier channels",

    "Source:Signal%20Properties int TriggerBox= 0 0 0 1 "
       " // using triggerbox or not: 0 no, 1 yes (boolean)",

 END_PARAMETER_DEFINITIONS
}

void
NeuSenWADC::OnAutoConfig()
{

}

void
NeuSenWADC::OnPreflight( SignalProperties& Output ) const
{
    SignalType sigType = SignalType::float32;
    int channel_num = Parameter("SourceCh");
    int sample_rate = Parameter("SamplingRate");
    int sample_block_size = Parameter("SampleBlockSize");

    if (sample_block_size * 1000 % sample_rate)
        bcierr << "SampleBlockSize * 1000 / SamplingRate should be an integer. Please check.";

    if (sample_rate == 2000)
    {
        if (channel_num > 32)
        {
            bcierr << "When sampling rate is equal to 2000, channel num can not be greater than 32";
        }
    }
    if (sample_rate == 4000)
    {
        if (channel_num > 16)
        {
            bcierr << "When sampling rate is equal to 4000, channel num can not be greater than 16";
        }
    }
    if (sample_rate == 8000)
    {
        if (channel_num > 8)
        {
            bcierr << "When sampling rate is equal to 8000, channel num can not be greater than 8";
        }
    }
    if (sample_rate == 16000)
    {
        if (channel_num > 1)
        {
            bcierr << "When sampling rate is equal to 16000, channel num can not be greater than 1";
        }
    }

    if (Parameter("TriggerBox"))
    {
        bciout << "When use TriggerBox, there may be data loss";

        Output = SignalProperties(channel_num + 2, sample_block_size, sigType);
        (Output.ChannelLabels())[channel_num] = "@light_trigger";
        (Output.ChannelLabels())[channel_num + 1] = "@audio_trigger";
    }

    bool use_trigger = Parameter("TriggerBox");

    NeuracleDiscoveryController* pController;

    NeuracleResultCode ret = NeuracleDeviceDiscovery(&pController);
    if (NeuracleResult_Succeed != ret)
    {
        bcierr << "NeuracleDeviceDiscovery Failed";
    }

    DeviceInformation* deviceInformations;
    int deviceCount = 0;
    int lastDeviceCount = 0;
    int deviceBufferCount = 10;
    deviceInformations = (DeviceInformation*)malloc(sizeof(DeviceInformation) * deviceBufferCount);

    NeuracleStartDeviceDiscovery(pController);

    clock_t start, end;
    double time_interval = 0;

    start = clock();

    while (true)
    {

        deviceCount = deviceBufferCount;

        ret = NeuracleGetFoundedDevices(pController, deviceInformations, &deviceCount);

        if (NeuracleResult_Warning_NeedMoreSpace == ret)
        {
            bciout << "need more space";

            deviceBufferCount = deviceCount + 10;
            deviceInformations = (DeviceInformation*)realloc(deviceInformations, sizeof(DeviceInformation) * deviceBufferCount);
            ret = NeuracleGetFoundedDevices(pController, deviceInformations, &deviceCount);
        }
        if (NeuracleResult_Succeed == ret)
        {
            if (use_trigger)
            {
                if (deviceCount == 2)
                    break;
            }
            else
            {
                if (deviceCount != 0)
                    break;
            }
        }

        Sleep(100);

        end = clock();

        time_interval = double((end - start) * 1000) / CLOCKS_PER_SEC;

        if (time_interval >= 10000)
            break;
    }

    if (time_interval >= 10000)
    {
        bcierr << "Network connection failed";
    }

    NeuracleStopDeviceDiscovery(pController);
}

void
NeuSenWADC::OnInitialize( const SignalProperties& Output )
{
    m_trigger_box = Parameter("TriggerBox");

    m_sample_block = Parameter("SampleBlockSize");
    m_sample_rate = Parameter("SamplingRate");

    NeuracleDiscoveryController* pController;

    NeuracleResultCode ret = NeuracleDeviceDiscovery(&pController);
    if (NeuracleResult_Succeed != ret)
    {
        bcierr << "NeuracleDeviceDiscovery Failed";
    }

    DeviceInformation* deviceInformations;
    int deviceCount = 0;
    int lastDeviceCount = 0;
    int deviceBufferCount = 10;
    deviceInformations = (DeviceInformation*)malloc(sizeof(DeviceInformation) * deviceBufferCount);

    NeuracleStartDeviceDiscovery(pController);

    clock_t start, end;
    start = clock();

    while (true)
    {
        deviceCount = deviceBufferCount;

        ret = NeuracleGetFoundedDevices(pController, deviceInformations, &deviceCount);

        if (NeuracleResult_Warning_NeedMoreSpace == ret)
        {
            deviceBufferCount = deviceCount + 10;
            deviceInformations = (DeviceInformation*)realloc(deviceInformations, sizeof(DeviceInformation) * deviceBufferCount);
            ret = NeuracleGetFoundedDevices(pController, deviceInformations, &deviceCount);
        }
        if (NeuracleResult_Succeed == ret)
        {
            if (lastDeviceCount != deviceCount)
            {
                lastDeviceCount = deviceCount;
                for (int i = 0; i < deviceCount; i++)
                {
                    if (deviceInformations[i].IsTriggerBox)
                    {
                        m_trigger_address = deviceInformations[i].IPAddress;
                    }
                    else
                    {
                        m_device_address = deviceInformations[i].IPAddress;
                    }
                }
                lastDeviceCount = deviceCount;
            }
        }

        if (m_trigger_box)
        {
            if (deviceCount == 2)
                break;
        }
        else
        {
            if (deviceCount != 0)
                break;
        }
        Sleep(100);
    }

    end = clock();

    NeuracleStopDeviceDiscovery(pController);

    int channel_num = Parameter("SourceCh");
    int sample_rate = Parameter("SamplingRate");

    int sample_block_size = Parameter("SampleBlockSize");

    /*int max_delay_milli_seconds = 0;
    if (m_sample_rate == 250)
        max_delay_milli_seconds = 4;
    else if (m_sample_rate == 500)
        max_delay_milli_seconds = 2;
    else
        max_delay_milli_seconds = 1;*/

    int max_delay_milli_seconds = 1000 * sample_block_size / sample_rate;

    if (m_trigger_box)
    {
        ret = NeuracleControllerInitializeWithTriggerBox(&m_Controller, m_device_address, m_trigger_address, channel_num, sample_rate, 12, max_delay_milli_seconds, true);
    }
    else
    {
        ret = NeuracleControllerInitialize(&m_Controller, m_device_address, channel_num, sample_rate, 12, max_delay_milli_seconds, true);
    }

    if (NeuracleResult_Succeed != ret)
    {
        bciout << "Device initialize failed" << ret;
    }

    if (m_trigger_box)
    {
        for (int i = 0; i < 10; i++)
        {
            std::string temp = "COM" + std::to_string(i);
            m_port_num = temp.c_str();
            m_com = CreateFileA(m_port_num,
                GENERIC_READ | GENERIC_WRITE,
                0,
                NULL,
                OPEN_EXISTING,
                0,
                NULL);

            if (m_com != INVALID_HANDLE_VALUE)
                break;
        }

        SetupComm(m_com, 1700, 1700);
        COMMTIMEOUTS TimeOuts;

        TimeOuts.ReadIntervalTimeout = 100;
        TimeOuts.ReadTotalTimeoutMultiplier = 500;
        TimeOuts.ReadTotalTimeoutConstant = 500;
        TimeOuts.WriteTotalTimeoutMultiplier = 500;
        TimeOuts.WriteTotalTimeoutConstant = 2000;

        if (!SetCommTimeouts(m_com, &TimeOuts))
            bciout << "set comm timeout failed";

        DCB dcb;
        GetCommState(m_com, &dcb);
        dcb.fDtrControl = DTR_CONTROL_DISABLE;
        dcb.fRtsControl = RTS_CONTROL_DISABLE;

        dcb.BaudRate = 115200;

        dcb.ByteSize = 8;
        dcb.Parity = NOPARITY;
        dcb.StopBits = ONESTOPBIT;

        if (!SetCommState(m_com, &dcb))
            bciout << "set comm state failed";

        GetCommState(m_com, &dcb);

        PurgeComm(m_com, PURGE_TXCLEAR | PURGE_RXCLEAR);

        char order[8] = { (char)0x01,(char)0xE7,(char)0x04,(char)0x00, (char)0x01, (char)0x07, (char)0x00, (char)0x0F };
        DWORD dlength = sizeof(order) / sizeof(order[0]);

        DWORD dwError = 0;
        if (ClearCommError(m_com, &dwError, NULL) && dwError > 0)
        {
            PurgeComm(m_com, PURGE_TXABORT | PURGE_TXCLEAR);
        }

        DWORD dwTx = 0;
        BOOL ret = FALSE;
        ret = WriteFile(m_com, order, dlength, &dwTx, NULL);
    }
}

void
NeuSenWADC::OnStartAcquisition()
{
    while (true)
    {
        NeuracleResultCode ret = NeuracleControllerStart(m_Controller);

        if (ret != NeuracleResult_Succeed)
        {
            if (ret == 134217731)
                bciout << "Doggle verify fail";
            else
                bciout << "controller start failed. ret is " << ret;
        }
        else
        {
            break;
        }

        Sleep(1);
    }
}

void
NeuSenWADC::DoAcquire( GenericSignal& Output )
{
    /*int minimum_block = 0;

    if (m_sample_rate == 250)
        minimum_block = 0.004 * m_sample_rate;
    else if (m_sample_rate == 500)
        minimum_block = 0.002 * m_sample_rate;
    else
        minimum_block = 0.001 * m_sample_rate;

    int time = m_sample_block / minimum_block;*/

    DataBlock* dataBlock = nullptr;
    NeuracleResultCode ret;

    int failure = 0, not_enough = 0, actual = 0;
    //bciout << "time is " << time;

    //for (int t = 0; t < time; t++)
    //{
    while (true)
    {
        actual += 1;
        ret = NeuracleControllerReadData(m_Controller, &dataBlock);
        if (ret == NeuracleResult_Succeed)
        {
            break;
        }
        else if (ret == NeuracleResult_Warning_NoEnoughData)
        {
            not_enough += 1;
        }
        else
            failure += 1;

        Sleep(5);
    }

    for (int i = 0; i < Output.Channels(); i++)
    {
        for (int j = 0; j < Output.Elements(); j++)
        {
            Output(i, j) = dataBlock->Datas[i * Output.Elements() + j];
        }

        /*for (int j = 0; j < minimum_block; j++)
        {
            int output_index = t * minimum_block + j;
            int data_index = i * minimum_block + j;

            Output(i, output_index) = dataBlock->Datas[data_index];
        }*/
    }

    NeuracleFreeDataBlock(dataBlock);
    //}

    /*bciout << "not_enough is " << not_enough;
    bciout << "failure is " << failure;
    bciout << "actual is " << actual;*/
}

void
NeuSenWADC::OnStopAcquisition()
{
    NeuracleResultCode ret = NeuracleControllerStop(m_Controller);
    NeuracleControllerFinialize(m_Controller);
}

