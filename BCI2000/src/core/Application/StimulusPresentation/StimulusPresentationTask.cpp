////////////////////////////////////////////////////////////////////////////////
// $Id: StimulusPresentationTask.cpp 8097 2024-05-02 12:53:16Z mellinger $
// Authors: jhizver@wadsworth.org, schalk@wadsworth.org,
//   juergen.mellinger@uni-tuebingen.de
// Description: The task filter for a stimulus presentation task.
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
#include "StimulusPresentationTask.h"

#include "AVStimulus.h"
#include "Expression/Expression.h"
#include "ImageStimulus.h"
#include "Localization.h"
#include "TextStimulus.h"
#include "ProgressBarVis.h"

#include <algorithm>
#include <regex>

RegisterFilter(StimulusPresentationTask, 3);

StimulusPresentationTask::StimulusPresentationTask()
    : mNumberOfSequences(0), mSequenceType(SequenceTypes::Deterministic), mPreSequenceBlockCount(0),
      mPostResultBlockCount(0), mSequenceCount(0), mToBeCopiedPos(mToBeCopied.begin()),
      mSequencePos(mSequence.begin()), mpProgressBar(nullptr)
{
  // Sequencing
  BEGIN_PARAMETER_DEFINITIONS
      "Application:Sequencing intlist Sequence= 4 1 3 4 2 % % % // "
         "Sequence in which stimuli are presented (deterministic mode)/"
         " Stimulus frequencies for each stimulus (random mode)/"
         " Ignored (P3Speller compatible mode)",

      "Application:Sequencing int SequenceType= 0 0 0 2 // "
          "Sequence of stimuli is 0 deterministic, 1 random, 2 P3Speller compatible (enumeration)",

      "Application:Sequencing int NumberOfSequences= 3 1 0 % // "
          "number of sequence repetitions",

      "Application:Sequencing intlist ToBeCopied= 3 1 2 3 1 1 % // "
          "Sequence in which stimuli need to be copied (only used in copy mode)",

      "Application:Sequencing string UserComment= % % % % // "
          "User comments for a specific run",
  END_PARAMETER_DEFINITIONS

  // Stimulus definition
  BEGIN_PARAMETER_DEFINITIONS
      "Application:Stimuli matrix Stimuli= "
          "{ caption icon av } "                                             // row labels
          "{ stimulus1 stimulus2 stimulus3 stimulus4 stimulus5 stimulus6 } " // column labels
          " One Two Three Four Five Six "
          "images/1.bmp images/2.bmp images/3.bmp images/4.bmp images/5.bmp images/6.bmp "
          "sounds/1.wav sounds/2.wav sounds/3.wav sounds/4.wav sounds/5.wav sounds/6.wav  "
          " // captions and icons to be displayed, av media to be played for different stimuli",

      "Application:Stimuli matrix FocusOn= "
          "{ caption icon av } " // row labels
          "{ focuson } "         // column labels
          "Please%20focus%20on " // caption
          "images/focuson.bmp " // video
          "sounds/uh-uh.wav  "  // audio
          " // initial announcement what to focus on",

      "Application:Stimuli matrix Result= "
          "{ caption icon av } " // row labels
          "{ result } "          // column labels
          "The%20result%20was "  // caption
          "images/result.bmp "  // video
          "sounds/uh-uh.wav "   // audio
          " // final result announcement ",
  END_PARAMETER_DEFINITIONS

  // Stimulus properties
  BEGIN_PARAMETER_DEFINITIONS
      "Application:Stimuli int StimulusWidth= 5 0 0 100 // "
          "StimulusWidth in percent of screen width (zero for original pixel size)",
      "Application:Stimuli int CaptionHeight= 10 0 0 100 // "
          "Height of stimulus caption text in percent of screen height",
      "Application:Stimuli string CaptionColor= 0x00FFFFFF 0x00FFFFFF 0x00000000 0xFFFFFFFF // "
          "Color of stimulus caption text (color)",
      "Application:Stimuli string BackgroundColor= 0x00FFFF00 0x00FFFF00 0x00000000 0xFFFFFFFF // "
          "Color of stimulus background (color)",
      "Application:Stimuli int CaptionSwitch= 1 1 0 1 // "
          "Present captions (boolean)",
      "Application:Stimuli int IconSwitch= 1 1 0 1 // "
          "Present icon files (boolean)",
      "Application:Stimuli int AVSwitch= 0 0 0 1 // "
          "Present media files (boolean)",
      "Application:Stimuli float AudioVolume= 100 100 0 % // "
          "Volume for audio playback in percent",
      "Application:Stimuli int AVPlayToEnd= 1 1 0 1 // "
          "Never abort playback of media files (boolean)",
      "Application:Stimuli int AudioSwitch= 1 1 0 1 // "
          "Present audio files (boolean)(superseded by AVSwitch)",

      "Application:Stimuli int TestAllStimuli= 0 1 0 1 // "
          "Load all stimuli during preflight phase, even if not presented (boolean)",

      "Application:ProgressBar int ProgressBar= 0 1 0 1 // "
          "Display progress bar (boolean)",
      "Application:ProgressBar float ProgressBarHeight= 50 50 0 % //"
          "Progress bar height in pixels",
      "Application:ProgressBar float ProgressBarWidth= 250 250 0 % //"
          "Progress bar width in pixels",
      "Application:ProgressBar string ProgressBarBackgroundColor= 0x00808080 0x00808080 0x00000000 0xFFFFFFFF // "
          "Color of progress bar background (color)",
      "Application:ProgressBar string ProgressBarForegroundColor= 0x00ffff00 0x00f0f0f0 0x00000000 0xFFFFFFFF // "
          "Color of progress bar foreground (color)",
  END_PARAMETER_DEFINITIONS

  LANGUAGES "German",
    BEGIN_LOCALIZED_STRINGS "TIME OUT !!!", "Zeit abgelaufen!",
          "Waiting to start ...", "Warte ...",
    END_LOCALIZED_STRINGS

  BEGIN_STATE_DEFINITIONS
    "SelectedStimulus 16 0 0 0",
  END_STATE_DEFINITIONS
}

