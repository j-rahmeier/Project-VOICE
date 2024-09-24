////////////////////////////////////////////////////////////////////////////////
// $Id: AudioStimulus.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A stimulus that plays a sound, or speaks a text, when it is
//   presented.
//   When a sound string is enclosed in single quotes ('text'), it is rendered
//   using the OS's text-to-speech engine.
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
#include "AudioStimulus.h"
#include "BCIStream.h"
#include "SoundStimulus.h"
#include "SpeechStimulus.h"

AudioStimulus::AudioStimulus()
{
    WithLocked(p = mData.Mutable())
    {
        p->mpStimulus = nullptr;
        p->mVolume = 1.0;
        p->mAbortOnConceal = false;
    }
}

AudioStimulus::~AudioStimulus()
{
    delete mData->mpStimulus;
}

AudioStimulus &AudioStimulus::SetSound(const std::string &inSound)
{
    WithLocked(p = mData.Mutable())
    {
        delete p->mpStimulus;
        p->mSound = "";
        p->mError = "";

        if (!inSound.empty())
        {
            p->mSound = inSound;
            if (inSound[0] == cSpeechQuote)
            {
                std::string text = inSound.substr(1, inSound.rfind(cSpeechQuote));
                SpeechStimulus *pSpeech = new SpeechStimulus;
                pSpeech->SetText(text);
                p->mpStimulus = pSpeech;
            }
            else
            {
                SoundStimulus *pSound = new SoundStimulus;
                pSound->SetFile(inSound);
                if (pSound->ErrorState() != WavePlayer::noError)
                    p->mError = "Could not open \"" + inSound + "\" as a sound file";
                p->mpStimulus = pSound;
            }
        }
    }
    return *this;
}

std::string AudioStimulus::Sound() const
{
    return mData->mSound;
}

AudioStimulus &AudioStimulus::SetVolume(float inVolume)
{
    WithLocked(p = mData.Mutable())
    {
        p->mError = "";
        p->mVolume = inVolume;
    }
    return *this;
}

float AudioStimulus::Volume() const
{
    return mData->mVolume;
}

AudioStimulus &AudioStimulus::SetAbortOnConceal(bool inAbort)
{
    WithLocked(p = mData.Mutable())
    {
        p->mError = "";
        p->mAbortOnConceal = inAbort;
    }
    return *this;
}

bool AudioStimulus::AbortOnConceal() const
{
    return mData->mAbortOnConceal;
}

std::string AudioStimulus::Error() const
{
    return mData->mError;
}

void AudioStimulus::OnPresent()
{
    WithLocked(p = mData.Const())
    {
        SoundStimulus *pSoundStimulus = dynamic_cast<SoundStimulus *>(p->mpStimulus);
        if (pSoundStimulus != NULL)
            pSoundStimulus->SetVolume(p->mVolume);

        SpeechStimulus *pSpeechStimulus = dynamic_cast<SpeechStimulus *>(p->mpStimulus);
        if (pSpeechStimulus != NULL)
            pSpeechStimulus->SetVolume(p->mVolume);

        if (p->mpStimulus != NULL)
            p->mpStimulus->Present();
    }
}

void AudioStimulus::OnConceal()
{
    WithLocked(p = mData.Const())
    {
        if (p->mAbortOnConceal && p->mpStimulus)
            p->mpStimulus->Conceal();
    }
}
