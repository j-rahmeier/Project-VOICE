////////////////////////////////////////////////////////////////////////////////
// $Id: MatlabFilter.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
//         jeremy.hill@tuebingen.mpg.de
// Description: This BCI2000 filter calls a Matlab instance to act upon signals,
//    parameters, and states, thus providing the full BCI2000 filter interface
//    to a Matlab filter implementation.
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
#include "MatlabFilter.h"
#include "BCIStream.h"
#include "FileUtils.h"
#include "SharedMemory.h"

RegisterFilter(MatlabFilter, 2.C);

#define MATLAB_NAME(x) "bci_" #x // How variables and functions are named in Matlab.

// Matlab function names
#define CONSTRUCT MATLAB_NAME(Construct)
#define DESTRUCT MATLAB_NAME(Destruct)
#define PREFLIGHT MATLAB_NAME(Preflight)
#define INITIALIZE MATLAB_NAME(Initialize)
#define PROCESS MATLAB_NAME(Process)
#define START_RUN MATLAB_NAME(StartRun)
#define STOP_RUN MATLAB_NAME(StopRun)
#define RESTING MATLAB_NAME(Resting)
#define HALT MATLAB_NAME(Halt)

// Matlab variable names
#define DATA_HANDLE MATLAB_NAME(Data)
#define DATA_STRUCT DATA_HANDLE ".Data(1)."
#define PARAM_DEFS MATLAB_NAME(ParamDefs)
#define STATE_DEFS MATLAB_NAME(StateDefs)
#define IN_SIGNAL "InSignal"
#define IN_SIGNAL_DIMS MATLAB_NAME(InSignalDims)
#define OUT_SIGNAL "OutSignal"
#define OUT_SIGNAL_DIMS MATLAB_NAME(OutSignalDims)
#define PARAMETERS MATLAB_NAME(Parameters)
#define STATES MATLAB_NAME(States)
#define STATE_SAMPLES MATLAB_NAME(StateSamples)
#define STATE_DATA MATLAB_NAME(StateData)

#define VAR_F MATLAB_NAME(f)
#define VAR_N MATLAB_NAME(n)
#define VAR_I MATLAB_NAME(i)

static bool IsIgnoredState(const State &s)
{
    return s.Name().empty() || s.Name().front() == '_';
}

MatlabFilter::MatlabFilter() : mpBci_Process(0), mpData(0), mStatesOffset(0), mSampleBlockSize(0)
{
}

void MatlabFilter::Publish()
{
    std::string matlabExecutable = OptionalParameter("MatlabExecutable");
    if (matlabExecutable.empty())
        matlabExecutable = MatlabConnection::FindMatlab();
    if (matlabExecutable.empty())
        throw bcierr << "Could not find Matlab executable. "
                     << "Use --MatlabExecutable in your batch file to specify the location of Matlab.";
    bciout__ << "Waiting for Matlab to start up ...";
    MatlabConnection::Open(matlabExecutable);
    if (!MatlabConnection::IsOpen())
        bcierr << "Could not connect to Matlab.";
    else
    {
        bciout__ << "Connected to Matlab";
        // Configure matlab behavior as specified by --MatlabStayOpen.
        mMatlabStayOpen = OptionalParameter("MatlabStayOpen", closeMatlab);
        // Change matlab's working directory to the directory specified by --MatlabWD.
        std::string wd = OptionalParameter("MatlabWD", ".");
        wd = FileUtils::AbsolutePath(wd);
        MatlabConnection::PutString("bci_WD", wd);
        CallMatlab(MatlabFunction("cd").InputArgument("bci_WD"));
        MatlabConnection::ClearVariable("bci_WD");

        MatlabFunction bci_Construct(CONSTRUCT);
        bci_Construct.OutputArgument(PARAM_DEFS).OutputArgument(STATE_DEFS);
        if (CallMatlab(bci_Construct))
        {
            // Add the parameters and states requested by the Matlab bci_Construct function.
            int numParamDefs = static_cast<int>(MatlabConnection::GetIntScalar("max(size(" PARAM_DEFS "))"));
            for (int i = 1; i <= numParamDefs; ++i)
            {
                std::ostringstream expr;
                expr << PARAM_DEFS << "{" << i << "}";
                std::string paramDef = MatlabConnection::GetString(expr.str());
                if (!Parameters->Add(paramDef))
                    bcierr << "Error in parameter definition: " << paramDef;
            }
            int numStateDefs = static_cast<int>(MatlabConnection::GetIntScalar("max(size(" STATE_DEFS "))"));
            for (int i = 1; i <= numStateDefs; ++i)
            {
                std::ostringstream expr;
                expr << STATE_DEFS << "{" << i << "}";
                std::string stateDef = MatlabConnection::GetString(expr.str());
                if (!States->Add(stateDef))
                    bcierr << "Error in state definition: " << stateDef;
            }
        }
        MatlabConnection::ClearVariable(PARAM_DEFS);
        MatlabConnection::ClearVariable(STATE_DEFS);

        // Issue a warning to indicate potential mis-configuration if there is no
        // Preflight, Initialize, or Process function available for Matlab.
        const char *essentialFunctions[] = {
            PREFLIGHT,
            INITIALIZE,
            PROCESS,
        };
        std::ostringstream oss;
        for (size_t i = 0; i < sizeof(essentialFunctions) / sizeof(*essentialFunctions); ++i)
            if (!MatlabFunction(essentialFunctions[i]).Exists())
                oss << ", " << essentialFunctions[i];
        if (!oss.str().empty())
            bciwarn << "The following functions could not be found in the Matlab path:\n"
                    << oss.str().substr(2) << ".\n"
                    << "Make sure that the m-files exist within path or working directory, "
                    << "and contain appropriate function definitions.\n"
                    << "Consider using the --MatlabWD command line option to set Matlab's "
                    << "working directory at startup";
    }
    MatlabConnection::Pause();
}

