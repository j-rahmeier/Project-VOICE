////////////////////////////////////////////////////////////////////////////////
// $Id: GLDisplayWindow.h 7464 2023-06-30 15:04:08Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A GraphDisplay descendant which is a frameless GUI window for
//   an application's user display, based on OpenGL.
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
#ifndef GL_DISPLAY_WINDOW_H
#define GL_DISPLAY_WINDOW_H

#include "DisplayWindow.h"

namespace GUI
{

class GLDisplayWindow : public DisplayWindow
{
  public:
    GLDisplayWindow();
    virtual ~GLDisplayWindow();

    // Properties
    DisplayWindow &SetTitle(const std::string &) override;
    const std::string &Title() const override;
    DisplayWindow &SetLeft(int) override;
    int Left() const override;
    DisplayWindow &SetTop(int) override;
    int Top() const override;
    DisplayWindow &SetWidth(int) override;
    int Width() const override;
    DisplayWindow &SetHeight(int) override;
    int Height() const override;
    DisplayWindow &SetVisible(bool) override;
    bool Visible() const override;
    DisplayWindow &Show()
    {
        return SetVisible(true);
    }
    DisplayWindow &Hide()
    {
        return SetVisible(false);
    }
    DisplayWindow& SetFullscreen(bool) override;
    bool Fullscreen() const override;
    DisplayWindow &SetCursorVisible(bool) override;
    bool CursorVisible() const override;

    // An argument to Update() will be available in RenderState::userData when OnUpdateReceived
    // or OnUpdateDisplayed is broadcast.
    DisplayWindow &Update(void * = 0) override;

    DisplayWindow &SetAsyncRendering(bool) override;
    bool AsyncRendering() const override;
    DisplayWindow &SetUpdateRateDecimation(int) override;
    int UpdateRateDecimation() const override;

    ClockTick CurrentFrame() const override;
    ClockTick::Statistics FrameClockStatistics() const override;
    struct DisplayWindow::RenderStatistics RenderStatistics() const override;
    struct DisplayWindow::PresentationStatistics PresentationStatistics() const override;
    StringUtils::NameValueList Info() const override;

    DisplayWindow &ConfigureBitmapBuffer(int width, int height) override;

  protected:
    void OnBitmapData(GUI::Bitmap &, const GUI::Rect *) const override;

  private:
    using GraphDisplay::SetContext;

    struct Data;
    Data *d;
};

} // namespace GUI

#endif // DISPLAY_WINDOW_H
