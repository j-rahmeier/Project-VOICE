/* $BEGIN_BCI2000_LICENSE$
 *
 * This file is part of BCI2000, a platform for real-time bio-signal research.
 * [ Copyright (C) 2000-2023: BCI2000 team and many external contributors ]
 *
 * BCI2000 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * BCI2000 is distributed in the hope that it will be useful, but
 *                         WITHOUT ANY WARRANTY
 * - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_BCI2000_LICENSE$
/*/
#include "GenericFilter.h"

#define REPORTFUNCTION                                                                                                 \
    {                                                                                                                  \
        ReportProcessCount();                                                                                          \
        bciout << "entered function " __FUNC__;                                                                        \
    }

class DebugFilter : public GenericFilter
{
  public:
    DebugFilter();
    ~DebugFilter();
    void Preflight(const SignalProperties &, SignalProperties &) const override;
    void Initialize(const SignalProperties &, const SignalProperties &) override;
    void StartRun() override;
    void StopRun() override;
    void Process(const GenericSignal &Input, GenericSignal &Output) override;
    void Resting() override;
    void Halt() override;

  private:
    void ReportProcessCount() const;
    mutable unsigned long mProcessCount;
};

RegisterFilter(DebugFilter, 2.Y);

DebugFilter::DebugFilter() : mProcessCount(0)
{
    REPORTFUNCTION;
}

DebugFilter::~DebugFilter()
{
    REPORTFUNCTION;
}

void DebugFilter::Preflight(const SignalProperties &inSignalProperties, SignalProperties &outSignalProperties) const
{
    bciout << "Input Signal Properties are " << inSignalProperties;
    // bcierr << "Reporting a bogus error";
    outSignalProperties = inSignalProperties;
}

void DebugFilter::Initialize(const SignalProperties &inSignalProperties, const SignalProperties &outSignalProperties)
{
    REPORTFUNCTION;
}

void DebugFilter::StartRun()
{
    REPORTFUNCTION;
}

void DebugFilter::StopRun()
{
    REPORTFUNCTION;
}

void DebugFilter::Process(const GenericSignal &input, GenericSignal &output)
{
    ++mProcessCount;
    output = input;
}

void DebugFilter::Resting()
{
    REPORTFUNCTION;
}

void DebugFilter::Halt()
{
    REPORTFUNCTION;
}

void DebugFilter::ReportProcessCount() const
{
    if (mProcessCount > 0)
    {
        bciout__ << "DebugFilter::Process called " << mProcessCount << " times";
    }
    mProcessCount = 0;
}
