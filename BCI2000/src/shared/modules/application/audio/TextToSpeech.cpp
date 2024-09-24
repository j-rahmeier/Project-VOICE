////////////////////////////////////////////////////////////////////////////////
// $Id: TextToSpeech.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A simple wrapper class for text-to-speech audio output.
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
#include "TextToSpeech.h"
#include "BCIStream.h"
#include <string>
#if _WIN32
#include <SAPI.h>
#endif // _WIN32

int TextToSpeech::sNumInstances = 0;

#if _WIN32

TextToSpeech::TextToSpeech() : mVolume(1.0), mpVoice(NULL)
{
    if (sNumInstances++ < 1)
        ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED); // COINIT_MULTITHREADED not allowed in GUI threads
    void *pInterface = NULL;
    ::CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, &pInterface);
    if (pInterface == NULL)
        bcierr << "Could not initialize text-to-speech subsystem";
    mpVoice = reinterpret_cast<ISpVoice *>(pInterface);
}

TextToSpeech::~TextToSpeech()
{
    if (mpVoice != NULL)
        mpVoice->Release();
    if (--sNumInstances < 1)
        ::CoUninitialize();
}

bool TextToSpeech::IsSpeaking() const
{
    bool result = false;
    if (mpVoice != NULL)
    {
        SPVOICESTATUS status;
        if (S_OK == mpVoice->GetStatus(&status, NULL))
            result = (SPRS_IS_SPEAKING == status.dwRunningState || 0 == status.dwRunningState);
    }
    return result;
}

TextToSpeech &TextToSpeech::Speak()
{
    if (mpVoice != NULL)
    {
        const std::ctype<wchar_t> &ct = std::use_facet<std::ctype<wchar_t>>(std::locale());
        std::wstring ws(mText.size(), '\0');
        for (size_t i = 0; i < mText.size(); ++i)
            ws[i] = ct.widen(mText[i]);
        mpVoice->SetVolume(static_cast<USHORT>(mVolume * 100));
        if (S_OK != mpVoice->Speak(ws.c_str(), SPF_ASYNC | SPF_PURGEBEFORESPEAK, NULL))
            bcierr << "Could not speak text \"" << mText << "\"";
    }
    return *this;
}

TextToSpeech &TextToSpeech::Stop()
{
    if (mpVoice != NULL)
        mpVoice->Speak(NULL, SPF_ASYNC | SPF_PURGEBEFORESPEAK, NULL);
    return *this;
}

#else // _WIN32

TextToSpeech::TextToSpeech()
{
}

TextToSpeech::~TextToSpeech()
{
}

bool TextToSpeech::IsSpeaking() const
{
    return false;
}

TextToSpeech &TextToSpeech::Speak()
{
    return *this;
}

TextToSpeech &TextToSpeech::Stop()
{
    return *this;
}

#endif // _WIN32
