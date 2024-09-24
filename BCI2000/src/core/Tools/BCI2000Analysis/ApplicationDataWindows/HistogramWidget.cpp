////////////////////////////////////////////////////////////////////////////////
// $Id: HistogramWidget.cpp 8270 2024-07-23 19:34:53Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A widget that displays a histogram from analysis data.
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
#include "HistogramWidget.h"

#include "AnalysisData.h"

#include <QMouseEvent>
#include <QPainter>
#include <QToolTip>

struct HistogramWidget::Private : QObject
{
    Private(QObject *parent) : QObject(parent)
    {
    }
    int pointToIndex(const QPointF& p) const
    {
        if (p.x() < mDataRect.left())
            return -1;
        return ::floor((p.x() - mDataRect.left()) * mHistogram1.size() / mDataRect.width());
    }
    double indexToValue(int i) const
    {
        return mMin + i * mStep;
    }

    QRectF mDataRect;
    double mMin = 0, mMax = 0, mStep = 1;
    std::vector<int> mHistogram1, mHistogram2;
    RGBColor mColor1 = 0, mColor2 = 0;
    int mNumBins = 2, mMaxHeight = 0;
};

HistogramWidget::HistogramWidget(QWidget *parent)
: QWidget(parent), p(new Private(this))
{
}

void HistogramWidget::setData(const AnalysisData &data, int dimension, int flags, int numBins)
{
    if (flags & integrated)
    {
        AnalysisData integratedData = data;
        integratedData.integrateTrials();
        setData(integratedData, dimension, flags & ~integrated, numBins);
        return;
    }

    Assert(numBins > 1);
    p->mNumBins = numBins;
    std::vector<const std::vector<GenericSignal>*> collections;
    if (flags & includeTrainingData)
    {
      collections.push_back(&data.trainingData1);
      collections.push_back(&data.trainingData2);
    }
    if (flags & includeTestData)
    {
        collections.push_back(&data.testData1);
        collections.push_back(&data.testData2);
    }

    Assert(dimension >= 0);
    double min = +Inf<double>(), max = -Inf<double>();
    for (const auto pCollection : collections)
    {
        for (const auto& block : *pCollection)
        {
            if(block.Channels() > dimension)
            {
                for (int i = 0; i < block.Elements(); ++i)
                {
                    min = std::min(min, block(dimension, i));
                    max = std::max(max, block(dimension, i));
                }
            }
        }
    }
    p->mMin = min;
    p->mMax = max;
    Assert(numBins > 0);
    p->mStep = (max - min) / (numBins - 1);
    p->mHistogram1.clear();
    p->mHistogram1.resize(numBins, 0);
    p->mHistogram2.clear();
    p->mHistogram2.resize(numBins, 0);

    if (flags & includeTrainingData)
    {
        for (const auto& block : data.trainingData1)
        {
            if (block.Channels() > dimension)
            {
                for (int i = 0; i < block.Elements(); ++i)
                {
                    int idx = p->mStep == 0 ? numBins/2 : (block(dimension, i) - p->mMin) / p->mStep;
                    ++p->mHistogram1[idx];
                }
            }
        }
        for (const auto& block : data.trainingData2)
        {
            if (block.Channels() > dimension)
            {
                for (int i = 0; i < block.Elements(); ++i)
                {
                    int idx = p->mStep == 0 ? numBins / 2 : (block(dimension, i) - p->mMin) / p->mStep;
                    ++p->mHistogram2[idx];
                }
            }
        }
    }
    if (flags & includeTestData)
    {
        for (const auto& block : data.testData1)
        {
            if (block.Channels() > dimension)
            {
                for (int i = 0; i < block.Elements(); ++i)
                {
                    int idx = p->mStep == 0 ? numBins / 2 : (block(dimension, i) - p->mMin) / p->mStep;
                    ++p->mHistogram1[idx];
                }
            }
        }
        for (const auto& block : data.testData2)
        {
            if (block.Channels() > dimension)
            {
                for (int i = 0; i < block.Elements(); ++i)
                {
                    int idx = p->mStep == 0 ? numBins / 2 : (block(dimension, i) - p->mMin) / p->mStep;
                    ++p->mHistogram2[idx];
                }
            }
        }
    }

    int maxHeight = 0;
    for (auto * pHistogram : { &p->mHistogram1, &p->mHistogram2 })
        for(const auto& value : *pHistogram)
            maxHeight = std::max(value, maxHeight);
    p->mMaxHeight = maxHeight;

    update();
}

