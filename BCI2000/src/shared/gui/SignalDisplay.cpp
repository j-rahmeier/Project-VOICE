////////////////////////////////////////////////////////////////////////////////
// $Id: SignalDisplay.cpp 8268 2024-07-23 16:38:37Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A SignalDisplay class that renders GenericSignal data into a
//   given DisplayContext.
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
#include "SignalDisplay.h"

#include "Exception.h"
#include "FastConv.h"
#include "Tree.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <algorithm>

#include <QPainter>
#include <QWidget>

#undef TEST_UPDATE_RGN

////////////////////////////////////////////////////////////////////////////////
const RGBColor SignalDisplay::cAxisColorDefault = RGBColor::Aqua;
const RGBColor SignalDisplay::cChannelColorsDefault[] = {RGBColor::White, RGBColor::White,  RGBColor::White,
                                                         RGBColor::White, RGBColor::Yellow, ColorList::End};

SignalDisplay::SignalDisplay()
    : mDataWidth(0), mDataHeight(0), mDataLeft(cLabelWidth), mLabelWidth(cLabelWidth), mMarkerHeight(0),
      mDisplayMode(polyline), mShowCursor(false), mWrapAround(false), mTimeLabels(false), mShowNumericValues(false),
      mShowBaselines(false), mShowChannelLabels(false), mShowValueUnit(false), mDisplayColors(true), mInverted(false),
      mNumSamples(cNumSamplesDefault), mSampleCursor(0), mNumDisplayGroups(0), mMaxDisplayGroups(0),
      mTopGroup(0), mChannelGroupSize(1), mMarkerChannels(0), mTickLength(4), mCursorWidth(3),
      mCursorColor(RGBColor::Yellow),
      mFontSize(11), mNumericValueWidth(0), mMinValue(cMinValueDefault), mMaxValue(cMaxValueDefault),
      mAxisColor(cAxisColorDefault), mChannelColors(cChannelColorsDefault), mParent(nullptr), mpSignalPoints(nullptr),
      mSymbolWidth(cLabelWidth), mXTicksVisible(true), mYTicksVisible(true), mFastScrolling(false), mAutoDecimationMaxLinesPerPixel(0)
{
}

SignalDisplay::~SignalDisplay()
{
    delete[] mpSignalPoints;
}

namespace
{

void SetUnitFromString(const std::string &s, PhysicalUnit &u)
{
    if (::atof(s.c_str()))
        u.SetGainWithSymbol(s);
    else
        u.SetSymbol(s);
}

float Dither(float f)
{
    // modf() returns signed results, which we don't want
    float intpart = ::floor(f),
          fracpart = f - intpart;
    if (::rand() < static_cast<float>(RAND_MAX) * fracpart)
        intpart += 1;
    return intpart;
}

struct MarkerDef
{
    std::string label;
    QColor color;
    Qt::PenStyle style;
    float width;
};

Qt::PenStyle ParsePenStyle(const std::string& style)
{
    if (style == "-")
        return Qt::SolidLine;
    else if (style == "." || style == "..")
        return Qt::DotLine;
    else if (style == "--")
        return Qt::DashLine;
    else if (style == ".-" || style == "-.")
        return Qt::DashDotLine;
    else if (style == ".-." || style == "-.." || style == "..-")
        return Qt::DashDotDotLine;
    return Qt::NoPen;
}

MarkerDef ParseMarkerDef(const std::string& markerdef, bool inverted)
{
    auto def = StringUtils::Split("|", markerdef);
    Assert(!def.empty());
    // https://www.w3.org/TR/SVG/types.html#ColorKeywords
    if (def.size() < 2)
        def.push_back("");
    if (def[1].empty())
        def[1] = "white";
    if (def.size() < 3)
        def.push_back("");
    if (def.size() < 4)
        def.push_back("1");
    if (inverted)
    {
        if (def[1] == "white")
            def[1] = "black";
        else if (def[1] == "black")
            def[1] = "white";
    }
    QColor color(def[1].c_str());
    Qt::PenStyle style = ParsePenStyle(def[2]);
    float width = ::atof(def[3].c_str());
    return MarkerDef{def[0], color, style, width};
}

} // namespace

SignalDisplay &SignalDisplay::SetChannelUnit(const std::string &s)
{
    SetUnitFromString(s, mChannelUnit);
    InvalidateWidget();
    return *this;
}

SignalDisplay& SignalDisplay::SetMinValues(const std::vector<double>& v)
{
    if (v != mMinValues)
    {
        mMinValues = v;
        return Invalidate();
    }
    return *this;
}

const std::vector<double>& SignalDisplay::MinValues() const
{
    return mMinValues;
}

SignalDisplay& SignalDisplay::SetMaxValues(const std::vector<double>& v)
{
    if (v != mMaxValues)
    {
        mMaxValues = v;
        return Invalidate();
    }
    return *this;
}

const std::vector<double>& SignalDisplay::MaxValues() const
{
    return mMaxValues;
}

SignalDisplay &SignalDisplay::SetSampleUnit(const std::string &s)
{
    SetUnitFromString(s, mSampleUnit);
    mTimeLabels = (mSampleUnit.Symbol() == ":s");
    InvalidateWidget();
    return *this;
}

SignalDisplay &SignalDisplay::SetValueUnit(const std::string &s)
{
    SetUnitFromString(s, mValueUnit);
    InvalidateWidget();
    return *this;
}

SignalDisplay& SignalDisplay::SetValueUnits(const ValueList<std::string>& units)
{
    mValueUnits.clear();
    mValueUnits.reserve(units.size());
    for (const auto& unit : units)
    {
        PhysicalUnit u;
        SetUnitFromString(unit, u);
        mValueUnits.push_back(u);
    }
    InvalidateWidget();
    return *this;
}

ValueList<std::string> SignalDisplay::ValueUnits() const
{
    ValueList<std::string> units;
    units.reserve(mValueUnits.size());
    for (const auto& unit : mValueUnits)
        units.push_back(unit.GainWithSymbol());
    return units;
}

SignalDisplay& SignalDisplay::SetChannelVisible(int ch, bool visible)
{
    if (ch < 0)
        return *this;

    if (ch >= mChannelVisible.size())
        mChannelVisible.resize(ch + 1, true);

    if (visible != mChannelVisible[ch])
    {
        mChannelVisible[ch] = visible;
        RebuildChannelGroups();
        AdjustDisplayGroups();
        Invalidate();
    }
    return *this;
}

bool SignalDisplay::ChannelVisible(int inCh) const
{
    if (inCh < 0)
        return false;
    if (inCh >= mChannelVisible.size())
        return true;
    return mChannelVisible[inCh];
}

SignalDisplay& SignalDisplay::SetChannelsVisible(const ValueList<bool>& channelsVisible)
{
    if (channelsVisible != mChannelVisible)
    {
        mChannelVisible = channelsVisible;
        RebuildChannelGroups();
        AdjustDisplayGroups();
        Invalidate();
    }
    return *this;
}

ValueList<bool> SignalDisplay::ChannelsVisible() const
{
    ValueList<bool> list;
    list.reserve(mChannelVisible.size());
    for (bool b : mChannelVisible)
        list.push_back(b);
    return list;
}

