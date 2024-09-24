////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: An AVSource implementation for libavdevice devices.
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
#include "AVSourceLibAVDevice.h"

#include "VideoMediaType.h"
#include "AudioMediaType.h"
#include "Exception.h"
#include "AVLog.h"
#include "AVError.h"

#if __APPLE__
#include "AVFoundationHelper.h"
#endif

extern "C" {
#include "libavdevice/avdevice.h"
#include "libavutil/avutil.h"
#include "libavcodec/avcodec.h"
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

struct AVSourceLibAVDevice::Private
{
    std::string mUrl;
    const AVInputFormat* mpInputFormat = nullptr;
    AVDictionary* mpOptions = nullptr;
    AVFormatContext* mpFormatContext = nullptr;
    AVCodecContext* mpCodecContext = nullptr;
    int mVideoStream = -1;
    AVFrame* mpDecodedFrame = nullptr;
    AVFrame* mpRgbFrame = nullptr;
    SwsContext* mpSwsContext = nullptr;
    AVPacket mEncodedPacket = {0};

    struct AVSource::Properties mProperties;

    ~Private();
    bool Init2();
};

AVSourceLibAVDevice::AVSourceLibAVDevice()
    : p(new Private)
{
    AVLog::AddClient();
}

AVSourceLibAVDevice::~AVSourceLibAVDevice()
{
    AVLog::RemoveClient();
    delete p;
}

bool AVSourceLibAVDevice::Init(const AVSourceEnumerator::Device& inDevice, const VideoMediaType& inVideoType)
{
    p->mUrl = inDevice.url;
    p->mpInputFormat = ::av_find_input_format(inDevice.driver.c_str());
    std::string video_size = std::to_string(inVideoType.width) + "x" + std::to_string(inVideoType.height);
    ::av_dict_set(&p->mpOptions, "video_size", video_size.c_str(), 0);
    std::ostringstream oss;
    oss << inVideoType.framerateN * 1.0f / inVideoType.framerateD;
    ::av_dict_set(&p->mpOptions, "framerate", oss.str().c_str(), 0);
#if 0
    if (inVideoType.codec) {
        std::string input_format = inVideoType.CodecAsString();
        ::av_dict_set(&p->mpOptions, "input_format", input_format.c_str(), 0);
    }
#endif
    return p->Init2();
}

bool AVSourceLibAVDevice::Init(const std::string& inUrl, double inTimeoutSec, int inBufferSizeBytes)
{
    p->mUrl = inUrl;
    double microsecondsPerSec = 1e6;
    ::av_dict_set_int(&p->mpOptions, "timeout", inTimeoutSec * microsecondsPerSec, 0); // microseconds
    ::av_dict_set_int(&p->mpOptions, "buffer_size", inBufferSizeBytes, 0); // bytes
    return p->Init2();
}

bool AVSourceLibAVDevice::Private::Init2()
{
#if __APPLE__
    bool access = AVFoundationGetCameraAccess();
    if (!access) {
        throw std_runtime_error << "User did not allow access to camera";
    }
#endif
    int err =::avformat_open_input(&mpFormatContext, mUrl.c_str(), mpInputFormat, &mpOptions);
    if (err < 0) {
        throw std_runtime_error << "avformat_open_input(): " << AVError(err).Message();
    }
    ::av_dict_free(&mpOptions);

    mpFormatContext->max_analyze_duration = 0; // avoid avformat_find_stream_info() to block indefinitely
    err = ::avformat_find_stream_info(mpFormatContext, nullptr);
    if (err < 0) {
        throw std_runtime_error << "avformat_find_stream_info(): " << AVError(err).Message();
    }
    mVideoStream = -1;
    for (int i = 0; i < mpFormatContext->nb_streams; ++i) {
        if (mpFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            mVideoStream = i;
            break;
        }
    }
    if (mVideoStream < 0) {
        throw std_runtime_error << "Could not identify video stream";
    }

    AVCodecID id = mpFormatContext->streams[mVideoStream]->codecpar->codec_id;
    const AVCodec* pCodec = ::avcodec_find_decoder(id);
    if (!pCodec) {
        throw std_runtime_error << "Codec not found: " << id;
    }

    mpCodecContext = ::avcodec_alloc_context3(pCodec);
    if (!mpCodecContext) {
        throw std_runtime_error << "Could not allocate codec context";
    }

    err = ::avcodec_parameters_to_context(mpCodecContext, mpFormatContext->streams[mVideoStream]->codecpar);
    if (err < 0) {
        throw std_runtime_error << "avcodec_parameters_to_context(): " << AVError(err).Message();
    }
    
    err = ::avcodec_open2(mpCodecContext, pCodec, nullptr);
    if (err < 0) {
        throw std_runtime_error << "avcodec_open2(): " << AVError(err).Message();
    }

    ::av_init_packet(&mEncodedPacket);
    mpDecodedFrame = ::av_frame_alloc();

    int height = mpCodecContext->height, width = mpCodecContext->width;

    mpRgbFrame = ::av_frame_alloc();
    AVPixelFormat rgbFormat = AV_PIX_FMT_RGB32;
    int rgbAlign = 32; // Must be power of 2. If this is too small, sws_scale() will write out-of-bounds (?)
    int size = ::av_image_alloc(mpRgbFrame->data, mpRgbFrame->linesize, width, height, rgbFormat, rgbAlign);
    if (size <= 0) {
        throw std_runtime_error << "av_image_alloc(): " << AVError(size).Message();
    }

    mpSwsContext = ::sws_getContext(width, height,
        mpCodecContext->pix_fmt, width, height,
        rgbFormat, 0, nullptr, nullptr, nullptr);
    if (!mpSwsContext) {
        throw std_runtime_error << "sws_getContext() failed";
    }

    mProperties.width = width;
    mProperties.height = height;

    AVRational rate = ::av_guess_frame_rate(mpFormatContext, mpFormatContext->streams[mVideoStream], nullptr);
    Rate fps(rate.num, Time::Interval::OneSecond() * rate.den);
    mProperties.fps = fps;
    return true;
}

const struct AVSource::Properties& AVSourceLibAVDevice::Properties() const
{
    return p->mProperties;
}


bool AVSourceLibAVDevice::WaitForData(std::vector<uint8_t>& outFrameData)
{
    AVLog::Client logclient; // enable logging of AV errors to bcierr

    bool gotFrame = false;
    while (!gotFrame) {

        int err = AVERROR(EAGAIN);
        while (err == AVERROR(EAGAIN)) {
            err = ::av_read_frame(p->mpFormatContext, &p->mEncodedPacket);
        }
        if (err < 0) {
            throw std_runtime_error << "av_read_frame(): " << AVError(err).Message();
        }

        // Send packet to decoder
        if (p->mEncodedPacket.stream_index == p->mVideoStream) {
            err = ::avcodec_send_packet(p->mpCodecContext, &p->mEncodedPacket);
            ::av_packet_unref(&p->mEncodedPacket);
            if (err < 0) {
                continue; // may fail intermittently
            }
        }
        else {
            ::av_packet_unref(&p->mEncodedPacket);
            continue;
        }

        // Receive uncompressed frame from decoder
        // For certain resolutions, the decoded frame is distorted by libavdevice.
        // This can be reproduced with the command line version of ffmpeg (libavdevice 60).
        // This has been observed for resolutions:
        //  176x144, 
        err = ::avcodec_receive_frame(p->mpCodecContext, p->mpDecodedFrame);
        if (err == AVERROR(EAGAIN)) {
            continue; // not every packet may produce a frame
        }
        else if (err < 0) {
            throw std_runtime_error << "avcodec_receive_frame(): " << AVError(err).Message();
        }
        ::sws_scale(p->mpSwsContext,
            p->mpDecodedFrame->data, p->mpDecodedFrame->linesize, 0, p->mpDecodedFrame->height,
            p->mpRgbFrame->data, p->mpRgbFrame->linesize
        );
        ::av_image_copy_to_buffer(outFrameData.data(), outFrameData.size(),
            p->mpRgbFrame->data, p->mpRgbFrame->linesize, AV_PIX_FMT_RGB32,
            p->mProperties.width, p->mProperties.height, 1
        );
        gotFrame = true;
    }
    return true;
}

AVSourceLibAVDevice::Private::~Private()
{
    ::av_dict_free(&mpOptions);
    ::avformat_close_input(&mpFormatContext);
    ::avcodec_free_context(&mpCodecContext);
    ::av_frame_free(&mpDecodedFrame);
    ::av_frame_free(&mpRgbFrame);
    ::sws_freeContext(mpSwsContext);
}

