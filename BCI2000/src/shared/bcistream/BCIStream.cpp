////////////////////////////////////////////////////////////////////////////////
// $Id: BCIStream.cpp 8066 2024-04-23 22:47:26Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Declarations for stream symbols related to error/info messages.
//              To report an error, write e.g.
//               bcierr << "My error message"; // no std::endl required
//              For an informational message, write
//               bciout << "My info message"; // no std::endl required
//              To report an error and abort the current operation, write
//               throw bcierr << "My error message";
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
#include "BCIStream.h"

#include "ParamList.h"
#include "ParamRef.h"
#include "ThreadUtils.h"
#include "SynchronizedObject.h"
#include <ctime>
#include <set>

namespace BCIStream
{

class MessageDispatcher : public Dispatcher
{
    static const int cMaxTimeDiff = 1; // s

  public:
    MessageDispatcher();
    ~MessageDispatcher();

  protected:
    void OnCompress(BCIStream::Action, const std::string &);
    void OnCheck();
    int OnExecute();

  private:
    void ReportRepetitions();

    static std::set<MessageDispatcher *> &Instances();
    static LockableObject &GlobalMutex();

    int mCount;
    time_t mLastTime;
    BCIStream::Action mPrevAction;
    std::string mPrevMessage;
};

} // namespace BCIStream

// Definitions of the actual stream objects.
static BCIStream::OutStream bcierr____(&BCIStream::RuntimeError);
static BCIStream::OutStream bciwarn____(&BCIStream::Warning);
static BCIStream::OutStream bciout____(&BCIStream::PlainMessage, 0);
static BCIStream::OutStream bcidbg____(&BCIStream::DebugMessage, 0);
static BCIStream::OutStream bcinote____(&BCIStream::NoteMessage);
static BCIStream::OutStream *spBcierr = &bcierr____;
static BCIStream::OutStream *spBciwarn = &bciwarn____;
static BCIStream::OutStream *spBciout = &bciout____;
static BCIStream::OutStream *spBcidbg = &bcidbg____;
static BCIStream::OutStream* spBcinote = &bcinote____;

thread_local std::list<std::string> BCIStream::OutStream::stContext;

static std::streambuf *sb_cerr, *sb_clog, *sb_cout, *sb_bcierr, *sb_bciout, *sb_bciwarn, *sb_bcidbg;

static bool SetupRedirection()
{
    sb_cerr = std::cerr.rdbuf();
    sb_clog = std::clog.rdbuf();
    sb_cout = std::cout.rdbuf();
    sb_bcierr = bcierr__.rdbuf();
    sb_bciout = bciout__.rdbuf();
    sb_bciwarn = bciwarn__.rdbuf();
    sb_bcidbg = bcidbg__.rdbuf();
    return BCIStream::InitialRedirection();
}
static bool sRedirectionsInitializer = SetupRedirection();

bool BCIStream::RedirectFromStdio()
{
    bcierr__.rdbuf(sb_bcierr);
    bciout__.rdbuf(sb_bciout);
    bciwarn__.rdbuf(sb_bciwarn);
    bcidbg__.rdbuf(sb_bcidbg);

    std::cerr.rdbuf(sb_bciwarn);
    std::cout.rdbuf(sb_bciout);
    std::clog.rdbuf(sb_bciout);
    return true;
}

bool BCIStream::RedirectToStdio()
{
    std::cerr.rdbuf(sb_cerr);
    std::cout.rdbuf(sb_cout);
    std::clog.rdbuf(sb_clog);

    bcierr__.rdbuf(sb_cerr);
    bciout__.rdbuf(sb_cout);
    bciwarn__.rdbuf(sb_clog);
    return true;
}

bool BCIStream::ResetRedirection()
{
    bcierr__.rdbuf(sb_bcierr);
    bciout__.rdbuf(sb_bciout);
    bciwarn__.rdbuf(sb_bciwarn);
    bcidbg__.rdbuf(sb_bcidbg);

    std::cerr.rdbuf(sb_cerr);
    std::cout.rdbuf(sb_cout);
    std::clog.rdbuf(sb_clog);
    return true;
}

