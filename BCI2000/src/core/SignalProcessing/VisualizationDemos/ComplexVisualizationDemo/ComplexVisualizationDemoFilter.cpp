////////////////////////////////////////////////////////////////////////////////
// $Id: ComplexVisualizationDemoFilter.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: juergen.mellinger@neurotechcenter.org
// Description: A more complex demo that shows how to render multiple images
//   into offscreen canvases, and display them inside an operator window.
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
#include "ComplexVisualizationDemoFilter.h"
#include "BitmapImage.h"
#include "GenericVisualization.h"
#include "WorkerThread.h"
#include "gui/GUI.h"
#include <iomanip>

/*
BCI2000 GraphDisplay vs. QPainter rendering

GraphDisplay is a layer of abstraction that allows to render shapes and text objects into
a normalized coordinate system.
Code that uses GraphDisplay is most likely to survive breaking changes in the
drawing backend (currently Qt) and BCI2000 dependencies.

In contrast, QPainter rendering provides access to more complex drawing functions
but suffers from a limitation in Qt which makes text rendering impossible outside
the main GUI thread.
*/

// enable the following line to switch to QPainter rendering
//#define USE_QT_PAINTER 1

#if USE_QT_PAINTER
#include <QPainter>
#else
#include "gui/GraphDisplay.h"
#include "gui/Shapes.h"
#include "gui/TextField.h"
#endif

RegisterFilter(ComplexVisualizationDemoFilter, 2.Z);

struct ComplexVisualizationDemoFilter::Private
{
    RGBColor mBackground;
    int mHeight, mWidth, mDecimation;
    int mDecimationCounter;

    GenericVisualization mContainerVis;

    class VisualizationObject;
    std::vector<VisualizationObject *> mVisualizations;

    ~Private()
    {
        destroyVisualizations();
    }
    // Creates visualization objects for pairs of channels.
    void createVisualizations(const SignalProperties &, int maxWindows);
    // Destroys all visualization objects.
    void destroyVisualizations();
    // Sets visualizations to their initial state.
    void resetVisualizations();
    // Computes data values, and asynchronously updates visualization bitmaps.
    void updateVisualizations(const GenericSignal &);
    // Waits for asynchronous activity in all visualizations to terminate.
    void waitForVisualizations();

    class VisualizationObject
    {
      public:
        VisualizationObject(const Private *, const std::string &visID);
        ~VisualizationObject();
        void setTitle(const std::string &, const std::string &info = "");
        void setPosition(int row, int col, int width, int height);
        void reset();
        void update(const GenericSignal &);
        void wait();

        struct Computation
        {
            int inputCh1, inputCh2;
            double result;
            // run() is called whenever a new signal arrives.
            void run(const GenericSignal &);
        } mComputation;

      private:
        void asyncUpdate();

        MemberCall<void(VisualizationObject *)> mCallAsyncUpdate;

        const Private *p;
        WorkerThread mWorker;
        BitmapVisualization mVis;
        std::string mTitle, mInfo;
        int mRow, mCol;
#if USE_QT_PAINTER
        QImage mImage;
#else
        GUI::GraphDisplay mImage;
        PieShape *mpShape;
        TextField *mpValueField, *mpInfoField;
#endif
    };
};

void ComplexVisualizationDemoFilter::Private::createVisualizations(const SignalProperties &Input, int maxWindows)
{
    destroyVisualizations();

    int count = 0;
    for (int ch1 = 0; ch1 < Input.Channels(); ++ch1)
    {
        for (int ch2 = ch1; ch2 < Input.Channels(); ++ch2)
        {
            std::ostringstream oss;
            oss << "VisDemo" << std::setw(3) << std::setfill('0') << ++count;
            auto pVis = new VisualizationObject(this, oss.str());
            pVis->setTitle("Vis Demo (" + Input.ChannelLabels()[ch1] + "<->" + Input.ChannelLabels()[ch2] + ")",
                           Input.ChannelLabels()[ch1] + "\n" + Input.ChannelLabels()[ch2]);
            // Choose position within parent window
            pVis->setPosition(ch1 + 1, ch2 + 1, mWidth, mHeight);

            pVis->mComputation.inputCh1 = ch1;
            pVis->mComputation.inputCh2 = ch2;
            mVisualizations.push_back(pVis);

            if (maxWindows > 0 && count >= maxWindows)
                break;
        }
    }
}

