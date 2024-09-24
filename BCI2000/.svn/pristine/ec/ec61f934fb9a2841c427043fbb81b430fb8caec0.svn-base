////////////////////////////////////////////////////////////////////////////////
// Authors: lijintao@56B3
// Description: NeuroSparkADC header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_NEUROSPARK_H  // makes sure this header is not included more than once
#define INCLUDED_NEUROSPARK_H

#include "BufferedADC.h"
#include <vector>

class NeuroSparkADC : public BufferedADC
{
public:
	NeuroSparkADC();
	~NeuroSparkADC();
	void OnPublish() override;
	void OnAutoConfig() override;
	void OnPreflight(SignalProperties& Output) const override;
	void OnInitialize(const SignalProperties& Output) override;
	void OnStartAcquisition() override;
	void DoAcquire(GenericSignal& Output) override;
	void OnStopAcquisition() override;

private:
	std::vector<float> mpBuffer;
	int mNumberOfSignalChannels;
	int mSampleBlockSize;
	int mSampleRate;

	class NeuroRece;
	NeuroRece* receiver;
};

#endif // INCLUDED_NEUROSPARK_H
