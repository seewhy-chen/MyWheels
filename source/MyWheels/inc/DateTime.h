#ifndef __MWL_DATETIME_H__
#define __MWL_DATETIME_H__

#include "inc/BasicDefines.h"
#include "inc/TimeSpan.h"
#include "inc/String.h"

namespace mwl {

    class MWL_API DateTime {
    public:
        static DateTime Now();
        DateTime();
        DateTime(const DateTime &src);
        ~DateTime();
        DateTime &operator=(const DateTime &rhs);
        
        DateTime operator+(const TimeSpan &offset) const;
        DateTime& operator+=(const TimeSpan &offset);

        TimeSpan operator-(const DateTime &rhs) const;
        DateTime operator-(const TimeSpan &offset) const;
        DateTime& operator-=(const TimeSpan &offset);

        int32_t Year() const;
        int32_t Month() const;
        int32_t MonthDay() const;
        int32_t WeekDay() const;
        int32_t Hour() const;
        int32_t Minute() const;
        int32_t Second() const;

        bool operator==(const DateTime &rhs) const;
        bool operator!=(const DateTime &rhs) const;
        bool operator<(const DateTime &rhs) const;
        bool operator<=(const DateTime &rhs) const;
        bool operator>(const DateTime &rhs) const;
        bool operator>=(const DateTime &rhs) const;

        String ToStr(const String &fmt = String::Null()) const;
        void Swap(DateTime &other);

    private:
        struct Implement;
        Implement *m_pImpl;
    };
}

namespace std {
    inline void swap(mwl::DateTime &d1, mwl::DateTime &d2) {
        d1.Swap(d2);
    }
}

#endif // __MWL_DATETIME_H__
