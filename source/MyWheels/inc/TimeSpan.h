#ifndef __MWL_TIME_SPAN_H__
#define __MWL_TIME_SPAN_H__

#include "inc/BasicDefines.h"
#include "inc/TimeDefines.h"

namespace mwl {

    class MWL_API TimeSpan {
    public:
        TimeSpan(double tv = 0, TimeUnit tu = MILLISEC);
        TimeSpan(const TimeSpan &src);
        ~TimeSpan();

        TimeSpan& operator=(const TimeSpan &rhs);
        TimeSpan operator+(const TimeSpan &rhs) const;
        TimeSpan& operator+=(const TimeSpan &rhs);
        TimeSpan operator-() const;
        TimeSpan operator-(const TimeSpan &rhs) const;
        TimeSpan& operator-=(const TimeSpan &rhs);

        bool operator==(const TimeSpan &rhs) const;
        bool operator!=(const TimeSpan &rhs) const;
        bool operator<(const TimeSpan &rhs) const;
        bool operator>(const TimeSpan &rhs) const;
        bool operator<=(const TimeSpan &rhs) const;
        bool operator>=(const TimeSpan &rhs) const;

        int32_t ToI32(TimeUnit tu) const;
        uint32_t ToU32(TimeUnit tu) const;
        int64_t ToI64(TimeUnit tu) const;
        uint64_t ToU64(TimeUnit tu) const;
        double ToDouble(TimeUnit tu) const;
    private:
        struct Implement;
        Implement *m_pImpl;
    };

}

#endif // __MWL_TIME_SPAN_H__
