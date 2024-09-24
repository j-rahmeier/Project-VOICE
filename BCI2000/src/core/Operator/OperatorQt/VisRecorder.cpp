//////////////////////////////////////////////////////////////////////
// $Id: VisRecorder.cpp 8231 2024-06-29 19:09:33Z mellinger $
// Authors: mellinger@neurotechcenter.org
// Description: A class that allows to record a sequence of bitmap
//   images to a video file.If images come with layer IDs (inside
//   visualization IDs), they are automatically joined into a single
//   video stream.
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
///////////////////////////////////////////////////////////////////////
#include "VisRecorder.h"

#include "FileUtils.h"
#include "TimeUtils.h"
#include "AVEncoder.h"
#include "Rate.h"
#include "BCIStream.h"
#include "BCI_OperatorLib.h"

#include <QImage>
#include <QPainter>

static void BitmapToRGB(const BitmapImage& img, void* pRGBData, int width, int height)
{
    QImage qimage(static_cast<unsigned char*>(pRGBData), width, height, QImage::Format_RGB32);
    QPainter painter(&qimage);
    for (int x = 0; x < img.Width(); ++x)
    {
        float left = 1.0 * x * width / img.Width();
        float right = 1.0 * (x + 1) * width / img.Width();
        for (int y = 0; y < img.Height(); ++y)
        {
            RGBColor c = img(x, y);
            if (c != RGBColor::NullColor) // NullColor is used as transparent color
            {
                float top = 1.0 * y * height / img.Height();
                float bottom = 1.0 * (y + 1) * height / img.Height();
                QRectF rect(left, top, right - left, bottom - top);
                QColor qc(c.R(), c.G(), c.B());
                painter.fillRect(rect, qc);
            }
        }
    }
}

// Recorder instance
struct VisRecorder::Private
{
    AVEncoder mEncoder;
    int mVideoStreamIdx = -1;
    uint32_t* mpRGBData = nullptr;
    int mWidth = 0, mHeight = 0;
    Rate mFps;

    std::string mVisID;
    int64_t mFrameNumber = 0;
    bool mRecording = false;
    std::recursive_mutex mMutex;
};

VisRecorder::VisRecorder(const std::string& visID)
: p(new Private)
{
    std::lock_guard<std::recursive_mutex> lock(p->mMutex);
    p->mVisID = visID;
    p->mFps = Rate(1, Time::OneSecond());
}

VisRecorder::~VisRecorder()
{
    {
        std::lock_guard<std::recursive_mutex> lock(p->mMutex);
        OnStopRun();
    }
    delete p;
}

void VisRecorder::OnStartRun(const std::string& videoFile)
{
    OnStopRun();
    std::lock_guard<std::recursive_mutex> lock(p->mMutex);
    p->mEncoder.OpenFile(videoFile + ".mp4");
    p->mEncoder.AddVideoStream(&p->mVideoStreamIdx, AVEncoder::h264);
}

void VisRecorder::OnStopRun()
{
    std::lock_guard<std::recursive_mutex> lock(p->mMutex);
    delete[] p->mpRGBData;
    p->mpRGBData = nullptr;
    if (p->mVideoStreamIdx >= 0)
        p->mEncoder.FinalizeStream(p->mVideoStreamIdx);
    p->mVideoStreamIdx = -1;
    p->mEncoder.CloseFile();
    p->mRecording = false;
}

void VisRecorder::OnNextFilePart(const std::string& nextVideoFile)
{
    // The lock makes sure that OnHandleBitmap() is blocked until the 
    // new file has been set up.
    std::lock_guard<std::recursive_mutex> lock(p->mMutex);
    OnStartRun(nextVideoFile);
}

