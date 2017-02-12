#include "inc/TimeDefines.h"
#include "TimeCounterImplement.h"

#ifdef __MWL_WIN__
#else
#include <unistd.h> // for usleep
#endif

namespace mwl {

    static const double s_timeUnitScale[TimeUnitCount][TimeUnitCount] = {
        {1.0, 60.0, 60.0*60, 60.0*60*1000, 60.0*60*1000*1000, 60.0*60*1000*1000*1000},
        {1.0/60, 1.0, 60.0, 60.0*1000, 60.0*1000*1000, 60.0*1000*1000*1000},
        {1.0/60/60, 1.0/60, 1.0, 1000.0, 1000.0*1000, 1000.0*1000*1000},
        {1.0/60/60/1000, 1.0/60/1000, 1.0/1000, 1.0, 1000.0, 1000.0*1000},
        {1.0/60/60/1000/1000, 1.0/60/1000/1000, 1.0/1000/1000, 1.0/1000, 1.0, 1000.0},
        {1.0/60/60/1000/1000/1000, 1.0/60/1000/1000/1000, 1.0/1000/1000/1000, 1.0/1000/1000, 1.0/1000, 1.0},
    };

    static inline double _TimeUnitConvert(double tv, TimeUnit srcUnit, TimeUnit dstUnit) {
        if (srcUnit < 0 || srcUnit >= TimeUnitCount) {
            srcUnit = MILLISEC;
        }
        if (dstUnit < 0 || dstUnit >= TimeUnitCount) {
            dstUnit = MILLISEC;
        }
        if (srcUnit == dstUnit) {
            return tv;
        }
        return tv*s_timeUnitScale[srcUnit][dstUnit];
    }

    struct TimeSpec::Implement {
        Implement(double tv, TimeUnit tu) {
            if (0 <= tu && tu < TimeUnitCount) {
                nsVal = _TimeUnitConvert(tv, tu, NANOSEC); 
            } else{
                nsVal = 0;
            }
        }
        double nsVal; // timeval in nanosecond
    };

    TimeSpec::TimeSpec(double tv, TimeUnit tu)
    : m_pImpl(new Implement(tv, tu)) {}

    TimeSpec::~TimeSpec() {
        delete m_pImpl;
    }

    int32_t TimeSpec::ToI32(TimeUnit tu) const {
        if (0 <= tu && tu < TimeUnitCount) {
            return static_cast<int32_t>(_TimeUnitConvert(m_pImpl->nsVal, NANOSEC, tu));
        } else {
            return 0;
        }
    }

    uint32_t TimeSpec::ToU32(TimeUnit tu) const {
        if (0 <= tu && tu < TimeUnitCount) {
            return static_cast<uint32_t>(_TimeUnitConvert(m_pImpl->nsVal, NANOSEC, tu));
        } else {
            return 0;
        }
    }

    int64_t TimeSpec::ToI64(TimeUnit tu) const {
        if (0 <= tu && tu < TimeUnitCount) {
            return static_cast<int64_t>(_TimeUnitConvert(m_pImpl->nsVal, NANOSEC, tu));
        } else {
            return 0;
        }
    }

    uint64_t TimeSpec::ToU64(TimeUnit tu) const {
        if (0 <= tu && tu < TimeUnitCount) {
            return static_cast<uint64_t>(_TimeUnitConvert(m_pImpl->nsVal, NANOSEC, tu));
        } else {
            return 0;
        }
    }

    double TimeSpec::ToDouble(TimeUnit tu) const {
        if (0 <= tu && tu < TimeUnitCount) {
            return _TimeUnitConvert(m_pImpl->nsVal, NANOSEC, tu);
        } else {
            return 0;
        }
    }

    void TimeSleep(const TimeSpec &sleepTime) {
#ifdef __MWL_WIN__
        Sleep(static_cast<DWORD>(sleepTime.ToU64(MILLISEC)));
#else
        uint64_t nanoSec = sleepTime.ToU64(NANOSEC);
        struct timespec request, remain;
        request.tv_sec = nanoSec / 1000000000;
        request.tv_nsec = nanoSec % 1000000000;
        int32_t ret = nanosleep(&request, &remain);
        while(ret < 0 && EINTR == errno) {
            request = remain;
            ret = nanosleep(&request, &remain);
        }
        if (ret < 0) {
            int32_t err = errno;
            MWL_WARN_ERRNO("nanosleep failed", err);
        }
#endif
    }

}
