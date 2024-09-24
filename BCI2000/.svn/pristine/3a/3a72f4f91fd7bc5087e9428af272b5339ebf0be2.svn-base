////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: Helper functions for interfacing with Video4Linux.
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
#include "Video4LinuxHelper.h"
#include "Ratio.h"
#include "Debugging.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <linux/videodev2.h>

static void FrameIntervalsToVideoMediaTypes(const v4l2_frmivalenum&, std::vector<VideoMediaType>&);

void AVEnumerateSourceDevicesVideo4Linux(std::vector<AVSourceEnumerator::Device>& outDevices)
{
    DIR* dir = ::opendir("/dev");
    if (dir)
    {
        struct dirent *entry;
        while (NULL != (entry = ::readdir(dir)))
        {
            std::string name = entry->d_name;
            if (name.find("video") != 0)
                continue;
            errno = 0;
            name = "/dev/" + name;
            int fd = ::open(name.c_str(), O_RDONLY);
            if (fd < 0)
                continue;
            struct v4l2_capability vcap;
            int err = ::ioctl(fd, VIDIOC_QUERYCAP, &vcap);
            ::close(fd);
            if (err)
                continue;
            if (!(V4L2_CAP_VIDEO_CAPTURE & vcap.device_caps))
                continue;
            AVSourceEnumerator::Device device;
            device.driver = "v4l2";
            device.url = name;
            device.name = device.url;
            vcap.card[sizeof(vcap.card) - 1] = 0;
            device.friendlyName = reinterpret_cast<const char*>(vcap.card);
            outDevices.push_back(device);
        }
        ::closedir(dir);
    }
}

std::vector<VideoMediaType> AVListVideoMediaTypesVideo4Linux(const AVSourceEnumerator::Device& inDevice)
{
    std::vector<VideoMediaType> mediaTypes;
    int fd = ::open(inDevice.name.c_str(), O_RDONLY);
    if (fd < 0)
        return mediaTypes;
    struct v4l2_fmtdesc format = {0};
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    while (::ioctl(fd, VIDIOC_ENUM_FMT, &format) >= 0)
    {
        struct v4l2_frmsizeenum framesize = {0};
        framesize.pixel_format = format.pixelformat;
        while (::ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &framesize) >= 0)
        {
            if (framesize.type == V4L2_FRMSIZE_TYPE_DISCRETE)
            {
                struct v4l2_frmivalenum frameintervals = {0};
                frameintervals.pixel_format = framesize.pixel_format;
                frameintervals.width = framesize.discrete.width;
                frameintervals.height = framesize.discrete.height;
                while (::ioctl(fd, VIDIOC_ENUM_FRAMEINTERVALS, &frameintervals) >= 0)
                {
                    FrameIntervalsToVideoMediaTypes(frameintervals, mediaTypes);
                    ++frameintervals.index;
                }
                ++framesize.index;
            }
        }
        ++format.index;
    }
    ::close(fd);
    return mediaTypes;
}

static VideoMediaType VideoMediaTypeFromFrameInterval(const v4l2_frmivalenum& frameinterval)
{
    Assert(frameinterval.type == V4L2_FRMIVAL_TYPE_DISCRETE);
    VideoMediaType type;
    type.codec = frameinterval.pixel_format;
    type.width = frameinterval.width;
    type.height = frameinterval.height;
    type.pxN = 1;
    type.pxD = 1;
    type.framerateN = frameinterval.discrete.denominator;
    type.framerateD = frameinterval.discrete.numerator;
    return type;
}

static void ContinuousFrameIntervalsToVideoMediaTypes(const v4l2_frmivalenum& frameintervals, std::vector<VideoMediaType>& types)
{
    struct v4l2_frmivalenum interval = frameintervals;
    interval.type = V4L2_FRMIVAL_TYPE_DISCRETE;
    interval.discrete = frameintervals.stepwise.min;
    types.push_back(VideoMediaTypeFromFrameInterval(interval));
    interval.discrete = frameintervals.stepwise.max;
    types.push_back(VideoMediaTypeFromFrameInterval(interval));
}

static void StepwiseFrameIntervalsToVideoMediaTypes(const v4l2_frmivalenum& frameintervals, std::vector<VideoMediaType>& types)
{
    struct v4l2_frmivalenum interval = frameintervals;
    interval.type = V4L2_FRMIVAL_TYPE_DISCRETE;
    for (Ratio dt(frameintervals.stepwise.min.numerator, frameintervals.stepwise.min.denominator);
            dt < Ratio(frameintervals.stepwise.max.numerator, frameintervals.stepwise.max.denominator);
            dt = dt + Ratio(frameintervals.stepwise.step.numerator, frameintervals.stepwise.step.denominator)
        )
    {
        interval.discrete.numerator = dt.Numerator();
        interval.discrete.denominator = dt.Denominator();
        types.push_back(VideoMediaTypeFromFrameInterval(interval));
    }
    interval.discrete = frameintervals.stepwise.max;
    types.push_back(VideoMediaTypeFromFrameInterval(interval));
}

static void FrameIntervalsToVideoMediaTypes(const v4l2_frmivalenum& frameintervals, std::vector<VideoMediaType>& types)
{
    switch (frameintervals.type)
    {
    case V4L2_FRMIVAL_TYPE_DISCRETE:
        types.push_back(VideoMediaTypeFromFrameInterval(frameintervals));
        break;
    case V4L2_FRMIVAL_TYPE_CONTINUOUS:
        ContinuousFrameIntervalsToVideoMediaTypes(frameintervals, types);
        break;
    case V4L2_FRMIVAL_TYPE_STEPWISE:
        StepwiseFrameIntervalsToVideoMediaTypes(frameintervals, types);
        break;
    }
}

