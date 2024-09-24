////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: Implementation of the AVSourceEnumerator interface using
//   Windows Media Foundation.to enumerate devices.
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
#include "AVSourceEnumeratorMF.h"

#include <limits>

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#include "Win32/Errors.h"
#include "ComPtr.h"
#include "ComStrings.h"
#include "AVSourceMF.h"

#include "Debugging.h"

namespace {
    com::Ptr<IMFMediaSource> OpenDeviceByPath(const std::string& path)
    {
        com::Ptr<IMFAttributes> pConfig;
        // Create an attribute store to hold the search criteria.
        HRSucceedOrThrow hr = ::MFCreateAttributes(pConfig.Assignee(), 1);
        // Request video capture devices.
        hr = pConfig->SetGUID(
            MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
            MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID
        );

        IMFActivate** pList = nullptr;
        UINT32 count = 0;
        hr = ::MFEnumDeviceSources(pConfig, &pList, &count);

        com::Ptr<IMFMediaSource> pSource;
        com::DualString wpath = path;
        for (int i = 0; i < count; ++i) {

            auto pDev = pList[i];

            wchar_t* pString = nullptr;
            UINT32 length = 0;
            pDev->GetAllocatedString(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK, &pString, &length);
            if (wpath == pString) {
                hr = pDev->ActivateObject(IID_IMFMediaSource, pSource.Assignee());
            }
            ::CoTaskMemFree(pString);

            pDev->Release();
        }
        ::CoTaskMemFree(pList);
        return pSource;
    }

    VideoMediaType GetVideoMediaType(IMFMediaType* pType)
    {
        VideoMediaType v;
        GUID guid;
        HRSucceedOrThrow hr = pType->GetGUID(MF_MT_SUBTYPE, &guid);
        v.codec = guid.Data1;
        hr = ::MFGetAttributeSize(pType, MF_MT_FRAME_SIZE, &v.width, &v.height);
        hr = ::MFGetAttributeRatio(pType, MF_MT_FRAME_RATE, &v.framerateN, &v.framerateD);
        if (S_OK != ::MFGetAttributeRatio(pType, MF_MT_PIXEL_ASPECT_RATIO, &v.pxN, &v.pxD)) {
            v.pxN = 1;
            v.pxD = 1;
        }
        if (S_OK != pType->GetUINT32(MF_MT_INTERLACE_MODE, &v.interlaceType)) {
            v.interlaceType = MFVideoInterlace_Progressive;
        }
        v.avgbitrate = ::MFGetAttributeUINT32(pType, MF_MT_AVG_BITRATE, 0);
        if (v.avgbitrate == 0) {
            v.avgbitrate = v.width * v.height * v.framerateN / (1.0 * v.framerateD) * 32;
        }
        return v;
    }

    std::vector<VideoMediaType>
    ListOrSetVideoMediaTypes(IMFMediaSource* pSource, int index = -1)
    {
        com::Ptr<IMFPresentationDescriptor> pPresentationDescriptor;
        HRSucceedOrThrow hr = pSource->CreatePresentationDescriptor(pPresentationDescriptor.Assignee());

        com::Ptr<IMFStreamDescriptor> pStreamDescriptor;
        BOOL selected = FALSE;
        hr = pPresentationDescriptor->GetStreamDescriptorByIndex(0, &selected, pStreamDescriptor.Assignee());

        com::Ptr<IMFMediaTypeHandler> pMediaTypeHandler;
        hr = pStreamDescriptor->GetMediaTypeHandler(pMediaTypeHandler.Assignee());

        DWORD count = 0;
        hr = pMediaTypeHandler->GetMediaTypeCount(&count);

        std::vector<VideoMediaType> types;
        for (int i = 0; i < count; ++i) {
            com::Ptr<IMFMediaType> pMediaType;
            hr = pMediaTypeHandler->GetMediaTypeByIndex(i, pMediaType.Assignee());
            types.push_back(GetVideoMediaType(pMediaType));
            if (i == index) {
                pMediaTypeHandler->SetCurrentMediaType(pMediaType);
            }
        }
        return types;
    }