std::string SignalDisplay::ValueColors() const
{
    Tree tree;
    int i = 0;
    for (const auto &pair : mValueColors)
    {
        tree[i][0].Data() << pair.first;
        Tree &c = tree[i][1];
        c[0].Data() << pair.second.R();
        c[1].Data() << pair.second.G();
        c[2].Data() << pair.second.B();
        ++i;
    }
    return tree.ToString();
}

SignalDisplay &SignalDisplay::SetValueColors(std::string s)
{
    mValueColors.clear();
    Tree tree;
    if (tree.Parse(s))
    {
        for (const auto &entry : tree.Children())
        {
            double value;
            entry[0].Data() >> value;
            RGBColor color;
            if (entry[1].Size() == 1)
            {
                entry[1].Data() >> color;
            }
            else
            {
                int rgb[3] = { 0 };
                for (int i = 0; i < 3; ++i)
                    entry[1][i].Data() >> rgb[i];
                color = RGBColor(rgb[0], rgb[1], rgb[2]);
            }
            mValueColors.push_back(std::make_pair(value, color));
        }
        std::sort(mValueColors.begin(), mValueColors.end());
    }
    return Invalidate();
}

SignalDisplay &SignalDisplay::SetContext(const GUI::DrawContext &dc)
{
    mDisplayRect = QRect(ToInt(dc.rect.left), ToInt(dc.rect.top), ToInt(dc.rect.right - dc.rect.left),
                         ToInt(dc.rect.bottom - dc.rect.top));
    mDisplayRgn = mDisplayRect;
    if (mImageBuffer.size() != mDisplayRect.size())
    {
        mImageBuffer = QPixmap(mDisplayRect.size());
        InvalidateBuffer();
    }
    mParent = dc.device;
    InvalidateWidget();
    return *this;
}

void SignalDisplay::InvalidateBuffer(const void* inRegion)
{
    std::lock_guard<std::recursive_mutex> lock(mBufferMutex);
    const QRegion *pRegion = static_cast<const QRegion*>(inRegion);
    if (!pRegion)
        mInvalidBufferRgn = mDisplayRect;
    else
        mInvalidBufferRgn += *pRegion;
}

void SignalDisplay::InvalidateWidget(const void *inRegion)
{
    QWidget *pWidget = dynamic_cast<QWidget *>(mParent);
    if (pWidget)
    {
        if (!inRegion)
            pWidget->update(mDisplayRect);
        else
            pWidget->update(*static_cast<const QRegion*>(inRegion));
    }
}

SignalDisplay& SignalDisplay::Invalidate(const void* inRegion)
{
    InvalidateBuffer(inRegion);
    InvalidateWidget(inRegion);
    return *this;
}

void SignalDisplay::AdaptTo(const GenericSignal &inSignal)
{
    // Any changes in signal size that we must react to?
    bool reconfigure = false;
    if (inSignal.Elements() > mNumSamples)
    {
        std::lock_guard<std::recursive_mutex> lock(mDataMutex);
        SetNumSamples(inSignal.Elements());
        reconfigure = true;
    }
    if (inSignal.Channels() != mData.Channels())
    {
        std::lock_guard<std::recursive_mutex> lock(mDataMutex);
        mChannelVisible.resize(inSignal.Channels() - mMarkerChannels, true);
        RebuildChannelGroups();
        int newNumDisplayGroups = mChannelGroups.size();
        if (mMaxDisplayGroups == 0)
            mMaxDisplayGroups = std::min<int>(newNumDisplayGroups, cInitialMaxDisplayGroups);
        else if (newNumDisplayGroups < mMaxDisplayGroups)
            mMaxDisplayGroups = newNumDisplayGroups;
        reconfigure = true;
    }
    if (reconfigure)
    {
        std::lock_guard<std::recursive_mutex> lock(mDataMutex);
        mData = GenericSignal(SignalProperties(inSignal.Channels(), mNumSamples), GenericSignal::NaN);
        SetDisplayGroups(mMaxDisplayGroups);
        SyncLabelWidth();
        mSampleCursor = 0;
        Invalidate();
    }
}

SignalDisplay &SignalDisplay::WrapForward(const GenericSignal &inSignal)
{
    if (mParent == nullptr)
        return *this;

    AdaptTo(inSignal);
    std::lock_guard<std::recursive_mutex> lock(mDataMutex);
    mShowCursor = (inSignal.Elements() < mNumSamples);

    for (int i = 0; i < inSignal.Channels(); ++i)
        for (int j = 0; j < inSignal.Elements(); ++j)
            mData(i, (mSampleCursor + j) % mData.Elements()) = inSignal(i, j);

    SyncGraphics();

    int firstInvalidSample = mSampleCursor, firstValidSample = mSampleCursor + inSignal.Elements();
    bool wrappingAround = false;
    if (mNumSamples > 0)
    {
        wrappingAround = (firstValidSample / mNumSamples > 0);
        mWrapAround |= wrappingAround;
        wrappingAround |= bool(mSampleCursor == 0);
        mSampleCursor = firstValidSample % mNumSamples;
    }

    long firstInvalidPixel = mDataRect.left(), firstValidPixel = mDataRect.right();

    switch (mDisplayMode)
    {
    case polyline:
        firstInvalidPixel = SampleLeft(firstInvalidSample - 1);
        firstValidPixel = SampleLeft(firstValidSample + 1);
        break;
    case field2d:
        firstInvalidPixel = SampleLeft(firstInvalidSample);
        firstValidPixel = SampleLeft(firstValidSample);
        break;
    default:
        Assert(false);
    }

    QWidget *pWindow = dynamic_cast<QWidget *>(mParent);
    if (pWindow)
    {
        QRegion invalidRgn;

        invalidRgn += mPrevCursorRect;
        invalidRgn += CursorRect(mSampleCursor);

        QRect invalidRect = mDataRect;

        // The non-wrapped area.
        invalidRect.setLeft(std::max<int>(firstInvalidPixel, mDataRect.left()) - mSymbolWidth / 2);
        invalidRect.setRight(std::min<int>(firstValidPixel, mDataRect.right()));
        if (invalidRect.left() < invalidRect.right())
            invalidRgn += invalidRect;

        // The area wrapped around the left edge.
        invalidRect.setLeft(std::max<int>(firstInvalidPixel + mDataWidth, mDataRect.left()) - mSymbolWidth / 2);
        invalidRect.setRight(std::min<int>(firstValidPixel + mDataWidth, mDataRect.right()));
        if (invalidRect.left() < invalidRect.right())
            invalidRgn += invalidRect;

        // The area wrapped around the right edge.
        invalidRect.setLeft(std::max<int>(firstInvalidPixel - mDataWidth, mDataRect.left()) - mSymbolWidth / 2);
        invalidRect.setRight(std::min<int>(firstValidPixel - mDataWidth, mDataRect.right()));
        if (invalidRect.left() < invalidRect.right())
            invalidRgn += invalidRect;

        if (mShowNumericValues)
        {
            invalidRect.setLeft(NumericValuesLeft());
            invalidRect.setRight(NumericValuesRight());
            invalidRgn += invalidRect;
        }
        Invalidate(&invalidRgn);
    }
    return *this;
}

