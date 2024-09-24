//////////////////////////////////////////////////////////////////////////////
// $Id: VideoWidget.cpp 7957 2024-03-25 18:38:10Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A widget to display a recorded webcam file.
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
//////////////////////////////////////////////////////////////////////////////
#include "VideoWidget.h"
#include <QPaintEvent>
#include <QPainter>
#include <QResizeEvent>

VideoWidget::VideoWidget(QWidget *inParent) : QWidget(inParent), mNativeWidth(0), mNativeHeight(0)
{
}

VideoWidget &VideoWidget::setFramePosition(int64_t inFrame)
{
    Ratio r = mDecoder.BofPosition().rate / mDecoder.InputFrameRate();
    int64_t tick = (inFrame * r).Ceil();
    mDecoder.SeekToTick(tick);
    this->update();
    return *this;
}

VideoWidget &VideoWidget::setVideoFile(const std::string &inFile)
{
    mDecoder.BeginConfiguration();
    if (mDecoder.Open(inFile, AVDecoder::Video, 0).Error().Empty())
    {
        mNativeWidth = mDecoder.FrameDimension(0);
        mNativeHeight = mDecoder.FrameDimension(1);
        resize(mNativeWidth, mNativeHeight);
    }
    mDecoder.EndConfiguration();
    mDecoder.DecodeToBuffer();
    this->update();
    return *this;
}

void VideoWidget::paintEvent(QPaintEvent *)
{
    int width = mDecoder.FrameDimension(0), height = mDecoder.FrameDimension(1);
    int x = 0, y = 0;
    if (width < this->width())
        x = (this->width() - width) / 2;
    if (height < this->height())
        y = (this->height() - height) / 2;
    AVDecoder::Chunk frame = mDecoder.GetFrames(1);
    const uint32_t *p, *pEnd;
    if (!frame || frame.GetData(p, pEnd) != width * height)
        return;
    QImage img(reinterpret_cast<const uchar *>(p), width, height, QImage::Format_RGB32);
    QPainter painter(this);
    painter.fillRect(0, 0, this->width(), this->height(), Qt::black);
    painter.drawImage(x, y, img);
}

void VideoWidget::resizeEvent(QResizeEvent *pEvent)
{
    mDecoder.BeginConfiguration();
    int w1 = pEvent->size().width(), h1 = pEvent->size().height();
    if (mNativeWidth > 0 && mNativeHeight > 0)
    {
        int h2 = w1 * mNativeHeight / mNativeWidth, w2 = h1 * mNativeWidth / mNativeHeight;
        if (h2 < h1)
        {
            mDecoder.SetFrameDimension(0, w1);
            mDecoder.SetFrameDimension(1, h2);
        }
        else
        {
            mDecoder.SetFrameDimension(0, w2);
            mDecoder.SetFrameDimension(1, h1);
        }
    }
    mDecoder.EndConfiguration();
}
