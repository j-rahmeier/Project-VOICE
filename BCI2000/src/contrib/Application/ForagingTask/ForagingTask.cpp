////////////////////////////////////////////////////////////////////////////////
// Authors: brunnerlab@3260-al-05006.attlocal.net
// Description: ForagingTask implementation
////////////////////////////////////////////////////////////////////////////////

#include "ForagingTask.h"
#include "BCIStream.h"
#include "ProgressBarVis.h"

#include <cstdlib>
#include <random>
#include <vector>
#include <algorithm>
#include <chrono>
#include <math.h>
#include <iomanip>
#include <fstream>
#include <string>
#include <iostream>
#include <numeric>
//#if _HAS_CXX17
//	#include <filesystem>
//#else
//	#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
//	#include <experimental/filesystem>
//#endif

#define PI 3.14159265
using namespace std;


RegisterFilter( ForagingTask, 3 );

ForagingTask::ForagingTask() :
  mrDisplay( Window() ),
	mpProgressBar(nullptr)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	rng = std::default_random_engine( seed );
	std::srand( seed );
}

ForagingTask::~ForagingTask()
{
	Halt();
	delete mpProgressBar;
}

void
ForagingTask::Publish()
{
	BEGIN_PARAMETER_DEFINITIONS

    //experiment settings
		"Application:Foraging int ForagingColumns= 4 % % % // Number of Columns of Dots",
		"Application:Foraging int ForagingRows= 4 % % % // Number of Rows of Dots",
    "Application:Foraging int Trials= 32 % % % // Number of trials to run",
    "Application:Foraging int PresentImmediateFeedback= 1 1 0 1 // Show score after forage (boolean)",
    "Application:Foraging int PresentTrialFeedback= 0 0 0 1 // Show cumulative trial score after foraging trial (boolean)",
    "Application:Foraging int SimpleScore= 0 0 0 1 // Enable to keep constant score for each target (boolean)",

    //cursor
    "Application:Foraging%20Cursor int EyetrackerCursor= 0 0 0 1 // Enable to use eyetracker, otherwise use expression filter (boolean)",
    "Application:Foraging%20Cursor int ShowCursor= 1 1 0 1 // Show the participant the cursor (boolean)",
    "Application:Foraging%20Cursor string CursorColor= 0xDDDDDD % % % // Color of the cursor (color)",
    "Application:Foraging%20Cursor int CursorSize= 5 % % % // Size of the cursor",
    "Application:Foraging%20Cursor int UseVelocity= 0 % % % // Whether to control the cursor by adding velocity to it or setting its position (boolean)",

    //dot size
		"Application:Foraging%20Dot%20Sizes int MinDotAngle= 0.5 1 0 180 // Normal visual angle of the radius of each dot",
		"Application:Foraging%20Dot%20Sizes int MaxDotAngle= 2 2 0 180 // Maximum visual angle of the radius of each dot when increasing upon dwelling",
		"Application:Foraging%20Dot%20Sizes float DwellVisualAngle= 5 5 0 180 // The degrees of visual angle that triggers dwell",
    "Application:Foraging%20Dot%20Sizes int FixationDotAngle= 1 1 0 180 // Visual angle of fixation dot",
    "Application:Foraging%20Dot%20Sizes float FixationDwellVisualAngle= 5 5 0 180 // The degrees of visual angle for fixation dot to be held",
    "Application:Foraging%20Dot%20Sizes float ViewingDistanceInCm= 40 40 0 %  // Only used for mouse or joystick mode! The distance of the screen from the eyes(cm)",
    "Application:Foraging%20Dot%20Sizes float WindowDiagonalInCm= 22 22 0 %  // Diagonal distance of the application window(cm)",

    //stimuli
		"Application:Foraging%20Stimuli string ForagingStimuliDirectory= ../tasks/berkeley_foraging_task_2D/wav_RT_2ms // Folder that audio stimuli are held (directory)",
		"Application:Foraging%20Stimuli stringmatrix ForagingStimuliDistribution= { Mean StdDev } 4 20 40 50 60 2 2 2 2 % % % // Dots color and distribution of sounds (color)",
		"Application:Foraging%20Stimuli stringmatrix ForagingStimuliColors= 4 {Name Value} Brown 0xaa5500 Green 0x44ddaf Pink 0xeaa0ff Yellow 0xfffd79 % % % // Available stimuli colors: name and value (color)",
		"Application:Foraging%20Stimuli int ColorsInATrial= 3 3 1 % // Number of colors to show per trial, randomly if subset (color)",
    "Application:Foraging%20Stimuli int MaximumColorCanMiss= 2 2 1 % // Maximum number of trials that a color can miss in a row",
    "Application:Foraging%20Stimuli int MinimumAudioClicks= 5 0 0 % // Minimum available audio clicks",
    "Application:Foraging%20Stimuli int MaximumAudioClicks= 75 1 1 % // Maximum available audio clicks",
		
    //time settings
    "Application:Foraging%20Timing int ForagingTrialTime= 15s % % % // Length of Time that Subject has to forage",
		"Application:Foraging%20Timing int DwellTime= 1s % % % // Length of Time that Subject has to dwell on each dot",
		"Application:Foraging%20Timing int InterTrialTime= 1500ms % % % // Length of time between trials",
		"Application:Foraging%20Timing int ScoreFeedbackTime= 1000ms % % % // Length of time to show user score",
    "Application:Foraging%20Timing int FixationTime= 2s % % % // Time needed to fixate between trials",
    "Application:Foraging%20Timing int FixationHoldTime= 500ms % % % // Time to hold fixation before starting trial",

		//progress bar
		"Application:ProgressBar int ProgressBar= 1 1 0 1 // Display progress bar (boolean)",
		"Application:ProgressBar float ProgressBarHeight= 50 50 0 % // Progress bar height in pixels",
		"Application:ProgressBar float ProgressBarWidth= 250 250 0 % // Progress bar width in pixels",
		"Application:ProgressBar string ProgressBarBackgroundColor= 0x00808080 0x00808080 0x00000000 0xFFFFFFFF // Color of progress bar background (color)",
		"Application:ProgressBar string ProgressBarForegroundColor= 0x00ffff00 0x00f0f0f0 0x00000000 0xFFFFFFFF // Color of progress bar foreground (color)",

		//photodiode
		"Application:PhotoDiodePatch int PhotoDiodePatch= 1 1 0 1 // Display photo diode patch (boolean)",
		"Application:PhotoDiodePatch float PhotoDiodePatchHeight= 0.065 1 0 1 // Photo diode patch height in relative coordinates",
		"Application:PhotoDiodePatch float PhotoDiodePatchWidth= 0.05 1 0 1 // Photo diode patch width in relative coordinates",
		"Application:PhotoDiodePatch float PhotoDiodePatchLeft= 0 1 0 1 // Photo diode patch left in relative coordinates",
		"Application:PhotoDiodePatch float PhotoDiodePatchTop= 0.935 1 0 1 // Photo diode patch top in relative coordinates",
		"Application:PhotoDiodePatch int PhotoDiodePatchShape= 0 1 0 1 // Photo diode patch shape: 0 rectangle, 1 ellipse (enumeration)",
		"Application:PhotoDiodePatch int PhotoDiodePatchActiveColor= 0x000000 0 0 0xffffffff // Photo diode patch color when active (color)",
		"Application:PhotoDiodePatch int PhotoDiodePatchInactiveColor= 0xffffff 0 0 0xffffffff // Photo diode patch color when inactive, use 0xff000000 for transparent (color)",
	END_PARAMETER_DEFINITIONS

	BEGIN_STATE_DEFINITIONS
		"CursorPosX 32 0 0 0",
		"CursorPosY 32 0 0 0",
		"TargetX    8  0 0 0",
		"TargetY    8  0 0 0",
		"TargetType 8  0 0 0",
		"Score      16  0 0 0",
		"TrialPhase 8  0 0 0",
		"Trial      8  0 0 0",
		"Dwelling	  8  0 0 0",
		"DotExists	32 0 0 0",
    "AudioType  8 0 0 0",
    "AudioVariation 8 0 0 0",
    "BoardLayout 8 0 0 0"
	END_STATE_DEFINITIONS

}