StimulusPresentationTask::~StimulusPresentationTask()
{
    mStimuli.DeleteObjects();
    mTargets.DeleteObjects();
    mFocusAnnouncement.DeleteObjects();
    mResultAnnouncement.DeleteObjects();
    delete mpProgressBar;
}

void StimulusPresentationTask::OnAutoConfig(const SignalProperties &)
{
}

void StimulusPresentationTask::OnPreflight(const SignalProperties &Input) const
{
    mStimulusModeWarningsReported.clear();

    ParamRef Sequence = Parameter("Sequence");
    switch (int(Parameter("SequenceType")))
    {
    case SequenceTypes::Deterministic:
        for (int i = 0; i < Sequence->NumValues(); ++i)
            if (Sequence(i) < 1)
                bcierr << "Invalid stimulus code "
                       << "(" << Sequence(i) << ") "
                       << "at Sequence(" << i + 1 << ")";
        break;

    case SequenceTypes::Random:
        for (int i = 0; i < Sequence->NumValues(); ++i)
            if (Sequence(i) < 0)
                bcierr << "Invalid frequency "
                       << "(" << Sequence(i) << ") "
                       << "at Sequence(" << i + 1 << ")";
        break;

    case SequenceTypes::P3Speller:
        if (Parameter("ProgressBar") != 0)
            bcierr << "Cannot display progress bar in P3Speller mode";
        break;

    default:
        bcierr << "Unknown value in SequenceType parameter";
    }

    GUI::GraphDisplay preflightDisplay;
    preflightDisplay.SetAssociatedAudio(Display().AssociatedAudio());
    ImageStimulus *pImageStimulus = NULL;
    if (Parameter("IconSwitch") == 1)
        pImageStimulus = new ImageStimulus(preflightDisplay);

    AVStimulus *pAVStimulus = 0;
    if (Parameter("AVSwitch") == 1 || Parameter("AudioSwitch") == 1)
    {
        pAVStimulus = new AVStimulus(preflightDisplay);
        if (!pAVStimulus->Error().Empty())
            throw bcierr << pAVStimulus->Error();
    }
    bool videoReported = false, audioReported = false;

    bool presentFocusOn = false, presentResult = false;

    switch (int(Parameter("InterpretMode")))
    {
    case InterpretModes::Copy:
        presentFocusOn = true;
        presentResult = (Parameter("DisplayResults") == 1);
        Parameter("ToBeCopied");
        break;

    case InterpretModes::Free:
        presentFocusOn = false;
        presentResult = (Parameter("DisplayResults") == 1);
        break;

    case InterpretModes::None:
    default:
        presentFocusOn = false;
        presentResult = false;
    }
    int focusDuration =
            static_cast<int>(StimulusProperty(Parameter("FocusOn"), 0, "StimulusDuration").InSampleBlocks()),
        resultDuration =
            static_cast<int>(StimulusProperty(Parameter("Result"), 0, "StimulusDuration").InSampleBlocks()),
        preSequenceDuration = static_cast<int>(Parameter("PreSequenceDuration").InSampleBlocks()),
        postSequenceDuration = static_cast<int>(Parameter("PostSequenceDuration").InSampleBlocks());

    if (presentFocusOn && preSequenceDuration < 2 * focusDuration)
        bcierr << "When FocusOn message and target stimulus are presented, "
               << "PreSequenceDuration must at least be twice the StimulusDuration "
               << "applying for the FocusOn message";
    if (presentResult && postSequenceDuration < 2 * resultDuration)
        bcierr << "When Result message and result stimulus are presented, "
               << "PostSequenceDuration must at least be twice the StimulusDuration "
               << "applying for the Result message";

    std::vector<std::string> stimParams;
    stimParams.push_back("Stimuli");
    if (presentFocusOn)
        stimParams.push_back("FocusOn");
    if (presentResult)
        stimParams.push_back("Result");

    // For parameters defining a time value, issue a warning if limited temporal
    // resolution leads to a discrepancy greater than 1ms.
    const char *timeParams[] = {
        "StimulusDuration",
        "ISIMinDuration",
        "ISIMaxDuration",
    };
    double oneMillisecond = MeasurementUnits::TimeInSampleBlocks("1ms");
    int minStimDuration = 0, minISIDuration = 0;
    double epochLength = OptionalParameter("EpochLength", 0).InSampleBlocks();

    std::set<int> activeStimuli = DetermineActiveStimuli();

    enum
    {
        caption,
        icon,
        av
    };
    for (size_t i = 0; i < stimParams.size(); ++i)
        for (int j = 0; j < Parameter(stimParams[i])->NumColumns(); ++j)
        {
            if(!Parameter("TestAllStimuli"))
            {
              // For efficiency, avoid loading stimuli that are not going to be used
              if (stimParams[i] == "Stimuli" && activeStimuli.find(j + 1) == activeStimuli.end())
                  continue;
            }

            int stimDuration = static_cast<int>(
                    StimulusProperty(Parameter(stimParams[i]), j, "StimulusDuration").InSampleBlocks()),
                isiDuration =
                    static_cast<int>(StimulusProperty(Parameter(stimParams[i]), j, "ISIMinDuration").InSampleBlocks());
            if (minStimDuration > stimDuration)
                minStimDuration = stimDuration;
            if (minISIDuration > isiDuration)
                minISIDuration = isiDuration;
            for (size_t k = 0; k < sizeof(timeParams) / sizeof(*timeParams); ++k)
            { // Check individual stimulus durations.
                float value = StimulusProperty(Parameter(stimParams[i]), j, timeParams[k]).InSampleBlocks();

                // Warning if StimulusDuration is rounded down to the next smaller integer because of a StimulusDuration
                // a) smaller than a multiple of the SampleBlockSize
                // b) larger than a multiple of the SampleBlockSize by more than one millisecond
                // c) less than one SampleBlockSize
                if (value > std::numeric_limits<float>::epsilon() && (value < 1.0 || fmod(value, 1.0) > oneMillisecond))
                    bciwarn << "Due to a sample block duration of " << 1.0f / oneMillisecond << "ms,"
                            << " the actual value of " << timeParams[k] << " for stimulus " << j + 1 << " will be "
                            << ::floor(value) / oneMillisecond << "ms"
                            << " rather than " << value / oneMillisecond << "ms";
            }
            // Check whether EarlyOffsetExpression is valid.
            std::string exprstr = StimulusProperty(Parameter(stimParams[i]), j, "EarlyOffsetExpression");
            GenericSignal preflightSignal(Input);
            if (!Expression(exprstr).IsValid(&preflightSignal))
            {
                bcierr << "error in EarlyOffsetExpression field for stimulus #" << j + 1 << ": ";
                Expression(exprstr).Evaluate(&preflightSignal);
            }

            // Test availability of icon and audio files.
            if (pImageStimulus != NULL)
            {
                std::string file = StimulusMode(Parameter(stimParams[i]), j, "icon", icon);
                if (!file.empty())
                    pImageStimulus->SetFile(file);
            }
            if (pAVStimulus != NULL)
            {
                std::string file = StimulusMode(Parameter(stimParams[i]), j, "av|audio", av);
                if (!file.empty())
                {
                    pAVStimulus->SetSource(file);
                    if (!pAVStimulus->Error().Empty())
                        bcierr << pAVStimulus->Error();
                    else
                    {
                        if (!videoReported && !pAVStimulus->VideoDevice().Empty())
                        {
                            bciout << "Rendering visual stimuli to \"" << pAVStimulus->VideoDevice() << "\"";
                            videoReported = true;
                        }
                        if (!audioReported && !pAVStimulus->AudioDevice().Empty())
                        {
                            bciout << "Rendering auditory stimuli to \"" << pAVStimulus->AudioDevice() << "\"";
                            audioReported = true;
                        }
                    }
                }
            }
        }

    int minStimToClassInterval = minStimDuration + minISIDuration + postSequenceDuration;
    if (presentResult && epochLength > minStimToClassInterval)
        bcierr << "EpochLength is " << epochLength / oneMillisecond << "ms, exceeds "
               << "allowable maximum of " << minStimToClassInterval / oneMillisecond << "ms. "
               << "This maximum corresponds to the condition that classification "
               << "must be finished before the next sequence of stimuli may begin. "
               << "Increase the sum of the "
               << "StimulusDuration, ISIMinDuration, and PostSequenceDuration "
               << "parameters, or decrease EpochLength, to fix this problem";

    delete pImageStimulus;
    delete pAVStimulus;
}

