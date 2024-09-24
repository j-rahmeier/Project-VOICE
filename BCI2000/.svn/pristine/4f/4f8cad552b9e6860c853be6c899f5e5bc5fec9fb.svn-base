////////////////////////////////////////////////////////////////////////////////
// Authors: brunnerlab@3260-al-05006.attlocal.net
// Description: ForagingTask header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_FORAGINGTASK_H  // makes sure this header is not included more than once
#define INCLUDED_FORAGINGTASK_H

#include "ApplicationBase.h"
#include "Shapes.h"
#include "TextField.h"
#include "SoundStimulus.h"
#include <random>
#include <vector>
#include <algorithm>
#include "TextStimulus.h"
#include <map>

using namespace std;

using std::map; using std::string; using std::vector;

class ProgressBarVis;

class ForagingTask : public ApplicationBase
{
public:
	ForagingTask();
	~ForagingTask();
	void Publish() override;
	void Preflight( const SignalProperties& Input, SignalProperties& Output ) const override;
	void Initialize( const SignalProperties& Input, const SignalProperties& Output ) override;
	void StartRun() override;
	void Process( const GenericSignal& Input, GenericSignal& Output ) override;
	void StopRun() override;
	void Halt() override;
	void Reset();

private:
	void UpdateCursorPosition(const GenericSignal& Input);
  void UpdateBoardState();
	double GetAverageState(std::string);
	double GetEyeGaze(bool x, bool average);
	double GetEyeDist();
	double ParameterAngleToNormUnits(string);
	std::vector<int> GetShuffledIndices(int n);
	struct Stimuli {
		SoundStimulus* sound;
		int mean;
    int variation;
	};
  vector<vector<int>> colorsInTrials;
	std::vector<std::vector<SoundStimulus>> soundLocations;
	std::vector<std::vector<Stimuli>> sounds;
  vector<pair<double, double>> orderedStimuli;
  std::map<int, std::pair<int, RGBColor>> mColors;
  int prevLoc;
	double averageEyeDistCm, unitPerCm;
	uint64_t eyeDistCounter;
	//std::vector<RGBColor> mColors;
	TextStimulus* scoreText;
  std::vector<TextStimulus*> boardScores;
  double mScoreOffset, mScoreScale;
	double xDrift, yDrift;
	float xFixPoint, yFixPoint;
	ApplicationWindow& mrDisplay;

	std::default_random_engine rng;

	std::vector<int> board;
	std::vector<int> colorspresented;
	std::vector<int> colorsharvested;
	int m;
	int n;
	float xspace;
	float yspace;

	RectangularShape* cursor;
	float cursorsize;

	std::vector<EllipticShape*> stimuli;
	float radius;
	float maxradius;
	float dwellradius;
	int ncolors;

	float heightfactor;

	int lockedx;
	int lockedy;

	int ctrialtime;
	int trialtime;

	int clockedtime;
	int lockedtime;

	int cfixationtime;
	int fixationtime;
	int fixationholdtime;

	float fixationdwellradius;

	int scoretime;
  std::map<int, double> presentedScores;
	int intertrialtime;

	int ntrials;
	int ctrial;

	EllipticShape* fixation;

	bool usevelocity;

	enum TrialPhase {
		Fixation,
		FixationHold,
		Feedback,
		InterTrial,
		ScoreFeedback
	};

	ProgressBarVis* mpProgressBar = nullptr;

	bool showcursor;
  int boardStateCounter;

};

#endif // INCLUDED_FORAGINGTASK_H
