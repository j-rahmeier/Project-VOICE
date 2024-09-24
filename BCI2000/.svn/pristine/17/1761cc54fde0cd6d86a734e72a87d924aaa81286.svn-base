////////////////////////////////////////////////////////////////////////////////
// Authors: lijintao@56B3
// Description: NeuroSparkADC implementation
////////////////////////////////////////////////////////////////////////////////
#include "NeuroSparkADC.h"

#include "BCIStream.h"
#include "BCIEvent.h"
#include <winsock.h>
#include <ctime>
#include <cstdlib>

#define PORT 8172
#define DEFAULTSIZE 20
#define LENGTH 4
#define TOTAL_LENGTH_OFFSET 6
#define CHANNEL_COUNT_OFFSET 14
#define SAMPLE_RATE_OFFSET 18
#define DATA_OFFSET 26

RegisterFilter(NeuroSparkADC, 1);

class NeuroSparkADC::NeuroRece
{
public:
    NeuroRece();
    ~NeuroRece();

    void Initialize(int buff);
    void StartReceData();
    void Close();

    SOCKET neuro_spark;
    bool connected;

    int m_channel_num;
    int m_sample_rate;
    int m_data_count_per_channel;
    int m_total_length;
};

NeuroSparkADC::NeuroRece::NeuroRece()
    :connected(false), m_channel_num(0), m_sample_rate(0), m_data_count_per_channel(0), m_total_length(0)
{
}

NeuroSparkADC::NeuroRece::~NeuroRece()
{
    if (connected)
    {
        Close();
    }
}

void NeuroSparkADC::NeuroRece::Initialize(int buff = -1)
{
#ifdef _WIN32
    WSADATA wsaData;
#endif

    SOCKADDR_IN neuro_addr;

    WORD w_req = MAKEWORD(2, 2);

#ifdef _WIN32
    WSAStartup(w_req, &wsaData);
#endif

    neuro_spark = socket(AF_INET, SOCK_STREAM, 0);

    if (buff > 0)
    {
        int size_2 = sizeof(buff);
        int err = setsockopt(neuro_spark, SOL_SOCKET, SO_RCVBUF, (char*)&buff, size_2);
    }

    neuro_addr.sin_family = AF_INET;
    neuro_addr.sin_port = htons(PORT);
    neuro_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    if (connect(neuro_spark, (SOCKADDR*)&neuro_addr, sizeof(SOCKADDR)) != SOCKET_ERROR)
    {
        connected = true;
    }

    char packet[17500];
    int readsize = recv(neuro_spark, packet, sizeof(packet), 0);
}

void NeuroSparkADC::NeuroRece::StartReceData()
{
    if (connected)
    {
        int offset_size = 0;
        char packet[17500];

        int readsize = recv(neuro_spark, packet, sizeof(packet), 0);

        if (!m_channel_num)
        {
            memcpy(&m_total_length, packet + TOTAL_LENGTH_OFFSET, LENGTH);
            memcpy(&m_channel_num, packet + CHANNEL_COUNT_OFFSET, LENGTH);
            memcpy(&m_sample_rate, packet + SAMPLE_RATE_OFFSET, LENGTH);

            if (m_sample_rate <= 1000)
                m_data_count_per_channel = 1;
            else if (m_sample_rate >= 2000)
                m_data_count_per_channel = 2;
        }
    }
}

void NeuroSparkADC::NeuroRece::Close()
{
    closesocket(neuro_spark);
    connected = false;

#ifdef _WIN32
    WSACleanup();
#endif
}

NeuroSparkADC::NeuroSparkADC()
    : mNumberOfSignalChannels(0), mSampleBlockSize(0), mSampleRate(0), receiver(new NeuroRece())
{
}

NeuroSparkADC::~NeuroSparkADC()
{
    delete receiver;
}

void
NeuroSparkADC::OnPublish()
{
    BEGIN_PARAMETER_DEFINITIONS

        "Source:Signal%20Properties int SourceCh= auto "
        "auto 1 % // number of digitized and stored channels",

        "Source:Signal%20Properties int SampleBlockSize= auto "
        "auto 1 % // number of samples transmitted at a time",

        "Source:Signal%20Properties float SamplingRate= auto "
        "auto 0.0 % // sample rate",

        "Source:Signal%20Properties list SourceChGain= 1 auto "
        " // physical units per raw A/D unit",

        "Source:Signal%20Properties list SourceChOffset= 1 auto "
        " // raw A/D offset to subtract, typically 0",

        "Source:Signal%20Properties list ChannelNames= 1 auto "
        " // names of amplifier channels",

        END_PARAMETER_DEFINITIONS
}

