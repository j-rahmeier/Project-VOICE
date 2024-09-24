////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: An AVSource implementation for Windows Media Foundation devices.
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
#include "AVSourceMF.h"

#include "VideoMediaType.h"
#include "AudioMediaType.h"

#include "ComPtr.h"
#include "ComStrings.h"
#include "Errors.h"
#include "../win32mf/MFInit.h"

#include <Wmcodecdsp.h>
const GUID CLSID_CColorConvertDMO = { 0x98230571, 0x0087, 0x4204, { 0xb0, 0x20, 0x32, 0x82, 0x53, 0x8e, 0x57, 0xd3 } };

struct AVSourceMF::Private
{
    com::Ptr<IMFSourceReader> mpReader;
    struct AVSource::Properties mProperties;
    com::Ptr<IMFTransform> mpTransform;

    MFInit mInit;
};

AVSourceMF::AVSourceMF()
    : p(new Private)
{
}

AVSourceMF::~AVSourceMF()
{
    delete p;
}

bool AVSourceMF::Init(com::Ptr<IMFSourceReader> pReader)
{
    p->mpReader = pReader;

    com::Ptr<IMFMediaType> pType;
    HRSucceedOrThrow hr = pReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, pType.Assignee());
    UINT32 width = 0, height = 0;
    hr = ::MFGetAttributeSize(pType, MF_MT_FRAME_SIZE, &width, &height);
    p->mProperties.width = width;
    p->mProperties.height = height;
    UINT32 num = 0, denom = 1;
    hr = ::MFGetAttributeRatio(pType, MF_MT_FRAME_RATE, &num, &denom);
    p->mProperties.fps = Rate(num, denom * Time::OneSecond());
    UINT32 interlaceMode = 0;
    hr = pType->GetUINT32(MF_MT_INTERLACE_MODE, &interlaceMode);
    if (interlaceMode != MFVideoInterlace_Progressive && interlaceMode != MFVideoInterlace_Unknown)
        p->mProperties.fps = p->mProperties.fps * Ratio(1, 2);

    GUID subtype;
    hr = pType->GetGUID(MF_MT_SUBTYPE, &subtype);
    if (subtype != MFVideoFormat_RGB32) {
        com::Ptr<IMFMediaType> pOutputType;
        hr = ::MFCreateMediaType(pOutputType.Assignee());
        UINT32 count = 0;
        hr = pType->GetCount(&count);
        for (int idx = 0; idx < count; ++idx) {
            GUID guid = { 0 };
            PROPVARIANT value = { 0 };
            hr = pType->GetItemByIndex(idx, &guid, &value);
            hr = pOutputType->SetItem(guid, value);
        }
        hr = pOutputType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);
        // Now, pOutputType is a clone of pType, except for the subtype, which is RGB32.

        com::Ptr<IMFTransform> pTransform;
        hr = ::CoCreateInstance(CLSID_CColorConvertDMO, nullptr, CLSCTX_ALL, IID_IMFTransform, pTransform.Assignee());
        pTransform->SetInputType(0, pType, 0);
        pTransform->SetOutputType(0, pOutputType, 0);
        p->mpTransform = pTransform;
    }

    return true;
}

const struct AVSource::Properties& AVSourceMF::Properties() const
{
    return p->mProperties;
}


bool AVSourceMF::WaitForData(std::vector<uint8_t>& outFrameData)
{
    bool gotFrame = false;
    while (!gotFrame) {
        DWORD streamIndex = 0, streamFlags = 0;
        LONGLONG timestamp = 0;
        com::Ptr<IMFSample> pSample;
        HRSucceedOrThrow hr = p->mpReader->ReadSample(
            MF_SOURCE_READER_FIRST_VIDEO_STREAM,
            0,
            &streamIndex,
            &streamFlags,
            &timestamp,
            pSample.Assignee()
        );
        if (streamFlags & ~MF_SOURCE_READERF_STREAMTICK)
            return false;
        if (!pSample)
            continue;

        DWORD count = 0;
        hr = pSample->GetBufferCount(&count);
        if (count < 1)
            continue;
        if (count > 1)
            throw std_runtime_error << "Unexpected buffer count: " << count;

        if (p->mpTransform) {
            com::Ptr<IMFSample> pTransformedSample;
            hr = ::MFCreateSample(pTransformedSample.Assignee());
            //hr = pSample->CopyAllItems(pTransformedSample);
            com::Ptr<IMFMediaBuffer> pBuffer;
            hr = ::MFCreateMemoryBuffer(outFrameData.size(), pBuffer.Assignee());
            hr = pTransformedSample->AddBuffer(pBuffer);
            MFT_OUTPUT_DATA_BUFFER outputBuffer = {0};
            outputBuffer.dwStreamID = 0;
            outputBuffer.pSample = pTransformedSample;
            DWORD status = 0;
            hr = p->mpTransform->ProcessInput(0, pSample, 0);
            hr = p->mpTransform->ProcessOutput(0, 1, &outputBuffer, &status);
            pSample = pTransformedSample;
        }

        com::Ptr<IMFMediaBuffer> pBuffer;
        hr = pSample->GetBufferByIndex(0, pBuffer.Assignee());
        DWORD currentLength = 0;
        hr = pBuffer->GetCurrentLength(&currentLength);
        if (currentLength != outFrameData.size()) {
            ErrorStream() << "Unexpected buffer size: " << outFrameData.size() << ", expected " << currentLength;
            return false;
        }

        BYTE* bufferData = nullptr;
        DWORD maxLength = 0;
        hr = pBuffer->Lock(&bufferData, &maxLength, &currentLength);
        ::memcpy(outFrameData.data(), bufferData, outFrameData.size());
        hr = pBuffer->Unlock();
        gotFrame = true;
    }
    return true;
}

void AVSourceMF::AbortWaiting()
{
    p->mpReader->Flush(MF_SOURCE_READER_ALL_STREAMS);
}

