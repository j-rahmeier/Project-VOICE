////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000FileReader.cpp 7769 2023-12-18 15:20:41Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: Class that provides an interface to the data stored in a
//              BCI2000 data file.
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
////////////////////////////////////////////////////////////////////////////////
#include "BCI2000FileReader.h"
#include "BCIException.h"
#include "Files.h"
#include "Streambuf.h"

#include <fstream>
#include <iostream>
#include <sstream>

struct BCI2000FileReader::Private
{
    Private();
    ~Private();
    void Reset();
    void ReadHeader(const char *);
    void CalculateNumSamples();
    const char *BufferSample(int64_t sample);

    ParamList mParamlist;
    StateList mStatelist;
    class StateVector *mpStatevector = nullptr;
    bool mInitialized = false;

    File *mpFile = nullptr;
    bool mFromFd = false;
    std::string mFilename, mFileFormatVersion;

    ::SignalProperties mSignalProperties;
    SignalType mSignalType = SignalType::defaultType;
    int mDataSize = 0;

    int mChannels = 0, mHeaderLength = 0, mStatevectorLength = 0;
    double mSamplingRate = 1;
    std::vector<GenericSignal::ValueType> mSourceOffsets, mSourceGains;

    uint64_t mNumSamples = 0;

    char *mpBuffer = nullptr;
    int mBufferSize = 0;
    int64_t mBufferBegin = 0, mBufferEnd = 0;

    int mErrorState = NoError;
};

BCI2000FileReader::Private::Private()
{
    mpFile = new File;
}

BCI2000FileReader::Private::~Private()
{
    Reset();
    delete mpFile;
}

void BCI2000FileReader::Private::Reset()
{
    mInitialized = false;

    mParamlist.Clear();
    mStatelist.Clear();
    delete mpStatevector;
    mpStatevector = NULL;

    mFilename = "";
    mpFile->Close();
    mFromFd = false;
    delete[] mpBuffer;
    mpBuffer = NULL;
    mBufferSize = 0;
    mBufferBegin = 0;
    mBufferEnd = 0;

    mFileFormatVersion = "n/a";
    mChannels = 0;
    mHeaderLength = 0;
    mStatevectorLength = 0;
    mSamplingRate = 1;
    mSignalType = SignalType::int16;
    mDataSize = mSignalType.Size();
    mSourceOffsets.clear();
    mSourceGains.clear();
    mNumSamples = 0;
    mSignalProperties = ::SignalProperties(0, 0, mSignalType);

    mErrorState = NoError;
}

template <typename T> static GenericSignal::ValueType ReadValue(const char *p)
{
    return *reinterpret_cast<const T *>(p);
}

template <typename T> static GenericSignal::ValueType ReadValue_SwapBytes(const char *p)
{
    static uint8_t buf[sizeof(T)];
    uint8_t *b = buf + sizeof(T);
    for (size_t i = 0; i < sizeof(T); ++i)
        *--b = *p++;
    return *reinterpret_cast<const T *>(reinterpret_cast<char *>(b));
}

BCI2000FileReader::BCI2000FileReader()
: p(new Private)
{
}

BCI2000FileReader::BCI2000FileReader(const char *inFileName)
: p(new Private)
{
    Open(inFileName);
}

BCI2000FileReader::BCI2000FileReader(const std::string& inFileName)
: p(new Private)
{
    Open(inFileName);
}

BCI2000FileReader::BCI2000FileReader(int fd)
: p(new Private)
{
    Open(fd);
}

BCI2000FileReader::~BCI2000FileReader()
{
    delete p;
}

BCI2000FileReader& BCI2000FileReader::Open(const std::string& fileName, int bufferSize, const std::string& prm)
{
    const char* pFileName = fileName.empty() ? nullptr : fileName.c_str();
    const char* pPrmFileName = prm.empty() ? nullptr : prm.c_str();
    return Open(pFileName, bufferSize, pPrmFileName);
}

BCI2000FileReader& BCI2000FileReader::Open(int fd, int bufferSize, const std::string& prm)
{
    const char* pPrmFileName = prm.empty() ? nullptr : prm.c_str();
    return Open(fd, bufferSize, pPrmFileName);
}

BCI2000FileReader& BCI2000FileReader::Close()
{
    return Open(nullptr);
}

