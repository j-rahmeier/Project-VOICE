////////////////////////////////////////////////////////////////////
// $Id: bci_filtertool.cpp 7536 2023-08-17 17:46:25Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A BCI2000 filter wrapper that reads a BCI2000
//          compliant binary stream from standard input, applies
//          a BCI2000 filter, and writes its output to the
//          standard output as a BCI2000 compliant binary stream.
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
////////////////////////////////////////////////////////////////////
#include "Files.h"
#include "GenericFilter.h"
#include "GenericVisualization.h"
#include "MessageChannel.h"
#include "Param.h"
#include "ParamList.h"
#include "State.h"
#include "StateList.h"
#include "StateVector.h"
#include "SysCommand.h"
#include "bci_tool.h"

#define FILTER_NAME "$FILTER$"

std::string ToolInfo[] = {"",
                          "Process standard input with the \"" FILTER_NAME "\" BCI2000 filter.",
                          "Reads a BCI2000 compliant binary stream from standard input, applies the\n" FILTER_NAME
                          " BCI2000 filter, and writes its output to standard output\n"
                          "as a BCI2000 compliant binary stream.",
                          "binary",
                          "          --operator=<file>     Direct visualization messages to <file>"
                          "          --operator=-          Direct visualization messages to stdout",
                          ""};

class FilterWrapper : private MessageChannel
{
  public:
    FilterWrapper(Streambuf &, Streambuf &, Streambuf &);
    ~FilterWrapper();

    static std::string FilterName();
    void Run();
    template <typename T> bool Send(const T &) = delete;

  protected:
    bool OnParam(std::istream &) override;
    bool OnState(std::istream &) override;
    bool OnVisSignalProperties(std::istream &) override;
    bool OnVisSignal(std::istream &) override;
    bool OnStateVector(std::istream &) override;

  private:
    void FinishProcessing();
    void StopRun();
    void OutputParameterChanges();
    void InitializeInputStatevector();
    void InitializeOutputStatevector();
    void DisposeStatevectors();
    void SynchronizeStatevectors();

  private:
    MessageChannel mOperator, mOutput;
    SignalProperties *mpInputProperties;
    GenericSignal mInputSignal, mOutputSignal;
    ParamList mParamlist;
    StateList mInputStatelist, mOutputStatelist, mFilterStatelist;
    StateVector *mpInputStatevector, *mpOutputStatevector;
    bool mSingleStatevector;
    Environment::Context &mEnvironment;
};

ToolResult ToolInit()
{
    std::string filterName = FilterWrapper::FilterName();
    for (int i = 0; ToolInfo[i] != ""; ++i)
    {
        size_t namePos;
        while ((namePos = ToolInfo[i].find(FILTER_NAME)) != std::string::npos)
            ToolInfo[i].replace(namePos, std::string(FILTER_NAME).length(), filterName);
    }
    return noError;
}

ToolResult ToolMain(Streambuf &in, Streambuf &out, OptionSet &arOptions)
{
    ToolResult result = noError;
    File operatorOut;
    if (arOptions.size() == 1)
    {
        std::string operatorFile = arOptions.getopt("--operator", "");
        if (operatorFile == "")
            return illegalOption;
        if (operatorFile == "-")
            operatorOut.AttachToFd(::fileno(stdout));
        else
            operatorOut.Open(operatorFile);
    }
    BufferedIO op;
    op.SetOutput(&operatorOut.Output());
    FilterWrapper(in, out, op).Run();
    if (!bcierr__.Empty() || in.Input()->Bad() || in.Input()->Failed())
        result = illegalInput;
    return result;
}

FilterWrapper::FilterWrapper(Streambuf &in, Streambuf &out, Streambuf &op)
    : MessageChannel(in), mOutput(out), mOperator(op), mpInputProperties(nullptr), mpInputStatevector(nullptr),
      mpOutputStatevector(nullptr), mSingleStatevector(true), mEnvironment(*Environment::Context::GlobalInstance())
{
    GenericVisualization::SetOutputChannel(&mOperator);
}

FilterWrapper::~FilterWrapper()
{
    DisposeStatevectors();
    delete mpInputProperties;
}

std::string FilterWrapper::FilterName()
{
    std::string name = "<n/a>";
    ParamList paramlist;
    StateList statelist;
    Environment::Context environment;
    environment.EnterPhase(Environment::construction, &paramlist, &statelist, nullptr);
    GenericFilter::InstantiateFilters();
    environment.EnterPhase(Environment::nonaccess);
    GenericFilter *pFilter = GenericFilter::GetFilter<GenericFilter>();
    if (!pFilter)
        std::cerr << "Could not create filter instance.\n"
                  << "Make sure there is a filter definition with a "
                  << "\"RegisterFilter\" statement linked into the executable." << std::endl;
    else
        name = ClassName(typeid(*pFilter));
    environment.EnterPhase(Environment::destruction);
    GenericFilter::DisposeFilters();
    return name;
}