void HistogramWidget::setColors(RGBColor color1, RGBColor color2)
{
    p->mColor1 = color1;
    p->mColor2 = color2;
    update();
}

void HistogramWidget::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
    QSize textSize = fontMetrics().size(0, QString::number(p->mMaxHeight) + " ");
    QRectF paintRect = rect().adjusted(textSize.width(), textSize.height(), 0, -textSize.height());
    p->mDataRect = paintRect;
    p->mDataRect.setHeight(rect().height());

    const int cThickness = 2;
    QRectF xaxis(0, rect().height() - textSize.height(), rect().width(), cThickness);
    painter.fillRect(xaxis, RGBColor(RGBColor::Aqua).ToQRgb());
    QRectF yaxis(textSize.width() - cThickness, 0, cThickness, rect().height());
    painter.fillRect(yaxis, RGBColor(RGBColor::Aqua).ToQRgb());

    painter.setPen(RGBColor(RGBColor::Aqua).ToQRgb());
    painter.drawText(1, textSize.height(), QString::number(p->mMaxHeight));
    painter.drawText(textSize.width(), rect().height() - 1, QString::number(p->mMin));
#if 0
    if (p->mMax > 0 && p->mMin < 0)
    {
        QString zerotext = "0";
        QSize zerosize = fontMetrics().size(0, zerotext);
        float zeropos = (rect().width() - textSize.width()) * p->mMin / (p->mMin - p->mMax) + textSize.width();
        painter.drawText(zeropos - zerosize.width() / 2.0, rect().height() - 1, zerotext);
    }
#endif
    textSize = fontMetrics().size(0, QString::number(p->mMax));
    painter.drawText(rect().width() - textSize.width(), rect().height() - 1, QString::number(p->mMax));

    float yscale = 1.0 * paintRect.height() / p->mMaxHeight;
    float xscale = 1.0 * paintRect.width() / p->mHistogram1.size();
    for (int i = 0; i < p->mHistogram1.size(); ++i)
    {
        float x = i * xscale, height = p->mHistogram1[i] * yscale,
            top = (p->mMaxHeight - p->mHistogram1[i]) * yscale;
        QRectF r(paintRect.left() + x, paintRect.top() + top, xscale/2, height);
        painter.fillRect(r, p->mColor1.ToQRgb());
    }
    for (int i = 0; i < p->mHistogram2.size(); ++i)
    {
        float x = i * xscale, height = p->mHistogram2[i] * yscale,
            top = (p->mMaxHeight - p->mHistogram2[i]) * yscale;
        QRectF r(paintRect.left() + x + xscale/2, paintRect.top() + top, xscale/2, height);
        painter.fillRect(r, p->mColor2.ToQRgb());
    }
}

void HistogramWidget::mousePressEvent(QMouseEvent *ev)
{
    QPointF pos = ev->pos();
    int idx = p->pointToIndex(pos);
    if (idx < 0)
        return;

    float value = p->indexToValue(idx);

    // Readability against yellow tooltip background
    auto color1 = p->mColor1, color2 = p->mColor2;
    for (auto pColor : { &color1, &color2 })
        if (*pColor == RGBColor::White || *pColor == RGBColor::Yellow)
            *pColor = RGBColor::Black;

    QString text = QString("%1, <font color='#%3'>%2</font>, <font color='#%5'>%4</font>")
        .arg(value)
        .arg(p->mHistogram1[idx]).arg(color1, 6, 16, QChar('0'))
        .arg(p->mHistogram2[idx]).arg(color2, 6, 16, QChar('0'));

    QToolTip::showText(ev->globalPos(), text, this);
}

void HistogramWidget::mouseReleaseEvent(QMouseEvent *)
{
    QToolTip::hideText();
}

QSize HistogramWidget::minimumSizeHint() const
{
    int charWidth = fontMetrics().averageCharWidth();
    return QSize(
        p->mNumBins * charWidth / 2, 
        100
    );
}
