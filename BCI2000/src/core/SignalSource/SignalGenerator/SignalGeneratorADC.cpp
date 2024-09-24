////////////////////////////////////////////////////////////////////////////////
// $Id: SignalGeneratorADC.cpp 8336 2024-09-03 11:09:29Z mellinger $
// Author: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: An ADC class for testing purposes.
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
#include "SignalGeneratorADC.h"

#include "BCIStream.h"
#include "GenericSignal.h"

#if _WIN32
#include <Windows.h>
#elif USE_QT
#include <QApplication>
#include <QCursor>
#include <QDesktopWidget>
#endif

// Register the source class with the framework.
RegisterFilter(SignalGeneratorADC, 1);

SignalGeneratorADC::SignalGeneratorADC()
    : mRandomGenerator(this), mNoiseAmplitude(0), mDCOffset(0),
      mSineChannelX(0), mSineChannelY(0), mSineChannelZ(0),
      mModulateAmplitude(true), mVariateTiming(false), mDisableRandomNoise(false),
      mAmplitudeX(1), mAmplitudeY(1), mAmplitudeZ(1),
      mHaveMouseLog(false), mDesktopWidth(0), mDesktopHeight(0)
{
}

SignalGeneratorADC::~SignalGeneratorADC()
{
}

void SignalGeneratorADC::Publish()
{
  BEGIN_PARAMETER_DEFINITIONS
    "Source:Signal%20Properties int SourceCh= 16 "
        "16 1 % // number of digitized and stored channels",
    "Source:Signal%20Properties int SampleBlockSize= 32 "
        "32 1 % // number of samples transmitted at a time",
    "Source:Signal%20Properties int SamplingRate= 256Hz "
        "256Hz 1 % // sample rate",

    "Source list SourceChOffset= 1 auto % % %",
    "Source list SourceChGain= 1 auto % % %",
    "Source list ChannelNames= 1 auto % % %",

    "Source int ModulateAmplitude= 0 0 0 1 "
        "// Modulate the amplitude with the mouse (0=no, 1=yes) (boolean)",
    "Source int SineChannelX= 0 0 0 % "
        "// Channel number of sinewave controlled by mouse x position",
    "Source int SineChannelY= 0 0 0 % "
        "// Channel number of sinewave controlled by mouse y position (0 for all)",
    "Source int SineChannelZ= 0 0 0 % "
        "// Channel number of sinewave controlled by mouse key state",
#if _WIN32
    "Source list NaNChannels= 1 auto % % % "
        "// Channel numbers set to NaN if space is pressed (empty if not used)",
#endif

    "Source float SineFrequency= 10Hz 10Hz % % "
        "// Frequency of sine wave",
    "Source int SineAmplitude= 100muV 100muV % % "
        "// Amplitude of sine wave",
    "Source int NoiseAmplitude= 30muV 30muV % % "
        "// Amplitude of white noise (common to all channels)",
    "Source int DisableRandomNoise= 0 0 0 1 "
        "// disable random noise generation (boolean)",
    "Source int DCOffset= 0muV 0muV % % "
        "// DC offset (common to all channels)",
    "Source string AmplitudeMultiplier= 1 1 % % "
        "// Expression to multiply amplitude by",
    "Source string OffsetMultiplier= % StimulusType % % "
        "// Expression to multiply offset by",
    "Source int SignalType= 0 0 0 2 "
        "// numeric type of output signal: "
        " 0: int16,"
        " 1: float32,"
        " 2: int32 "
        "(enumeration)",

    "Source matrix SourceProperties= 0 [ Frequency Amplitude ] // Source properties",
    "Source matrix MixingMatrix= 0 1 // Source-to-sensor projection, rows are sources, columns are sensors",
    "Source float VariateTiming= 0 0 0 0.9 // Introduce random variation into block duration",
  END_PARAMETER_DEFINITIONS

  BEGIN_STREAM_DEFINITIONS
    "SourceChannel1 32 0",
  END_STREAM_DEFINITIONS
}