BCI2000FileReader &BCI2000FileReader::Open(const char *inFilename, int inBufSize, const char *inPrmfile)
{
    Reset();

    if (inFilename != nullptr)
        p->mpFile->Open(inFilename, File::in);
    if (!p->mpFile->IsOpen())
    {
        p->mErrorState = FileOpenError;
    }
    if (ErrorState() == NoError)
    {
        if (inFilename == nullptr)
            p->mFilename.clear();
        else
            p->mFilename = inFilename;
        p->ReadHeader(inPrmfile);
        if (ErrorState() == NoError)
        {
            p->CalculateNumSamples();
            p->mBufferSize = inBufSize;
            p->mpBuffer = new char[p->mBufferSize];
            p->mBufferBegin = 0;
            p->mBufferEnd = 0;
            p->mInitialized = true;
        }
    }
    return *this;
}

BCI2000FileReader& BCI2000FileReader::Open(int inFd, int inBufSize, const char* inPrmfile)
{
    Reset();

    if (inFd >= 0)
    {
        p->mpFile->AttachToFd(inFd);
        p->mFromFd = true;
    }
    if (!p->mpFile->IsOpen())
    {
        p->mErrorState = FileOpenError;
    }
    if (ErrorState() == NoError)
    {
        p->mFilename = "<fd=" + std::to_string(inFd) + ">";
        p->ReadHeader(inPrmfile);
        if (ErrorState() == NoError)
        {
            p->CalculateNumSamples();
            p->mBufferSize = inBufSize;
            p->mpBuffer = new char[p->mBufferSize];
            p->mBufferBegin = 0;
            p->mBufferEnd = 0;
            p->mInitialized = true;
        }
    }
    return *this;
}

int BCI2000FileReader::ErrorState() const
{
    return p->mErrorState;
}

bool BCI2000FileReader::IsOpen() const
{
    return p->mInitialized;
}

int64_t BCI2000FileReader::NumSamples() const
{
    return p->mNumSamples;
}

double BCI2000FileReader::SamplingRate() const
{
    return p->mSamplingRate;
}

const class SignalProperties &BCI2000FileReader::SignalProperties() const
{
    return p->mSignalProperties;
}

const std::string &BCI2000FileReader::FileFormatVersion() const
{
    return p->mFileFormatVersion;
}

const ParamList *BCI2000FileReader::Parameters() const
{
    return &p->mParamlist;
}

const StateList *BCI2000FileReader::States() const
{
    return &p->mStatelist;
}

const class StateVector *BCI2000FileReader::StateVector() const
{
    return p->mpStatevector;
}

int BCI2000FileReader::HeaderLength() const
{
    return p->mHeaderLength;
}

int BCI2000FileReader::StateVectorLength() const
{
    return p->mStatevectorLength;
}

int BCI2000FileReader::SourceCh() const
{
    return p->mChannels;
}

SignalType BCI2000FileReader::DataFormat() const
{
    return p->mSignalType;
}
void BCI2000FileReader::Reset()
{
    p->Reset();
}

ParamRef BCI2000FileReader::Parameter(const std::string &path) const
{
    const Param *param = NULL;
    if (Parameters() != NULL && Parameters()->Exists(path))
        param = &Parameters()->ByPath(path);
    else
        throw std_range_error << "Parameter " << path << " does not exist";
    return ParamRef(param);
}

const StateRef BCI2000FileReader::State(const std::string &name) const
{
    const class State *pState = NULL;
    if (StateVector() != NULL && States() != NULL && States()->Exists(name))
    {
        pState = &States()->ByName(name);
        if (pState->Length() < 1)
            throw std_range_error << "Requested state " << name << " has zero length";
    }
    else
        throw std_runtime_error << "Requested state " << name << " is not accessible";

    return StateRef(const_cast<class State *>(pState), const_cast<class StateVector *>(StateVector()), 0);
}

