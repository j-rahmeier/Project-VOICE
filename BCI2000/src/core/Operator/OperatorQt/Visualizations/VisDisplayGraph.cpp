////////////////////////////////////////////////////////////////////////////////
// $Id: VisDisplayGraph.cpp 8181 2024-06-05 13:57:42Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A class for graph type visualization windows.
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
#include "VisDisplayGraph.h"

#include "ColorListChooser.h"
#include "MappedProperty.h"

#include <limits>
#include <cmath>

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QColorDialog>
#include <QLabel>
#include <QLayout>
#include <QMenu>
#include <QPaintEvent>
#include <QPainter>
#include <QStatusBar>
#include <QStyle>
#include <QScreen>
#include <QListWidget>

#define OFF "off"

static const std::string cAutoScaleTime = "50ms";
static const double cAutoScaleFactor = 1.3; // additional margin factor
static const int cAutoDecimationMaxLinesPerPixel = 5;

static const RGBColor sInitialCustomColors[] =
{
    RGBColor::White, RGBColor::LtGray,
    RGBColor::DkGray, RGBColor::Black,

    RGBColor::Red, RGBColor::Yellow,
    RGBColor::Orange, RGBColor::Purple,
    RGBColor::Fuchsia, RGBColor::Lime,
    RGBColor::Green, RGBColor::Maroon,
    RGBColor::Blue, RGBColor::Aqua,
    RGBColor::Olive, RGBColor::Navy,

    ColorList::End
};

VisDisplayGraph::VisDisplayGraph(const std::string &inSourceID)
    : VisDisplayLayer(inSourceID), mpGraphContextMenu(NULL), mpHPMenu(NULL), mpLPMenu(NULL), mpNotchMenu(NULL),
      mpActEnlargeSignal(NULL), mpActReduceSignal(NULL), mpActToggleAutoScale(NULL), mpActFewerSamples(NULL),
      mpActMoreSamples(NULL), mpActResetSampleZoom(nullptr),
      mpActMoreChannels(NULL), mpActFewerChannels(NULL), mpActToggleDisplayMode(NULL),
      mpActToggleNumericValues(NULL), mpActToggleBaselines(NULL),
      mpActToggleChannelLabels(NULL), mpActToggleColor(NULL), mpActInvertDisplay(NULL),
      mpActSmallerFont(nullptr), mpActLargerFont(nullptr), mNumChannels(0), mSignalElements(0),
      mUserZoom(0), mpCurrentHPItem(NULL), mpCurrentLPItem(NULL), mpCurrentNotchItem(NULL), mpStatusBar(NULL),
      mpStatusLabel(NULL), mAutoScale(false), mAutoScaleTime(cAutoScaleTime), mFixedScale(false), mElementGain(0),
      mpSignalWidget(nullptr), mpChannelList(nullptr),
      mUserScaling(0), mConfigDisplaySamples(0), mMinValue(0), mMaxValue(0)
{
    QVBoxLayout* pVLayout = new QVBoxLayout;
    pVLayout->setContentsMargins(0, 0, 0, 0);
    pVLayout->setSpacing(0);
    QHBoxLayout* pHLayout = new QHBoxLayout;
    pHLayout->setContentsMargins(0, 0, 0, 0);
    pHLayout->setSpacing(0);
    pVLayout->addLayout(pHLayout);
    mpSignalWidget = new SignalWidget(this);
    mpSignalWidget->Display().SetAutoDecimation(cAutoDecimationMaxLinesPerPixel);
    pHLayout->addWidget(mpSignalWidget);
    BuildStatusBar();
    pVLayout->addWidget(mpStatusBar);
    UpdateStatusBar();
    BuildGraphContextMenu();
    BuildChannelList();
    BuildChannelListContextMenu();
    pHLayout->addWidget(mpChannelList);
    setLayout(pVLayout);
}

VisDisplayGraph::~VisDisplayGraph()
{
    delete mpGraphContextMenu;
    delete mpChannelListContextMenu;
}

