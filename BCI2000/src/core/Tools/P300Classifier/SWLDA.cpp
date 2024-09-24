#include "SWLDA.h"

#include <algorithm>

///////////////////////////////////////////////////////////////////
/// Apply the Stepwise Linear Discriminant Analysis (SWLDA) classifier
/// to a given data. SWLDA models a response variable as a function of
/// the predictor variables represented by the columns of the input data.
/// The provided data must have ROWS > COLUMS.
/// @param [in] Responses	Given data
/// @param [in] Type		ste Stimulus Type
/// @param [in] trialnr		ste trialnr
/// @param [in] windowlen	parameter windowlen
/// @param [in] channels	parameter channel set specefied by the user
/// @param [in] MAfilter	parameter Filter order
/// @param [in] DecFact		parameter Decimation factor
/// @param [in] SF			parameter Spatial filter
///	@param [in] smprate		parameter Sampling rate
/// @param [in] penter		parameter penter
///	@param [in] premove		parameter premove
/// @param [in] maxiter		parameter Maximum number of features
/// @param [in] SoftwareCh	parameter Software channel
/// @param [out] params		A structure with the necessary parameters to
///							classify BCI2000 data online
/// \author Cristhian Potes
/// \date May 28, 2009
/// Reference: Draper, N. R., and H. Smith. Applied Regression Analysis, Jhon Wiley & Sons, 1966. pp. 173-216

bool SWLDA(ap::template_2d_array<float, true> &Responses, ap::template_1d_array<unsigned short int, true> &Type,
           ap::template_1d_array<short int, true> &trialnr, ap::template_1d_array<double, true> &windowlen,
           ap::template_1d_array<double, true> &channels, int MAfilter, int DecFact, int SF, int smprate, double penter,
           double premove, int maxiter, int SoftwareCh, ClassificationParameters &params, CALLBACK_STATUS callback_status)

