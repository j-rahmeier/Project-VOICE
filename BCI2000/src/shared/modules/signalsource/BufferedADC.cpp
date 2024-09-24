////////////////////////////////////////////////////////////////////////////////
// $Id: BufferedADC.cpp 7520 2023-08-04 17:40:46Z mellinger $
// Description: BufferedADC is a base class for signal source filters that
//   provides buffering for data packets read from the ADC, to avoid data
//   loss when data isn't read timely enough.
//   See the accompagnying header file for more information.
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
#include "BufferedADC.h"
#include "BCIStream.h"
#include "Lockable.h"
#include "PrecisionTime.h"
#include "Thread.h"
#include "WaitableEvent.h"

struct BufferedADC::BufferedADCPrivate : Thread
{
    BufferedADCPrivate(BufferedADC *);
    ~BufferedADCPrivate();

    void StartAcquisition();
    void StopAcquisition();
    int OnExecute();

    BufferedADC *mpSelf;
    mutable SignalProperties mAcquisitionProperties;
    std::atomic<bool> mStarted = false;
    bool mFirstBlock, mOverflowReported;
    Time::Interval mBlockDuration;
    SynchronizedObject<Time::Interval> mAcquisitionTimeout, mInitialAcquisitionTimeout;
    struct AcquisitionBuffer
    {
        GenericSignal Signal;
        PrecisionTime TimeStamp;
    };
    std::vector<AcquisitionBuffer> mBuffers;
    AcquisitionBuffer *mpLockedBuffer;

    struct BufferState
    {
        int readPos, used;
    };
    SynchronizedObject<BufferState> mBufferState;
    WaitableEvent mDataAvailable;
    std::vector<StateRef> mStreams;
};

BufferedADC::BufferedADCPrivate::BufferedADCPrivate(BufferedADC *pSelf)
    : mpSelf(pSelf), mBuffers(0), mpLockedBuffer(0), mFirstBlock(false), mOverflowReported(false), mDataAvailable(false)
{
    WithLocked(p = mBufferState.Mutable())
    {
        p->readPos = 0;
        p->used = 0;
    }
}

BufferedADC::BufferedADCPrivate::~BufferedADCPrivate()
{
}

BufferedADC::BufferedADC()
    : d(new BufferedADCPrivate(this))
{
  BEGIN_PARAMETER_DEFINITIONS
    "Source:Buffering int SourceBufferSize= 2s "
        "2s % % // size of data acquisition ring buffer (in blocks or seconds)",
  END_PARAMETER_DEFINITIONS
}

BufferedADC::~BufferedADC()
{
    BufferedADC::Halt();
    delete d;
}

void BufferedADC::SetInitialAcquisitionTimeout(const Time::Interval &timeout)
{
    WithLocked(p = d->mInitialAcquisitionTimeout.Mutable())
        *p = timeout;
}

const Time::Interval BufferedADC::InitialAcquisitionTimeout() const
{
    Time::Interval timeout;
    WithLocked(p = d->mInitialAcquisitionTimeout.Const())
        timeout = *p;
    return timeout;
}

void BufferedADC::SetAcquisitionTimeout(const Time::Interval& timeout)
{
    WithLocked(p = d->mInitialAcquisitionTimeout.Mutable())
        * p = timeout;
}

const Time::Interval BufferedADC::AcquisitionTimeout() const
{
    Time::Interval timeout;
    WithLocked(p = d->mInitialAcquisitionTimeout.Const())
        timeout = *p;
    return timeout;
}

void BufferedADC::AutoConfig(const SignalProperties &)
{
    this->OnAutoConfig();
}

void BufferedADC::Preflight(const SignalProperties &, SignalProperties &Output) const
{
    State("SourceTime");
    State("Running");
    Parameter("SourceBufferSize");
    d->mAcquisitionProperties = Output;
    this->OnPreflight(d->mAcquisitionProperties);
    Output = d->mAcquisitionProperties;
    int numStateChannels = 0;
    for (int ch = 0; ch < Output.Channels(); ++ch)
    {
        bool isStateChannel = (*Output.ChannelLabels()[ch].c_str() == StateMark);
        if (numStateChannels && !isStateChannel)
            bcierr_ << "State channels must be located at the end of the channel list";
        else if (isStateChannel)
            ++numStateChannels;
    }
    Output.SetChannels(Output.Channels() - numStateChannels);
}

void BufferedADC::Initialize(const SignalProperties &, const SignalProperties &Output)
{
    WithLocked(p = d->mBufferState.Mutable())
    {
        p->readPos = 0;
        p->used = 0;
    }
    d->mOverflowReported = false;
    d->mDataAvailable.Reset();

    d->mBuffers.clear();
    SetInitialAcquisitionTimeout(Time::Seconds(1.5));
    int sourceBufferSize = Parameter("SourceBufferSize").InSampleBlocks();
    sourceBufferSize = std::max(2, sourceBufferSize);
    d->mBlockDuration = Time::Seconds(1.0 / Output.UpdateRate());
    SetAcquisitionTimeout(2 * sourceBufferSize * d->mBlockDuration);
    d->mBuffers.resize(sourceBufferSize);
    for (size_t i = 0; i < sourceBufferSize; ++i)
    {
        d->mBuffers[i].Signal.SetProperties(d->mAcquisitionProperties);
        d->mBuffers[i].Signal.SetAllValues(GenericSignal::NaN);
        d->mBuffers[i].TimeStamp = 0;
    }
    this->OnInitialize(d->mAcquisitionProperties);

    d->mStreams.clear();
    const LabelIndex &labels = d->mAcquisitionProperties.ChannelLabels();
    for (int ch = Output.Channels(); ch < d->mAcquisitionProperties.Channels(); ++ch)
        d->mStreams.push_back(State(labels[ch].c_str() + 1));

    if (bcierr__.Empty())
        d->StartAcquisition();
    d->mFirstBlock = true;
}