SignalDisplay &SignalDisplay::ScrollForward(const GenericSignal &inSignal)
{
    if (mParent == nullptr)
        return *this;

    if (inSignal.Elements() < 1)
        return *this;

    AdaptTo(inSignal);
    std::lock_guard<std::recursive_mutex> lock(mDataMutex);
    mShowCursor = false;
    // Shift data to the left, and then append the input signal.
    GenericSignal::ValueType* pData = mData.MutableData();
    for (int i = 0; i < inSignal.Channels(); ++i)
    {
        int j = 0, k = inSignal.Elements();
        while (j < mData.Elements() - inSignal.Elements())
            pData[mData.LinearIndex(i, j++)] = pData[mData.LinearIndex(i, k++)];
        k = 0;
        while (j < mData.Elements())
            pData[mData.LinearIndex(i, j++)] = inSignal(i, k++);
    }

    if (mFastScrolling)
    {
        std::lock_guard<std::recursive_mutex> lock(mBufferMutex);
        SyncGraphics();
        float delta = SampleLeft(mData.Elements()) - SampleLeft(mData.Elements() - inSignal.Elements());
        delta = Dither(delta);
        {
            QPainter painter(&mImageBuffer);
            QRectF clipRect(mDataLeft, 0, mDataWidth, mDisplayRect.height());
            painter.setClipRect(clipRect);
            painter.drawPixmap(-delta, 0, mImageBuffer);
        }

        QRect redrawRect = mDataRect;
        redrawRect.setLeft(SampleLeft(mData.Elements() - inSignal.Elements() - 1));
        QRegion redrawRegion = redrawRect;
        PaintToBuffer(&redrawRegion);
        InvalidateWidget();
    }
    else
    {
        Invalidate();
    }
    return *this;
}

SignalDisplay &SignalDisplay::ScrollBack(const GenericSignal &inSignal)
{
    if (mParent == nullptr)
        return *this;

    if (inSignal.Elements() < 1)
        return *this;

    AdaptTo(inSignal);
    std::lock_guard<std::recursive_mutex> lock(mDataMutex);
    mShowCursor = false;
    // Shift data to the right, and then prepend the input signal.
    GenericSignal::ValueType* pData = mData.MutableData();
    for (int i = 0; i < inSignal.Channels(); ++i)
    {
        int j = mData.Elements(), k = mData.Elements() - inSignal.Elements();
        while (k > 0)
            pData[mData.LinearIndex(i, --j)] = pData[mData.LinearIndex(i, --k)];
        k = inSignal.Elements();
        while (j > 0)
            pData[mData.LinearIndex(i, --j)] = inSignal(i, --k);
    }

    if (mFastScrolling)
    {
        std::lock_guard<std::recursive_mutex> lock(mBufferMutex);
        SyncGraphics();
        float delta = SampleLeft(inSignal.Elements()) - SampleLeft(0);
        delta = Dither(delta);
        {
            QPainter painter(&mImageBuffer);
            painter.drawPixmap(delta, 0, mImageBuffer);
        }

        QRect redrawRect = mDataRect;
        redrawRect.setRight(SampleLeft(inSignal.Elements() + 1));
        QRegion redrawRegion = redrawRect;
        PaintToBuffer(&redrawRegion);
        InvalidateWidget();
    }
    else
    {
        Invalidate();
    }
    return *this;
}

QFont SignalDisplay::AxisFont()
{
    QFont font;
    font.setPixelSize(mFontSize);
    font.setStyleHint(QFont::SansSerif, QFont::PreferBitmap);
    return font;
}

QFont SignalDisplay::MonoFont()
{
    QFont font;
    font.setPixelSize(mFontSize);
    font.setStyleHint(QFont::Monospace, QFont::PreferBitmap);
    return font;
}

QFont SignalDisplay::SymbolFont()
{
    QFont font;
    font.setPixelSize(mFontSize);
    font.setStyleHint(QFont::SansSerif, QFont::PreferBitmap);
    return font;
}

void SignalDisplay::SyncLabelWidth()
{
    mChannelNameCache.clear();
    if (!mParent)
    {
        mTickLength = cTickLength;
        mLabelWidth = cLabelWidth;
        mMarkerHeight = mLabelWidth;
    }
    else
    {
        QFontMetrics fontMetrics(AxisFont());
        std::ostringstream oss;
        oss << mData.Channels();
        QSize size = fontMetrics.size(0, oss.str().c_str());
        mLabelWidth = 0;
        mMarkerHeight = size.height();
        mTickLength = cTickLength;

        if (mYTicksVisible || mChannelGroupSize > 1)
        {
            mLabelWidth = size.width();
            mMarkerHeight = size.height();
            mTickLength = Round(size.height() / 3.0);
            if (mShowChannelLabels)
            {
                for (size_t i = 0; i < mChannelLabels.size(); ++i)
                {
                    int address = mChannelLabels[i].Address();
                    if (address >= int(mChannelNameCache.size()))
                        mChannelNameCache.resize(address + 1, "");
                    const std::string &label = mChannelLabels[i].Text();
                    mChannelNameCache[address] = label;
                    if (address >= mData.Channels() - mMarkerChannels)
                        mChannelNameCache[address] += " ";
                }
            }
            for (int i = 0; i < mData.Channels(); ++i)
            {
                if (int(mChannelNameCache.size()) <= i)
                    mChannelNameCache.resize(i + 1);
                if (mChannelNameCache[i].empty())
                {
                    std::ostringstream oss;
                    if (mChannelUnit.Symbol().empty())
                        oss << i + cChannelBase;
                    else
                        oss << std::fixed << std::setprecision(1) << mChannelUnit.RawToPhysical(i).Utf8();
                    mChannelNameCache[i] = oss.str();
                }
                const std::string &label = mChannelNameCache[i];
                size = fontMetrics.size(0, label.c_str());
                if (size.width() > mLabelWidth)
                    mLabelWidth = size.width();
            }
        }
        for (const auto& marker : mYAxisMarkers)
        {
            auto def = ParseMarkerDef(marker.Text(), mInverted);
            size = fontMetrics.size(0, def.label.c_str());
            if (size.width() > mLabelWidth)
                mLabelWidth = size.width();
        }
        if (!mChannelLabelWidth.empty())
            mLabelWidth = fontMetrics.size(0, mChannelLabelWidth.c_str()).width();
        mLabelWidth += 3 * cAxisWidth + cTickWidth;
    }
    if (!mShowNumericValues)
        mNumericValueWidth = 0;
    mDataLeft = mLabelWidth + mNumericValueWidth + cAxisWidth + mTickLength;
    if (mShowNumericValues && mChannelGroupSize == 1 && mYTicksVisible)
        mDataLeft += cAxisWidth;
}

inline void SignalDisplay::SyncGraphics()
{
    mDataRect.setLeft(mDataLeft);
    mDataRect.setTop(0);
    mDataRect.setRight(mDisplayRect.right() - mDisplayRect.left());
    mDataRect.setBottom(mDisplayRect.bottom() - mDisplayRect.top());
    mDataWidth = std::max<int>(0, mDataRect.right() - mDataRect.left());
    mDataHeight = std::max<int>(0, mDataRect.bottom() - mDataRect.top() - mMarkerHeight * mMarkerChannels - mFontSize -
                                       cAxisWidth - 2 * mTickLength);
}

SignalDisplay &SignalDisplay::SetDisplayGroups(int inDisplayGroups)
{
    mMaxDisplayGroups = inDisplayGroups;
    AdjustDisplayGroups();
    return *this;
}

