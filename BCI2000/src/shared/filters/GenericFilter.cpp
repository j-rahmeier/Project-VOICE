////////////////////////////////////////////////////////////////////////////////
// $Id: GenericFilter.cpp 8296 2024-08-01 16:12:02Z mellinger $
// Description: This file declares a purely abstract GenericFilter interface
//   which all BCI2000 filters are supposed to implement.
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
#include "GenericFilter.h"
#include "BCIException.h"
#include "BCIStream.h"
#include "ClassName.h"
#include "StopWatch.h"
#include "SubchainFilter.h"
#include <iomanip>
#include <limits>
#include <sstream>

#undef AutoConfig_

// GenericFilter class definitions.
size_t GenericFilter::Registrar::sCreatedInstances = 0;
static const std::string sEmptyPos = "";
static const char sSeparator = '/';

struct GenericFilter::Private
{
    static thread_local void *stAuxThreadData;
    bool mTimedCalls;
    bool mProfiling;
    struct PerformanceData mPerformanceData;

    Private() : mTimedCalls(false), mProfiling(false)
    {
    }
};
thread_local void *GenericFilter::Private::stAuxThreadData;

GenericFilter::Chain &GenericFilter::RootChain()
{
    static GenericFilter::Chain rootChain(Registrar::Registrars());
    return rootChain;
}

GenericFilter::GenericFilter() : p(new Private)
{
    void *data = Private::stAuxThreadData;
    Directory::Node *pNode = static_cast<Directory::Node *>(data);
    SetParent(pNode);
    AllFilters().push_back(this);
}

GenericFilter::~GenericFilter()
{
    AllFilters().remove(this);
    delete p;
}

bool GenericFilter::TimedCalls() const
{
    return p->mTimedCalls;
}

const struct GenericFilter::PerformanceData &GenericFilter::PerformanceData() const
{
    return p->mPerformanceData;
}

void GenericFilter::EnableProfiling(bool b)
{
    p->mProfiling = b;
}

void GenericFilter::SetAuxThreadData(void *data)
{
    Private::stAuxThreadData = data;
}

// Static members
GenericFilter::FiltersType &GenericFilter::AllFilters()
{
    static FiltersType allFilters;
    return allFilters;
}

Directory::Node *GenericFilter::Directory()
{
    static Directory::RootNode *root = new Directory::RootNode;
    return root;
}

// GenericFilter::Registrar definitions
GenericFilter::Registrar::Registrar(bool inAutoDelete) : mInstance(sCreatedInstances++)
{
    std::ostringstream oss;
    oss << std::hex << std::setfill('0') << std::setw(8) << mInstance;
    mPos = oss.str();
    if (inAutoDelete)
        AutoDeleteInstance().insert(this);
}

GenericFilter::Registrar::Registrar(const char *inPos, int inPriority, bool inAutoDelete)
    : mPos(inPos), mPriority(inPriority), mInstance(sCreatedInstances++)
{
    int maxPriority = inPriority;
    RegistrarSet_::iterator i = Registrars().begin();
    while (i != Registrars().end())
    {
        RegistrarSet_::iterator j = i++;
        // Determine max priority present.
        if ((*j)->mPriority > maxPriority)
            maxPriority = (*j)->mPriority;
        // Remove all registrars with lower priority.
        if ((*j)->mPriority < inPriority)
            Registrars().erase(j);
    }
    // Only insert if priority is high enough.
    if (inPriority >= maxPriority)
        Registrars().insert(this);

    if (inAutoDelete)
        AutoDeleteInstance().insert(this);
}

GenericFilter::Registrar::~Registrar()
{
    Registrars().erase(this);
}

void GenericFilter::Registrar::Clear()
{
    AutoDeleteInstance().Clear();
}

bool GenericFilter::Registrar::less::operator()(const Registrar *a, const Registrar *b) const
{
    if (a->mPos != b->mPos)
        return (a->mPos < b->mPos);
    return (a->mInstance < b->mInstance);
}

GenericFilter::RegistrarSet &GenericFilter::Registrar::Registrars()
{
    static RegistrarSet registrars;
    return registrars;
}

void GenericFilter::Registrar::AutoDeleteSet::Clear()
{
    while (!empty())
    {
        delete *begin();
        erase(begin());
    }
}

GenericFilter::Registrar::AutoDeleteSet::~AutoDeleteSet()
{
    Clear();
}