void
ForagingTask::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
	Parameter( "ForagingColumns" );
	Parameter( "ForagingRows" );
  Parameter("PresentImmediateFeedback");
  Parameter("PresentTrialFeedback");
  Parameter("SimpleScore");
	Parameter( "MinDotAngle" );
	Parameter( "MaxDotAngle" );
	Parameter("DwellVisualAngle");
	Parameter( "ForagingTrialTime" );
	Parameter( "DwellTime" );
	Parameter( "InterTrialTime" );
	Parameter("ScoreFeedbackTime");
  Parameter("ForagingStimuliDirectory");

	Parameter( "FixationTime" );
	Parameter( "FixationHoldTime" );
	Parameter( "FixationDotAngle" );
	Parameter("FixationDwellVisualAngle");

	Parameter("ShowCursor");
	Parameter( "CursorColor" );
	Parameter( "CursorSize" );
	Parameter( "UseVelocity" );

	Parameter( "WindowWidth" );
	Parameter( "WindowHeight" );
	Parameter( "WindowTop" );
	Parameter( "WindowLeft" );
  Parameter( "ViewingDistanceInCm" );
  Parameter( "WindowDiagonalInCm" );

	State( "CursorPosX" );
	State( "CursorPosY" );
	State( "TargetX" );
	State( "TargetY" );
	State( "TargetType" );
	State( "Score" );
	State( "TrialPhase" );
	State( "Trial" );
	State("DotExists");
	State("Dwelling");
  State("AudioType");
  State("AudioVariation");

	State( "Running" );
	Parameter("SampleBlockSize");
	Parameter("SamplingRate");
	int EyetrackerCursor = Parameter("EyetrackerCursor");
  if (Parameter("EyetrackerCursor") == 1) {
    //eyetracker
    Parameter("GazeResolution");
		Parameter("EyetrackerLegacyCorrectionOffset");
    Parameter("EyetrackerLegacyCorrectionFactor");
    State("EyetrackerLeftEyeGazeX");
    State("EyetrackerRightEyeGazeX");
    State("EyetrackerLeftEyeGazeY");
    State("EyetrackerRightEyeGazeY");
    State("EyetrackerLeftEyeValidity");
    State("EyetrackerRightEyeValidity");
    State("EyetrackerLeftEyeDist");
    State("EyetrackerRightEyeDist");
  }

  //preflight sanity checks
	if (Parameter("ColorsInATrial") > Parameter("ForagingStimuliColors")->NumRows())
		bcierr << "ForagingTask: Parameter ColorsInATrial must be less than or equal to the number of colors in ForagingStimuliColors" << endl;

  PreflightCondition(Parameter("ForagingStimuliDistribution")->NumColumns() == Parameter("ForagingStimuliColors")->NumRows());

	PreflightCondition(Parameter("ForagingStimuliDistribution")->NumRows() == 2);

  double reqFreq = ceil((double)Parameter("Trials") / (double)Parameter("ForagingStimuliDistribution")->NumColumns() * Parameter("ColorsInATrial"));
  int minMiss = ceil((double)Parameter("Trials") / reqFreq);
  PreflightCondition(Parameter("MaximumColorCanMiss") >= minMiss);

  PreflightCondition(Parameter("MaximumAudioClicks").ToNumber() > Parameter("MinimumAudioClicks").ToNumber());

	Output = Input; // this simply passes information through about SampleBlock dimensions, etc....

}

