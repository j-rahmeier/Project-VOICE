#ifndef _SWLDA_h
#define _SWLDA_h

#include "ap.h"
#include "stepwisefit.h"

///////////////////////////////////////////////////////////////////////////////
/// \struct parameters
/// This structure contains all the parameters needed for real time classification
/// using BCI2000
struct ClassificationParameters
{
    /// feature weight matrix
    ap::real_2d_array weights;
    /// Contains only the channels with the predictor variables given by the SWLDA
    ap::real_1d_array channels;
    ap::real_1d_array chSet;
    /// Response window
    ap::real_1d_array windowlen;
    /// Decimation frequency
    int decimation;
    /// Sampling rate
    int smprate;
};

bool SWLDA(ap::template_2d_array<float, true> &Responses, ap::template_1d_array<unsigned short int, true> &Type,
           ap::template_1d_array<short int, true> &trialnr, ap::template_1d_array<double, true> &windowlen,
           ap::template_1d_array<double, true> &channels, int MAfilter, int DecFact, int SF, int smprate, double penter,
           double premove, int maxiter, int SoftwareCh, ClassificationParameters &, CALLBACK_STATUS callback_status = nullptr);
#endif
