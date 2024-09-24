#include "ReadIniParameters.h"
#include "FileUtils.h"

///////////////////////////////////////////////////////////////////////////////
/// Read initial parameters given in a file (.ini). The defined parameters are
/// then written in the structure Parameter.
/// @param [in] szFile		Name of the file (.ini) containing all the initial parameters.
/// @param [out] mUserParams	Structure Parameter containing all the initial parameters.
/// \author Cristhian Potes
/// \date July 15, 2009

bool ReadIniParameters(const std::string &szFile, UserParameters &mUserParams)
{
    ///////////////////////////////////////////////////////////////////////////////
    // Section: Define variables
    CDataFile CfgFile;
    std::vector<int> chset;
    std::vector<float> windlen;
    ///////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////
    // Section: Testing input parameters and initialize parameters
    if (!FileUtils::IsFile(szFile))
        return false; // avoid warning message from CfgFile

    if (CfgFile.Load(szFile.c_str()))
    {
        if (CheckCfgParameterKeyExist(CfgFile, "dir", "Initialization"))
            mUserParams.dir = CfgFile.GetString("dir", "Initialization");
        else
            mUserParams.dir = "C:";

        if (CheckCfgParameterKeyExist(CfgFile, "decimation_frequency_Hz", "Initialization"))
            mUserParams.Decimation_Frequency = CfgFile.GetInt("decimation_frequency_Hz", "Initialization");
        else
            mUserParams.Decimation_Frequency = 20;

        if (CheckCfgParameterKeyExist(CfgFile, "spatial_filter", "Initialization"))
            mUserParams.SF = CfgFile.GetInt("spatial_filter", "Initialization");
        else
            mUserParams.SF = UserParameters::Raw;

        if (CheckCfgParameterKeyExist(CfgFile, "penter", "Initialization"))
            mUserParams.penter = CfgFile.GetFloat("penter", "Initialization");
        else
            mUserParams.penter = 0.1000f;

        if (CheckCfgParameterKeyExist(CfgFile, "premove", "Initialization"))
            mUserParams.premove = CfgFile.GetFloat("premove", "Initialization");
        else
            mUserParams.premove = 0.1500f;

        if (CheckCfgParameterKeyExist(CfgFile, "maxiter", "Initialization"))
            mUserParams.maxiter = CfgFile.GetInt("maxiter", "Initialization");
        else
            mUserParams.maxiter = 60;

        if (CheckCfgParameterKeyExist(CfgFile, "channel_set", "Initialization"))
        {
            std::string channelset = CfgFile.GetValue("channel_set", "Initialization");
            read_vector(channelset, mUserParams.channel_set);
        }
        else
            mUserParams.channel_set.push_back(1);

        if (CheckCfgParameterKeyExist(CfgFile, "Stimulus_onset_expr", "Initialization"))
            mUserParams.onset_expr = CfgFile.GetValue("Stimulus_onset_expr", "Initialization");
        else
            mUserParams.onset_expr = "StimulusBegin>0";

        if (CheckCfgParameterKeyExist(CfgFile, "Resp_window_ms", "Initialization"))
        {
            std::string Resp_window = CfgFile.GetValue("Resp_window_ms", "Initialization");
            read_vector(Resp_window, mUserParams.windlen);
        }
        else
        {
            mUserParams.windlen.push_back(0);
            mUserParams.windlen.push_back(800);
        }
        return true;
    }
    else
        // file does not exist
        return false;
}

///////////////////////////////////////////////////////////////////////////////
/// Checks if a key that is supposed in a specific section exists in ini file.
/// If the key does not exists an error message is generated and the programm
/// is quit.
/// @param [in] CfgFile Reference to the loaded ini file.
/// @param [in] key the key that is to be checked.
/// @param [in] section the section in which the key is supposed to be.
///////////////////////////////////////////////////////////////////////////////
bool CheckCfgParameterKeyExist(CDataFile &CfgFile, const std::string &key, const std::string &section)
{
    if (CfgFile.GetString(key, section).length() == 0)
        return false;
    else
        return true;
}