vector<int> 
ForagingTask::GetShuffledIndices(int n)
{
	std::vector<int> iSoundsRandom(n); // vector with 10 ints.
	std::iota(std::begin(iSoundsRandom), std::end(iSoundsRandom), 0); // Fill with 0, 1, ..., 9.
	//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); // obtain a time-based seed:
	std::shuffle(iSoundsRandom.begin(), iSoundsRandom.end(), rng); // shuffle vector
	return iSoundsRandom;
}

void
ForagingTask::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
	if ( stimuli.size() ) {
		for ( auto s : stimuli )
			delete s;
		sounds.clear();

		delete fixation;
		delete cursor;
		delete scoreText;
    for (auto t : boardScores)
      delete t;
    boardScores.clear();
	}
	//tedious math, but necessary for variable screen resolutions and sizes
	double wDiagPx = sqrt(pow(Parameter("WindowWidth").ToNumber(), 2) + pow(Parameter("WindowHeight").ToNumber(), 2));
	unitPerCm = wDiagPx / Parameter("WindowDiagonalInCm").ToNumber() / Parameter("WindowWidth").ToNumber();
	averageEyeDistCm = Parameter("ViewingDistanceInCm").ToNumber();

	//Scaling of dots based on visual angle
  dwellradius = ParameterAngleToNormUnits("DwellVisualAngle");
  fixationdwellradius = ParameterAngleToNormUnits("FixationDwellVisualAngle");
  radius = ParameterAngleToNormUnits("MinDotAngle");
  maxradius = ParameterAngleToNormUnits("MaxDotAngle");
	float r = ParameterAngleToNormUnits("FixationDotAngle");

  //experiment set up
	m = Parameter( "ForagingColumns" );
	n = Parameter( "ForagingRows" );
  ntrials = Parameter("Trials");
  State("Trial") = 1;

  //time vars
	trialtime = Parameter( "ForagingTrialTime" ).InSampleBlocks();
	lockedtime = Parameter( "DwellTime" ).InSampleBlocks();
	intertrialtime = Parameter( "InterTrialTime" ).InSampleBlocks();
	scoretime = Parameter("ScoreFeedbackTime").InSampleBlocks();
  fixationtime = Parameter("FixationTime").InSampleBlocks();
  fixationholdtime = Parameter("FixationHoldTime").InSampleBlocks();
  ctrialtime = trialtime + 1;
  clockedtime = 0;
  cfixationtime = 0;

  //create dots
	stimuli = std::vector<EllipticShape*>( n * m );
  if (Parameter("PresentImmediateFeedback"))
    boardScores = vector<TextStimulus*>(n * m);
  xspace = 1.f / m;
	yspace = 1.f / n;
  heightfactor = Parameter( "WindowWidth" ) / Parameter( "WindowHeight" );
	for ( int i = 0; i < m; i++ ) {
		for ( int p = 0; p < n; p++ ) {
      //stimulus
      int v = i + p * m;
			stimuli[ v ] = new EllipticShape( mrDisplay, 2 );
			float x = ( i + 0.5f ) * xspace;
			float y = ( p + 0.5f ) * yspace;
			stimuli[ v ]->SetObjectRect( { x - radius, y - radius * heightfactor, x + radius, y + radius * heightfactor } );
			stimuli[ v ]->Hide();

      //score feedback
      if (Parameter("PresentImmediateFeedback"))
      {
        boardScores[v] = new TextStimulus(mrDisplay);
        boardScores[v]->SetText("");
        boardScores[v]->SetTextHeight(0.50f);//text size, adaptive with the window size
        boardScores[v]->SetObjectRect({ x - dwellradius, y - dwellradius * heightfactor, x + dwellradius, y + dwellradius * heightfactor });
        boardScores[v]->SetTextColor(RGBColor::White);
        boardScores[v]->Conceal();
      }

		}
	}
  ncolors = Parameter("ForagingStimuliDistribution")->NumColumns();

  //create fixation dot
  fixation = new EllipticShape(mrDisplay, 2);
  fixation->SetObjectRect({ 0.5f - r, 0.5f - r * heightfactor, 0.5f + r, 0.5f + r * heightfactor });
  fixation->SetFillColor(RGBColor::White);
  fixation->SetColor(RGBColor::White);

  //create cursor
  showcursor = Parameter("ShowCursor");
	if (showcursor)
	{
		cursorsize = Parameter("CursorSize") / Parameter("WindowWidth");
		cursor = new RectangularShape(mrDisplay, 1);
		cursor->SetColor(RGBColor(Parameter("CursorColor")));
		cursor->SetFillColor(RGBColor(Parameter("CursorColor")));
		cursor->SetObjectRect({ 0.5f - cursorsize, 0.5f - cursorsize * heightfactor, 0.5f + cursorsize, 0.5f + cursorsize * heightfactor });
	}
  usevelocity = Parameter("UseVelocity");

  //create sound stimuli
	int suf = 0;
	int nSounds = Parameter("MaximumAudioClicks").ToNumber() - Parameter("MinimumAudioClicks")+1; //num of different sounds
	int nSoundTypes = 10; //num of variation in directory for each sound
	std::vector<std::vector<int>> soundIndices;
	for (int i = 0; i < ncolors; i++)
		soundIndices.push_back(GetShuffledIndices(nSoundTypes));

  //store locations and create pointers
	soundLocations.clear();
	SoundStimulus nullS;
	std::vector<SoundStimulus> soundTypes(nSoundTypes, nullS);
	vector<vector<SoundStimulus>> newSoundLocations(nSounds, soundTypes);
	soundLocations = newSoundLocations;

  //order stimuli in increasing mean blips
  orderedStimuli.clear();
  for (int i = 0; i < Parameter("ForagingStimuliDistribution")->NumColumns(); i++)
  {
    orderedStimuli.push_back({ Parameter("ForagingStimuliDistribution")(0, i), Parameter("ForagingStimuliDistribution")(1, i) });
  }
  sort(orderedStimuli.begin(), orderedStimuli.end(), [](pair<double, double> a, pair<double, double> b) { return a.first < b.first; });

  int maxMean = 0;
  int minMean = 1000;
	for (int i = 0; i < Parameter("ForagingStimuliDistribution")->NumColumns(); i++)
	{
		//normal dist parameters
		default_random_engine generator;
		normal_distribution<double> distribution(orderedStimuli[i].first, orderedStimuli[i].second);

		//generate normal dist for entire run
		vector<Stimuli> empV;
		sounds.push_back(empV);
		for (int t = 0; t < ntrials; t++)
		{
			//prepare for maximum amount of dots
			int dotsPerTrial = ceil((double)(m * n) / (double)ncolors);
			for (int d = 0; d < dotsPerTrial; d++)
			{
				double number = distribution(generator);
				int numPulses = int(number); 
        if (numPulses > Parameter("MaximumAudioClicks").ToNumber())
        {
          bciwarn << "ForagingTask: Audio file with " << numPulses << " pulses is not available. Changing to " << Parameter("MaximumAudioClicks").ToNumber() << endl;
          numPulses = Parameter("MaximumAudioClicks");
        }
        else if (numPulses < Parameter("MinimumAudioClicks").ToNumber())
        {
          bciwarn << "ForagingTask: Audio file with " << numPulses << " pulses is not available. Changing to " << Parameter("MinimumAudioClicks").ToNumber() << endl;
          numPulses = Parameter("MinimumAudioClicks");
        }

				//get new shuffled array if we have gone thru all sound types
				if (soundIndices[i].empty())
					soundIndices[i] = GetShuffledIndices(nSoundTypes);

				//access then remove so we don't access again until we use them all
				int suffix = soundIndices[i].back();
				soundIndices[i].pop_back(); 

        //get audio file
        string nStr = "";
        if (numPulses < 10)
          nStr = "0" + to_string(numPulses);
        else
          nStr = to_string(numPulses);
        string fName = nStr + "-" + to_string(suffix) + ".wav";
        string fPath = Parameter("ForagingStimuliDirectory").ToString() + "/" + fName;
        ifstream f(fPath);
        if (f.good())
        {
          //file exists
          SoundStimulus s;
          s.SetFile(fPath);
          if (s.ErrorState() == WavePlayer::noError)
          {
            soundLocations[numPulses - Parameter("MinimumAudioClicks")][suffix] = s;
          }
          minMean = min(minMean, numPulses);
          maxMean = max(maxMean, numPulses);
          sounds[i].push_back({ &soundLocations[numPulses - Parameter("MinimumAudioClicks")][suffix], numPulses, suffix });
        }
				else {
					bcierr << "ForagingTask: File " << fName << " does not exist in " << Parameter("ForagingStimuliDirectory") << endl;
				}
			}
		}
		if (any_of(sounds.begin(), sounds.end(), [](vector<Stimuli> v) {return v.empty(); }))
			bcierr << "Change the Parameter \"ForagingStimuliDirectory\" to where your Foraging audio files are located" << endl;
	}
  //scaling factor for points
  mScoreOffset = -minMean;
  mScoreScale = 100.0/(maxMean - minMean);

	//init color array, will be randomized each run
	mColors.clear();
  vector<int> mColorIndices(ncolors);
  std::iota(mColorIndices.begin(), mColorIndices.end(), 0); // Fill with 0, 1, ..., ncolors
  std::shuffle(mColorIndices.begin(), mColorIndices.end(), rng);
  //use pair to access ForagingStimuliColors, use map index for everything else
	for (int i = 0; i < Parameter("ForagingStimuliColors")->NumRows(); i++)
	{
    mColors[i] = std::pair<int, RGBColor>( mColorIndices[i], RGBColor(Parameter("ForagingStimuliColors")(mColorIndices[i], 1)) ); //mix up color with reward
	}
  colorspresented = std::vector<int>(ncolors);
  colorsharvested = std::vector<int>(ncolors);

  //create color presentation order
  int subNColors = Parameter("ColorsInATrial");
  colorsInTrials.clear();
  int nCmissed = ncolors - subNColors;
  map<int, int> colorFrequency;
  //init array
  for (int i = 0; i < ntrials; i++)
  {
    std::shuffle(mColorIndices.begin(), mColorIndices.end(), rng);
    for (auto it = mColorIndices.begin(); it < mColorIndices.end() - nCmissed; it++)
      colorFrequency[*it]++;
    colorsInTrials.push_back(vector<int>(mColorIndices.begin(), mColorIndices.end() - nCmissed));
  }
  //guarentee we have equal number of colors
  int reqFreq = floor((double)ntrials / (double)ncolors * subNColors);
  struct ColorFreqency
  {
    const int nX;
    ColorFreqency(int n) : nX(n) {}
    bool operator()(pair<const int, int>(n)) const { return n.second < nX; }
  };

  //require each color be displayed the same number of times (as much as possible)
  while (any_of(colorFrequency.begin(), colorFrequency.end(), ColorFreqency(reqFreq)))
  {
    for (int c = 0; c < ncolors; c++)
    {
      if (colorFrequency[c] < reqFreq)
      {
        std::shuffle(colorsInTrials.begin(), colorsInTrials.end(), rng); //shuffle so we don't keep replacing same ones
        //find trial color is not in, and switch it with random color
        for (auto trialColors = colorsInTrials.begin(); trialColors < colorsInTrials.end(); trialColors++)
        {
          if (find(trialColors->begin(), trialColors->end(), c) == trialColors->end())
          {
            //color is not present
            int maxI = 0;
            int swapIndex = 0;
            //replace with most frequent color
            for (auto it = trialColors->begin(); it != trialColors->end(); it++)
            {
              if (colorFrequency[*it] > maxI) {
                maxI = colorFrequency[*it];
                swapIndex = distance(trialColors->begin(), it);
              }
            }
            int oldC = trialColors->at(swapIndex);
            trialColors->at(swapIndex) = c;
            colorFrequency[oldC]--;
            colorFrequency[c]++;
            if (colorFrequency[c] < reqFreq)
              continue;
            else
              break;
          }
        }
      }
    }
  }
  //require each color only miss a certain number in a row
  struct ColorsMissed
  {
    const int nX;
    ColorsMissed(int n) : nX(n) {}
    bool operator()(int n) const { return n > nX; }
  };
  vector<int> maxMissed;
  do {
    //brute force, just shuffle until requirement is met
    std::shuffle(colorsInTrials.begin(), colorsInTrials.end(), rng);
    map<int, int> missed;
    maxMissed = vector<int>(ncolors, 0);
    for (auto trialColors = colorsInTrials.begin(); trialColors < colorsInTrials.end(); trialColors++)
    {
      for (int c = 0; c < ncolors; c++)
      {
        if (find(trialColors->begin(), trialColors->end(), c) != trialColors->end())
        {
          maxMissed[c] = max(maxMissed[c], missed[c]);
          missed[c] = 0;
        }
        else
          missed[c]++;
      }
    }
  } while (any_of(maxMissed.begin(), maxMissed.end(), ColorsMissed(Parameter("MaximumColorCanMiss").ToNumber())));


  //text for scoring in between trials
	GUI::Rect mIntroTxtRect;
	mIntroTxtRect.left = 0.2f;
	mIntroTxtRect.right = 0.8f;
	mIntroTxtRect.top = 0.3f;
	mIntroTxtRect.bottom = 0.7f;
	scoreText = new TextStimulus(mrDisplay);
	scoreText->SetText("");
	scoreText->SetTextHeight(0.15f);//text size, adaptive with the window size
    scoreText->SetObjectRect(mIntroTxtRect);
	scoreText->SetTextColor(RGBColor::White);
    scoreText->SetScalingMode(GUI::ScalingMode::AdjustWidth);
	scoreText->Conceal();

	//progress bar
	if (mpProgressBar)
	{
		mpProgressBar->Send(CfgID::Visible, false);
	}
	delete mpProgressBar;
	mpProgressBar = nullptr;
	if (Parameter("ProgressBar") != 0)
	{
		mpProgressBar = new ProgressBarVis();
		mpProgressBar->SetBackgroundColor(RGBColor(Parameter("ProgressBarBackgroundColor")));
		mpProgressBar->SetForegroundColor(RGBColor(Parameter("ProgressBarForegroundColor")));
		mpProgressBar->SetHeight(Parameter("ProgressBarHeight"));
		mpProgressBar->SetWidth(Parameter("ProgressBarWidth"));

		mpProgressBar->Send(CfgID::WindowTitle, "Progress");
		mpProgressBar->Send(CfgID::Visible, true);
		mpProgressBar->SendReferenceFrame();
	}
}

