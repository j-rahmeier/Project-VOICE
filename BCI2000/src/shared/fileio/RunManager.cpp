////////////////////////////////////////////////////////////////////////////////
// $Id: RunManager.cpp 8335 2024-09-02 16:39:42Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A class that manages BCI2000 data file names.
//   Some of the functionality here consists of workarounds that will no longer
//   be necessary in 4.x (which will introduce publication of output file name).
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2023: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#include "RunManager.h"

#include "BCIStream.h"
#include "FilePath.h"
#include "FileUtils.h"
#include "GenericFilter.h"

#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

static const std::string sbTag = "${", seTag = "}";
static const int sDataFileShmSize = 4096;

static std::string IntToString(int inNumber, int inWidth)
{
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(inWidth) << inNumber;
    return oss.str();
}

static int StringToInt(const std::string &inString)
{
    return ::atoi(inString.c_str());
}

static std::string SubstituteDate(const std::string& inString)
{
    time_t now = std::time(nullptr);
    struct tm local_now = { 0 };
#if _MSC_VER // switch to localtime_r() when C++23 is available
    ::localtime_s(&local_now, &now);
#else
    ::localtime_r(&now, &local_now);
#endif

    std::string s = inString, tag = sbTag + "DateTime:";
    size_t pos = 0;
    while ((pos = s.find(tag)) != std::string::npos) {

        size_t pos2 = pos + tag.length(),
               pos3 = s.find(seTag, pos2);

        if (pos3 != std::string::npos) {

            std::string format = s.substr(pos2, pos3 - pos2);
            char buf[1024];
            std::string datetime;
            if (::strftime(buf, sizeof(buf), format.c_str(), &local_now))
                datetime = buf;
            s = s.substr(0, pos) + datetime + s.substr(pos3 + 1);
        }
    }
    return s;
}

RunManager::RunManager()
: mPublished(false), mInWritingModule(false), mIsSecondaryBCI2000(false), mFilePart(0), mpDataFileShm(nullptr)
{
}

RunManager::~RunManager()
{
    std::lock_guard<std::recursive_mutex> lock(mMutex);
    if (mCurrentRunFile.Length() == 0)
    {
        mCurrentRunFile.Close();
        FileUtils::RemoveFile(mCurrentRun);
    }
    delete mpDataFileShm;
}

std::string RunManager::CurrentSession()
{
    return CurrentSession(*Parameters);
}

const std::string &RunManager::CurrentRun()
{
    std::lock_guard<std::recursive_mutex> lock(mMutex);

    if (!mInWritingModule || mIsSecondaryBCI2000)
    {
        try {
            SharedMemory shm(mDataFileShmName);
            char* pData = static_cast<char*>(shm.Memory());
            if (!pData)
                throw std::exception();
            mCurrentRun = pData;
            if (mIsSecondaryBCI2000)
                mCurrentRun += "_1";
        }
        catch (const std::exception&)
        {
            if (!mIsSecondaryBCI2000)
                bcierr << "Cannot obtain current run outside writing module, "
                       << "is source module running on a different machine?";
            else
                bcierr << "Cannot obtain current run in secondary BCI2000 instance, "
                          "is secondary BCI2000 instance running on a different machine?";
        }
    }
    else if (!mPublished)
    {
        bcierr << "Trying to access current run file, which is undefined";
    }
    else if (!mCurrentRunFile.IsOpen())
    {
        mCurrentRun = ConstructRun();
        char* pData = static_cast<char*>(mpDataFileShm->Memory());
        if (pData)
        {
            ::strncpy(pData, mCurrentRun.c_str(), sDataFileShmSize);
            *(pData + sDataFileShmSize - 1) = 0;
        }
    }
    if (mInWritingModule && mIsSecondaryBCI2000 && !mCurrentRunFile.IsOpen())
    {
        if (!mCurrentRunFile.Open(mCurrentRun, File::out | File::exclusive | File::nodelete))
            bcierr << "Could not open \"" << mCurrentRun << "\" for writing";
    }
    return mCurrentRun;
}

std::string RunManager::BeginNextFilePart()
{
    std::lock_guard<std::recursive_mutex> lock(mMutex);
    if (mFilePart < 1)
        bcierr << "Cannot call RunManager::BeginNextFilePart() at this time";
    std::string base = FileUtils::StripExtension(mCurrentRun);
    std::string ext = FileUtils::ExtractExtension(mCurrentRun);
    std::string filename = base + ".part" + std::to_string(++mFilePart) + ext;
    mCurrentRunFile.Close();
    if (!mCurrentRunFile.Open(filename, File::out | File::exclusive | File::nodelete))
        bcierr << "Could not open \"" << filename << "\" for writing";
    return filename;
}

Tiny::File &RunManager::CurrentFile()
{
    CurrentRun();
    return mCurrentRunFile;
}

