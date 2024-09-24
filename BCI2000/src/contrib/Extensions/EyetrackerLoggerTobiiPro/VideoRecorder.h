////////////////////////////////////////////////////////////////////////////////
// $Id: VideoRecorder.h 7539 2023-08-21 14:51:27Z mellinger $
// Author: mellinger@neurotechcenter.com
// Description: A class that displays video frames in a window, and records
//   to a file.
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
#ifndef VIDEO_RECORDER_H
#define VIDEO_RECORDER_H

#include <string>
#include "gui/GUI.h"

class VideoRecorder
{
public:
  static const int cBitsPerPixel = 32;

  VideoRecorder();
  ~VideoRecorder();

  VideoRecorder(const VideoRecorder&) = delete;
  VideoRecorder& operator=(const VideoRecorder&) = delete;

  VideoRecorder& showWindow();
  VideoRecorder& hideWindow();

  VideoRecorder& setName(const std::string&);
  const std::string& name() const;

  VideoRecorder& setId(int);
  int id() const;

  VideoRecorder& setHeight(int);
  int height() const;
  VideoRecorder& setWidth(int);
  int width() const;
  VideoRecorder& setFramesPerSecond(float);
  float framesPerSecond() const;

  VideoRecorder& startRecording(const std::string& file);
  VideoRecorder& stopRecording();
  bool isRecording() const;

  struct Presentation
  {
    int64_t timeMicroseconds, durationMicroseconds;
  };
  VideoRecorder& setBlank(const Presentation*, bool);
  VideoRecorder& writeFrame(const Presentation*, void*, bool autoBlack = true);
  VideoRecorder& writeBlackFrame(const Presentation*);

  std::string error() const;

private:
  struct Private;
  Private* p;
};

#endif // VIDEO_RECORDER_H