void FilterWrapper::FinishProcessing()
{
    if (mEnvironment.Phase() == Environment::processing)
        StopRun();
    if (mEnvironment.Phase() != Environment::nonaccess)
        mEnvironment.EnterPhase(Environment::nonaccess);
}

void FilterWrapper::Run()
{
    while (HandleMessage() && bcierr__.Empty())
        ;
    FinishProcessing();
    mEnvironment.EnterPhase(Environment::destruction);
    GenericFilter::DisposeFilters();
}

bool FilterWrapper::OnParam(std::istream &is)
{
    FinishProcessing();

    Param p;
    if (p.Unserialize(is))
        mParamlist.ByPath(p.Path()) = p;
    return true;
}

bool FilterWrapper::OnState(std::istream &is)
{
    if (mEnvironment.Phase() == Environment::processing)
        StopRun();

    if (mpInputStatevector)
    {
        mInputStatelist.Clear();
        DisposeStatevectors();
    }

    State s;
    if (s.Unserialize(is))
        mInputStatelist.Add(s);
    return true;
}

bool FilterWrapper::OnStateVector(std::istream &is)
{
    if (!mpInputStatevector)
        InitializeInputStatevector();
    mpInputStatevector->Unserialize(is);
    SynchronizeStatevectors();
    if (!mpInputStatevector->StateValue("Running") && mEnvironment.Phase() == Environment::processing)
        StopRun();
    return true;
}

bool FilterWrapper::OnVisSignalProperties(std::istream &is)
{
    delete mpInputProperties;
    mpInputProperties = nullptr;
    VisSignalProperties v;
    if (v.Unserialize(is) && v.VisID().Empty())
    {
        mpInputProperties = new SignalProperties(v.SignalProperties());
        mInputSignal = GenericSignal(*mpInputProperties);
    }
    else
        mInputSignal = GenericSignal();
    return mpInputProperties;
}

bool FilterWrapper::OnVisSignal(std::istream &is)
{
    VisSignal s;
    if (s.Unserialize(is) && s.VisID().Empty())
    {
        mInputSignal.AssignValues(s);
        SignalProperties outputProperties;
        switch (mEnvironment.Phase())
        {
        case Environment::nonaccess: {
            GenericFilter::DisposeFilters();

            ParamList filterParams;
            mFilterStatelist.Clear();
            mEnvironment.EnterPhase(Environment::construction, &filterParams, &mFilterStatelist);
            GenericFilter::InstantiateFilters();
            mEnvironment.EnterPhase(Environment::nonaccess);
            mEnvironment.EnterPhase(Environment::publication, &filterParams, &mFilterStatelist);
            GenericFilter::PublishFilters();
            if (!bcierr__.Empty())
                break;
            // Make sure the filter's parameters get their properties from the filter
            // rather than the input stream.
            for (auto& param : mParamlist)
            {
              std::string path = param.Path();
              if (filterParams.Exists(path))
              {
                auto& filterParam = filterParams.ByPath(path);
                filterParam.AssignValues(param);
              }
            }
            for (const auto& param : filterParams)
            {
                mParamlist.ByPath(param.Path()) = param;
            }
        }
            /* no break */
        case Environment::publication:
            mEnvironment.EnterPhase(Environment::nonaccess);
            if (!mpInputStatevector)
                InitializeInputStatevector();
            InitializeOutputStatevector();
            mParamlist.Add("% int EvaluateTiming= 0");
            mParamlist("EvaluateTiming") = 0;
            MeasurementUnits::Initialize(mParamlist);
            mEnvironment.EnterPhase(Environment::preflight, &mParamlist, &mOutputStatelist, mpOutputStatevector);
            if (mpInputProperties && mInputSignal.Channels() == mpInputProperties->Channels() &&
                mInputSignal.Elements() == mpInputProperties->Elements())
            {
                mpInputProperties->SetUpdateRate(1.0 / MeasurementUnits::SampleBlockDuration());
                mInputSignal.SetProperties(*mpInputProperties);
                GenericFilter::PreflightFilters(*mpInputProperties, outputProperties);
            }
            else
            {
                delete mpInputProperties;
                mpInputProperties = nullptr;
                SignalProperties inputProperties(mInputSignal.Properties());
                inputProperties.SetUpdateRate(1.0 / MeasurementUnits::SampleBlockDuration());
                GenericFilter::PreflightFilters(inputProperties, outputProperties);
            }
            mOutputSignal.SetProperties(outputProperties);
            if (!bcierr__.Empty())
                break;
            /* no break */
        case Environment::preflight:
            mEnvironment.EnterPhase(Environment::nonaccess);
            mEnvironment.EnterPhase(Environment::initialization, &mParamlist, &mOutputStatelist, mpOutputStatevector);
            GenericFilter::InitializeFilters();
            for (int i = 0; i < mParamlist.Size(); ++i)
                mOutput.Send(mParamlist.ByIndex(i));
            mOutput.Send(outputProperties);
            if (!bcierr__.Empty())
                break;
            /* no break */
        case Environment::initialization:
        case Environment::resting:
            mEnvironment.EnterPhase(Environment::nonaccess);
            mEnvironment.EnterPhase(Environment::startRun, &mParamlist, &mOutputStatelist, mpOutputStatevector);
            GenericFilter::StartRunFilters();
            /* no break */
        case Environment::processing: {
            mEnvironment.EnterPhase(Environment::nonaccess);
            mEnvironment.EnterPhase(Environment::processing, &mParamlist, &mOutputStatelist, mpOutputStatevector);
            GenericFilter::ProcessFilters(mInputSignal, mOutputSignal);
            if (!bcierr__.Empty())
                break;
            mOutput.Send(*mpOutputStatevector);
            mOutput.Send(mOutputSignal);
        }
        break;
        default:
            bcierr << "Unknown Environment phase";
        }
    }
    return !!is;
}

