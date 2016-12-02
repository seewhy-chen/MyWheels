#include "TimeCounterImplement.h"

#ifdef __LINUX__
namespace mwl {
    TimeCounter::Implement::Implement() {}

    TimeCounter::Implement::~Implement() {}

    int32_t TimeCounter::Implement::_Start(const char *tag) {
        std::string perfTag(tag ? tag : "");
        clock_gettime(CLOCK_MONOTONIC, &_counters[perfTag]);
        return 0;
    }

    uint64_t TimeCounter::Implement::_TimeElapsed(const char *tag, TimeUnit unit) {
        if (_counters.empty()) {
            MWL_ERR("no started time counter");
            return 0;
        }
        timespec currTime;
        clock_gettime(CLOCK_MONOTONIC, &currTime);
        std::string currTag(tag ? tag : "");
        timespec startTime;
        if (_counters.end() == _counters.find(currTag)) {
            startTime = _counters[""];
        } else {
            startTime = _counters[currTag];
        }

        uint64_t nanoSecElapsed = (currTime.tv_sec - startTime.tv_sec) * 1E9 + currTime.tv_nsec - startTime.tv_nsec;
        switch (unit) {
        case HOUR:
            return nanoSecElapsed / 1E9 / 3600;
        case MINUTE:
            return nanoSecElapsed / 1E9 / 60;
        case SECOND:
            return nanoSecElapsed / 1E9;
        default:
            MWL_ERR("unknown time unit %d", unit);
            // fall through
        case MILLISEC:
            return nanoSecElapsed / 100000;
        case MICROSEC:
            return nanoSecElapsed / 1000;
        }
    }
}

#endif // __LINUX__