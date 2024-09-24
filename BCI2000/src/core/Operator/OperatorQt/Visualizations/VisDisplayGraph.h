////////////////////////////////////////////////////////////////////////////////
// $Id: VisDisplayGraph.h 8114 2024-05-06 14:25:21Z mellinger $
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
#ifndef VIS_DISPLAY_GRAPH_H
#define VIS_DISPLAY_GRAPH_H

#include "DecimationFilter.h"
#include "DisplayFilter.h"
#include "MappedProperty.h"
#include "SignalWidget.h"
#include "VisDisplayLayer.h"

#include <map>
#include <string>

class QMenu;
class QAction;
class QMouseEvent;
class QLabel;
class QStatusBar;
class QListWidget;
class QListWidgetItem;

class VisDisplayGraph : public VisDisplayLayer
{
    Q_OBJECT

    enum
    {
        cMaxUserScaling = 32,      // The maximum number of scaling steps a user
                                   // can take from the default.
        cDisplayOversampling = 10, // The number of data points drawn per pixel, if available.
        cStatusBarHeight = 12,     // The height of the status bar, in pixels.
    };

  public:
    VisDisplayGraph(const std::string &visID);
    virtual ~VisDisplayGraph();

  protected:
    void OnSignal(const GenericSignal &) override;
    void OnSetConfig(ConfigSettings &) override;
    void OnContextMenu(const QPoint &) override;

    // User interaction.
  private:
    void OnGraphContextMenu(const QPoint&);
    void OnChannelListContextMenu(const QPoint&);
    void BuildStatusBar();
    void ShowStatusBar(bool);
    void BuildGraphContextMenu();
    void BuildChannelListContextMenu();

    QMenu *mpGraphContextMenu, *mpHPMenu, *mpLPMenu, *mpNotchMenu;
    QAction *mpActEnlargeSignal, *mpActReduceSignal, *mpActResetSignalScale, *mpActToggleAutoScale,
        *mpActFewerSamples, *mpActMoreSamples, *mpActResetSampleZoom,
        *mpActMoreChannels, *mpActFewerChannels, *mpActToggleDisplayMode, *mpActToggleNumericValues,
        *mpActToggleBaselines, *mpActToggleChannelLabels, *mpActToggleColor, *mpActInvertDisplay,
        *mpActLargerFont, *mpActSmallerFont, *mpActToggleValueUnit,
        *mpActToggleChannelList, *mpActChannelList_Enable, *mpActChannelList_Disable,
        *mpActChannelList_Enlarge, *mpActChannelList_Reduce, *mpActChannelList_Reset,
        *mpActChannelList_ShowUnit, *mpActChannelList_HideUnit, *mpActChannelList_ChooseColor;
    SignalWidget *mpSignalWidget;
    QListWidget *mpChannelList;
    QMenu *mpChannelListContextMenu;
    QStatusBar *mpStatusBar;
    QLabel *mpStatusLabel;

  private slots:
    // Menu action slots
    void EnlargeSignal();
    void ReduceSignal();
    void ResetSignalScale();
    void ToggleAutoScale();
    void FewerSamples();
    void MoreSamples();
    void ResetSampleZoom();
    void MoreChannels();
    void FewerChannels();
    void ToggleDisplayMode();
    void ToggleNumericValues();
    void ToggleValueUnit();
    void ToggleBaselines();
    void ToggleChannelLabels();
    void LargerFont();
    void SmallerFont();
    void ToggleChannelList();
    void ToggleColor();
    void InvertDisplay();

    void SetHPOff();
    void SetHP01();
    void SetHP1();
    void SetHP5();
    void SetLPOff();
    void SetLP30();
    void SetLP40();
    void SetLP70();
    void SetNotchOff();
    void SetNotch50();
    void SetNotch60();

