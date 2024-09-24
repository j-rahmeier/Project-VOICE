////////////////////////////////////////////////////////////////////////////////
// $Id: SignalDisplay.h 8108 2024-05-04 08:49:57Z mellinger $
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
#ifndef SIGNAL_DISPLAY_H
#define SIGNAL_DISPLAY_H

#include "Color.h"
#include "GUI.h"
#include "GenericSignal.h"
#include "Label.h"
#include <set>
#include <vector>
#include <mutex>

#include <QColor>
#include <QFont>
#include <QPaintDevice>
#include <QPen>
#include <QPoint>
#include <QRect>
#include <QRegion>
#include <QPixmap>

class SignalDisplay
{
  private:
    enum
    {
        cNumSamplesDefault = 128,
        cMinValueDefault = -(1 << 15),
        cMaxValueDefault = (1 << 16) - 1,

        cChannelBase = 1, // displayed number of first channel
        cSampleBase = 0,  // displayed number of first sample

        cLabelWidth = 25,
        cAxisWidth = 2,
        cTickWidth = cAxisWidth,
        cTickLength = 4,
        cInitialMaxDisplayGroups = 32,
    };

    static const RGBColor cAxisColorDefault, cChannelColorsDefault[];

  public:
    enum eDisplayMode
    {
        polyline = 0,
        field2d,
        /* ... */
        numDisplayModes
    };

  public:
    SignalDisplay();
    virtual ~SignalDisplay();

  private:
    SignalDisplay(const SignalDisplay &) = delete;
    SignalDisplay &operator=(const SignalDisplay &) = delete;

  public:
    SignalDisplay &SetContext(const GUI::DrawContext &);
    SignalDisplay &Invalidate(const void *Region = nullptr);
    SignalDisplay &Paint(const void *Region = nullptr);

    SignalDisplay &WrapForward(const GenericSignal &);
    SignalDisplay &ScrollForward(const GenericSignal &);
    SignalDisplay &ScrollBack(const GenericSignal &);

    SignalDisplay &SetNumSamples(int);
    int NumSamples() const
    {
        return mNumSamples;
    }
    SignalDisplay &SetDisplayGroups(int);
    int DisplayGroups() const
    {
        return mMaxDisplayGroups;
    }
    SignalDisplay &SetChannelGroupSize(int i)
    {
        mChannelGroupSize = i;
        return Invalidate();
    }
    int ChannelGroupSize() const
    {
        return mChannelGroupSize;
    }
    SignalDisplay &SetNumMarkerChannels(int n)
    {
        mMarkerChannels = n;
        return Invalidate();
    }
    int NumMarkerChannels() const
    {
        return mMarkerChannels;
    }

    SignalDisplay &SetMinValue(double f)
    {
        mMinValue = f;
        mMinValues.clear();
        return Invalidate();
    }
    double MinValue() const
    {
        return mMinValue;
    }

    SignalDisplay &SetMinValues(const std::vector<double>&);
    const std::vector<double>& MinValues() const;

    SignalDisplay &SetMaxValue(double f)
    {
        mMaxValue = f;
        mMaxValues.clear();
        return Invalidate();
    }
    double MaxValue() const
    {
        return mMaxValue;
    }

    SignalDisplay& SetMaxValues(const std::vector<double>&);
    const std::vector<double>& MaxValues() const;

    SignalDisplay &SetSampleOffset(double f)
    {
        mSampleUnit.SetOffset(-f);
        InvalidateWidget();
        return *this;
    }
    double SampleOffset() const
    {
        return -mSampleUnit.Offset();
    }
    SignalDisplay &SetSampleUnit(const std::string &);
    const std::string &SampleUnit() const
    {
        return mSampleUnit.Symbol();
    }
    SignalDisplay &SetValueUnit(const std::string &);
    std::string ValueUnit() const
    {
        return mValueUnit.GainWithSymbol();
    }

    SignalDisplay &SetValueUnits(const ValueList<std::string> &);
    ValueList<std::string> ValueUnits() const;