void VisDisplayGraph::OnSetConfig(ConfigSettings &inConfig)
{
    VisDisplayBase::OnSetConfig(inConfig);

    SignalDisplay& display = mpSignalWidget->Display();

    double minValue, maxValue;
    if (inConfig.Get(CfgID::MinValue, minValue))
        mMinValue = minValue;
    if (inConfig.Get(CfgID::MaxValue, maxValue))
        mMaxValue = maxValue;

    ValueList<double> minValues, maxValues;
    if (inConfig.Get(CfgID::MinValues, minValues))
        mMinValues = minValues;
    if (inConfig.Get(CfgID::MaxValues, maxValues))
        mMaxValues = maxValues;

    double userScaling = 0;
    if (inConfig.Get(CfgID::UserScaling, userScaling))
        mUserScaling = userScaling;

    MappedProperty<double> channelUserScaling;
    if (inConfig.Get(CfgID::ChannelUserScaling, channelUserScaling))
        MappedPropertyToContainer(channelUserScaling, mChannelUserScaling);

    ApplyUserScaling();

    inConfig.Get(CfgID::FixedScale, mFixedScale);

    int numSamples = NominalDisplaySamples();
    if (inConfig.Get(CfgID::NumSamples, numSamples))
    {
        mConfigDisplaySamples = numSamples;
        SetNominalDisplaySamples(numSamples);
    }

    double userZoom = 0;
    if (inConfig.Get(CfgID::UserSampleZoom, userZoom))
    {
        mUserZoom = userZoom;
        ApplyUserZoom();
    }

    int channelGroupSize = display.ChannelGroupSize();
    if (inConfig.Get(CfgID::ChannelGroupSize, channelGroupSize))
    {
        if (channelGroupSize < 1)
            channelGroupSize = std::numeric_limits<int>::max();
        display.SetChannelGroupSize(channelGroupSize);
    }
    int graphType = display.DisplayMode();
    if (inConfig.Get(CfgID::GraphType, graphType))
        switch (graphType)
        {
        case CfgID::Polyline:
            display.SetDisplayMode(SignalDisplay::polyline);
            break;
        case CfgID::Field2d:
            display.SetDisplayMode(SignalDisplay::field2d);
            break;
        }
    bool showNumericValues = display.NumericValuesVisible();
    if (inConfig.Get(CfgID::ShowNumericValues, showNumericValues))
        display.SetNumericValuesVisible(showNumericValues);

    bool showBaselines = display.BaselinesVisible();
    if (inConfig.Get(CfgID::ShowBaselines, showBaselines))
        display.SetBaselinesVisible(showBaselines);

    bool invertedDisplay = display.Inverted();
    if (inConfig.Get(CfgID::InvertedDisplay, invertedDisplay))
        display.SetInverted(invertedDisplay);

    int fontSize = display.FontSize();
    if (inConfig.Get(CfgID::FontSize, fontSize))
        display.SetFontSize(fontSize);

    mCustomColors = ColorList(sInitialCustomColors);
    inConfig.Get(CfgID::CustomColors, mCustomColors);

    // Channel colors must be stored as a ColorList (rather than a MappedProperty<RGBColor>)
    // for backward compatibility.
    ColorList channelColors = display.ChannelColors();
    if (inConfig.Get(CfgID::ChannelColors, channelColors))
        display.SetChannelColors(channelColors);

    std::string channelStyles = display.ChannelStyles();
    if (inConfig.Get(CfgID::ChannelStyles, channelStyles))
        display.SetChannelStyles(channelStyles);

    std::string valueColors;
    if (inConfig.Get(CfgID::ValueColors, valueColors))
        display.SetValueColors(valueColors);

    std::string unit;
    std::istringstream iss;
    if (inConfig.Get(CfgID::SampleUnit, unit))
    {
        double oldSampleUnit = NominalUnitsPerSample() * FilterUnitToValue(display.SampleUnit());
        PhysicalUnit u;
        u.SetGainWithSymbol(unit);
        float unitsPerSample = u.Gain();
        std::string sampleUnit = u.Symbol();
        SetNominalUnitsPerSample(unitsPerSample);
        display.SetSampleUnit(u.Symbol());
        if (oldSampleUnit != unitsPerSample * FilterUnitToValue(sampleUnit))
        {
            mDisplayFilter.HPCorner(0);
            mDisplayFilter.LPCorner(0);
            mDisplayFilter.NotchCenter(0);
        }
        mElementGain = unitsPerSample * FilterUnitToValue(sampleUnit);
    }
    float sampleOffset;
    if (inConfig.Get(CfgID::SampleOffset, sampleOffset))
        display.SetSampleOffset(-sampleOffset);

    if (mFixedScale)
    {
        mAutoScale = false;
        mAutoScaleTime = OFF;
    }
    else
    {
        std::string autoScale;
        mAutoScale = inConfig.Get(CfgID::AutoScale, autoScale);
        if (autoScale == OFF)
        {
            mAutoScale = false;
        }
        else
        {
            if (autoScale.empty() || autoScale.back() != 's')
                autoScale = cAutoScaleTime;
            mAutoScale = true;
        }
        mAutoScaleTime = autoScale;
    }
    if (inConfig.Get(CfgID::ChannelUnit, unit))
        display.SetChannelUnit(unit);

    if (inConfig.Get(CfgID::ValueUnit, unit))
        display.SetValueUnit(unit);

    ValueList<std::string> units = display.ValueUnits();
    if (inConfig.Get(CfgID::ValueUnits, units))
        display.SetValueUnits(units);

    bool showValueUnit = display.ValueUnitVisible();
    if (inConfig.Get(CfgID::ShowValueUnit, showValueUnit))
        display.SetValueUnitVisible(showValueUnit);

    bool xAxisLabels = display.XTicksVisible();
    if (inConfig.Get(CfgID::ShowXTicks, xAxisLabels))
        display.SetXTicksVisible(xAxisLabels);

    bool yAxisLabels = display.YTicksVisible();
    if (inConfig.Get(CfgID::ShowYTicks, yAxisLabels))
      display.SetYTicksVisible(yAxisLabels);

    LabelList labels = display.XAxisMarkers();
    if (inConfig.Get(CfgID::XAxisMarkers, labels))
        display.SetXAxisMarkers(labels);

    labels = display.YAxisMarkers();
    if (inConfig.Get(CfgID::YAxisMarkers, labels))
        display.SetYAxisMarkers(labels);

    inConfig.Get(CfgID::ChannelsVisible, mChannelsVisible);

    labels = display.ChannelLabels();
    if (inConfig.Get(CfgID::ChannelLabels, labels))
    {
        display.SetChannelLabels(labels);
        display.SetChannelLabelsVisible(!labels.empty());
        UpdateChannelList(labels);
    }

    MappedProperty<bool> valueUnitsVisibleByName;
    if (inConfig.Get(CfgID::ValueUnitsVisible, valueUnitsVisibleByName))
    {
        std::vector<bool> valueUnitsVisible = display.ValueUnitsVisible();
        MappedPropertyToContainer(valueUnitsVisibleByName, valueUnitsVisible);
        display.SetValueUnitsVisible(valueUnitsVisible);
    }

    std::string hpFilter = OFF;
    if (inConfig.Get(CfgID::HPFilter, hpFilter))
        SetHP(hpFilter);

    std::string lpFilter = OFF;
    if (inConfig.Get(CfgID::LPFilter, lpFilter))
        SetLP(lpFilter);

    std::string notchFilter = OFF;
    if (inConfig.Get(CfgID::NotchFilter, notchFilter))
        SetNotch(notchFilter);

    bool showStatusBar = true;
    inConfig.Get(CfgID::ShowStatusBar, showStatusBar);
    ShowStatusBar(showStatusBar && Filter_Enabled());

    bool showChannelList = (graphType != CfgID::Polyline);
    inConfig.Get(CfgID::ShowChannelList, showChannelList);
    if (mpChannelList)
        mpChannelList->setVisible(showChannelList);

    // Sanity checks.
    if (display.MinValue() == display.MaxValue())
        display.SetMaxValue(display.MinValue() + 1);
}

void VisDisplayGraph::ShowStatusBar(bool inVisible)
{
    mpStatusBar->setVisible(inVisible);
}

void VisDisplayGraph::OnSignal(const GenericSignal &s)
{
    if (s.Channels() < 1 || s.Elements() < 1)
        return;

    SignalDisplay& display = mpSignalWidget->Display();

    int curChannels = mNumChannels, curSamples = display.NumSamples();

    // Apply the visualization filter.
    GenericSignal filteredSignal(s.Properties());
    mDisplayFilter.Process(s, filteredSignal);
    if (mElementGain > 0)
    {
        SignalProperties s = filteredSignal.Properties();
        s.ElementUnit().SetSymbol("s").SetGain(mElementGain);
        filteredSignal.SetProperties(s);
    }

    mNumChannels = filteredSignal.Channels();

    // Apply the decimation filter.
    GenericSignal decimatedSignal(0, 0);
    mDecimationFilter.Process(filteredSignal, decimatedSignal);

    // Add the signal to the display.
    display.WrapForward(decimatedSignal);

    ApplyAutoScale(decimatedSignal.Properties().ElementUnit());

    mSignalElements = s.Elements();

    if (curChannels != mNumChannels || curSamples != display.NumSamples())
        OnSetConfig(Visconfigs()[mVisID]);
}

void VisDisplayGraph::BuildStatusBar()
{
    mpStatusBar = new QStatusBar(this);
    // Without modifying the palette, the text will be white on gray on macOS
    QPalette palette = mpStatusBar->style()->standardPalette();
    palette.setColor(QPalette::WindowText, Qt::black);
    mpStatusBar->setPalette(palette);
    mpStatusBar->setAutoFillBackground(true);
    mpStatusLabel = new QLabel(mpStatusBar);
    mpStatusBar->addPermanentWidget(mpStatusLabel);
}

