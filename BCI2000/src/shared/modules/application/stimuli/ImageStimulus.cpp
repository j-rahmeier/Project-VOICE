////////////////////////////////////////////////////////////////////////////////
// $Id: ImageStimulus.cpp 7916 2024-03-09 14:49:43Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A stimulus consisting of an image.
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
#include "ImageStimulus.h"

#include "BCIStream.h"
#include "FileUtils.h"
#include "Lockable.h"
#include "NumericConstants.h"
#include "AVUtils.h"

#if USE_QT
#include <QBitmap>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#endif

struct ImageStimulus::Data : Lockable<>
{
    std::string mFile;
    int mRenderingMode;

#if USE_QT
    QImage *mpImage;
    QPixmap *mpImageBufferNormal, *mpImageBufferHighlighted;

    Data() : mpImage(0), mpImageBufferNormal(0), mpImageBufferHighlighted(0)
    {
    }
    ~Data()
    {
        delete mpImage;
        delete mpImageBufferNormal;
        delete mpImageBufferHighlighted;
    }
#endif
};

#if USE_QT
static QPixmap *NewBufferFromImage(QImage &inImage, bool inTransparent)
{
    QPixmap *pBuffer = new QPixmap(QPixmap::fromImage(inImage));
    if (inTransparent && !inImage.hasAlphaChannel())
    {
        QRgb c = inImage.pixel(0, 0);
        QImage mask = inImage.createMaskFromColor(c, Qt::MaskInColor);
        pBuffer->setMask(QBitmap::fromImage(mask));
    }
    return pBuffer;
}

static void LibavCleanup(void* p)
{
    delete[] static_cast<uint8_t*>(p);
}

enum {
  resultOk = 0,
  resultErr = 1,
  resultOutOfMemory = 2,
};