    SignalDisplay &SetChannelVisible(int, bool);
    bool ChannelVisible(int) const;
    SignalDisplay &SetChannelsVisible(const ValueList<bool> &);
    ValueList<bool> ChannelsVisible() const;

    SignalDisplay &SetChannelUnit(const std::string &);
    const std::string &ChannelUnit() const
    {
        return mChannelUnit.Symbol();
    }

    SignalDisplay &SetUnitsPerSample(float f)
    {
        mSampleUnit.SetGain(f);
        return Invalidate();
    }
    float UnitsPerSample() const
    {
        return mSampleUnit.Gain();
    }
    SignalDisplay &SetUnitsPerValue(float f)
    {
        mValueUnit.SetGain(f);
        return Invalidate();
    }
    float UnitsPerValue() const
    {
        return mValueUnit.Gain();
    }
    SignalDisplay &SetUnitsPerChannel(float f)
    {
        mChannelUnit.SetGain(f);
        return Invalidate();
    }
    float UnitsPerChannel() const
    {
        return mChannelUnit.Gain();
    }

    SignalDisplay &SetCursorColor(const RGBColor& c)
    {
        mCursorColor = c;
        InvalidateWidget();
        return *this;
    }

    const RGBColor& CursorColor() const
    {
        return mCursorColor;
    }

    SignalDisplay& SetCursorWidth(int width)
    {
        mCursorWidth = width;
        InvalidateWidget();
        return *this;
    }

    int CursorWidth() const
    {
        return mCursorWidth;
    }

    SignalDisplay &SetAxisColor(const RGBColor &c)
    {
        mAxisColor = c;
        return Invalidate();
    }
    const RGBColor &AxisColor() const
    {
        return mAxisColor;
    }
    SignalDisplay &SetChannelColors(const ColorList &l)
    {
        mChannelColors = l;
        return Invalidate();
    }
    const ColorList &ChannelColors() const
    {
        return mChannelColors;
    }
    SignalDisplay &SetChannelStyles(const std::string &s)
    {
        mChannelStyles = s;
        return Invalidate();
    }
    const std::string &ChannelStyles() const
    {
        return mChannelStyles;
    }
    SignalDisplay &SetChannelLabels(const LabelList &l)
    {
        mChannelLabels = l;
        SyncLabelWidth();
        return Invalidate();
    }
    const LabelList &ChannelLabels() const
    {
        return mChannelLabels;
    }
    SignalDisplay &SetChannelLabelWidth(const std::string &s)
    {
        mChannelLabelWidth = s;
        SyncLabelWidth();
        return Invalidate();
    }
    const std::string &ChannelLabelWidth() const
    {
        return mChannelLabelWidth;
    }
    SignalDisplay &SetXAxisMarkers(const LabelList &l)
    {
        mXAxisMarkers = l;
        InvalidateWidget();
        return *this;
    }
    const LabelList& XAxisMarkers() const
    {
        return mXAxisMarkers;
    }
    SignalDisplay& SetYAxisMarkers(const LabelList& l)
    {
        mYAxisMarkers = l;
        InvalidateWidget();
        return *this;
    }
    const LabelList& YAxisMarkers() const
    {
        return mYAxisMarkers;
    }


    SignalDisplay &SetValueColors(std::string);
    std::string ValueColors() const;

    SignalDisplay &SetTopGroup(int);
    int TopGroup() const
    {
        return mTopGroup;
    }

    SignalDisplay &SetDisplayMode(int);
    int DisplayMode() const
    {
        return mDisplayMode;
    }

    SignalDisplay &SetNumericValuesVisible(bool b)
    {
        mShowNumericValues = b;
        SyncLabelWidth();
        return Invalidate();
    }
    SignalDisplay &SetBaselinesVisible(bool b)
    {
        mShowBaselines = b;
        return Invalidate();
    }
    bool NumericValuesVisible() const
    {
        return mShowNumericValues;
    }
    bool BaselinesVisible() const
    {
        return mShowBaselines;
    }

