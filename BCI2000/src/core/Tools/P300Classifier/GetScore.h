#ifndef _GetScore_h
#define _GetScore_h

#include "SWLDA.h"
#include "ap.h"

bool GetScore(ap::template_2d_array<float, true> &Responses, ap::template_1d_array<unsigned short int, true> &Code,
              const ClassificationParameters &, ap::template_1d_array<short int, true> &trialnr,
              ap::template_1d_array<double, true> &windowlen, int numchannels, int NumberOfSequences,
              int NumberOfChoices, int mode, ap::real_2d_array &pscore);
#endif