GenericSignal::ValueType BCI2000FileReader::RawValue(int inChannel, int64_t inSample)
{
    GenericSignal::ValueType value = 0;
    const char *address = p->BufferSample(inSample) + p->mDataSize * inChannel;

    // When running on a big endian machine, we need to swap bytes.
    static const bool isBigEndian = (*reinterpret_cast<const uint16_t *>("\0\1") == 0x0001);
    if (isBigEndian)
    {
        switch (p->mSignalType)
        {
        case SignalType::int16:
            value = ReadValue_SwapBytes<int16_t>(address);
            break;
        case SignalType::int32:
            value = ReadValue_SwapBytes<int32_t>(address);
            break;
        case SignalType::float32:
            value = ReadValue_SwapBytes<float>(address);
            break;
        default:
            break;
        }
    }
    else
    { // Little endian machine
        switch (p->mSignalType)
        {
        case SignalType::int16:
            value = ReadValue<int16_t>(address);
            break;
        case SignalType::int32:
            value = ReadValue<int32_t>(address);
            break;
        case SignalType::float32:
            value = ReadValue<float>(address);
            break;
        default:
            break;
        }
    }
    return value;
}

GenericSignal::ValueType BCI2000FileReader::CalibratedValue(int inChannel, int64_t inSample)
{
    return (RawValue(inChannel, inSample) - p->mSourceOffsets[inChannel]) * p->mSourceGains[inChannel];
}

BCI2000FileReader &BCI2000FileReader::ReadStateVector(int64_t inSample)
{
    ::memcpy(p->mpStatevector->Data(), p->BufferSample(inSample) + p->mDataSize * SignalProperties().Channels(),
             p->mpStatevector->Length());
    return *this;
}

void BCI2000FileReader::Private::ReadHeader(const char *inPrmfile)
{
    // For unknown reasons, this stalls in ReadFile() when reading from a network share on Windows.
    // Typically, reading from a file descriptor is restricted to stdin though.
    BufferedIO buf;
    buf.SetInput(mpFile);
    std::istream file(&buf), *pFile = &file;
#if _WIN32 // This does not stall.
    std::ifstream file2;
    if (!mFromFd)
    {
      file2.open(mFilename);
      pFile = &file2;
    }
#endif

    mErrorState = MalformedHeader;

    try
    {
        // read the first line and do consistency checks
        std::string line, element;
        if (!std::getline(*pFile, line, '\n'))
            return;
        std::istringstream linestream(line);
        linestream >> element;
        if (element == "BCI2000V=")
            linestream >> mFileFormatVersion >> element;
        else
            mFileFormatVersion = "1.0";
        if (element != "HeaderLen=")
            return;

        linestream >> mHeaderLength >> element >> mChannels;
        if (element != "SourceCh=")
            return;

        linestream >> element >> mStatevectorLength;
        if (element != "StatevectorLen=")
            return;

        mSignalType = SignalType::int16;
        if (linestream >> element)
        {
            if (element != "DataFormat=")
                return;
            if (!(linestream >> mSignalType))
                return;
        }
        mDataSize = mSignalType.Size();

        // now go through the header and read all parameters and states
        std::getline(*pFile >> std::ws, line, '\n');
        if (line.find("[ State Vector Definition ]") != 0)
            return;
        while (std::getline(*pFile, line, '\n') && line.find("[ Parameter Definition ]") == line.npos)
            mStatelist.Add(line);
        while (std::getline(*pFile, line, '\n') && line != "" && line != "\r")
            mParamlist.Add(line);

        if (inPrmfile)
        {
            std::ifstream prm(inPrmfile, std::ios_base::in | std::ios_base::binary);
            while (std::getline(prm, line, '\n') && line != "" && line != "\r")
                mParamlist.Add(line);
        }
        // build statevector using specified positions
        mpStatevector = new (class StateVector)(mStatelist);
        if (mpStatevector->Length() > mStatevectorLength)
            throw std_runtime_error << "Inconsistent state vector length";
        if (!mParamlist.Exists("/SamplingRate"))
            return;
        std::string samplingRate = mParamlist.ByPath("/SamplingRate").Value();
        PhysicalUnit hz;
        hz.SetGain(1.0).SetOffset(0.0).SetSymbol("Hz");
        if (hz.IsPhysical(samplingRate))
            mSamplingRate = hz.PhysicalToRaw(samplingRate);
        else if (hz.IsPhysical(samplingRate + "Hz"))
            mSamplingRate = hz.PhysicalToRaw(samplingRate + "Hz");
        else
            mSamplingRate = ::atof(samplingRate.c_str());

        // Read information about signal dimensions.
        int sampleBlockSize = 1;
        if (mParamlist.Exists("/SampleBlockSize"))
            sampleBlockSize = static_cast<int>(PhysicalUnit().SetGain(1.0).SetOffset(0.0).SetSymbol("").PhysicalToRaw(
                mParamlist.ByPath("/SampleBlockSize").Value().c_str()));
        mSignalProperties = ::SignalProperties(mChannels, sampleBlockSize, mSignalType);
        mSignalProperties.ElementUnit().SetGain(1.0 / mSamplingRate).SetOffset(0.0).SetSymbol("s");

        const float defaultOffset = 0.0;
        mSourceOffsets.clear();
        if (mParamlist.Exists("/SourceChOffset"))
        {
            const Param& SourceChOffset = mParamlist.ByPath("/SourceChOffset");
            for (int i = 0; i < SourceChOffset.NumValues(); ++i)
                mSourceOffsets.push_back(::atof(SourceChOffset.Value(i).c_str()));
        }
        mSourceOffsets.resize(mChannels, defaultOffset);

        const double defaultGain = 0.033;
        mSourceGains.clear();
        if (mParamlist.Exists("/SourceChGain"))
        {
            const Param& SourceChGain = mParamlist.ByPath("/SourceChGain");
            // Rules for parsing gain values:
            // (1) If no unit is present, gain converts from raw to microvolts.
            // (2) If an SI unit prefix (mu, k, etc) is present, it is multiplied into
            //     the gain.
            // (3) After calibration, values are in a factor of 1e-6 of their units, for
            //     compatibility with microvolt values.
            for (int i = 0; i < std::min(SourceChGain.NumValues(), mChannels); ++i)
            {
                PhysicalUnit unit;
                unit.SetGainWithSymbol(SourceChGain.Value(i)); // (2)
                if (unit.Symbol().empty()) // (1)
                    unit.SetSymbol("V").SetGain(unit.Gain() * 1e-6);
                mSourceGains.push_back(unit.Gain() * 1e6); // (3)
                unit.SetGain(1e-6); // (3)
                mSignalProperties.ValueUnit(i) = unit;
            }
        }
        mSourceGains.resize(mChannels, defaultGain);

        if (mParamlist.Exists("/ChannelNames"))
        {
            const Param& ChannelNames = mParamlist.ByPath("/ChannelNames");
            for (int i = 0; i < std::min(mChannels, ChannelNames.NumValues()); ++i)
                mSignalProperties.ChannelLabels()[i] = ChannelNames.Value(i);
        }

        if (*pFile)
            mErrorState = NoError;
    }
    catch (const std::exception&)
    {
    }
    catch (const Exception&)
    {
    }
}