void
ForagingTask::StartRun()
{
	State( "Score" ) = 0;
	fixation->Hide();
	if (Parameter("ShowCursor"))
		cursor->Show();

	for ( int i = 0; i < n * m; i++ ) {
		stimuli[ i ]->Hide();
	}

	ctrialtime = trialtime + 1;
	clockedtime = 0;

	cfixationtime = 0;

	State( "Trial" ) = 1;

	State( "TrialPhase" ) = Fixation;

	xDrift = 0;
	yDrift = 0;
	xFixPoint = 0;
	yFixPoint = 0;
  prevLoc = 0;
	eyeDistCounter = 0;
  boardStateCounter = m*n;
	State("DotExists") = pow(2, (m * n)); //bit per element
  presentedScores.clear();

	//progress bar
	if (mpProgressBar)
	{
		mpProgressBar->SetTotal(ntrials).SetCurrent(0);
		mpProgressBar->SendDifferenceFrame();
	}

  //AppLog!
  AppLog << "============================" << endl;
  AppLog << "Color\tReward\tMean of sound blips\n";
  for (auto it = mColors.begin(); it != mColors.end(); ++it)
  {
    AppLog << Parameter("ForagingStimuliColors")(it->second.first, 0) << "\t" << it->first + 1 << "\t" << orderedStimuli[it->first].first << endl;
  }
  AppLog << "\nNumber of Trials: " << ntrials << endl;
  AppLog << "============================" << endl;
  AppLog << "Trial number\t| Score\t"; //to match spacing of further rows
  auto labs = Parameter("ForagingStimuliColors")->ColumnLabels();
  for (auto it = mColors.begin(); it != mColors.end(); ++it)
  {
    AppLog << "|   " << Parameter("ForagingStimuliColors")(it->second.first, 0) << "\t";// << setw(2) << "|";
  }
  AppLog << endl;
}