void VisDisplayGraph::BuildGraphContextMenu()
{
    mpGraphContextMenu = new QMenu;
    mpActEnlargeSignal = mpGraphContextMenu->addAction(tr("Enlarge Signal"), this, SLOT(EnlargeSignal()));
    mpActReduceSignal = mpGraphContextMenu->addAction(tr("Reduce Signal"), this, SLOT(ReduceSignal()));
    mpActResetSignalScale = mpGraphContextMenu->addAction("Reset Global Signal Scale", this, &VisDisplayGraph::ResetSignalScale);
    mpActToggleAutoScale = mpGraphContextMenu->addAction(tr("Auto Scale"), this, SLOT(ToggleAutoScale()));
    mpActToggleAutoScale->setCheckable(true);
    mpGraphContextMenu->addSeparator();

    mpActFewerSamples = mpGraphContextMenu->addAction(tr("Fewer Samples"), this, SLOT(FewerSamples()));
    mpActMoreSamples = mpGraphContextMenu->addAction(tr("More Samples"), this, SLOT(MoreSamples()));
    mpActResetSampleZoom = mpGraphContextMenu->addAction(tr("Reset Sample Zoom"), this, &VisDisplayGraph::ResetSampleZoom);
    mpGraphContextMenu->addSeparator();

    mpActMoreChannels = mpGraphContextMenu->addAction(tr("More Channels"), this, SLOT(MoreChannels()));
    mpActFewerChannels = mpGraphContextMenu->addAction(tr("Fewer Channels"), this, SLOT(FewerChannels()));
    mpGraphContextMenu->addSeparator();

    mpActToggleDisplayMode = mpGraphContextMenu->addAction(tr("Toggle Display Mode"), this, SLOT(ToggleDisplayMode()));
    mpActToggleColor = mpGraphContextMenu->addAction(tr("Color Display"), this, SLOT(ToggleColor()));
    mpActToggleColor->setCheckable(true);
    mpActInvertDisplay = mpGraphContextMenu->addAction(tr("Invert"), this, SLOT(InvertDisplay()));
    mpActInvertDisplay->setCheckable(true);
    mpGraphContextMenu->addSeparator();

    mpActToggleNumericValues = mpGraphContextMenu->addAction(tr("Show Numeric Values"), this, SLOT(ToggleNumericValues()));
    mpActToggleNumericValues->setCheckable(true);
    mpActToggleBaselines = mpGraphContextMenu->addAction(tr("Show Baselines"), this, SLOT(ToggleBaselines()));
    mpActToggleBaselines->setCheckable(true);
    mpActToggleChannelLabels = mpGraphContextMenu->addAction(tr("Show Legend"), this, SLOT(ToggleChannelLabels()));
    mpActToggleChannelLabels->setCheckable(true);
    mpActToggleValueUnit = mpGraphContextMenu->addAction("Show Value Unit", this, &VisDisplayGraph::ToggleValueUnit);
    mpActToggleValueUnit->setCheckable(true);
    mpActToggleChannelList = mpGraphContextMenu->addAction(tr("Show Channel List"), this, SLOT(ToggleChannelList()));
    mpActToggleChannelList->setCheckable(true);
    mpActLargerFont = mpGraphContextMenu->addAction(tr("Larger Font"), this, SLOT(LargerFont()));
    mpActSmallerFont = mpGraphContextMenu->addAction(tr("Smaller Font"), this, SLOT(SmallerFont()));
    mpGraphContextMenu->addSeparator();

    mpHPMenu = new QMenu(tr("High Pass"), mpGraphContextMenu);
    QActionGroup *pHPGroup = new QActionGroup(mpHPMenu);
    QAction *pAct = mpHPMenu->addAction(OFF, this, SLOT(SetHPOff()));
    pAct->setCheckable(true);
    pHPGroup->addAction(pAct);
    pAct->setChecked(true);
    mpCurrentHPItem = pAct;
    pAct = mpHPMenu->addAction("0.1Hz", this, SLOT(SetHP01()));
    pAct->setCheckable(true);
    pHPGroup->addAction(pAct);
    pAct = mpHPMenu->addAction("1Hz", this, SLOT(SetHP1()));
    pAct->setCheckable(true);
    pHPGroup->addAction(pAct);
    pAct = mpHPMenu->addAction("5Hz", this, SLOT(SetHP5()));
    pAct->setCheckable(true);
    pHPGroup->addAction(pAct);
    mpGraphContextMenu->addMenu(mpHPMenu);

    mpLPMenu = new QMenu(tr("Low Pass"), mpGraphContextMenu);
    QActionGroup *pLPGroup = new QActionGroup(mpLPMenu);
    pAct = mpLPMenu->addAction(OFF, this, SLOT(SetLPOff()));
    pAct->setCheckable(true);
    pAct->setChecked(true);
    mpCurrentLPItem = pAct;
    pLPGroup->addAction(pAct);
    pAct = mpLPMenu->addAction("30Hz", this, SLOT(SetLP30()));
    pAct->setCheckable(true);
    pLPGroup->addAction(pAct);
    pAct = mpLPMenu->addAction("40Hz", this, SLOT(SetLP40()));
    pAct->setCheckable(true);
    pLPGroup->addAction(pAct);
    pAct = mpLPMenu->addAction("70Hz", this, SLOT(SetLP70()));
    pAct->setCheckable(true);
    pLPGroup->addAction(pAct);
    mpGraphContextMenu->addMenu(mpLPMenu);

    mpNotchMenu = new QMenu(tr("Notch"), mpGraphContextMenu);
    QActionGroup *pNotchGroup = new QActionGroup(mpNotchMenu);
    pAct = mpNotchMenu->addAction(OFF, this, SLOT(SetNotchOff()));
    pAct->setCheckable(true);
    pAct->setChecked(true);
    mpCurrentNotchItem = pAct;
    pNotchGroup->addAction(pAct);
    pAct = mpNotchMenu->addAction("50Hz", this, SLOT(SetNotch50()));
    pAct->setCheckable(true);
    pNotchGroup->addAction(pAct);
    pAct = mpNotchMenu->addAction("60Hz", this, SLOT(SetNotch60()));
    pAct->setCheckable(true);
    pNotchGroup->addAction(pAct);
    mpGraphContextMenu->addMenu(mpNotchMenu);
}

void VisDisplayGraph::BuildChannelListContextMenu()
{
    QMenu* pMenu = new QMenu;
    
    pMenu->addAction("Enable all", this, &VisDisplayGraph::ChannelList_EnableAll);
    pMenu->addAction("Disable all", this, &VisDisplayGraph::ChannelList_DisableAll);
    pMenu->addSeparator();

    mpActChannelList_Enable = pMenu->addAction("Enable", this, &VisDisplayGraph::ChannelList_Enable);
    mpActChannelList_Disable = pMenu->addAction("Disable", this, &VisDisplayGraph::ChannelList_Disable);
    pMenu->addSeparator();

    mpActChannelList_Enlarge = pMenu->addAction("Enlarge Signal", this, &VisDisplayGraph::ChannelList_EnlargeSignal);
    mpActChannelList_Reduce = pMenu->addAction("Reduce Signal", this, &VisDisplayGraph::ChannelList_ReduceSignal);
    mpActChannelList_Reset = pMenu->addAction("Reset Per-Channel Signal Scale", this, &VisDisplayGraph::ChannelList_ResetSignalScale);
    pMenu->addSeparator();

    mpActChannelList_ShowUnit = pMenu->addAction("Show Unit", this, &VisDisplayGraph::ChannelList_ShowUnit);
    mpActChannelList_HideUnit = pMenu->addAction("Hide Unit", this, &VisDisplayGraph::ChannelList_HideUnit);
    pMenu->addSeparator();

    mpActChannelList_ChooseColor = pMenu->addAction("Choose Color...", this, &VisDisplayGraph::ChannelList_ChooseColor);

    mpChannelListContextMenu = pMenu;
}

void VisDisplayGraph::OnContextMenu(const QPoint &inP)
{
    if (mpChannelList && mpChannelList->isVisible() && mpChannelList->geometry().contains(inP))
        OnChannelListContextMenu(inP);
    else
        OnGraphContextMenu(inP);
}