void SignalDisplay::AdjustDisplayGroups()
{
    mNumDisplayGroups = mMaxDisplayGroups;
    if (mMaxDisplayGroups >= mChannelGroups.size())
        mNumDisplayGroups = mChannelGroups.size();
    if (mNumDisplayGroups > mChannelGroups.size())
        mNumDisplayGroups = mChannelGroups.size();
    SetTopGroup(mTopGroup);
    Invalidate();
}

SignalDisplay &SignalDisplay::SetTopGroup(int inTopGroup)
{
    int newTopGroup = inTopGroup,
        maxTopGroup = mChannelGroups.size() - mNumDisplayGroups;
    if (newTopGroup > maxTopGroup)
        newTopGroup = maxTopGroup;
    if (newTopGroup < 0)
        newTopGroup = 0;
    if (newTopGroup != mTopGroup)
    {
        mTopGroup = newTopGroup;
        Invalidate();
    }
    return *this;
}

SignalDisplay &SignalDisplay::SetDisplayMode(int mode)
{
    if (mode != mDisplayMode)
    {
        mDisplayMode = eDisplayMode(mode);
        Invalidate();
    }
    return *this;
}

SignalDisplay &SignalDisplay::SetNumSamples(int inNumSamples)
{
    int newNumSamples = inNumSamples;
    if (newNumSamples < 0)
        newNumSamples = 0;
    if (newNumSamples != mNumSamples)
    {
        std::lock_guard<std::recursive_mutex> lock(mDataMutex);
        GenericSignal newData(mData.Channels(), newNumSamples);
        int idx1 = mSampleCursor, idx2 = newNumSamples - mNumSamples;
        if (idx2 < 0)
        {
            idx1 = (idx1 - idx2) % mNumSamples;
            idx2 = 0;
        }
        for (int i = 0; i < idx2; ++i)
            for (int ch = 0; ch < newData.Channels(); ++ch)
                newData(ch, i) = NaN(newData(ch, i));
        while (idx2 < newNumSamples)
        {
            for (int ch = 0; ch < newData.Channels(); ++ch)
                newData(ch, idx2) = mData(ch, idx1);
            ++idx1 %= mNumSamples;
            ++idx2;
        }
        mData = newData;
        mSampleCursor = 0;
        Invalidate();
        mNumSamples = newNumSamples;
    }
    return *this;
}

void SignalDisplay::PaintToBuffer(const void* inUpdateRgn)
{
    const QRegion* pUpdateRgn = static_cast<const QRegion*>(inUpdateRgn);
    if (pUpdateRgn && pUpdateRgn->isEmpty())
        return;

    std::lock_guard<std::recursive_mutex> dataLock(mDataMutex);
    std::lock_guard<std::recursive_mutex> bufferLock(mBufferMutex);
    PaintInfo p;
    SetupPainting(p, &mImageBuffer, inUpdateRgn);
    ClearBackground(p);

    std::vector<QRegion> rgns;
    if (inUpdateRgn)
        for (const auto& rect : *pUpdateRgn)
            rgns.push_back(rect);
    else
        rgns.push_back(mDataRect);

    for (const auto& rgn : rgns)
    {
        p.updateRgn = &rgn;
        p.painter->setClipRegion(rgn);
        switch (mDisplayMode)
        {
        case polyline:
            DrawSignalPolyline(p);
            break;
        case field2d:
            DrawSignalField2d(p);
            break;
        default:
            Assert(false);
        }
    }
    CleanupPainting(p);
}

SignalDisplay &SignalDisplay::Paint(const void *inUpdateRgn)
{
    std::lock_guard<std::recursive_mutex> lock(mBufferMutex);
    PaintToBuffer(&mInvalidBufferRgn);
    mInvalidBufferRgn = QRegion();

    if (mParent == nullptr)
        return *this;

    PaintInfo p;
    SetupPainting(p, mParent, inUpdateRgn);
    p.painter->drawPixmap(0, 0, mImageBuffer);
    if (mXTicksVisible)
        DrawXTicks(p);
    DrawAxes(p);
    switch (mDisplayMode)
    {
    case polyline:
        DrawYLabels(p, true);
        break;
    case field2d:
        DrawYLabels(p, false);
        break;
    default:
        Assert(false);
    }

#ifdef TEST_UPDATE_RGN
    p.painter->fillRect(mDisplayRect, QColor(::rand() << 8 & 0x00ffff00));
#endif // TEST_UPDATE_RGN

    DrawMarkerChannels(p);
    DrawChannelLabels(p);
    DrawCursor(p);
    DrawMarkers(p);
    if (mDisplayMode == polyline)
        DrawValueUnits(p);
    CleanupPainting(p);
    return *this;
}

void SignalDisplay::RebuildChannelGroups()
{
    mChannelGroups.clear();

    std::vector<int> group;
    for (int channel = 0; channel < mChannelVisible.size(); ++channel)
    {
        if (mChannelVisible[channel])
            group.push_back(channel);

        if (channel % mChannelGroupSize == mChannelGroupSize - 1 && !group.empty())
        {
            mChannelGroups.push_back(group);
            group.clear();
        }
    }
    if (!group.empty())
        mChannelGroups.push_back(group);
}

float SignalDisplay::ChannelTop(int ch) const
{
    return GroupTop(ChannelToGroup(ch) - mTopGroup);
}

float SignalDisplay::ChannelBottom(int ch) const
{
    return GroupBottom(ChannelToGroup(ch) - mTopGroup);
}

int SignalDisplay::PosToChannel(float p) const
{
    return GroupToChannel(PosToGroup(p) + mTopGroup);
}

int SignalDisplay::PosToGroup(float p) const
{
    if (p < GroupTop(0))
        return -1;
    if (p >= GroupBottom(mNumDisplayGroups - 1))
        return -1;
    return (p - GroupTop(0)) / (GroupTop(1) - GroupTop(0));
}

int SignalDisplay::ChannelToGroup(int inCh) const
{
    for (int group = 0; group < mChannelGroups.size(); ++group)
        for (int channel : mChannelGroups[group])
            if (channel == inCh)
                return group;
    return -1;
}

int SignalDisplay::GroupToChannel(int g) const
{
    return mChannelGroups[g].front();
}

double SignalDisplay::NormData(size_t ch, size_t el) const
{
    double minValue = mMinValues.size() > ch ? mMinValues[ch] : mMinValue;
    double maxValue = mMaxValues.size() > ch ? mMaxValues[ch] : mMaxValue;
    return (mData(ch, el) - minValue) / (maxValue - minValue);
}

std::string SignalDisplay::GetNumericValue(int ch) const
{
    if (ch >= mData.Channels() || mData.Elements() < 1)
        return " ";
    int sampleNumber = mSampleCursor - 1;
    if (sampleNumber < 0)
        sampleNumber += mData.Elements();
    double value = mData(ch, sampleNumber);

    std::ostringstream oss;
    oss.setf(std::ios::fixed);
    bool channelShowValueUnit = mValueUnitsVisible.size() > ch ? mValueUnitsVisible[ch] : false;
    if (mShowValueUnit || channelShowValueUnit)
    {
        const PhysicalUnit& unit = mValueUnits.size() > ch ? mValueUnits[ch] : mValueUnit;
        oss.precision(2);
        oss.width(6);
        if (value == Inf(value) || value == -Inf(value) || value == 0 || IsNaN(value))
            oss << Pretty(value);
        else
            oss << unit.RawToPhysical(value).Utf8();
    }
    else
    {
        oss.precision(10);
        oss << Pretty(value);
    }
    return oss.str();
}