void RunManager::Publish()
{
#ifdef TODO
#error Remove this test from V4
#endif
    GenericFilter::ChainInfo info = GenericFilter::GetChainInfo();
    for (GenericFilter::ChainInfo::const_iterator i = info.begin(); i != info.end(); ++i)
        mInWritingModule = mInWritingModule || !::stricmp(i->name.c_str(), "DataIOFilter");

    mIsSecondaryBCI2000 = OptionalParameter("/IsSecondaryBCI2000", 0);

    if (mInWritingModule)
    {
        bool existed = Parameters->Exists("DataFile");
        BEGIN_PARAMETER_DEFINITIONS
            "Storage:Data%20Location string DataFile= % % % % "
                "// name of data file relative to DataDirectory",
        END_PARAMETER_DEFINITIONS
        if (!existed)
        {
            Parameter("DataFile") = "${SubjectName}${SubjectSession}/${SubjectName}S${SubjectSession}R${SubjectRun}.${FileFormat}";
        }
#ifdef TODO
# error Remove this hack from v4
#endif
        BEGIN_PARAMETER_DEFINITIONS
          "Storage string DataFileShm= % % % % "
          "// name of shared memory object holding data file name (readonly)",
        END_PARAMETER_DEFINITIONS
        if (!mIsSecondaryBCI2000)
        {
            delete mpDataFileShm;
            mpDataFileShm = new SharedMemory(sDataFileShmSize);
            Parameter("DataFileShm") = mpDataFileShm->Name();
        }
    }
    mPublished = true;
}

void RunManager::Preflight() const
{
    std::lock_guard<std::recursive_mutex> lock(mMutex);
    if (mCurrentRunFile.Length() == 0)
    {
        mCurrentRunFile.Close();
        FileUtils::RemoveFile(mCurrentRun);
    }
    else if (mCurrentRunFile.Length() > 0)
    {
        mCurrentRunFile.Close();
    }
    // CurrentRun() may be called during filter Preflight()
    mDataFileShmName = OptionalParameter("DataFileShm").ToString();
}

void RunManager::Initialize()
{
    mDataFileShmName = OptionalParameter("DataFileShm").ToString();
}


std::string RunManager::DataDirectory(ParamList &inParams)
{
    Param &session = inParams.ByPath("SubjectSession");
    session.Value() = IntToString(StringToInt(session.Value()), 3);
    Param &run = inParams.ByPath("SubjectRun");
    run.Value() = IntToString(StringToInt(run.Value()), 2);

    Tiny::FilePath path = inParams.ByPath("DataDirectory").Value().AsString();
    path.ToAbsolute(FileUtils::InstallationDirectory()).Simplify();
    return path.ToString() + path.NativeSeparator();
}

std::string RunManager::CurrentSession(ParamList &inParams)
{
    std::string dataDir = DataDirectory(inParams), file = inParams.ByPath("DataFile").Value(),
                dir = FileUtils::ExtractDirectory(file), runRef = sbTag + "SubjectRun" + seTag;
    file = FileUtils::ExtractBase(file);

    size_t pos;
    while (std::string::npos != (pos = file.find(runRef)))
    {
        size_t i = pos;
        while (i > 0 && ::isalpha(file[i - 1]))
            --i;
        file = file.substr(0, i) + file.substr(pos + runRef.length());
    }
    if (file.empty())
        file = "Session${SubjectSession}";

    while (std::string::npos != (pos = dir.find(runRef)))
        dir = dir.substr(0, pos) + "_MultipleRuns_" + dir.substr(pos + runRef.length());

    if (inParams.Exists("/IsSecondaryBCI2000") && ::atoi(inParams.ByPath("/IsSecondaryBCI2000").Value().c_str()))
        file += "_1";

    return dataDir + SubstituteVariables(dir + file, inParams);
}

std::string RunManager::ConstructRun()
{
    Assert(mInWritingModule && !mIsSecondaryBCI2000);

    std::lock_guard<std::recursive_mutex> lock(mMutex);
    ParamList &parameters = *Parameters;
    std::string path = DataDirectory(parameters), first, file, result;
    int run = ::atoi(parameters.ByPath("SubjectRun").Value().c_str());
    if (--run < 0)
        run = 0;
    bool done = false, ok = true;
    while (ok && !done)
    {
        parameters.ByPath("SubjectRun").Value() = IntToString(++run, 2);
        file = SubstituteVariables(parameters.ByPath("DataFile").Value(), *Parameters);
        if (first.empty())
            first = file;
        else if (file == first)
        {
            std::string ext = FileUtils::ExtractExtension(file);
            file = FileUtils::StripExtension(file) + IntToString(run, 0) + ext;
        }
        result = path + file;

        std::string dir = FileUtils::ExtractDirectory(result);
        if (!FileUtils::MakeDirectory(dir, true))
        {
            ok = false;
            bcierr << "Could not create directory: " << dir;
        }
        else
        {
            done = mCurrentRunFile.Open(result, File::out | File::exclusive | File::nodelete);
            ok = FileUtils::IsFile(result); // ok but not done if file existed
            if (!ok)
                bcierr << "Could not create output file: " << result;
        }
    }
    if (ok)
    {
        result = FileUtils::NormalizedPath(result);
        mFilePart = 1;
    }
    return result;
}


std::string RunManager::SubstituteParameters(const std::string &inExpr, const ParamList &inParams)
{
    std::set<std::string> occurred;

    std::string result = inExpr;
    size_t beg;
    while (std::string::npos != (beg = result.find(sbTag)))
    {
        size_t end = result.find(seTag, beg + sbTag.length());
        if (end != std::string::npos)
        {
            std::string name = result.substr(beg, end - beg);
            name = name.substr(sbTag.length());
            std::string value;
            if (occurred.find(name) != occurred.end())
                bcierr << "Circular reference to parameter " << name;
            else if (inParams.Exists(name))
                value = inParams.ByPath(name).Value();
            result = result.substr(0, beg) + value + result.substr(end + 1);
        }
    }
    return result;
}

std::string RunManager::SubstituteVariables(const std::string& s, const ParamList& params)
{
    std::string result = SubstituteDate(s);
    return SubstituteParameters(result, params);
}