MatlabFilter::~MatlabFilter()
{
    MatlabConnection::Continue();
    delete mpBci_Process;
    if (MatlabConnection::IsOpen())
    {
        { // Make sure bci_Destruct is out of scope when calling MatlabConnection::Close().
            MatlabFunction bci_Destruct(DESTRUCT);
            CallMatlab(bci_Destruct);
        }

        if (mMatlabStayOpen != dontClear)
        {
            MatlabConnection::ClearVariable(DATA_HANDLE); // Matlab memmap object must be treated as variable
            MatlabConnection::ClearVariable(PARAMETERS);
            MatlabConnection::ClearVariable(STATES);
            MatlabConnection::ClearVariable(STATE_DATA);
            MatlabConnection::ClearVariable(STATE_SAMPLES);
            MatlabConnection::ClearVariable(VAR_F);
            MatlabConnection::ClearVariable(VAR_I);
            MatlabConnection::ClearVariable(VAR_N);
        }
        MatlabConnection::Close(mMatlabStayOpen == closeMatlab);
    }
    delete mpData;
}

void MatlabFilter::Preflight(const SignalProperties &Input, SignalProperties &Output) const
{
    MatlabConnection::Continue();
    Parameter("SampleBlockSize");
    int matlabStayOpen = OptionalParameter("MatlabStayOpen", closeMatlab);
    switch (matlabStayOpen)
    {
    case closeMatlab:
    case clearVariables:
    case dontClear:
        break;
    default:
        bcierr << "Undefined value of MatlabStayOpen\n"
               << "Possible values are:\n"
               << " 0: close Matlab;\n"
               << " 1: keep Matlab open, clear variables;\n"
               << " 2: keep Matlab open, keep variables.";
    }

    InitializeMatlabWS();
    Output = Input;
    MatlabConnection::PutIntMatrix(IN_SIGNAL_DIMS, MatlabConnection::IntMatrix::fromSignalProperties(Input));
    MatlabFunction bci_Preflight(PREFLIGHT);
    bci_Preflight.InputArgument(IN_SIGNAL_DIMS).OutputArgument(OUT_SIGNAL_DIMS);
    if (CallMatlab(bci_Preflight))
    {
        Output = MatlabConnection::GetIntMatrix(OUT_SIGNAL_DIMS).toSignalProperties();
        Output.ElementUnit().SetOffset(0).SetGain(1.0 / Output.Elements() / Input.UpdateRate()).SetSymbol("s");
        if (!Input.IsStream() && Output.Elements() > 1)
            Output.SetIsStream(false);
    }
    MatlabConnection::ClearVariable(IN_SIGNAL_DIMS);
    MatlabConnection::ClearVariable(OUT_SIGNAL_DIMS);
}