void ComplexVisualizationDemoFilter::Private::destroyVisualizations()
{
    for (auto pVis : mVisualizations)
    {
        pVis->wait();
        delete pVis;
    }
    mVisualizations.clear();
}

void ComplexVisualizationDemoFilter::Private::resetVisualizations()
{
    for (auto pVis : mVisualizations)
        pVis->reset();
}

void ComplexVisualizationDemoFilter::Private::updateVisualizations(const GenericSignal &s)
{
    for (auto pVis : mVisualizations)
        pVis->update(s);
}

void ComplexVisualizationDemoFilter::Private::waitForVisualizations()
{
    for (auto pVis : mVisualizations)
        pVis->wait();
}

ComplexVisualizationDemoFilter::Private::VisualizationObject::VisualizationObject(const Private *p,
                                                                                  const std::string &visID)
    : p(p), mVis(visID),
      mCallAsyncUpdate(&VisualizationObject::asyncUpdate, this),
      mRow(1), mCol(1)
{
    ::memset(&mComputation, 0, sizeof(mComputation));
#if !USE_QT_PAINTER
    mpShape = new PieShape(mImage);
    mpShape->SetPositionX(0.5).SetPositionY(0.5);
    mpShape->SetWidth(1).SetHeight(1);
    mpShape->SetLineWidth(1);
    mpShape->SetColor(RGBColor::Black);
    mpShape->SetFillColor(RGBColor::Yellow);

    mpValueField = new TextField(mImage);
    mpValueField->SetPositionX(0.5).SetPositionY(0.8);
    mpValueField->SetWidth(1).SetHeight(0.2);
    mpValueField->SetColor(RGBColor::None);
    mpValueField->SetTextColor(RGBColor::Black);

    mpInfoField = new TextField(mImage);
    mpInfoField->SetPositionX(0.5).SetPositionY(0.5).SetAlignment(GUI::Alignment::Center);
    mpInfoField->SetWidth(0).SetScalingMode(GUI::ScalingMode::AdjustWidth);
    mpInfoField->SetTextHeight(0.45).SetHeight(0.5);
    mpInfoField->SetColor(RGBColor::White);
    mpInfoField->SetTextColor(RGBColor::Black);
#endif
}

ComplexVisualizationDemoFilter::Private::VisualizationObject::~VisualizationObject()
{
    mVis.Send(CfgID::Visible, false);
}

void ComplexVisualizationDemoFilter::Private::VisualizationObject::setTitle(const std::string &title,
                                                                            const std::string &info)
{
    mTitle = title;
    mInfo = info.empty() ? title : info;
}

void ComplexVisualizationDemoFilter::Private::VisualizationObject::setPosition(int row, int col, int width, int height)
{
#if USE_QT_PAINTER
    mImage = QImage(width, height, QImage::Format_RGB16);
#else
    GUI::DrawContext c = {0};
    c.rect.right = width;
    c.rect.bottom = height;
    mImage.SetContext(c);
#endif
    mRow = row;
    mCol = col;
}

void ComplexVisualizationDemoFilter::Private::VisualizationObject::update(const GenericSignal &Input)
{
    mComputation.run(Input);
    mWorker.Run(mCallAsyncUpdate); // will fail if worker still busy
}

void ComplexVisualizationDemoFilter::Private::VisualizationObject::wait()
{
    mWorker.Wait();
}

void ComplexVisualizationDemoFilter::Private::VisualizationObject::Computation::run(const GenericSignal &Input)
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

void ComplexVisualizationDemoFilter::Private::VisualizationObject::reset()
{
    // reset position and size
    mVis.Send(CfgID::PlacementVis, "CPLX");
    mVis.Send(CfgID::PlacementRow, mRow);
    mVis.Send(CfgID::PlacementCol, mCol);
    mVis.Send(CfgID::WindowFrame, false); // hide title
    mVis.Send(CfgID::Visible, true);

    // paint an empty image
#if USE_QT_PAINTER
    QPainter painter(&mImage);
    painter.fillRect(mImage.rect(), p->mBackground.ToQRgb());
    mVis.SendReferenceFrame(mImage);
#else
    mpInfoField->SetText(mInfo);
    mpInfoField->SetVisible(true);
    mpValueField->SetVisible(false);
    mpShape->SetVisible(false);
    mImage.SetColor(p->mBackground);
    mImage.Paint();
    mVis.SendReferenceFrame(mImage.BitmapData());
    mpShape->SetVisible(true);
    mpValueField->SetVisible(true);
    mpInfoField->SetVisible(false);
#endif
}

