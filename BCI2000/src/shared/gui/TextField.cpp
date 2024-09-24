////////////////////////////////////////////////////////////////////////////////
// $Id: TextField.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A GraphObject displaying a line of text.
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
#if USE_QT
#include "QtProxyThread.h"
#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#endif

#include "SynchronizedObject.h"
#include "TextField.h"

struct TextField::Private
{
    TextField *mpSelf;
    struct Properties
    {
        std::string mText;
        float mTextHeight, mOriginalHeight;
        RGBColor mColor, mTextColor;
#if USE_QT
        QFont mFont;
        QString mTextCache;
#endif
    };
    SynchronizedObject<Properties> mProperties;
#if USE_QT
    void DoPaint(const GUI::DrawContext &, RGBColor, RGBColor);
    void OnChange(GUI::DrawContext &);
    void OnResize(GUI::DrawContext &);
#endif
};

#if USE_QT
// Access to Qt font functionality requires a QThread.
static QtProxyThread &ProxyThread()
{
    static QtProxyThread sInstance;
    return sInstance;
}
#endif

TextField::TextField(GUI::GraphDisplay &display, int zOrder) : GraphObject(display, zOrder), p(new Private)
{
    p->mpSelf = this;
    WithLocked(prop = p->mProperties.Mutable())
    {
        prop->mTextHeight = 1.0;
        prop->mColor = RGBColor::NullColor;
        prop->mTextColor = RGBColor::Gray;
        prop->mOriginalHeight = -1;
    }
#if USE_QT
    ProxyThread();
#endif
}

TextField::~TextField()
{
    DestructorEntered();
    delete p;
}

TextField &TextField::SetText(const std::string &s)
{
    p->mProperties.Mutable()->mText = s;
    Change(Other);
    return *this;
}

std::string TextField::Text() const
{
    return p->mProperties->mText;
}

TextField &TextField::SetTextHeight(float f)
{
    p->mProperties.Mutable()->mTextHeight = f;
    Change(Other);
    return *this;
}

float TextField::TextHeight() const
{
    return p->mProperties->mTextHeight;
}

TextField &TextField::SetTextColor(RGBColor c)
{
    p->mProperties.Mutable()->mTextColor = c;
    Change(Other);
    return *this;
}

RGBColor TextField::TextColor() const
{
    return p->mProperties->mTextColor;
}

TextField &TextField::SetColor(RGBColor c)
{
    p->mProperties.Mutable()->mColor = c;
    Change(Other);
    return *this;
}

RGBColor TextField::Color() const
{
    return p->mProperties->mColor;
}

void TextField::OnChange(GUI::DrawContext& ioDC)
{
#if USE_QT
    MemberCall<void(Private*, GUI::DrawContext&)> onChange(&Private::OnChange, p, ioDC);
    ProxyThread().Run(&onChange);
#endif
}

#if USE_QT
void TextField::Private::OnChange(GUI::DrawContext& ioDC)
{
    WithLocked(prop = mProperties.Mutable())
    {
        int width = ioDC.rect.right - ioDC.rect.left, height = ioDC.rect.bottom - ioDC.rect.top,
            hCenter = (ioDC.rect.right + ioDC.rect.left) / 2, vCenter = (ioDC.rect.bottom + ioDC.rect.top) / 2;
        if (prop->mOriginalHeight < 0)
            prop->mOriginalHeight = height;
        int fontSize = prop->mTextHeight * prop->mOriginalHeight;

        prop->mTextCache = QString::fromUtf8(prop->mText.c_str());
        prop->mFont.setFamily("Helvetica");
        prop->mFont.setStyleStrategy(QFont::ForceOutline);
        prop->mFont.setBold(true);

        QSize textSize = { 0, 0 };
        if (fontSize > 0)
        {
            prop->mFont.setPixelSize(fontSize);
            QFontMetrics fm(prop->mFont);
            textSize = fm.size(0, prop->mTextCache);
            textSize.setWidth(textSize.width() + fontSize / 2);
        }

        switch (mpSelf->ScalingMode())
        {
        case GUI::ScalingMode::AdjustWidth:
            width = textSize.width();
            break;

        case GUI::ScalingMode::AdjustHeight:
            if (textSize.width() && height)
                height = (height * width) / textSize.width();
            else
                height = textSize.height();
            break;

        case GUI::ScalingMode::AdjustBoth:
            height = textSize.height();
            width = textSize.width();
            break;

        case GUI::ScalingMode::AdjustNone:
            [[fallthrough]];
        default:;
        }
        ioDC.rect.left = hCenter - width / 2.0;
        ioDC.rect.right = hCenter + width / 2.0;
        ioDC.rect.top = vCenter - height / 2.0;
        ioDC.rect.bottom = vCenter + height / 2.0;
    }
}
#endif

