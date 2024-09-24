////////////////////////////////////////////////////////////////////////////////
// $Id: CfgID.h 8138 2024-05-15 21:10:15Z mellinger $
// Description: Visualization Configuration IDs and their conversion from
//   constant into text form, and back.
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
#ifndef CFG_ID_H
#define CFG_ID_H

#include <string>
#include <cstdint>

class CfgID
{
  public:
    typedef uint8_t IDType;

    enum CfgID_ // Must have a name to be used as a template argument.
    {
        None = 251, // -5

        Top = 252,    // -4
        Left = 253,   // -3
        Width = 254,  // -2
        Height = 255, // -1
        WindowTitle = 1,

        // Graph options
        MinValue, // in raw value units
        MaxValue,
        NumSamples, // number of samples in x direction
        Reserved_1, // was XAxisLabel
        Reserved_2, // was YAxisLabel
        ChannelGroupSize, // number of channels being displayed on the same YTick
        GraphType,
          // Graph types
          Polyline,
            // Polyline options
            ShowBaselines, // true/false
            ChannelColors, // ColorList object

          Field2d,

        // Units (unit gain with physical unit, e.g. "1ms")
        SampleUnit,
        ChannelUnit,
        ValueUnit,

        // Memo options
        NumLines,

        // Label lists (send LabelList objects)
        ChannelLabels, // labels displayed with channels
        GroupLabels, // labels displayed with channel groups if there are multiple channels in a group
        // Tick options (whether to draw ticks and labels)
        ShowXTicks, // true/false
        ShowYTicks, // true/false
        // Marker lists (send LabelList objects containing marker defs of the form "<label>|<color>|<style>|<width>")
        XAxisMarkers, // <color> may be one of the color names defined at https://www.w3.org/TR/SVG/types.html#ColorKeywords
        YAxisMarkers, // <style> may be composed of . and -, defining a line style; any of the elements may be omitted,
                      // <width> is relative to the marker's tick at the axis
        // Miscellaneous
        ShowSampleUnit, // true/false
        ShowChannelUnit, // true/false
        ShowValueUnit, // true/false
        SampleOffset, // number of samples to add when displaying x unit

        Visible, // true/false
        InvertedDisplay, // true/false
        // Set filter property to "off" to disable a filter
        HPFilter, // e.g., "30Hz"
        LPFilter,
        NotchFilter,

        ShowNumericValues, // whether to display the current value as a number
        AutoScale, // auto scale option, true/false
        ChannelStyles, // a string containing characters to plot at sample points, '-' for line
        FixedScale, // true: user cannot adjust value scaling
        ValueColors, // a string in the form "[ { <val1> 0xRRGGBB } ...]" with rgb values ranging from 0 to 255
        FontSize, // in points
        WindowFrame, // display a window frame, true/false

        ValueUnits, // allow for different units in different channels (send a ValueList<std::string>, by channel index)
        MinValues,  // ValueList<double>, allow for different min values in different channels (by channel index)
        MaxValues,  // ValueList<double>, allow for different max values in different channels (by channel index)

        PlacementVis,  // put visualization into a container window, give ContainerVisID as a string argument
        PlacementRow,  // 1-based index of row in container window
        PlacementCol,  // 1-based index of column in container window
        PlacementHeight, // number of rows to extend over
        PlacementWidth, // number of columns to extend over
        PlacementMargin, // margin between matrix entries
        PlacementColor, // color of margin between matrix entries

        ShowStatusBar, // in graph, true/false

        ShowChannelList, // in graph, true/false
        ChannelsVisible, // MappedProperty<bool> object (by channel name)
        ValueUnitsVisible, // MappedProperty<bool> object (by channel name)
        ChannelUserScaling, // MappedProperty<float> object (by channel name)
        CustomColors, // ColorList object
        UserScaling, // log2(magnificationFactor), applied globally
        UserSampleZoom, // log2(magnificationFactor)

        // When adding new IDs, don't forget to add them to the string list in CfgID.cpp as well.
    };

  public:
    CfgID(IDType i) : mID(i)
    {
    }
    CfgID(const std::string &);

    operator IDType() const
    {
        return mID;
    }
    operator std::string() const;

  private:
    IDType mID;
};

#endif // CFG_ID_H
