////////////////////////////////////////////////////////////////////////////////
// $Id: WebcamRecorder.cpp 7882 2024-03-01 20:34:21Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A thread performing video input from a camera, and saving to
//  to file. Callbacks to a controller are done through a WebcamClient object
//  pointer.
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
#ifndef WEBCAM_THREAD_H
#define WEBCAM_THREAD_H

#include <string>

#include "WebcamController.h"

class WebcamClient;

class WebcamThread
{
public:
    WebcamThread();
    ~WebcamThread();
    WebcamThread(const WebcamThread&) = delete;
    WebcamThread& operator=(const WebcamThread&) = delete;

    bool Start(int cameraIndex, const WebcamController::Parameters&, WebcamClient*);
    void Stop();

    bool StartRecording(const std::string&);
    bool SwitchRecording(const std::string&);
    bool StopRecording();
    enum {
        TimestampNone,
        TimestampTopRight,
        TimestampTopLeft,
        TimestampBottomRight,
        TimestampBottomLeft,
        FrameCounter,
        NumTimestampModes,
    };
    bool SetTimestampMode(int);
    bool SetDecimation(int);
    void SetPreflight(bool);

    std::string WindowTitle() const;
    const std::string& Error() const;

private:
    struct Private;
    Private* p;
};

#endif // WEBCAM_THREAD_H
