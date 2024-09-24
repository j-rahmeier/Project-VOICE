#include "GetScore.h"

#include <algorithm>

///////////////////////////////////////////////////////////////////
/// Compute a score matrix. The number of rows indicates the number
/// of available choices (e.g., The P300 Speller 6 x 6 has 12 possible choices).
/// The number of columns is composed by the number of trials for each
/// letter to identify.
/// @param [in] Responses         Given data
/// @param [in] Code              ste Stimulus Code
/// @param [in] parms             A structure with the necessary parameters to
///                               classify BCI2000 data online
/// @param [in] trialnr           ste trialnr
/// @param [in] windowlen         parameter windowlen
/// @param [in] numchannels       Number of channels specified by the user in channel_set
///	@param [in] NumberOfSequences Number of sequences. Default = 15;
/// @param [in] NumberOfChoices   Number of choices. This values is computed by adding the
///                               rows and columns given in the P300 Speller
///	@param [out] pscore           Matrix containing the score values for each row and column
/// \author Cristhian Potes
/// \date June 29, 2009

bool GetScore(ap::template_2d_array<float, true> &Responses, ap::template_1d_array<unsigned short int, true> &Code,
              const ClassificationParameters &parms, ap::template_1d_array<short int, true> &trialnr,
              ap::template_1d_array<double, true> &windowlen, int numchannels, int NumberOfSequences,
              int NumberOfChoices, int mode, ap::real_2d_array &pscore)

{
    ///////////////////////////////////////////////////////////////////////
    // Section: Define variables
    int row_Responses, col_Responses, row_weights, col_weights, dslen, count, max, NumberOfEpochs, numVariables;

    bool flag = true;

    ap::real_2d_array Responses_double;
    ap::template_2d_array<float, true> Responses_copy;
    ap::real_2d_array DATA;
    ap::real_2d_array tmp_weights;
    ap::real_1d_array score;
    ap::real_1d_array weights;

    std::vector<short int> trial;
    std::vector<int> range;
    std::vector<int> code_indx;
    std::vector<short int>::iterator it;

    ///////////////////////////////////////////////////////////////////////
    // Section: Get Dimensions
    row_Responses = Responses.gethighbound(1) + 1;
    col_Responses = Responses.gethighbound(0) + 1;
    row_weights = parms.weights.gethighbound(1) + 1;
    col_weights = parms.weights.gethighbound(0) + 1;

    ///////////////////////////////////////////////////////////////////////
    // Section: Extract from the signal only the channels containing the "in" variables
    numVariables = static_cast<int>(col_Responses / static_cast<double>(numchannels));
    Responses_copy.setbounds(0, row_Responses - 1, 0, numVariables * (parms.channels.gethighbound(1) + 1) - 1);
    Responses_double.setbounds(0, row_Responses - 1, 0, numVariables * (parms.channels.gethighbound(1) + 1) - 1);

    for (int i = 0; i < row_Responses; i++)
    {
        for (int j = 0; j < parms.channels.gethighbound(1) + 1; j++)
        {
            ap::vmove(Responses_copy.getrow(i, j * numVariables, ((j + 1) * numVariables) - 1),
                      Responses.getrow(i, static_cast<int>(parms.channels(j) - 1) * numVariables,
                                       static_cast<int>(parms.channels(j) * numVariables) - 1));
        }
    }

    for (int i = 0; i < row_Responses; i++)
    {
        for (int j = 0; j < numVariables * (parms.channels.gethighbound(1) + 1); j++)
            Responses_double(i, j) = static_cast<double>(Responses_copy(i, j));
    }

    for (int i = 0; i < row_Responses; i++)
        trial.push_back(trialnr(i));

    ///////////////////////////////////////////////////////////////////////
    // Section: Downsampling the weights
    dslen = ap::iceil((row_weights - 1) / parms.decimation) + 1;
    tmp_weights.setbounds(0, dslen, 0, col_weights - 1);
    for (int j = 0; j < col_weights; j++)
    {
        for (int i = 0; i < dslen; i++)
        {
            if (j == 0)
                tmp_weights(i, 0) = parms.weights(i * parms.decimation, 0) - 1;

            if (j == 1)
            {
                tmp_weights(i, 1) = parms.weights(i * parms.decimation, 1) - windowlen(0);
                tmp_weights(i, 1) = ap::ifloor(tmp_weights(i, 1) / parms.decimation) + 1;
                tmp_weights(i, 1) = tmp_weights(i, 1) + (tmp_weights(i, 0) * numVariables);
            }
            if (j == 2)
                tmp_weights(i, 2) = parms.weights(i * parms.decimation, 2);
        }
    }
    ///////////////////////////////////////////////////////////////////////
    // Section: Computing the score
    DATA.setbounds(0, row_Responses - 1, 0, dslen - 1);
    score.setbounds(0, row_Responses - 1);
    weights.setbounds(0, dslen - 1);

    //double valor;
    for (int i = 0; i < dslen; i++)
    {
        //valor = tmp_weights(i, 1);
        ap::vmove(DATA.getcolumn(i, 0, row_Responses - 1),
                  Responses_double.getcolumn(static_cast<int>(tmp_weights(i, 1)), 0, row_Responses - 1));
        //valor = DATA(0, i);
        weights(i) = tmp_weights(i, 2);
    }

    matrixvectormultiply(DATA, 0, row_Responses - 1, 0, dslen - 1, false, weights, 0, dslen - 1, 1, score, 0,
                         row_Responses - 1, 0);

///////////////////////////////////////////////////////////////////////
// Section: Make sure that the epochs are not outside of the boundaries
    NumberOfEpochs = *max_element(trial.begin(), trial.end());

///////////////////////////////////////////////////////////////////////
// Section: Create a matrix with the scores for each sequence
// Initialize with NaN to account for missing data
    pscore.setbounds(0, NumberOfChoices - 1, 0, (NumberOfEpochs * NumberOfSequences) - 1);
    for (int i = 0; i < NumberOfChoices; ++i)
      for (int j = 0; j < NumberOfEpochs * NumberOfSequences; ++j)
        pscore(i, j) = std::numeric_limits<double>::quiet_NaN();

    for (int i = 0; i < NumberOfEpochs; i++)
    {
        for (size_t j = 0; j < trial.size(); j++)
        {
            if (trial[j] == i + 1)
                range.push_back(static_cast<int>(j));
        }
        if (range.empty())
            return false;

        for (int k = 0; k < NumberOfChoices; k++)
        {
            for (size_t j = 0; j < range.size(); j++)
            {
                if (Code(range[j]) == k + 1)
                    code_indx.push_back(range[j]);
            }
            for (size_t j = 0; j < code_indx.size(); j++)
            {
                if (code_indx.size() <= NumberOfSequences)
                    pscore(k, static_cast<int>((i * NumberOfSequences) + j)) = score(code_indx[j]);
            }
            code_indx.clear();
        }
        range.clear();
    }
    return true;
}