GenericFilter::Registrar::AutoDeleteSet &GenericFilter::Registrar::AutoDeleteInstance()
{
    static AutoDeleteSet instance;
    return instance;
}

// GenericFilter definitions
const std::string &GenericFilter::GetFirstFilterPosition()
{
    if (Registrar::Registrars().empty())
        return sEmptyPos;
    return (*Registrar::Registrars().begin())->Position();
}

const std::string &GenericFilter::GetLastFilterPosition()
{
    if (Registrar::Registrars().empty())
        return sEmptyPos;
    return (*Registrar::Registrars().rbegin())->Position();
}

// GenericFilter::Chain definitions
std::vector<GenericFilter::Chain::FilterHook*> GenericFilter::Chain::sFilterHooks;


GenericFilter::Chain::Chain(const Registrar::RegistrarSet_ &r) : mRegistrars(r)
{
}

GenericFilter::ChainInfo GenericFilter::Chain::Info()
{
    ChainInfo result;
    for (Registrar::RegistrarSet_::const_iterator i = mRegistrars.begin(); i != mRegistrars.end(); ++i)
    {
        ChainEntry entry;
        entry.position = (*i)->Position();
        entry.name = ClassName((*i)->Typeid());
        result.push_back(entry);
    }
    return result;
}

std::ostream &GenericFilter::ChainInfo::InsertInto(std::ostream &os)
{
    for (const_iterator i = begin(); i != end(); ++i)
        os << "{ " << i->position << " " << i->name << " } ";
    return os;
}