void SignalGeneratorADC::AutoConfig(const SignalProperties &)
{
    ParamRef MixingMatrix = Parameter("MixingMatrix");
#if _WIN32
    Parameter("NaNChannels")->SetNumValues(0);
#endif
    if (MixingMatrix->NumValues() > 0)
    {
        int numChannels = MixingMatrix->NumColumns();
        Parameter("SourceCh") = numChannels;
        if (!MixingMatrix->ColumnLabels().IsTrivial())
        {
            Parameter("ChannelNames")->SetNumValues(numChannels);
            for (int i = 0; i < numChannels; ++i)
                Parameter("ChannelNames")(i) = MixingMatrix->ColumnLabels()[i];
        }
    }
    int numChannels = Parameter("SourceCh");
    Parameter("SourceChOffset")->SetNumValues(numChannels);
    Parameter("SourceChGain")->SetNumValues(numChannels);
    for (int i = 0; i < numChannels; ++i)
    {
        Parameter("SourceChOffset")(i) = 0;
        Parameter("SourceChGain")(i) = "0.1muV";
    }
}

void SignalGeneratorADC::Preflight(const SignalProperties &, SignalProperties &Output) const
{
#if 0 // allow for units other than V
    for (int ch = 0; ch < Output.Channels(); ++ch)
    {
        PhysicalUnit u = Output.ValueUnit(ch);
        u.SetRawMin(u.PhysicalToRaw("-100muV")).SetRawMax(u.PhysicalToRaw("100muV"));
    }
#endif
    Parameter("SamplingRate").InHertz();
    Parameter("SineFrequency").InHertz();
    Parameter("SineAmplitude").InVolts();
    Parameter("NoiseAmplitude").InVolts();
    if (Parameter("DCOffset").InVolts() != 0)
        Expression(Parameter("OffsetMultiplier")).Evaluate();
    Expression(Parameter("AmplitudeMultiplier")).Evaluate();
    Parameter("RandomSeed");

    if (Parameter("MixingMatrix")->NumValues())
        PreflightCondition(Parameter("SourceCh") == Parameter("MixingMatrix")->NumColumns());
    Parameter("SourceProperties");
    
    OptionalState("MousePosX");
    OptionalState("MousePosY");
    OptionalState("MouseKeys");
    OptionalParameter("DesktopRect");
    if ((bool)Parameter("ModulateAmplitude"))
    {
      if (Parameter("SineChannelX") < 0 || Parameter("SineChannelX") > Parameter("SourceCh"))
        bcierr << "SineChannelX " << std::to_string((int)Parameter("SineChannelX")) << "  out of range";
      if (Parameter("SineChannelY") < 0 || Parameter("SineChannelY") > Parameter("SourceCh"))
        bcierr << "SineChannelY " << std::to_string((int)Parameter("SineChannelY")) << "  out of range";
      if (Parameter("SineChannelZ") < 0 || Parameter("SineChannelZ") > Parameter("SourceCh"))
        bcierr << "SineChannelZ " << std::to_string((int)Parameter("SineChannelZ")) << "  out of range";
    }
#if _WIN32
    if (Parameter("NaNChannels")->NumValues() > Parameter("SourceCh"))
        bcierr << "More NaNChannels than source channels defined!";

    for (int i = 0; i < Parameter("NaNChannels")->NumValues(); i++)
        if (Parameter("NaNChannels")(i) < 1 || Parameter("NaNChannels")(i) > Parameter("SourceCh"))
            bcierr << "NaNChannel " << std::to_string((int)Parameter("NaNChannels")(i)) << "  out of range";
#endif

    SignalType signalType;
    switch (int(Parameter("SignalType")))
    {
    case 0:
        signalType = SignalType::int16;
        break;
    case 1:
        signalType = SignalType::float32;
        break;
    case 2:
        signalType = SignalType::int32;
        break;
    default:
        bcierr << "Unknown SignalType value";
    }
    Output.SetType(signalType);
}