void
ForagingTask::Process( const GenericSignal& Input, GenericSignal& Output )
{
	UpdateCursorPosition( Input );
  UpdateBoardState();

	if (Parameter("ShowCursor"))
	{
		cursor->SetObjectRect({ State("CursorPosX") / 1000.f - cursorsize, State("CursorPosY") / 1000.f - cursorsize * heightfactor, State("CursorPosX") / 1000.f + cursorsize, State("CursorPosY") / 1000.f + cursorsize * heightfactor });
		cursor->Show();
	}

	// Check if Feedback Phase is over
	if ( State( "TrialPhase" ) == Feedback ) {
		if ( ctrialtime >= trialtime ) {
			State( "TrialPhase" ) = InterTrial;

			float dotx = ( State( "TargetX" ) + 0.5f ) * xspace;
			float doty = ( State( "TargetY" ) + 0.5f ) * yspace;
			stimuli[ State( "TargetX" ) + State( "TargetY" ) * m ]->SetObjectRect( { dotx - radius, doty - radius * heightfactor, dotx + radius, doty + heightfactor * radius } );
			stimuli[ State( "TargetX" ) + State( "TargetY" ) * m ]->Change();

			ctrialtime = 0;
		}
	}

	// Do Inter-Trial Phase
	if ( State( "TrialPhase" ) == InterTrial ) {
    //clear board
		for ( int i = 0; i < n * m; i++ ) {
			stimuli[ i ]->Hide();
      if (Parameter("PresentImmediateFeedback"))
      {
        boardScores[i]->Conceal();
        presentedScores.clear();
      }
		}

		if ( ctrialtime > intertrialtime ) {
			
			if (mpProgressBar)
			{
				Assert(mpProgressBar->Current() < mpProgressBar->Total());
				int current = mpProgressBar->Current();
				mpProgressBar->SetCurrent(current + 1);
				mpProgressBar->SendDifferenceFrame();
			}

      char buf1[256];
      char prePattern[] = " #%4s/%4i\t| %3s\t";
      sprintf(buf1, prePattern, to_string(State("Trial")).c_str(), ntrials, to_string(State("Score")).c_str());
      AppLog << buf1;
      char buf[256];
      char pattern[] = "| %7.0f%%\t";
      char strPattern[] = "| %7s  \t"; //make equivalent spaces to above pattern
      //sprintf(buf, pattern, "Bob", "Doe", 10.96, 7.61, 14.39, 2.11, 47.30, 14.21, 44.58, 5.00, 60.23);
      int count = 0;
      for (int i = 0; i < ncolors; i++)
      {
        if (colorspresented[i] == 0) {
          sprintf(buf, strPattern, "N/A");
          //count--;
        }
        else {
          sprintf(buf, pattern, (double)colorsharvested[i] / (double)colorspresented[i] * 100.0);
          count++;
        }
        AppLog << buf;
      } 
			AppLog << endl;

      //show score at end of trial, or go to fixation
      if (Parameter("PresentTrialFeedback"))
      {
        State("TrialPhase") = ScoreFeedback;
        scoreText->SetText(to_string(State("Score")));
        scoreText->Present();
      }
      else {
        State("TrialPhase") = Fixation;
      }

			if ( State( "Trial" ) >= ntrials ) {
				State( "Running" ) = 0;
			} else {
				State( "Trial" ) = State( "Trial" ) + 1;
			}
		}

		ctrialtime++;
	}
	//Present user with last round's score after Intertrial phase
	if (State("TrialPhase") == ScoreFeedback) {
		ctrialtime++;
		if (ctrialtime > scoretime) {
			scoreText->SetText("");
			State("TrialPhase") = Fixation;
			fixation->Change();
			ctrialtime = 0;
		}
	}

	// Check if Fixation Phase is Over
	if ( State( "TrialPhase" ) == Fixation ) {
		for ( int i = 0; i < n * m; i++ ) {
			stimuli[ i ]->Hide();
		}
		State("DotExists") = pow(2, (m * n));

		if ( cfixationtime >= fixationtime ) {
			Reset();
			State( "TrialPhase" ) = FixationHold;
			cfixationtime = 0;
			xDrift = xFixPoint / (double)fixationtime;
			yDrift = yFixPoint / (double)fixationtime;
			xFixPoint = 0;
			yFixPoint = 0;
		}
	}

	// Check if Fixation Hold Phase is Over
	if ( State( "TrialPhase" ) == FixationHold ) {
		if ( cfixationtime >= fixationholdtime ) {
			fixation->Hide();
			State( "TrialPhase" ) = Feedback;
		}
	}

	// Do Fixation and Fixation Hold Phases
	if ( State( "TrialPhase" ) == Fixation || State( "TrialPhase" ) == FixationHold ) {
		fixation->Show();
		float x = ( float )State( "CursorPosX" ) / 1000.f;
		float y = ( float )State( "CursorPosY" ) / 1000.f;

		float dotx = 0.5f;
		float doty = 0.5f;

		float d = sqrt( ( ( dotx - x ) * ( dotx - x ) ) + ( ( ( doty - y ) / heightfactor ) * ( ( doty - y ) / heightfactor ) ) );

		if ( d < fixationdwellradius ) {
			cfixationtime++;
			if (Parameter("EyetrackerCursor") == 1)
			{
				bool leftValid = State("EyetrackerLeftEyeValidity") == 0;
				bool rightValid = State("EyetrackerRightEyeValidity") == 0;
				if (leftValid || rightValid) {
					xFixPoint += GetEyeGaze(true, false) - 0.5f;
					yFixPoint += GetEyeGaze(false, false) - 0.5f;
				}
			}
		} 
		else {
			cfixationtime = 0;
			xFixPoint = 0;
			yFixPoint = 0;
		}
		return;
	}

	// Do Feedback Phase
	if ( State( "TrialPhase" ) == Feedback ) {
		ctrialtime++;

		if ( clockedtime > lockedtime ) {
      //foraged!
			int i = board[ lockedx + lockedy * m ];
      sounds[i].back().sound->Present();
      int score;
      if (Parameter("SimpleScore"))
        score = i + 1;
      else
        //scale between 0 - 100, following sound distribution
        score = round((sounds[i].back().mean + mScoreOffset) * mScoreScale);
			State( "Score" ) = State( "Score" ) + score;
      State("AudioType") = sounds[i].back().mean;
      State("AudioVariation") = sounds[ i ].back().variation;
			sounds[i].pop_back();

      //grey out foraged dot
			board[ lockedx + lockedy * m ] = ncolors;
      if (Parameter("PresentImmediateFeedback"))
      {
        stimuli[lockedx + lockedy * m]->Hide();
        //present score
        boardScores[lockedx + lockedy * m]->SetText(to_string(score));
        boardScores[lockedx + lockedy * m]->SetTextColor(RGBColor::White);
        boardScores[lockedx + lockedy * m]->Present();
        presentedScores.insert({ lockedx + lockedy * m, 1.0 });
      }
      else 
      {
        stimuli[ lockedx + lockedy * m ]->SetFillColor( RGBColor::Gray );
        stimuli[ lockedx + lockedy * m ]->SetColor( RGBColor::Gray );
      }
			clockedtime = 0;

			//update state
			int c = State("TargetX") + State("TargetY") * m;
			State("DotExists") = State("DotExists") - (1 << c); //set relevant bit to 0
			colorsharvested[State("TargetType")]++;
		}

		float x = ( float )State( "CursorPosX" ) / 1000.f;
		float y = ( float )State( "CursorPosY" ) / 1000.f;

		int mloc = floor( m * x );
		int nloc = floor( n * y );
		
		// Reset Radius of Previous Dot
		if ( mloc != State( "TargetX" ) || nloc != State( "TargetY" ) ) {
			float dotx = ( State( "TargetX" ) + 0.5f ) * xspace;
			float doty = ( State( "TargetY" ) + 0.5f ) * yspace;
			stimuli[ State( "TargetX" ) + State( "TargetY" ) * m ]->SetObjectRect( { dotx - radius, doty - radius * heightfactor, dotx + radius, doty + heightfactor * radius } );
			stimuli[ State( "TargetX" ) + State( "TargetY" ) * m ]->Change();

		}

		State( "TargetX" ) = mloc;
		State( "TargetY" ) = nloc;
		State( "TargetType" ) = board[ mloc + nloc * m ];

		float dotx = ( mloc + 0.5f ) * xspace;
		float doty = ( nloc + 0.5f ) * yspace;

		float d = sqrt( ( ( dotx - x ) * ( dotx - x ) ) + ( ( ( doty - y ) / heightfactor ) * ( ( doty - y ) / heightfactor ) ) );

		if ( d < dwellradius && board[ mloc + nloc * m ] != ncolors ) {
      if (prevLoc != mloc + nloc * m)
        clockedtime = 0;
			clockedtime++;
			lockedx = mloc;
			lockedy = nloc;
      prevLoc = mloc + nloc * m;

			float r = radius + ( ( maxradius - radius ) * ( ( float )clockedtime / ( float )lockedtime ) );

			stimuli[ mloc + nloc * m ]->SetObjectRect( { dotx - r, doty - r * heightfactor, dotx + r, doty + r * heightfactor } );
			stimuli[ mloc + nloc * m ]->Change();
		} else {
			clockedtime = 0;
			stimuli[ mloc + nloc * m ]->SetObjectRect( { dotx - radius, doty - radius * heightfactor, dotx + radius, doty + heightfactor * radius } );
			stimuli[ mloc + nloc * m ]->Change();
		}

    //dim score feedback
    for (auto it = presentedScores.cbegin(); it != presentedScores.cend();)
    {
      if (it->second <= 0)
      {
        boardScores[it->first]->Conceal();
        presentedScores.erase(it++);
      }
      else
      {
        boardScores[it->first]->SetTextColor(RGBColor().FromHSV(0, 0, it->second));
        //boardScores[it->first]->SetDimFactor(it->second);
        presentedScores[it->first] = it->second - 1.0 / scoretime;
        it++;
      }
    }
	}

	Output = Input; // This passes the signal through unmodified.
	State("Dwelling") = clockedtime;
}