void TextField::OnMove(GUI::DrawContext &)
{
}

void TextField::OnResize(GUI::DrawContext &ioDC)
{
#if USE_QT
    MemberCall<void(Private *, GUI::DrawContext &)> onResize(&Private::OnResize, p, ioDC);
    ProxyThread().Run(&onResize);
#endif
}

#if USE_QT
void TextField::Private::OnResize(GUI::DrawContext &ioDC)
{
    WithLocked(prop = mProperties.Mutable())
    {
        int width = ioDC.rect.right - ioDC.rect.left, height = ioDC.rect.bottom - ioDC.rect.top,
            hCenter = (ioDC.rect.right + ioDC.rect.left) / 2, vCenter = (ioDC.rect.bottom + ioDC.rect.top) / 2,
            fontSize = prop->mTextHeight * height;
        prop->mOriginalHeight = height;

        QSize textSize = { 0, 0 };
        if (fontSize > 0)
        {
            prop->mFont.setPixelSize(fontSize);
            QFontMetrics fm(prop->mFont);
            textSize = fm.size(0, prop->mTextCache);
            textSize.setWidth(textSize.width() + fontSize / 2);
        }

        switch (mpSelf->ScalingMode())
        {
        case GUI::ScalingMode::AdjustWidth:
            width = textSize.width();
            break;

        case GUI::ScalingMode::AdjustHeight:
            if (textSize.width() && height)
                height = (height * width) / textSize.width();
            else
                height = textSize.height();
            break;

        case GUI::ScalingMode::AdjustBoth:
            height = textSize.height();
            width = textSize.width();
            break;

        case GUI::ScalingMode::AdjustNone:
            [[fallthrough]];
        default:;
        }
        ioDC.rect.left = hCenter - width / 2.0;
        ioDC.rect.right = hCenter + width / 2.0;
        ioDC.rect.top = vCenter - height / 2.0;
        ioDC.rect.bottom = vCenter + height / 2.0;
    }
}
#endif // USE_QT

void TextField::OnPaint(const GUI::DrawContext &inDC)
{
    RGBColor textColor, backgroundColor;
    WithLocked(prop = p->mProperties.Const())
    {
        textColor = prop->mTextColor;
        backgroundColor = prop->mColor;
    }
    DoPaint(inDC, textColor, backgroundColor);
}

void TextField::DoPaint(const GUI::DrawContext &inDC, RGBColor inTextColor, RGBColor inBackgroundColor)
{
#if USE_QT
    MemberCall<void(Private *, const GUI::DrawContext &, RGBColor, RGBColor)> doPaint(&Private::DoPaint, p, inDC,
                                                                                      inTextColor, inBackgroundColor);
    ProxyThread().Run(&doPaint);
#endif
}

#if USE_QT
void TextField::Private::DoPaint(const GUI::DrawContext &inDC, RGBColor inTextColor, RGBColor inBackgroundColor)
{
    WithLocked(prop = mProperties.Const())
    {
        QPainter *painter = inDC.handle.dc;
        QRect rect(inDC.rect.left, inDC.rect.top, inDC.rect.Width(), inDC.rect.Height());
        painter->setClipRect(rect);
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        if (prop->mColor != RGBColor::NullColor)
        {
            QColor backColor(prop->mColor.R(), prop->mColor.G(), prop->mColor.B());
            brush.setColor(backColor);
            painter->fillRect(rect, brush);
        }

        QColor textColor(inTextColor.R(), inTextColor.G(), inTextColor.B());
        QPen pen;
        brush.setColor(textColor);
        pen.setColor(textColor);
        painter->setPen(pen);
        painter->setBrush(brush);
        painter->setFont(prop->mFont);

        if (prop->mTextHeight > 0)
            painter->drawText(rect, Qt::AlignCenter, prop->mTextCache);
    }
}
#endif // USE_QT