void BCI2000FileReader::Private::CalculateNumSamples()
{
    int64_t dataSize = mpFile->Length();
    Assert(dataSize > 0);
    dataSize -= mHeaderLength;
    mNumSamples = dataSize / (mDataSize * mChannels + mStatevectorLength);
}

const char *BCI2000FileReader::Private::BufferSample(int64_t inSample)
{
    if (inSample >= mNumSamples)
        throw std_range_error << "Sample position " << inSample << " exceeds file size of " << mNumSamples
                              << " samples";
    int numChannels = mSignalProperties.Channels();
    int64_t filepos = mHeaderLength + inSample * (mDataSize * numChannels + mStatevectorLength);
    if (filepos < mBufferBegin || filepos + mDataSize * numChannels + mStatevectorLength >= mBufferEnd)
    {
        if (mpFile->SeekTo(filepos) != filepos)
            throw std_runtime_error << "Could not seek to sample position";

        mBufferBegin = filepos;
        mBufferEnd = mBufferBegin;
        while (!mpFile->Eof() && mpFile->Good() && mBufferEnd - mBufferBegin < mStatevectorLength)
        {
            size_t bytesRead =
                mpFile->Read(mpBuffer + (mBufferEnd - mBufferBegin), mBufferSize - (mBufferEnd - mBufferBegin));
            mBufferEnd += bytesRead;
        }
        mpFile->ClearIOState();
    }
    return mpBuffer + (filepos - mBufferBegin);
}