void
ForagingTask::StopRun()
{
	bciout << "Stopping Run";
}

void
ForagingTask::Halt()
{
}

double
ForagingTask::ParameterAngleToNormUnits(string p) {
	double unitDist = averageEyeDistCm * unitPerCm;
	double angRad = Parameter(p).ToNumber() / 2 * PI / 180; //we want radius, so 1/2
	double angNorm = unitDist * tan(angRad);

	//returns in units 0-1
	return angNorm;
}
double
ForagingTask::GetAverageState(string pName)
{
	double sum = 0;
	for (int i = 0; i < Statevector->Samples(); ++i)
	{
		sum += State(pName)(i);
	}
	return sum / Statevector->Samples();
}
double
ForagingTask::GetEyeGaze(bool x, bool drift)
{
	bool leftValid = State("EyetrackerLeftEyeValidity") == 0;
	bool rightValid = State("EyetrackerRightEyeValidity") == 0;
	string dir = x ? "X" : "Y";
	double st = (leftValid * GetAverageState("EyetrackerLeftEyeGaze" + dir) + rightValid * GetAverageState("EyetrackerRightEyeGaze" + dir));
	double normSt = (st / (double)(leftValid + rightValid) - Parameter("EyetrackerLegacyCorrectionOffset")(x ? "x" : "y"))
									* Parameter("EyetrackerLegacyCorrectionFactor")(x ? "x" : "y") / 65535.0 ;
	if (drift)
		normSt -= x ? xDrift : yDrift;
	return normSt;
}
double
ForagingTask::GetEyeDist()
{
	bool leftValid = State("EyetrackerLeftEyeValidity") == 0;
	bool rightValid = State("EyetrackerRightEyeValidity") == 0;

	double st = (leftValid * GetAverageState("EyetrackerLeftEyeDist") + rightValid * GetAverageState("EyetrackerRightEyeDist"));
	double distCm = (st / (double)(leftValid + rightValid) - Parameter("EyetrackerLegacyCorrectionOffset")("z")) 
									* Parameter("EyetrackerLegacyCorrectionFactor")("z") / 10; //mm to cm

	return distCm;
}

