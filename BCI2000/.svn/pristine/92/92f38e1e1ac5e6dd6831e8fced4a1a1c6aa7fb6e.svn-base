////////////////////////////////////////////////////////////////////////////////
// $Id $
// Author: mellinger@neurotechcenter.org
// Description: A Visualization object representing a progress bar.
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
#include "ProgressBarVis.h"
#include "Shapes.h"
#include "GraphDisplay.h"

struct ProgressBarVis::Private : GUI::GraphDisplay
{
    int mCurrent, mTotal;
    int mWidth, mHeight;
    BitmapVisualization& mVis;
    RectangularShape mBlock;

    Private(BitmapVisualization& vis) : mVis(vis), mBlock(*this), 
        mCurrent(0), mTotal(1), mWidth(100), mHeight(30)
    {
        this->SetColor(RGBColor::Black);
        mBlock.SetColor(RGBColor::Yellow);
        mBlock.SetCenterY(0.5).SetHeight(1).SetCenterX(0).SetWidth(0);
    }

    void Render()
    {
        GUI::DrawContext dc = { 0 };
        dc.rect.right = mWidth;
        dc.rect.bottom = mHeight;
        this->SetContext(dc);
        GUI::Rect blockRect = { 0, 0, mCurrent * 1.0 / mTotal, 1 };
        mBlock.SetObjectRect(blockRect);
        this->Paint();
    }
};

ProgressBarVis::ProgressBarVis() : BitmapVisualization("ProgressBar"), 
p(new Private(*this))
{
}

ProgressBarVis::~ProgressBarVis()
{
    delete p;
}

ProgressBarVis &ProgressBarVis::SetCurrent(int i)
{
    p->mCurrent = i;
    return *this;
}

int ProgressBarVis::Current() const
{
    return p->mCurrent;
}

ProgressBarVis &ProgressBarVis::SetTotal(int i)
{
    p->mTotal = i;
    return *this;
}

int ProgressBarVis::Total() const
{
    return p->mTotal;
}

ProgressBarVis &ProgressBarVis::SetBackgroundColor(RGBColor c)
{
    p->SetColor(c);
    return *this;
}

RGBColor ProgressBarVis::BackgroundColor() const
{
    return p->Color();
}

ProgressBarVis &ProgressBarVis::SetForegroundColor(RGBColor c)
{
    p->mBlock.SetColor(c).SetFillColor(c);
    return *this;
}

RGBColor ProgressBarVis::ForegroundColor() const
{
    return p->mBlock.FillColor();
}

ProgressBarVis &ProgressBarVis::SetWidth(int width)
{
    p->mWidth = width;
    return *this;
}

int ProgressBarVis::Width() const
{
    return p->mWidth;
}

ProgressBarVis& ProgressBarVis::SetHeight(int height)
{
    p->mHeight = height;
    return *this;
}

int ProgressBarVis::Height() const
{
    return p->mHeight;
}

ProgressBarVis& ProgressBarVis::SendReferenceFrame()
{
    p->Render();
    auto bitmap = p->BitmapData();
    p->mVis.Send(CfgID::Width, p->mWidth);
    p->mVis.Send(CfgID::Height,p->mHeight);
    p->mVis.SendReferenceFrame(bitmap);
    return *this;
}

ProgressBarVis& ProgressBarVis::SendDifferenceFrame()
{
    p->Render();
    auto bitmap = p->BitmapData();
    p->mVis.SendDifferenceFrame(bitmap);
    return *this;
}