    SignalDisplay& SetXTicksVisible(bool b)
    {
        mXTicksVisible = b;
        return Invalidate();
    }
    bool XTicksVisible() const
    {
        return mXTicksVisible;
    }
    SignalDisplay& SetYTicksVisible(bool b)
    {
      mYTicksVisible = b;
      SyncLabelWidth();
      return Invalidate();
    }
    bool YTicksVisible() const
    {
      return mYTicksVisible;
    }

    SignalDisplay &SetChannelLabelsVisible(bool b)
    {
        mShowChannelLabels = b;
        SyncLabelWidth();
        return Invalidate();
    }
    bool ChannelLabelsVisible() const
    {
        return mShowChannelLabels;
    }
    SignalDisplay &SetValueUnitVisible(bool b)
    {
        mShowValueUnit = b;
        mNumericValueWidth = 0;
        return Invalidate();
    }
    bool ValueUnitVisible() const
    {
        return mShowValueUnit;
    }

    SignalDisplay& SetValueUnitsVisible(const std::vector<bool>& v)
    {
        mValueUnitsVisible = v;
        mNumericValueWidth = 0;
        return Invalidate();
    }
    const std::vector<bool>& ValueUnitsVisible() const
    {
        return mValueUnitsVisible;
    }

    SignalDisplay &SetColorDisplay(bool b)
    {
        mDisplayColors = b;
        return Invalidate();
    }
    bool ColorDisplay() const
    {
        return mDisplayColors;
    }

    SignalDisplay &SetInverted(bool b)
    {
        mInverted = b;
        return Invalidate();
    }
    bool Inverted() const
    {
        return mInverted;
    }

    SignalDisplay &SetFontSize(int i)
    {
        mFontSize = std::max(0, i);
        SyncLabelWidth();
        return Invalidate();
    }
    int FontSize() const
    {
        return mFontSize;
    }

    SignalDisplay& SetFastScrolling(bool b)
    {
        mFastScrolling = b;
        return Invalidate();
    }
    bool FastScrolling() const
    {
        return mFastScrolling;
    }
    // Set to 0 to disable auto decimation.
    SignalDisplay& SetAutoDecimation(int maxLinesPerPixel)
    {
        mAutoDecimationMaxLinesPerPixel = maxLinesPerPixel;
        return Invalidate();
    }
    int AutoDecimation() const
    {
        return mAutoDecimationMaxLinesPerPixel;
    }

    // Functions that centralize sample/channel -> pixel conversion in painting
    // and invalidating contexts.
  private:
    float mDataWidth, mDataHeight, mDataLeft;
    int mLabelWidth, mNumericValueWidth, mMarkerHeight, mTickLength;

    void SyncLabelWidth();
    void RebuildChannelGroups();
    void SyncGraphics();
    void AdaptTo(const GenericSignal &);
    void AdjustDisplayGroups();

  public:
    float SampleLeft(int s) const
    {
        return mDataLeft + (mNumSamples ? (s * mDataWidth) / float(mNumSamples) : 0);
    }
    float SampleRight(int s) const
    {
        return SampleLeft(s + 1);
    }
    float PosToSample(float p) const
    {
        return mDataWidth ? ((p - mDataLeft) * mNumSamples) / float(mDataWidth) : 0;
    }

  private:
    float MarkerChannelTop(int ch) const
    {
        return mMarkerChannels != 0 ? mMarkerHeight * ch + cAxisWidth : 0;
    }
    float MarkerChannelBottom(int ch) const
    {
        return mMarkerChannels != 0 ? MarkerChannelTop(ch) + mMarkerHeight : 0;
    }

    float GroupTop(int g) const
    {
        float top = MarkerChannelBottom(mMarkerChannels - 1) + cAxisWidth;
        if (mNumDisplayGroups < 2)
            top += g * mDataHeight;
        else
            top += (g * mDataHeight) / float(mNumDisplayGroups);
        return top;
    }

    float GroupBottom(int g) const
    {
        return GroupTop(g + 1);
    }

  public:
    float ChannelTop(int ch) const;
    float ChannelBottom(int ch) const;
    int PosToChannel(float p) const;

    int NumChannels() const
    {
        return mData.Channels();
    }
    double Data(int ch, int el) const
    {
        return mData(ch, el);
    }

