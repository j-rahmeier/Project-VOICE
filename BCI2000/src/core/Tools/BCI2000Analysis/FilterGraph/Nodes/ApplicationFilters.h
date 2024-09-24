////////////////////////////////////////////////////////////////////////////////
// $Id: ApplicationFilters.h 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Filters representing application modules.
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
#ifndef APPLICATION_FILTERS_H
#define APPLICATION_FILTERS_H

#include "GenericFilter.h"
#include <iostream>

class ApplicationFilter : public GenericFilter
{
protected:
    ApplicationFilter() : mEnabled(false) {}

public:
    void setEnabled(bool b) { mEnabled = true; }
    bool enabled() const { return mEnabled; }
    virtual void Report(std::ostream&) const {}

private:
    bool mEnabled;
};

class Application : public ApplicationFilter
{
protected:
    void Preflight(const SignalProperties&, SignalProperties&) const override {}
    void Initialize(const SignalProperties&, const SignalProperties&) override {}
    void Process(const GenericSignal&, GenericSignal&) override {}
};

class SMRApplication : public ApplicationFilter
{
protected:
    void Preflight(const SignalProperties&, SignalProperties&) const override {}
    void Initialize(const SignalProperties&, const SignalProperties&) override {}
    void Process(const GenericSignal&, GenericSignal&) override {}
};

class ERPApplication : public ApplicationFilter
{
public:
    ERPApplication();
    ~ERPApplication();

protected:
    void Publish() override;
    void AutoConfig(const SignalProperties&) override;
    void Preflight(const SignalProperties&, SignalProperties&) const override;
    void Initialize(const SignalProperties&, const SignalProperties&) override;
    void Process(const GenericSignal&, GenericSignal&) override;

public:
    void Report(std::ostream&) const override;

private:
    struct Private;
    Private* p;
};

#endif // APPLICATION_FILTERS_H
