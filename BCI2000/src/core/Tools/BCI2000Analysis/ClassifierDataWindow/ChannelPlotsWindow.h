////////////////////////////////////////////////////////////////////////////////
// $Id: ChannelPlotsWindow.h 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Channel plots window for BCI2000 Analysis.
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
#ifndef CHANNEL_PLOTS_WINDOW_H
#define CHANNEL_PLOTS_WINDOW_H

#include <QMainWindow>

class GenericSignal;
class SignalProperties;

class ChannelPlotsWindow : public QMainWindow
{
public:
    ChannelPlotsWindow(const GenericSignal&, const GenericSignal&, const GenericSignal&, int ch, QWidget*);
    ~ChannelPlotsWindow();
    ChannelPlotsWindow(const ChannelPlotsWindow&) = delete;
    ChannelPlotsWindow& operator=(const ChannelPlotsWindow&) = delete;

    static int askForChannel(const SignalProperties*);
};

#endif // CHANNEL_PLOTS_WINDOW_H
