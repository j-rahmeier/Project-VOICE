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
#ifndef VIS_DISPLAY_VIDEO_FRAME_H
#define VIS_DISPLAY_VIDEO_FRAME_H

#include "VideoFrame.h"
#include "VisDisplayLayer.h"

class VisDisplayVideoFrame : public VisDisplayLayer
{
    Q_OBJECT

  public:
    VisDisplayVideoFrame(const std::string &visID);
    virtual ~VisDisplayVideoFrame();

  protected:
    void OnSetConfig(ConfigSettings &) override;
    void OnVideoFrame(const VideoFrame &) override;
    void OnContextMenu(const QPoint &) override;

  protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;

  private:
    struct Private;
    Private *p;
};

#endif // VIS_DISPLAY_VIDEO_FRAME_H
