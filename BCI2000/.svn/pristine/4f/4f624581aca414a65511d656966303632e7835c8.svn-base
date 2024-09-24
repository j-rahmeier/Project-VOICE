////////////////////////////////////////////////////////////////////////////////
// $Id: WebcamRecorder.cpp 7882 2024-03-01 20:34:21Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A thread performing video input from a camera, and saving to
//  to file. Callbacks to a controller are done through a WebcamClient object
//  pointer.
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
#include "WebcamThread.h"

#if _WIN32
#include "AVSourceEnumeratorMF.h"
#endif
#include "AVSourceEnumeratorLibAVDevice.h"
#include "AVSource.h"


#include "fourcc.h"
#include "VideoMediaType.h"
#include "AudioMediaType.h"
#include "TextRenderer.h"
#include "Exception.h"
#include "Debugging.h"
#include "Resources.h"
#include "SaveToFile.h"
#include "Thread.h"
#include "Runnable.h"

#include <mutex>
#include <condition_variable>
#include <regex>
#include <limits>

namespace {
    time_t Now()
    {
        return ::time(0);
    }

    std::string TimeToString(time_t inTime)
    {
        struct ::tm* p = ::localtime(&inTime);
        if (!p)
            return "<n/a>";
        char buf[32];
        ::strftime(buf, sizeof(buf), "%d %b %y %T", p);
        return buf;
    }
    
    std::string VideoFormatToString(const VideoMediaType& inType)
    {
        std::ostringstream oss;
        oss << inType.width << "x" << inType.height << "@" << (1.0 * inType.framerateN) / inType.framerateD << "Hz";
        return oss.str();
    }
}

struct WebcamThread::Private
{
    int mCameraIndex = -1;
    WebcamController::Parameters mConfig;
    WebcamClient* mpClient = nullptr;
    std::unique_ptr<AVSource> mpSource;

    MemberCall<void(Private*)> mThreadFuncCall;
    Thread mThread;

    std::condition_variable mConditionVariable;
    std::mutex mMutex;
    enum StateEnum { idle, initializing, running, terminating, cleanup, error };
    std::atomic<StateEnum> mState = idle;
    std::string mError, mTitle;
    bool mPreflight = false;

    std::atomic<int> mTimestampMode = 0;
    TextRenderer mTextRenderer;

    SaveToFile mSaveToFile;

    Private();
    void ThreadFunc();
    void AddTimestamp(int width, int height, uint8_t* data);
};

WebcamThread::WebcamThread()
    : p(new Private)
{
}

WebcamThread::~WebcamThread()
{
    if (p->mpSource)
        p->mpSource->AbortWaiting();
    delete p;
}

const std::string& WebcamThread::Error() const
{
    std::lock_guard<std::mutex> lock(p->mMutex);
    return p->mError;
}

std::string WebcamThread::WindowTitle() const
{
    std::lock_guard<std::mutex> lock(p->mMutex);
    return p->mTitle;
}

bool WebcamThread::StartRecording(const std::string& file)
{
    return p->mSaveToFile.StartRecording(file);
}

bool WebcamThread::SwitchRecording(const std::string& file)
{
    return p->mSaveToFile.SwitchRecording(file);
}

bool WebcamThread::StopRecording()
{
    return p->mSaveToFile.StopRecording();
}

bool WebcamThread::SetDecimation(int d)
{
    if (d < 1)
        return false;
    p->mSaveToFile.SetDecimation(d);
    return true;
}

bool WebcamThread::SetTimestampMode(int d)
{
    if (d >= NumTimestampModes)
        return false;
    p->mTimestampMode = d;
    return true;
}

void WebcamThread::SetPreflight(bool preflight)
{
    p->mPreflight = preflight;
}

void WebcamThread::Stop()
{
    std::unique_lock<std::mutex> lock(p->mMutex);
    p->mState = Private::terminating;
    lock.unlock();
    if (p->mpSource)
        p->mpSource->AbortWaiting();
    p->mConditionVariable.notify_one();
    p->mThread.Terminate();
}

bool WebcamThread::Start(int cameraIndex, const WebcamController::Parameters& config, WebcamClient* pClient)
{
    if (p->mThread.Running())
        Stop();
    p->mCameraIndex = cameraIndex;
    p->mConfig = config;
    p->mpClient = pClient;
    p->mThread.Start();
    std::unique_lock<std::mutex> lock(p->mMutex);
    p->mConditionVariable.wait(lock, [this] {
        return p->mState == Private::running || p->mState == Private::error;
    });
    return p->mError.empty();
}

WebcamThread::Private::Private()
: mThreadFuncCall(&Private::ThreadFunc, this),
  mThread(&mThreadFuncCall, "WebcamThread")
{
}

