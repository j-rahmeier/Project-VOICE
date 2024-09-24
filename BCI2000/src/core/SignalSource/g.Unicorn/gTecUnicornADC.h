////////////////////////////////////////////////////////////////////////////////
// Authors: madamek@neurotechcenter.org
// Description: gTecUnicornADC header
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

#ifndef INCLUDED_GTECUNICORNADC_H // makes sure this header is not included more than once
#define INCLUDED_GTECUNICORNADC_H

#include "BufferedADC.h"
#include "Unicorn.imports.h"

class gTecUnicornADC : public BufferedADC
{
  public:
    gTecUnicornADC();
    ~gTecUnicornADC();
    void OnPublish() override;
    void OnAutoConfig() override;
    void OnPreflight(SignalProperties &Output) const override;
    void OnInitialize(const SignalProperties &Output) override;
    void OnStartAcquisition() override;
    void DoAcquire(GenericSignal &Output) override;
    void OnStopAcquisition() override;

  private:
    // Use this space to declare any gTecUnicornADC-specific methods and member variables you'll need

    UNICORN_HANDLE mDeviceHandle;
    float *mpBuffer;
    unsigned int mNumberOfSignalChannels;
    int mNumberOfSamples; // samplingfreq & blocksize
    int mbufferSize;
    float mSamplingRate;

    std::string ChannelNames[UNICORN_EEG_CHANNELS_COUNT] = {"Fz", "Cz", "P3", "Pz", "P5", "Oz", "Po8", "Po7"};

    void printErrorString(unsigned int err);
};

#endif // INCLUDED_GTECUNICORNADC_H
