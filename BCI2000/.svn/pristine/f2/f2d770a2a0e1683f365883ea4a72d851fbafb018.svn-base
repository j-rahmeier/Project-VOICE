#include "StimulusTask.h"
#include "TextStimulus.h"
#include "ImageStimulus.h"

#include <vector>
#include <string>

class wcst : public StimulusTask {
public:
	wcst();
	~wcst();

	void OnInitialize(const SignalProperties&) override;
	void OnPreflight(const SignalProperties&) const override;

	void DoPreRun(const GenericSignal&, bool& doProgress) override;

    int OnNextStimulusCode() override;

	void DoPreSequence(const GenericSignal&, bool& doProgress) override;
	void DoStimulus(const GenericSignal&, bool& doProgress) override;

	void OnStimulusBegin( int stimulusCode ) override;

	void DoPostRun(const GenericSignal&, bool& doProgress) override;

    void OnHalt() override;


private:
	bool CheckKeyPress(std::string, int);

	ApplicationWindow& mrDisplay;

	TextStimulus* text;

	ImageStimulus* fixationCross;

	ImageStimulus* topImage;
	ImageStimulus* rightImage;
	ImageStimulus* bottomImage;
	ImageStimulus* leftImage;

	TextStimulus* correctText;
	TextStimulus* incorrectText;

	std::vector<int> answers;

	std::string location;

	int stim;
	int trials;

	int maxTrials;

	bool sequenced;

	// 1: up
	// 2: right
	// 3: down
	// 4: left

	// 0: Shape
	// 1: Color
	// 2: Pattern
	char ruleChars[3][4] = {
		{ 'C', 'Q', 'S', 'T' },
		{ 'B', 'G', 'M', 'Y' },
		{ 'L', 'P', 'R', 'S' },
	};

	int correctInARow;

	std::string** sequence;

};
