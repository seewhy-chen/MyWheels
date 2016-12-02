#include "TimeCounterImplement.h"

#ifdef __WINDOWS__

#include <WinBase.h>
#include <string>

namespace mwl {

    TimeCounter::Implement::Implement() {
        QueryPerformanceFrequency(&_freq);
    }

    TimeCounter::Implement::~Implement() {}

    int32_t TimeCounter::Implement::_Start(const char *tag) {
        std::string perfTag(tag ? tag : "");
        return QueryPerformanceCounter(&_counters[perfTag]) ? 0 : -1;
    }

    uint64_t TimeCounter::Implement::_TimeElapsed(const char *tag, TimeUnit unit) {
        if (_counters.empty()) {
            MWL_ERR("no started time counter");
            return 0;
        }
        LARGE_INTEGER currCount;
        currCount.QuadPart = 0;
        LARGE_INTEGER startCount;
        startCount.QuadPart = 0;
        if (QueryPerformanceCounter(&currCount)) {
            std::string currTag(tag ? tag : "");
            if (_counters.end() == _counters.find(currTag)) {
                startCount = _counters[""];
            } else {
                startCount = _counters[currTag];
            }
        }

        LONGLONG ticks = currCount.QuadPart - startCount.QuadPart;
        switch (unit) {
        case HOUR:
            return ticks / _freq.QuadPart / 3600;
        case MINUTE:
            return ticks / _freq.QuadPart / 60;
        case SECOND:
            return ticks / _freq.QuadPart;
        default:
            MWL_ERR("unknown time unit %d", unit);
            // fall through
        case MILLISEC:
            return ticks * 1000 / _freq.QuadPart;
        case MICROSEC:
            return ticks * 1000 * 1000 / _freq.QuadPart;}
    }

}

#endif // __WINDOWS__