void SignalDisplay::SetupPainting(PaintInfo &p, QPaintDevice* pDevice, const void *inUpdateRgn)
{
    SyncGraphics();

    p.updateRgn = static_cast<const QRegion *>(inUpdateRgn);
    p.painter = new QPainter(pDevice);
    if (p.updateRgn)
        p.painter->setClipRegion(*p.updateRgn);
    p.painter->setRenderHint(QPainter::Antialiasing, false);
    p.painter->setRenderHint(QPainter::TextAntialiasing, false);
    p.signalPens.resize(mData.Channels());
    p.signalBrushes.resize(mData.Channels());
    p.signalStyles.resize(mData.Channels());

    // Background properties
    RGBColor c = mInverted ? RGBColor::White : RGBColor::Black;
    p.backgroundColor = QColor(c.R(), c.G(), c.B());
    p.backgroundBrush = QBrush(p.backgroundColor);

    // Cursor properties
    c = mCursorColor;
    p.cursorColor = QColor(c.R(), c.G(), c.B());
    p.cursorBrush = QBrush(p.cursorColor);

    // Axis properties
    RGBColor axisColor = mAxisColor;
    if (mInverted && (axisColor == RGBColor::White))
        axisColor = RGBColor::Black;
    QColor qAxisColor(axisColor.R(), axisColor.G(), axisColor.B());
    p.axisBrush = QBrush(qAxisColor);
    p.axisY = GroupBottom(std::max(0, mNumDisplayGroups - 1));

    p.markerWidth = 1;
    p.baselinePen = QPen(qAxisColor);
    c = RGBColor(mInverted ? RGBColor::Black : RGBColor::White);
    p.markerColor = QColor(c.R(), c.G(), c.B());
    p.markerBrush = QBrush(p.markerColor);
    p.labelColor = qAxisColor;
    p.labelFont = AxisFont();
    p.monoFont = MonoFont();
    p.symbolFont = SymbolFont();
    mSymbolWidth = QFontMetrics(p.symbolFont).maxWidth();

    // Signal properties
    if (mDisplayColors)
        for (int i = 0; i < mData.Channels(); ++i)
        {
            RGBColor channelColor = ChannelColor(i);
            if (mInverted && channelColor == RGBColor::White)
                channelColor = RGBColor::Black;
            QColor c(channelColor.R(), channelColor.G(), channelColor.B());
            p.signalPens[i] = QPen(c);
            p.signalBrushes[i] = QBrush(c);
        }
    else
    {
        RGBColor channelColor = mInverted ? RGBColor::Black : RGBColor::White;
        QColor c(channelColor.R(), channelColor.G(), channelColor.B());
        QPen pen(c);
        for (int i = 0; i < mData.Channels(); ++i)
            p.signalPens[i] = pen;
    }
    p.signalStyles = QString::fromUtf8(mChannelStyles.c_str());
    while (p.signalStyles.length() < mData.Channels())
        p.signalStyles.append('-');
}

void SignalDisplay::ClearBackground(const PaintInfo &p)
{
    p.painter->fillRect(mDisplayRect, p.backgroundColor);
}

static void DrawLine(QPainter *p, const QString &style, const QPointF *points, int pointCount, int decimation = 1)
{
    if (style == "-")
    {
        int i = 0;
        for (i = 0; i < pointCount - decimation; i += decimation)
        {
          qreal x1 = points[i].x(), x2 = points[i + decimation].x();
          if (decimation > 1)
          {
            qreal ymin = Inf<float>(), ymax = -Inf<float>();
            for (int j = 0; j < decimation; ++j)
            {
              ymin = std::min(points[i + j].y(), ymin);
              ymax = std::max(points[i + j].y(), ymax);
            }
            p->drawLine((x1 + x2) / 2, ymin, (x1 + x2) / 2, ymax);
          }
          p->drawLine(points[i], points[i + decimation]);
        }
        if (i < pointCount - 1)
        {
          qreal x1 = points[i].x(), x2 = points[pointCount - 1].x();
          if (pointCount - i > 1)
          {
            qreal ymin = Inf<float>(), ymax = -Inf<float>();
            for (int j = 0; j < pointCount - i; ++j)
            {
              ymin = std::min(points[i + j].y(), ymin);
              ymax = std::max(points[i + j].y(), ymax);
            }
            p->drawLine((x1 + x2) / 2, ymin, (x1 + x2) / 2, ymax);
          }
          p->drawLine(points[i], points[pointCount - 1]);
        }
    }
    else
    {
        QPoint delta = p->fontMetrics().tightBoundingRect(style).center();
        for (int i = 0; i < pointCount; i += decimation)
            p->drawText(points[i] - delta, style);
    }
}

void SignalDisplay::DrawSignalPolyline(const PaintInfo &p)
{
    float baseInterval = mNumDisplayGroups > 0 ? mDataHeight / mNumDisplayGroups : mDataHeight;
    double maxY = mDataRect.bottom() + 1, minY = mDataRect.top() - 1;
    // Draw baselines.
    if (mShowBaselines)
    {
        QPoint baselinePoints[2];
        p.painter->setPen(p.baselinePen);
        baselinePoints[0].setX(SampleLeft(0));
        baselinePoints[1].setX(SampleRight(mNumSamples));
        for (int i = 0; i < mNumDisplayGroups; ++i)
        {
            baselinePoints[0].setY(ToInt(GroupBottom(i) + (baseInterval * mMinValue) / (mMaxValue - mMinValue)));
            baselinePoints[1].setY(baselinePoints[0].y());
            p.painter->drawPolyline(baselinePoints, 2);
        }
    }

    int sampleBegin = 0, sampleEnd = mNumSamples;
    if (p.updateRgn)
    { // We restrict drawing to the actually requested update region.
        QRect clipRect = p.updateRgn->boundingRect();
        sampleBegin = ::floor(PosToSample(clipRect.left()) - 2);
        sampleBegin = std::max(sampleBegin - 1, 0);
        sampleEnd = ::ceil(PosToSample(clipRect.right()) + 2);
        sampleEnd = std::min(sampleEnd + 1, mNumSamples);
    }
    delete[] mpSignalPoints;
    try
    {
        mpSignalPoints = new QPointF[mNumSamples];
    }
    catch (const std::bad_alloc &)
    {
        throw std_bad_alloc << "Could not allocate memory for " << mNumSamples << " points";
    }
    p.painter->setFont(p.symbolFont);
    int numPens = static_cast<int>(p.signalPens.size());

    int decimation = 1;
    if (mAutoDecimationMaxLinesPerPixel > 0 && mDataRect.width() > 0)
    {
        int linesPerPixel = mNumSamples / mDataRect.width();
        if (linesPerPixel > mAutoDecimationMaxLinesPerPixel)
            decimation = linesPerPixel / mAutoDecimationMaxLinesPerPixel;
    }

    for (int j = sampleBegin; j < sampleEnd; ++j)
        mpSignalPoints[j].setX(SampleLeft(j));

    for (int group = mTopGroup; group < mTopGroup + mNumDisplayGroups; ++group)
    {
        for (int channel : mChannelGroups[group])
        {
            int channelBottom = ChannelBottom(channel);
            QString style = p.signalStyles[channel % p.signalStyles.length()];
            p.painter->setPen(p.signalPens[channel % numPens]);
            int runEnd = sampleBegin;
            while (runEnd < sampleEnd)
            {
                int runBegin = runEnd;
                while (runBegin < sampleEnd && IsNaN(NormData(channel, runBegin)))
                    ++runBegin;
                double value;
                runEnd = runBegin;
                while (runEnd < sampleEnd && !IsNaN(value = NormData(channel, runEnd)))
                {
                    double y = channelBottom - 1 - baseInterval * value;
                    if (y > maxY)
                        y = maxY;
                    else if (y < minY)
                        y = minY;
                    mpSignalPoints[runEnd++].setY(y);
                }
                if (runBegin <= mSampleCursor && mSampleCursor < runEnd)
                {
                    DrawLine(p.painter, style, mpSignalPoints + runBegin, mSampleCursor - runBegin, decimation);
                    DrawLine(p.painter, style, mpSignalPoints + mSampleCursor, runEnd - mSampleCursor, decimation);
                }
                else
                {
                    DrawLine(p.painter, style, mpSignalPoints + runBegin, runEnd - runBegin, decimation);
                }
            }
        }
    }
}

