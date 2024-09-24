/////////////////////////////////////////////////////////////////////////////
// $Id: TestLogger.cpp 7795 2024-01-19 13:42:22Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A logger component designed for stress-testing the BCI2000
//   bcievent mechanism, and thread-safe bcistreams.
//   To activate it, set the TestLoggerFrequency parameter from the command
//   line:
//   SignalGenerator --TestLoggerFrequency=1kHz
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
/////////////////////////////////////////////////////////////////////////////
#include "TestLogger.h"

#include "BCIEvent.h"
#include "BCIStream.h"

Extension(TestLogger);

TestLogger::TestLogger()
: mTestLoggerFrequency(0), mTestLoggerEmitDebug(true),
  mTestLoggerDataType(TestLogger::int32),
  mTestLoggerMode(TestLogger::count)
{
    mTestLoggerCounter.i = 0;
}

TestLogger::~TestLogger()
{
    Halt();
}

void TestLogger::Publish()
{
    bool enabled = (OptionalParameter("TestLoggerFrequency", 0) > 0);
    if (enabled)
    {
      BEGIN_PARAMETER_DEFINITIONS
        "Source float /TestLoggerFrequency= 256Hz % % % // Frequency of emission for bcievents",
        "Source int   TestLoggerEmitDebug= 1 1 0 1 // Emit bcidbg messages (boolean)",
        "Source int   TestLoggerDataType= 0 0 0 1 // Data type of logger counter: 0: int32, 1: float32 (enumeration)",
        "Source int   TestLoggerMode= 0 0 0 1 // Logger mode: 0: count, 1: pulse (enumeration)",
      END_PARAMETER_DEFINITIONS

      BEGIN_EVENT_DEFINITIONS
         "TestLoggerCounter 32 4294967295 0 0",
      END_EVENT_DEFINITIONS
    }
}

void TestLogger::Preflight() const
{
    float TestLoggerFrequency = OptionalParameter("TestLoggerFrequency", 0).InHertz();
    PreflightCondition(TestLoggerFrequency >= 0);
    if (TestLoggerFrequency > 1000)
      bciwarn << "Test logger period is too small for system accuracy, frequency will be around 1kHz";
}

void TestLogger::Initialize()
{
    mTestLoggerFrequency = OptionalParameter("TestLoggerFrequency", 0).InHertz();
    if (mTestLoggerFrequency > 0)
    {
        mTestLoggerEmitDebug = (Parameter("TestLoggerEmitDebug") == 1);
        mTestLoggerDataType = Parameter("TestLoggerDataType");
        mTestLoggerMode = Parameter("TestLoggerMode");
        if (mTestLoggerMode == pulse)
            State("TestLoggerCounter") = 0;
    }
}

void TestLogger::StartRun()
{
    if (mTestLoggerFrequency > 0)
    {
        if (mTestLoggerMode == pulse)
            State("TestLoggerCounter") = 0;
        Thread::Start();
    }
}

void TestLogger::StopRun()
{
    if (mTestLoggerFrequency > 0)
    {
        Thread::Terminate();
        if (mTestLoggerMode == pulse)
            State("TestLoggerCounter") = 0;
    }
}

void TestLogger::Halt()
{
    Thread::Terminate();
}

int TestLogger::OnExecute()
{
    switch (mTestLoggerMode)
    {
        case count:
            OnExecuteCountMode();
            break;
        case pulse:
            OnExecutePulseMode();
            break;
        default:
            throw bcierr << "Unknown value of TestLoggerMode: " << mTestLoggerMode;
    }
    return 0;
}

void TestLogger::OnExecuteCountMode()
{
    if (mTestLoggerEmitDebug)
    {
        double value = (mTestLoggerDataType == float32 ? mTestLoggerCounter.f : mTestLoggerCounter.i);
        bcidbg(0) << "Started logging, counter is " << value;
    }

    while (!Thread::Terminating())
    {
        PrecisionTime wakeupTime = PrecisionTime::Now() + static_cast<int>(1000 / mTestLoggerFrequency);
        bcievent << "TestLoggerCounter " << mTestLoggerCounter.i << std::flush;
        if (mTestLoggerEmitDebug)
        {
            double value = (mTestLoggerDataType == float32 ? mTestLoggerCounter.f : mTestLoggerCounter.i);
            bcidbg(0) << "Logging, counter is " << value;
        }
        mTestLoggerDataType == float32 ? ++mTestLoggerCounter.f : ++mTestLoggerCounter.i;
        ThreadUtils::SleepUntil(wakeupTime);
    }

    if (mTestLoggerEmitDebug)
    {
        double value = (mTestLoggerDataType == float32 ? mTestLoggerCounter.f : mTestLoggerCounter.i);
        bcidbg(0) << "Finished logging, counter is " << value;
    }
}

void TestLogger::OnExecutePulseMode()
{
    while (!Thread::Terminating())
    {
        PrecisionTime wakeupTime = PrecisionTime::Now() + static_cast<int>(1000 / mTestLoggerFrequency);
        bcievent << "TestLoggerCounter 1 0" << std::flush;
        if (mTestLoggerEmitDebug)
            bcidbg(0) << "Logging, pulsed TestLoggerCounter";
        ThreadUtils::SleepUntil(wakeupTime);
    }
}