void MatlabFilter::Initialize(const SignalProperties &Input, const SignalProperties &Output)
{
    MatlabConnection::Continue();
    // Re-configure matlab engine behavior as specified by the MatlabStayOpen parameter.
    mMatlabStayOpen = OptionalParameter("MatlabStayOpen", closeMatlab);

    MatlabConnection::PutIntMatrix(IN_SIGNAL_DIMS, MatlabConnection::IntMatrix::fromSignalProperties(Input));
    MatlabConnection::PutIntMatrix(OUT_SIGNAL_DIMS, MatlabConnection::IntMatrix::fromSignalProperties(Output));
    MatlabFunction bci_Initialize(INITIALIZE);
    bci_Initialize.InputArgument(IN_SIGNAL_DIMS).InputArgument(OUT_SIGNAL_DIMS);
    CallMatlab(bci_Initialize);
    MatlabConnection::ClearVariable(IN_SIGNAL_DIMS);
    MatlabConnection::ClearVariable(OUT_SIGNAL_DIMS);

    InitializeMatlabWS();
    InitializeMatlabProcessing(Input, Output);
    MatlabConnection::Pause();
}

void MatlabFilter::Process(const GenericSignal &Input, GenericSignal &Output)
{
    StatesToMatlabWS();
    double *p = reinterpret_cast<double *>(mpData->Memory());
    for (int el = 0; el < Input.Elements(); ++el)
        for (int ch = 0; ch < Input.Channels(); ++ch)
            *p++ = Input(ch, el);

    if (State("Running") && mpBci_Process->Exists())
    {
        std::string err = mpBci_Process->Execute();
        for (int el = 0; el < Output.Elements(); ++el)
            for (int ch = 0; ch < Output.Channels(); ++ch)
                Output(ch, el) = *p++;
        MatlabWSToStates();
        if (!err.empty())
        {
            bciwarn << err << "\nAborting current run";
            State("Running") = 0;
        }
    }
}

void MatlabFilter::StartRun()
{
    MatlabConnection::Continue();
    MatlabFunction bci_StartRun(START_RUN);
    if (bci_StartRun.Exists())
    {
        ParamsToMatlabWS();
        CallMatlab(bci_StartRun);
    }
}

void MatlabFilter::StopRun()
{
    MatlabConnection::Continue();
    MatlabFunction bci_StopRun(STOP_RUN);
    if (CallMatlab(bci_StopRun))
        MatlabWSToParams();
}

void MatlabFilter::Resting()
{
    MatlabConnection::Continue();
    MatlabFunction bci_Resting(RESTING);
    CallMatlab(bci_Resting);
    MatlabConnection::Pause();
}

void MatlabFilter::Halt()
{
    MatlabConnection::Continue();
    MatlabFunction bci_Halt(HALT);
    CallMatlab(bci_Halt);
}

// Internal functions
void MatlabFilter::InitializeMatlabWS() const
{
    MatlabConnection::ClearVariable(PARAMETERS);
    MatlabConnection::CreateGlobal(PARAMETERS);
    ParamsToMatlabWS();
    MatlabConnection::ClearVariable(STATES);
    MatlabConnection::CreateGlobal(STATES);
    MatlabConnection::ClearVariable(STATE_SAMPLES);
    MatlabConnection::CreateGlobal(STATE_SAMPLES);
    MatlabConnection::ClearVariable(STATE_DATA);
    MatlabConnection::CreateGlobal(STATE_DATA);
    for (int i = 0; i < States->Size(); ++i)
    {
        const class State &s = States->ByIndex(i);
        if (!IsIgnoredState(s))
            MatlabConnection::PutIntScalar(STATES "." + s.Name(), State(s.Name()));
    }
}

void MatlabFilter::StatesToMatlabWS() const
{
    double *p = reinterpret_cast<double *>(mpData->Memory()) + mStatesOffset;
    for (int i = 0; i != States->Size(); ++i)
    {
        const class State &s = States->ByIndex(i);
        if (!IsIgnoredState(s))
            for (int j = 0; j < mSampleBlockSize; ++j)
                *p++ = Statevector->StateValue(s.Location(), s.Length(), j);
    }
}

void MatlabFilter::MatlabWSToStates()
{
    // To ensure backward compatibility, Matlab code is not allowed to change states
    // that vary during a single block of data.
    // Changes to such states are silently ignored.
    const double *p = reinterpret_cast<const double *>(mpData->Memory()) + mStatesOffset;
    for (int i = 0; i != Statevector->StateList().Size(); ++i)
    {
        const class State &s = Statevector->StateList().ByIndex(i);
        if (!IsIgnoredState(s))
        {
            double val = Statevector->StateValue(s.Location(), s.Length(), 0);
            if (*p != val)
            {
                bool isConst = true;
                for (int j = 1; j < Statevector->Samples() && isConst; ++j)
                    isConst = (Statevector->StateValue(s.Location(), s.Length(), j) == val);
                if (isConst)
                    State(s.Name()) = static_cast<State::ValueType>(*p);
            }
            p += mSampleBlockSize;
        }
    }
}

