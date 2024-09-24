#ifndef _ReadIniParameters_h
#define _ReadIniParameters_h

#include "CDataFile.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
/// \struct UserParameters
/// This structure contains all the initial parameters to train or test the
/// BCI 2000 data files using the SWLDA (Stepwise Linear Discriminant Analysis).

struct UserParameters
{
    std::string dir;
    /// Decimation Frequency. Used to specify the temporal decimation frequency
    /// of the data in Hz. Set to the sampling rate for no decimation. The lower
    /// the Decimation Frequency, the less original data retained for processing.
    /// Default = 20.
    int Decimation_Frequency;
    /// Spatial Filter. Raw is no spatial filter applied to the data and CAR is a
    /// Common Average Reference.
    enum { Raw = 1, CAR = 2, }; int SF;
    /// Maximum p-value for a predictor to be added. Default = 0.1000.
    float penter;
    /// Minimum p-value for a predictor to be removed. Default = 0.1500.
    float premove;
    /// Maximum model features. Used to specify the maximum number of features to
    /// be kept in the SWLDA model. Only a single value can be entered for
    /// evaluation. Default = 60.
    int maxiter;
    /// Channel Set. Used to specify the channels set that will be used to create
    /// feature weights. The specified channels must be a subset of the channels
    /// contained in the training data file.
    std::vector<unsigned int> channel_set;
    /// Response Window. Used to specify the \e begin and \e end time points in
    /// milliseconds (ms) following the stimuli collected for the analysis. This
    /// automatically converted into samples according to the sampling rate of the
    /// data (rounded). Only a single data window can be entered and will be evaluated.
    /// Negative values are accepted and \e begin < \e end. Default [0 800].
    std::vector<float> windlen;
    /// Stimulus onset expression. Used to determine the onset of a stimulus.
    /// Defaults to "StimulusBegin>0".
    std::string onset_expr;
};

///////////////////////////////////////////////////////////////////////////////
/// Stores the Type value <T>  of a string into a STL vector. Therefore
/// the STL vector is resized anyway.
/// @param [in]  inputvector
/// @param [out] outputvector
///////////////////////////////////////////////////////////////////////////////
template <class T> void read_vector(std::string inputvector, std::vector<T> &outputvector)
{
    const char *inputline = inputvector.c_str();
    int length = static_cast<int>(inputvector.length());
    int num_samples = 0;
    int idx = 0;
    outputvector.resize(length);

    while (idx < length)
    {
        while ((inputline[idx] == ' ') && (idx < length))
            idx++;
        if (inputline[idx] == '\0')
            break;
        outputvector[num_samples] = static_cast<T>(atoi(&inputline[idx]));
        while ((inputline[idx] != ' ') && (idx < length))
            idx++;
        num_samples++;
    }
    outputvector.resize(num_samples);
}
bool ReadIniParameters(const std::string &szFile, UserParameters &mUserParams);

bool CheckCfgParameterKeyExist(CDataFile &CfgFile, const std::string &key, const std::string &section);
#endif