void WebcamThread::Private::ThreadFunc()
{
  try {
    std::unique_lock<std::mutex> lock(mMutex);
    mState = initializing;
    mError.clear();
    lock.unlock();

    const auto& font = Resources::FreeMonoBold_ttf;
    mTextRenderer.LoadFont(font.data, font.length);
    mTextRenderer.SetTextColor(RGBColor::LtGreen);

    mSaveToFile.SetVideoProfile(mConfig.videoprofile);

#if 0
    AudioMediaType audioType;
    audioType.codec = FCC('sowt'); // 16bit pcm little-endian
    audioType.channels = 2;
    audioType.bitspersample = 16;
    audioType.samplerate = 44100;
#endif

    std::string url, displayName;

    if (!mConfig.url.empty()) {
        url = mConfig.url;
        displayName = url;
        auto pEnumerator = std::make_unique<AVSourceEnumeratorLibAVDevice>();
        mpSource.reset(pEnumerator->CreateSource(url, 1, 1024*1024));
    }
    else {
#if _WIN32
        auto pEnumerator = std::make_unique<AVSourceEnumeratorMF>();
#else
        auto pEnumerator = std::make_unique<AVSourceEnumeratorLibAVDevice>();
#endif
        auto devices = pEnumerator->ListDevices();
        if (mCameraIndex < 0 || mCameraIndex >= devices.size()) {
            throw std_runtime_error << "No device found with index " + std::to_string(mCameraIndex);
        }
        auto& device = devices[mCameraIndex];
        url = device.url;
        displayName = device.friendlyName;
        auto videoMediaTypes = pEnumerator->ListVideoMediaTypes(device);

        VideoMediaType videoType;
        videoType.codec = FCC('raw ');
        videoType.width = mConfig.width;
        videoType.height = mConfig.height;
        videoType.framerateN = mConfig.fps * 10000;
        videoType.framerateD = 10000;
        videoType = pEnumerator->MatchVideoMediaType(videoMediaTypes, videoType);
        displayName += " (" + VideoFormatToString(videoType) + ")";
        mpSource.reset(pEnumerator->CreateSource(device, videoType));
    }

    lock.lock();
    mTitle = "Camera " + std::to_string(mCameraIndex) + ": " + displayName;
    mState = running;
    lock.unlock();
    mConditionVariable.notify_one();

    const struct AVSource::Properties& properties = mpSource->Properties();
    int height = properties.height, width = properties.width;
    std::vector<uint8_t> rgbData(width * height * 4, 0);

    Rate fps = properties.fps;
    mSaveToFile.SetVideoProfile(mConfig.videoprofile);
    mSaveToFile.SetCompressionFactor(mConfig.compressionFactor);
    mSaveToFile.SetVideoProperties(width, height, fps);
    mSaveToFile.SetClient(mpClient);

    // about 20 characters in date/time
    mTextRenderer.SetTextSize(width / 24);

    static const int cMeasureFrames = 100;
    Time begin = TimeUtils::MonotonicTime();
    int frameCount = 0;

    bool ok = true;
    while (!mPreflight && mState != terminating && ok) {

        ok = mpSource->WaitForData(rgbData);
        if (ok) {

            if (++frameCount == cMeasureFrames) {
                Time now = TimeUtils::MonotonicTime();
                double ratio = (fps * (now - begin)).To<double>() / frameCount;
                if (ratio < 0.9 || ratio > 1.1) {
                    std::ostringstream oss;
                    double nominal = (fps * Time::OneSecond()).To<double>();
                    double factor = std::round(10.0 / ratio) / 10;
                    oss << "Actual frame rate deviates from nominal (" << nominal << "fps) by a factor of " << factor;
                    mpClient->OnWebcamMessage(oss.str());
                }
            }

            AddTimestamp(width, height, rgbData.data());

            if (!mSaveToFile.HandleVideo(rgbData.data())) {
                throw std_runtime_error << mSaveToFile.Error();
            }
            if (mpClient) {
                mpClient->OnWebcamFrameData(width, height, rgbData.data());
            }
        }
    }

    lock.lock();
    mConditionVariable.wait(lock, [this]{return mState == terminating;});
    mState = cleanup;
    lock.unlock();
  }
  catch (std::exception& e) {
    std::unique_lock<std::mutex> lock(mMutex);
    mError = e.what();
    mState = error;
    lock.unlock();
    if (mpClient) {
        mpClient->OnWebcamFrameData(0, 0, nullptr);
    }
    mConditionVariable.notify_one();
  }
}

void WebcamThread::Private::AddTimestamp(int width, int height, uint8_t* pData)
{
    if (mTimestampMode == TimestampNone)
        return;

    std::string text;
    if (mTimestampMode == FrameCounter)
        text = std::to_string(mSaveToFile.CurrentFrameCount());
    else
        text = TimeToString(Now());

    auto size = mTextRenderer.MeasureText(text);
    const int margin = 10;
    int left = margin, top = margin;
    switch (mTimestampMode) {
    case TimestampTopLeft:
        break;
    case TimestampTopRight:
    case FrameCounter:
        left = width - margin - size.width;
        break;
    case TimestampBottomLeft:
        top = height - margin - size.height;
        break;
    case TimestampBottomRight:
        left = width - margin - size.width;
        top = height - margin - size.height;
        break;
    }
    union { uint8_t* u8; uint32_t* u32; } ptr = { pData };
    mTextRenderer.AttachToImage(width, height, ptr.u32);
    mTextRenderer.RenderText(left, top, text);
}