static int LibavLoad(QImage& outImage, const std::string& inName)
{
    AVUtils::Log log;
    int result = resultErr;
    AVFormatContext* pFormatContext = nullptr;
    if (::avformat_open_input(&pFormatContext, inName.c_str(), nullptr, nullptr) == 0)
    {
        if (::avformat_find_stream_info(pFormatContext, nullptr) >= 0)
        {
            int videoStreamIndex = -1;
            for (int i = 0; i < pFormatContext->nb_streams; ++i)
            {
                if (pFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
                {
                    videoStreamIndex = i;
                    break;
                }
            }
            if (videoStreamIndex >= 0)
            {
                AVCodecParameters* pCodecpar = pFormatContext->streams[videoStreamIndex]->codecpar;
                const AVCodec* pCodec = ::avcodec_find_decoder(pCodecpar->codec_id);
                AVCodecContext* pCodecCtx = ::avcodec_alloc_context3(pCodec);
                ::avcodec_parameters_to_context(pCodecCtx, pCodecpar);
                pCodecCtx->thread_count = 1;
                AVDictionary* optionsDict = nullptr;
                if (pCodec && ::avcodec_open2(pCodecCtx, pCodec, &optionsDict) >= 0)
                {
                    AVPacket encodedPacket;
                    ::av_init_packet(&encodedPacket);
                    encodedPacket.data = nullptr;
                    encodedPacket.size = 0;
                    if (::av_read_frame(pFormatContext, &encodedPacket) >= 0)
                    {
                        AVFrame* decodedFrame = ::av_frame_alloc();
                        int err = ::avcodec_send_packet(pCodecCtx, &encodedPacket);
                        if (!err)
                            err = ::avcodec_receive_frame(pCodecCtx, decodedFrame);
                        if (!err)
                        {
                            AVFrame* destPic = ::av_frame_alloc();
                            AVPixelFormat destFormat = AV_PIX_FMT_RGB32;
                            int destAlign = 32; // Must be power of 2. If this is too small, sws_scale() will write out-of-bounds (?)
                            int size =::av_image_alloc(destPic->data, destPic->linesize, decodedFrame->width, decodedFrame->height, destFormat, destAlign);
                            SwsContext* ctxt = ::sws_getContext(decodedFrame->width, decodedFrame->height,
                                (AVPixelFormat)decodedFrame->format, decodedFrame->width, decodedFrame->height,
                                destFormat, 0, nullptr, nullptr, nullptr);
                            if (size > 0 && ctxt != nullptr)
                            {
                                ::sws_scale(ctxt, decodedFrame->data, decodedFrame->linesize, 0, decodedFrame->height, destPic->data, destPic->linesize);
                                ::sws_freeContext(ctxt);
                                int raw_data_size = decodedFrame->height * decodedFrame->width * 4;
                                try {
                                  uint8_t* raw_data = new uint8_t[raw_data_size];
                                  ::av_image_copy_to_buffer(raw_data, raw_data_size, destPic->data, destPic->linesize,
                                    AV_PIX_FMT_RGB32, decodedFrame->width, decodedFrame->height, 1);
                                  outImage = QImage(raw_data, decodedFrame->width, decodedFrame->height, QImage::Format_ARGB32, &LibavCleanup, raw_data);
                                  result = resultOk;
                                  // don't delete[] raw_data, will be deleted from QImage destructor
                                }
                                catch (const std::bad_alloc&) {
                                  result |= resultOutOfMemory;
                                }
                            }
                            ::av_frame_free(&destPic);
                        }
                        ::av_frame_free(&decodedFrame);
                    }
                    ::av_packet_unref(&encodedPacket);
                    ::avcodec_close(pCodecCtx);
                    ::avcodec_free_context(&pCodecCtx);
                }
            }
        }
        ::avformat_close_input(&pFormatContext);
    }
    if ((result & resultErr) && ::strstr(log.Current(), "get_buffer() failed"))
      result |= resultOutOfMemory;
    return result;
}

#endif // USE_QT

ImageStimulus::ImageStimulus(GUI::GraphDisplay &display) : GraphObject(display, ImageStimulusZOrder), d(new Data)
{
    d->mRenderingMode = GUI::RenderingMode::Opaque;
}

ImageStimulus::~ImageStimulus()
{
    DestructorEntered();
    delete d;
}

ImageStimulus &ImageStimulus::SetFile(const std::string &inName)
{
    ScopedLock(d);

    int result = resultOk;

    // Attempt to load the image
    std::string file = FileUtils::AbsolutePath(inName);
#if USE_QT
    delete d->mpImage;
    d->mpImage = new QImage();
    if (!d->mpImage->load(QString(file.c_str())))
        result |= resultErr;
    if (result != resultOk)
        result = LibavLoad(*d->mpImage, file);
#endif

    // An error occurred while loading the image
    if (result & resultErr)
    {
        std::string details;
        if (result & resultOutOfMemory)
            details = "out of memory";
        bcierr << "Could not load image from file \"" << inName << "\""
               << (details.empty() ? "" : ": " + details);
        delete d->mpImage;
        d->mpImage = nullptr;
        d->mFile = "";
    }
    else
        d->mFile = inName;

    Change();
    return *this;
}

std::string ImageStimulus::File() const
{
    ScopedLock(d);
    return d->mFile;
}

ImageStimulus &ImageStimulus::SetRenderingMode(int inMode)
{
    ScopedLock(d);
    if (inMode != d->mRenderingMode)
    {
        d->mRenderingMode = inMode;
        Change();
    }
    return *this;
}

int ImageStimulus::RenderingMode() const
{
    ScopedLock(d);
    return d->mRenderingMode;
}

#if USE_QT
int ImageStimulus::NativeWidth() const
{
    ScopedLock(d);
    return d->mpImage ? d->mpImage->width() : 0;
}

int ImageStimulus::NativeHeight() const
{
    ScopedLock(d);
    return d->mpImage ? d->mpImage->height() : 0;
}

void ImageStimulus::OnPaint(const GUI::DrawContext &inDC)
{
    // Draw the proper buffered image using the given DrawContext
    QPixmap *pBuffer = 0;
    WithLock(d)
    {
        if (BeingPresented())
            pBuffer = d->mpImageBufferHighlighted;
        else
            pBuffer = d->mpImageBufferNormal;
    }

    if (pBuffer != NULL)
    {
        int width = ::Floor(inDC.rect.Width()), height = ::Floor(inDC.rect.Height());
        if (width == pBuffer->width() && height == pBuffer->height())
            inDC.handle.dc->drawPixmap(::Floor(inDC.rect.left), ::Floor(inDC.rect.top), *pBuffer);
        else
            inDC.handle.dc->drawPixmap(::Floor(inDC.rect.left), ::Floor(inDC.rect.top), width, height, *pBuffer);
    }
}
#endif

void ImageStimulus::OnResize(GUI::DrawContext &ioDC)
{
    if (ScalingMode() == GUI::ScalingMode::AdjustBoth)
        OnMove(ioDC);
    else
        OnChange(ioDC);
}

void ImageStimulus::OnMove(GUI::DrawContext &ioDC)
{
    AdjustRect(ioDC.rect);
}

void ImageStimulus::OnChange(GUI::DrawContext &ioDC)
{
    ScopedLock(d);

    delete d->mpImageBufferNormal;
    d->mpImageBufferNormal = nullptr;
    delete d->mpImageBufferHighlighted;
    d->mpImageBufferHighlighted = nullptr;

    AdjustRect(ioDC.rect);
    int width = ioDC.rect.Width(), height = ioDC.rect.Height();

#if USE_QT

    if (d->mpImage != NULL)
    {
        bool storeScaled = width * height < 2 * OriginalWidth() * OriginalHeight();
        storeScaled |= width < OriginalWidth();
        storeScaled |= height < OriginalHeight();
        QImage img = storeScaled ? d->mpImage->scaled(width, height) : *d->mpImage;
        // Create the normal pixmap
        if (PresentationMode() != ShowHide)
            d->mpImageBufferNormal = NewBufferFromImage(img, d->mRenderingMode == GUI::RenderingMode::Transparent);
        // Create the highlighted pixmap by modifying img
        switch (PresentationMode())
        {
        case ShowHide:
            delete d->mpImageBufferNormal;
            d->mpImageBufferNormal = nullptr;
            break;

        case Intensify:
            img = img.convertToFormat(QImage::Format_Indexed8);
            for (int i = 0; i < img.colorCount(); ++i)
                img.setColor(i, QColor(img.color(i)).lighter(static_cast<int>(100 * DimFactor())).rgb());
            break;

        case Grayscale:
            img = img.convertToFormat(QImage::Format_Indexed8);
            for (int i = 0; i < img.colorCount(); ++i)
                img.setColor(i, QColor(img.color(i)).value());
            break;

        case Invert:
            img.invertPixels();
            break;

        case Dim:
            img = img.convertToFormat(QImage::Format_Indexed8);
            for (int i = 0; i < img.colorCount(); ++i)
                img.setColor(i, QColor(img.color(i)).darker(static_cast<int>(100 * DimFactor())).rgb());
            break;

        case HideShow:
            break;
        }
        if (PresentationMode() != HideShow)
        {
            d->mpImageBufferHighlighted = NewBufferFromImage(img, d->mRenderingMode == GUI::RenderingMode::Transparent);
        }
        else
        {
            delete d->mpImageBufferHighlighted;
            d->mpImageBufferHighlighted = nullptr;
        }
    }

#endif // USE_QT
}

void ImageStimulus::AdjustRect(GUI::Rect &ioRect) const
{
    ScopedLock(d);
    if (d->mpImage != nullptr)
    {
        int width = ioRect.Width(), height = ioRect.Height(), hCenter = (ioRect.left + ioRect.right) / 2,
            vCenter = (ioRect.bottom + ioRect.top) / 2, imageWidth = NativeWidth(), imageHeight = NativeHeight();

        switch (ScalingMode())
        {
        case GUI::ScalingMode::AdjustWidth:
            width = (imageWidth * height) / imageHeight;
            ioRect.left = hCenter - width / 2;
            ioRect.right = ioRect.left + width;
            break;

        case GUI::ScalingMode::AdjustHeight:
            height = (imageHeight * width) / imageWidth;
            ioRect.top = vCenter - height / 2;
            ioRect.bottom = ioRect.top + height;
            break;

        case GUI::ScalingMode::AdjustBoth:
            width = imageWidth;
            height = imageHeight;
            ioRect.left = hCenter - width / 2;
            ioRect.right = ioRect.left + width;
            ioRect.top = vCenter - height / 2;
            ioRect.bottom = ioRect.top + height;
            break;

        case GUI::ScalingMode::AdjustNone:
        default:;
        }
    }
}