void MatlabFilter::ParamsToMatlabWS() const
{
    for (int i = 0; i < Parameters->Size(); ++i)
    {
        Param &p = Parameters->ByIndex(i);
        const char *pName = p.Name().c_str();
        MatlabConnection::PutCells(std::string(PARAMETERS ".") + p.Name(),
                                   MatlabConnection::StringMatrix::fromParam(p));
    }
}

void MatlabFilter::MatlabWSToParams()
{
    for (int i = 0; i < Parameters->Size(); ++i)
    {
        Param &p = Parameters->ByIndex(i);
        MatlabConnection::StringMatrix values = MatlabConnection::GetCells(std::string(PARAMETERS ".") + p.Name());
        int newRows = static_cast<int>(values.size()), newCols = static_cast<int>(newRows ? values.at(0).size() : 1);
        if (newRows != p.NumRows() || newCols != p.NumColumns())
            p.SetDimensions(newRows, newCols);
        for (int row = 0; row < newRows; ++row)
            for (int col = 0; col < newCols; ++col)
                if (std::string(p.Value(row, col)) != values.at(row).at(col))
                    p.Value(row, col) = values.at(row).at(col);
    }
}

void MatlabFilter::InitializeMatlabProcessing(const SignalProperties &Input, const SignalProperties &Output)
{
    int numStates = 0;
    for (int i = 0; i < States->Size(); ++i)
        if (!IsIgnoredState(States->ByIndex(i)))
            ++numStates;
    mSampleBlockSize = Parameter("SampleBlockSize");

    MatlabConnection::ClearVariable(DATA_HANDLE);
    delete mpData;
    mStatesOffset = Input.Channels() * Input.Elements() + Output.Channels() * Output.Elements();
    mpData = new SharedMemory("file://", (mStatesOffset + numStates * mSampleBlockSize) * sizeof(double));

    std::ostringstream oss;
    oss << DATA_HANDLE << " = memmapfile('" << mpData->Name() << "', 'Format', { "
        << "'double' [" << Input.Channels() << " " << Input.Elements() << "] '" IN_SIGNAL "';"
        << "'double' [" << Output.Channels() << " " << Output.Elements() << "] '" OUT_SIGNAL "';"
        << "'double' [" << mSampleBlockSize << " " << numStates << "] 'States';"
        << " }, 'Writable', true);";
    std::string error;
    if (!MatlabConnection::Execute(oss.str(), error))
        bcierr << "Could not create memmapfile object in Matlab workspace:\n" << error;

    delete mpBci_Process;
    mpBci_Process = new MatlabFunction(PROCESS);
    mpBci_Process->InputArgument(DATA_STRUCT IN_SIGNAL).OutputArgument(DATA_STRUCT OUT_SIGNAL);
    mpBci_Process->CodePre(STATE_DATA
                           "=" DATA_STRUCT "States.';" VAR_F "=fieldnames(" STATES ");" VAR_N "=length(" VAR_F ");"
                           "if(" VAR_N "~=size(" DATA_STRUCT "States,2)) error('Unexpected number of fields in " STATES
                           "'); end;"
                           "for " VAR_I "=1:" VAR_N ";" STATES ".(" VAR_F "{" VAR_I "})=" STATE_DATA "(" VAR_I
                           ",1);" STATE_SAMPLES ".(" VAR_F "{" VAR_I "})=" STATE_DATA "(" VAR_I ",:);"
                           "end;");
    mpBci_Process->CodePost(
        VAR_F "=fieldnames(" STATES ");" VAR_N "=length(" VAR_F ");"
              "if(" VAR_N "~=size(" DATA_STRUCT "States,2)) error('Unexpected number of fields in " STATES "'); end;"
              "for " VAR_I "=1:" VAR_N ";" STATE_DATA "(" VAR_I ",1)=" STATES ".(" VAR_F "{" VAR_I "});"
              "end;" DATA_STRUCT "States=" STATE_DATA ".';");
}

bool MatlabFilter::CallMatlab(MatlabFunction &inFunction) const
{
    if (!inFunction.Exists())
        return false;
    std::string err = inFunction.Execute();
    if (!err.empty())
    {
        bcierr__ << "Matlab function \"" << inFunction.Name() << "\": " << err;
        return false;
    }
    return true;
}
