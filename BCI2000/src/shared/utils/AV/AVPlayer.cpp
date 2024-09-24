////////////////////////////////////////////////////////////////////////////////
// $Id: AVPlayer.cpp 7955 2024-03-23 17:22:33Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A base class for video, audio, and other media playing
//   components.
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
#include "AVPlayer.h"

#include "AVDecoder.h"
#include "ClockTick.h"
#include "FileUtils.h"
#include "ThreadUtils.h"
#include "Lockable.h"
#include "SynchronizedObject.h"
#include "SynchronizedQueue.h"

#include <algorithm>
#include <list>

enum
{
    blocked = 0,
    playing,
    playingAtEnd,
    paused
};

namespace
{

struct PlayerSet : SynchronizedObject<std::list<AVPlayer *>, std::recursive_mutex>
{
    bool Add(AVPlayer *p)
    {
        if (p)
            WithLocked(t = this->Mutable())
            {
                if (std::find(t->begin(), t->end(), p) == t->end())
                {
                    t->push_back(p);
                    return true;
                }
            }
        return false;
    }
    bool Remove(const AVPlayer *p)
    {
        WithLocked(t = this->Mutable())
        {
            auto i = std::find(t->begin(), t->end(), p);
            if (i != t->end())
            {
                t->erase(i);
                return true;
            }
        }
        return false;
    }
};

struct TaskQueue : Thread, SynchronizedQueue<Runnable *>
{
    TaskQueue()
    {
        Thread::Start();
    }
    ~TaskQueue()
    {
        TerminateAndWait();
    }
    int OnExecute() override
    {
        ThreadUtils::Priority::Set(ThreadUtils::Priority::Maximum - 1);
        while (SynchronizedQueue<Runnable*>::Wait())
        {
            SynchronizedQueue<Runnable *>::Consumable task;
            while ((task = Consume()))
            {
                SyncMemory<BeforeRead>();
                (*task)->Run();
                delete *task;
                SyncMemory<AfterWrite>();
            }
        }
        return 0;
    }
};

} // namespace

struct AVPlayer::Data : Lockable<std::mutex>
{
    Data(AVPlayer *);
    ~Data();
    void Delete();
    void CloseFile();
    void AdaptFrameRate();

    int64_t DecoderTickTarget(Time) const;
    int64_t DecoderTickToFrame(int64_t) const;
    int64_t FrameToDecoderTick(int64_t) const;
    Time::Interval PlaybackLatency() const;
    void SeekToDecoderTick(int64_t);

    void OnStateChange();
    std::atomic<int> mState{ 0 };
    std::atomic<Runnable *> mpOnStateChange{ nullptr };
    std::atomic<bool> mSettingFile, mSeeking;

    void AsyncFunctionCall(void (Data::*)());
    template <typename T> const char *BroadcastFunctionCall(AVPlayer &(AVPlayer::*)(T), T);
    const char *BroadcastFunctionCall(AVPlayer &(AVPlayer::*)());
    const char *BroadcastConfiguration();
    const char *BroadcastPlaybackTick(const ClockTick &);

    AVPlayer *PlayerWithCoarsestTimeResolution() const;
    AVPlayer *PlayerWithGreatestLatency() const;

    PlayerSet mTiedPartners, mClockSlaves;

    AVPlayer *mpSelf;
    std::atomic<AVPlayer *> mpClockMaster{ nullptr };

    double mFrameRateAccuracy;
    Rate mNominalFrameRate, mActualFrameRate;
    Ratio mInputFramesPerDecoderTick;
    StringUtils::String mFile, mFileCanonical, mContainer, mCodec, mDeviceInfo, mError;

    ClockTick mRenderingTick, mPlaybackTick, mStartDecoderTick;
    Time mStartTime;
    struct PlayPause
    {
        Time play, pause;
    };
    SynchronizedObject<PlayPause> mPlayPause;
    Time::Interval mInternalLatency, mExternalLatency;
    AVDecoder mDecoder;

    static TaskQueue &Tasks();
};

