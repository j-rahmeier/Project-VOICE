////////////////////////////////////////////////////////////////////////////////
// $Id: VisDisplayWindow.h 7849 2024-02-21 18:20:24Z mellinger $
// Authors: griffin.milsap@gmail.com, juergen.mellinger@uni-tuebingen.de
// Description: Root window class for visualization displays.
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
#ifndef VIS_DISPLAY_WINDOW_H
#define VIS_DISPLAY_WINDOW_H

#include "VisDisplayBase.h"
#include <QStackedLayout>

class VisDisplayContainer;

class VisDisplayWindow : public VisDisplayBase
{
    Q_OBJECT

  public:
    VisDisplayWindow(const std::string &visID, bool createLayout = true);
    virtual ~VisDisplayWindow();

    std::string WindowID() const
    {
        return mVisID.WindowID();
    }

    static void SetParentWindow(QWidget *);
    static void SetWindowMenu(QMenu *);

  protected:
    void moveEvent(QMoveEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void OnSetConfig(ConfigSettings &) override;

  private:
    static void buildWindowMenu(QMenu *);

    bool mUserIsMoving;
    QStackedLayout *mpLayout;
};

#endif // VIS_DISPLAY_WINDOW_H