void VisDisplayGraph::OnGraphContextMenu(const QPoint &inP)
{
    mpActEnlargeSignal->setEnabled(EnlargeSignal_Enabled());
    mpActReduceSignal->setEnabled(ReduceSignal_Enabled());
    mpActResetSignalScale->setEnabled(ResetSignalScale_Enabled());
    mpActToggleAutoScale->setEnabled(ToggleAutoScale_Enabled());
    mpActToggleAutoScale->setChecked(ToggleAutoScale_Checked());

    mpActFewerSamples->setEnabled(FewerSamples_Enabled());
    mpActMoreSamples->setEnabled(MoreSamples_Enabled());
    mpActResetSampleZoom->setEnabled(ResetSampleZoom_Enabled());

    mpActFewerChannels->setEnabled(FewerChannels_Enabled());
    mpActMoreChannels->setEnabled(MoreChannels_Enabled());

    mpActToggleColor->setEnabled(ToggleColor_Enabled());
    mpActToggleColor->setChecked(ToggleColor_Checked());

    mpActInvertDisplay->setChecked(InvertDisplay_Checked());

    mpActToggleNumericValues->setEnabled(ToggleNumericValues_Enabled());
    mpActToggleNumericValues->setChecked(ToggleNumericValues_Checked());

    mpActToggleBaselines->setEnabled(ToggleBaselines_Enabled());
    mpActToggleBaselines->setChecked(ToggleBaselines_Checked());

    mpActToggleChannelLabels->setEnabled(ToggleChannelLabels_Enabled());
    mpActToggleChannelLabels->setChecked(ToggleChannelLabels_Checked());

    mpActToggleValueUnit->setEnabled(ToggleValueUnit_Enabled());
    mpActToggleValueUnit->setChecked(ToggleValueUnit_Checked());

    mpActToggleChannelList->setEnabled(ToggleChannelList_Enabled());
    mpActToggleChannelList->setChecked(ToggleChannelList_Checked());

    mpActSmallerFont->setEnabled(SmallerFont_Enabled());

    mpHPMenu->setEnabled(Filter_Enabled());
    mpLPMenu->setEnabled(Filter_Enabled());
    mpNotchMenu->setEnabled(Filter_Enabled());

    mpGraphContextMenu->exec(this->mapToGlobal(inP));
}

void VisDisplayGraph::OnChannelListContextMenu(const QPoint& inP)
{
    bool enabled = ChannelList_Enabled();
    for (auto pAction : {
        mpActChannelList_Enable,
        mpActChannelList_Disable,
        mpActChannelList_Reset,
        })
    {
        pAction->setEnabled(enabled);
    }
    mpActChannelList_ShowUnit->setEnabled(enabled && ChannelList_ShowUnit_Enabled());
    mpActChannelList_HideUnit->setEnabled(enabled && ChannelList_HideUnit_Enabled());
    mpActChannelList_Enlarge->setEnabled(enabled && ChannelList_EnlargeSignal_Enabled());
    mpActChannelList_Reduce->setEnabled(enabled && ChannelList_ReduceSignal_Enabled());
    mpActChannelList_ChooseColor->setEnabled(enabled && ChannelList_ChooseColor_Enabled());
    mpChannelListContextMenu->exec(this->mapToGlobal(inP));
}

void VisDisplayGraph::ToggleAutoScale()
{
    mAutoScale = !mAutoScale;
    if (mAutoScale)
    {
        mAutoScaleTime = cAutoScaleTime;
    }
    else
    {
        mAutoScaleTime = OFF;
        ApplyUserScaling();
    }
    Visconfigs()[mVisID].Put(CfgID::AutoScale, mAutoScaleTime, UserDefined);
}

bool VisDisplayGraph::ToggleAutoScale_Enabled() const
{
    return !mFixedScale;
}

bool VisDisplayGraph::ToggleAutoScale_Checked() const
{
    return mAutoScale;
}

void VisDisplayGraph::ToggleDisplayMode()
{
    mpSignalWidget->Display().SetDisplayMode((mpSignalWidget->Display().DisplayMode() + 1) % SignalDisplay::numDisplayModes);
}

void VisDisplayGraph::ToggleNumericValues()
{
    mpSignalWidget->Display().SetNumericValuesVisible(!mpSignalWidget->Display().NumericValuesVisible());
    Visconfigs()[mVisID].Put(CfgID::ShowNumericValues, mpSignalWidget->Display().NumericValuesVisible(), UserDefined);
}

void VisDisplayGraph::ToggleValueUnit()
{
    mpSignalWidget->Display().SetValueUnitVisible(!mpSignalWidget->Display().ValueUnitVisible());
    Visconfigs()[mVisID].Put(CfgID::ShowValueUnit, mpSignalWidget->Display().ValueUnitVisible(), UserDefined);
}

bool VisDisplayGraph::ToggleNumericValues_Enabled() const
{
    return true;
}

bool VisDisplayGraph::ToggleNumericValues_Checked() const
{
    return mpSignalWidget->Display().NumericValuesVisible();
}

void VisDisplayGraph::ToggleBaselines()
{
    mpSignalWidget->Display().SetBaselinesVisible(!mpSignalWidget->Display().BaselinesVisible());
    Visconfigs()[mVisID].Put(CfgID::ShowBaselines, mpSignalWidget->Display().BaselinesVisible(), UserDefined);
}

bool VisDisplayGraph::ToggleBaselines_Enabled() const
{
    return mpSignalWidget->Display().DisplayMode() == SignalDisplay::polyline;
}

bool VisDisplayGraph::ToggleBaselines_Checked() const
{
    return mpSignalWidget->Display().BaselinesVisible();
}

void VisDisplayGraph::ToggleChannelLabels()
{
    mpSignalWidget->Display().SetChannelLabelsVisible(!mpSignalWidget->Display().ChannelLabelsVisible());
}

bool VisDisplayGraph::ToggleValueUnit_Checked() const
{
    return mpSignalWidget->Display().ValueUnitVisible();
}

bool VisDisplayGraph::ToggleValueUnit_Enabled() const
{
    return true;
}

bool VisDisplayGraph::ToggleChannelLabels_Enabled() const
{
    return !mpSignalWidget->Display().ChannelLabels().empty();
}

bool VisDisplayGraph::ToggleChannelLabels_Checked() const
{
    return mpSignalWidget->Display().ChannelLabelsVisible();
}

bool VisDisplayGraph::ToggleChannelList_Enabled() const
{
    return mpChannelList;
}

bool VisDisplayGraph::ToggleChannelList_Checked() const
{
    return mpChannelList->isVisible();
}

void VisDisplayGraph::ToggleColor()
{
    mpSignalWidget->Display().SetColorDisplay(!mpSignalWidget->Display().ColorDisplay());
}

bool VisDisplayGraph::ToggleColor_Enabled() const
{
    return mpSignalWidget->Display().DisplayMode() == SignalDisplay::polyline || mpSignalWidget->Display().DisplayMode() == SignalDisplay::field2d;
}

bool VisDisplayGraph::ToggleColor_Checked() const
{
    return mpSignalWidget->Display().ColorDisplay();
}