// Data
AVPlayer::Data::Data(AVPlayer *p)
    : mpSelf(p), mpClockMaster(p), mState(blocked), mInternalLatency(0), mExternalLatency(0),
      mRenderingTick(ClockTick::Null), mPlaybackTick(ClockTick::Null), mStartDecoderTick(ClockTick::Null),
      mStartTime(Time::Never()), mInputFramesPerDecoderTick(1, 1), mSettingFile(false), mSeeking(false)
{
    WithLocked(p = mPlayPause.Mutable())
    {
        p->play = Time::Never();
        p->pause = Time::Never();
    }
}

AVPlayer::Data::~Data()
{
}

void AVPlayer::Data::Delete()
{
    delete this;
}

void AVPlayer::Data::CloseFile()
{
    mFile = "";
    mCodec = "";
    mContainer = "";
    mState = blocked;
    mInputFramesPerDecoderTick = Ratio(1, 1);
    mDecoder.Close();
    mStartTime = Time::Never();
    WithLocked(p = mPlayPause.Mutable())
    {
        p->play = Time::Never();
        p->pause = Time::Never();
    }
}

void AVPlayer::Data::AsyncFunctionCall(void (Data::*func)())
{
    mpSelf->AbsorbIntoTaskQueue(new MemberCall<void(Data *)>(func, this));
}

template <typename T> const char *AVPlayer::Data::BroadcastFunctionCall(AVPlayer &(AVPlayer::*func)(T), T t)
{
    WithLocked(p = mTiedPartners.Const())
        if (func)
            for (auto i = p->begin(); i != p->end(); ++i)
                ((*i)->*func)(t);
    return nullptr;
}

const char *AVPlayer::Data::BroadcastFunctionCall(AVPlayer &(AVPlayer::*func)())
{
    WithLocked(p = mTiedPartners.Const())
    {
        if (func)
            for (auto i = p->begin(); i != p->end(); ++i)
                ((*i)->*func)();
    }
    return nullptr;
}

const char *AVPlayer::Data::BroadcastConfiguration()
{
    WithLocked(p = mTiedPartners.Const())
      for (auto i = p->begin(); i != p->end(); ++i)
        (*i)->SetNominalFrameRate((*i)->NativeFrameRate() * (mpSelf->FrameRate() / mpSelf->NativeFrameRate()));
    return BroadcastFunctionCall<Time::Interval>(&AVPlayer::SeekTo, mpSelf->TimePosition());
}

AVPlayer *AVPlayer::Data::PlayerWithCoarsestTimeResolution() const
{
    AVPlayer *p = mpSelf;
    WithLocked(t = mTiedPartners.Const())
        for (auto i = t->begin(); i != t->end(); ++i) 
            if ((*i)->RenderingTick().rate < p->RenderingTick().rate) p = *i;
    return p;
}

AVPlayer *AVPlayer::Data::PlayerWithGreatestLatency() const
{
    AVPlayer *p = mpSelf;
    WithLocked(t = mTiedPartners.Const())
        for (auto i = t->begin(); i != t->end(); ++i)
            if ((*i)->PlaybackLatency() > p->PlaybackLatency()) p = *i;
    return p;
}

const char *AVPlayer::Data::BroadcastPlaybackTick(const ClockTick &inTick)
{
    WithLocked(p = mClockSlaves.Const())
      for (auto i = p->begin(); i != p->end(); ++i)
        (*i)->SetPlaybackTick(inTick);
    return 0;
}

void AVPlayer::Data::OnStateChange()
{
    Runnable *pOnStateChange = mpOnStateChange;
    if (pOnStateChange)
        pOnStateChange->Run();
}

int64_t AVPlayer::Data::DecoderTickTarget(Time t) const
{
    // NB: Multiply ratios and rate in proper order to avoid overflow.
    return mStartDecoderTick.count +
           Round((t - mStartTime) * (mStartDecoderTick.rate * (mActualFrameRate / mDecoder.InputFrameRate())));
}

void AVPlayer::Data::AdaptFrameRate()
{
    Rate outputFrameRate = mNominalFrameRate * mDecoder.ResamplingRatio();
    if (!IsNaN(outputFrameRate) && !IsNaN(mRenderingTick.rate))
    {
        Ratio r = outputFrameRate / mRenderingTick.rate, adaptation = r.Simplified(mFrameRateAccuracy) / r;
        mActualFrameRate = mNominalFrameRate * adaptation;
    }
}