void SignalDisplay::DrawSignalField2d(const PaintInfo &p)
{
    int sampleBegin = 0, sampleEnd = mNumSamples;
    if (p.updateRgn)
    { // We restrict drawing to the actually requested update region.
        QRect clipRect = p.updateRgn->boundingRect();
        sampleBegin = PosToSample(clipRect.left());
        sampleBegin = std::max(sampleBegin - 1, 0);
        sampleEnd = PosToSample(clipRect.right() + 1);
        sampleEnd = std::min(sampleEnd + 1, mNumSamples);
    }
    for (int group = mTopGroup; group < mTopGroup + mNumDisplayGroups; ++group)
    {
        for (int channel : mChannelGroups[group])
        {
            for (int sample = sampleBegin; sample < sampleEnd; ++sample)
            {
                bool draw = true;
                double dataValue = NormData(channel, sample);
                if (dataValue < 0.0)
                    dataValue = 0.0;
                else if (dataValue > 1.0)
                    dataValue = 1.0;

                QRectF dotRect(SampleLeft(sample), ChannelTop(channel), 
                               SampleRight(sample) - SampleLeft(sample),
                               ChannelBottom(channel) - ChannelTop(channel)
                               );
                RGBColor rgb;
                if (IsNaN(dataValue))
                    rgb = RGBColor::Black;
                else if (!mValueColors.empty())
                {
                    typedef decltype(*mValueColors.begin()) pair;
                    auto i = std::lower_bound(mValueColors.begin(), mValueColors.end(), dataValue,
                                              [](pair &a, double b) { return a.first < b; });
                    if (i == mValueColors.begin())
                        rgb = i->second;
                    else if (i == mValueColors.end())
                        rgb = mValueColors.back().second;
                    else
                    {
                        auto j = i--;
                        double p = dataValue / (j->first - i->first);
                        rgb = (1 - p) * i->second + p * j->second;
                    }
                }
                else if (mDisplayColors)
                    rgb = RGBColor::FromHSV(dataValue - 1.0 / 3.0, 1.0, dataValue);
                else
                    rgb = RGBColor::FromHSV(0.0, 0.0, dataValue);
                p.painter->fillRect(dotRect, QColor(rgb.R(), rgb.G(), rgb.B()));
            }
        }
    }
}

void SignalDisplay::DrawMarkerChannels(const PaintInfo &p)
{
    for (int markerCh = 0; markerCh < mMarkerChannels; ++markerCh)
    {
        p.painter->setPen(p.labelColor);
        p.painter->setFont(p.labelFont);
        p.painter->setBackgroundMode(Qt::TransparentMode);
        int baseline = MarkerChannelBottom(markerCh);
        size_t channelNumber = mData.Channels() - mMarkerChannels + markerCh;
        if (mData.Elements() > 0)
        {
            uint32_t prevVal = ToInt(mData(channelNumber, 0));
            for (int sample = 0; sample < mData.Elements(); ++sample)
            {
                uint32_t curVal = ToInt(mData(channelNumber, sample));
                if (curVal != prevVal)
                {
                    QRect posRect;
                    posRect.setLeft(SampleLeft(sample) - p.markerWidth);
                    posRect.setTop(baseline);
                    posRect.setRight(SampleLeft(sample));
                    posRect.setBottom(baseline);
                    std::ostringstream oss;
                    oss << curVal;
                    p.painter->drawText(posRect,
                                        Qt::AlignHCenter | Qt::TextSingleLine | Qt::AlignBottom | Qt::TextDontClip,
                                        oss.str().c_str());
                    QRect markerRect;
                    markerRect.setLeft(SampleLeft(sample) - p.markerWidth);
                    markerRect.setTop(GroupTop(0));
                    markerRect.setRight(SampleLeft(sample));
                    markerRect.setBottom(p.axisY);
                    p.painter->fillRect(markerRect, p.axisBrush);
                    prevVal = curVal;
                }
            }
        }
        p.painter->setBackground(p.backgroundBrush);
        p.painter->setBackgroundMode(Qt::OpaqueMode);
        QRect labelRect(cTickWidth, baseline, 0, 0);
        const char *labelText = "";
        if (mChannelNameCache.size() > channelNumber)
            labelText = mChannelNameCache[channelNumber].c_str();
        p.painter->drawText(labelRect, Qt::AlignLeft | Qt::TextSingleLine | Qt::AlignBottom | Qt::TextDontClip,
                            labelText);
    }
}

QRect SignalDisplay::CursorRect(int sample) const
{
    return QRect(SampleLeft(sample), 0, mCursorWidth,
        GroupBottom(std::max(0, mNumDisplayGroups - 1)));
}

void SignalDisplay::DrawCursor(const PaintInfo &p)
{
    if (mShowCursor)
    {
        QRect cursorRect = CursorRect(mSampleCursor);
        p.painter->fillRect(cursorRect, p.cursorBrush);
        // Remember the cursor rectangle for redrawing when the next
        // data packet arrives.
        mPrevCursorRect = cursorRect;
    }
}

void SignalDisplay::DrawXTicks(const PaintInfo &p)
{
    p.painter->setPen(p.labelColor);
    p.painter->setFont(p.labelFont);
    p.painter->setBackgroundMode(Qt::TransparentMode);
    float xStart = 0, xDivision = 1;
    if (mNumSamples > 0)
    { // Are samples spaced too dense to allow for individual labels?
        float pixelsPerSample = (SampleLeft(mNumSamples - 1) - SampleLeft(0)) / mNumSamples;
        if (pixelsPerSample < 10)
        { // Samples are dense
            float displayLength = ::fabs(mNumSamples * mSampleUnit.Gain()),
                  scale = ::pow(10.0, ::floor(::log10(displayLength) + 0.5));
            xDivision = scale / mSampleUnit.Gain() / 5,
            xStart = xDivision - ::fmod(float(-mSampleUnit.Offset()), xDivision);
        }
    }
    if (xDivision < 1)
        xDivision = 1;

    int nextLabelPos = mDataRect.left();
    for (float j = xStart; j < float(mNumSamples); j += xDivision)
    {
        int tickX = 0;
        switch (mDisplayMode)
        {
        case field2d:
            tickX = (SampleRight(ToInt(j)) + SampleLeft(ToInt(j))) / 2;
            break;
        case polyline:
        default:
            tickX = SampleLeft(ToInt(j));
        }
        QRect tickRect(tickX - cTickWidth / 2, p.axisY + cAxisWidth, cTickWidth, mTickLength);
        p.painter->fillRect(tickRect, p.axisBrush);
        if (tickX > nextLabelPos)
        {
            tickRect.setTop(tickRect.top() + (mTickLength + 3) / 2);
            std::ostringstream label;
            if (mTimeLabels)
            {
                float val = (j - mSampleUnit.Offset()) * mSampleUnit.Gain();
                label << ' ' << std::setfill('0') << std::setw(2) << int(val) / 60 << ':' << std::setw(2)
                      << std::setprecision(2) << int(val) % 60;
                if (::fmod(xDivision * mSampleUnit.Gain(), 1.0) != 0.0)
                    label << '.' << std::setw(2) << int(100 * val + 0.5) % 100;
                label << ' ';
            }
            else
            {
                label << ' ' << std::setprecision(6) << mSampleUnit.RawToPhysical(j, mNumSamples).Utf8() << ' ';
            }

            p.painter->drawText(tickRect, Qt::AlignHCenter | Qt::TextSingleLine | Qt::AlignTop | Qt::TextDontClip,
                                label.str().c_str());
            nextLabelPos = tickX + p.painter->fontMetrics().boundingRect(label.str().c_str()).width();
        }
    }
}

