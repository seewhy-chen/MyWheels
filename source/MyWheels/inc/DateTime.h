#ifndef __MWL_DATETIME_H__
#define __MWL_DATETIME_H__

#include "inc/BasicDefines.h"
#include "inc/TimeSpan.h"
#include "inc/String.h"

namespace mwl {

    class MWL_API DateTime {
    public:
        // TODO: replace isUTC/isLocal with TimeZone
        enum TimeZone {
            TZ_UTC,
            TZ_LOCAL
        };

        static DateTime Now(TimeZone tz = TZ_LOCAL);
        explicit DateTime(TimeZone tz = TZ_LOCAL);
        DateTime(const DateTime &src);
        ~DateTime();
        DateTime &operator=(const DateTime &rhs);
        
        DateTime operator+(const TimeSpan &offset) const;
        DateTime& operator+=(const TimeSpan &offset);

        TimeSpan operator-(const DateTime &rhs) const;
        DateTime operator-(const TimeSpan &offset) const;
        DateTime& operator-=(const TimeSpan &offset);

        bool IsUTC() const;
        DateTime AsUTC() const;
        DateTime& ToUTC();

        bool IsLocal() const;
        DateTime AsLocal() const;
        DateTime& ToLocal();

        int32_t Year() const;
        // [0, 11]
        int32_t Month() const;
        // [0, 365]
        int32_t YearDay() const;
        // [1, 31]
        int32_t MonthDay() const;
        // [0, 6]
        int32_t WeekDay() const;
        // [0, 23]
        int32_t Hour() const;
        // [0, 59]
        int32_t Minute() const;
        // [0, 60]
        int32_t Second() const;

        bool operator==(const DateTime &rhs) const;
        bool operator!=(const DateTime &rhs) const;
        bool operator<(const DateTime &rhs) const;
        bool operator<=(const DateTime &rhs) const;
        bool operator>(const DateTime &rhs) const;
        bool operator>=(const DateTime &rhs) const;

        const String& DefaultFmt() const;
        void SetDefaultFmt(const String &defaultFmt);
        String ToStr(const String &fmt = String::Null()) const;
        int32_t FromStr(const String &timeString, const String &fmt);
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