    void SetVideoMediaType(IMFMediaSource* pSource, int index)
    {
        Assert(index >= 0);
        ListOrSetVideoMediaTypes(pSource, index);
    }
}

AVSourceEnumeratorMF::AVSourceEnumeratorMF()
{
}

AVSourceEnumeratorMF::~AVSourceEnumeratorMF()
{
}

std::vector<AVSourceEnumerator::Device>
AVSourceEnumeratorMF::ListDevices() const
{
    std::vector<Device> devices;

    com::Ptr<IMFAttributes> pConfig;
    // Create an attribute store to hold the search criteria.
    HRSucceedOrThrow hr = ::MFCreateAttributes(pConfig.Assignee(), 1);
    // Request video capture devices.
    hr = pConfig->SetGUID(
        MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
        MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID
    );

    IMFActivate** pList = nullptr;
    UINT32 count = 0;
    hr = ::MFEnumDeviceSources(pConfig, &pList, &count);
    for (int i = 0; i < count; ++i) {
        auto pDev = pList[i];

        AVSourceEnumerator::Device device;

        wchar_t* pString = nullptr;
        UINT32 length = 0;
        pDev->GetAllocatedString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, &pString, &length);
        device.friendlyName = com::DualString(pString);
        ::CoTaskMemFree(pString);

        pDev->GetAllocatedString(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK, &pString, &length);
        device.name = com::DualString(pString);
        ::CoTaskMemFree(pString);
        
        devices.push_back(device);
        pDev->Release();
    }
    ::CoTaskMemFree(pList);
    return devices;
}

std::vector<VideoMediaType>
AVSourceEnumeratorMF::ListVideoMediaTypes(const AVSourceEnumerator::Device& inDevice) const
{
    auto pSource = OpenDeviceByPath(inDevice.name);
    return ListOrSetVideoMediaTypes(pSource, -1);
}

AVSource* 
AVSourceEnumeratorMF::CreateSource(const Device& inDevice, const VideoMediaType& inVideoMediaType) const
{
    auto pMediaSource = OpenDeviceByPath(inDevice.name);
    if (!pMediaSource)
        return nullptr;

    auto mediaTypes = ListOrSetVideoMediaTypes(pMediaSource);
    int idx = -1;
    double mindist = std::numeric_limits<double>::infinity();
    for (int i = 0; i < mediaTypes.size(); ++i) {
        double dist = inVideoMediaType.SqDist(mediaTypes[i]);
        if (dist < mindist) {
            idx = i;
            mindist = dist;
        }
    }
    SetVideoMediaType(pMediaSource, idx);

    com::Ptr<IMFAttributes> pAttributes;
    HRSucceedOrThrow hr = ::MFCreateAttributes(pAttributes.Assignee(), 1);
    pAttributes->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE);
    pAttributes->SetUINT32(MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS, TRUE);
    com::Ptr<IMFSourceReader> pSourceReader;
    hr = ::MFCreateSourceReaderFromMediaSource(pMediaSource, pAttributes, pSourceReader.Assignee());

    // If we directly set the desired media format to RGB32, it may happen that the decoding pipeline
    // search process takes a long time (around 30s) and fails in the end.
    // Instead, we request NV12 (which is supposed to be always supported), and transform that to RGB32
    // in a later step.
    com::Ptr<IMFMediaType> pMediaType;
    hr = pSourceReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, pMediaType.Assignee());
    hr = pMediaType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_NV12);
    hr = pSourceReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, nullptr, pMediaType);

    AVSourceMF* pSource = new AVSourceMF;
    pSource->Init(pSourceReader);
    return pSource;
}

AVSource*
AVSourceEnumeratorMF::CreateSource(const std::string& url, double timeoutSec, int bufferSizeBytes) const
{
    return nullptr;
}