void SignalDisplay::DrawYLabels(const PaintInfo &p, bool inDrawTicks)
{
    p.painter->setPen(p.labelColor);
    p.painter->setBackgroundMode(Qt::TransparentMode);
    int nextLabelPos = mDataRect.top();
    for (int i = 0; i < mNumDisplayGroups; ++i)
    {
        int group = i + mTopGroup;
        size_t channelNumber = GroupToChannel(group);
        int tickY = (GroupTop(i) + GroupBottom(i)) / 2;
        if (inDrawTicks && mYTicksVisible)
        {
            QRect tickRect(mDataLeft - cAxisWidth - mTickLength, tickY - cTickWidth / 2, mTickLength, cTickWidth);
            if (mDisplayColors && mChannelGroupSize == 1)
            {
                tickRect.setTop(tickRect.top() - 1);
                tickRect.setBottom(tickRect.bottom() + 1);
                p.painter->fillRect(tickRect, p.signalBrushes[channelNumber]);
            }
            else
            {
                p.painter->fillRect(tickRect, p.axisBrush);
            }
        }
        if (tickY >= nextLabelPos)
        {
            const char *labelText = "";
            if (mChannelGroupSize == 1 && mChannelNameCache.size() > channelNumber)
                labelText = mChannelNameCache[channelNumber].c_str();
            if (mChannelGroupSize == 1)
            {
                QRect r(0, tickY - cTickWidth / 2, 0, 0);
                if (mShowNumericValues)
                    r.setRight(NumericValuesLeft() - 2 * cAxisWidth);
                else
                    r.setRight(NumericValuesLeft());
                p.painter->setFont(p.labelFont);
                if (mYTicksVisible)
                {
                    p.painter->setPen(p.labelColor);
                    p.painter->drawText(r, Qt::AlignVCenter | Qt::TextSingleLine | Qt::AlignRight | Qt::TextDontClip,
                      labelText);
                }
                if (mShowNumericValues)
                {
                    p.painter->setPen(p.signalPens[channelNumber]);
                    r.setRight(NumericValuesRight());
                    DrawNumericValue(p, r, GetNumericValue(channelNumber).c_str());
                }
            }
            nextLabelPos = tickY + p.painter->fontMetrics().height();
        }
    }
}

void SignalDisplay::DrawAxes(const PaintInfo &p)
{
    QRect xAxis(0, p.axisY, mDisplayRect.right(), cAxisWidth);
    p.painter->fillRect(xAxis, p.axisBrush);
    QRect yAxis;
    yAxis.setLeft(mDataLeft - cAxisWidth);
    yAxis.setTop(mMarkerChannels > 0 ? GroupTop(0) : 0);
    yAxis.setWidth(cAxisWidth);
    yAxis.setBottom(mDisplayRect.bottom());
    p.painter->fillRect(yAxis, p.axisBrush);
    if (mChannelGroupSize == 1 && mShowNumericValues)
    {
        QRect divider(NumericValuesLeft() - cAxisWidth, yAxis.top(), cAxisWidth, yAxis.height());
        p.painter->fillRect(divider, p.axisBrush);
    }
    if (mMarkerChannels > 0)
    {
        QRect divider;
        divider.setLeft(0);
        divider.setRight(mDisplayRect.right());
        divider.setTop(GroupTop(0) - cAxisWidth);
        divider.setBottom(GroupTop(0));
        p.painter->fillRect(divider, p.axisBrush);
    }
}

void SignalDisplay::DrawMarkers(const PaintInfo &p)
{
    // Draw Y axis markers.
    const double baseInterval = mNumDisplayGroups > 0 ? mDataHeight / mNumDisplayGroups : mDataHeight;
    for (size_t i = 0; i < mYAxisMarkers.size(); ++i)
    {
        auto def = ParseMarkerDef(mYAxisMarkers[i].Text(), mInverted);

        QColor markerColor = def.color;
        if (!markerColor.isValid())
            markerColor = p.markerColor;

        const double value = (mYAxisMarkers[i].Address() - mMinValue) / (mMaxValue - mMinValue);
        const double markerY = GroupBottom(DisplayGroups() - 1) - baseInterval * value;

        QRect markerBar;
        markerBar.setLeft(mDataLeft - mTickLength - cAxisWidth);
        markerBar.setTop(markerY - cAxisWidth);
        markerBar.setWidth(mTickLength + cAxisWidth);
        markerBar.setHeight(2 * cAxisWidth);
        p.painter->fillRect(markerBar, markerColor);

        if (!def.label.empty())
        {
            QString qlabel = QString::fromUtf8(def.label.c_str());
            QRect labelRect;
            labelRect.setLeft(markerBar.left());
            labelRect.setRight(labelRect.left());
            labelRect.setTop(markerY - 1);
            labelRect.setBottom(labelRect.top());
            p.painter->save();
            p.painter->setBackground(p.backgroundBrush);
            p.painter->setBackgroundMode(Qt::OpaqueMode);
            p.painter->setPen(markerColor);
            p.painter->drawText(labelRect, Qt::AlignVCenter | Qt::TextSingleLine | Qt::AlignRight | Qt::TextDontClip, qlabel);
            p.painter->restore();
        }

        if (def.style != Qt::NoPen)
        {
            QPen pen;
            pen.setBrush(markerColor);
            pen.setWidthF(markerBar.height() * def.width);
            pen.setStyle(def.style);
            p.painter->save();
            p.painter->setPen(pen);
            p.painter->setBackground(p.backgroundBrush);
            p.painter->setBackgroundMode(Qt::OpaqueMode);
            p.painter->drawLine(mDataLeft, markerY, SampleRight(mNumSamples - 1), markerY);
            p.painter->restore();
        }
    }

    // Draw X axis markers.
    for (size_t i = 0; i < mXAxisMarkers.size(); ++i)
    {
        auto def = ParseMarkerDef(mXAxisMarkers[i].Text(), mInverted);

        QColor markerColor = def.color;
        if (!markerColor.isValid())
            markerColor = p.markerColor;

        int markerX = SampleRight(ToInt(mXAxisMarkers[i].Address() + mSampleUnit.Offset()));
        QRect markerBar(markerX - cAxisWidth, p.axisY, 2 * cAxisWidth, mTickLength + cAxisWidth);
        p.painter->fillRect(markerBar, markerColor);

        if (!def.label.empty())
        {
            QString qlabel = QString::fromUtf8(def.label.c_str());
            QRect labelRect(markerX, p.axisY + cAxisWidth, 0, cAxisWidth);
            labelRect.setTop(labelRect.top() + (mTickLength + 3) / 2);
            p.painter->save();
            p.painter->setBackground(p.backgroundBrush);
            p.painter->setBackgroundMode(Qt::OpaqueMode);
            p.painter->setPen(markerColor);
            p.painter->drawText(labelRect, Qt::AlignHCenter | Qt::TextSingleLine | Qt::AlignTop | Qt::TextDontClip, qlabel);
            p.painter->restore();
        }

        if (def.style != Qt::NoPen)
        {
            QPen pen;
            pen.setBrush(markerColor);
            pen.setWidthF(markerBar.width() * def.width);
            pen.setStyle(def.style);
            p.painter->save();
            p.painter->setPen(pen);
            p.painter->setBackground(p.backgroundBrush);
            p.painter->setBackgroundMode(Qt::OpaqueMode);
            p.painter->drawLine(markerX, p.axisY, markerX, 0);
            p.painter->restore();
        }
    }
}

