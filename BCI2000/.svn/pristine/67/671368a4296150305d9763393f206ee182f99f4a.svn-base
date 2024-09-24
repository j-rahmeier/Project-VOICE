////////////////////////////////////////////////////////////////////////////////
// Authors: schalklab@HR18818.wucon.wustl.edu
// Description: ANTTitration implementation
////////////////////////////////////////////////////////////////////////////////

#include "ANTTitration.h"
#include "BCIStream.h"
#include <QVBoxLayout>
#include <numeric>
#include <iomanip>
#include <Windows.h>
#include "ProgressBarVis.h"

using namespace std;


RegisterFilter( ANTTitration, 3 );
     // Change the location if appropriate, to determine where your filter gets
     // sorted into the chain. By convention:
     //  - filter locations within SignalSource modules begin with "1."
     //  - filter locations within SignalProcessing modules begin with "2."
     //       (NB: SignalProcessing modules must specify this with a Filter() command in their PipeDefinition.cpp file too)
     //  - filter locations within Application modules begin with "3."


ANTTitration::ANTTitration() :
    mrDisplay(Window()),
    introductTxtStimls(nullptr),
    correctTxtStimls(nullptr),
    mImgStimlsCenter(nullptr),
    mpProgressBar(nullptr),
    mStimTriggered(0),
    mNoAmpCounter(0), 
  myPhase(Phase::none)
{
  // C++ does not initialize simple types such as numbers, or pointers, by default.
  // Rather, they will have random values.
  // Take care to initialize any member variables here, so they have predictable values
  // when used for the first time.
}

ANTTitration::~ANTTitration()
{
    Halt();
    delete introductTxtStimls;
    delete correctTxtStimls;
    delete mImgStimlsCenter;
    delete mpProgressBar;
}

void
ANTTitration::Publish()
{

  // Define any parameters that the filter needs....
    BEGIN_PARAMETER_DEFINITIONS

      //new
      "Application:Experiment string CrossFile= % % % % "
      " //The file of fixation cross (inputfile)",

      //keep
     "Application:Experiment float CrossWidth= 0.006 0.006 0 1 "
     " //The width of fixation cross",
     "Application:Experiment float CrossHeight= 0.012 0.012 0 1 "
     " //The height of fixation cross",

     // override background color parameter
     "Application:Application%20Window int WindowBackgroundColor= 0xffffff % % % "
     " // Application window background color (color)",
     // color of text
     "Application:Experiment string TextFontColor= 0x00000000 0x00000000 0x00000000 0xFFFFFFFF // "
     "Color of text (color)",

     //progress bar
     "Application:ProgressBar int ProgressBar= 1 1 0 1 // "
     "Display progress bar (boolean)",
     "Application:ProgressBar float ProgressBarHeight= 50 50 0 % //"
     "Progress bar height in pixels",
     "Application:ProgressBar float ProgressBarWidth= 250 250 0 % //"
     "Progress bar width in pixels",
     "Application:ProgressBar string ProgressBarBackgroundColor= 0x00808080 0x00808080 0x00000000 0xFFFFFFFF // "
     "Color of progress bar background (color)",
     "Application:ProgressBar string ProgressBarForegroundColor= 0x00ffff00 0x00f0f0f0 0x00000000 0xFFFFFFFF // "
     "Color of progress bar foreground (color)",
     //photodiode
     "Application:PhotoDiodePatch int PhotoDiodePatch= 1 1 0 1 "
     "// Display photo diode patch (boolean)",
     "Application:PhotoDiodePatch float PhotoDiodePatchHeight= 0.065 1 0 1 "
     "// Photo diode patch height in relative coordinates",
     "Application:PhotoDiodePatch float PhotoDiodePatchWidth= 0.05 1 0 1 "
     "// Photo diode patch width in relative coordinates",
     "Application:PhotoDiodePatch float PhotoDiodePatchLeft= 0 1 0 1 "
     "// Photo diode patch left in relative coordinates",
     "Application:PhotoDiodePatch float PhotoDiodePatchTop= 0.935 1 0 1 "
     "// Photo diode patch top in relative coordinates",
     "Application:PhotoDiodePatch int PhotoDiodePatchShape= 0 1 0 1 "
     "// Photo diode patch shape: 0 rectangle, 1 ellipse (enumeration)",
     "Application:PhotoDiodePatch int PhotoDiodePatchActiveColor= 0x000000 0 0 0xffffffff "
     "// Photo diode patch color when active (color)",
     "Application:PhotoDiodePatch int PhotoDiodePatchInactiveColor= 0xffffff 0 0 0xffffffff "
     "// Photo diode patch color when inactive, use 0xff000000 for transparent (color)",
 END_PARAMETER_DEFINITIONS

 BEGIN_STATE_DEFINITIONS

   "TitrationTrialNumber 4 0 0 0" //1 - 10 based on how far they make it thru stimulation titration

 END_STATE_DEFINITIONS

}

