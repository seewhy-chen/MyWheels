#ifndef __MWL_TIME_DEFINES_H__
#define __MWL_TIME_DEFINES_H__

#include "inc/BasicDefines.h"

namespace mwl {

    enum TimeUnit {
        HOUR,
        MINUTE,
        SECOND,
        MILLISEC,
        MICROSEC,
        NANOSEC,

        TimeUnitCount
    };

    class MWL_API TimeSpec {
    public:
        TimeSpec(double tv = 0, TimeUnit tu = MILLISEC);
        TimeSpec(const TimeSpec &src);
        TimeSpec& operator=(const TimeSpec &rhs);
        ~TimeSpec();
        int32_t ToI32(TimeUnit tu) const;
        uint32_t ToU32(TimeUnit tu) const;
        int64_t ToI64(TimeUnit tu) const;
        uint64_t ToU64(TimeUnit tu) const;
        double ToDouble(TimeUnit tu) const;
        bool operator==(const TimeSpec &rhs) const;
        bool operator!=(const TimeSpec &rhs) const;
        bool operator<(const TimeSpec &rhs) const;
        bool operator>(const TimeSpec &rhs) const;
        bool operator<=(const TimeSpec &rhs) const;
        bool operator>=(const TimeSpec &rhs) const;
    private:
        struct Implement;
        Implement *m_pImpl;
    };

    const TimeSpec MWL_PERMANANT(-1);

    void MWL_API TimeSleep(const TimeSpec &sleepTime);

}

#endif // __MWL_TIME_DEFINES_H__