void ComplexVisualizationDemoFilter::Private::VisualizationObject::asyncUpdate()
{
    double value = mComputation.result;

#if USE_QT_PAINTER

    QPainter painter(&mImage);
    painter.fillRect(mImage.rect(), p->mBackground.ToQRgb());
    painter.setBrush(Qt::yellow);
    int angle = 270 * 16;
    if (value == value) // not NaN
        angle = floor(360 * 16 * value + 0.5);
    painter.drawPie(mImage.rect(), 180 * 16 - angle / 2, angle);

    mVis.SendDifferenceFrame(mImage);
#else

    std::ostringstream oss;
    oss << std::setprecision(2) << std::fixed << value;
    mpValueField->SetText(oss.str());

    // Draw a pie shape that is full angle when value == 1, and that reduces to a line when value == 1.
    float angle = 270;
    if (value == value) // not NaN
        angle = 360 * value;
    mpShape->SetStartAngle(180 - angle / 2).SetEndAngle(180 + angle / 2);
    // render the image
    mImage.Paint();
    // send bitmap data to operator
    mVis.SendDifferenceFrame(mImage.BitmapData());
#endif
}

// Filter boilerplate code
ComplexVisualizationDemoFilter::ComplexVisualizationDemoFilter() : p(new Private)
{
    p->mContainerVis.SetVisID("CPLX");
}

ComplexVisualizationDemoFilter::~ComplexVisualizationDemoFilter()
{
    delete p;
}

void ComplexVisualizationDemoFilter::Publish()
{
    BEGIN_PARAMETER_DEFINITIONS
    "ComplexVisualizationDemoFilter int VisImageWidth= 50 % 0 % // native image width",
        "ComplexVisualizationDemoFilter int VisImageHeight= 50 % 0 % // native image height",
        "ComplexVisualizationDemoFilter int VisImageBackground= 0x808080 % // image background color (color)",
        "ComplexVisualizationDemoFilter int VisImageDecimation= 1 % 1 % // update on every nth frame only",
        "ComplexVisualizationDemoFilter int VisMaxWindows= 0 % 0 % // maximum number of windows, 0 for any",
        END_PARAMETER_DEFINITIONS
}

void ComplexVisualizationDemoFilter::Preflight(const SignalProperties &Input, SignalProperties &Output) const
{
    Output = Input;
    Parameter("VisImageBackground");
}

void ComplexVisualizationDemoFilter::Initialize(const SignalProperties &Input, const SignalProperties &Output)
{
    p->mContainerVis.Send(CfgID::Visible, false);
    p->mContainerVis.Send(CfgID::WindowTitle, "Signal Correlation");
    p->mContainerVis.Send(CfgID::PlacementMargin, 0);
    p->mContainerVis.Send(CfgID::PlacementColor, RGBColor::Black);
    p->mWidth = Parameter("VisImageWidth");
    p->mHeight = Parameter("VisImageHeight");
    p->mContainerVis.Send(CfgID::Height, Input.Channels() * p->mHeight);
    p->mContainerVis.Send(CfgID::Width, Input.Channels() * p->mWidth);
    p->mBackground = RGBColor(Parameter("VisImageBackground"));
    p->mDecimation = Parameter("VisImageDecimation");
    p->createVisualizations(Input, Parameter("VisMaxWindows"));
    p->resetVisualizations();
    p->mContainerVis.Send(CfgID::Visible, true);
}

void ComplexVisualizationDemoFilter::Process(const GenericSignal &Input, GenericSignal &Output)
{
    Output = Input;
    if (p->mDecimationCounter == 0)
        p->updateVisualizations(Input);
    ++p->mDecimationCounter %= p->mDecimation;
}

void ComplexVisualizationDemoFilter::StartRun()
{
    p->mDecimationCounter = 0;
    p->waitForVisualizations();
    p->resetVisualizations();
}

void ComplexVisualizationDemoFilter::StopRun()
{
    p->waitForVisualizations();
    p->resetVisualizations();
}
