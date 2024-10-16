////////////////////////////////////////////////////////////////////////////////
// $Id: SignalWidget.cpp 8270 2024-07-23 19:34:53Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A widget that holds a BCI2000 signal display.
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
#include "SignalWidget.h"
#include "Numeric.h"
#include "SignalDisplay.h"
#include <QMenu>
#include <QMouseEvent>
#include <QToolTip>

struct SignalWidget::Private : QObject
{
    SignalDisplay mDisplay;
    SignalProperties mSignalProperties;
    QWidget *mpWidget;
    bool mContextMenu;

    Private(QWidget *pWidget) : QObject(pWidget), mpWidget(pWidget), mContextMenu(false)
    {
    }
    void setContext();
};

void SignalWidget::Private::setContext()
{
    GUI::DrawContext c;
    c.rect.left = 0;
    c.rect.top = 0;
    c.rect.right = mpWidget->width();
    c.rect.bottom = mpWidget->height();
    c.device = mpWidget;
    mDisplay.SetContext(c);
}

SignalWidget::SignalWidget(QWidget* parent) : QWidget(parent), p(new Private(this))
{
    p->setContext();
}

SignalWidget::SignalWidget(const GenericSignal &signal, QWidget *parent) : QWidget(parent), p(new Private(this))
{
    p->setContext();
    setSignal(signal);
}

void SignalWidget::setSignal(const GenericSignal& signal)
{
    p->mSignalProperties = signal.Properties();
    p->mDisplay.SetNumSamples(signal.Elements());
    p->mDisplay.ScrollForward(signal);
    p->mDisplay.SetDisplayMode(SignalDisplay::field2d);
    p->mDisplay.SetDisplayGroups(signal.Channels());
    double signalMax = -Tiny::Inf<double>(), signalMin = Tiny::Inf<double>();
    for (int ch = 0; ch < signal.Channels(); ++ch)
        for (int el = 0; el < signal.Elements(); ++el)
        {
            signalMax = std::max(signalMax, signal(ch, el));
            signalMin = std::min(signalMin, signal(ch, el));
        }
    p->mDisplay.SetMaxValue(signalMax);
    p->mDisplay.SetMinValue(signalMin);
    LabelList channelLabels;
    for (int ch = 0; ch < signal.Channels(); ++ch)
        channelLabels.push_back(Label(ch, signal.Properties().ChannelLabel(ch)));
    p->mDisplay.SetChannelLabels(channelLabels);
    p->mDisplay.SetChannelLabelsVisible(true);
    p->mDisplay.SetSampleUnit(signal.Properties().ElementUnit().GainWithSymbol());
    p->mDisplay.SetSampleOffset(-signal.Properties().ElementUnit().Offset());
    p->mDisplay.SetValueUnit(signal.Properties().ValueUnit().GainWithSymbol());
}

SignalDisplay &SignalWidget::display()
{
    return p->mDisplay;
}

const SignalDisplay &SignalWidget::display() const
{
    return p->mDisplay;
}

bool SignalWidget::contextMenu() const
{
    return p->mContextMenu;
}

void SignalWidget::setContextMenu(bool b)
{
    p->mContextMenu = b;
}

SignalWidget::~SignalWidget()
{
}

QSize SignalWidget::sizeHint() const
{
    QSize s;
    s.setWidth(p->mSignalProperties.Elements() * 5 + 10);
    s.setHeight(p->mSignalProperties.Channels() * 5 + 10);
    if (s.width() < 320)
        s.setWidth(320);
    if (s.width() > 640)
        s.setWidth(640);
    if (s.height() < 240)
        s.setHeight(240);
    if (s.height() > 480)
        s.setHeight(480);
    return s;
}

QSize SignalWidget::minimumSizeHint() const
{
    QSize s;
    s.setWidth(100);
    s.setHeight(100);
    return s;
}

void SignalWidget::resizeEvent(QResizeEvent *ev)
{
    p->setContext();
    QWidget::resizeEvent(ev);
}

void SignalWidget::paintEvent(QPaintEvent *ev)
{
    p->mDisplay.Paint();
    QWidget::paintEvent(ev);
}

void SignalWidget::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        QPointF pos = ev->pos();
        int s = p->mDisplay.PosToSample(pos.x());
        int ch = p->mDisplay.PosToChannel(pos.y());
        int n = p->mDisplay.ChannelGroupSize();
        if (s >= 0 && s < p->mDisplay.NumSamples() && ch >= 0 && ch + n <= p->mDisplay.NumChannels())
        {
            QStringList values;
            for (int i = 0; i < n; ++i)
                values.push_back(QString::number(p->mDisplay.Data(ch + i, s)));
            QToolTip::showText(ev->globalPos(), values.join(", "), this);
        }
    }
}

void SignalWidget::mouseReleaseEvent(QMouseEvent *)
{
    QToolTip::hideText();
}

void SignalWidget::contextMenuEvent(QContextMenuEvent *ev)
{
    if (!p->mContextMenu)
        return;

    QMenu menu(this);
    QPointF pos = ev->pos();
    int ch = p->mDisplay.PosToChannel(pos.y());
    QString chName;
    if (ch >= 0 && ch < p->mDisplay.NumChannels())
    {
        chName = QString::fromUtf8(p->mSignalProperties.ChannelLabel(ch).c_str());
        QString text = "Show &Curve(s) at channel " + chName;
        menu.addAction(text, [this, ch] { showCurvesAt(ch); });
    }
    else
    {
        ch = -1;
    }
    int s = p->mDisplay.PosToSample(pos.x());
    QString sName;
    if (s >= 0 && s < p->mDisplay.NumSamples())
    {
        std::string label;
        if (!p->mSignalProperties.ElementLabels().IsTrivial())
            label = p->mSignalProperties.ElementLabel(s);
        else
            label = p->mSignalProperties.ElementUnit().RawToPhysical(s);
        sName = QString::fromUtf8(label.c_str());
        QString text = "Show &Distribution at " + sName;
        menu.addAction(text, [this, s] { showDistributionAt(s); });
    }
    else
    {
        s = -1;
    }
    emit addToContextMenu(ch, s, chName, sName, &menu);
    if (!menu.isEmpty())
        menu.exec(ev->globalPos());
}
