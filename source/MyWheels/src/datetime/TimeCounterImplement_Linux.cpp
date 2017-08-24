#include "inc/BasicDefines.h"

#ifdef __MWL_LINUX__

#include "TimeCounterImplement.h"

namespace mwl {

    TimeCounter::Implement::Implement() {}

    TimeCounter::Implement::~Implement() {}

    int32_t TimeCounter::Implement::_Start(const String &tag) {
        if (clock_gettime(CLOCK_MONOTONIC, &_counters[tag]) < 0) {
            int32_t err = errno;
            MWL_WARN("gettime for time counter '%s' failed: %s(%d)", tag.C_Str(), strerror(err), err);
            return -err;
        }
        return 0;
    }

    uint64_t TimeCounter::Implement::_TimeElapsed(const String &tag, TimeUnit unit) {
        if (_counters.empty()) {
            MWL_ERR("no started time counter");
            return 0;
        }

        timespec currTime;
        if (clock_gettime(CLOCK_MONOTONIC, &currTime) < 0) {
            int32_t err = errno;
            MWL_WARN("gettime failed: %s(%d)", strerror(err), err);
            return 0;
        }

        timespec startTime;
        if (_counters.end() == _counters.find(tag)) {
            MWL_ERR("No time counter has tag '%s'", tag.C_Str());
            return 0;
        }
        startTime = _counters[tag];
        uint64_t nanoSecElapsed = (currTime.tv_sec - startTime.tv_sec) * 1E9 + currTime.tv_nsec - startTime.tv_nsec;
        switch (unit) {
        case HOUR:
            return nanoSecElapsed / 1E9 / 3600;
        case MINUTE:
            return nanoSecElapsed / 1E9 / 60;
        case SECOND:
            return nanoSecElapsed / 1E9;
        default:
            MWL_WARN("unknown time unit %d", unit);
            // fall through
        case MILLISEC:
            return nanoSecElapsed / 1E6;
        case MICROSEC:
            return nanoSecElapsed / 1000;
        }
    }

}

#endif // __MWL_LINUX__