void VisDisplayGraph::InvertDisplay()
{
    mpSignalWidget->Display().SetInverted(!mpSignalWidget->Display().Inverted());
    Visconfigs()[mVisID].Put(CfgID::InvertedDisplay, mpSignalWidget->Display().Inverted(), UserDefined);
}

bool VisDisplayGraph::InvertDisplay_Checked() const
{
    return mpSignalWidget->Display().Inverted();
}

void VisDisplayGraph::EnlargeSignal()
{
    ++mUserScaling;
    ApplyUserScaling();
}

bool VisDisplayGraph::EnlargeSignal_Enabled() const
{
    if (mFixedScale)
        return false;
    if (mAutoScale)
        return false;

    return mUserScaling < cMaxUserScaling;
}

void VisDisplayGraph::ReduceSignal()
{
    --mUserScaling;
    ApplyUserScaling();
}

bool VisDisplayGraph::ReduceSignal_Enabled() const
{
    if (mFixedScale)
        return false;
    if (mAutoScale)
        return false;

    return mUserScaling > -cMaxUserScaling;
}

void VisDisplayGraph::ResetSignalScale()
{
    mUserScaling = 0;
    ApplyUserScaling();
}

bool VisDisplayGraph::ResetSignalScale_Enabled() const
{
    if (mAutoScale)
        return false;
    return mUserScaling != 0;
}

void VisDisplayGraph::FewerSamples()
{
    ++mUserZoom;
    ApplyUserZoom();
}

bool VisDisplayGraph::FewerSamples_Enabled() const
{
    float nominalSamples = mConfigDisplaySamples * ::pow(2, -mUserZoom - 1);
    return mUserZoom < cMaxUserScaling && nominalSamples >= 2 * mSignalElements;
}

void VisDisplayGraph::MoreSamples()
{
    --mUserZoom;
    ApplyUserZoom();
}

bool VisDisplayGraph::MoreSamples_Enabled() const
{
    return mUserZoom > -cMaxUserScaling;
}

void VisDisplayGraph::ResetSampleZoom()
{
    mUserZoom = 0;
    SetNominalDisplaySamples(mConfigDisplaySamples);
}

bool VisDisplayGraph::ResetSampleZoom_Enabled() const
{
    return mUserZoom != 0;
}

void VisDisplayGraph::FewerChannels()
{
    // Round down to the nearest power of 2.
    int n = mpSignalWidget->Display().DisplayGroups(), roundedN = 1;
    while (n >>= 1)
        roundedN <<= 1;

    int newGroups = roundedN;
    if (newGroups == mpSignalWidget->Display().DisplayGroups())
        newGroups /= 2;
    mpSignalWidget->Display().SetDisplayGroups(newGroups);
}

bool VisDisplayGraph::FewerChannels_Enabled() const
{
    return mpSignalWidget->Display().DisplayGroups() > 1;
}

void VisDisplayGraph::MoreChannels()
{
    int maxDisplayGroups = mNumChannels / mpSignalWidget->Display().ChannelGroupSize(),
        newDisplayGroups = std::min(maxDisplayGroups, mpSignalWidget->Display().DisplayGroups() * 2);
    mpSignalWidget->Display().SetDisplayGroups(newDisplayGroups);
}

bool VisDisplayGraph::MoreChannels_Enabled() const
{
    return mpSignalWidget->Display().DisplayGroups() < mNumChannels / mpSignalWidget->Display().ChannelGroupSize();
}

double VisDisplayGraph::FilterCaptionToValue(const char *inCaption) const
{
    const char *p = inCaption;
    while (*p && !::isdigit(*p))
        ++p;
    return ::atof(p) * NominalUnitsPerSample() * FilterUnitToValue(mpSignalWidget->Display().SampleUnit());
}

double VisDisplayGraph::FilterUnitToValue(const std::string &inUnit) const
{
    PhysicalUnit u;
    u.SetGainWithSymbol("1" + inUnit);
    return u.Symbol() == "s" ? u.Gain() : 0;
}

void VisDisplayGraph::LargerFont()
{
    int size = Round(mpSignalWidget->Display().FontSize() * 1.1);
    if (size == mpSignalWidget->Display().FontSize())
        ++size;
    mpSignalWidget->Display().SetFontSize(size);
    Visconfigs()[mVisID].Put(CfgID::FontSize, mpSignalWidget->Display().FontSize(), UserDefined);
}

void VisDisplayGraph::SmallerFont()
{
    int size = Round(mpSignalWidget->Display().FontSize() / 1.1);
    if (size == mpSignalWidget->Display().FontSize())
        --size;
    mpSignalWidget->Display().SetFontSize(size);
    Visconfigs()[mVisID].Put(CfgID::FontSize, mpSignalWidget->Display().FontSize(), UserDefined);
}

void VisDisplayGraph::ToggleChannelList()
{
    if (mpChannelList)
        mpChannelList->setVisible(!mpChannelList->isVisible());
    Visconfigs()[mVisID].Put(CfgID::ShowChannelList, mpChannelList && mpChannelList->isVisible(), UserDefined);
}

bool VisDisplayGraph::SmallerFont_Enabled() const
{
    return mpSignalWidget->Display().FontSize() > 1;
}

int VisDisplayGraph::NominalDisplaySamples() const
{
    return mpSignalWidget->Display().NumSamples() * mDecimationFilter.Decimation();
}

void VisDisplayGraph::SetNominalDisplaySamples(int inSamples)
{
    if (inSamples != NominalDisplaySamples())
    {
        double nominalUnitsPerSample = NominalUnitsPerSample();
        int actualSamples = inSamples, decimation = 1;
        int screenWidth = QGuiApplication::primaryScreen()->geometry().width();
        if (screenWidth < inSamples)
        {
            decimation = ::ceil(static_cast<float>(inSamples) / screenWidth / cDisplayOversampling);
            actualSamples = inSamples / decimation;
        }
        mpSignalWidget->Display().SetNumSamples(actualSamples);
        mDecimationFilter.Decimation(decimation);
        SetNominalUnitsPerSample(nominalUnitsPerSample);
    }
}

double VisDisplayGraph::NominalUnitsPerSample() const
{
    return mpSignalWidget->Display().UnitsPerSample() / mDecimationFilter.Decimation();
}

void VisDisplayGraph::SetNominalUnitsPerSample(double inUps)
{
    mpSignalWidget->Display().SetUnitsPerSample(inUps * mDecimationFilter.Decimation());
}

bool VisDisplayGraph::Filter_Enabled() const
{
    return !mFixedScale && NominalUnitsPerSample() * FilterUnitToValue(mpSignalWidget->Display().SampleUnit()) != 0;
}

void VisDisplayGraph::SetHP(const std::string &inCaption, bool inWriteToConfig)
{
    if (inCaption == OFF)
    {
        mDisplayFilter.HPCorner(0);
    }
    else
    {
        mDisplayFilter.HPCorner(FilterCaptionToValue(inCaption.c_str()));
        mLastHP = inCaption;
    }
    mpCurrentHPItem = SyncFilterMenu(inCaption, mpHPMenu);
    if (inWriteToConfig)
        Visconfigs()[mVisID].Put(CfgID::HPFilter, inCaption, UserDefined);
    UpdateStatusBar();
}

void VisDisplayGraph::SetHPOff()
{
    SetHP(OFF, true);
}

void VisDisplayGraph::SetHP01()
{
    SetHP("0.1Hz", true);
}

