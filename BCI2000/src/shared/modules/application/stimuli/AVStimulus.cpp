////////////////////////////////////////////////////////////////////////////////
// $Id: AVStimulus.cpp 8090 2024-05-01 10:33:38Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A stimulus that plays audio and/or video when being presented.
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
#include "AVStimulus.h"

#include "AudioPlayer.h"
#include "AudioSink.h"
#include "BCIStream.h"
#include "BCIEvent.h"
#include "FileUtils.h"
#include "GraphDisplay.h"
#include "Lockable.h"
#include "SpeechStimulus.h"
#include "StringUtils.h"

static const char cSpeechQuote = '\'';
static const char cStreamSeparator = ':';

struct AVStimulus::Data : Lockable<>
{
    StringUtils::String mError, mSource;
    double mVolume;
    SpeechStimulus mSpeech;
    Stimulus *mpStimulus;
    VideoPlayer *mpVideoPlayer;
    AudioPlayer mAudioPlayer;
    bool mAbortOnConceal;
    struct : Runnable
    {
        AVStimulus *pSelf;
        AVPlayer *pControl;
        bool wasPlaying = false;
        void OnRun()
        {
            OnPlayerStateChange();
        }
        void OnPlayerStateChange()
        {
            Assert(pControl);
            if (!pControl->IsPlaying() && pControl->FramePosition() > 0)
                pControl->Stop();
            if (!pControl->Error().Empty() && pSelf->d->mError.Empty())
                pSelf->d->mError = pControl->Error();
            if (!pControl->IsPlaying() && wasPlaying) {
                pSelf->d->mpVideoPlayer->Hide();
                Data::OnVideoStimulusConcealed(pSelf);
            }
            wasPlaying = pControl->IsPlaying();
        }
    } mPlayer;
    Data(AudioSinkRef *ppSink) : mAudioPlayer(ppSink ? *ppSink : nullptr)
    {
    }

    static std::atomic<AVStimulus*> spCurrentStimulus;
    static std::atomic<uint32_t> sLastValue;
    static const uint32_t cInvalidValue = ~(0L);

    static void OnVideoStimulusTick(AVStimulus* inpStimulus)
    {
        if (inpStimulus == spCurrentStimulus) {
            uint32_t value = inpStimulus->VideoPlayer::FramePosition();
            if (value != sLastValue) {
                sLastValue = value;
                bcievent << "VideoStimulusFrame " << value;
            }
        }
    }
    static void OnVideoStimulusPresented(AVStimulus* inpStimulus)
    {
        spCurrentStimulus = inpStimulus;
        bcievent << "VideoStimulusFrame " << inpStimulus->VideoPlayer::FramePosition();
    }
    static void OnVideoStimulusConcealed(AVStimulus* inpStimulus)
    {
        AVStimulus* pStimulus = inpStimulus;
        spCurrentStimulus.compare_exchange_weak(pStimulus, nullptr);
        if (pStimulus == inpStimulus) {
            bcievent << "VideoStimulusFrame " << cInvalidValue;
            sLastValue = cInvalidValue;
        }
    }
};

std::atomic<AVStimulus*> AVStimulus::Data::spCurrentStimulus;
std::atomic<uint32_t> AVStimulus::Data::sLastValue; // = 0;

AVStimulus::AVStimulus(GUI::GraphDisplay &display)
    : VideoPlayer(display, AVStimulusZOrder), d(new Data(display.AssociatedAudio()))
{
    WithLock(d)
    {
        d->mpStimulus = this;
        d->mpVideoPlayer = this;
        d->mAbortOnConceal = false;
        AVStimulus::SetVolume(1.0);
        VideoPlayer::SetVisible(false);
        d->mPlayer.pSelf = this;
        d->mPlayer.pControl = 0;
        if (!d->mAudioPlayer.Error().Empty())
            d->mError = d->mAudioPlayer.Error();
        else
            d->mError = VideoPlayer::Error();
    }
}

AVStimulus::~AVStimulus()
{
    SetSource("");
    Data::OnVideoStimulusConcealed(this);
    delete d;
}

const StringUtils::String &AVStimulus::AudioDevice() const
{
    return d->mAudioPlayer.DeviceInfo();
}

const StringUtils::String &AVStimulus::VideoDevice() const
{
    return VideoPlayer::DeviceInfo();
}

