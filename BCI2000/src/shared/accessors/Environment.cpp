////////////////////////////////////////////////////////////////////////////////
// $Id: Environment.cpp 8346 2024-09-12 17:18:04Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Environment is a mix-in base class
//   that channels access to enviroment-like
//   global objects of types ParamList, StateList,
//   StateVector, and provides convenient accessor functions which perform
//   checks for parameter/state existence, and bounds.
//
//   Inheriting from Environment, EnvironmentExtension is an interface class
//   for components that need to handle system-wide events such as Preflight,
//   Initialize, etc. Direct inheritance from Environment should be used
//   for components that need access to parameters and states without handling
//   global events.
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
#include "Environment.h"

#include "BCIEvent.h"
#include "BCIException.h"
#include "BCIStream.h"
#include "Directory.h"
#include "FileUtils.h"
#include "MeasurementUnits.h"
#include "ParamList.h"
#include "PhysicalUnit.h"
#include "RequiredParameters.h"
#include "RunManager.h"
#include "StateList.h"
#include "SubchainFilter.h"

#include <algorithm>
#include <cstdlib>

static std::string DescribeParameterEntry(const Param &inParam, size_t inIdx1, size_t inIdx2)
{
    std::ostringstream oss;
    oss << "Parameter \"" << inParam.Section() << "->" << inParam.Name() << "\": Value";

    if (std::string(inParam.Type()).find("matrix") != std::string::npos)
        oss << " at index (" << inIdx1 + 1 << ", " << inIdx2 + 1 << ")";
    else if (std::string(inParam.Type()).find("list") != std::string::npos)
        oss << " at index " << inIdx1 + 1;

    return oss.str();
}

static bool IsAutoConfigParam(const Param &p)
{
    enum
    {
        autoType = 0,
        autoTag,
        autoKind,
        count
    };
    static const char *s[][count] = {
        {"blob", 0},
        {"auto", "AutoConfig", 0},
        {"(enumeration)", 0},
    };
    struct
    {
        bool Is(int type, const char *str)
        {
            const char **p = s[type];
            while (*p)
                if (!::stricmp(*p++, str))
                    return true;
            return false;
        }
        bool Contains(int type, const char *str)
        {
            while (*str)
                if (Is(type, str++))
                    return true;
            return false;
        }
    } strings;

    bool result = strings.Is(autoType, p.Type().c_str());
    if (p.NumValues() == 1)
        result = result || strings.Is(autoTag, p.Value().c_str());
    if (p.NumValues() > 0 && !::atoi(p.Value().c_str()))
        result =
            result || (strings.Contains(autoKind, p.Comment().c_str()) && strings.Contains(autoTag, p.Comment().c_str()));
    return result;
}

////////////////////////////////////////////////////////////////////////////////
// EnvironmentBase::Context definitions
////////////////////////////////////////////////////////////////////////////////
EnvironmentBase::Context *EnvironmentBase::Context::GlobalInstance()
{
    static struct Global : EnvironmentBase::Context
    {
        Global()
        {
            mGlobal = true;
        }
    } sGlobal;
    return &sGlobal;
}

static thread_local EnvironmentBase::Context* stpCurrentContext;

EnvironmentBase::Context *EnvironmentBase::Context::CurrentInstance()
{
    return stpCurrentContext;
}

void EnvironmentBase::Context::SetCurrentInstance(Context *pInstance)
{
    stpCurrentContext = pInstance;
}

EnvironmentBase::Context::Context()
    : mPhase(nonaccess), mpParameters(nullptr), mpStates(nullptr), mpStatevector(nullptr), mGlobal(false)
{
}