// Wrapper functions for "handler" calls.
#define CALL_BODY_(x, b)                                                                                               \
    ErrorContext(#x, this);                                                                                            \
    try                                                                                                                \
    {                                                                                                                  \
        this->x b;                                                                                                     \
    }                                                                                                                  \
    catch (const BCIException &e)                                                                                      \
    {                                                                                                                  \
        bcierr_ << e.What();                                                                                           \
    }                                                                                                                  \
    catch (BCIStream::OutStream::Ref & r)                                                                              \
    {                                                                                                                  \
        if (!r.RefersTo(bcierr__))                                                                                     \
            throw;                                                                                                     \
    }                                                                                                                  \
    catch (const std::exception &e)                                                                                    \
    {                                                                                                                  \
        bcierr_ << e.what();                                                                                           \
    }                                                                                                                  \
    ErrorContext("");

#define TIMED_CALL_BODY_(x, b)                                                                                         \
    if (TimedCalls())                                                                                                  \
    {                                                                                                                  \
        StopWatch watch_;                                                                                              \
        {                                                                                                              \
            CALL_BODY_(x, b)                                                                                           \
        }                                                                                                              \
        if (watch_.Lapse() > MeasurementUnits::SampleBlockDuration() * 1e3 && bcierr__.Empty())                        \
            bciwarn_ << ClassName(typeid(*this)) << "::" #x ": "                                                       \
                     << "Execution required more than a sample block duration";                                        \
    }                                                                                                                  \
    else                                                                                                               \
    {                                                                                                                  \
        CALL_BODY_(x, b)                                                                                               \
    }

#define CALL_DEF_(x, a, b)                                                                                             \
    void GenericFilter::Call##x a                                                                                      \
    {                                                                                                                  \
        CALL_BODY_(x, b)                                                                                               \
    }

#define CALL_0(x) CALL_DEF_(x, (), ())
#define CALL_1(x, T) CALL_DEF_(x, (T t), (t))
#define CALL_2(x, T, U) CALL_DEF_(x, (T t, U u), (t, u))

CALL_0(Publish)

void GenericFilter::CallAutoConfig(const SignalProperties &Input)
{
    bool prev = AutoConfig_(true);
    CALL_BODY_(AutoConfig, (Input));
    AutoConfig_(prev);
}

void GenericFilter::CallPreflight(const SignalProperties &Input, SignalProperties &Output) const
{
    OptionalParameter("EvaluateTiming");
    CALL_BODY_(Preflight, (Input, Output));
}

void GenericFilter::CallInitialize(const SignalProperties &Input, const SignalProperties &Output)
{
#if !BCIDEBUG
    p->mTimedCalls = (OptionalParameter("EvaluateTiming", 1) != 0);
#endif
    p->mPerformanceData.blockDuration = MeasurementUnits::SampleBlockDuration();
    p->mPerformanceData.count = 0;
    p->mPerformanceData.totalDuration = 0;
    p->mPerformanceData.maxDuration = 0;
    p->mPerformanceData.minDuration = Inf<double>();
    CALL_BODY_(Initialize, (Input, Output));
}

void GenericFilter::CallProcess(const GenericSignal &Input, GenericSignal &Output)
{
    if (p->mProfiling)
    {
        StopWatch profWatch_;
        TIMED_CALL_BODY_(Process, (Input, Output));
        double t = profWatch_.Lapse() * 1e-3;
        ++p->mPerformanceData.count;
        p->mPerformanceData.totalDuration += t;
        p->mPerformanceData.minDuration = std::min(p->mPerformanceData.minDuration, t);
        p->mPerformanceData.maxDuration = std::max(p->mPerformanceData.maxDuration, t);
    }
    else
    {
        TIMED_CALL_BODY_(Process, (Input, Output));
    }
}

void GenericFilter::CallResting(const GenericSignal &Input, GenericSignal &Output)
{
    TIMED_CALL_BODY_(Resting, (Input, Output));
}

CALL_0(StartRun)
CALL_0(StopRun)
CALL_0(Resting)
CALL_0(Halt)

void GenericFilter::Chain::Add(GenericFilter::Registrar *pReg)
{
    mRegistrars.insert(pReg);
}

void GenericFilter::Chain::ClearRegistrars()
{
    mRegistrars.clear();
}

// Instantiate all registered filters once.
// We iterate through the registrars in reverse order. This implies that
// sub-filters acquired via PassFilter<>() must have a position string greater
// than that of their master filter; this appears more intuitive because
// sub-filters may then share the first part of their position
// strings with their master filters.
void GenericFilter::Chain::Instantiate()
{
    float sortingHint = 0;
    for (RegistrarSet::reverse_iterator i = mRegistrars.rbegin(); i != mRegistrars.rend(); ++i)
    {
        std::string filterName = ClassName((*i)->Typeid());
        const std::string &posString = (*i)->Position();
        ErrorContext(filterName + "::Constructor");
        GenericFilter *pFilter = (*i)->NewInstance();
        mOwnedFilters.push_front(pFilter);
        for (auto pHook : sFilterHooks)
            pHook->OnInstantiate(pFilter, posString);
        ErrorContext("");
    }
}

void GenericFilter::Chain::Dispose()
{
    for (FiltersType::iterator i = mOwnedFilters.begin(); i != mOwnedFilters.end(); ++i)
    {
        for (auto pHook : sFilterHooks)
            pHook->OnDispose(*i);
        ErrorContext("Destructor", *i);
        delete *i;
        ErrorContext("");
    }
    mOwnedFilters.clear();
}

void GenericFilter::Chain::OnPublish()
{
    for (FiltersType::iterator i = mOwnedFilters.begin(); i != mOwnedFilters.end(); ++i) {
        (*i)->CallPublish();
        for (auto pHook : sFilterHooks)
            pHook->OnPublish(*i);
    }
}

void GenericFilter::Chain::OnPreflight(const SignalProperties &Input, SignalProperties &Output)
{
    mOwnedSignals[NULL].SetProperties(Input);
    GenericFilter *currentFilter = NULL;
    const SignalProperties *currentInput = &Input;
    for (FiltersType::iterator i = mOwnedFilters.begin(); i != mOwnedFilters.end(); ++i)
    {
        currentFilter = *i;
        int f = bcierr__.Flushes();
        currentFilter->CallAutoConfig(*currentInput);
        if (bcierr__.Flushes() > f)
            return;
        SignalProperties currentOutput;
        currentFilter->CallPreflight(*currentInput, currentOutput);
        if (currentOutput.Name() == currentInput->Name() || currentOutput.Name().empty())
        {
            std::string name = currentFilter->Name();
            std::string prefix = currentFilter->SimplifiedPath<SubchainFilter>();
            if (!prefix.empty())
                name = prefix + sSeparator + name;
            currentOutput.SetName(name);
        }
        for (auto pHook : sFilterHooks)
            pHook->OnPreflight(currentFilter, *currentInput, currentOutput);
        // The output signal will be created here if it does not exist.
        mOwnedSignals[currentFilter].SetProperties(currentOutput);
        currentInput = &mOwnedSignals[currentFilter].Properties();
    }
    Output = mOwnedSignals[currentFilter].Properties();
}

void GenericFilter::Chain::OnInitialize()
{
    const GenericSignal::ValueType NaN = std::numeric_limits<GenericSignal::ValueType>::quiet_NaN();
    const SignalProperties *currentInput = &mOwnedSignals[NULL].Properties();
    GenericFilter *currentFilter = NULL;
    for (FiltersType::iterator i = mOwnedFilters.begin(); i != mOwnedFilters.end(); ++i)
    {
        currentFilter = *i;
        // This will implicitly create the output signal if it does not exist.
        const SignalProperties &currentOutput = mOwnedSignals[currentFilter].Properties();
        currentFilter->CallInitialize(*currentInput, currentOutput);
        for (auto pHook : sFilterHooks)
            pHook->OnInitialize(currentFilter, *currentInput, currentOutput);

        currentInput = &mOwnedSignals[currentFilter].Properties();
    }
}

void GenericFilter::Chain::OnStartRun()
{
    for (FiltersType::iterator i = mOwnedFilters.begin(); i != mOwnedFilters.end(); ++i) {
        (*i)->CallStartRun();
        for (auto pHook : sFilterHooks)
            pHook->OnStartRun(*i);
    }
}

void GenericFilter::Chain::OnProcess(const GenericSignal &Input, GenericSignal &Output, bool inResting)
{
    const GenericSignal *currentInput = &Input;
    GenericFilter *currentFilter = nullptr;
    for (FiltersType::iterator i = mOwnedFilters.begin(); i != mOwnedFilters.end(); ++i)
    {
        currentFilter = *i;
        // This will implicitly create the output signal if it does not exist.
        GenericSignal &currentOutput = mOwnedSignals[currentFilter];
        if (inResting) {
            currentFilter->CallResting(*currentInput, currentOutput);
            for (auto pHook : sFilterHooks)
                pHook->OnResting(currentFilter, *currentInput, currentOutput);
        }
        else {
            currentFilter->CallProcess(*currentInput, currentOutput);
            for (auto pHook : sFilterHooks)
                pHook->OnProcess(currentFilter, *currentInput, currentOutput);
        }

        currentInput = &mOwnedSignals[currentFilter];
    }
    if (currentFilter)
        Output.AssignValues(mOwnedSignals[currentFilter]);
    else
        Output.AssignValues(Input);
}

void GenericFilter::Chain::OnStopRun()
{
    for (FiltersType::iterator i = mOwnedFilters.begin(); i != mOwnedFilters.end(); ++i) {
        (*i)->CallStopRun();
        for (auto pHook : sFilterHooks)
            pHook->OnStopRun(*i);
    }
}

void GenericFilter::Chain::OnResting()
{
    for (FiltersType::iterator i = mOwnedFilters.begin(); i != mOwnedFilters.end(); ++i) {
        (*i)->CallResting();
        for (auto pHook : sFilterHooks)
            pHook->OnResting(*i);
    }
}

void GenericFilter::Chain::OnHalt()
{
    for (FiltersType::iterator i = mOwnedFilters.begin(); i != mOwnedFilters.end(); ++i) {
        (*i)->CallHalt();
        for (auto pHook : sFilterHooks)
            pHook->OnHalt(*i);
    }
}

void GenericFilter::Chain::AddFilterHook(FilterHook* pHook)
{
    auto i = std::find(sFilterHooks.begin(), sFilterHooks.end(), pHook);
    if (i == sFilterHooks.end())
        sFilterHooks.push_back(pHook);
}

void GenericFilter::Chain::RemoveFilterHook(FilterHook* pHook)
{
    auto i = std::find(sFilterHooks.begin(), sFilterHooks.end(), pHook);
    if (i != sFilterHooks.end())
        sFilterHooks.erase(i);
}

// Create an instance of the same type as a given one.
GenericFilter *GenericFilter::NewInstance(const GenericFilter *existingInstance)
{
    Registrar *registrarFound = NULL;
    RegistrarSet::iterator i = Registrar::Registrars().begin();
    while (i != Registrar::Registrars().end() && registrarFound == NULL)
    {
        if (typeid(*existingInstance) == (*i)->Typeid())
            registrarFound = *i;
        ++i;
    }
    GenericFilter *newInstance = NULL;
    if (registrarFound)
        newInstance = registrarFound->NewInstance();
    return newInstance;
}
