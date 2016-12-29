
#ifdef __MWL_WIN__

#include "TimeCounterImplement.h"
#include <WinBase.h>
#include <string>

namespace mwl {

    TimeCounter::Implement::Implement() {
        QueryPerformanceFrequency(&_freq);
    }

    TimeCounter::Implement::~Implement() {}

    int32_t TimeCounter::Implement::_Start(const char *tag) {
        std::string perfTag(tag ? tag : "");
        if(QueryPerformanceCounter(&_counters[perfTag])) {
            return 0;
        } else {
            int32_t err = GetLastError();
            MWL_WARN_ERRNO("QueryPerformanceCounter for time counter '%s' failed", err);
            return -err;
        }
    }

    uint64_t TimeCounter::Implement::_TimeElapsed(const char *tag, TimeUnit unit) {
        if (_counters.empty()) {
            MWL_WARN("no time counter running");
            return 0;
        }

        LARGE_INTEGER currCount;
        if (!QueryPerformanceCounter(&currCount)) {
            int32_t err = GetLastError();
            MWL_WARN("QueryPerformanceCounter failed", err);
            return 0;
        }

        std::string currTag(tag ? tag : "");
        if (_counters.end() == _counters.find(currTag)) {
            MWL_WARN("No time counter has tag '%s'", currTag.c_str());
            return 0;
        }
        LARGE_INTEGER startCount = _counters[currTag];
        LONGLONG ticks = currCount.QuadPart - startCount.QuadPart;
        switch (unit) {
        case HOUR:
            return ticks / _freq.QuadPart / 3600;
        case MINUTE:
            return ticks / _freq.QuadPart / 60;
        case SECOND:
            return ticks / _freq.QuadPart;
        default:
            MWL_ERROR("unknown time unit %d", unit);
            // fall through
        case MILLISEC:
            return ticks * 1000 / _freq.QuadPart;
        case MICROSEC:
            return ticks * 1000 * 1000 / _freq.QuadPart;}
    }

}

#endif // __MWL_WIN__