int64_t AVPlayer::Data::FrameToDecoderTick(int64_t frame) const
{
    return Floor(frame / mInputFramesPerDecoderTick);
}

int64_t AVPlayer::Data::DecoderTickToFrame(int64_t tick) const
{
    return Floor(tick * mInputFramesPerDecoderTick);
}

void AVPlayer::Data::SeekToDecoderTick(int64_t tick)
{
    int oldState = mState.exchange(blocked);
    WithLocked(p = mPlayPause.Mutable())
    {
        p->play = Time::Never();
        p->pause = Time::Never();
    }
    StringUtils::String err;
    bool expected = false;
    if (mSeeking.compare_exchange_strong(expected, true))
    {
        err = mDecoder.SeekToTick(tick).Error();
        if (err.Empty())
            err = BroadcastFunctionCall<Time::Interval>(&AVPlayer::SeekTo, mpSelf->TimePosition());
        mSeeking = false;
    }
    if (!err.Empty())
        mpSelf->SetError(err);
    else
        mState = paused;
    if (oldState != paused)
        OnStateChange();
}

Time::Interval AVPlayer::Data::PlaybackLatency() const
{
    return mInternalLatency + mExternalLatency;
}

TaskQueue &AVPlayer::Data::Tasks()
{
    static TaskQueue sTasks;
    return sTasks;
}

// AVPlayer
AVPlayer::AVPlayer() : d(new Data(this))
{
    SetFrameRateAccuracy(FrameRateAccuracyDefault());
}

AVPlayer::~AVPlayer()
{
    Assert(!IsPlaying());
    WithLocked(p = d->mClockSlaves.Const())
      for (auto i = p->begin(); i != p->end(); ++i)
        (*i)->SetClockMaster(nullptr);

    SetClockMaster(nullptr);

    std::list<AVPlayer*> tiedPartners;
    WithLocked(p = d->mTiedPartners.Const())
      tiedPartners = *p;
    for (auto i = tiedPartners.begin(); i != tiedPartners.end(); ++i)
      Untie(*i);

    d->AsyncFunctionCall(&AVPlayer::Data::Delete);
}

const StringUtils::String &AVPlayer::Error() const
{
    SyncMemory<BeforeRead>();
    if (d->mError.Empty() && !d->mDecoder.Error().Empty())
        d->mError = d->mDecoder.Error();
    return d->mError;
}

AVPlayer &AVPlayer::SetError(const std::string &s)
{
    if (!s.empty())
    {
        WithLock(d) d->mError = s;
        d->AsyncFunctionCall(&Data::OnStateChange);
    }
    return *this;
}

const StringUtils::String &AVPlayer::File() const
{
    SyncMemory<BeforeRead>();
    return d->mFile;
}

const StringUtils::String &AVPlayer::Codec() const
{
    SyncMemory<BeforeRead>();
    return d->mCodec;
}

const StringUtils::String &AVPlayer::DeviceInfo() const
{
    SyncMemory<BeforeRead>();
    return d->mDeviceInfo;
}

const StringUtils::String &AVPlayer::ContainerFormat() const
{
    SyncMemory<BeforeRead>();
    return d->mContainer;
}

const Rate &AVPlayer::NativeFrameRate() const
{
    SyncMemory<BeforeRead>();
    return d->mDecoder.InputFrameRate();
}

Rate AVPlayer::FrameRate() const
{
    SyncMemory<BeforeRead>();
    return d->mActualFrameRate;
}

AVPlayer &AVPlayer::SetNominalFrameRate(const Rate &inRate)
{
    WithLock(d)
    {
        Ratio relativeChange = inRate / d->mNominalFrameRate;
        d->mActualFrameRate = d->mActualFrameRate * relativeChange;
        d->mNominalFrameRate = inRate;
        Ratio resamplingRatio = d->mDecoder.ResamplingRatio();
        resamplingRatio = resamplingRatio / relativeChange;
        d->mDecoder.BeginConfiguration();
        d->mDecoder.SetResamplingRatio(resamplingRatio);
        d->mDecoder.EndConfiguration();
    }
    return *this;
}

