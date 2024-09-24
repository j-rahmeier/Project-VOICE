////////////////////////////////////////////////////////////////////////////////
// $Id: VisualizationDemoFilter.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: juergen.mellinger@neurotechcenter.org
// Description: A demo that shows how to render an image offscreen, and
//   send it to the operator module for display.
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
#include "VisualizationDemoFilter.h"
#include "BitmapImage.h"
#include "GenericVisualization.h"
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

RegisterFilter(VisualizationDemoFilter, 2.Z);

struct VisualizationDemoFilter::Private
{
    Private();
    ~Private();
    double computeCorrelation(const GenericSignal &, int idx1, int idx2);

    RGBColor mBackground;
    int mHeight, mWidth, mDecimation;
    int mDecimationCounter;
    // Object used to send visualization data to the operator
    BitmapVisualization mVis;
#if USE_QT_PAINTER
    QImage mImage;
#else
    // Object that holds image contents
    GUI::GraphDisplay mImage;
    // Objects that populate the image
    PieShape *mpShape;
    TextField *mpValueField;
#endif
};

VisualizationDemoFilter::Private::Private() : mVis("VDMO")
{
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
#endif
}

VisualizationDemoFilter::Private::~Private()
{
    mVis.Send(CfgID::Visible, false);
}

double VisualizationDemoFilter::Private::computeCorrelation(const GenericSignal &Input, int inputCh1, int inputCh2)
{
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
    return cov12 / ::sqrt(var1) / ::sqrt(var2);
}

// Filter boilerplate code
VisualizationDemoFilter::VisualizationDemoFilter() : p(new Private)
{
}

VisualizationDemoFilter::~VisualizationDemoFilter()
{
    delete p;
}

void VisualizationDemoFilter::Publish()
{
    BEGIN_PARAMETER_DEFINITIONS
    "VisualizationDemoFilter int VisImageWidth= 250 % 0 % // native image width",
        "VisualizationDemoFilter int VisImageHeight= 250 % 0 % // native image height",
        "VisualizationDemoFilter int VisImageBackground= 0x808080 % // image background color (color)",
        "VisualizationDemoFilter int VisImageDecimation= 1 % 1 % // update on every nth frame only",
        END_PARAMETER_DEFINITIONS
}

void VisualizationDemoFilter::Preflight(const SignalProperties &Input, SignalProperties &Output) const
{
    Output = Input;
    Parameter("VisImageBackground");
}

void VisualizationDemoFilter::Initialize(const SignalProperties &Input, const SignalProperties &Output)
{
    // Initialize properties from parameters.
    p->mWidth = Parameter("VisImageWidth");
    p->mHeight = Parameter("VisImageHeight");
    p->mBackground = RGBColor(Parameter("VisImageBackground"));
    p->mDecimation = Parameter("VisImageDecimation");
    // Set the image object's width and height.
#if USE_QT_PAINTER
    p->mImage = QImage(p->mWidth, p->mHeight, QImage::Format_RGB16);
#else
    GUI::DrawContext c = {0};
    c.rect.right = p->mWidth;
    c.rect.bottom = p->mHeight;
    p->mImage.SetContext(c);
#endif

    // Set visualization window properties.
    p->mVis.Send(CfgID::Left, 50);
    p->mVis.Send(CfgID::Top, 50);
    p->mVis.Send(CfgID::Width, p->mWidth);
    p->mVis.Send(CfgID::Height, p->mHeight);
    p->mVis.Send(CfgID::WindowTitle, "Visualization Demo");
    // Setting WindowFrame to false will hide title bar and window frame.
    p->mVis.Send(CfgID::WindowFrame, true);
    p->mVis.Send(CfgID::Visible, true);

    // Send an initial reference frame.
#if USE_QT_PAINTER
    QPainter painter(&p->mImage);
    painter.fillRect(p->mImage.rect(), p->mBackground.ToQRgb());
    p->mVis.SendReferenceFrame(p->mImage);
#else
    p->mpValueField->SetVisible(false);
    p->mpShape->SetVisible(false);
    p->mImage.SetColor(p->mBackground);
    p->mImage.Paint();
    p->mVis.SendReferenceFrame(p->mImage.BitmapData());
    p->mpShape->SetVisible(true);
    p->mpValueField->SetVisible(true);
#endif
}

void VisualizationDemoFilter::Process(const GenericSignal &Input, GenericSignal &Output)
{
    Output = Input;
    if (p->mDecimationCounter == 0)
    {
        // For demonstration, compute squared correlation between channel 1 and channel 2.
        double value = p->computeCorrelation(Input, 0, 1);
        value *= value;

        std::ostringstream oss;
        oss << std::setprecision(2) << std::fixed << value;

        // Draw a pie shape that is full angle when value == 1, and that reduces to a line when value == 1.
        // Send the image as a difference frame when done.
#if USE_QT_PAINTER

        QPainter painter(&p->mImage);
        painter.fillRect(p->mImage.rect(), p->mBackground.ToQRgb());
        painter.setBrush(Qt::yellow);
        int angle = 270 * 16;
        if (value == value) // not NaN
            angle = floor(360 * 16 * value + 0.5);
        painter.drawPie(p->mImage.rect(), 180 * 16 - angle / 2, angle);
        QFont font("Arial", 12);
        painter.setFont(font);
        QRect rect = p->mImage.rect();
        painter.drawText(rect.left() + 10, rect.bottom() - font.pixelSize() - 10, oss.str().c_str());

        p->mVis.SendDifferenceFrame(p->mImage);
#else

        p->mpValueField->SetText(oss.str());
        float angle = 270;
        if (value == value) // not NaN
            angle = 360 * value;
        p->mpShape->SetStartAngle(180 - angle / 2).SetEndAngle(180 + angle / 2);
        // Actually render the image.
        p->mImage.Paint();
        // Send the image's bitmap data as a difference frame.
        p->mVis.SendDifferenceFrame(p->mImage.BitmapData());
#endif
    }
    // Update decimation counter.
    ++p->mDecimationCounter %= p->mDecimation;
}

void VisualizationDemoFilter::StartRun()
{
    p->mDecimationCounter = 0;
}

void VisualizationDemoFilter::StopRun()
{
}