void VisDisplayGraph::SetHP1()
{
    SetHP("1Hz", true);
}

void VisDisplayGraph::SetHP5()
{
    SetHP("5Hz", true);
}

void VisDisplayGraph::SetLP(const std::string &inCaption, bool inWriteToConfig)
{
    if (inCaption == OFF)
    {
        mDisplayFilter.LPCorner(0);
    }
    else
    {
        mDisplayFilter.LPCorner(FilterCaptionToValue(inCaption.c_str()));
        mLastLP = inCaption;
    }
    mpCurrentLPItem = SyncFilterMenu(inCaption, mpLPMenu);
    if (inWriteToConfig)
        Visconfigs()[mVisID].Put(CfgID::LPFilter, inCaption, UserDefined);
    UpdateStatusBar();
}

void VisDisplayGraph::SetLPOff()
{
    SetLP(OFF, true);
}

void VisDisplayGraph::SetLP30()
{
    SetLP("30Hz", true);
}

void VisDisplayGraph::SetLP40()
{
    SetLP("40Hz", true);
}

void VisDisplayGraph::SetLP70()
{
    SetLP("70Hz", true);
}

void VisDisplayGraph::SetNotch(const std::string &inCaption, bool inWriteToConfig)
{
    if (inCaption == OFF)
    {
        mDisplayFilter.NotchCenter(0);
    }
    else
    {
        mDisplayFilter.NotchCenter(FilterCaptionToValue(inCaption.c_str()));
        mLastNotch = inCaption;
    }
    mpCurrentNotchItem = SyncFilterMenu(inCaption, mpNotchMenu);
    if (inWriteToConfig)
        Visconfigs()[mVisID].Put(CfgID::NotchFilter, inCaption, UserDefined);
    UpdateStatusBar();
}

void VisDisplayGraph::SetNotchOff()
{
    SetNotch(OFF, true);
}

void VisDisplayGraph::SetNotch50()
{
    SetNotch("50Hz", true);
}

void VisDisplayGraph::SetNotch60()
{
    SetNotch("60Hz", true);
}

void VisDisplayGraph::UpdateStatusBar()
{
    std::ostringstream oss;
    oss << "HP: ";
    if (mDisplayFilter.HPCorner() == 0.0)
        oss << OFF;
    else
        oss << mLastHP;
    oss << "   LP: ";
    if (mDisplayFilter.LPCorner() == 0.0)
        oss << OFF;
    else
        oss << mLastLP;
    oss << "   Notch: ";
    if (mDisplayFilter.NotchCenter() == 0.0)
        oss << OFF;
    else
        oss << mLastNotch;
    mpStatusLabel->setText(oss.str().c_str());
}

QAction *VisDisplayGraph::SyncFilterMenu(const std::string &inCaption, QMenu *inpMenu)
{
    QAction *result = NULL;
    for (int i = 0; i < inpMenu->actions().size(); ++i)
    {
        QAction *pAction = inpMenu->actions().at(i);
        bool isCurrent = (inCaption == pAction->text().toLocal8Bit().constData());
        if (isCurrent)
            result = pAction;
        pAction->setChecked(isCurrent);
    }
    return result;
}

QAction *VisDisplayGraph::NextMenuItem(QMenu *inpMenu, QAction *inpItem)
{ // Beginning from the position of the input item, and wrapping around at the end,
    // get the next action item from the menu.
    if (inpMenu->actions().empty())
        return NULL;

    int idx = 0;
    QAction *pAction = NULL;
    while (idx < inpMenu->actions().size() && pAction != inpItem)
        pAction = inpMenu->actions().at(idx++);
    if (idx == inpMenu->actions().size())
        idx = 0;
    return inpMenu->actions().at(idx);
}

void VisDisplayGraph::BuildChannelList()
{
    Assert(mpChannelList == nullptr);
    mpChannelList = new QListWidget;
    mpChannelList->setFrameStyle(QFrame::NoFrame);
    mpChannelList->setFixedWidth(0);
    mpChannelList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mpChannelList->setFocusPolicy(Qt::NoFocus);
    connect(mpChannelList, &QListWidget::itemChanged, this, &VisDisplayGraph::OnChannelListItemChanged);
}

void VisDisplayGraph::UpdateChannelList(const LabelList& labels)
{
    if (mpChannelList)
    {
        mpChannelList->clear();
        mChannelMap.clear();
        for (int i = 0; i < labels.Size(); ++i)
        {
            mChannelMap[labels[i].Text()] = labels[i].Address();

            QSignalBlocker block(mpChannelList);
            auto pItem = new QListWidgetItem(mpChannelList);
            pItem->setData(Qt::UserRole, QVariant::fromValue(labels[i].Address()));
            pItem->setText(QString::fromUtf8(labels[i].Text().c_str()));
            pItem->setFlags(pItem->flags() | Qt::ItemIsUserCheckable);
            block.unblock();

            if (!mChannelsVisible.Exists(labels[i].Text()))
              mChannelsVisible[labels[i].Text()] = true;

            Qt::CheckState state = mChannelsVisible[labels[i].Text()] ? Qt::Checked : Qt::Unchecked;
            pItem->setCheckState(state);
        }
        int width = mpChannelList->sizeHintForColumn(0);
        width += mpChannelList->fontMetrics().horizontalAdvance("  ");
        width += qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent);
        mpChannelList->setFixedWidth(width);
    }
}

bool VisDisplayGraph::ChannelList_Enabled() const
{
    return mpChannelList && mpChannelList->selectedItems().count() > 0;
}

bool VisDisplayGraph::ChannelList_EnlargeSignal_Enabled() const
{
    if (mFixedScale)
        return false;

    int minScaling = cMaxUserScaling;
    if (mpChannelList)
    {
        auto items = mpChannelList->selectedItems();
        for (auto pItem : items)
        {
            auto idx = pItem->data(Qt::UserRole);
            Assert(idx.isValid());
            int ch = idx.toInt();
            int scaling = GetUserScalingForChannel(ch);
            if (scaling < minScaling)
                minScaling = scaling;
        }
    }
    return minScaling < cMaxUserScaling;
}

bool VisDisplayGraph::ChannelList_ReduceSignal_Enabled() const
{
    if (mFixedScale)
        return false;

    int maxScaling = -cMaxUserScaling;
    if (mpChannelList)
    {
        auto items = mpChannelList->selectedItems();
        for (auto pItem : items)
        {
            auto idx = pItem->data(Qt::UserRole);
            Assert(idx.isValid());
            int ch = idx.toInt();
            int scaling = GetUserScalingForChannel(ch);
            if (scaling > maxScaling)
                maxScaling = scaling;
        }
    }
    return maxScaling > -cMaxUserScaling;
}

void VisDisplayGraph::ChannelList_Enable()
{
    if (mpChannelList)
    {
        auto selected = mpChannelList->selectedItems();
        for (const auto pItem : selected)
            pItem->setCheckState(Qt::Checked);
    }
}

void VisDisplayGraph::ChannelList_Disable()
{
    if (mpChannelList)
    {
        auto selected = mpChannelList->selectedItems();
        for (const auto pItem : selected)
            pItem->setCheckState(Qt::Unchecked);
    }
}