void StimulusPresentationTask::OnInitialize(const SignalProperties & /*Input*/)
{
    // Read sequence parameters.
    mNumberOfSequences = Parameter("NumberOfSequences");
    mSequenceType = Parameter("SequenceType");
    mToBeCopied.clear();

    bool presentFocusOn = false, presentResult = false;
    switch (int(Parameter("InterpretMode")))
    {
    case InterpretModes::Copy:
        presentFocusOn = true;
        presentResult = (Parameter("DisplayResults") == 1);
        // Sequence of stimuli to attend.
        for (int i = 0; i < Parameter("ToBeCopied")->NumValues(); ++i)
            mToBeCopied.push_back(Parameter("ToBeCopied")(i));
        mToBeCopiedPos = mToBeCopied.begin();
        break;

    case InterpretModes::Free:
        presentFocusOn = false;
        presentResult = (Parameter("DisplayResults") == 1);
        break;

    case InterpretModes::None:
    default:
        presentFocusOn = false;
        presentResult = false;
    }

    // Create stimuli and targets
    mStimuli.DeleteObjects();
    mTargets.DeleteObjects();
    Associations().clear();

    bool captionSwitch = (Parameter("CaptionSwitch") == 1), iconSwitch = (Parameter("IconSwitch") == 1),
         avSwitch = (Parameter("AVSwitch") == 1 || Parameter("AudioSwitch") == 1);

    RGBColor backgroundColor = RGBColor::NullColor;
    if (!iconSwitch)
        backgroundColor = RGBColor(Parameter("BackgroundColor"));

    enum
    {
        caption,
        icon,
        av
    };
    ParamRef Stimuli = Parameter("Stimuli");
    std::set<int> activeStimuli = DetermineActiveStimuli();
    for (int i = 0; i < Stimuli->NumColumns(); ++i)
    {
        // For efficiency, avoid loading stimuli that are not going to be used
        if (activeStimuli.find(i + 1) == activeStimuli.end())
            continue;
      
        Associations()[i + 1].SetStimulusDuration(
            static_cast<int>(StimulusProperty(Stimuli, i, "StimulusDuration").InSampleBlocks()));
        Associations()[i + 1].SetISIMinDuration(
            static_cast<int>(StimulusProperty(Stimuli, i, "ISIMinDuration").InSampleBlocks()));
        Associations()[i + 1].SetISIMaxDuration(
            static_cast<int>(StimulusProperty(Stimuli, i, "ISIMaxDuration").InSampleBlocks()));
        Associations()[i + 1].SetEarlyOffsetExpression(
            std::string(StimulusProperty(Stimuli, i, "EarlyOffsetExpression")));

        double stimulusWidth = StimulusProperty(Stimuli, i, "StimulusWidth") / 100.0,
               captionHeight = StimulusProperty(Stimuli, i, "CaptionHeight") / 100.0,
               audioVolume = StimulusProperty(Stimuli, i, "AudioVolume") / 100.0;
        RGBColor captionColor = RGBColor(StimulusProperty(Stimuli, i, "CaptionColor"));
        bool avPlayToEnd = (StimulusProperty(Stimuli, i, "AVPlayToEnd") == 1);

        if (captionSwitch)
        {
            GUI::Rect captionRect = {0.5, (1 - captionHeight) / 2, 0.5, (1 + captionHeight) / 2};
            TextStimulus *pStimulus = new TextStimulus(Display());
            pStimulus->SetText(StimulusMode(Stimuli, i, "caption", caption))
                .SetTextHeight(1.0)
                .SetTextColor(captionColor)
                .SetColor(backgroundColor)
                .SetScalingMode(GUI::ScalingMode::AdjustBoth)
                .SetObjectRect(captionRect);
            pStimulus->SetPresentationMode(VisualStimulus::ShowHide);
            mStimuli.Add(pStimulus);
            Associations()[i + 1].Add(pStimulus);
        }
        int iconSizeMode = GUI::ScalingMode::AdjustHeight;
        if (stimulusWidth <= 0)
            iconSizeMode = GUI::ScalingMode::AdjustBoth;
        GUI::Rect iconRect = {(1 - stimulusWidth) / 2, 0.5, (1 + stimulusWidth) / 2, 0.5};
        if (iconSwitch && StimulusMode(Stimuli, i, "icon", icon) != "")
        {
            ImageStimulus *pStimulus = new ImageStimulus(Display());
            pStimulus->SetFile(StimulusMode(Stimuli, i, "icon", icon))
                .SetRenderingMode(GUI::RenderingMode::Opaque)
                .SetScalingMode(iconSizeMode)
                .SetObjectRect(iconRect);
            pStimulus->SetPresentationMode(VisualStimulus::ShowHide);
            mStimuli.Add(pStimulus);
            Associations()[i + 1].Add(pStimulus);
        }
        if (avSwitch && StimulusMode(Stimuli, i, "av|audio", av) != "")
        {
            AVStimulus *pStimulus = new AVStimulus(Display());
            pStimulus->SetSource(StimulusMode(Stimuli, i, "av|audio", av))
                .SetVolume(audioVolume)
                .SetAbortOnConceal(!avPlayToEnd)
                .SetScalingMode(iconSizeMode)
                .SetObjectRect(iconRect);
            mStimuli.Add(pStimulus);
            Associations()[i + 1].Add(pStimulus);
        }
        Target *pTarget = new Target;
        pTarget->SetTag(i + 1);
        mTargets.Add(pTarget);
        Associations()[i + 1].Add(pTarget);
    }

    // Create FocusOn stimuli
    mFocusAnnouncement.DeleteObjects();
    if (presentFocusOn)
    {
        ParamRef FocusOn = Parameter("FocusOn");
        mFocusAnnouncement.SetStimulusDuration(
            static_cast<int>(StimulusProperty(FocusOn, 0, "StimulusDuration").InSampleBlocks()));
        for (int i = 0; i < FocusOn->NumColumns(); ++i)
        {
            float stimulusWidth = StimulusProperty(FocusOn, i, "StimulusWidth") / 100.0,
                  captionHeight = StimulusProperty(FocusOn, i, "CaptionHeight") / 100.0,
                  audioVolume = StimulusProperty(FocusOn, i, "AudioVolume") / 100.0;
            RGBColor captionColor = RGBColor(StimulusProperty(FocusOn, i, "CaptionColor"));
            bool avPlayToEnd = (StimulusProperty(FocusOn, i, "AVPlayToEnd") == 1);

            if (captionSwitch)
            {
                GUI::Rect captionRect = {0.5, (1 - captionHeight) / 2, 0.5, (1 + captionHeight) / 2};
                TextStimulus *pStimulus = new TextStimulus(Display());
                pStimulus->SetText(StimulusMode(FocusOn, i, "caption", caption))
                    .SetTextHeight(1.0)
                    .SetTextColor(captionColor)
                    .SetColor(backgroundColor)
                    .SetScalingMode(GUI::ScalingMode::AdjustWidth)
                    .SetObjectRect(captionRect);
                pStimulus->SetPresentationMode(VisualStimulus::ShowHide);
                mFocusAnnouncement.Add(pStimulus);
            }
            int iconSizeMode = GUI::ScalingMode::AdjustHeight;
            if (stimulusWidth <= 0)
                iconSizeMode = GUI::ScalingMode::AdjustBoth;
            GUI::Rect iconRect = {(1 - stimulusWidth) / 2, 0.5, (1 + stimulusWidth) / 2, 0.5};
            if (iconSwitch && StimulusMode(FocusOn, i, "icon", icon) != "")
            {
                ImageStimulus *pStimulus = new ImageStimulus(Display());
                pStimulus->SetFile(StimulusMode(FocusOn, i, "icon", icon))
                    .SetRenderingMode(GUI::RenderingMode::Opaque)
                    .SetScalingMode(iconSizeMode)
                    .SetObjectRect(iconRect);
                pStimulus->SetPresentationMode(VisualStimulus::ShowHide);
                mFocusAnnouncement.Add(pStimulus);
            }
            if (avSwitch && StimulusMode(FocusOn, i, "av|audio", av) != "")
            {
                AVStimulus *pStimulus = new AVStimulus(Display());
                pStimulus->SetSource(StimulusMode(FocusOn, i, "av|audio", av))
                    .SetVolume(audioVolume)
                    .SetAbortOnConceal(!avPlayToEnd)
                    .SetScalingMode(iconSizeMode)
                    .SetObjectRect(iconRect);
                mFocusAnnouncement.Add(pStimulus);
            }
        }
    }
    // Create result stimuli
    mResultAnnouncement.DeleteObjects();
    if (presentResult)
    {
        ParamRef Result = Parameter("Result");
        mResultAnnouncement.SetStimulusDuration(
            static_cast<int>(StimulusProperty(Result, 0, "StimulusDuration").InSampleBlocks()));
        for (int i = 0; i < Result->NumColumns(); ++i)
        {
            float stimulusWidth = StimulusProperty(Result, i, "StimulusWidth") / 100.0,
                  captionHeight = StimulusProperty(Result, i, "CaptionHeight") / 100.0,
                  audioVolume = StimulusProperty(Result, i, "AudioVolume") / 100.0;
            RGBColor captionColor = RGBColor(StimulusProperty(Result, i, "CaptionColor"));
            bool avPlayToEnd = (StimulusProperty(Result, i, "AVPlayToEnd") == 1);

            if (captionSwitch)
            {
                GUI::Rect captionRect = {0.5, (1 - captionHeight) / 2, 0.5, (1 + captionHeight) / 2};
                TextStimulus *pStimulus = new TextStimulus(Display());
                pStimulus->SetText(StimulusMode(Result, i, "caption", caption))
                    .SetTextHeight(1.0)
                    .SetTextColor(captionColor)
                    .SetColor(backgroundColor)
                    .SetScalingMode(GUI::ScalingMode::AdjustWidth)
                    .SetObjectRect(captionRect);
                pStimulus->SetPresentationMode(VisualStimulus::ShowHide);
                mResultAnnouncement.Add(pStimulus);
            }
            int iconSizeMode = GUI::ScalingMode::AdjustHeight;
            if (stimulusWidth <= 0)
                iconSizeMode = GUI::ScalingMode::AdjustBoth;
            GUI::Rect iconRect = {(1 - stimulusWidth) / 2, 0.5, (1 + stimulusWidth) / 2, 0.5};
            if (iconSwitch && StimulusMode(Result, i, "icon", icon) != "")
            {
                ImageStimulus *pStimulus = new ImageStimulus(Display());
                pStimulus->SetFile(StimulusMode(Result, i, "icon", icon))
                    .SetRenderingMode(GUI::RenderingMode::Opaque)
                    .SetScalingMode(iconSizeMode)
                    .SetObjectRect(iconRect);
                pStimulus->SetPresentationMode(VisualStimulus::ShowHide);
                mResultAnnouncement.Add(pStimulus);
            }
            if (avSwitch && StimulusMode(Result, i, "av|audio", av) != "")
            {
                AVStimulus *pStimulus = new AVStimulus(Display());
                pStimulus->SetSource(StimulusMode(Result, i, "av|audio", av))
                    .SetVolume(audioVolume)
                    .SetAbortOnConceal(!avPlayToEnd)
                    .SetScalingMode(iconSizeMode)
                    .SetObjectRect(iconRect);
                mResultAnnouncement.Add(pStimulus);
            }
        }
    }

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

void StimulusPresentationTask::OnStartRun()
{
    DisplayMessage(LocalizableString("Waiting to start ..."));

    // Create a sequence for this run.
    mSequence.clear();
    int numSubSequences = mNumberOfSequences, totalStimuli = 0;
    if (Parameter("InterpretMode") == InterpretModes::Copy)
        numSubSequences *= static_cast<int>(mToBeCopied.size());
    switch (mSequenceType)
    {
    case SequenceTypes::Deterministic:
        totalStimuli = numSubSequences * Parameter("Sequence")->NumValues();
        for (int i = 0; i < numSubSequences; ++i)
        {
            for (int j = 0; j < Parameter("Sequence")->NumValues(); ++j)
                mSequence.push_back(Parameter("Sequence")(j));
            mSequence.push_back(0);
        }
        break;

    case SequenceTypes::Random: {
        std::vector<int> seq;
        for (int i = 0; i < Parameter("Sequence")->NumValues(); ++i)
            for (int j = 0; j < Parameter("Sequence")(i); ++j)
                seq.push_back(i + 1);
        totalStimuli = numSubSequences * seq.size();

        for (int i = 0; i < numSubSequences; ++i)
        {
            std::shuffle(seq.begin(), seq.end(), RandomNumberGenerator);
            mSequence.insert(mSequence.end(), seq.begin(), seq.end());
            mSequence.push_back(0);
        }
    }
    break;

    case SequenceTypes::P3Speller:
        totalStimuli = 1;
        mSequenceCount = 0;
        // start with an empty sequence
        break;
    }
    mSequencePos = mSequence.begin();
    mToBeCopiedPos = mToBeCopied.begin();
    if (mpProgressBar)
    {
        mpProgressBar->SetTotal(totalStimuli).SetCurrent(0);
        mpProgressBar->SendDifferenceFrame();
    }
}

void StimulusPresentationTask::OnStopRun()
{
    mStimuli.Stop();
    mFocusAnnouncement.Stop();
    mResultAnnouncement.Stop();
    DisplayMessage(LocalizableString("TIME OUT !!!"));
}

void StimulusPresentationTask::OnPreSequence()
{
    DetermineAttendedTarget();
    DisplayMessage("");
    State("SelectedStimulus") = 0;
    mFocusAnnouncement.Present();
    mPreSequenceBlockCount = 0;
}

void StimulusPresentationTask::DoPreSequence(const GenericSignal &, bool & /*doProgress*/)
{
    if (mPreSequenceBlockCount == mFocusAnnouncement.StimulusDuration())
    {
        mFocusAnnouncement.Conceal();
        if (AttendedTarget() != NULL)
            Associations()[AttendedTarget()->Tag()].Present();
    }

    if (mPreSequenceBlockCount == 2 * mFocusAnnouncement.StimulusDuration())
    {
        if (AttendedTarget() != NULL)
            Associations()[AttendedTarget()->Tag()].Conceal();
    }

    ++mPreSequenceBlockCount;
}

void StimulusPresentationTask::OnSequenceBegin()
{
    mFocusAnnouncement.Conceal();
}

void StimulusPresentationTask::OnPostRun()
{
    State("SelectedStimulus") = 0;
}

Target *StimulusPresentationTask::OnClassResult(const ClassResult &inResult)
{
    Target *pTarget = StimulusTask::OnClassResult(inResult);
    if (pTarget != NULL)
    {
        State("SelectedStimulus") = pTarget->Tag();
        mResultAnnouncement.Present();
        mPostResultBlockCount = 0;
    }
    return pTarget;
}

void StimulusPresentationTask::DoPostSequence(const GenericSignal &, bool & /*doProgress*/)
{
    if (mPostResultBlockCount == mResultAnnouncement.StimulusDuration())
    {
        mResultAnnouncement.Conceal();
        if (State("SelectedStimulus") > 0)
            Associations()[State("SelectedStimulus")].Present();
    }

    if (mPostResultBlockCount == 2 * mResultAnnouncement.StimulusDuration())
    {
        if (State("SelectedStimulus") > 0)
            Associations()[State("SelectedStimulus")].Conceal();
    }

    ++mPostResultBlockCount;
}

void StimulusPresentationTask::Process(const GenericSignal& Input, GenericSignal& Output)
{
  StimulusTask::Process(Input, Output);
}

int StimulusPresentationTask::OnNextStimulusCode()
{
    // When in P3Speller compatible mode, create a new sequence each time we have come to end of the previous one.
    // Also, in P3Speller compatible mode, do not terminate the run except when we are in copy mode.
    if (mSequenceType == SequenceTypes::P3Speller && mSequencePos == mSequence.end() &&
        !(int(Parameter("InterpretMode")) == InterpretModes::Copy && mSequenceCount == int(mToBeCopied.size())))
    {
        ++mSequenceCount;
        mSequence.clear();
        for (int i = 0; i < mNumberOfSequences; ++i)
        {
            std::vector<int> seq;
            for (int j = 0; j < Parameter("Stimuli")->NumColumns(); ++j)
                seq.push_back(j + 1);
            std::shuffle(seq.begin(), seq.end(), RandomNumberGenerator);
            mSequence.insert(mSequence.end(), seq.begin(), seq.end());
        }
        mSequence.push_back(0);
        mSequencePos = mSequence.begin();
    }
    int result =  mSequencePos == mSequence.end() ? 0 : *mSequencePos++;
    if (mpProgressBar && result != 0)
    {
        Assert(mpProgressBar->Current() < mpProgressBar->Total());
        int current = mpProgressBar->Current();
        mpProgressBar->SetCurrent(current + 1);
        mpProgressBar->SendDifferenceFrame();
    }
    return result;
}

void StimulusPresentationTask::DetermineAttendedTarget()
{
    Target *pTarget = NULL;

    if (mToBeCopiedPos == mToBeCopied.end())
        mToBeCopiedPos = mToBeCopied.begin();

    if (mToBeCopiedPos != mToBeCopied.end())
    {
        int code = *mToBeCopiedPos++;
        SetOfTargets::const_iterator i = mTargets.begin();
        while (pTarget == NULL && i != mTargets.end())
        {
            if ((*i)->Tag() == code)
                pTarget = *i;
            ++i;
        }
    }
    SetAttendedTarget(pTarget);
}

ParamRef StimulusPresentationTask::StimulusMode(const ParamRef& inMatrixParam, int inColIndex,
    const std::string& inRowLabelRegex, int inRowIndex) const
{
    std::regex expr(inRowLabelRegex, std::regex::icase);
    for (int i = 0; i < inMatrixParam->RowLabels().Size(); ++i)
    {
        const std::string& label = inMatrixParam->RowLabels()[i];
        if (std::regex_match(label, expr))
            return inMatrixParam(label, inColIndex);
    }
    std::ostringstream warning;
    warning << "Required row label \"" << inRowLabelRegex << "\" does not exist in parameter \""
            << inMatrixParam->Name() << "\", using row #" << inRowIndex + 1 << " instead";
    if (mStimulusModeWarningsReported.find(warning.str()) == mStimulusModeWarningsReported.end())
    {
        bciwarn << warning.str();
        mStimulusModeWarningsReported.insert(warning.str());
    }
    return inMatrixParam(inRowIndex, inColIndex);
}

ParamRef StimulusPresentationTask::StimulusProperty(const ParamRef &inMatrixParam, int inColIndex,
                                                    const std::string &inProperty) const
{
    return inMatrixParam->RowLabels().Exists(inProperty) ? inMatrixParam(inProperty, inColIndex)
                                                         : Parameter(inProperty);
}

std::set<int> StimulusPresentationTask::DetermineActiveStimuli() const
{
  std::set<int> activeStimuli;
  ParamRef Sequence = Parameter("Sequence");
  ParamRef Stimuli = Parameter("Stimuli");
  switch (int(Parameter("SequenceType")))
  {
  case SequenceTypes::Deterministic:
    for (int i = 0; i < Sequence->NumValues(); ++i)
      activeStimuli.insert(Sequence(i));
    break;

  case SequenceTypes::Random:
    for (int i = 0; i < Sequence->NumValues(); ++i)
      activeStimuli.insert(i + 1);
    break;

  case SequenceTypes::P3Speller:
    for (int i = 0; i < Stimuli->NumColumns(); ++i)
      activeStimuli.insert(i + 1);
    break;
  }
  return activeStimuli;
}

