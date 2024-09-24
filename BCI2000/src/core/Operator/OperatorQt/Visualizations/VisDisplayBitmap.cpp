////////////////////////////////////////////////////////////////////////////////
// $Id: VisDisplayBitmap.cpp 7876 2024-02-29 19:23:31Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class for bitmap type visualization windows.
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
#include "VisDisplayBitmap.h"

#include "QtSignalConnector.h"
#include "SynchronizedObject.h"
#include <QMenu>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

struct VisDisplayBitmap::Private : QtSignalConnector
{
    Private(VisDisplayBitmap *);
    ~Private();

    void ResetAspectRatio();
    void ResetSize();

    VisDisplayBitmap *mpSelf;
    bool mWaitForReferenceFrame;
    BitmapImage mFrame;
    QMenu mMenu;
};

VisDisplayBitmap::Private::Private(VisDisplayBitmap *pSelf) : mpSelf(pSelf), mWaitForReferenceFrame(false)
{
    QAction *pAction = mMenu.addAction("Reset Size");
    Connect(pAction, SIGNAL(triggered()), MemberCall<void(Private *)>(&Private::ResetSize, this));
    pAction = mMenu.addAction("Reset Aspect Ratio");
    Connect(pAction, SIGNAL(triggered()), MemberCall<void(Private *)>(&Private::ResetAspectRatio, this));
}

VisDisplayBitmap::Private::~Private()
{
}

void VisDisplayBitmap::Private::ResetAspectRatio()
{
    if (mFrame.Width() && mFrame.Height())
    {
        QWidget *pParent = mpSelf->parentWidget();
        pParent->resize(pParent->width(), (mFrame.Height() * pParent->width()) / mFrame.Width());
        Visconfigs()[mpSelf->mVisID.WindowID()].Put(CfgID::Height, pParent->height(), UserDefined);
    }
}

void VisDisplayBitmap::Private::ResetSize()
{
    if (mFrame.Width() && mFrame.Height())
    {
        QWidget *pParent = mpSelf->parentWidget();
        pParent->resize(mFrame.Width(), mFrame.Height());
        Visconfigs()[mpSelf->mVisID.WindowID()].Put(CfgID::Height, pParent->height(), UserDefined);
        Visconfigs()[mpSelf->mVisID.WindowID()].Put(CfgID::Width, pParent->width(), UserDefined);
    }
}

VisDisplayBitmap::VisDisplayBitmap(const std::string &inSourceID) : VisDisplayLayer(inSourceID), p(nullptr)
{
    setVisible(false);
    p = new Private(this);
    SyncMemory<AfterWrite>();
    this->show();
}

VisDisplayBitmap::~VisDisplayBitmap()
{
    setVisible(false);
    SyncMemory<AfterWrite>();
    delete p;
}

void VisDisplayBitmap::OnSetConfig(ConfigSettings &inConfig)
{
    VisDisplayBase::OnSetConfig(inConfig);
}

void VisDisplayBitmap::OnContextMenu(const QPoint &pos)
{
    p->mMenu.exec(mapToGlobal(pos));
}

void VisDisplayBitmap::OnBitmap(const BitmapImage &inImage)
{
    if (inImage.Empty())
    {
        p->mWaitForReferenceFrame = true;
        return;
    }

    if (p->mWaitForReferenceFrame)
    {
        if ((p->mFrame.Width() != inImage.Width()) || (p->mFrame.Height() != inImage.Height()))
        {
            // Adapt the window's aspect ratio without changing its width.
            if (this->hasFocus() && inImage.Width() > 0)
            {
                QWidget *pParent = this->parentWidget();
                pParent->resize(pParent->width(), (inImage.Height() * pParent->width()) / inImage.Width());
                Visconfigs()[mVisID.WindowID()].Put(CfgID::Height, pParent->height(), UserDefined);
            }
        }
        p->mFrame = inImage;
        p->mWaitForReferenceFrame = false;
    }
    else
    {
        p->mFrame += inImage;
    }
    this->update();
}

void VisDisplayBitmap::paintEvent(QPaintEvent *iopEvent)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setRenderHint(QPainter::TextAntialiasing, false);

    int formWidth = this->width(), formHeight = this->height();
    if (p && !p->mFrame.Empty())
    {
        for (int x = 0; x < p->mFrame.Width(); ++x)
        {
            for (int y = 0; y < p->mFrame.Height(); ++y)
            {
                RGBColor rgb = p->mFrame(x, y);
                if (rgb != RGBColor::NullColor)
                {
                    QRect pixelRect;
                    pixelRect.setLeft((x * formWidth) / p->mFrame.Width());
                    pixelRect.setTop((y * formHeight) / p->mFrame.Height());
                    pixelRect.setRight(((x + 1) * formWidth) / p->mFrame.Width());
                    pixelRect.setBottom(((y + 1) * formHeight) / p->mFrame.Height());
                    painter.fillRect(pixelRect, QColor(rgb.R(), rgb.G(), rgb.B()));
                }
            }
        }
#ifdef _WIN32
        ::Sleep(0);
#endif // _WIN32
    }
    VisDisplayBase::paintEvent(iopEvent);
    iopEvent->accept();
}

void VisDisplayBitmap::mousePressEvent(QMouseEvent *iopEvent)
{
    this->parentWidget()->activateWindow();
    VisDisplayBase::mousePressEvent(iopEvent);
}
