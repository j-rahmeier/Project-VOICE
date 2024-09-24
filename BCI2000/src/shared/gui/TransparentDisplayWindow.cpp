////////////////////////////////////////////////////////////////////////////////
// $Id: TransparentDisplayWindow.cpp 7766 2023-12-14 22:40:44Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A GraphDisplay descendant which is a frameless, transparent GUI
//   window for an application's user display.
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
#include "TransparentDisplayWindow.h"

#include "Debugging.h"

#include <QWidget>
#include <QScreen>
#include <QPainter>
#include <QPaintEvent>

#if _WIN32
#include "dwmapi.h"
#endif

namespace GUI {

    struct TransparentDisplayWindow::Data : QWidget
    {
        Data(GraphDisplay& display)
            : mrDisplay(display)
        {
            setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
            setAttribute(Qt::WA_TranslucentBackground);
            setAttribute(Qt::WA_NoSystemBackground);
            setAttribute(Qt::WA_PaintOnScreen);
#if _WIN32
            HRESULT DwmEnableMMCSSResult = ::DwmEnableMMCSS(TRUE);
            Assert(SUCCEEDED(DwmEnableMMCSSResult));
#endif
        }

        void paintEvent(QPaintEvent*) override
        {
            GUI::DrawContext dc = {0};
            dc.device = this;
            dc.rect.left = 0;
            dc.rect.right = width();
            dc.rect.top = 0;
            dc.rect.bottom = height();
            mrDisplay.SetContext(dc);
            mrDisplay.Paint();
        }

        GraphDisplay& mrDisplay;
        int mBitmapBufferWidth = 0, mBitmapBufferHeight = 0;
        bool mFullscreen = false;
        std::string mTitle;
    };


    TransparentDisplayWindow::TransparentDisplayWindow()
        : d(new Data(*this))
    {
    }

    TransparentDisplayWindow::~TransparentDisplayWindow()
    {
        delete d;
    }

    DisplayWindow& TransparentDisplayWindow::Update(void*)
    {
        d->update();
        return *this;
    }

    DisplayWindow& TransparentDisplayWindow::SetAsyncRendering(bool)
    {
        return *this;
    }

    bool TransparentDisplayWindow::AsyncRendering() const
    {
        return false;
    }

    DisplayWindow& TransparentDisplayWindow::SetUpdateRateDecimation(int)
    {
        return *this;
    }

    int TransparentDisplayWindow::UpdateRateDecimation() const
    {
        return 1;
    }

    StringUtils::NameValueList TransparentDisplayWindow::Info() const
    {
        StringUtils::NameValueList info;
        static const float mmPerInch = 25.4;
        float xpxPerMM = d->logicalDpiX() / mmPerInch,
              ypxPerMM = d->logicalDpiY() / mmPerInch;
        info["PhysicalWidth"] << d->widthMM() << "mm";
        info["PhysicalHeight"] << d->heightMM() << "mm";
        info["PhysicalLeftEdge"] << d->pos().x() / xpxPerMM << "mm";
        info["PhysicalTopEdge"] << d->pos().y() / ypxPerMM << "mm";
        info["PhysicalScreenWidth"] << d->screen()->physicalSize().width() << "mm";
        info["PhysicalScreenHeight"] << d->screen()->physicalSize().height() << "mm";
        info["LogicalResolutionX"] << xpxPerMM << "px/mm";
        info["LogicalResolutionY"] << ypxPerMM << "px/mm";
        return info;
    }

    bci::ClockTick::Statistics TransparentDisplayWindow::FrameClockStatistics() const
    {
        bci::ClockTick::Statistics statistics = {0};
        return statistics;
    }

    struct DisplayWindow::RenderStatistics TransparentDisplayWindow::RenderStatistics() const
    {
      struct DisplayWindow::RenderStatistics statistics = {0};
        return statistics;
    }

    struct DisplayWindow::PresentationStatistics TransparentDisplayWindow::PresentationStatistics() const
    {
        struct DisplayWindow::PresentationStatistics statistics = { 0 };
        return statistics;
    }

    DisplayWindow& TransparentDisplayWindow::SetTitle(const std::string& s)
    {
        d->mTitle = s;
        d->setWindowTitle(QString::fromUtf8(s.c_str()));
        return *this;
    }

    const std::string& TransparentDisplayWindow::Title() const
    {
        return d->mTitle;
    }

    DisplayWindow& TransparentDisplayWindow::SetLeft(int left)
    {
        d->move(left, d->pos().y());
        return *this;
    }

    int TransparentDisplayWindow::Left() const
    {
        return d->pos().x();
    }

    DisplayWindow& TransparentDisplayWindow::SetTop(int top)
    {
        d->move(d->pos().x(), top);
        return *this;
    }

    int TransparentDisplayWindow::Top() const
    {
        return d->pos().y();
    }

    DisplayWindow& TransparentDisplayWindow::SetWidth(int width)
    {
        d->resize(width, d->size().height());
        return *this;
    }

    int TransparentDisplayWindow::Width() const
    {
        return d->width();
    }

    DisplayWindow& TransparentDisplayWindow::SetHeight(int height)
    {
        d->resize(d->size().width(), height);
        return *this;
    }

    int TransparentDisplayWindow::Height() const
    {
        return d->height();
    }

    DisplayWindow& TransparentDisplayWindow::SetVisible(bool v)
    {
        if (v && !d->isVisible())
        {
            if (d->mFullscreen)
            {
                d->showMinimized();
                d->showNormal();
                d->showFullScreen();
            }
            else
                d->showNormal();
        }
        else if (!v && d->isVisible())
        {
            d->hide();
        }
        return *this;
    }

    bool TransparentDisplayWindow::Visible() const
    {
        return d->isVisible();
    }

    DisplayWindow& TransparentDisplayWindow::SetCursorVisible(bool v)
    {
        if (v)
            d->unsetCursor();
        else
            d->setCursor(Qt::BlankCursor);
        return *this;
    }

    bool TransparentDisplayWindow::CursorVisible() const
    {
        return d->cursor() != Qt::BlankCursor;
    }

    DisplayWindow& TransparentDisplayWindow::SetFullscreen(bool b)
    {
        if (b != d->mFullscreen)
        {
            d->mFullscreen = b;
            if (d->isVisible())
            {
                d->hide();
                SetVisible(true);
            }
        }
        return *this;
    }

    bool TransparentDisplayWindow::Fullscreen() const
    {
        return d->mFullscreen;
    }

    DisplayWindow& TransparentDisplayWindow::ConfigureBitmapBuffer(int width, int height)
    {
        d->mBitmapBufferWidth = width;
        d->mBitmapBufferHeight = height;
        return *this;
    }

    void TransparentDisplayWindow::OnBitmapData(GUI::Bitmap& b, const GUI::Rect* r) const
    {
        b = GUI::Bitmap(d->mBitmapBufferHeight, d->mBitmapBufferWidth);
    }

    ClockTick TransparentDisplayWindow::CurrentFrame() const
    {
        return ClockTick::Null;
    }
}