void SignalGeneratorADC::Initialize(const SignalProperties &, const SignalProperties &Output)
{
    double SineFrequency = Parameter("SineFrequency").InHertz() / Parameter("SamplingRate").InHertz(),
           SineAmplitude = Parameter("SineAmplitude").InMicrovolts() * 1e-6;
    mNoiseAmplitude = Parameter("NoiseAmplitude").InMicrovolts() * 1e-6;
    mDisableRandomNoise = Parameter("DisableRandomNoise");
    if (::fabs(mNoiseAmplitude) < Eps(mNoiseAmplitude))
        mDisableRandomNoise = true;
    mDCOffset = Parameter("DCOffset").InMicrovolts() * 1e-6;
    if (mDCOffset == 0)
        mOffsetMultiplier = Expression("");
    else
        mOffsetMultiplier = Expression(Parameter("OffsetMultiplier"));
    mAmplitudeMultiplier = Expression(Parameter("AmplitudeMultiplier"));
    mHaveMouseLog = States->Exists("MousePosX") && States->Exists("MousePosY") && States->Exists("MouseKeys");
    if (Parameters->Exists("DesktopRect") && Parameter("DesktopRect")->NumValues() > 3)
    {
        mDesktopHeight = OptionalParameter("DesktopRect")(2);
        mDesktopWidth = OptionalParameter("DesktopRect")(3);
    }
    mModulateAmplitude = (Parameter("ModulateAmplitude") != 0);
    mSineChannelX = Parameter("SineChannelX");
    mSineChannelY = Parameter("SineChannelY");
    mSineChannelZ = Parameter("SineChannelZ");
    mAmplitudeX = 1.0;
    mAmplitudeY = 1.0;
    mAmplitudeZ = 1.0;

    mMixingMatrix.clear();
    if (Parameter("SourceProperties")->NumValues() > 0)
    {
        ParamRef MixingMatrix = Parameter("MixingMatrix");
        mMixingMatrix.resize(MixingMatrix->NumColumns(), Vector(MixingMatrix->NumRows()));
        for (int i = 0; i < MixingMatrix->NumRows(); ++i)
            for (int j = 0; j < MixingMatrix->NumColumns(); ++j)
                mMixingMatrix[j][i] = MixingMatrix(i, j);

        ParamRef SourceProperties = Parameter("SourceProperties");
        mSourceFrequencies.resize(SourceProperties->NumRows());
        mSourceAmplitudes.resize(SourceProperties->NumRows());
        mSourcePhases.resize(SourceProperties->NumRows());
        for (int i = 0; i < SourceProperties->NumRows(); ++i)
        {
            mSourceFrequencies[i] = SourceProperties(i, "Frequency").InHertz() / Parameter("SamplingRate").InHertz();
            mSourceAmplitudes[i] = SourceProperties(i, "Amplitude").InMicrovolts() * 1e-6;
        }
    }
    else
    {
        int numCh = Parameter("SourceCh"), numSrc = 4;
        Vector normalChannel(numSrc);
        normalChannel[0] = 1;
        mMixingMatrix.resize(numCh, normalChannel);
        if (mModulateAmplitude)
        {
            if (mSineChannelX)
            {
                mMixingMatrix[mSineChannelX - 1][0] = 0;
                mMixingMatrix[mSineChannelX - 1][1] = 1;
                mSineChannelX = 2;
            }
            if (mSineChannelY)
            {
                mMixingMatrix[mSineChannelY - 1][0] = 0;
                mMixingMatrix[mSineChannelY - 1][2] = 1;
                mSineChannelY = 3;
            }
            if (mSineChannelZ)
            {
                mMixingMatrix[mSineChannelZ - 1][0] = 0;
                mMixingMatrix[mSineChannelZ - 1][3] = 1;
                mSineChannelZ = 4;
            }
        }
        mSourceFrequencies.resize(numSrc, SineFrequency);
        mSourceAmplitudes.resize(numSrc, SineAmplitude);
        mSourcePhases.resize(numSrc, 0);
    }

    mVariateTiming = Parameter("VariateTiming");

    mClock.SetInterval(Time::Seconds(MeasurementUnits::SampleBlockDuration()));
    mClock.Start();
}

void SignalGeneratorADC::StartRun()
{
    if (Parameter("RandomSeed") != 0)
        for (size_t i = 0; i < mSourcePhases.size(); ++i)
            mSourcePhases[i] = 0;
}