const Rate &AVPlayer::NominalFrameRate() const
{
    SyncMemory<BeforeRead>();
    return d->mNominalFrameRate;
}

AVPlayer &AVPlayer::SetFrameRateAccuracy(double acc)
{
    acc = std::max(acc, 0.0);
    acc = std::min(acc, FrameRateAccuracyMaximum());
    WithLock(d)
    {
        d->mFrameRateAccuracy = acc;
        d->AdaptFrameRate();
    }
    return *this;
}

double AVPlayer::FrameRateAccuracy() const
{
    SyncMemory<BeforeRead>();
    return d->mFrameRateAccuracy;
}

ClockTick AVPlayer::PlaybackTick() const
{
    SyncMemory<BeforeRead>();
    return d->mPlaybackTick;
}

ClockTick AVPlayer::RenderingTick() const
{
    SyncMemory<BeforeRead>();
    return d->mRenderingTick;
}

int64_t AVPlayer::FramePosition() const
{
    return d->DecoderTickToFrame(Decoder().HeadFrameTick());
}

Time::Interval AVPlayer::TimePosition() const
{
    return FramePosition() / Decoder().InputFrameRate();
}

Time::Interval AVPlayer::PlaybackLatency() const
{
    return d->PlaybackLatency();
}

AVPlayer &AVPlayer::SetInternalPlaybackLatency(Time::Interval t)
{
    Assert(t >= 0);
    d->mInternalLatency = t;
    return *this;
}

AVPlayer &AVPlayer::SetExternalPlaybackLatency(Time::Interval t)
{
    if (t < 0)
        SetError("ExternalPlaybackLatency must be >= 0");
    else
        d->mExternalLatency = t;
    return *this;
}

AVPlayer &AVPlayer::PlayAt(Time t)
{
    if (!IsPlaying())
    {
        AVPlayer *pCoarse = d->PlayerWithCoarsestTimeResolution(), *pLatent = d->PlayerWithGreatestLatency();
        Time earliestPossible = pLatent->RenderingTick().time + pLatent->RenderingTick().rate.Period();
        earliestPossible = std::max(t, earliestPossible);
        Time closestCoarse = earliestPossible;
        if (pCoarse != pLatent)
            closestCoarse = pCoarse->RenderingTick().time +
                            Round((earliestPossible - pCoarse->RenderingTick().time) * pCoarse->RenderingTick().rate) /
                                pCoarse->RenderingTick().rate;

        WithLocked(p = d->mTiedPartners.Const())
            for (auto i = p->begin(); i != p->end(); ++i)
                (*i)->d->mPlayPause.Mutable()->play = closestCoarse;
        d->mPlayPause.Mutable()->play = closestCoarse;
    }
    return *this;
}

AVPlayer &AVPlayer::PauseAt(Time t)
{
    if (IsPlaying())
        d->mPlayPause.Mutable()->pause = t;
    return *this;
}

AVPlayer &AVPlayer::Play()
{
    return PlayAt(RenderingTick().time - RenderingTick().rate.Period() / 2);
}

AVPlayer &AVPlayer::Pause()
{
    return PauseAt(RenderingTick().time - RenderingTick().rate.Period() / 2);
}

AVPlayer &AVPlayer::Stop()
{
    SeekTo(0);
    return *this;
}

int64_t AVPlayer::FrameCount() const
{
    if (!IsOpen())
        return 0;
    if (Decoder().EofPosition().count <= 0)
        return Maximum<int64_t>();
    return d->DecoderTickToFrame(Decoder().EofPosition().count);
}

Time::Interval AVPlayer::NativeDuration() const
{
    if (!IsOpen())
        return 0;
    if (Decoder().EofPosition().count <= 0)
        return Time::Forever();
    return FrameCount() / Decoder().InputFrameRate();
}

AVPlayer &AVPlayer::SeekTo(int64_t inNativeFrame)
{
    d->SeekToDecoderTick(d->FrameToDecoderTick(inNativeFrame));
    return *this;
}

AVPlayer &AVPlayer::SeekTo(Time::Interval t)
{
    d->SeekToDecoderTick(Round(t * Decoder().BofPosition().rate));
    return *this;
}

bool AVPlayer::IsPlaying() const
{
    switch (d->mState.load())
    {
    case playing:
    case playingAtEnd:
        return true;
    }
    return false;
}