void FilterWrapper::StopRun()
{
    mEnvironment.EnterPhase(Environment::nonaccess);
    mEnvironment.EnterPhase(Environment::stopRun, &mParamlist, &mOutputStatelist, mpOutputStatevector);
    GenericFilter::StopRunFilters();
    mEnvironment.EnterPhase(Environment::nonaccess);
    OutputParameterChanges();
    mEnvironment.EnterPhase(Environment::resting, &mParamlist, &mOutputStatelist, mpOutputStatevector);
    GenericFilter::RestingFilters();
    OutputParameterChanges();
}

void FilterWrapper::OutputParameterChanges()
{
    ParamList changedParameters;
    for (int i = 0; i < mParamlist.Size(); ++i)
        if (mParamlist.ByIndex(i).Changed())
            changedParameters.Add(mParamlist.ByIndex(i));

    if (!changedParameters.Empty())
    {
        bool success = mOutput.Send(changedParameters) && mOutput.Send(SysCommand::EndOfParameter);
        if (!success)
            bcierr << "Could not publish changed parameters";
    }
}

void FilterWrapper::InitializeInputStatevector()
{
    delete mpInputStatevector;
    mpInputStatevector = new StateVector(mInputStatelist);
}

void FilterWrapper::InitializeOutputStatevector()
{
    if (!mSingleStatevector)
        delete mpOutputStatevector;

    mSingleStatevector = mFilterStatelist.Empty();
    if (mSingleStatevector)
    {
        mOutputStatelist = mInputStatelist;
        mpOutputStatevector = mpInputStatevector;
    }
    else
    {
        for (const auto &state : mInputStatelist)
            mOutputStatelist.Add(state);
        for (const auto &state : mFilterStatelist)
            mOutputStatelist.Add(state);
        mOutputStatelist.AssignPositions();
        mpOutputStatevector = new StateVector(mOutputStatelist, mpInputStatevector->Samples());
    }
    SynchronizeStatevectors();
    for (const auto &state : mOutputStatelist)
        mOutput.Send(state);
}

void FilterWrapper::SynchronizeStatevectors()
{
    if (!mSingleStatevector)
    {
        // Copy state values by name if input and output statevectors differ.
        for (const auto& state : mInputStatelist)
        {
            const auto& outState = mOutputStatelist.ByName(state.Name());
            for (int i = 0; i < mpInputStatevector->Samples(); ++i)
            {
                State::ValueType value = mpInputStatevector->StateValue(state.Location(), state.Length(), i);
                mpOutputStatevector->SetStateValue(outState.Location(), outState.Length(), i, value);
            }
        }
    }
}

void FilterWrapper::DisposeStatevectors()
{
    delete mpInputStatevector;
    mpInputStatevector = 0;
    if (!mSingleStatevector)
        delete mpOutputStatevector;
    mpOutputStatevector = 0;
    mSingleStatevector = true;
}