void ForagingTask::UpdateCursorPosition( const GenericSignal& Input ) {
	float x, y;
	int type = (int)Parameter("EyetrackerCursor");
  if (Parameter("EyetrackerCursor") == 1)
  {
    //if invalid, hold position
    if (!(State("EyetrackerLeftEyeValidity") == 0) && !(State("EyetrackerRightEyeValidity") == 0))
			return;

		//at least one eye is valid
		x = GetEyeGaze(true, true);
		y = GetEyeGaze(false, true);

		//update eye dist
		eyeDistCounter++;
		averageEyeDistCm = (eyeDistCounter * averageEyeDistCm + GetEyeDist())/(eyeDistCounter+1);

  }
  else
  {
		//x = (State("MousePosX") - 32768) / Parameter("WindowWidth");
		//y = ((State("MousePosY") - 32768) - 20) / Parameter("WindowHeight");
    //x = State("JoystickXpos") / 32767.0;
    //y = State("JoystickYpos") / 32767.0;

    //use expression filter
    if (Input.Channels() > 0 && !IsNaN(Input(0, 0)))
      x = Input(0, 0);
    if (Input.Channels() > 1 && !IsNaN(Input(0, 0)))
      y = Input(1, 0);
	}

  //bounds check
  if (x >= 1) x = 0.9999f;
  if (x < 0) x = 0.f;
  if (y >= 1) y = 0.9999f;
  if (y < 0) y = 0.f;
	float stX = usevelocity ? State("CursorPosX") / 1000.f + x : x;
	float stY = usevelocity ? State("CursorPosY") / 1000.f + y : y;
	State("CursorPosX") = (uint32_t)(stX * 1000);
	State("CursorPosY") = (uint32_t)(stY * 1000);
}

