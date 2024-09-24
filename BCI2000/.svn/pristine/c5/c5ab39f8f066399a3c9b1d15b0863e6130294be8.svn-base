////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: A class for video frame type visualization windows.
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
#include "VisDisplayVideoFrame.h"

#include "SynchronizedObject.h"
#include <QMenu>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

struct VisDisplayVideoFrame::Private : QObject
{
    Private(VisDisplayVideoFrame *);
    ~Private();

    void ResetAspectRatio();
    void ResetSize();

    VisDisplayVideoFrame *mpSelf;
    VideoFrame mFrame;
    QMenu mMenu;
};

VisDisplayVideoFrame::Private::Private(VisDisplayVideoFrame *pSelf) : mpSelf(pSelf), QObject(pSelf)
{
    QAction *pAction = mMenu.addAction("Reset Size");
    connect(pAction, &QAction::triggered, this, &Private::ResetSize);
    pAction = mMenu.addAction("Reset Aspect Ratio");
    connect(pAction, &QAction::triggered, this, &Private::ResetAspectRatio);
}

VisDisplayVideoFrame::Private::~Private()
{
}

void VisDisplayVideoFrame::Private::ResetAspectRatio()
{
    if (mFrame.Width() && mFrame.Height())
    {
        QWidget *pParent = mpSelf->parentWidget();
        pParent->resize(pParent->width(), (mFrame.Height() * pParent->width()) / mFrame.Width());
        Visconfigs()[mpSelf->mVisID.WindowID()].Put(CfgID::Height, pParent->height(), UserDefined);
    }
}

void VisDisplayVideoFrame::Private::ResetSize()
{
    if (mFrame.Width() && mFrame.Height())
    {
        QWidget *pParent = mpSelf->parentWidget();
        pParent->resize(mFrame.Width(), mFrame.Height());
        Visconfigs()[mpSelf->mVisID.WindowID()].Put(CfgID::Height, pParent->height(), UserDefined);
        Visconfigs()[mpSelf->mVisID.WindowID()].Put(CfgID::Width, pParent->width(), UserDefined);
    }
}

VisDisplayVideoFrame::VisDisplayVideoFrame(const std::string &inSourceID) : VisDisplayLayer(inSourceID), p(nullptr)
{
    setVisible(false);
    p = new Private(this);
    SyncMemory<AfterWrite>();
    this->show();
}

VisDisplayVideoFrame::~VisDisplayVideoFrame()
{
    setVisible(false);
    SyncMemory<AfterWrite>();
    // p is deleted through Qt
}

void VisDisplayVideoFrame::OnSetConfig(ConfigSettings &inConfig)
{
    VisDisplayBase::OnSetConfig(inConfig);
}

void VisDisplayVideoFrame::OnContextMenu(const QPoint &pos)
{
    p->mMenu.exec(mapToGlobal(pos));
}

void VisDisplayVideoFrame::OnVideoFrame(const VideoFrame &inFrame)
{
    if ((p->mFrame.Width() != inFrame.Width()) || (p->mFrame.Height() != inFrame.Height()))
    {
        // Adapt the window's aspect ratio without changing its width.
        if (this->hasFocus() && inFrame.Width() > 0)
        {
            QWidget *pParent = this->parentWidget();
            pParent->resize(pParent->width(), (inFrame.Height() * pParent->width()) / inFrame.Width());
            Visconfigs()[mVisID.WindowID()].Put(CfgID::Height, pParent->height(), UserDefined);
        }
    }
    p->mFrame = inFrame;
    this->update();
}

void VisDisplayVideoFrame::paintEvent(QPaintEvent *iopEvent)
{
    if (p && !p->mFrame.Empty())
    {
        QPainter painter(this);
        QRect rect = this->rect();
        painter.fillRect(rect, Qt::black);
        const uchar* pData = reinterpret_cast<const uchar*>(p->mFrame.RawData());
        int width = p->mFrame.Width(), height = p->mFrame.Height();
        QImage img(pData, width, height, QImage::Format_RGB32);
        // Always draw the full image with proper aspect ratio.
        float ratio = 1.0f * width / height;
        if (rect.width() / ratio > rect.height()) {
            int correctedWidth = rect.height() * ratio;
            rect.setLeft(rect.left() + (rect.width() - correctedWidth) / 2);
            rect.setWidth(correctedWidth);
        }
        else if (rect.height() * ratio > rect.width()) {
            int correctedHeight = rect.width() / ratio;
            rect.setTop(rect.top() + (rect.height() - correctedHeight) / 2);
            rect.setHeight(correctedHeight);
        }
        painter.drawImage(rect, img);
#ifdef _WIN32
        ::Sleep(0);
#endif // _WIN32
    }
    VisDisplayBase::paintEvent(iopEvent);
    iopEvent->accept();
}

void VisDisplayVideoFrame::mousePressEvent(QMouseEvent *iopEvent)
{
    this->parentWidget()->activateWindow();
    VisDisplayBase::mousePressEvent(iopEvent);
}
