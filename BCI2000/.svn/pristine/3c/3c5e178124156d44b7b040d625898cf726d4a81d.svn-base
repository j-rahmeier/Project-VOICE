#include "load_bcidat.h"
#include "BCI2000FileReader.h"
#include "Expression.h"

///////////////////////////////////////////////////////////////////////////////
/// Loads signal, state, and parameter data from the file whose name is given
/// in the first argument.
/// @param [in]  string   File
/// @param [in]  AnalysisMode Mode
/// @param [in]  string   Onset Expression, taken from file if empty
/// @param [out] signal   Signal.
/// @param [out] parms    Parameters.
/// @param [out] state    States.
/// returns      Error string.
/// \author Cristhian Potes
/// \date July 05, 2009

std::string load_bcidat(const std::string &filename, AnalysisMode mode,
                        const std::string &onsetExpression,
                        ap::template_2d_array<float, true> &signal, prm &parms, ste &state)
{
    ///////////////////////////////////////////////////////////////////////////////
    // Section: Define variables
    int cum, cc;
    std::vector<int> channelsInFile;
    std::vector<int> samplesInFile;
    std::vector<unsigned short int> stateStimulusCode;
    std::vector<unsigned short int> stateStimulusType;
    std::vector<unsigned short int> stateSelectedTarget;
    std::vector<unsigned short int> stateSelectedStimulus;
    std::vector<short int> statePhaseInSequence;
    std::vector<unsigned char> stateStimulusOnset;
    std::vector<int> indTarget;
    std::vector<int> SelectedRow;
    std::vector<int> SelectedCol;
    std::vector<unsigned short int> SelectedTarget;

    ///////////////////////////////////////////////////////////////////////////////////
    // Section: Open BCI2000 File, extract parameters, states, and calibrate the signal
    BCI2000FileReader file;
    file.Open(filename.c_str());

    int NumSamples = static_cast<int>(file.NumSamples());
    int NumChannels = file.SignalProperties().Channels();

    signal.setbounds(0, NumSamples - 1, 0, NumChannels - 1);
    state.StimulusCode.setbounds(0, NumSamples - 1);
    state.StimulusType.setbounds(0, NumSamples - 1);
    state.StimulusOnset.setbounds(0, NumSamples - 1);
    state.trialnr.setbounds(0, NumSamples - 1);
    state.TargetDefinitions.clear(); // jm

    // Prepare context for evaluating onset expression
    std::stringbuf sbuf;
    auto oldbuf = bcierr__.rdbuf();
    bcierr__.rdbuf(&sbuf);

    const ParamList *parameters = file.Parameters();
    const StateVector *stateVector = file.StateVector();
    const StateList *states = file.States();
    Environment::Context* pContext = Environment::Context::GlobalInstance();

    pContext->EnterPhase(Environment::preflight, const_cast<ParamList*>(parameters),
        const_cast<StateList*>(states), const_cast<StateVector*>(stateVector));

    Expression expr = onsetExpression;
    expr.Compile();
    SignalProperties properties = file.SignalProperties();
    properties.SetElements(1);
    GenericSignal exprSignal(properties);

    expr.Evaluate(&exprSignal);

    pContext->EnterPhase(Environment::nonaccess);

    pContext->EnterPhase(Environment::processing, const_cast<ParamList*>(parameters),
        const_cast<StateList*>(states), const_cast<StateVector*>(stateVector));

    // Read states and signal
    if (mode != TestExpression)
    {
        // temporarily store the signal to allow for temporal alignment of channels
        std::vector<std::vector<double>> unalignedSignal(NumChannels, std::vector<double>(NumSamples));
        for (int i = 0; i < NumSamples; i++)
        {
            file.ReadStateVector(i);

            // Get calibrated signal
            for (int j = 0; j < NumChannels; j++)
            {
                double value = file.CalibratedValue(j, i);
                unalignedSignal[j][i] = value;
                exprSignal(j, 0) = value;
            }

            stateStimulusOnset.push_back(expr.Evaluate(&exprSignal) ? 1 : 0);

            if (file.States()->Exists("StimulusCode"))
                stateStimulusCode.push_back(static_cast<unsigned short>(stateVector->StateValue("StimulusCode")));

            if (file.States()->Exists("StimulusType"))
                stateStimulusType.push_back(static_cast<unsigned short>(stateVector->StateValue("StimulusType")));

            if (file.States()->Exists("PhaseInSequence"))
                statePhaseInSequence.push_back(static_cast<short>(stateVector->StateValue("PhaseInSequence")));

            if (file.States()->Exists("SelectedTarget"))
                stateSelectedTarget.push_back(static_cast<unsigned short>(stateVector->StateValue("SelectedTarget")));

            if (file.States()->Exists("SelectedStimulus"))
                stateSelectedStimulus.push_back(static_cast<unsigned short>(stateVector->StateValue("SelectedStimulus")));
        }

        std::vector<double> weightPrev(NumChannels, 0), weightCur(NumChannels, 1);
        if (file.Parameters()->Exists("AlignChannels") && file.Parameter("AlignChannels") != 0)
        {
            if (file.Parameters()->Exists("SourceChTimeOffset"))
            {
                ParamRef SourceChTimeOffset = file.Parameter("SourceChTimeOffset");
                // Calculate weight values for linear interpolation if we do not use the default value.
                if (SourceChTimeOffset->NumValues() > 0)
                {
                    for (int ch = 0; ch < std::min(SourceChTimeOffset->NumValues(), NumChannels); ++ch)
                    {
                        weightPrev[ch] = SourceChTimeOffset(ch);
                        weightCur[ch] = 1 - weightPrev[ch];
                    }
                }
                // If we do use the default value, assume that all sampled channels are evenly distributed in time.
                else
                {
                    double delta = 1.0 / NumChannels;
                    for (int ch = 0; ch < NumChannels; ++ch)
                    {
                        weightPrev[ch] = delta * ch;
                        weightCur[ch] = 1 - weightPrev[ch];
                    }
                }
            }
        }

        for (int ch = 0; ch < NumChannels; ++ch)
            signal(0, ch) = unalignedSignal[ch][0] * weightCur[ch];
        for (int sample = 1; sample < NumSamples; ++sample) {
            for (int ch = 0; ch < NumChannels; ++ch) {
                signal(sample, ch) = unalignedSignal[ch][sample] * weightCur[ch]
                                   + unalignedSignal[ch][sample - 1] * weightPrev[ch];
            }
        }
    }

    pContext->EnterPhase(Environment::nonaccess);
    bcierr__.rdbuf(oldbuf);

    if (mode == TestExpression || !sbuf.str().empty())
        return sbuf.str();

    /////////////////////////////////////////////////////////////////////////////////
    // Section: Get parameters

    // Get the Sampling Rate
    parms.SamplingRate = static_cast<int>(file.SamplingRate());

    // Get the Number of Sequences

    if (file.Parameters()->Exists("NumberOfSequences"))
    {
        parms.NumberOfSequences = file.Parameter("NumberOfSequences");
    }
    else
    {
        return "Parameter \"NumberOfSequences\" does not exist";
    }

    if (mode & P3Speller)
    {
        // Get the NumMatrixRows
        parms.NumMatrixRows = file.Parameter("NumMatrixRows");
        // Get the NumMatrixColumns
        parms.NumMatrixColumns = file.Parameter("NumMatrixColumns");
    }
    else
    {
        if (mode & StimulusPresentation)
        {
            parms.NumMatrixRows = 0;
            parms.NumMatrixColumns = 0;
        }
    }

    parms.SoftwareCh = NumChannels;

    // parms.NumberOfSequences = *(std::min(NumberOfSequences.begin(), NumberOfSequences.end()));

    // Get the Target Definitions
    if (file.Parameters()->Exists("TargetDefinitions"))
    {
        ParamRef parameter = file.Parameter("TargetDefinitions");

        for (int i = 0; i < parameter->NumRows(); i++)
        {
            enum
            {
                Display = 0,
                Enter = 1
            };
            state.TargetDefinitions.push_back(parameter(i, Display));
            if (state.TargetDefinitions.back().empty()) // jm: use the "Enter" value if the "Display" value is empty
                state.TargetDefinitions.back() = std::string(parameter(i, Enter));
        }
    }
    else
    {
        if (mode == P3SpellerFree || mode == P3SpellerCopy)
        {
            return "The \"TargetDefinitions\" parameter for the P300 Speller task does not exist";
        }
    }

    // Get the Stimuli
    if (file.Parameters()->Exists("Stimuli"))
    {
        ParamRef parameter = file.Parameter("Stimuli");

        for (int i = 0; i < parameter->NumColumns(); i++) // jm
            state.TargetDefinitions.push_back(parameter(0, i));
    }
    else
    {
        if (mode == StimulusPresentationFree || mode == StimulusPresentationCopy)
        {
            return "The \"Stimuli\" parameter for the Stimulus Presentation task does not exist";
        }
    }

    // Simulate StimulusType if mode = P3SpellerFree
    if (mode == P3SpellerFree)
    {
        indTarget.push_back(0);
        cc = 0;
        for (int i = 0; i < NumSamples - 1; i++)
        {
            if ((stateSelectedTarget[i] == 0) && (stateSelectedTarget[i + 1] > 0))
            {
                SelectedTarget.push_back(stateSelectedTarget[i + 1]);
                SelectedRow.push_back(ap::iceil(double(SelectedTarget[cc]) / parms.NumMatrixColumns));
                SelectedCol.push_back(SelectedTarget[cc] - parms.NumMatrixColumns * (SelectedRow[cc] - 1));
                SelectedCol[cc] = SelectedCol[cc] + parms.NumMatrixColumns;
                indTarget.push_back(i);
                cc++;
            }
        }
        for (size_t i = 0; i < indTarget.size() - 1; i++)
        {
            for (int j = indTarget[i]; j < indTarget[i + 1]; j++)
            {
                if ((stateStimulusCode[j] == SelectedRow[i]) || (stateStimulusCode[j] == SelectedCol[i]))
                    stateStimulusType[j] = 1;
                else
                    stateStimulusType[j] = 0;
            }
        }
    }

    // Simulate StimulusType if mode = StimulusPresentationFree
    if (mode == StimulusPresentationFree)
    {
        indTarget.push_back(0);
        for (int i = 0; i < NumSamples - 1; i++)
        {
            if ((stateSelectedStimulus[i] == 0) && (stateSelectedStimulus[i + 1] > 0))
            {
                SelectedTarget.push_back(stateSelectedStimulus[i + 1]);
                indTarget.push_back(i);
            }
        }
        for (size_t i = 0; i < indTarget.size() - 1; i++)
        {
            for (int j = indTarget[i]; j < indTarget[i + 1]; j++)
            {
                if (stateStimulusCode[j] == SelectedTarget[i])
                    stateStimulusType[j] = 1;
                else
                    stateStimulusType[j] = 0;
            }
        }
    }
    for (int i = 0; i < NumSamples; i++)
    {
        if (stateStimulusCode[i] <= parms.NumMatrixRows && stateStimulusCode[i] != 0)
            state.StimulusCode(i) = stateStimulusCode[i] + parms.NumMatrixColumns;
        else
        {
            if (stateStimulusCode[i] > parms.NumMatrixRows && stateStimulusCode[i] != 0)
                state.StimulusCode(i) = stateStimulusCode[i] - parms.NumMatrixRows;
            else
                state.StimulusCode(i) = 0;
        }

        state.StimulusType(i) = stateStimulusType[i];
        state.StimulusOnset(i) = stateStimulusOnset[i];
    }
    // Create the state trialnr
    if (mode & P3Speller)
    {
        cum = 0;
        for (int i = 1; i < NumSamples; i++)
        {
            state.trialnr(i - 1) = cum;
            if ((statePhaseInSequence[i - 1] != 2) && (statePhaseInSequence[i] == 2))
                cum++;
        }
    }

    // Create the state trialnr
    if (mode == StimulusPresentationFree || mode == StimulusPresentationCopy)
    {
        if (file.Parameters()->Exists("SequenceType") && file.Parameter("SequenceType") == 2) // P3Speller compatible
        {
            int trial = 0;
            for (int i = 1; i < NumSamples; ++i)
            {
                state.trialnr(i - 1) = trial;
                if ((statePhaseInSequence[i - 1] != 2) && (statePhaseInSequence[i] == 2))
                    ++trial;
            }
        }
        else
        {
            int trial = 1, sequence = 0;

            for (int i = 1; i < NumSamples; ++i)
            {
                state.trialnr(i - 1) = trial;
                if ((statePhaseInSequence[i - 1] != 2) && (statePhaseInSequence[i] == 2))
                {
                    if (++sequence == parms.NumberOfSequences + 1)
                    {
                        sequence = 1;
                        ++trial;
                    }
                }
            }
        }
    }
    // Extend stimulus code and stimulus type into non-stimulation phases
    // to allow delayed onset expressions to match, jm
    int lastStimulusCode = 0, lastStimulusType = 0;
    for (int i = 0; i <= state.StimulusCode.gethighbound(); ++i)
    {
        int curStimulusCode = state.StimulusCode(i),
            curStimulusType = state.StimulusType(i);
        if (curStimulusCode == 0)
        {
            state.StimulusCode(i) = lastStimulusCode;
            state.StimulusType(i) = lastStimulusType;
        }
        else
        {
            lastStimulusCode = curStimulusCode;
            lastStimulusType = curStimulusType;
        }
    }
    return "";
}
