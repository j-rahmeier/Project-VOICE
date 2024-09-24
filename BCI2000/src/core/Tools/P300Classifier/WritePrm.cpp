#include "WritePrm.h"
#include "ReadIniParameters.h"

#include "pages.h"

#include <fstream>
#include <string>

///////////////////////////////////////////////////////////////////////////////
/// Generate prm fragment
/// @param [in] filename		String containing the directory where you want to save the prm
/// @param [in] params			Structure containing all the parameters.
/// @param [in] SF				Specify the type of spatial filter (SF). If SF = 1; then SF is RAW
///								If SF = 2; then SF is CAR.
/// \author Cristhian Potes
/// \date July 22, 2009
bool WritePrm(const std::string &filename, const ClassificationParameters &params, int SF)
{
    int numCh = params.channels.gethighbound(1) + 1;
    std::ofstream fout(filename);

    if (fout.is_open())
    {
        fout << "Filtering:LinearClassifier matrix Classifier= " << params.weights.gethighbound(1) + 1
             << " { input%20channel input%20element%20(bin) output%20channel weight } ";
        for (int i = 0; i < params.weights.gethighbound(1) + 1; i++)
        {
            for (int j = 0; j < params.weights.gethighbound(0) + 1; j++)
            {
                if (j == 2)
                    fout << int(1) << " ";

                fout << params.weights(i, j) << " ";
            }
        }
        fout << "// Linear classification matrix in sparse representation" << std::endl;
        fout << "Filtering:P3TemporalFilter int EpochLength= " << ap::ifloor(params.windowlen(1) * 1000 / params.smprate)
             << "ms 500ms 0 % // Length of data epoch from stimulus onset" << std::endl;
        if (SF == UserParameters::CAR)
        {
            fout << "Filtering:SpatialFilter int SpatialFilterType= 3"
                 << " 2 0 3 // spatial filter type 0: none, 1: full matrix, 2: sparse matrix, 3: common average "
                    "reference (CAR) (enumeration)"
                 << std::endl;
        }
        if (SF == UserParameters::Raw)
        {
            fout << "Filtering:SpatialFilter int SpatialFilterType= 0"
                 << " 2 0 3 // spatial filter type 0: none, 1: full matrix, 2: sparse matrix, 3: common average "
                    "reference (CAR) (enumeration)"
                 << std::endl;
        }
        fout << "Source:Online%20Processing list TransmitChList= " << numCh << " ";
        for (int i = 0; i < numCh; i++)
            // Cristhian modification April 17, 2010
            fout << params.chSet(i) << " ";

        fout << "1 1 128 // list of transmitted channels" << std::endl;

        fout.close();
        return true;
    }
    return false;
}