    void ChannelList_DisableAll();
    void ChannelList_EnableAll();
    void ChannelList_Enable();
    void ChannelList_Disable();
    void ChannelList_EnlargeSignal();
    void ChannelList_ReduceSignal();
    void ChannelList_ResetSignalScale();
    void ChannelList_ShowUnit();
    void ChannelList_HideUnit();
    void ChannelList_ChooseColor();
    void OnChannelListItemChanged(QListWidgetItem*);

private:
    bool EnlargeSignal_Enabled() const;
    bool ReduceSignal_Enabled() const;
    bool ResetSignalScale_Enabled() const;
    bool ToggleAutoScale_Enabled() const;
    bool ToggleAutoScale_Checked() const;
    bool FewerSamples_Enabled() const;
    bool MoreSamples_Enabled() const;
    bool ResetSampleZoom_Enabled() const;
    bool MoreChannels_Enabled() const;
    bool FewerChannels_Enabled() const;
    bool ToggleNumericValues_Enabled() const;
    bool ToggleNumericValues_Checked() const;
    bool ToggleBaselines_Enabled() const;
    bool ToggleBaselines_Checked() const;
    bool ToggleValueUnit_Enabled() const;
    bool ToggleValueUnit_Checked() const;
    bool ToggleChannelLabels_Enabled() const;
    bool ToggleChannelLabels_Checked() const;
    bool ToggleChannelList_Enabled() const;
    bool ToggleChannelList_Checked() const;
    bool SmallerFont_Enabled() const;
    bool ToggleColor_Enabled() const;
    bool ToggleColor_Checked() const;
    bool InvertDisplay_Checked() const;
    bool Filter_Enabled() const;
    double FilterUnitToValue(const std::string &inUnit) const;
    double FilterCaptionToValue(const char *inCaption) const;

    int NominalDisplaySamples() const;
    void SetNominalDisplaySamples(int);
    double NominalUnitsPerSample() const;
    void SetNominalUnitsPerSample(double);

    void SetHP(const std::string &, bool writeToConfig = false);
    void SetLP(const std::string &, bool writeToConfig = false);
    void SetNotch(const std::string &, bool writeToConfig = false);
    void UpdateStatusBar();
    QAction *SyncFilterMenu(const std::string &, QMenu *);
    QAction *NextMenuItem(QMenu *, QAction *);

    void BuildChannelList();
    void UpdateChannelList(const LabelList&);

    bool ChannelList_Enabled() const;
    bool ChannelList_EnlargeSignal_Enabled() const;
    bool ChannelList_ReduceSignal_Enabled() const;
    bool ChannelList_ChooseColor_Enabled() const;
    bool ChannelList_ShowUnit_Enabled() const;
    bool ChannelList_HideUnit_Enabled() const;
    void ChannelList_SetUnitVisible(bool);

    void ApplyUserScaling();
    void SetUserScalingForChannel(int ch, int value);
    int GetUserScalingForChannel(int ch) const;
    void ApplyAutoScale(const PhysicalUnit&);

    void SaveValueUnitsVisible() const;

    void ApplyUserZoom();

    template<class T> void MappedPropertyToContainer(const MappedProperty<T>&, std::vector<T>&) const;
    template<class T> void ContainerToMappedProperty(const std::vector<T>&, MappedProperty<T>&) const;

  private:
    int mNumChannels, mSignalElements, mConfigDisplaySamples;
    double mUserScaling, mUserZoom;
    std::vector<double> mChannelUserScaling;
    double mMinValue, mMaxValue;
    std::vector<double> mMinValues, mMaxValues;
    std::string mLastHP, mLastLP, mLastNotch, mAutoScaleTime;
    QAction *mpCurrentHPItem, *mpCurrentLPItem, *mpCurrentNotchItem;
    DisplayFilter mDisplayFilter;
    DecimationFilter mDecimationFilter;
    bool mAutoScale, mFixedScale;
    float mElementGain;

    std::map<std::string, int> mChannelMap;
    MappedProperty<bool> mChannelsVisible;
    ColorList mCustomColors;

  protected:
    // Qt event handlers
    void moveEvent(QMoveEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void keyReleaseEvent(QKeyEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
};

#endif // VIS_DISPLAY_GRAPH_H
