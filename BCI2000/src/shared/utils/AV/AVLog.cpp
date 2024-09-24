//////////////////////////////////////////////////////////////////////
// $Id: AVLog.cpp 8197 2024-06-18 14:03:58Z mellinger $
// Authors: mellinger@neurotechcenter.org
// Description: An interface to the libav logging facility.
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
///////////////////////////////////////////////////////////////////////
#include "AVLog.h"

#include "BCIStream.h"
#include "StringUtils.h"

#include <vector>
#include <algorithm>

extern "C" {
#include <libavutil/opt.h>
}
static void AVLogCallback(void* inContext, int level, const char* inFormat, va_list inArgs);

static std::mutex sMutex;
static std::vector<AVLogFilter*> sInstances;
static int sClientCount = 0;

static void AVLogCallback(void* inContext, int level, const char* inFormat, va_list inArgs)
{
    char buf[1024];
    int len = sizeof(buf) - 1, pos = 0;
    if (inContext) {
        pos += ::snprintf(buf + pos, len - pos, "%s@%p: ", ::av_default_item_name(inContext), inContext);
    }
    if (inFormat) {
        pos += ::vsnprintf(buf + pos, len - pos, inFormat, inArgs);
    }
    buf[pos] = 0;
    std::string message = StringUtils::Trim(buf);

    std::lock_guard<std::mutex> lock(sMutex);
    for (auto pInstance : sInstances) {
        if (pInstance->FilterFunction(inContext, level, inFormat, inArgs)) {
            return;
        }
    }
    if (sClientCount > 0) {
        switch (level) {
        case AV_LOG_DEBUG:
            bcidbg__(10) << message;
            break;
        case AV_LOG_WARNING:
        case AV_LOG_ERROR:
            bciout__ << message;
            break;
        case AV_LOG_FATAL:
            bcierr__ << message;
            break;
        }
    }
}

void AVLog::AddClient()
{
    std::lock_guard<std::mutex> lock(sMutex);
    if (++sClientCount == 1) {
        ::av_log_set_level(AV_LOG_DEBUG);
        ::av_log_set_callback(&AVLogCallback);
    }
}

void AVLog::RemoveClient()
{
    std::lock_guard<std::mutex> lock(sMutex);
    --sClientCount;
}

// AVLogFilter
AVLogFilter::AVLogFilter(int level, void* pContext)
: mLevel(level), mpContext(pContext)
{
    AVLog::AddClient();
    std::lock_guard<std::mutex> lock(sMutex);
    sInstances.push_back(this);
}

AVLogFilter::~AVLogFilter()
{
    AVLog::RemoveClient();
    std::lock_guard<std::mutex> lock(sMutex);
    auto i = std::find(sInstances.begin(), sInstances.end(), this);
    if (i != sInstances.end())
        sInstances.erase(i);
}

bool AVLogFilter::FilterFunction(void* pContext, int level, const char* pFormat, va_list args)
{
    if (level != mLevel)
        return false;
    if (mpContext != nullptr && mpContext != pContext)
        return false;
    char buf[1024] = "";
    ::vsnprintf(buf, sizeof(buf) - 1, pFormat, args);
    mLogData += buf;
    return false; // only return true to prevent processing by concurrent logs
}

const std::string& AVLogFilter::LogData() const
{
    return mLogData;
}
