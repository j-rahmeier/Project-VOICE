////////////////////////////////////////////////////////////////////////////////
// $Id: gNautilusADC.h 7463 2023-06-30 14:59:17Z mellinger $
// Authors:
// Description: gNautilusADC header
//
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

#ifndef INCLUDED_gNautilusADC_H // makes sure this header is not included more than once
#define INCLUDED_gNautilusADC_H

#include "BufferedADC.h"
#include "gNautilusDevice.h"

class gNautilusADC : public BufferedADC
{
  public:
    gNautilusADC();
    virtual ~gNautilusADC();
    void OnPreflight(SignalProperties &Output) const override;
    void OnInitialize(const SignalProperties &Output) override;
    void OnStartAcquisition() override;
    void DoAcquire(GenericSignal &Output) override;
    void OnStopAcquisition() override;
    void OnHalt() override;

  private:
    bool DetermineFilterNumber(int &oFilterNumber, GDS_FILTER_INFO *filters, int nof) const;
    bool DetermineNotchNumber(int &oFilterNumber, GDS_FILTER_INFO *filters, int nof) const;

    static const int cNumberOfDigitalChannelsPerDevice = 8;
    static const int cNumberOfAccelerationChannelsPerDevice = 3;

    size_t mMasterIdx;
    gNautilusDeviceContainer mDevices;

    class SrcCh
    {
      public:
        explicit SrcCh(std::string s);
        int Dev()
        {
            return mDev;
        }
        int Channel()
        {
            return mChannel;
        }

      private:
        int mDev, mChannel;
    };
};

#endif // INCLUDED_gNautilusADC_H
