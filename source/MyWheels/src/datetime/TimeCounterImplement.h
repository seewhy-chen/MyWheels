#ifndef __MWL_TIME_COUNTER_IMPLEMENT_H__
#define __MWL_TIME_COUNTER_IMPLEMENT_H__

#include "TimeCounter.h"
#include "InternalCommon.h"

#include <map>

namespace mwl {

    struct TimeCounter::Implement {
        Implement();
        ~Implement();
        int32_t _Start(const char *tag);
        uint64_t _TimeElapsed(const char *tag, TimeUnit unit = MILLISEC);
#ifdef __MWL_WIN__
        LARGE_INTEGER _freq;
        std::map<std::string, LARGE_INTEGER> _counters;
#else
        std::map<std::string, timespec> _counters;
#endif // __MWL_WIN__
    };

}

#endif // __MWL_TIMECOUNTER_IMPLEMENT_H__
