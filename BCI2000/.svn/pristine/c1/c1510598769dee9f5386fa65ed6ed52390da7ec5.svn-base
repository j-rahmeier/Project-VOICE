////////////////////////////////////////////////////////////////////////////////
// Authors: lijintao@SH-lijintao
// Description: NeuSenWADC header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_NEUSENWADC_H
#define INCLUDED_NEUSENWADC_H

#include "BufferedADC.h"
#include "NeuracleSDK.h"
#include "windows.h"

class NeuSenWADC : public BufferedADC
{
 public:
  NeuSenWADC();
  ~NeuSenWADC();
  void OnPublish() override;
  void OnAutoConfig() override;
  void OnPreflight( SignalProperties& Output ) const override;
  void OnInitialize( const SignalProperties& Output ) override;
  void OnStartAcquisition() override;
  void DoAcquire( GenericSignal& Output ) override;
  void OnStopAcquisition() override;

 private:
	 NeuracleController* m_Controller;
	 bool m_trigger_box;
	 char* m_device_address;
	 char* m_trigger_address;

	 int m_sample_rate;
	 int m_sample_block;

	 HANDLE m_com;
	 LPCSTR m_port_num;
};

#endif