void
ANTTitration::AutoConfig(const SignalProperties& Input) 
{

}

void
ANTTitration::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
    //check parameter
    float sampleRate = Parameter("SamplingRate");//Sampling rate
    unsigned int blockSize = Parameter("SampleBlockSize");// Sample block size
    float sBlockMs = ((float)(blockSize) / (float)(sampleRate)) * 1000;

    Parameter("CrossFile");

    Parameter("SamplingRate");
    Parameter("SampleBlockSize");
    Parameter("CrossWidth");
    Parameter("CrossHeight");
    Parameter("WindowWidth");
    Parameter("WindowHeight");
    Parameter("WindowLeft");
    Parameter("WindowTop");

    //check state
    State("KeyDown");
    State("Running");
    State("TitrationTrialNumber");

    OptionalState("RTButton2", 0); //ResponseTimeBox states
    OptionalState("RTButton3", 0); //ResponseTimeBox states
    OptionalState("RTButton4", 0); //ResponseTimeBox states

    OptionalState("GTENTrainRunning", 0); //AmpServerPro state

  Output = Input; // this simply passes information through about SampleBlock dimensions, etc....

}


void
ANTTitration::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{

    //sample, block
    mBlockSize = Parameter("SampleBlockSize");// Sample block size


    //keep
    mIntroTxtRect.left = 0.05f;// 0.2f;
    mIntroTxtRect.right = 0.95f;// 0.8f;
    mIntroTxtRect.top = 0.05f;// 0.3f;
    mIntroTxtRect.bottom = 0.95f;// 0.4f;

    //keep
    introductTxtStimls = new TextStimulus(mrDisplay);
    introductTxtStimls->SetText("");
    introductTxtStimls->SetTextHeight(0.025f);//text size, adaptive with the window size
    introductTxtStimls->SetDisplayRect(mIntroTxtRect);
    introductTxtStimls->SetTextColor(RGBColor(Parameter("TextFontColor")));
    //introductTxtStimls->SetPresentationMode(VisualStimulus::ShowHide);
    introductTxtStimls->SetAspectRatioMode(1);
    introductTxtStimls->Conceal();

    mCorTxtRect.left = 0.05f;
    mCorTxtRect.right = 0.95f;
    mCorTxtRect.top = 0.3f;
    mCorTxtRect.bottom = 0.9f;

    correctTxtStimls = new TextStimulus(mrDisplay);
    correctTxtStimls->SetText("");
    correctTxtStimls->SetTextHeight(0.05f);//text size, adaptive with the window size
    correctTxtStimls->SetDisplayRect(mCorTxtRect);
    correctTxtStimls->SetTextColor(RGBColor::Blue);
    correctTxtStimls->Conceal();



    //keep
    mImgRectCenter.left = 0.5f - Parameter("CrossWidth") / 2;// 0.497f;
    mImgRectCenter.right = 0.5f + Parameter("CrossWidth") / 2;// 0.503f;
    mImgRectCenter.top = 0.5f - Parameter("CrossHeight") / 2;// 0.489f;
    mImgRectCenter.bottom = 0.5f + Parameter("CrossHeight") / 2;// 0.511f;
    mImgStimlsCenter = new ImageStimulus(mrDisplay);
    mImgStimlsCenter->SetDisplayRect(mImgRectCenter);
    mImgStimlsCenter->SetRenderingMode(GUI::RenderingMode::Transparent);
    mImgStimlsCenter->SetFile(Parameter("CrossFile").ToString());
    mImgStimlsCenter->Conceal();

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

    //photodiode
    delete mPhotoDiodePatch.pShape;
    mPhotoDiodePatch.pShape = nullptr;
    if (Parameter("PhotoDiodePatch") != 0)
    {
        mPhotoDiodePatch.activeColor = Parameter("PhotoDiodePatchActiveColor").ToNumber();
        mPhotoDiodePatch.inactiveColor = Parameter("PhotoDiodePatchinActiveColor").ToNumber();
        enum { rectangle = 0, ellipse = 1 };
        switch (int(Parameter("PhotoDiodePatchShape")))
        {
        case rectangle:
            mPhotoDiodePatch.pShape = new RectangularShape(Display(), -1);
            break;
        case ellipse:
        default:
            mPhotoDiodePatch.pShape = new EllipticShape(Display(), -1);
            break;
        }
        mPhotoDiodePatch.pShape->SetHeight(Parameter("PhotoDiodePatchHeight"))
            .SetWidth(Parameter("PhotoDiodePatchWidth"))
            .SetPositionX(Parameter("PhotoDiodePatchLeft") + Parameter("PhotoDiodePatchWidth") / 2.0)
            .SetPositionY(Parameter("PhotoDiodePatchTop") + Parameter("PhotoDiodePatchHeight") / 2.0);
        Patch(0);
    }
}

