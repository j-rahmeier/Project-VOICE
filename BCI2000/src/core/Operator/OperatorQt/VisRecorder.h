//////////////////////////////////////////////////////////////////////
// $Id: VisRecorder.h 7735 2023-11-28 18:54:02Z mellinger $
// Authors: mellinger@neurotechcenter.org
// Description: A class that allows to record a sequence of bitmap
//   images to a video file.If images come with layer IDs (inside
//   visualization IDs), they are automatically joined into a single
//   video stream.
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
///////////////////////////////////////////////////////////////////////
#ifndef VIS_RECORDER_H
#define VIS_RECORDER_H

#include <string>
#include <map>

#include "BitmapImage.h"

class VisRecorder
{
private:
    VisRecorder(const std::string& visID);
    ~VisRecorder();
    void OnStartRun(const std::string& videoFile);
    void OnNextFilePart(const std::string& nextVideoFile);
    void OnStopRun();
    void OnHandleBitmap(const std::map<std::string, BitmapImage>&);

private:
    struct Private;
    Private* p;

public:
    static bool VisualizationRegistered(const std::string& visID);
    static void Clear();
    static void StartRecording(const std::string& visID);
    static void StopRecording(const std::string& visID);
    static void StartRun(const std::string& runFile);
    static void NextFilePart(int filePart);
    static void StopRun();
    static void HandleBitmap(const std::string&, const BitmapImage&);

private:
    static std::string VideoFileName(const std::string& visID);
    static std::string EventName(const std::string& visID);

    static std::string sRunFile;
    static int sFilePart;
    static std::map<std::string, VisRecorder*> sRecorders;
    static std::map<std::string, std::map<std::string, BitmapImage>> sImages;
};

#endif // VIS_RECORDER_H