bool AVPlayer::IsOpen() const
{
    return !d->mFile.Empty();
}

bool AVPlayer::Eof() const
{
    return Decoder().EndOfData();
}

AVPlayer &AVPlayer::SpendDecodingEffort()
{
    d->mDecoder.DecodeToBuffer();
    return *this;
}

AVPlayer &AVPlayer::LoadIntoMemory()
{
    d->mDecoder.DecodeAllIntoMemory();
    return *this;
}

Runnable *AVPlayer::OnStateChange() const
{
    return d->mpOnStateChange;
}

AVPlayer &AVPlayer::SetOnStateChange(Runnable *p)
{
    d->mpOnStateChange = p;
    return *this;
}

AVPlayer &AVPlayer::SetFile(const std::string &s)
{
    bool expected = false;
    if (d->mSettingFile.compare_exchange_strong(expected, true))
    {
        std::string fileCanonical = s.empty() ? "" : FileUtils::CanonicalPath(s);
        if (!FileUtils::IsFile(fileCanonical))
            fileCanonical.clear();
        if (!s.empty() && fileCanonical.empty()) {
          d->mSettingFile = false;
          return SetError("File \"" + s + "\" does not exist");
        }

        if (d->mState != blocked && d->mFileCanonical == fileCanonical && Error().Empty()) {
          d->mSettingFile = false;
          return *this;
        }

        d->mState = blocked;
        d->mFile = "";
        d->mFileCanonical = "";
        d->mError.Clear();
        if (s.empty())
        {
            d->AsyncFunctionCall(&Data::OnStateChange);
            d->mSettingFile = false;
            return *this;
        }

        StringUtils::String err = OnSetFile(s);
        if (err.Empty())
        {
            d->mFile = s;
            d->mFileCanonical = fileCanonical;
            d->mNominalFrameRate = Decoder().InputFrameRate();
            d->mActualFrameRate = d->mNominalFrameRate;
            d->mInputFramesPerDecoderTick = Decoder().InputFrameRate() / Decoder().BofPosition().rate;
            d->mInputFramesPerDecoderTick.Normalize();
            d->mContainer = Decoder().ContainerFormat();
            d->mCodec = Decoder().Codec();
            Decoder().SeekToTick(0);
            Decoder().DecodeToBuffer();
            err = Decoder().Error();
        }
        if (err.Empty())
            err = OnGetDeviceInfo(d->mDeviceInfo);
        if (err.Empty())
        {
            d->mState = paused;
            d->BroadcastFunctionCall<const std::string &>(&AVPlayer::SetFile, d->mFile);
            d->BroadcastConfiguration();
            d->AsyncFunctionCall(&Data::OnStateChange);
        }
        else
        {
            SetError("Could not open media file \"" + s + "\": " + err);
            d->mFile = "";
            d->mFileCanonical = "";
        }
        d->mSettingFile = false;
    }
    return *this;
}

AVPlayer &AVPlayer::SetPlaybackTick(const ClockTick &t)
{
    const char *pErr = 0;
    bool changed = (IsNaN(d->mPlaybackTick.rate) && !IsNaN(t.rate)) || d->mPlaybackTick.rate != t.rate;
    d->mPlaybackTick = t;
    SyncMemory<AfterWrite>();
    pErr = OnPlaybackTick(d->mPlaybackTick, changed);
    if (pErr)
        SetError(pErr);
    else
        pErr = d->BroadcastPlaybackTick(d->mPlaybackTick);

    if (IsPlaying())
        pErr = OnAdvance(d->DecoderTickTarget(d->mPlaybackTick.time) - Decoder().HeadTick());
    if (pErr)
        SetError(pErr);
    return *this;
}

