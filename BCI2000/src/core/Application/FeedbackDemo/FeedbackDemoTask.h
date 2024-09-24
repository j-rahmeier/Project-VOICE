////////////////////////////////////////////////////////////////////////////////
// $Id: FeedbackDemoTask.h 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A demo feedback task.
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
#ifndef FEEDBACK_DEMO_TASK_H
#define FEEDBACK_DEMO_TASK_H

#include "FeedbackTask.h"
#include "TrialStatistics.h"

class FeedbackDemoTask : public FeedbackTask
{
  public:
    FeedbackDemoTask();
    virtual ~FeedbackDemoTask();

  private:
    // Events to be handled by FeedbackTask descendants.
    //  Events triggered by the GenericFilter interface
    void OnPreflight(const SignalProperties &Input) const override;
    void OnInitialize(const SignalProperties &Input) override;
    void OnStartRun() override;
    void OnStopRun() override;
    void OnHalt() override
    {
    }
    //  Events triggered during the course of a trial
    void OnTrialBegin() override;
    void OnTrialEnd() override;
    void OnFeedbackBegin() override;
    void OnFeedbackEnd() override;
    //  Dispatching of the input signal.
    //  Each call to GenericSignal::Process() is dispatched to one of these
    //  events, depending on the phase in the sequence.
    //  There, each handler function corresponds to a phase.
    //  If a handler sets the "progress" argument to true, the application's
    //  state will switch to the next phase.
    void DoPreRun(const GenericSignal &, bool &doProgress) override;
    void DoPreFeedback(const GenericSignal &, bool &doProgress) override;
    void DoFeedback(const GenericSignal &, bool &doProgress) override;
    void DoPostFeedback(const GenericSignal &, bool &doProgress) override;
    void DoITI(const GenericSignal &, bool &doProgress) override;

    void SetLabel(const char *text, RGBColor &color);

  private:
    int mRunCount, mTrialCount;
    float mCursorPosX, mCursorPosY, mCursorSpeedX, mCursorSpeedY;

    TrialStatistics mTrialStatistics;

    class QWidget *mpWindow;
    class QGraphicsScene *mpScene;
    class QGraphicsView *mpSceneView;
    class QGraphicsSimpleTextItem *mpLabel;
    class QGraphicsRectItem *mpTarget;
    class QGraphicsEllipseItem *mpCursor;
};

#endif // FEEDBACK_DEMO_TASK_H