{

    //////////////////////////////////////////////////////////////////////
    // Section: Define variables
    int row_Responses, col_Responses, row_windowlen, numchannels, first, lendsig, row_Data, col_Data, step, count, ind;

    ap::real_2d_array Responses_double;
    ap::real_2d_array Data;
    ap::real_2d_array data;
    ap::real_2d_array weights;
    ap::real_1d_array Label;
    ap::real_1d_array B;
    ap::real_1d_array SE;
    ap::real_1d_array PVAL;
    ap::boolean_1d_array in;
    ap::real_1d_array Variables;

    std::vector<int> target;
    std::vector<int> standard;
    std::vector<int> var_ind;
    std::vector<int> sample;
    std::vector<int> channel;
    std::vector<int> ch;
    std::vector<int> chused;
    std::vector<int> newind;
    std::vector<int>::iterator it;
    //////////////////////////////////////////////////////////////////////
    // Get Dimmensions
    row_Responses = Responses.gethighbound(1) + 1;
    col_Responses = Responses.gethighbound(0) + 1;
    row_windowlen = windowlen.gethighbound(1) + 1;
    numchannels = channels.gethighbound(1) + 1;
    //////////////////////////////////////////////////////////////////////
    // Section: Recasting Responses and selected the data according to the
    //			parameter first
    first = ap::iceil((MAfilter - 1) / static_cast<double>(DecFact));
    lendsig = ap::iceil((windowlen(1) - windowlen(0)) / static_cast<double>(DecFact));
    row_Data = row_Responses;
    col_Data = col_Responses - (first * numchannels);
    Responses_double.setbounds(0, row_Responses - 1, 0, col_Responses - 1);
    Data.setbounds(0, row_Data - 1, 0, col_Data - 1);

    for (int i = 0; i < row_Responses; i++)
    {
        for (int j = 0; j < col_Responses; j++)
            Responses_double(i, j) = (double)Responses(i, j);
    }

    for (int j = 0; j < row_Responses; j++)
    {
        for (int i = 0; i < numchannels; i++)
            ap::vmove(Data.getrow(j, i * (lendsig - 1), (i + 1) * (lendsig - 1) - 1),
                      Responses_double.getrow(j, (i * lendsig) + first, (i + 1) * (lendsig)-1));
    }

    //////////////////////////////////////////////////////////////////////
    // Section: Find target -> type ==1 and standard -> type ==0
    data.setbounds(0, row_Data - 1, 0, col_Data - 1);
    Label.setbounds(0, row_Data - 1);

    for (int i = 0; i < row_Data; i++)
    {
        if (Type(i) == 1)
            target.push_back(i);
        if (Type(i) == 0)
            standard.push_back(i);
    }

    //////////////////////////////////////////////////////////////////////
    // Section: Organize Data with the rows going from target to standard
    for (int i = 0; i < target.size(); i++)
    {
        ap::vmove(data.getrow(i, 0, col_Data - 1), Data.getrow(target[i], 0, col_Data - 1));
        Label(i) = 2 * (Type(target[i]) - 0.5);
    }
    for (size_t i = 0; i < standard.size(); i++)
    {
        ap::vmove(data.getrow(i + target.size(), 0, col_Data - 1),
                  Data.getrow(standard[i], 0, col_Data - 1));
        Label(i + target.size()) = 2 * (Type(standard[i]) - 0.5);
    }
    //////////////////////////////////////////////////////////////////////
    // Section: Compute SWLDA
    B.setbounds(0, col_Data - 1);
    SE.setbounds(0, col_Data - 1);
    PVAL.setbounds(0, col_Data - 1);
    in.setbounds(0, col_Data - 1);

    for (int i = 0; i < col_Data; i++)
        in(i) = false;

    stepwisefit(data, Label, penter, premove, maxiter, B, SE, PVAL, in, callback_status);

    //////////////////////////////////////////////////////////////////////
    // Section: Extract the in-variables from the data according to the variable -in-
    for (int i = 0; i < col_Data; i++)
    {
        if (in(i) != 0)
            var_ind.push_back(i);
    }

    // Modification Cristhian Potes, Oct 09, 2009
    // Check if it has been generated usable weights
    if (var_ind.size() == 0)
        return false;

    Variables.setbounds(0, var_ind.size() - 1);

    for (int i = 0; i < var_ind.size(); i++)
    {
        Variables(i) = B(var_ind[i]);
        if (var_ind[i] < (lendsig - first))
        {
            sample.push_back(var_ind[i] + 1);
            channel.push_back(1);
            newind.push_back(windowlen(0) + (sample[i] + first - 1) * DecFact);
        }
        else
        {
            sample.push_back((var_ind[i] % (lendsig - first)) + 1);
            channel.push_back(ap::ifloor(var_ind[i] / (lendsig - first)) + 1);
            newind.push_back(windowlen(0) + (sample[i] + first - 1) * DecFact);
        }
    }
    //////////////////////////////////////////////////////////////////////
    // Section: Generate the weights
    params.weights.setbounds(0, var_ind.size() * DecFact - 1, 0, 2);
    chused = channel;

    std::sort(chused.begin(), chused.end());
    it = std::unique(chused.begin(), chused.end());
    chused.resize(it - chused.begin());
    //
    for (size_t i = 0; i < chused.size(); i++)
    {
        var_ind.clear();
        for (size_t j = 0; j < channel.size(); j++)
        {
            if (channel[j] == chused[i])
                var_ind.push_back(j);
        }
        for (size_t j = 0; j < var_ind.size(); j++)
            ch.push_back(i + 1);
    }

    step = 0;
    for (int i = 0; i < channel.size(); i++)
    {
        count = 0;
        for (int j = step; j < (i + 1) * MAfilter; j++)
        {
            params.weights(j, 0) = ch[i];
            params.weights(j, 1) = (newind[i] - MAfilter + 1) + count;
            params.weights(j, 2) = Variables(i);
            count++;
        }
        step = (i + 1) * MAfilter;
    }

    params.channels.setbounds(0, chused.size() - 1);
    // Cristhian modification April 17, 2010
    params.chSet.setbounds(0, chused.size() - 1);
    for (int i = 0; i < chused.size(); i++)
    {
        params.channels(i) = chused[i];
        params.chSet(i) = channels(chused[i] - 1); //*** modified
    }

    params.windowlen.setbounds(0, row_windowlen - 1);
    for (int i = 0; i < row_windowlen; i++)
        params.windowlen(i) = windowlen(i);

    params.decimation = DecFact;

    params.smprate = smprate;

    return true;
}
