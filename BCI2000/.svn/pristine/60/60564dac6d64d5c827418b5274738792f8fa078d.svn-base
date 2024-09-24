////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: Implementation of the AVSourceEnumerator interface using
//   libavdevice.to enumerate devices.
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
#include "AVSourceEnumeratorLibAVDevice.h"

#include "AVSourceLibAVDevice.h"
#if __APPLE__
# include "AVFoundationHelper.h"
#endif
#if __linux__
# include "Video4LinuxHelper.h"
#endif
#include "AVLog.h"

extern "C" {
#include "libavdevice/avdevice.h"
}

#include <string>
#include <regex>

AVSourceEnumeratorLibAVDevice::AVSourceEnumeratorLibAVDevice()
{
    AVLog::AddClient();
}

AVSourceEnumeratorLibAVDevice::~AVSourceEnumeratorLibAVDevice()
{
    AVLog::RemoveClient();
}

std::vector<AVSourceEnumerator::Device>
AVSourceEnumeratorLibAVDevice::ListDevices() const
{
    std::vector<Device> devices;
    ::avdevice_register_all();
    const AVInputFormat* pFormat = nullptr;
    while ((pFormat = ::av_input_video_device_next(pFormat))) {

#if __APPLE__
        // avdevice_list_input_sources() not implemented for macOS avfoundation
        if (!::stricmp(pFormat->name, "avfoundation")) {
            ::AVEnumerateSourceDevicesAVFoundation(devices);
            continue;
        }
#endif
#if __linux__
	// avdevice_list_input_sources() not implemented for video4linux
	if (!::stricmp(pFormat->name, "video4linux2,v4l2")) {
	    ::AVEnumerateSourceDevicesVideo4Linux(devices);
	    continue;
	}
#endif
            
        AVDeviceInfoList* pSources = nullptr;
        int r = ::avdevice_list_input_sources(pFormat, nullptr, nullptr, &pSources);
        if (r >= 0) {
            for (int i = 0; i < pSources->nb_devices; ++i) {
                bool hasVideo = false;
                for (int j = 0; j < pSources->devices[i]->nb_media_types; ++j) {
                    hasVideo = hasVideo || pSources->devices[i]->media_types[j] == AVMEDIA_TYPE_VIDEO;
                }
                if (!hasVideo) {
                    continue;
                }

                Device device = {
                    pFormat->name,
                    pSources->devices[i]->device_description,
                    "",
                    pSources->devices[i]->device_name,
                };
                // some drivers have special device naming schemes
                device.url = device.name;
                devices.push_back(device);
            }
            ::avdevice_free_list_devices(&pSources);
        }
    }
    return devices;

}

std::vector<VideoMediaType>
AVSourceEnumeratorLibAVDevice::ListVideoMediaTypes(const AVSourceEnumerator::Device& inDevice) const
{
#if __APPLE__
    if (inDevice.driver == "avfoundation") {
        return AVListVideoMediaTypesAVFoundation(inDevice);
    }
#endif
#if __linux__
    if (inDevice.driver == "v4l2") {
        return AVListVideoMediaTypesVideo4Linux(inDevice);
    }
#endif
    return std::vector<VideoMediaType>();
}

AVSource* 
AVSourceEnumeratorLibAVDevice::CreateSource(const Device& inDevice, const VideoMediaType& inVideoMediaType) const
{
    AVSourceLibAVDevice* pSource = new AVSourceLibAVDevice;
    pSource->Init(inDevice, inVideoMediaType);
    return pSource;
}

AVSource*
AVSourceEnumeratorLibAVDevice::CreateSource(const std::string& url, double timeoutSec, int bufferSizeBytes) const
{
    AVSourceLibAVDevice* pSource = new AVSourceLibAVDevice;
    pSource->Init(url, timeoutSec, bufferSizeBytes);
    return pSource;
}
