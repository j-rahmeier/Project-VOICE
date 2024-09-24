////////////////////////////////////////////////////////////////////////////////
// $Id: SignalSharingDemoWidget.cpp 7919 2024-03-12 14:14:39Z mellinger $
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
#include "SignalSharingDemoWidget.h"

#include "GenericSignal.h"
#include "Runnable.h"
#include "Sockets.h"
#include "Streambuf.h"
#include "MessageChannel.h"
#include "GenericVisualization.h"
#include "StringUtils.h"
#include "SynchronizedObject.h"
#include "Thread.h"

#include <QPaintEvent>
#include <QPainter>
#include <atomic>

struct SignalSharingDemoWidget::Private : MessageChannel
{
    SignalSharingDemoWidget *mpSelf;
    std::vector<QColor> mSignalColors;

    ServerTCPSocket mListeningSocket;
    std::atomic<bool> mConnected;
    SynchronizedObject<GenericSignal> mpSignal;
    double mSamplingRate;

    Thread mThread;
    void ThreadFunc();
    MemberCall<void(Private *)> mThreadCall;

    UnbufferedIO mBuffer;
    // MessageChannel overrides
    bool OnVisSignalProperties(std::istream&) override;
    bool OnVisSignal(std::istream&) override;

    void Invalidate();
    Private();
};

SignalSharingDemoWidget::Private::Private()
    : MessageChannel(mBuffer),
      mThreadCall(&Private::ThreadFunc, this),
      mThread(&mThreadCall, "SignalSharingDemoWidget listening/receiving thread"),
      mSamplingRate(0.0),
      mpSelf(nullptr)
{
    mSignalColors.resize(8);
    for (int i = 0; i < mSignalColors.size(); ++i)
        mSignalColors[i].setHsvF(i * 1.0 / mSignalColors.size(), 1, 0.9);
}

void SignalSharingDemoWidget::Private::Invalidate()
{
    // must use invokeMethod() because we are calling across threads
    QMetaObject::invokeMethod(mpSelf, "update", Qt::QueuedConnection);
}

void SignalSharingDemoWidget::Private::ThreadFunc()
{
    while (!mThread.Terminating())
    { // wait for a connection
        while (mListeningSocket.Input().Wait())
        { // accept pending connection
            ClientTCPSocket clientSocket;
            if (mListeningSocket.WaitForAccept(clientSocket, 0))
            {
                mConnected = true;
                Invalidate();
                mBuffer.SetInput(&clientSocket.Input());
                MessageChannel::ClearIOState();
                while (clientSocket.Input().Wait()) // will be interrupted by Thread::Terminate()
                {
                    MessageChannel::HandleMessage(); // will dispatch to our overridden functions
                                                     // as appropriate
                }
                *mpSignal.Mutable() = GenericSignal();
                mConnected = false;
                Invalidate();
            }
        }
    }
}

bool SignalSharingDemoWidget::Private::OnVisSignalProperties(std::istream& is)
{
    VisSignalProperties properties; // signal properties are wrapped into VisSignalProperties
    properties.Unserialize(is);
    // The only information we need from signal properties is the sampling rate
    // because the signal's dimensions will be transported by signal messages as well.
    double sampleDuration = properties.SignalProperties().ElementUnit().RawToPhysicalValue(1);
    mSamplingRate = 1.0 / sampleDuration;
    return true; // indicate we read our data from the stream
}

bool SignalSharingDemoWidget::Private::OnVisSignal(std::istream& is)
{
    VisSignal visSignal; // signals are wrapped into VisSignal messages
    visSignal.Unserialize(is);
    mpSignal.Mutable()->AssignValues(visSignal.Signal()); // get current signal content
    Invalidate(); // request window update
    return true; // indicate we read our data from the stream
}

SignalSharingDemoWidget::SignalSharingDemoWidget(const QString &inAddress) : p(new Private)
{
    p->mpSelf = this;
    std::string address = inAddress.toStdString();
    p->mListeningSocket.Open(address);
    if (!p->mListeningSocket.Listening())
        throw std_runtime_error << "Cannot listen on " << address;
    p->mThread.Start();
}

SignalSharingDemoWidget::~SignalSharingDemoWidget()
{
    delete p;
}

void SignalSharingDemoWidget::paintEvent(QPaintEvent *ev)
{
    ev->accept();
    WithLocked(pSignal = p->mpSignal.Const()) // lock the signal while reading from it
    {
        if (pSignal->Empty())
        {
            QPainter painter(this);
            painter.fillRect(ev->rect(), Qt::gray);
            painter.setPen(Qt::white);
            painter.drawText(geometry(), Qt::AlignCenter,
                             p->mConnected ? "Waiting for signal ..." : "Waiting for connection ...");
        }
        else
        {
            QPainter painter(this);
            painter.fillRect(ev->rect(), Qt::black);
            painter.setPen(Qt::white);
            painter.drawText(5, 15, QString("Sampling Rate: %1 Hz").arg(p->mSamplingRate));

            float radius = std::min(width(), height()) / 4.0;
            QPointF center = QPointF(width() / 2.0, height() / 2.0);

            double totalEnergy = 0;
            for (int ch = 0; ch < pSignal->Channels(); ++ch)
                for (int el = 0; el < pSignal->Elements(); ++el)
                    totalEnergy += pSignal->Value(ch, el) * pSignal->Value(ch, el);
            float norm = totalEnergy > 0 ? ::sqrt(pSignal->Channels() * pSignal->Elements() / totalEnergy) : 0;

            for (int ch = 0; ch < pSignal->Channels(); ++ch)
            {
                std::vector<QPointF> line;
                line.reserve(pSignal->Elements());
                for (int el = 0; el < pSignal->Elements(); ++el)
                {
                    float r = radius * (1 + norm * pSignal->Value(ch, el) / 4),
                          phi = el * 2 * Pi() / pSignal->Elements();
                    line.push_back(QPointF(center.x() + r * ::cos(phi), center.y() - r * ::sin(phi)));
                }
                if (!line.empty())
                    line.push_back(line.front());
                painter.setPen(p->mSignalColors[ch % p->mSignalColors.size()]);
                painter.drawPolyline(line.data(), line.size());
            }
        }
    }
}