void ANTTitration::Patch( bool active )
{
    if (mPhotoDiodePatch.pShape) {
        if( active )
            mPhotoDiodePatch.pShape->SetColor(mPhotoDiodePatch.activeColor).SetFillColor(mPhotoDiodePatch.activeColor);
        else
            mPhotoDiodePatch.pShape->SetColor(mPhotoDiodePatch.inactiveColor).SetFillColor(mPhotoDiodePatch.inactiveColor);
    }
}

void
ANTTitration::StartRun()
{
    myPhase = Phase::titrationIntro;

    introductTxtStimls->Conceal();
    correctTxtStimls->Conceal();
    Patch(0);

    //progress bar
    if (mpProgressBar)
    {
        mpProgressBar->SetTotal(10).SetCurrent(0);
        mpProgressBar->SendDifferenceFrame();
    }

}

void
ANTTitration::Process( const GenericSignal& Input, GenericSignal& Output )
{
    //state
    switch (myPhase)
    {
    case Phase::titrationIntro:
      mTrialNum = 1;
      if (trialAborted()) {
        myPhase = Phase::titrationAbort;
        break;
      }
      introductTxtStimls->SetText("BEGINNING STIMULATION\n\n"
        "We will start by giving you a test stimulation burst.\n This stimulation will be the same as what you feel during the experiment,\n "
        "which might feel similar to someone pulling your hair.\n"
        "We will ramp up the stimulation amplitude to the normal amount over the course of 10 stimulations.\n Each one will increase in"
        " a small step size.\n"
        "In response to the stimulation, use the Response Box to record how you are feeling, or if you want to abort the experiment, with\n\n"
        "1 = I feel nothing\n 2 = There is a sensation\n 3 = There is a slight discomfort, but manageable.\n 4 = ABORT (this stops the experiment)\n\n"
        "After each stimulation, we will stop and ask you if you wish to continue.\n"
        "Press SPACE to begin the stimulation titration.");
      introductTxtStimls->Present();
      if (checkKeySpace()) {
        myPhase = Phase::titrationStim;
        introductTxtStimls->Conceal();
        mImgStimlsCenter->Present();
        Patch(0);
      }
      break;
    case Phase::titrationStim:
      if (trialAborted()) {
        myPhase = Phase::titrationAbort;
        break;
      }
      if (stimulationOccurred()) {
        myPhase = Phase::titrationPause;
        mTrialNum++;
      }
      break;
    case Phase::titrationPause:
      if (trialAborted()) {
        myPhase = Phase::titrationAbort;
        break;
      }
      mImgStimlsCenter->Conceal();
      if (mTrialNum <= 10)
      {
        introductTxtStimls->SetText("TESTING STIMULATION\n\n"
          "The stimulation has occurred.\n"
          "Please press a button on the Response Box about how you felt:\n\n"\
          "1 = I feel nothing\n 2 = There is a sensation\n 3 = There is a slight discomfort, but manageable.\n 4 = ABORT (this stops the experiment)\n\n"
          "We have " + std::to_string(11 - mTrialNum) + " more test stimulations to complete. Would you like to continue?\n\n"
          "Press SPACE to continue the stimulation titration.");
      }
      else
      {
        introductTxtStimls->SetText("TESTING STIMULATION\n\n"
          "The stimulation testing is over!\n"
          "Please press a button on the Response Box about how you felt about the final stimulation:\n\n"\
          "1 = I feel nothing\n 2 = There is a sensation\n 3 = There is a slight discomfort, but manageable.\n 4 = ABORT (this stops the experiment)\n\n"
          "We can now continue with the experiment.\n\n"
          "Press SPACE to continue.");
      }
      introductTxtStimls->Present();
      //progress bar
      if (mpProgressBar)
      {
        mpProgressBar->SetCurrent(mTrialNum);
        mpProgressBar->SendDifferenceFrame();
      }
      if (checkKeySpace()) {
        introductTxtStimls->Conceal();
        if (mTrialNum <= 10)
        {
          myPhase = Phase::titrationStim;
          mImgStimlsCenter->Present();
        }
        else
        {
          myPhase = Phase::none;
        }
      }
      break;
    case Phase::titrationAbort:
      mImgStimlsCenter->Conceal();
      introductTxtStimls->SetText("EXPERIMENT ABORTED\n\n"
        "You have aborted the experiment by pressing the Response Box Button 4.\n"
        "Thank you for participating, the experimenter will be with you soon.\n");
      introductTxtStimls->Present();
      State("Running") = 0;
      break;
    case Phase::none:
            State("Running") = 0;
        break;
    default:
        break;
    }
        
    State("TitrationTrialNumber") = mTrialNum;
    Output = Input;
}

