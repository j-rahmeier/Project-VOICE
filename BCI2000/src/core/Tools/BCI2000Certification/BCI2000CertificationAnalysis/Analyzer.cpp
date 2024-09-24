////////////////////////////////////////////////////////////////////////////////
// $Id: Analyzer.cpp 7536 2023-08-17 17:46:25Z mellinger $
// Authors: Adam Wilson, Kosta Andoni, juergen.mellinger@neurotechcenter.org
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
#include "Analyzer.h"
#include "AnalysisEngine.h"
#include "BCI2000FileReader.h"
#include "FileUtils.h"
#include "Resources.h"
#include "Runnable.h"
#include "StringUtils.h"
#include "TaskType.h"
#include "Thread.h"

namespace
{

std::ostream &operator<<(std::ostream &os, const AnalysisEngine::SessionData &data)
{
    const std::string &desc = data.long_description.empty() ? data.description : data.long_description;
    os << desc << " " << data.data_timestamp << "\n";
    double overall = 1;
    int failed = 0;
    for (const auto &req : data.requirements)
    {
        overall *= req.value;
        if (req.value == 0)
            ++failed;
    }
    int count = data.requirements.size();
    if (overall == 0)
        os << "failed " << failed << " of " << count << " requirements:\n";
    else if (overall == overall)
        os << "passed all " << count << " requirements:\n";
    else
        os << "could not evaluate all " << count << " requirements:\n";
    for (const auto &req : data.requirements)
    {
        std::string state;
        if (req.value == 0)
            state = "failed";
        else if (req.value == req.value)
            state = "passed";
        else
            state = "missing or invalid data";
        os << "  " << req.condition << "  " << state << "\n";
    }
    for (const auto &rep : data.reports)
    {
        if (rep.object->type() != "Group")
        {
            os << "  " << rep.object->name() << " -- " << rep.description << ":\n"
               << "  " << rep.object->summary() << "\n";
        }
    }
    return os << "\n";
}

} // namespace

struct Analyzer::Private : Thread, Runnable
{
    Analyzer *p;

    bool mVidFrameTest;
    std::vector<Tiny::String> mFileNames;
    Tiny::String mError, mOutFileName;

    std::atomic<bool> mFinished, mCanceled;
    MemberCall<void(Private *)> mOnEngineMessage;

    AnalysisEngine mEngine;
    std::string mOverrideRequirementsScript, mOverrideInputScript, mOverrideAnalysisScript;
    std::string mGlobalResult;

    time_t mStartTime;

    Private(Analyzer *p)
        : p(p), Thread(this, ""), mFinished(false), mCanceled(false), mVidFrameTest(false), mStartTime(0), mOnEngineMessage(&Private::onEngineMessage, this)
    {
        mEngine.AddListener(AnalysisEngine::onEvaluationProgress, &mOnEngineMessage);
    }
    ~Private()
    {
    }
    bool open(const std::string &file);
    bool analyze();
    void determineGlobalResult();
    void OnRun() override;

    void onEngineMessage()
    {
        MessageArg outArg = {nullptr, 0, 0, ""};
        auto inArg = mEngine.Message().Argument<AnalysisEngine::MessageArg *>();
        switch (mEngine.Message().Id())
        {
        case AnalysisEngine::onEvaluationProgress:
            outArg.total = inArg->total;
            outArg.cur = inArg->current;
            p->Emit(onDataProgress, &outArg);
            break;
        }
    }
};

Analyzer::Analyzer() : p(new Private(this))
{
}

Analyzer::~Analyzer()
{
    delete p;
}

const Tiny::String &Analyzer::error() const
{
    return p->mError;
}

bool Analyzer::finished() const
{
    return p->mFinished;
}

void Analyzer::run()
{
    p->mFinished = false;
    p->mCanceled = false;
    p->mGlobalResult.clear();
    p->mEngine.reset();
    p->Thread::Start();
}

bool Analyzer::Private::open(const std::string &file)
{
    mError.Clear();
    BCI2000FileReader reader(file.c_str());
    if (!reader.IsOpen())
    {
        mError << "Could not open \"" << file << "\" as a BCI2000 data file.";
        return false;
    }

    std::string taskName = FileUtils::ExtractBase(file);
    if (!mEngine.open(file, taskName))
    {
        mError = mEngine.errors();
        return false;
    }
    if (reader.Parameters()->Exists("VidFrameTest"))
        mVidFrameTest = reader.Parameter("VidFrameTest");

    std::string inputscript = mOverrideInputScript;
    if (inputscript.empty() && reader.Parameters()->Exists("CertInputScript"))
    {
        std::istringstream iss;
        iss.str(reader.Parameter("CertInputScript"));
      iss.clear();
      StringUtils::ReadAsBase64(iss, inputscript);
    }
    if (inputscript.empty())
        inputscript = std::string(Resources::DefaultInputScript.data, Resources::DefaultInputScript.length);
    if (!mEngine.execute(inputscript, "input"))
    {
        mError << "Input definition script, " << mEngine.errors();
        return false;
    }
    return true;
}