void VisDisplayGraph::ChannelList_EnlargeSignal()
{
    if (mpChannelList)
    {
        auto selected = mpChannelList->selectedItems();
        for (const auto pItem : selected)
        {
            auto idx = pItem->data(Qt::UserRole);
            Assert(idx.isValid());
            int ch = idx.toInt();
            int scaling = GetUserScalingForChannel(ch);
            SetUserScalingForChannel(ch, scaling + 1);
        }
        ApplyUserScaling();
    }
}

void VisDisplayGraph::ChannelList_ReduceSignal()
{
    if (mpChannelList)
    {
        if (mpChannelList)
        {
            auto selected = mpChannelList->selectedItems();
            for (const auto pItem : selected)
            {
                auto idx = pItem->data(Qt::UserRole);
                Assert(idx.isValid());
                int ch = idx.toInt();
                int scaling = GetUserScalingForChannel(ch);
                SetUserScalingForChannel(ch, scaling - 1);
            }
            ApplyUserScaling();
        }
    }
}

void VisDisplayGraph::ChannelList_ResetSignalScale()
{
    if (mpChannelList)
    {
        auto selected = mpChannelList->selectedItems();
        for (const auto pItem : selected)
        {
            auto idx = pItem->data(Qt::UserRole);
            Assert(idx.isValid());
            int ch = idx.toInt();
            SetUserScalingForChannel(ch, 0);
        }
        ApplyUserScaling();
    }
}

void VisDisplayGraph::ChannelList_ShowUnit()
{
    ChannelList_SetUnitVisible(true);
}

void VisDisplayGraph::ChannelList_HideUnit()
{
    ChannelList_SetUnitVisible(false);
}

void VisDisplayGraph::OnChannelListItemChanged(QListWidgetItem* pItem)
{
    auto var = pItem->data(Qt::UserRole);
    if (var.isValid())
    {
        double index = var.toDouble();
        bool checked = (pItem->checkState() == Qt::Checked);
        mpSignalWidget->Display().SetChannelVisible(index, checked);
        mChannelsVisible[pItem->text().toUtf8().constData()] = checked;
        Visconfigs()[mVisID].Put(CfgID::ChannelsVisible, mChannelsVisible, UserDefined);
    }
}

void VisDisplayGraph::ChannelList_DisableAll()
{
    if (mpChannelList)
    {
        for (int i = 0; i < mpChannelList->count(); ++i)
            mpChannelList->item(i)->setCheckState(Qt::Unchecked);
    }
}

void VisDisplayGraph::ChannelList_EnableAll()
{
    if (mpChannelList)
    {
        for (int i = 0; i < mpChannelList->count(); ++i)
            mpChannelList->item(i)->setCheckState(Qt::Checked);
    }
}

void VisDisplayGraph::ChannelList_ChooseColor()
{
    auto items = mpChannelList->selectedItems();
    if (!items.empty())
    {
        ColorList channelColors = mpSignalWidget->Display().ChannelColors();

        RGBColor color = RGBColor::White;
        auto idx = items.front()->data(Qt::UserRole);
        Assert(idx.isValid());
        color = channelColors[static_cast<int>(idx.toDouble()) % channelColors.size()];
        ColorListChooser chooser;
        chooser.SetPrompt("Choose channel color");
        if (chooser.Display(color, mCustomColors))
        {
            for (auto pItem : items)
            {
                auto idx = pItem->data(Qt::UserRole);
                Assert(idx.isValid());
                int i = idx.toDouble();
                if (i >= channelColors.Size())
                    channelColors.Resize(i + 1, RGBColor::White);
                channelColors[i] = color;
            }
        }
        mpSignalWidget->Display().SetChannelColors(channelColors);
        Visconfigs()[mVisID].Put(CfgID::ChannelColors, channelColors, UserDefined);
        Visconfigs()[mVisID].Put(CfgID::CustomColors, mCustomColors, UserDefined);
    }
}

bool VisDisplayGraph::ChannelList_ChooseColor_Enabled() const
{
    return mpSignalWidget->Display().ColorDisplay() && mpSignalWidget->Display().DisplayMode() == SignalDisplay::polyline;
}

bool VisDisplayGraph::ChannelList_ShowUnit_Enabled() const
{
    return !mpSignalWidget->Display().ValueUnitVisible();
}

bool VisDisplayGraph::ChannelList_HideUnit_Enabled() const
{
    return !mpSignalWidget->Display().ValueUnitVisible();
}

void VisDisplayGraph::ChannelList_SetUnitVisible(bool visible)
{
    auto items = mpChannelList->selectedItems();
    if (!items.empty())
    {
        auto unitsVisible = mpSignalWidget->Display().ValueUnitsVisible();
        for (const auto pItem : items)
        {
            auto data = pItem->data(Qt::UserRole);
            if (data.isValid())
            {
                int idx = data.toInt();
                if (idx >= unitsVisible.size())
                    unitsVisible.resize(idx + 1);
                unitsVisible[idx] = visible;
            }
        }
        mpSignalWidget->Display().SetValueUnitsVisible(unitsVisible);
        SaveValueUnitsVisible();
    }
}

void VisDisplayGraph::ApplyUserScaling()
{
    int channels = mChannelUserScaling.size();
    if (mMinValues.size() < channels)
        mMinValues.resize(channels, mMinValue);
    if (mMaxValues.size() < channels)
        mMaxValues.resize(channels, mMaxValue);
    std::vector<double> finalMinValues(channels), finalMaxValues(channels);

    double factor1 = ::pow(2.0, -mUserScaling);
    for (int ch = 0; ch < channels; ++ch)
    {
        double factor2 = ::pow(2.0, -mChannelUserScaling[ch]);
        finalMinValues[ch] = mMinValues[ch] * factor1 * factor2;
        finalMaxValues[ch] = mMaxValues[ch] * factor1 * factor2;
    }
    mpSignalWidget->Display().SetMinValues(finalMinValues).SetMaxValues(finalMaxValues);

    auto& config = Visconfigs()[mVisID];
    config.Put(CfgID::UserScaling, mUserScaling, UserDefined);

    MappedProperty<double> userScalingByName;
    config.Get(CfgID::ChannelUserScaling, userScalingByName);
    ContainerToMappedProperty(mChannelUserScaling, userScalingByName);
    config.Put(CfgID::ChannelUserScaling, userScalingByName, UserDefined);
}

void VisDisplayGraph::SetUserScalingForChannel(int channel, int value)
{
    if (mChannelUserScaling.size() <= channel)
        mChannelUserScaling.resize(channel + 1, 0);
    if (value > cMaxUserScaling)
        value = cMaxUserScaling;
    if (value < -cMaxUserScaling)
        value = -cMaxUserScaling;
    mChannelUserScaling[channel] = value;
}

int VisDisplayGraph::GetUserScalingForChannel(int channel) const
{
    if (mChannelUserScaling.size() <= channel)
        return 0;
    return mChannelUserScaling[channel];
}

