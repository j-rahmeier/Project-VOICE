/////////////////////////////////////////////////////////////////////////////
// $Id: TestLogger.h 7545 2023-08-22 13:16:31Z mellinger $
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
#ifndef TEST_LOGGER_H
#define TEST_LOGGER_H

#include "Environment.h"
#include "Thread.h"
#include <cinttypes>

class TestLogger : public EnvironmentExtension, private Thread
{
  public:
    TestLogger();
    ~TestLogger();

    // EnvironmentExtension interface
    void Publish() override;
    void Preflight() const override;
    void Initialize() override;
    void StartRun() override;
    void StopRun() override;
    void Halt() override;

    // Thread interface
    int OnExecute() override;

  private:
    void OnExecuteCountMode();
    void OnExecutePulseMode();

  private:
    double mTestLoggerFrequency;
    enum { int32 = 0, float32 = 1, };
    int mTestLoggerDataType;
    union { uint32_t i; float f; } mTestLoggerCounter;
    bool mTestLoggerEmitDebug;
    enum { count = 0, pulse = 1, };
    int mTestLoggerMode;
};

#endif // TEST_LOGGER_H
