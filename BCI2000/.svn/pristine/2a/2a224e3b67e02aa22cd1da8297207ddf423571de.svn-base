////////////////////////////////////////////////////////////////////////////////
// Authors: t moore
// Description: NC_flexXADC header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_NC_FLEXXADC_H  // makes sure this header is not included more than once
#define INCLUDED_NC_FLEXXADC_H

#include "BufferedADC.h"

class NC_flexXADC : public BufferedADC
{
  public:
    NC_flexXADC();
    ~NC_flexXADC();
    void OnPublish() override;
    void OnAutoConfig() override;
    void OnPreflight( SignalProperties& Output ) const override;
    void OnInitialize( const SignalProperties& Output ) override;
    void OnStartAcquisition() override;
    void DoAcquire( GenericSignal& Output ) override;
    void OnStopAcquisition() override;

  private:
    // Use this space to declare any NC_flexXADC-specific methods and member variables you'll need
    char* mpBuffer;
    int mNumberOfSignalChannels;
    bool mConnected;
    int mSamplingRate, mDCfilter, mResolutionCode;
};

#endif // INCLUDED_NC_FLEXXADC_H
