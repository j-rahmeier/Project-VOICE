////////////////////////////////////////////////////////////////////////////////
// $Id: VisualizationContainerDemoFilter.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: juergen.mellinger@neurotechcenter.org
// Description: A demo that shows how to place multiple visualizations
//   into a single container window.
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
#include "VisualizationContainerDemoFilter.h"
#include "GenericVisualization.h"
#include "Label.h"

#include <iostream>
#include <iomanip>

RegisterFilter(VisualizationContainerDemoFilter, 2.Z);

struct VisualizationContainerDemoFilter::Private
{
    GenericVisualization mContainerVis;

    class VisualizationObject;
    std::vector<VisualizationObject *> mVisualizations;

    ~Private()
    {
        destroyVisualizations();
    }
    // Creates visualization objects for pairs of channels.
    void createVisualizations(const SignalProperties &);
    // Destroys all visualization objects.
    void destroyVisualizations();
    // Sets visualizations to their initial state.
    void resetVisualizations();
    // Computes data values, and updates visualization windows.
    void updateVisualizations(const GenericSignal &);

    class VisualizationObject
    {
      public:
        VisualizationObject(const std::string &visID);
        ~VisualizationObject();
        void setTitle(const std::string &);
        void configureSignal(const SignalProperties&);
        void setPosition(int row, int col);
        void reset();
        void update(const GenericSignal &);

        struct Computation
        {
            int inputCh1, inputCh2;
            double result;
            // run() is called whenever a new signal arrives.
            void run(const GenericSignal &);
        } mComputation;

      private:
        GenericVisualization mVis;
        GenericSignal mSignal;
        PhysicalUnit mSampleUnit, mValueUnit;
        std::string mTitle;
        int mRow, mCol;
    };
};

void VisualizationContainerDemoFilter::Private::createVisualizations(const SignalProperties &Input)
{
    destroyVisualizations();

    int count = 0;
    for (int ch1 = 0; ch1 < Input.Channels(); ++ch1)
    {
        for (int ch2 = ch1; ch2 < Input.Channels(); ++ch2)
        {
            std::ostringstream oss;
            oss << "VisDemo" << std::setw(3) << std::setfill('0') << ++count;
            auto pVis = new VisualizationObject(oss.str());
            pVis->setTitle(Input.ChannelLabels()[ch1] + "<->" + Input.ChannelLabels()[ch2]);
            pVis->configureSignal(Input);
            // Choose position within parent window
            pVis->setPosition(ch1, ch2);

            pVis->mComputation.inputCh1 = ch1;
            pVis->mComputation.inputCh2 = ch2;
            mVisualizations.push_back(pVis);
        }
    }
}

void VisualizationContainerDemoFilter::Private::destroyVisualizations()
{
    for (auto pVis : mVisualizations)
        delete pVis;
    mVisualizations.clear();
}

void VisualizationContainerDemoFilter::Private::resetVisualizations()
{
    for (auto pVis : mVisualizations)
        pVis->reset();
}

void VisualizationContainerDemoFilter::Private::updateVisualizations(const GenericSignal &s)
{
    for (auto pVis : mVisualizations)
        pVis->update(s);
}

VisualizationContainerDemoFilter::Private::VisualizationObject::VisualizationObject(const std::string &visID)
    : mVis(visID),
      mRow(1), mCol(1)
{
    ::memset(&mComputation, 0, sizeof(mComputation));
}

VisualizationContainerDemoFilter::Private::VisualizationObject::~VisualizationObject()
{
    mVis.Send(CfgID::Visible, false);
}

void VisualizationContainerDemoFilter::Private::VisualizationObject::setTitle(const std::string &title)
{
    mTitle = title;
}

void VisualizationContainerDemoFilter::Private::VisualizationObject::configureSignal(const SignalProperties& input)
{
    SignalProperties properties(1, 1);
    mSignal.SetProperties(properties);
    mSampleUnit = input.ElementUnit();
    mValueUnit = PhysicalUnit().SetGain(1).SetOffset(0).SetRawMin(0).SetRawMax(1);
}

void VisualizationContainerDemoFilter::Private::VisualizationObject::setPosition(int row, int col)
{
    mRow = row;
    mCol = col;
}

