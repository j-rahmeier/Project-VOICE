////////////////////////////////////////////////////////////////////////////////
// Authors: schalklab@HR18818.wucon.wustl.edu
// Description: ANTTitration header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_ANTTASK_H  // makes sure this header is not included more than once
#define INCLUDED_ANTTASK_H

#include "ApplicationBase.h"
#include "TextStimulus.h"
#include "ImageStimulus.h"
#include "shapes.h"

using std::map; using std::string; using std::vector;

class ProgressBarVis;

class ANTTitration : public ApplicationBase
{
 public:
  ANTTitration();
  ~ANTTitration();
  void Publish() override;
  void AutoConfig(const SignalProperties& Input) override;
  void Preflight( const SignalProperties& Input, SignalProperties& Output ) const override;
  void Initialize( const SignalProperties& Input, const SignalProperties& Output ) override;
  void StartRun() override;
  void Process( const GenericSignal& Input, GenericSignal& Output ) override;
  void StopRun() override;
  void Halt() override;

 private:
	 enum class Phase
	 {
		 none,
		 titrationIntro,
		 titrationStim,
		 titrationPause,
		 titrationAbort
	 };
	 void Patch(bool active);
	 bool trialAborted();
	 bool checkKeySpace();
	 bool stimulationOccurred();

	 Phase myPhase = Phase::none;
	 unsigned int mBlockSize;
	 
	 //GUI
	 ApplicationWindow& mrDisplay;
	 TextStimulus* introductTxtStimls;
	 TextStimulus* correctTxtStimls;
	 ImageStimulus* mImgStimlsCenter;
	 GUI::Rect	 mImgRectCenter, mIntroTxtRect, mCorTxtRect;
	 ProgressBarVis* mpProgressBar = nullptr;

	 // Photo diode patch
	 struct {
		 Shape* pShape = nullptr;
		 RGBColor activeColor, inactiveColor;
	 } mPhotoDiodePatch;

	 // Access to the Display property.
	 GUI::GraphDisplay& Display()
	 {
		 return mrDisplay;
	 }

	 //stimulation titration
	 uint8_t mTrialNum;
	 bool mStimTriggered;
	 int mNoAmpCounter;
};

#endif // INCLUDED_ANTTASK_H
