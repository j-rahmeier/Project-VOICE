////////////////////////////////////////////////////////////////////////////////
// Authors: bci2000@DESKTOP-503T7ML
// Description: RippleADC header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_RIPPLEADC_H  // makes sure this header is not included more than once
#define INCLUDED_RIPPLEADC_H

#include "BufferedADC.h"
#include "xipplib.h"

class RippleADC : public BufferedADC
{
	#define MAX_NUM_CHLS 512
	#define RAW_SAMPLINGRATE 30000
	#define HIRES_SAMPLINGRATE 2000
	#define HIFREQ_SAMPLINGRATE 7500
	#define LFP_SAMPLINGRATE 1000


	typedef enum t_samplingRates
	{
		RAW = (1u << 0), //30k
		HIRES = (1u << 1), //2000
		HiFreq = (1u << 2), //7500
		LFP = (1u << 3), // 1000
		EMG = (1u << 4), // 2000
	} SamplingRates;

	typedef struct t_frontendinfo
	{
		char Identifier[STRLEN_LABEL];
		uint32_t BitDepth;
		uint32_t Channels;
		float Resolution;
		SamplingRates SamplingRates;

	} FrontEndInfo;

	 public:
	  RippleADC();
	  ~RippleADC();
	  void OnPublish() override;
	  void OnAutoConfig() override;
	  void OnPreflight( SignalProperties& Output ) const override;
	  void OnInitialize( const SignalProperties& Output ) override;
	  void OnStartAcquisition() override;
	  void DoAcquire( GenericSignal& Output ) override;
	  void OnStopAcquisition() override;
	  bool Open();
	  bool Close();
	  std::string GetFrontEndName(uint32_t chId);


	 private:
		 float* m_databuffer;
		 uint32_t m_chList[MAX_NUM_CHLS];
		 int32_t m_samplingRate;
		 uint32_t m_timestamp;
		 uint32_t m_newtimestamp;
		 uint32_t m_blocks;
		 static const FrontEndInfo cFrontends[2];
		 uint32_t m_buffersize;

};



#endif // INCLUDED_RIPPLEADC_H