  private:
    int PosToGroup(float p) const;
    int ChannelToGroup(int ch) const;
    int GroupToChannel(int g) const;

    double NormData(size_t i, size_t j) const;

    RGBColor ChannelColor(int ch) const
    {
        return mChannelColors[ch % mChannelColors.size()];
    }

    int NumericValuesLeft() const
    {
        return NumericValuesRight() - mNumericValueWidth;
    }
    int NumericValuesRight() const
    {
        return mDataLeft - 2 * cAxisWidth - cTickLength;
    }
    std::string GetNumericValue(int ch) const;

  private:
    eDisplayMode mDisplayMode;
    bool mShowCursor, mWrapAround, mTimeLabels, mShowNumericValues, mShowBaselines, mShowChannelLabels, mShowValueUnit,
        mDisplayColors, mInverted, mXTicksVisible, mYTicksVisible, mFastScrolling;
    int mNumSamples, mSampleCursor, mCursorWidth, mNumDisplayGroups, mMaxDisplayGroups, mTopGroup,
        mChannelGroupSize, mMarkerChannels, mFontSize, mAutoDecimationMaxLinesPerPixel;
    double mMinValue, mMaxValue;
    std::vector<double> mMinValues, mMaxValues;
    PhysicalUnit mChannelUnit, mSampleUnit, mValueUnit;
    std::vector<PhysicalUnit> mValueUnits;
    std::vector<bool> mChannelVisible, mValueUnitsVisible;
    RGBColor mCursorColor, mAxisColor;
    ColorList mChannelColors;
    std::string mChannelStyles;
    std::vector<std::string> mChannelNameCache;
    LabelList mChannelLabels, mXAxisMarkers, mYAxisMarkers;
    std::string mChannelLabelWidth;
    std::vector<std::pair<double, RGBColor>> mValueColors;
    GenericSignal mData;
    std::vector<std::vector<int>> mChannelGroups;

    std::recursive_mutex mDataMutex, mBufferMutex;

  private:
    void PaintToBuffer(const void *Region = nullptr);
    void InvalidateBuffer(const void *Region = nullptr);
    void InvalidateWidget(const void *Region = nullptr);
    QRect CursorRect(int sample) const;

    struct PaintInfo;
    void SetupPainting(PaintInfo &, QPaintDevice *, const void *);
    void ClearBackground(const PaintInfo &);
    void DrawSignalPolyline(const PaintInfo &);
    void DrawSignalField2d(const PaintInfo &);
    void DrawMarkerChannels(const PaintInfo &);
    void DrawCursor(const PaintInfo &);
    void DrawXTicks(const PaintInfo &);
    void DrawYLabels(const PaintInfo &, bool inDrawTicks);
    void DrawAxes(const PaintInfo &);
    void DrawMarkers(const PaintInfo &);
    void DrawChannelLabels(const PaintInfo &);
    void DrawValueUnits(const PaintInfo &);
    void DrawValueUnit(const PaintInfo &, int channel);
    void DrawNumericValue(const PaintInfo &, const QRect &, const char *);
    void CleanupPainting(PaintInfo &);

    // Qt graphics.
    QFont AxisFont();
    QFont MonoFont();
    QFont SymbolFont();
    QPaintDevice *mParent;
    QRect mDisplayRect, mDataRect, mPrevCursorRect;
    QPixmap mImageBuffer;
    QRegion mDisplayRgn, mInvalidBufferRgn;
    QPointF *mpSignalPoints;
    int mSymbolWidth;

    struct PaintInfo
    {
        QPainter *painter;
        const QRegion *updateRgn;
        QColor backgroundColor, cursorColor, axisColor, markerColor, labelColor;
        QBrush backgroundBrush, cursorBrush, axisBrush, markerBrush;
        QFont labelFont, monoFont, symbolFont;
        QPen baselinePen;
        int markerWidth, axisY;
        std::vector<QPen> signalPens;
        std::vector<QBrush> signalBrushes;
        QString signalStyles;
    };
};

#endif // SIGNAL_DISPLAY_H