void VisRecorder::OnHandleBitmap(const std::map<std::string, BitmapImage>& inLayers)
{
    std::lock_guard<std::recursive_mutex> lock(p->mMutex);
    if (p->mVideoStreamIdx < 0)
        return;

    // Guarantee: inLayers will not be empty, and will not contain empty images.
    if (!p->mRecording)
    {
        p->mRecording = true;
        p->mFrameNumber = 0;

        p->mWidth = inLayers.begin()->second.Width(),
        p->mHeight = inLayers.begin()->second.Height();
        if (p->mWidth % 2) // requirement of YUV420P format
            ++p->mWidth;
        if (p->mHeight % 2)
            ++p->mHeight;

        delete[] p->mpRGBData;
        p->mpRGBData = new uint32_t[p->mWidth * p->mHeight];

        p->mEncoder.InitializeVideoStream(p->mVideoStreamIdx, p->mWidth, p->mHeight, Rate(1, Time::OneSecond()));
        p->mEncoder.BeginWriting();
    }
    std::ostringstream oss;
    // If the event cannot be set (because we are not in Running state any more) that's not an error.
    oss << "SET LogLevel 0; SET AbortOnError false; ";
    oss << "SET EVENT " << EventName(p->mVisID) << " " << p->mFrameNumber;
    ::BCI_ExecuteScript(oss.str().c_str());

    int bytes = p->mWidth * p->mHeight * sizeof(*p->mpRGBData);
    ::memset(p->mpRGBData, 0, bytes);

    for (const auto& entry : inLayers)
        BitmapToRGB(entry.second, p->mpRGBData, p->mWidth, p->mHeight);

    Time::Interval timestamp = (p->mFrameNumber++) / p->mFps;
    p->mEncoder.WriteFrame(p->mVideoStreamIdx, p->mpRGBData, bytes, timestamp);
}

// static class objects
std::string VisRecorder::sRunFile;
int VisRecorder::sFilePart;
std::map<std::string, VisRecorder*> VisRecorder::sRecorders;
std::map<std::string, std::map<std::string, BitmapImage>> VisRecorder::sImages;

bool VisRecorder::VisualizationRegistered(const std::string& visID)
{
    return sRecorders.find(visID) != sRecorders.end();
}

void VisRecorder::Clear()
{
    for (const auto& entry : sRecorders)
        delete entry.second;
    sRecorders.clear();

    sImages.clear();
}

// static class methods
void VisRecorder::StartRecording(const std::string& visID)
{
    if (!VisualizationRegistered(visID))
    {
        // Add an event variable to store frame numbers.
        std::string name = EventName(visID);
        std::string command = "SET LogLevel 0; INSERT EVENT " + name + " 32 0";
        ::BCI_ExecuteScript(command.c_str());
    }
    auto i = sRecorders.find(visID);
    if (i == sRecorders.end() || i->second == nullptr)
    {
        sRecorders[visID] = new VisRecorder(visID);
        if (!sRunFile.empty())
            sRecorders[visID]->OnStartRun(VideoFileName(visID));
    }
}

void VisRecorder::StopRecording(const std::string& visID)
{
    auto i = sRecorders.find(visID);
    if (i != sRecorders.end() && i->second != nullptr)
    {
        i->second->OnStopRun();
        delete i->second;
        i->second = nullptr;
    }
}

void VisRecorder::StartRun(const std::string& runFile)
{
    sRunFile = FileUtils::StripExtension(runFile);
    sFilePart = 1;
    for (const auto& entry : sRecorders)
    {
        if (entry.second)
            entry.second->OnStartRun(VideoFileName(entry.first));
    }
}

void VisRecorder::NextFilePart(int filePart)
{
    sFilePart = filePart;
    for (const auto& entry : sRecorders)
        if (entry.second)
            entry.second->OnNextFilePart(VideoFileName(entry.first));
}

void VisRecorder::StopRun()
{
    for (const auto& entry : sRecorders)
        if (entry.second)
            entry.second->OnStopRun();
    sRunFile.clear();
    sFilePart = 0;
}

// Bitmap images may arrive at any time, and mainly consist of difference images.
// We need to integrate and store all incoming images in case
// they are going to be used later.
void VisRecorder::HandleBitmap(const std::string& inVisID, const BitmapImage& inImage)
{
    size_t pos = inVisID.find(":");
    std::string visID = inVisID.substr(0, pos);
    std::string layerID = (pos == std::string::npos) ? "" : inVisID.substr(pos + 1);

    if (VisualizationRegistered(visID))
    {
        if (sImages[visID][layerID].Empty() || inImage.Empty())
            sImages[visID][layerID] = inImage;
        else
            sImages[visID][layerID] += inImage;
    }

    if (inImage.Empty())
        return;

    // Guarantee: sImages[visID] will not be empty, and will not contain empty images.
    auto i = sRecorders.find(visID);
    if (i != sRecorders.end() && i->second)
        i->second->OnHandleBitmap(sImages[visID]);
}

std::string VisRecorder::VideoFileName(const std::string& inVisID)
{
    std::string name;
    if (sRunFile.empty())
        return name;
    name = sRunFile + "_" + inVisID;
    if (sFilePart > 1)
        name += ".part" + std::to_string(sFilePart);
    return name;
}

std::string VisRecorder::EventName(const std::string& visID)
{
    return "VisFrame_" + visID;
}

