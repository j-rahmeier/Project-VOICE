//////////////////////////////////////////////////////////////////////////////
// $Id: SignalWidget.h 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A widget to display signals from a BCI2000 file.
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
//////////////////////////////////////////////////////////////////////////////
#ifndef SIGNAL_WIDGET_H
#define SIGNAL_WIDGET_H

#include "SignalDisplay.h"
#include <QLabel>
#include <QWidget>

class SignalWidget : public QWidget
{
    Q_OBJECT

  public:
    SignalWidget(QWidget * = NULL);
    SignalDisplay &Display();
    const SignalDisplay &Display() const;

  protected:
    // Qt event handlers
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;

  private:
    void UpdateDrawContext();

    SignalDisplay mDisplay;
};

#endif // SIGNAL_WIDGET_H