void SignalDisplay::DrawChannelLabels(const PaintInfo &p)
{
    if (mChannelGroupSize > 1)
    { // Draw channel labels when channels don't coincide with groups.
        p.painter->setBackground(p.backgroundColor);
        p.painter->setBackgroundMode(Qt::OpaqueMode);
        int deltaY = QFontMetrics(p.labelFont).height();
        QRect legendRect(0, cAxisWidth, NumericValuesRight(), deltaY);
        for (size_t i = 0; i < mChannelLabels.size(); ++i)
        {
            if (!ChannelVisible(i))
                continue;

            p.painter->fillRect(legendRect, p.backgroundColor);
            RGBColor textColor = ChannelColor(mChannelLabels[i].Address());
            if (mInverted && textColor == RGBColor::White)
                textColor = RGBColor::Black;
            p.painter->setPen(QColor(textColor.R(), textColor.G(), textColor.B()));
            if (mShowChannelLabels)
            {
                p.painter->setFont(p.labelFont);
                p.painter->drawText(legendRect.adjusted(cAxisWidth, 0, 0, 0),
                                    Qt::AlignVCenter | Qt::TextSingleLine | Qt::AlignLeft | Qt::TextDontClip,
                                    mChannelLabels[i].Text().c_str());
            }
            if (mShowNumericValues)
                DrawNumericValue(p, legendRect, GetNumericValue(i).c_str());
            legendRect.translate(0, deltaY);
        }
    }
}

void SignalDisplay::DrawValueUnits(const PaintInfo &p)
{
    for (int visibleGroup = 0; visibleGroup < mNumDisplayGroups; ++visibleGroup)
    {
        int group = mTopGroup + visibleGroup;
        bool visibleUnit = false;
        int channel = -1;
        for (auto i : mChannelGroups[group])
        {
            if ((i < mValueUnitsVisible.size() && mValueUnitsVisible[i]) || mShowValueUnit)
            {
                visibleUnit = true;
                channel = i;
                break;
            }
        }
        if (visibleUnit)
            DrawValueUnit(p, channel);
    }
}

void SignalDisplay::DrawValueUnit(const PaintInfo &p, int ch)
{
    float baseInterval = mDataHeight;
    if (mNumDisplayGroups > 0)
        baseInterval /= mNumDisplayGroups;

    if (baseInterval > 0)
    {
        const PhysicalUnit& unit = mValueUnits.size() > ch ? mValueUnits[ch] : mValueUnit;
        double maxValue = mMaxValues.size() > ch ? mMaxValues[ch] : mMaxValue;
        double minValue = mMinValues.size() > ch ? mMinValues[ch] : mMinValue;
        p.painter->setFont(p.labelFont);
        p.painter->setPen(p.backgroundColor);
        p.painter->setBackground(p.markerColor);
        p.painter->setBackgroundMode(Qt::OpaqueMode);
        // Find a rounded value that is close to display range.
        double unitsPerPixel = ::fabs((maxValue - minValue) * unit.Gain() / baseInterval),
               scale = ::pow(10.0, ::ceil(::log10(unitsPerPixel * 0.95 * baseInterval)));
        int percent = 100;
        while (percent * scale / unitsPerPixel >= 95 * baseInterval &&
               percent * scale / unitsPerPixel > 100 * mMarkerHeight)
            percent -= 10;
        double rulerLength = percent * scale / 100;
        int pixelLength = ToInt(rulerLength / unitsPerPixel);

        std::ostringstream oss;
        oss << unit.RawToPhysical(rulerLength / unit.Gain()).Utf8();
        QString label(oss.str().c_str());
        QRect labelRect;
        if (minValue == 0)
        {
            int left = SampleLeft(0) + mTickLength, top = ChannelBottom(ch) - pixelLength;
            labelRect = QRect(left, top, 0, 0);
            p.painter->drawText(labelRect, Qt::AlignTop | Qt::TextSingleLine | Qt::AlignLeft, label, &labelRect);
            QRect lineRect;
            lineRect.setLeft(SampleLeft(0));
            lineRect.setTop(labelRect.top());
            lineRect.setRight(left);
            lineRect.setHeight(1);
            p.painter->fillRect(lineRect, p.markerBrush);
        }
        else
        {
            int left = SampleLeft(0),
                center = (ChannelBottom(ch) + ChannelTop(ch)) / 2;
            labelRect = QRect(left, center, 0, 0);
            p.painter->drawText(labelRect, Qt::AlignVCenter | Qt::TextSingleLine | Qt::AlignLeft, label, &labelRect);
            QRect lineRect;
            lineRect.setLeft(labelRect.left());
            lineRect.setTop(center - (pixelLength + p.markerWidth) / 2 - 1);
            lineRect.setRight(labelRect.right());
            lineRect.setBottom(center - (pixelLength - p.markerWidth) / 2 - 1);
            p.painter->fillRect(lineRect, p.markerBrush);
            lineRect.setTop(lineRect.top() + pixelLength);
            lineRect.setBottom(lineRect.bottom() + pixelLength);
            p.painter->fillRect(lineRect, p.markerBrush);
        }
        labelRect.adjust(-1, 0, 1, 0);
        p.painter->fillRect(labelRect, p.markerBrush);
        labelRect.adjust(1, 0, -1, 0);
        p.painter->drawText(labelRect, Qt::AlignVCenter | Qt::TextSingleLine | Qt::AlignLeft, label);
    }
}

void SignalDisplay::DrawNumericValue(const PaintInfo &p, const QRect &inRect, const char *inValue)
{
    p.painter->setFont(p.monoFont);
    int w = p.painter->fontMetrics().boundingRect(inValue).width();
    if (mNumericValueWidth < w)
    {
        mNumericValueWidth = w;
        SyncLabelWidth();
        Invalidate();
    }
    p.painter->drawText(inRect, Qt::AlignVCenter | Qt::TextSingleLine | Qt::AlignRight | Qt::TextDontClip, inValue);
}

void SignalDisplay::CleanupPainting(PaintInfo &p)
{
    delete p.painter;
}