void EnvironmentBase::Context::EnterPhase(ExecutionPhase inPhase, ParamList *inParamlist, StateList *inStates,
                                          StateVector *inStatevector)
{
    ExecutionPhase prevPhase = mPhase;
    mPhase = inPhase;
    if (mPhase != nonaccess)
    {
        stpCurrentContext = this;
        mpParameters = inParamlist;
        mpStates = inStates;
        mpStatevector = inStatevector;
    }

#define TRANSITION(x, y) (((x) << 8) | (y))
    switch (TRANSITION(prevPhase, inPhase))
    {
    case TRANSITION(nonaccess, construction):
        mOwnedParams.clear();
        mOwnedStates.clear();
        if (mGlobal)
        {
            bcierr__.SetAction(BCIStream::LogicError);
            BCIStream::Apply(*mpParameters);
        }
#ifdef TODO
#error temporary hack until Localization class has been moved elsewhere
#endif
        mEarlyPublished.clear();
        for (auto p : mExtensions)
        {
            if (ClassName(typeid(*p)).find("Localization") != std::string::npos)
            {
                p->CallPublish();
                mEarlyPublished.insert(p);
            }
        }
        break;

    case TRANSITION(construction, nonaccess):
        break;

    case TRANSITION(nonaccess, publication):
        for (auto p : mExtensions)
            if (mEarlyPublished.find(p) == mEarlyPublished.end())
                p->CallPublish();
        break;

    case TRANSITION(publication, nonaccess):
        break;

    case TRANSITION(nonaccess, preflight):
        if (mGlobal)
        {
            bcierr__.SetAction(BCIStream::ConfigurationError);
            BCIStream::Apply(*mpParameters);
        }
        mParamsRangeChecked.clear();
        if (mpParameters)
        {
            NameSet notAutoConfig;
            for (const auto& p : *mpParameters)
                if (!IsAutoConfigParam(p))
                    notAutoConfig.insert(mpParameters.load()->OriginalPath(&p));
            RangeCheckParams(mpParameters, notAutoConfig);
        }
        mParamsAccessedDuringPreflight.clear();
        mStatesAccessedDuringPreflight.clear();

        for (auto p : mExtensions)
        {
            p->CallHalt2();
            p->CallAutoConfig();
            p->CallPreflight();
        }
        break;

    case TRANSITION(preflight, nonaccess):
        break;

    case TRANSITION(nonaccess, initialization):
        if (mGlobal)
            bcierr__.SetAction(BCIStream::RuntimeError);
        for (auto p : mExtensions)
            p->CallInitialize();
        break;

    case TRANSITION(initialization, nonaccess):
        for (auto p : mExtensions)
            p->CallPostInitialize();
        break;

    case TRANSITION(nonaccess, startRun):
        if (mGlobal)
        {
            bcierr__.SetAction(BCIStream::RuntimeError);
            BCIEvent::AllowEvents();
        }
        for (auto p : mExtensions)
            p->CallStartRun();
        break;

    case TRANSITION(startRun, nonaccess):
        for (auto p : mExtensions)
            p->CallPostStartRun();
        break;

    case TRANSITION(nonaccess, processing):
        if (mGlobal)
            bcierr__.SetAction(BCIStream::RuntimeError);
        for (auto p : mExtensions)
            p->CallProcess();
        break;

    case TRANSITION(processing, nonaccess):
        for (auto p : mExtensions)
            p->CallPostProcess();
        break;

    case TRANSITION(nonaccess, stopRun):
        if (mGlobal)
            bcierr__.SetAction(BCIStream::RuntimeError);
        for (int i = 0; i < inParamlist->Size(); ++i)
            inParamlist->ByIndex(i).Unchanged();
        for (auto p : mExtensions)
            p->CallStopRun();
        break;

    case TRANSITION(stopRun, nonaccess):
        for (auto p : mExtensions)
            p->CallPostStopRun();
        if (mGlobal)
        {
            BCIEvent::DenyEvents();
            bcierr__.SetAction(BCIStream::LogicError);
            bciwarn__.SetAction(BCIStream::LogicError);
            bciout__.SetAction(BCIStream::LogicError);
            bcidbg__.SetAction(BCIStream::LogicError);
        }
        break;

    case TRANSITION(nonaccess, resting):
        if (mGlobal)
            bcierr__.SetAction(BCIStream::RuntimeError);
        for (int i = 0; i < inParamlist->Size(); ++i)
            inParamlist->ByIndex(i).Unchanged();
        for (auto p : mExtensions)
            p->CallResting();
        break;

    case TRANSITION(resting, nonaccess):
        break;

    case TRANSITION(nonaccess, destruction):
        for (auto p : mExtensions)
            p->CallHalt();
        if (mGlobal)
        {
            EnvironmentExtension::OnExit();
            bcierr__.SetAction(0);
            bciwarn__.SetAction(0);
            bciout__.SetAction(0);
            bcidbg__.SetAction(0);
        }
        break;

    case TRANSITION(destruction, nonaccess):
        break;

    default:
        bcierr << "Unexpected transition: " << prevPhase << "->" << inPhase;
    }
    if (inPhase == nonaccess)
    {
        stpCurrentContext = nullptr;
        mpParameters = nullptr;
        mpStates = nullptr;
        mpStatevector = nullptr;
        if (mGlobal)
        {
            bcierr__.SetAction(BCIStream::LogicError);
            bciwarn__.SetAction(BCIStream::Warning);
            bciout__.SetAction(BCIStream::PlainMessage);
            bcidbg__.SetAction(BCIStream::DebugMessage);
        }
    }
}

