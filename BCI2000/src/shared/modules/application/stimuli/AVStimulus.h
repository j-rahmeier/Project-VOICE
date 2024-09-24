////////////////////////////////////////////////////////////////////////////////
// $Id: AVStimulus.h 7990 2024-04-03 20:43:04Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A stimulus that plays audio and/or video when being presented.
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
#ifndef AV_STIMULUS_H
#define AV_STIMULUS_H

#include "Stimulus.h"
#include "utils/AV/VideoPlayer.h"

class AVStimulus : public Stimulus, public VideoPlayer
{
  public:
    AVStimulus(GUI::GraphDisplay &);
    ~AVStimulus();

    const StringUtils::String &Error() const;
    AVStimulus &SetSource(const std::string &inSource);
    const StringUtils::String &Source() const;
    AVStimulus &SetVolume(double);
    double Volume() const;

    const StringUtils::String &AudioDevice() const;
    const StringUtils::String &VideoDevice() const;

    AVStimulus &SetAbortOnConceal(bool);
    bool AbortOnConceal() const;

  protected:
    const char* OnRenderingTick(const ClockTick&, bool) override;
    void OnPresent() override;
    void OnConceal() override;
    void OnStop() override;

  private:
    using VideoPlayer::Open;
    using VideoPlayer::Close;
    using VideoPlayer::File;
    using VideoPlayer::SetFile;
    using VideoPlayer::OnStateChange;
    using VideoPlayer::SetOnStateChange;
    using VideoPlayer::Error;

    struct Data;
    Data *d;
};

#endif // AV_STIMULUS_H
