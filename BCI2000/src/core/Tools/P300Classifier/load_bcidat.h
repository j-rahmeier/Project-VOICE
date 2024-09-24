#ifndef _load_BCIdat_h
#define _load_BCIdat_h

#include "ap.h"
#include <string>
#include <vector>

enum AnalysisMode
{
    TestExpression = 0,
    P3SpellerFree = 1,
    P3SpellerCopy = 2,
    P3Speller = P3SpellerFree | P3SpellerCopy,
    StimulusPresentationFree = 4,
    StimulusPresentationCopy = 8,
    StimulusPresentation = StimulusPresentationFree | StimulusPresentationCopy,
};

///////////////////////////////////////////////////////////////////////////////
/// \struct prm
/// This structure contains some of the parameters provided in the BCI2000 data file.
struct prm
{
    /// Total number of channels used when the data was recorded.
    int SoftwareCh;
    /// Sampling rate.
    int SamplingRate;
    /// Number of sequences.
    int NumberOfSequences;
    /// Number of matrix rows when P3Speller Task.
    int NumMatrixRows;
    /// Number of columns when P3Speller Task.
    int NumMatrixColumns;
};

///////////////////////////////////////////////////////////////////////////////
/// \struct ste
/// This structure contains some of the states provided in the BCI2000 data file.
struct ste
{
    /// Stimulus code.
    ap::template_1d_array<unsigned short int, true> StimulusCode;
    /// Stimulus type.
    ap::template_1d_array<unsigned short int, true> StimulusType;
    /// StimulusOnset.
    ap::template_1d_array<unsigned char, true> StimulusOnset;
    /// trialnr.
    ap::template_1d_array<short int, true> trialnr;
    /// Target definitions.
    std::vector<std::string> TargetDefinitions;
};

std::string load_bcidat(const std::string &filename, AnalysisMode,
                        const std::string &onsetExpression, 
                        ap::template_2d_array<float, true> &sig, prm &parms, ste &state);
#endif