// The Process() function is called from the main thread in regular intervals.
void BufferedADC::Process(const GenericSignal &, GenericSignal &Output)
{
    this->OnProcess();

    int readPos = -1;
    WithLocked(p = d->mBufferState.Mutable())
    {
        if (p->used <= d->mBuffers.size())
        {
            readPos = p->readPos;
            d->mOverflowReported = false;
        }
        else
        {
            readPos = -1;
            p->used = 0;
            p->readPos = 0;
        }
    }
    if (readPos < 0)
    {
        if (!d->mOverflowReported)
        {
            d->mOverflowReported = true;
            int bufferCount = d->mBuffers.size();
            double bufferDuration = bufferCount / Output.Properties().UpdateRate();
            StringUtils::String message;
            message << "Write buffer overflow, data loss occurred (current buffer size is " << bufferCount
                    << " blocks == " << bufferDuration << "s), inserting NaNs";
            if (bufferCount < 3 || bufferDuration < 1)
                message += " -- Setting SourceBufferSize to a larger value may help";
            else
                message += " -- SourceBufferSize is reasonably large, processing may be too slow to keep up with "
                           "incoming data";
            bciwarn << message;
        }
        Output = GenericSignal(Output.Properties(), NaN<GenericSignal::ValueType>());
        d->mDataAvailable.Wait(d->mBlockDuration);
    }
    else
        WithThreadPriority(ThreadUtils::Priority::Maximum - 1)
        {
            auto timeout = d->mFirstBlock ? InitialAcquisitionTimeout() : AcquisitionTimeout();
            bool gotData = d->mDataAvailable.Wait(timeout);

            if (State("Running"))
                State("Running") = gotData;
            if (!gotData)
                bciwarn << "Data acquisition timeout";

            d->mFirstBlock = false;
            BufferedADCPrivate::AcquisitionBuffer &buffer = d->mBuffers[readPos];
            if (buffer.Signal.Channels() == Output.Channels())
                Output.AssignValues(buffer.Signal);
            else
            {
                for (int ch = 0; ch < Output.Channels(); ++ch)
                    for (int el = 0; el < Output.Elements(); ++el)
                        Output(ch, el) = buffer.Signal(ch, el);

                for (int ch = Output.Channels(); ch < buffer.Signal.Channels(); ++ch)
                    for (int el = 0; el < Output.Elements(); ++el)
                        d->mStreams[ch - Output.Channels()].Sample(el) = Round(buffer.Signal(ch, el));
            }
            State("SourceTime") = buffer.TimeStamp;
        }
    WithLocked(p = d->mBufferState.Mutable())
    {
        ++p->readPos %= d->mBuffers.size();
        if (--p->used < 1)
            d->mDataAvailable.Reset();
    }
}

void BufferedADC::Halt()
{
    d->StopAcquisition();
    OnHalt();
}

int BufferedADC::BufferedADCPrivate::OnExecute()
{
    ThreadUtils::Priority::Set(ThreadUtils::Priority::Maximum - 1);
    bool started = false;
    try
    {
        mpSelf->OnStartAcquisition();
        started = bcierr__.Empty();
        while (bcierr__.Empty() && !Thread::Terminating())
        {
            GenericSignal *p = mpSelf->LockWriteBuffer();
            mpSelf->DoAcquire(*p);
            mpSelf->ReleaseWriteBuffer(p);
        }
    }
    catch (const BCIStream::OutStream::Ref &r)
    {
        if (!r.RefersTo(bcierr__))
            throw;
    }
    catch (const Exception& e)
    {
        bcierr__ << e.Where() << ": " << e.What();
    }
    try
    {
        if (started)
            mpSelf->OnStopAcquisition();
    }
    catch (const BCIStream::OutStream::Ref &r)
    {
        if (!r.RefersTo(bcierr__))
            throw;
    }
    return 0;
}

GenericSignal *BufferedADC::LockWriteBuffer()
{
    Assert(d->mpLockedBuffer == 0);
    WithLocked(p = d->mBufferState.Const())
    {
        int writePos = (p->readPos + p->used) % d->mBuffers.size();
        d->mpLockedBuffer = &d->mBuffers[writePos];
    }
    return &d->mpLockedBuffer->Signal;
}

void BufferedADC::ReleaseWriteBuffer(GenericSignal *pSignal)
{
    Assert(d->mpLockedBuffer && pSignal == &d->mpLockedBuffer->Signal);
    d->mpLockedBuffer->TimeStamp = PrecisionTime::Now();
    d->mpLockedBuffer = 0;
    WithLocked(p = d->mBufferState.Mutable())
    {
        ++p->used;
        d->mDataAvailable.Set();
    }
}

void BufferedADC::BufferedADCPrivate::StartAcquisition()
{
    bool pValue = false;
    if (mStarted.compare_exchange_strong(pValue, true))
    {
        if (mpSelf->UseAcquisitionThread())
            Thread::Start();
        else
            mpSelf->OnStartAcquisition();
    }
}

void BufferedADC::BufferedADCPrivate::StopAcquisition()
{
    bool pValue = true;
    if (mStarted.compare_exchange_strong(pValue, false))
    {
        if (mpSelf->UseAcquisitionThread())
            Thread::Terminate();
        else
            mpSelf->OnStopAcquisition();
    }
}
