//////////////////////////////////////////////////////////////////////////////
// $Id: VideoWidget.h 7463 2023-06-30 14:59:17Z mellinger $
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
#ifndef VIDEO_WIDGET_H
#define VIDEO_WIDGET_H

#include "AVDecoder.h"
#include <QWidget>

class VideoWidget : public QWidget
{
    Q_OBJECT

  public:
    VideoWidget(QWidget *);
    VideoWidget &setVideoFile(const std::string &);
    VideoWidget &setFramePosition(int64_t);

  protected:
    // Qt event handlers
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;

  private:
    AVDecoder mDecoder;
    int mNativeWidth, mNativeHeight;
};

#endif // VIDEO_WIDGET_H