void ForagingTask::UpdateBoardState()
{
  if (boardStateCounter <= m * n)
  {
    if (boardStateCounter == m * n)
      State("BoardLayout") = ncolors;
    else
      State("BoardLayout") = board[boardStateCounter];
    boardStateCounter++;
  }
}

void ForagingTask::Reset() {
	board = std::vector<int>( n * m );

	int subNColors = Parameter("ColorsInATrial");
  vector<int> cInd = colorsInTrials.back();
  colorsInTrials.pop_back();

	//initialize colors evenly
	int percolor = ( n * m ) / subNColors;
	for ( int i = 0; i < subNColors; i++ ) {
		int ind = cInd[i];
		std::fill( board.begin() + percolor * i, board.begin() + percolor * (i + 1), ind);
	}

	//fills the extra spot(s) with a random color
	int p = std::rand() % subNColors;
	for ( int i = percolor * subNColors; i < n * m; i++ ) {
		board[ i ] = cInd[( p + i ) % subNColors];
	}
	std::shuffle( board.begin(), board.end(), rng );

	clockedtime = 0;
	//reset radius if eye tracker
	if (Parameter("EyetrackerCursor") == 1)
	{
		//update fixation radius based on current head position
		dwellradius = ParameterAngleToNormUnits("DwellVisualAngle");
		radius = ParameterAngleToNormUnits("MinDotAngle");
		maxradius = ParameterAngleToNormUnits("MaxDotAngle");
		//update fixation radius based on current head position
		fixationdwellradius = ParameterAngleToNormUnits("FixationDwellVisualAngle");
		float r = ParameterAngleToNormUnits("FixationDotAngle");
		fixation->SetObjectRect({ 0.5f - r, 0.5f - r * heightfactor, 0.5f + r, 0.5f + r * heightfactor });
	}

	//update each radius
	for (int i = 0; i < m; i++) {
		for (int p = 0; p < n; p++) {
			float x = (i + 0.5f) * xspace;
			float y = (p + 0.5f) * yspace;
			stimuli[i + p * m]->SetObjectRect({ x - radius, y - radius * heightfactor, x + radius, y + radius * heightfactor });
			stimuli[i + p * m]->Change();
		}
	}
	//update colors
	for ( int i = 0; i < n * m; i++ ) {
		stimuli[ i ]->Hide();
		stimuli[ i ]->SetFillColor(  mColors[board[ i ]].second );
		stimuli[ i ]->SetColor(  mColors[board[ i ]].second );
		stimuli[ i ]->Show();
	}
	ctrialtime = 0;

	State( "Score" ) = 0;

	// initialize targets per category on board
	for (int i = 0; i < ncolors; i++) {
		colorspresented[i] = 0;
		colorsharvested[i] = 0;
	}

	// count targets per category on board
	for (int i = 0; i < m*n; i++) {
		colorspresented[board[i]]++;
	}
  boardStateCounter = 0;

}