const StringUtils::String &AVStimulus::Error() const
{
    ScopedLock(d);
    if (d->mError.Empty() && d->mPlayer.pControl)
        d->mError = d->mPlayer.pControl->Error();
    return d->mError;
}

AVStimulus &AVStimulus::SetSource(const std::string &inSource)
{
    ScopedLock(d);
    if (d->mPlayer.pControl)
    {
        d->mPlayer.pControl->SetOnStateChange(0);
        d->mPlayer.pControl->Close();
        d->mPlayer.pControl = 0;
        VideoPlayer::Untie(&d->mAudioPlayer);
        VideoPlayer::SetClockMaster(this);
        d->mAudioPlayer.SetClockMaster(&d->mAudioPlayer);
    }
    d->mSource = StringUtils::Trim(inSource);
    d->mSpeech.SetText("");
    d->mError = "";
    d->mpStimulus = 0;
    if (!d->mSource.Empty())
    {
        size_t length = d->mSource.Length();
        if (length >= 2 && d->mSource.At(0) == cSpeechQuote && d->mSource.At(length - 1) == cSpeechQuote)
        {
            d->mpStimulus = &d->mSpeech;
            d->mSpeech.SetText(d->mSource.Substr(1, length - 2));
        }
        else if (VideoPlayer::Open(d->mSource))
        {
            d->mpStimulus = this;
            if (d->mAudioPlayer.Open(d->mSource))
            {
                VideoPlayer::Tie(&d->mAudioPlayer);
                d->mAudioPlayer.SetClockMaster(this);
                d->mPlayer.pControl = &d->mAudioPlayer;
            }
            else
                d->mPlayer.pControl = this;
        }
        else if (d->mAudioPlayer.Open(d->mSource))
        {
            d->mpStimulus = this;
            d->mPlayer.pControl = &d->mAudioPlayer;
            if (d->mAudioPlayer.NativeDuration() < Time::Seconds(5))
                d->mAudioPlayer.LoadIntoMemory();
        }
        else if (d->mError.Empty())
            d->mError = d->mAudioPlayer.Error();
    }
    if (d->mPlayer.pControl)
        d->mPlayer.pControl->SetOnStateChange(&d->mPlayer);
    return *this;
}

const StringUtils::String &AVStimulus::Source() const
{
    ScopedLock(d);
    return d->mSource;
}

AVStimulus &AVStimulus::SetVolume(double inVolume)
{
    ScopedLock(d);
    d->mVolume = inVolume;
    d->mAudioPlayer.SetVolume(inVolume);
    d->mSpeech.SetVolume(inVolume);
    return *this;
}

double AVStimulus::Volume() const
{
    ScopedLock(d);
    return d->mVolume;
}

AVStimulus &AVStimulus::SetAbortOnConceal(bool b)
{
    ScopedLock(d);
    d->mAbortOnConceal = b;
    return *this;
}

bool AVStimulus::AbortOnConceal() const
{
    ScopedLock(d);
    return d->mAbortOnConceal;
}

const char* AVStimulus::OnRenderingTick(const ClockTick& t, bool b)
{
    if (VideoPlayer::IsOpen())
        Data::OnVideoStimulusTick(this);
    return VideoPlayer::OnRenderingTick(t, b);
}

void AVStimulus::OnPresent()
{
    ScopedLock(d);
    if (d->mpStimulus == this)
    {
        d->mPlayer.pControl->Stop().Play();
        if (!d->mPlayer.pControl->Error().Empty() && d->mError.Empty())
            d->mError = d->mPlayer.pControl->Error();
        VideoPlayer::Show();
        if (VideoPlayer::IsOpen())
            Data::OnVideoStimulusPresented(this);
    }
    else if (d->mpStimulus)
        d->mpStimulus->Present();
    if (!Error().Empty())
        throw bcierr << Error();
}

void AVStimulus::OnConceal()
{
    ScopedLock(d);
    if (d->mpStimulus == this)
    {
        if (d->mAbortOnConceal)
            d->mPlayer.pControl->Stop();
    }
    else if (d->mpStimulus)
        d->mpStimulus->Conceal();
    if (!Error().Empty())
        throw bcierr << Error();
}

void AVStimulus::OnStop()
{
    ScopedLock(d);
    if (d->mpStimulus == this)
        d->mPlayer.pControl->Stop();
}