void VisualizationContainerDemoFilter::Private::VisualizationObject::update(const GenericSignal &Input)
{
    mComputation.run(Input);
    mSignal(0, 0) = mComputation.result;
    mVis.Send(mSignal);
}

void VisualizationContainerDemoFilter::Private::VisualizationObject::Computation::run(const GenericSignal &Input)
{
    // To have something to visualize, compute the squared correlation between channels.
    double p0 = 0, p1_1 = 0, p1_2 = 0, p2_11 = 0, p2_12 = 0, p2_22 = 0;
    for (int i = 0; i < Input.Elements(); ++i)
    {
        double val1 = Input(inputCh1, i), val2 = Input(inputCh2, i);
        p0 += 1;
        p1_1 += val1;
        p1_2 += val2;
        p2_11 += val1 * val1;
        p2_12 += val1 * val2;
        p2_22 += val2 * val2;
    }
    double var1 = p2_11 / p0 - p1_1 * p1_1 / p0 / p0, var2 = p2_22 / p0 - p1_2 * p1_2 / p0 / p0,
           cov12 = p2_12 / p0 - p1_1 * p1_2 / p0 / p0;
    result = cov12 * cov12 / var1 / var2;
}

void VisualizationContainerDemoFilter::Private::VisualizationObject::reset()
{
    // reset position and size
    mVis.Send(CfgID::PlacementVis, "CNTR");
    mVis.Send(CfgID::PlacementRow, mRow + 1); // rows are one-based
    mVis.Send(CfgID::PlacementCol, mCol + 1); // columns are one-based
    mVis.Send(CfgID::Visible, true);
    mVis.Send(CfgID::FixedScale, true);
    mVis.Send(CfgID::MinValue, mValueUnit.RawMin());
    mVis.Send(CfgID::MaxValue, mValueUnit.RawMax() * 1.1);
    mVis.Send(CfgID::ShowStatusBar, false);
    mVis.Send(CfgID::SampleUnit, mSampleUnit.RawToPhysical(1));
    mVis.Send(CfgID::ShowYTicks, false);
    LabelList markers;
    markers.push_back(Label(mValueUnit.PhysicalToRaw("1"), "1|red|-|0.5"));
    markers.push_back(Label(mValueUnit.PhysicalToRaw("0.5"), "0.5|green|-.|0.5"));
    mVis.Send(CfgID::YAxisMarkers, markers);
    mVis.Send(CfgID::WindowTitle, mTitle);
    mVis.Send(CfgID::WindowFrame, true);
    mVis.Send(GenericSignal());
}

// Filter boilerplate code
VisualizationContainerDemoFilter::VisualizationContainerDemoFilter() : p(new Private)
{
    p->mContainerVis.SetVisID("CNTR");
}

VisualizationContainerDemoFilter::~VisualizationContainerDemoFilter()
{
    delete p;
}

void VisualizationContainerDemoFilter::Publish()
{
}

void VisualizationContainerDemoFilter::Preflight(const SignalProperties &Input, SignalProperties &Output) const
{
    Output = Input;
}

void VisualizationContainerDemoFilter::Initialize(const SignalProperties &Input, const SignalProperties &Output)
{
    p->mContainerVis.Send(CfgID::Visible, false);
    p->mContainerVis.Send(CfgID::WindowTitle, "Squared Signal Correlation");
    p->mContainerVis.Send(CfgID::PlacementMargin, 5);
    p->mContainerVis.Send(CfgID::PlacementColor, RGBColor::LtGray);
    p->mContainerVis.Send(CfgID::Height, Input.Channels() * 90);
    p->mContainerVis.Send(CfgID::Width, Input.Channels() * 90);
    p->createVisualizations(Input);
    p->resetVisualizations();
    p->mContainerVis.Send(CfgID::Visible, true);
}

void VisualizationContainerDemoFilter::Process(const GenericSignal &Input, GenericSignal &Output)
{
  p->updateVisualizations(Input);
  Output = Input;
}

void VisualizationContainerDemoFilter::StartRun()
{
    p->resetVisualizations();
}

void VisualizationContainerDemoFilter::StopRun()
{
    p->resetVisualizations();
}
