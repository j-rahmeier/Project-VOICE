////////////////////////////////////////////////////////////////////////////////
// $Id: FrameComposer.h 7788 2024-01-08 21:27:12Z mellinger $
// Author: mellinger@neurotechcenter.com
// Description: A class that places subimages into a larger image.
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
#ifndef FRAME_COMPOSER_H
#define FRAME_COMPOSER_H

#include "gui/GUI.h"

class FrameComposer
{
public:
  static const int cBitsPerPixel = 32;

  FrameComposer();
  ~FrameComposer();

  FrameComposer(const FrameComposer&) = delete;
  FrameComposer& operator=(const FrameComposer&) = delete;

  FrameComposer& setHeight(int);
  int height() const;
  FrameComposer& setWidth(int);
  int width() const;
  FrameComposer& setRows(int);
  int rows() const;
  FrameComposer& setCols(int);
  int cols() const;

  FrameComposer& putSubImage(int row, int col, int rowSpan, int colSpan, int height, int width, void*);
  FrameComposer& putSubImage(int row, int col, int height, int width, void*);
  bool imageDone() const;
  FrameComposer& clear(RGBColor = RGBColor::LtGray);

  void* imageData() const;

  std::string error() const;

private:
  struct Private;
  Private* p;
};

#endif // FRAME_COMPOSER_H
