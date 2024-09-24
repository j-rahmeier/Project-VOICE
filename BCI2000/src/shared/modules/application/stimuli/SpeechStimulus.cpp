////////////////////////////////////////////////////////////////////////////////
// $Id: SpeechStimulus.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A stimulus that speaks text on the Stimulus::Present event.
//   To allow for finishing speech even after the SpeechStimulus object has
//   been deleted, underlying TextToSpeech instances are pooled, and only
//   deallocated after speech has been finished.
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
#include "SpeechStimulus.h"

#include "Exception.h"
#include "Lockable.h"

namespace
{

struct TTSEntry
{
    TextToSpeech *instance;
    bool owned;
};
std::vector<TTSEntry> sTTSInstances;
Lockable<> sTTSInstancesLock;

} // namespace

TextToSpeech *SpeechStimulus::AllocateTTS()
{
    ScopedLock(sTTSInstancesLock);
    for (std::vector<TTSEntry>::iterator i = sTTSInstances.begin(); i != sTTSInstances.end(); ++i)
        if (!i->owned && i->instance && !i->instance->IsSpeaking())
        {
            delete i->instance;
            i->instance = nullptr;
        }

    std::vector<TTSEntry>::iterator i = sTTSInstances.begin();
    for (; i != sTTSInstances.end(); ++i)
        if (i->instance == nullptr)
            break;
    if (i == sTTSInstances.end())
    {
        sTTSInstances.push_back(TTSEntry());
        i = sTTSInstances.end();
        --i;
    }
    i->instance = new TextToSpeech;
    i->owned = true;
    return i->instance;
}

void SpeechStimulus::FreeTTS(TextToSpeech *inpTTS)
{
    ScopedLock(sTTSInstancesLock);
    std::vector<TTSEntry>::iterator i = sTTSInstances.begin();
    for (; i != sTTSInstances.end(); ++i)
        if (i->instance == inpTTS)
            break;
    if (i == sTTSInstances.end())
        throw std_logic_error << "Trying to deallocate a TextToSpeech object"
                                 " that has not been allocated with"
                                 " SpeechStimulus::AllocateTTS()";
    if (!i->instance->IsSpeaking())
    {
        delete i->instance;
        sTTSInstances.erase(i);
    }
    else
    {
        i->owned = false;
    }
}