void SignalGeneratorADC::Process(const GenericSignal &, GenericSignal &Output)
{
    auto* pDataOut = Output.MutableData();

    WithThreadPriority(ThreadUtils::Priority::Maximum - 1) mClock.Wait();
    if (mVariateTiming)
        while (mRandomGenerator() < mRandomGenerator.RandMax() * mVariateTiming)
            ThreadUtils::SleepForMs(1);

    if (mModulateAmplitude)
    {
#if _WIN32
        POINT p = { 0, 0 };
        if (::GetCursorPos(&p))
        {
            int width = ::GetSystemMetrics(SM_CXVIRTUALSCREEN), height = ::GetSystemMetrics(SM_CYVIRTUALSCREEN);
            mAmplitudeX = float(p.x) / width;
            mAmplitudeY = 1.0 - float(p.y) / height;
        }
        enum
        {
            isPressed = 0x8000
        };
        bool leftButton = ::GetAsyncKeyState(VK_LBUTTON) & isPressed,
            rightButton = ::GetAsyncKeyState(VK_RBUTTON) & isPressed;
        mAmplitudeZ = 0.5 + (leftButton ? -0.5 : 0) + (rightButton ? 0.5 : 0);
#elif USE_QT
        QPoint p = QCursor::pos();
        QRect r = QApplication::desktop()->geometry();
        mAmplitudeX = float(p.x()) / r.width();
        mAmplitudeY = 1.0 - float(p.y()) / r.height();
#else
        if (mHaveMouseLog)
        {
            const int cMouseOffset = (1 << 15), cMouseLeftButton = 1, cMouseRightButton = 2;
            int mousePosX = State("MousePosX"), mousePosY = State("MousePosY");
            if (mousePosX == 0 && mousePosY == 0)
            {
                mAmplitudeX = 1.0;
                mAmplitudeY = 1.0;
            }
            else
            {
                mAmplitudeX = (mousePosX - cMouseOffset) / mDesktopWidth;
                mAmplitudeY = 1.0 - (mousePosY - cMouseOffset) / mDesktopHeight;
            }
            int mouseKeys = State("MouseKeys");
            mAmplitudeZ = 0.5;
            if (mouseKeys & cMouseLeftButton)
                mAmplitudeZ -= 0.5;
            if (mouseKeys & cMouseRightButton)
                mAmplitudeZ += 0.5;
        }
#endif // !_WIN32, !USE_QT
    }

    double offset = mDCOffset;
    if (offset != 0)
        offset *= mOffsetMultiplier.Evaluate();

#pragma omp parallel for
    for (int ch = 0; ch < Output.Channels(); ++ch)
        for (int sample = 0; sample < Output.Elements(); ++sample)
            pDataOut[Output.LinearIndex(ch, sample)] = offset;

    for (int sample = 0; sample < Output.Elements(); ++sample)
    {
        mSourcePhases += mSourceFrequencies;
        mSourcePhases -= mSourcePhases.apply(floor);
        mSourceValues = sin(mSourcePhases * (2 * Pi())) * mSourceAmplitudes * mAmplitudeMultiplier.Evaluate();
        if (mModulateAmplitude)
        {
            if (mSineChannelX > 0)
                mSourceValues[mSineChannelX - 1] *= mAmplitudeX;
            if (mSineChannelY > 0)
                mSourceValues[mSineChannelY - 1] *= mAmplitudeY;
            else if (mSineChannelY == 0)
                mSourceValues *= mAmplitudeY;
            if (mSineChannelZ > 0)
                mSourceValues[mSineChannelZ - 1] *= mAmplitudeZ;
        }
        for (int ch = 0; ch < Output.Channels(); ++ch)
            pDataOut[Output.LinearIndex(ch, sample)] +=
                (mMixingMatrix[ch] * mSourceValues).sum();

        if (!mDisableRandomNoise)
          for (int ch = 0; ch < Output.Channels(); ++ch)
            pDataOut[Output.LinearIndex(ch, sample)] +=
            (mRandomGenerator.Random() * mNoiseAmplitude / mRandomGenerator.RandMax() - mNoiseAmplitude / 2);
    }

#pragma omp parallel for
    for (int ch = 0; ch < Output.Channels(); ++ch)
      for (int sample = 0; sample < Output.Elements(); ++sample)
      {
          GenericSignal::ValueType &value = pDataOut[Output.LinearIndex(ch, sample)];
          value = Output.Properties().ValueUnit(ch).PhysicalToRawValue(value);
          value = std::max(value, Output.Type().Min());
          value = std::min(value, Output.Type().Max());
      }

    if (Output.Channels() > 0)
    {
        StateRef SourceChannel1 = State("SourceChannel1");
        for (int sample = 0; sample < Output.Elements(); ++sample)
        {
            auto value = pDataOut[Output.LinearIndex(0, sample)];
            SourceChannel1.Sample(sample) = value + (1ul << 31);
        }
    }

#if _WIN32
    if (::GetAsyncKeyState(VK_SPACE) & 0x80000000)
    {
        for (int i = 0; i < Parameter("NaNChannels")->NumValues(); ++i)
            for (int sample = 0; sample < Output.Elements(); ++sample)
                Output(Parameter("NaNChannels")(i) - 1, sample) = GenericSignal::NaN;
    }
#endif
}

void SignalGeneratorADC::Halt()
{
    mClock.Stop();
}
