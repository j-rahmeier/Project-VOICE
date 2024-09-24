#ifndef _GetP3Responses_h
#define _GetP3Responses_h

#include "ap.h"

void GetP3Responses(ap::template_2d_array<float, true> &signal, ap::template_1d_array<short int, true> &trialnr,
                    ap::template_1d_array<double, true> &windowlen,
                    ap::template_1d_array<unsigned short int, true> &stimulusCode,
                    ap::template_1d_array<unsigned short int, true> &stimulusType,
                    const ap::template_1d_array<unsigned char, true> &StimulusOnset,
                    const ap::template_1d_array<double, true> &channels, const int MAfilter, const int DecFact);
#endif
