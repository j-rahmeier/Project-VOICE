////////////////////////////////////////////////////////////////////////////////
// $Id: SoundcardADC.h 7463 2023-06-30 14:59:17Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A source module that receives input from a sound card.
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

#ifndef SOUNDCARD_ADC_H
#define SOUNDCARD_ADC_H

#include "BufferedADC.h"

class SoundcardADC : public BufferedADC
{
  public:
    SoundcardADC();
    virtual ~SoundcardADC();
    void OnPublish() override;
    void OnAutoConfig() override;
    void OnPreflight(SignalProperties &Output) const override;
    void OnInitialize(const SignalProperties &Output) override;
    void OnStartAcquisition() override;
    void OnStopAcquisition() override;
    // Using a callback API, we don't need an acquisition thread:
    bool UseAcquisitionThread() const override
    {
        return false;
    }

  private:
    class AudioStream *CreateStream() const;

    struct Data;
    Data *d;
};

#endif // SOUNDCARD_ADC_H