bool Analyzer::Private::analyze()
{
    std::string analysisScript = mOverrideAnalysisScript;
    if (analysisScript.empty())
    {
        if (mVidFrameTest)
            analysisScript =
                std::string(Resources::VideoFrameAnalysisScript.data, Resources::VideoFrameAnalysisScript.length);
        else
            analysisScript =
                std::string(Resources::LatencyAnalysisScript.data, Resources::LatencyAnalysisScript.length);
    }
    if (analysisScript.empty())
        analysisScript = std::string(Resources::LatencyAnalysisScript.data, Resources::LatencyAnalysisScript.length);
    if (!mEngine.execute(analysisScript, "analysis"))
    {
        mError << "Analysis script, " << mEngine.errors();
        return false;
    }

    std::string requirementsScript = mOverrideRequirementsScript;
    if (requirementsScript.empty())
    {
        if (mVidFrameTest)
            requirementsScript =
                std::string(Resources::VideoFrameRequirements.data, Resources::VideoFrameRequirements.length);
        else
            requirementsScript =
                std::string(Resources::LatencyRequirements.data, Resources::LatencyRequirements.length);
    }
    if (!mEngine.execute(requirementsScript, "requirements"))
    {
        mError << "Requirements script, " << mEngine.errors();
        return false;
    }
    return true;
}

void Analyzer::Private::OnRun()
{
    mStartTime = ::time(nullptr);

    MessageArg arg = {nullptr, 0, 0, ""};
    arg.total = mFileNames.size();
    p->Emit(onBegin, &arg);
    bool ok = true;
    for (int i = 0; ok && i < mFileNames.size() && !Thread::Terminating(); ++i)
    {
        arg.info.Clear();
        arg.pFile = &mFileNames[i];
        std::string session = FileUtils::ExtractBase(mFileNames[i]);
        arg.info = "Opening " + session;
        p->Emit(onFileProgress, &arg);

        ok = open(mFileNames[i]);
        if (ok)
            arg.info = "Analyzing " + session;
        else
            arg.info = "Error opening " + session;
        p->Emit(onFileProgress, &arg);
        if (ok)
            ok = analyze();
        ++arg.cur;
    }
    if (ok && !mCanceled)
        mFinished = true;
    arg.pFile = nullptr;
    arg.info.Clear();
    determineGlobalResult();
    p->Emit(onDone, &arg);
    mCanceled = false;
}

bool Analyzer::init()
{
    std::string initscript = std::string(Resources::InitScript.data, Resources::InitScript.length);
    if (!p->mEngine.execute(initscript, "init"))
    {
        p->mError << "Initialization script, " << p->mEngine.errors();
        return false;
    }
    return true;
}

void Analyzer::setInputFiles(const std::vector<Tiny::String> &fileNames)
{
    p->mFinished = false;
    p->mFileNames = fileNames;
}

void Analyzer::setOverrideInputScript(const Tiny::String &script)
{
    p->mFinished = false;
    p->mOverrideInputScript = script;
}

void Analyzer::setOverrideAnalysisScript(const Tiny::String &script)
{
    p->mFinished = false;
    p->mOverrideAnalysisScript = script;
}

void Analyzer::setOverrideRequirementsScript(const Tiny::String &script)
{
    p->mFinished = false;
    p->mOverrideRequirementsScript = script;
}

void Analyzer::cancel()
{
    p->mCanceled = true;
    p->Thread::Terminate(Time::FromMilliseconds(0));
    p->mEngine.cancel();
    p->Thread::Wait();
}


void Analyzer::Private::determineGlobalResult()
{
    std::ostringstream oss;
    auto results = mEngine.results();
    int total = 0, passed = 0, failed = 0, missing = 0;
    for (auto p : results)
    {
        if (p->description != "global")
        {
            double r = 1.0;
            int nans = 0;
            for (const auto& q : p->requirements)
                if (IsNaN(q.value))
                    ++nans;
                else
                    r *= q.value;
            if (r && nans != p->requirements.size())
                ++passed;
            if (!r && nans != p->requirements.size())
                ++failed;
            if (nans > 0)
                ++missing;
            ++total;
        }
    }
    oss << passed << "/" << total << " tasks passed, "
        << failed << "/" << total << " tasks failed, "
        << missing << " tasks are missing data";
    mGlobalResult = oss.str();
}

const std::string& Analyzer::globalResult() const
{
    return p->mGlobalResult;
}

bool Analyzer::saveProtocol(const Tiny::String &filename) const
{
    auto results = this->results();
    const AnalysisEngine::SessionData *pGlobal = nullptr;
    for (auto p : results)
        if (p->description == "global")
            pGlobal = p;

    if (!pGlobal)
    {
        p->mError << "Analysis incomplete, no protocol saved";
        return false;
    }

    std::ofstream file(filename);
    if (!file.is_open())
    {
        p->mError << "Could not write protocol to " << filename;
        return false;
    }

    file << "Overall result: " << p->mGlobalResult << "\n\n";
    file << *pGlobal << "\n\n";
    for (auto p : results)
        if (p != pGlobal)
            file << *p << "\n";
    return true;
}

AnalysisEngine::Preferences &Analyzer::preferences()
{
    return p->mEngine.preferences();
}

const AnalysisEngine::Preferences &Analyzer::preferences() const
{
    return p->mEngine.preferences();
}

AnalysisEngine::Results Analyzer::results() const
{
    return p->mEngine.results();
}