AVPlayer &AVPlayer::SetRenderingTick(const ClockTick &t)
{
    const char *pErr = 0;
    bool changed = (IsNaN(d->mRenderingTick.rate) && !IsNaN(t.rate)) || d->mRenderingTick.rate != t.rate;
    d->mRenderingTick = t;
    d->mRenderingTick.time += d->mExternalLatency;
    if (changed)
    {
        d->AdaptFrameRate();
        pErr = d->BroadcastConfiguration();
    }
    enum
    {
        nothing,
        startPlayback,
        pausePlayback
    } action = nothing;
    WithLocked(p = d->mPlayPause.Mutable())
    {
        if (d->mRenderingTick.time >= p->play && d->mRenderingTick.time >= p->pause)
        {
            if (p->play > p->pause)
                action = startPlayback;
            else
                action = pausePlayback;
            p->play = Time::Never();
            p->pause = Time::Never();
        }
        else if (d->mRenderingTick.time >= p->play)
        {
            action = startPlayback;
            p->play = Time::Never();
        }
        else if (d->mRenderingTick.time >= p->pause)
        {
            action = pausePlayback;
            p->pause = Time::Never();
        }
    }
    int value = paused;
    if (action == startPlayback && d->mState.compare_exchange_strong(value, playing))
    {
        d->mStartTime = d->mRenderingTick.time;
        d->mStartDecoderTick = d->mDecoder.HeadPosition();
        pErr = OnAdvance(d->DecoderTickTarget(PlaybackTick().time) - Decoder().HeadTick());
        d->AsyncFunctionCall(&Data::OnStateChange);
    }
    if (d->mpClockMaster == this)
        SetPlaybackTick(d->mRenderingTick);

    if (!pErr)
        pErr = OnRenderingTick(d->mRenderingTick, changed);
    if (pErr)
        SetError(pErr);

    value = playingAtEnd;
    if (d->mState.compare_exchange_strong(value, paused))
    {
        d->mPlayPause.Mutable()->pause = Time::Never();
        d->AsyncFunctionCall(&Data::OnStateChange);
    }
    else if (d->mDecoder.EndOfData()) {
        value = playing;
        d->mState.compare_exchange_strong(value, playingAtEnd);
    }

    value = playing;
    if (action == pausePlayback && d->mState.compare_exchange_strong(value, paused))
    {
        AVPlayer *p = d->PlayerWithCoarsestTimeResolution();
        p->AsyncFunctionCall<int64_t>(&AVPlayer::SeekTo, p->FramePosition());
    }
    return *this;
}

AVDecoder &AVPlayer::Decoder()
{
    SyncMemory<BeforeRead>();
    return d->mDecoder;
}

const AVDecoder &AVPlayer::Decoder() const
{
    SyncMemory<BeforeRead>();
    return d->mDecoder;
}

AVPlayer &AVPlayer::Tie(AVPlayer *p)
{
    if (p != this && d->mTiedPartners.Add(p))
        p->Tie(this);
    return *this;
}

AVPlayer &AVPlayer::Untie(AVPlayer *p)
{
    if (d->mTiedPartners.Remove(p))
        p->Untie(this);
    return *this;
}

AVPlayer &AVPlayer::SetClockMaster(AVPlayer *pMaster)
{
    if (pMaster != this)
    {
        AVPlayer *p = pMaster;
        while (p && Error().Empty())
        {
            AVPlayer *q = p->ClockMaster();
            if (q == this)
            {
                SetError("Circular clock master reference detected");
                SuggestDebugging << Error();
            }
            else if (q == p)
                p = 0;
            else
                p = q;
        }
    }
    if (Error().Empty() && pMaster != d->mpClockMaster)
    {
        AVPlayer* pClockMaster = d->mpClockMaster;
        if (pClockMaster)
            pClockMaster->d->mClockSlaves.Remove(this);
        d->mpClockMaster = pMaster;
        if (pMaster && pMaster != this)
            pMaster->d->mClockSlaves.Add(this);
    }
    return *this;
}

AVPlayer *AVPlayer::ClockMaster()
{
    return d->mpClockMaster;
}

const AVPlayer *AVPlayer::ClockMaster() const
{
    return d->mpClockMaster;
}

AVPlayer &AVPlayer::AbsorbIntoTaskQueue(Runnable *p)
{
    d->Tasks().Produce(p);
    return *this;
}

AVPlayer &AVPlayer::AsyncFunctionCall(AVPlayer &(AVPlayer::*func)())
{
    Runnable *p = new MemberCall<AVPlayer &(AVPlayer *)>(func, this);
    return AbsorbIntoTaskQueue(p);
}