void
NeuroSparkADC::OnAutoConfig()
{
    receiver->Initialize();
    receiver->StartReceData();

    Parameter("SourceCh") = receiver->m_channel_num;
    Parameter("SamplingRate") = receiver->m_sample_rate;
    if (receiver->m_data_count_per_channel < DEFAULTSIZE)
        Parameter("SampleBlockSize") = DEFAULTSIZE;
    else
        Parameter("SampleBlockSize") = receiver->m_data_count_per_channel;

    receiver->Close();
}

void
NeuroSparkADC::OnPreflight(SignalProperties& Output) const
{
    SignalType sigType = SignalType::float32;

    int samplesPerBlock = Output.Elements();
    int numberOfSignalChannels = Output.Channels();

    if (samplesPerBlock % receiver->m_data_count_per_channel != 0)
        bcierr << "SampleBlockSize is not a multiple of DataCountPerChannel. Please have a check";
    if (Parameter("SamplingRate") != receiver->m_sample_rate)
        bcierr << "Sampling rate does not match. Please have a check";
    if (Parameter("SourceCh") != receiver->m_channel_num)
        bcierr << "Channel number does not match. Please have a check";

    if (Parameter("SourceChGain")->NumValues() != numberOfSignalChannels)
        bcierr << "Length of SourceChGain does not match channel number. Please have a check";
    else
    {
        for (int i = 0; i < numberOfSignalChannels; ++i)
        {
            if (Parameter("SourceChGain")(i) != 1)
                bcierr << "SourceChGain value is not correct. Please have a check";
        }
    }
    if (Parameter("SourceChOffset")->NumValues() != numberOfSignalChannels)
        bcierr << "Length of SourceChOffset does not match channel number. Please have a check";
    else
    {
        for (int i = 0; i < numberOfSignalChannels; ++i)
        {
            if (Parameter("SourceChOffset")(i) != 0)
                bcierr << "SourceChOffset value is not correct. Please have a check";
        }
    }

    Output = SignalProperties(numberOfSignalChannels, samplesPerBlock, sigType);
}

void
NeuroSparkADC::OnInitialize(const SignalProperties& Output)
{
    mNumberOfSignalChannels = Parameter("SourceCh");
    mSampleBlockSize = Parameter("SampleBlockSize");
    mSampleRate = Parameter("SamplingRate");

    mpBuffer.resize(mSampleBlockSize * receiver->m_channel_num);
}

void
NeuroSparkADC::OnStartAcquisition()
{
    receiver->Initialize(receiver->m_total_length);
}

void
NeuroSparkADC::DoAcquire(GenericSignal& Output)
{
    int i = 0;
    int buff_size = receiver->m_total_length;
    char* packet = new char[buff_size];

    int time = Output.Elements() / receiver->m_data_count_per_channel;

    while (i < time)
    {
        int readsize = recv(receiver->neuro_spark, packet, buff_size, 0);
        float data = 0;
        int offset_size = 0;
        for (size_t channel = 0; channel < mNumberOfSignalChannels; ++channel)
        {
            for (size_t data_count = 0; data_count < receiver->m_data_count_per_channel; ++data_count)
            {
                memcpy(&data, packet + DATA_OFFSET + offset_size, LENGTH);
                int index = i * receiver->m_data_count_per_channel * mNumberOfSignalChannels + channel * receiver->m_data_count_per_channel + data_count;
                mpBuffer[index] = data;
                data = 0;
                offset_size += LENGTH;
            }
        }

        i++;
    }

    for (int ch = 0; ch < mNumberOfSignalChannels; ch++)
    {
        for (int sample = 0; sample < time; sample++)
        {
            for (int dcpc = 0; dcpc < receiver->m_data_count_per_channel; dcpc++)
            {
                Output(ch, sample * receiver->m_data_count_per_channel + dcpc) = mpBuffer[sample * mNumberOfSignalChannels * receiver->m_data_count_per_channel + dcpc + ch * receiver->m_data_count_per_channel];
            }
        }
    }
}

void
NeuroSparkADC::OnStopAcquisition()
{
    receiver->Close();
}