void VisDisplayGraph::ApplyAutoScale(const PhysicalUnit& unit)
{
    if (mAutoScale)
    {
        SignalDisplay& display = mpSignalWidget->Display();
        int autoScaleSamples = display.NumSamples();
        if (unit.Symbol() == "s")
            autoScaleSamples = unit.PhysicalToRaw(mAutoScaleTime);
        autoScaleSamples = std::max(1, autoScaleSamples);
        autoScaleSamples = std::min(display.NumSamples(), autoScaleSamples);
        int numChannels = display.NumChannels();
        std::vector<double> displayMin(numChannels), displayMax(numChannels);
        for (int ch = 0; ch < numChannels; ++ch)
        {
            double sigMax = -Infinity<double>(), sigMin = Infinity<double>();
            for (int sample = 0; sample < autoScaleSamples; ++sample)
            {
                auto value = display.Data(ch, sample);
                if (!std::isnan(value))
                {
                    sigMax = std::max(sigMax, value);
                    sigMin = std::min(sigMin, value);
                }
            }
            if (sigMin > 0)
            {
                sigMin = 0;
            }
            else if (sigMax < 0)
            {
                sigMax = 0;
            }
            else
            {
                float absMax = std::max(abs(sigMax), abs(sigMin));
                sigMax = absMax;
                sigMin = -absMax;
            }
            if (sigMax != sigMin)
            {
                displayMax[ch] = sigMax * cAutoScaleFactor;
                displayMin[ch] = sigMin * cAutoScaleFactor;
            }
        }
        display.SetMaxValues(displayMax);
        display.SetMinValues(displayMin);
    }
}

void VisDisplayGraph::SaveValueUnitsVisible() const
{
    auto& config = Visconfigs()[mVisID];
    std::vector<bool> valueUnitsVisible = mpSignalWidget->Display().ValueUnitsVisible();
    MappedProperty<bool> valueUnitsVisibleByName;
    config.Get(CfgID::ValueUnitsVisible, valueUnitsVisibleByName);
    ContainerToMappedProperty(valueUnitsVisible, valueUnitsVisibleByName);
    config.Put(CfgID::ValueUnitsVisible, valueUnitsVisibleByName, UserDefined);
}

void VisDisplayGraph::ApplyUserZoom()
{
    float newNominalSamples = mConfigDisplaySamples * ::pow(2, -mUserZoom);
    if (newNominalSamples >= 2 * mSignalElements)
        SetNominalDisplaySamples(newNominalSamples);
    Visconfigs()[mVisID].Put(CfgID::UserSampleZoom, mUserZoom, UserDefined);
}

void VisDisplayGraph::moveEvent(QMoveEvent *iopEvent)
{
    VisDisplayBase::moveEvent(iopEvent);
    iopEvent->accept();
}

void VisDisplayGraph::resizeEvent(QResizeEvent *iopEvent)
{
    VisDisplayBase::resizeEvent(iopEvent);
    iopEvent->accept();
}

void VisDisplayGraph::keyReleaseEvent(QKeyEvent *iopEvent)
{
    static int acc = 0;
    int wipe_acc = 1;
    int key = iopEvent->key(), modkey = iopEvent->modifiers();

    if (key == Qt::Key_Space && (modkey & Qt::ShiftModifier))
        key = Qt::Key_PageUp;
    if (acc == 0 && key == Qt::Key_G && (modkey & Qt::ShiftModifier))
        key = Qt::Key_End;
    if (acc != 0 && key == Qt::Key_Return)
        key = Qt::Key_G;

    switch (key)
    {
    case Qt::Key_Up:
        mpSignalWidget->Display().SetTopGroup(mpSignalWidget->Display().TopGroup() - 1);
        break;
    case Qt::Key_Down:
        mpSignalWidget->Display().SetTopGroup(mpSignalWidget->Display().TopGroup() + 1);
        break;
    case Qt::Key_PageUp:
    case Qt::Key_B:
        mpSignalWidget->Display().SetTopGroup(mpSignalWidget->Display().TopGroup() - mpSignalWidget->Display().DisplayGroups());
        break;
    case Qt::Key_PageDown:
    case Qt::Key_Space:
        mpSignalWidget->Display().SetTopGroup(mpSignalWidget->Display().TopGroup() + mpSignalWidget->Display().DisplayGroups());
        break;
    case Qt::Key_Right:
        if (FewerSamples_Enabled())
            FewerSamples();
        break;
    case Qt::Key_Left:
        if (MoreSamples_Enabled())
            MoreSamples();
        break;
    case Qt::Key_Less:
    case Qt::Key_Comma:
        if (FewerChannels_Enabled())
            FewerChannels();
        break;
    case Qt::Key_Greater:
    case Qt::Key_Period:
        if (MoreChannels_Enabled())
            MoreChannels();
        break;
    case Qt::Key_Minus:
        if (ReduceSignal_Enabled())
            ReduceSignal();
        break;
    case Qt::Key_Plus:
    case Qt::Key_Equal:
        if (EnlargeSignal_Enabled())
            EnlargeSignal();
        break;
    case Qt::Key_Home:
        mpSignalWidget->Display().SetTopGroup(0);
        break;
    case Qt::Key_End:
        mpSignalWidget->Display().SetTopGroup(mNumChannels / mpSignalWidget->Display().ChannelGroupSize());
        break;
    case Qt::Key_G:
        mpSignalWidget->Display().SetTopGroup(acc - 1);
        break;
    case Qt::Key_H:
        if (modkey & Qt::ShiftModifier)
        {
            QAction *pItem = NextMenuItem(mpHPMenu, mpCurrentHPItem);
            if (pItem)
                pItem->trigger();
        }
        else
        {
            if (mDisplayFilter.HPCorner() == 0.0)
                SetHP(mLastHP, true);
            else
                SetHP(OFF, true);
        }
        break;
    case Qt::Key_L:
        if (modkey & Qt::ShiftModifier)
        {
            QAction *pItem = NextMenuItem(mpLPMenu, mpCurrentLPItem);
            if (pItem)
                pItem->trigger();
        }
        else
        {
            if (mDisplayFilter.LPCorner() == 0.0)
                SetLP(mLastLP, true);
            else
                SetLP(OFF, true);
        }
        break;
    case Qt::Key_N:
        if (modkey & Qt::ShiftModifier)
        {
            QAction *pItem = NextMenuItem(mpNotchMenu, mpCurrentNotchItem);
            if (pItem)
                pItem->trigger();
        }
        else
        {
            if (mDisplayFilter.NotchCenter() == 0.0)
                SetNotch(mLastNotch, true);
            else
                SetNotch(OFF, true);
        }
        break;
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
        acc = acc * 10 + (key - Qt::Key_0);
        wipe_acc = 0;
        break;
    default:
        VisDisplayBase::keyReleaseEvent(iopEvent);
    }
    if (wipe_acc)
        acc = 0;
}

void VisDisplayGraph::mousePressEvent(QMouseEvent *iopEvent)
{
    this->activateWindow();
    VisDisplayBase::mousePressEvent(iopEvent);
}

template<class T>
void VisDisplayGraph::MappedPropertyToContainer(const MappedProperty<T>& map, std::vector<T>& container) const
{
    container.resize(mChannelMap.size());
    for (const auto& entry : map)
    {   // mChannelMap provides Channel Name->Channel Index
        auto i = mChannelMap.find(entry.first);
        if (i != mChannelMap.end())
        {
            if (i->second >= container.size())
                container.resize(i->second + 1);
            container[i->second] = entry.second;
        }
    }
}

template<class T>
void VisDisplayGraph::ContainerToMappedProperty(const std::vector<T>& container, MappedProperty<T>& map) const
{
    map.Clear();
    // mChannelMap provides Channel Name->Channel Index
    for (const auto& entry : mChannelMap)
        if (entry.second < container.size())
            map[entry.first] = container[entry.second];
}