void EnvironmentBase::Context::RangeCheckParams(const ParamList *inParamList, const Environment::NameSet &inCheck)
{ // iterate over all filters
    for (auto i = mOwnedParams.begin(); i != mOwnedParams.end(); ++i)
    {
        std::vector<std::string> inters(inCheck.size());
        std::vector<std::string>::iterator inters_end = set_intersection(
            inCheck.begin(), inCheck.end(), i->second.begin(), i->second.end(), inters.begin(), Param::NameCmp());
        // iterate over intersection for this filter
        for (std::vector<std::string>::const_iterator j = inters.begin(); j != inters_end; ++j)
        {
            const Param &p = inParamList->ByPath(*j);
            const std::string &lowRangeStr = p.LowRange(), &highRangeStr = p.HighRange();
            bool checkLowRange = (!lowRangeStr.empty()), checkHighRange = (!highRangeStr.empty());
            if (checkLowRange)
            {
                double lowRange = ::atof(lowRangeStr.c_str());
                for (int j = 0; j < p.NumRows(); ++j)
                    for (int k = 0; k < p.NumColumns(); ++k)
                    {
                        double value = ::atof(p.Value(j, k).c_str());
                        if (value < lowRange)
                            bcierr__ << DescribeParameterEntry(p, j, k) << " is " << value << ", exceeds lower range ("
                                     << lowRange << ")";
                    }
            }
            if (checkHighRange)
            {
                double highRange = ::atof(highRangeStr.c_str());
                for (int j = 0; j < p.NumRows(); ++j)
                    for (int k = 0; k < p.NumColumns(); ++k)
                    {
                        double value = ::atof(p.Value(j, k).c_str());
                        if (value > highRange)
                            bcierr__ << DescribeParameterEntry(p, j, k) << " is " << value << ", exceeds high range ("
                                     << highRange << ")";
                    }
            }
            if (checkLowRange || checkHighRange)
                mParamsRangeChecked.insert(*j);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// EnvironmentBase definitions
////////////////////////////////////////////////////////////////////////////////
#undef AutoConfig_
int EnvironmentBase::sMaxInstanceID = 0;
static thread_local const EnvironmentBase *stObjectContext;

Environment::ExecutionPhase EnvironmentBase::Phase()
{
    EnvironmentBase::Context *p = Context::CurrentInstance();
    if (!p)
        p = Context::GlobalInstance();
    return p->mPhase;
}

EnvironmentBase::Context *EnvironmentBase::InitializeContext(EnvironmentBase::Context *p)
{
    if (!p)
        p = Context::CurrentInstance();
    if (!p)
        p = Context::GlobalInstance();
    return p;
}

// Constructor
EnvironmentBase::EnvironmentBase(Context *pContext)
    : mpContext(InitializeContext(pContext)), mInstance(++sMaxInstanceID), mAutoConfig(false),
      Parameters(this, mpContext), States(this, mpContext), Statevector(this, mpContext)
{
    Assert(mpContext);
}

// Copy constructor, and assignment
EnvironmentBase::EnvironmentBase(const EnvironmentBase &other)
    : mpContext(InitializeContext(nullptr)), mInstance(++sMaxInstanceID), mAutoConfig(false),
      Parameters(this, mpContext), States(this, mpContext), Statevector(this, mpContext)
{
    Assert(mpContext);
}

EnvironmentBase &EnvironmentBase::operator=(const EnvironmentBase &other)
{
    mpContext = InitializeContext(other.mpContext);
    mAutoConfig = other.mAutoConfig;
    mAutoConfigParams = other.mAutoConfigParams;
    mTemporaryParams = other.mTemporaryParams;
    return *this;
}

// Destructor
EnvironmentBase::~EnvironmentBase()
{
}

// Helper function to construct and set a context string for displaying errors.
void EnvironmentBase::ErrorContext(const std::string &inQualifier, const EnvironmentBase *inpObject)
{
    stObjectContext = inpObject;
    std::string context;
    if (inpObject != NULL)
    {
        context += ClassName(typeid(*inpObject));
        context += "::";
    }
    context += inQualifier;
    BCIStream::OutStream::SetContext(context);
}

// Maintaining an object context to keep track of Parameter/State access.
const EnvironmentBase *EnvironmentBase::ObjectContext() const
{
    return stObjectContext ? static_cast<const EnvironmentBase *>(stObjectContext) : this;
}

Environment::NameSet &EnvironmentBase::ParamsRangeChecked() const
{
    return mpContext->mParamsRangeChecked;
}

Environment::NameSetMap &EnvironmentBase::OwnedParams() const
{
    return mpContext->mOwnedParams;
}

Environment::NameSetMap &EnvironmentBase::ParamsAccessedDuringPreflight() const
{
    return mpContext->mParamsAccessedDuringPreflight;
}

Environment::NameSetMap &EnvironmentBase::OwnedStates() const
{
    return mpContext->mOwnedStates;
}

Environment::NameSetMap &EnvironmentBase::StatesAccessedDuringPreflight() const
{
    return mpContext->mStatesAccessedDuringPreflight;
}

// Convenient accessor functions.

// Read/write access to a parameter by its name.
// Use an additional pair of brackets for indices.
MutableParamRef EnvironmentBase::Parameter(const std::string &inName)
{
    Param *pParam = ParamAccess(inName);
    return MutableParamRef(pParam);
}

ParamRef EnvironmentBase::Parameter(const std::string &inName) const
{
    return ParamRef(ParamAccess(inName));
}

ParamRef EnvironmentBase::ActualParameter(const std::string &inName) const
{
    return ParamRef(ParamAccess(inName, actual));
}

MutableParamRef EnvironmentBase::OptionalParameter(const std::string &inName, const std::string &inDefaultValue)
{
    Param *pParam = ParamAccess(inName, optional);
    if (!pParam)
    {
        mTemporaryParams.ByPath(inName).Value() = inDefaultValue;
        pParam = &mTemporaryParams.ByPath(inName);
    }
    return MutableParamRef(pParam);
}

ParamRef EnvironmentBase::OptionalParameter(const std::string &inName, const std::string &inDefaultValue) const
{
    return const_cast<EnvironmentBase *>(this)->OptionalParameter(inName, inDefaultValue);
}

MutableParamRef EnvironmentBase::OptionalParameter(const std::string &inName, double inDefaultValue)
{
    std::ostringstream oss;
    oss << inDefaultValue;
    return OptionalParameter(inName, oss.str());
}

ParamRef EnvironmentBase::OptionalParameter(const std::string &inName, double inDefaultValue) const
{
    return const_cast<EnvironmentBase *>(this)->OptionalParameter(inName, inDefaultValue);
}

std::string EnvironmentBase::DescribeValue(const Param &inParam, size_t inIdx1, size_t inIdx2)
{
    return DescribeParameterEntry(inParam, inIdx1, inIdx2);
}

Param *EnvironmentBase::ParamAccess(const std::string &inName, int inFlags) const
{
    Param *pParam = 0;
    if (Parameters == 0)
        bcierr_ << "Attempted parameter access during non-access phase.";
    else
    {
        bool required = bci::IsRequiredParameter(inName);
        if (required)
        {
            if (Parameters->Exists("/" + inName))
                pParam = &Parameters->ByPath("/" + inName);
        }
        else
        {
            auto pNode = dynamic_cast<const Directory::Node *>(this);
            std::string parent;
            if (pNode)
                parent = pNode->Path() + "/";
            else
                parent = "/" + ClassName(typeid(*this)) + "/";
            if (Parameters->Exists(parent + inName))
                pParam = &Parameters->ByPath(parent + inName);
            else if (Parameters->Exists(inName))
                pParam = &Parameters->ByPath(inName);
        }
    }
    if (!pParam && !(inFlags & optional))
        bcierr_ << "Parameter \"" << inName << "\" does not exist.";

    if (pParam)
    {
        std::string path = Parameters->OriginalPath(pParam);
        if (Phase() == preflight)
            mpContext->mParamsAccessedDuringPreflight[ObjectContext()].insert(path);
        OnParamAccess(path);
    }
    if (mAutoConfig && pParam)
    {
        std::string path = Parameters->OriginalPath(pParam);
        bool inAutoSet = (mAutoConfigParams.find(path) != mAutoConfigParams.end()), mayWrite = inAutoSet;
        // Ugly hack to make AutoConfig work for ApplicationBase descendants with ApplicationWindow parameters.
        inAutoSet = inAutoSet || path.find("/ApplicationWindow/") == 0;

        const NameSet &mOwnedParams = mpContext->mOwnedParams[ObjectContext()];
        if (mOwnedParams.find(path) == mOwnedParams.end())
        {
            if (!inAutoSet)
                bcierr_ << "Parameter " << path << " is inaccessible. "
                        << "From AutoConfig(), a filter may only access its own parameters.";
            mAutoConfigParams.insert(path);
            mayWrite = false;
        }
        if (!mayWrite && !mTemporaryParams.Exists(path))
        {
            mayWrite = IsAutoConfigParam(*pParam);
            if (mayWrite)
                mAutoConfigParams.insert(path);
            else
                mTemporaryParams.ByPath(path) = *pParam;
        }
        if (!mayWrite && !(inFlags & actual))
            pParam = &mTemporaryParams.ByPath(path);
    }
    return pParam;
}

bool EnvironmentBase::AutoConfig_(bool inAutoConfig)
{
    bool result = mAutoConfig;
    if (mAutoConfig && !inAutoConfig)
        mpContext->RangeCheckParams(Parameters, mAutoConfigParams);
    mAutoConfigParams.clear();
    mTemporaryParams.Clear();
    mAutoConfig = inAutoConfig;
    return result;
}

bool EnvironmentBase::PreflightCondition_(const char *inConditionString, bool inConditionValue) const
{
    if (!inConditionValue)
        bcierr_ << "A precondition is violated. "
                << "Please make sure that the following is true: " << inConditionString;
    return inConditionValue;
}

// Read/write access to a state by its name.
StateRef EnvironmentBase::State(const std::string &inName) const
{
    const class State *pState = NULL;
    const class StateList *pStatelist = StateListAccess();

    if (pStatelist != NULL)
    {
        if (!pStatelist->Exists(inName))
            bcierr_ << "State \"" << inName << "\" is inaccessible.";
        else
        {
            StateAccess(inName);
            pState = &pStatelist->ByName(inName);
            if (pState->Length() < 1)
                bcierr_ << "State \"" << inName << "\" has zero length.";
        }
    }
    return StateRef(pState, Statevector, 0);
}

// Read access to an optional state by its name.
StateRef EnvironmentBase::OptionalState(const std::string &inName, State::ValueType inDefaultValue) const
{
    StateAccess(inName);

    const class State *pState = NULL;
    StateVector *pStatevector = NULL;
    const StateList *pStatelist = StateListAccess();

    if (pStatelist != NULL && pStatelist->Exists(inName))
    {
        pState = &pStatelist->ByName(inName);
        pStatevector = Statevector;
        if (pState->Length() < 1)
            bcierr_ << "State \"" << inName << "\" has zero length.";
    }
    return StateRef(pState, pStatevector, 0, inDefaultValue);
}

const StateList *EnvironmentBase::StateListAccess() const
{
    const StateList *pStatelist = NULL;
    if (Statevector != NULL)
        pStatelist = &Statevector->StateList();
    else
        pStatelist = States;

    if (pStatelist == NULL)
    {
        const char *pMsg = "States are inaccessible from this thread at this time.";
        SuggestDebugging << pMsg;
        bcierr_ << pMsg;
    }

    return pStatelist;
}

void EnvironmentBase::StateAccess(const std::string &inName) const
{
    NameSet &accessedStates = StatesAccessedDuringPreflight()[ObjectContext()];
    if (Phase() == preflight)
        accessedStates.insert(inName);
    OnStateAccess(inName);
}

// Publish() helper functions
void EnvironmentBase::AddParameters(const std::string *inParams, size_t inCount) const
{
    auto pNode = dynamic_cast<const Directory::Node *>(this);
    std::string nodeName(pNode ? pNode->Name() : ClassName(typeid(*this)));
    std::string nodePath(pNode ? pNode->Path() : "/" + ClassName(typeid(*this)) + "/");
    std::string simplifiedNodePath = (pNode ? pNode->SimplifiedPath<SubchainFilter>() : "");
    for (size_t i = 0; i < inCount; ++i)
    {
        Param p;
        std::istringstream iss(inParams[i]);
        if (!(iss >> p))
            bcierr << "error in parameter definition:\n" << inParams[i];
        else
        {
            if (bci::IsRequiredParameter(p.Name()) && p.ParentPath().empty())
                p.SetParentPath("/");
            if (p.ParentPath().empty())
            {
                if (p.ParentScope())
                {
                    std::string truncatedNodePath = nodePath;
                    size_t pos = truncatedNodePath.rfind('/');
                    if (pos != std::string::npos)
                        truncatedNodePath = truncatedNodePath.substr(0, pos);
                    p.SetParentPath(truncatedNodePath);
                }
                else
                {
                    p.SetParentPath(nodePath);
                }
            }
            auto &sections = p.Sections();
            while (sections.size() < 2)
                sections.push_back(EncodedString(nodeName));
            sections.resize(2);
            if (!simplifiedNodePath.empty())
                sections[0] = sections[0] + ": " + simplifiedNodePath;
            const Param *realParam = nullptr;
            if (Parameters->Exists(p.Name()) && Parameters->ByPath(p.Name()).AllowOverride())
            {
                Parameters->ByPath(p.Name()).AssignValues(p);
                realParam = &Parameters->ByPath(p.Name());
            }
            else
            {
                if (Parameters->Exists("/" + p.Name()))
                    p.AssignValues(Parameters->ByPath("/" + p.Name()));
                if (Parameters->Exists(p.Path()))
                    p.AssignValues(Parameters->ByPath(p.Path()));
                Parameters->Add(p, -Instance());
                realParam = &Parameters->ByPath(p.Path());
            }
            bcidbg(10) << "Registered parameter " << realParam->Name() << ", "
                       << "sorting by (" << -Instance() << "," << realParam->Sections() << ")";
            OwnedParams()[ObjectContext()].insert(Parameters->OriginalPath(realParam));
        }
    }
};

void EnvironmentBase::AddStates(const std::string *inStates, size_t inCount, int inKind) const
{
    struct
    {
        const char *operator()(int k)
        {
            switch (k)
            {
            case State::StateKind:
                return "state";
            case State::EventKind:
                return "event";
            case State::StreamKind:
                return "stream";
            }
            return "unknown state kind";
        }
    } KindString;

    for (size_t i = 0; i < inCount; ++i)
    {
        class State s;
        if (!s.FromDefinition(inStates[i]))
            bcierr << "error in " << KindString(inKind) << " definition:\n" << inStates[i];
        else
        {
            s.SetKind(inKind);
            if (States->Exists(s.Name()))
            {
                const class State &existing = States->ByName(s.Name());
                if (existing.Kind() != inKind)
                    bcierr << "trying to define " << KindString(inKind) << " " << existing.Name()
                           << ", has been previously defined as " << KindString(existing.Kind());
                else
                    States->SetStateValue(s.Name(), s.Value());
            }
            else
                States->Add(s);
            OwnedStates()[ObjectContext()].insert(s.Name());
        }
    }
};

////////////////////////////////////////////////////////////////////////////////
// Environment definitions
////////////////////////////////////////////////////////////////////////////////
RunManager Environment::sRunManager;

RunManager* Environment::RunManager() const
{
    return &sRunManager;
}

void Environment::OnParamAccess(const std::string &inPath) const
{
    NameSet &accessedParams = ParamsAccessedDuringPreflight()[ObjectContext()];
    switch (EnvironmentBase::Phase())
    {
    case construction:
    case publication:
    case preflight:
        break;

    default:
        if (accessedParams.find(inPath) == accessedParams.end() &&
            ParamsRangeChecked().find(inPath) == ParamsRangeChecked().end())
            bcierr_ << "Parameter \"" << inPath << "\" was accessed during"
                    << " initialization or processing, but not checked for"
                    << " consistency during preflight phase.";
    }
}

const std::vector<std::string>& Environment::Enablers() const
{
    return mEnablers;
}

void Environment::PublishEnabler(const std::string& enabler)
{
    if (std::find(mEnablers.begin(), mEnablers.end(), enabler) == mEnablers.end())
        mEnablers.push_back(enabler);
}

void Environment::OnStateAccess(const std::string &inName) const
{
    NameSet &accessedStates = StatesAccessedDuringPreflight()[ObjectContext()],
            &ownedStates = OwnedStates()[ObjectContext()];
    switch (EnvironmentBase::Phase())
    {
    case construction:
    case publication:
    case preflight:
        break;

    default:
        if (accessedStates.find(inName) == accessedStates.end() && ownedStates.find(inName) == ownedStates.end())
            bcierr_ << "State \"" << inName << "\" was accessed during"
                    << " initialization or processing, but not checked for"
                    << " existence during preflight phase.";
    }
}

////////////////////////////////////////////////////////////////////////////////
// EnvironmentExtension definitions
////////////////////////////////////////////////////////////////////////////////
EnvironmentExtension::EnvironmentExtension()
{
    EnvironmentContext()->RegisterExtension__(this);
}

EnvironmentExtension::~EnvironmentExtension()
{
    EnvironmentContext()->UnregisterExtension(this);
}

namespace
{
struct ExtensionSet : std::set<EnvironmentExtension *>
{
};
ExtensionSet& AutoDeleteSet()
{
    static ExtensionSet instance;
    return instance;
}
} // namespace

EnvironmentExtension *EnvironmentExtension::AutoDelete(EnvironmentExtension *p)
{
    AutoDeleteSet().insert(p);
    return p;
}

void EnvironmentExtension::OnExit()
{
    for (ExtensionSet::iterator i = AutoDeleteSet().begin(); i != AutoDeleteSet().end(); ++i)
        delete *i;
    AutoDeleteSet().clear();
}

#define CALL_BODY_(x)                                                                                                  \
    ErrorContext(#x, this);                                                                                            \
    try                                                                                                                \
    {                                                                                                                  \
        this->x();                                                                                                     \
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
    ErrorContext("");

void EnvironmentExtension::CallAutoConfig()
{
    bool prev = AutoConfig_(true);
    CALL_BODY_(AutoConfig);
    AutoConfig_(prev);
}

#define CALL(x, y)                                                                                                     \
    void EnvironmentExtension::Call##x() y                                                                             \
    {                                                                                                                  \
        CALL_BODY_(x)                                                                                                  \
    }

CALL(Publish, )
CALL(Preflight, const)
CALL(Initialize, )
CALL(PostInitialize, )
CALL(StartRun, );
CALL(PostStartRun, )
CALL(Process, );
CALL(PostProcess, )
CALL(StopRun, );
CALL(PostStopRun, )
CALL(Resting, );
CALL(Halt, );
CALL(Halt2, );
