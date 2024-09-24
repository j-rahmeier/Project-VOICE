////////////////////////////////////////////////////////////////////////////////
// $Id: StimulusPresentationTask.h 7308 2023-04-12 15:51:21Z mellinger $
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
#ifndef STIMULUS_PRESENTATION_TASK_H
#define STIMULUS_PRESENTATION_TASK_H

#include "StimulusTask.h"
#include <vector>
#include <set>

class ProgressBarVis;

namespace SequenceTypes
{
enum
{
    Deterministic = 0,
    Random,
    P3Speller,
};
} // namespace SequenceTypes

class StimulusPresentationTask : public StimulusTask
{
  public:
    StimulusPresentationTask();
    ~StimulusPresentationTask();

  protected:
    // StimulusTask events
    void OnAutoConfig(const SignalProperties &) override;
    void OnPreflight(const SignalProperties &Input) const override;
    void OnInitialize(const SignalProperties &Input) override;
    void OnStartRun() override;
    void OnStopRun() override;
    void OnPreSequence() override;
    void OnSequenceBegin() override;
    void OnPostRun() override;
    Target *OnClassResult(const ClassResult &) override;
    int OnNextStimulusCode() override;

    void DoPreSequence(const GenericSignal &, bool &doProgress) override;
    void DoPostSequence(const GenericSignal &, bool &doProgress) override;

    void Process(const GenericSignal&, GenericSignal&) override;

  private:
    void DetermineAttendedTarget();
    ParamRef StimulusMode(const ParamRef& inMatrixParam, int inColIndex, const std::string& inRowLabelRegex, int inRowIndex) const;
    ParamRef StimulusProperty(const ParamRef &inMatrixParam, int inColIndex, const std::string &inPropertyName) const;
    // The set contains stimulus codes rather than stimulus indices (+1)
    std::set<int> DetermineActiveStimuli() const;

    // Configuration parameters.
    int mNumberOfSequences, mSequenceType;

    // Internal state.
    int mPreSequenceBlockCount, mPostResultBlockCount, mSequenceCount;

    std::vector<int> mToBeCopied;
    std::vector<int>::const_iterator mToBeCopiedPos;

    std::vector<int> mSequence;
    std::vector<int>::const_iterator mSequencePos;
    ProgressBarVis* mpProgressBar;

    // Sets of stimuli
    SetOfStimuli mStimuli;
    Association mFocusAnnouncement, mResultAnnouncement;
    // Set of all existing targets.
    SetOfTargets mTargets;

    mutable std::set<std::string> mStimulusModeWarningsReported;
};

#endif // STIMULUS_PRESENTATION_TASK_H
