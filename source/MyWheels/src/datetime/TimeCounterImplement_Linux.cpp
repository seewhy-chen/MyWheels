#include "BasicDefines.h"

#ifdef __MWL_LINUX__

#include "TimeCounterImplement.h"

namespace mwl {

    TimeCounter::Implement::Implement() {}

    TimeCounter::Implement::~Implement() {}

    int32_t TimeCounter::Implement::_Start(const char *tag) {
        std::string perfTag(tag ? tag : "");
        if (clock_gettime(CLOCK_MONOTONIC, &_counters[perfTag]) < 0) {
            int32_t err = errno;
            MWL_WARN("gettime for time counter '%s' failed: %s(%d)", perfTag.c_str(), strerror(err), err);
            return -err;
        }
        return 0;
    }

    uint64_t TimeCounter::Implement::_TimeElapsed(const char *tag, TimeUnit unit) {
        if (_counters.empty()) {
            MWL_ERROR("no started time counter");
            return 0;
        }

        timespec currTime;
        if (clock_gettime(CLOCK_MONOTONIC, &currTime) < 0) {
            int32_t err = errno;
            MWL_WARN("gettime failed: %s(%d)", strerror(err), err);
            return 0;
        }

        std::string currTag(tag ? tag : "");
        timespec startTime;
        if (_counters.end() == _counters.find(currTag)) {
            MWL_ERROR("No time counter has tag '%s'", currTag.c_str());
            return 0;
        }
        startTime = _counters[currTag];
        uint64_t nanoSecElapsed = (currTime.tv_sec - startTime.tv_sec) * 1E9 + currTime.tv_nsec - startTime.tv_nsec;
        switch (unit) {
        case HOUR:
            return nanoSecElapsed / 1E9 / 3600;
        case MINUTE:
            return nanoSecElapsed / 1E9 / 60;
        case SECOND:
            return nanoSecElapsed / 1E9;
        default:
            MWL_ERROR("unknown time unit %d", unit);
            // fall through
        case MILLISEC:
            return nanoSecElapsed / 1E6;
        case MICROSEC:
            return nanoSecElapsed / 1000;
        }
    }

}

#endif // __MWL_LINUX__
