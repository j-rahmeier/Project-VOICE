////////////////////////////////////////////////////////////////////////////////
// $Id: DisplayWindow.h 7645 2023-10-05 17:37:31Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A base class for the application's user display.
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
#ifndef DISPLAY_WINDOW_H
#define DISPLAY_WINDOW_H

#include "ClockTick.h"
#include "GraphDisplay.h"
#include "StringUtils.h"

namespace GUI
{

class DisplayWindow : public GraphDisplay
{
  public:
    enum
    {
        OnUpdateReceived = GraphDisplay::NextAvailableEvent,
        OnUpdateDisplayed,
        NextAvailableEvent
    };

    virtual ~DisplayWindow() {}

    // Properties
    virtual DisplayWindow &SetTitle(const std::string &) = 0;
    virtual const std::string &Title() const = 0;
    virtual DisplayWindow &SetLeft(int) = 0;
    virtual int Left() const = 0;
    virtual DisplayWindow &SetTop(int) = 0;
    virtual int Top() const = 0;
    virtual DisplayWindow &SetWidth(int) = 0;
    virtual int Width() const = 0;
    virtual DisplayWindow &SetHeight(int) = 0;
    virtual int Height() const = 0;
    virtual DisplayWindow &SetVisible(bool) = 0;
    virtual bool Visible() const = 0;
    DisplayWindow &Show()
    {
        return SetVisible(true);
    }
    DisplayWindow &Hide()
    {
        return SetVisible(false);
    }
    virtual DisplayWindow& SetFullscreen(bool) = 0;
    virtual bool Fullscreen() const = 0;
    virtual DisplayWindow &SetCursorVisible(bool) = 0;
    virtual bool CursorVisible() const = 0;

    // An argument to Update() will be available in RenderState::userData when OnUpdateReceived
    // or OnUpdateDisplayed is broadcast.
    virtual DisplayWindow &Update(void * = 0) = 0;

    virtual DisplayWindow &SetAsyncRendering(bool) = 0;
    virtual bool AsyncRendering() const = 0;
    virtual DisplayWindow &SetUpdateRateDecimation(int) = 0;
    virtual int UpdateRateDecimation() const = 0;

    struct RenderState
    {
        int64_t contentFrame, displayFrame;
        Time requested, displayed;
        bool presentation;
        void *userData;
        RenderState(int64_t f = 0, Time t = Time::Never(), void *p = nullptr)
            : contentFrame(f), displayFrame(-1), requested(t), displayed(Time::Never()), presentation(false), userData(p)
        {
        }
    };

    virtual ClockTick CurrentFrame() const = 0;
    virtual ClockTick::Statistics FrameClockStatistics() const = 0;
    struct RenderStatistics
    {
        int decimation;
        struct
        {
            Time::Interval mean, std;
        } render;
    };
    virtual struct RenderStatistics RenderStatistics() const = 0;
    struct PresentationStatistics
    {
        int64_t count, late;
        struct
        {
            Time::Interval mean, std;
        } latency;
    };
    virtual struct PresentationStatistics PresentationStatistics() const = 0;
    virtual StringUtils::NameValueList Info() const = 0;

    virtual DisplayWindow &ConfigureBitmapBuffer(int width, int height) = 0;
};

} // namespace GUI

#endif // DISPLAY_WINDOW_H