void BCIStream::Apply(const ParamList &p)
{
    if (p.Exists("Verbosity"))
    {
        int v = p("Verbosity");
        bciout__.SetVerbosity(v);
        bciwarn__.SetVerbosity(v);
    }
    if (p.Exists("DebugLevel"))
        bcidbg__.SetVerbosity(p("DebugLevel"));
}

LockableObject &BCIStream::MessageDispatcher::GlobalMutex()
{
    static Lockable<> instance;
    return instance;
}

namespace BCIStream
{

// OutStream
OutStream::OutStream(Action f, int level)
    : std::ostream(0), mVerbosityLevel(level), mVerbosityLocked(false), mpDispatcher(0)
{
    this->init(&mBuf);
    SetAction(f);
}

OutStream::~OutStream()
{
    mBuf.SetDispatcher(0);
    delete mpDispatcher;
}

void OutStream::SetAction(Action inAction)
{
    if (mpDispatcher && mpDispatcher->Action() != inAction)
    {
        mBuf.SetDispatcher(0);
        delete mpDispatcher;
        mpDispatcher = 0;
    }
    if (mpDispatcher)
    {
        mpDispatcher->Idle();
    }
    else
    {
        mpDispatcher = CompressMessages() ? new MessageDispatcher : new Dispatcher;
        mpDispatcher->SetAction(inAction);
        mBuf.SetDispatcher(mpDispatcher);
    }
}

OutStream &OutStream::MessageContext(const std::string &inContext)
{
    if (!ThreadUtils::InMainThread() || stContext.empty())
        mBuf.SetContext(inContext);
    else
        mBuf.SetContext(stContext);
    return ResetFormat();
}

OutStream &OutStream::ResetFormat()
{
    std::ostream defaultFormat(nullptr);
    this->copyfmt(defaultFormat);
    mVerbosityLocked = false;
    return *this;
}

OutStream &OutStream::MessageVerbosity(int inLevel)
{
    if (!mVerbosityLocked)
    {
        if (mVerbosityLevel >= inLevel)
            rdbuf(&mBuf);
        else
            rdbuf(NULL);
        mVerbosityLocked |= (inLevel == AlwaysDisplayMessage);
        mVerbosityLocked |= (inLevel == NeverDisplayMessage);
    }
    return *this;
}

void OutStream::SetContext(const std::string &s)
{
    if (s.empty())
    {
        if (!OutStream::stContext.empty())
            OutStream::stContext.pop_back();
    }
    else
        OutStream::stContext.push_back(s);
}

// OutStream::StringBuf
OutStream::StringBuf::StringBuf() : std::stringbuf(std::ios_base::out), mpDispatcher(0), mNumFlushes(0)
{
}

void OutStream::StringBuf::SetContext(const std::list<std::string> &s)
{
    mContext.clear();
    if (!s.empty())
        for (std::list<std::string>::const_iterator i = s.begin(); i != s.end(); ++i)
            mContext += *i + ": ";
}

void OutStream::StringBuf::SetContext(const std::string &s)
{
    if (s.empty())
        mContext.clear();
    else
        mContext = s + ": ";
}

void OutStream::StringBuf::SetDispatcher(Dispatcher *inpDispatcher)
{
    bool previous = mpDispatcher, empty = str().empty();
    if (previous && !empty)
        Flush();
    mpDispatcher = inpDispatcher;
    if (!previous && !empty)
        Flush();
}

void OutStream::StringBuf::DoFlush(std::string &s)
{
    ++mNumFlushes;
    if (!s.empty())
    {
        str("");
        if (s == "\n")
            s = "<empty message>";
        else
            for (size_t pos = s.find('\0'); pos != std::string::npos; pos = s.find('\0', pos))
                s = s.substr(0, pos) + s.substr(pos + 1);

        if (mpDispatcher)
            mpDispatcher->Dispatch(mContext, s);
    }
}

void OutStream::StringBuf::Flush()
{
    std::string s = str();
    DoFlush(s);
}

int OutStream::StringBuf::sync()
{
    if (pptr() == pbase())
        return 0;

    int r = std::stringbuf::sync();
    std::string s = str();
    if (!s.empty() && s.back() == '\n')
        DoFlush(s);
    return r;
}

// Dispatcher
void Dispatcher::Dispatch(const std::string &inContext, const std::string &inMessage)
{
    BCIStream::Action action = mAction;
    std::string message = inMessage;
    OnFilter(action, message);
    OnCompress(action, inContext + message);
}

void Dispatcher::OnFilter(BCIStream::Action &ioAction, std::string &ioMessage)
{
    if (!ioMessage.empty() && *ioMessage.rbegin() == '\n')
        ioMessage.erase(ioMessage.length() - 1);
    if (!ioMessage.empty() && !::ispunct(*ioMessage.rbegin()))
        ioMessage += '.';

    if (ioAction == &Warning || ioAction == &PlainMessage)
    {
        static const std::string warning = "warning";
        if (!::stricmp(ioMessage.substr(0, warning.length()).c_str(), warning.c_str()))
        {
            size_t pos = warning.length();
            while (pos < ioMessage.length() && (::ispunct(ioMessage[pos]) || ::isspace(ioMessage[pos])))
                ++pos;
            ioMessage = ioMessage.substr(pos);
            ioAction = &Warning;
        }
    }
}

// MessageDispatcher
MessageDispatcher::MessageDispatcher() : mPrevAction(LogicError), mLastTime(::time(0)), mCount(0)
{
    WithLock(GlobalMutex()) Instances().insert(this);
}

MessageDispatcher::~MessageDispatcher()
{
    ReportRepetitions();
    WithLock(GlobalMutex()) Instances().erase(this);
}

void MessageDispatcher::OnCompress(BCIStream::Action inAction, const std::string &inMessage)
{
    WithLock(GlobalMutex())
      for (std::set<MessageDispatcher *>::const_iterator i = Instances().begin();
                                 i != Instances().end(); ++i) 
        if (*i != this)
            (*i)->ReportRepetitions();
    SyncMemory<>();
    if (inMessage != mPrevMessage)
    {
        ReportRepetitions();
        if (inAction)
            inAction(inMessage);
        mPrevAction = inAction;
        mPrevMessage = inMessage;
    }
    else
    {
        ++mCount;
        if (::difftime(::time(0), mLastTime) >= cMaxTimeDiff)
            ReportRepetitions();
    }
    SyncMemory<>();
}

void MessageDispatcher::ReportRepetitions()
{
    WithLock(GlobalMutex())
    {
        if (mCount > 0)
        {
            std::string message = mPrevMessage;
            if (mCount > 1)
            {
                std::ostringstream oss;
                oss << " (";
                switch (mCount)
                {
                case 2:
                    oss << "twice";
                    break;
                default:
                    oss << mCount << " times";
                }
                oss << ")";
                message += oss.str();
            }
            if (mPrevAction)
                mPrevAction(message);
        }
        mCount = 0;
        mLastTime = ::time(0);
    }
}

void MessageDispatcher::OnCheck()
{
    ReportRepetitions();
}

std::set<MessageDispatcher *> &MessageDispatcher::Instances()
{
    static std::set<MessageDispatcher *> s;
    return s;
}

} // namespace BCIStream

BCIStream::OutStream &bcierr___()
{
    return *spBcierr;
}

void bcierr___(BCIStream::OutStream &s)
{
    spBcierr = &s;
}

BCIStream::OutStream &bciout___()
{
    return *spBciout;
}

void bciout___(BCIStream::OutStream &s)
{
    spBciout = &s;
}

BCIStream::OutStream &bciwarn___()
{
    return *spBciwarn;
}

void bciwarn___(BCIStream::OutStream &s)
{
    spBciwarn = &s;
}

BCIStream::OutStream &bcidbg___()
{
    return *spBcidbg;
}

void bcidbg___(BCIStream::OutStream &s)
{
    spBcidbg = &s;
}

BCIStream::OutStream& bcinote___()
{
    return *spBcinote;
}

void bcinote___(BCIStream::OutStream& s)
{
    spBcinote = &s;
}
