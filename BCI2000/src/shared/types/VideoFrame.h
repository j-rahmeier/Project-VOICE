////////////////////////////////////////////////////////////////////////////////
// $Id: VideoFrame.h 8039 2024-04-17 18:12:01Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A container for video frames in ARGB32 format, with support for
//   shared memory "transmission" if connected locally.
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
#ifndef VIDEO_FRAME_H
#define VIDEO_FRAME_H

#include "Color.h"
#include <cstdint>
#include <iostream>

class VideoFrame
{
  public:
    class PixelRef;
    friend class PixelRef;

    class PixelRef
    {
      public:
        PixelRef(VideoFrame &f, int x, int y);
        operator RGBColor() const;
        PixelRef &operator=(const RGBColor &c);

      private:
        uint32_t *mpData;
    };

    class ConstPixelRef
    {
    public:
        ConstPixelRef(const VideoFrame& f, int x, int y);
        operator RGBColor() const;

    private:
        const uint32_t* mpData;
    };

  public:
    VideoFrame(int inWidth = 0, int inHeight = 0, const uint32_t* inpData = nullptr);
    VideoFrame(int inWidth, int inHeight, const void*);
    VideoFrame(const VideoFrame &);
    VideoFrame &operator=(const VideoFrame&);
    virtual ~VideoFrame();

    bool Empty() const;
    int Width() const;
    int Height() const;
    const uint32_t *RawData() const;

    VideoFrame& SetBlack();
    VideoFrame& Fill(RGBColor);
    VideoFrame& AssignData(int width, int height, const void*);
    VideoFrame& AssignData(int width, int height, const uint32_t*);

    ConstPixelRef operator()(int x, int y) const;
    PixelRef operator()(int x, int y);

    bool ShareAcrossModules();
    bool IsSharedAcrossModules() const;
    enum { SharedFlag = 1 << 6, };
    std::ostream &Serialize(std::ostream &) const;
    std::istream &Unserialize(std::istream &);

  private:
    void Init(int, int, const uint32_t*);
    void AttachToSharedMemory(const std::string&);
    uint32_t* MutableRawData();

    struct Private;
    Private* p;
};

#endif // VIDEO_FRAME_H