void
ANTTitration::StopRun()
{
    myPhase = Phase::none;
    Patch(0);
}

void
ANTTitration::Halt()
{
}


//triggered if RTButton4 is pressed
bool
ANTTitration::trialAborted() {
  auto st = OptionalState("RTButton4", 0);
  if (st == 1)
    return true;
  else
    return false;
}

//stimulation is done if it happened and then stopped
//4th bit is low if stimulation is happening
bool
ANTTitration::stimulationOccurred() {
  auto st = OptionalState("GTENTrainRunning", 0);
  if (st != NULL) {
    bool s = (~st & 2) || (~st & 4) || (~st & 16); //bits 2, 3, and 5
    //bool s = (st >> 1) & 0; //bits 2, 3, and 5
    if (s == true) {
      //stim is occurring
      mStimTriggered = true;
      return false;
    }
    else {
      if (mStimTriggered) {
        //stim has ended
        mStimTriggered = false;
        return true;
      }
    }
  }
  else {
    mNoAmpCounter++;
    //mimic stimulation occurring when amp is not connected
    if (mNoAmpCounter >= 20) {
      mNoAmpCounter = 0;
      return true;
    }
  }
  return false;
}

bool
ANTTitration::checkKeySpace() {
    for (unsigned int i = 0; i < mBlockSize; i++)
    {
        if (State("KeyDown")(i) == 32){
            return true;        
        }        
    }
    return false;
}

