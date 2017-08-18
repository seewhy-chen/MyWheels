#ifndef __MWL_DATETIME_H__
#define __MWL_DATETIME_H__

#include "inc/BasicDefines.h"
#include "inc/String.h"
#include "inc/SmartPointer.h"

namespace mwl {
    struct TimeDelta;

    class MWL_API DateTime {
    public:
        DateTime();
        DateTime(const DateTime &src);
        ~DateTime();
        DateTime &operator=(const DateTime &rhs);
        
        TimeDelta operator+(const DateTime &rhs) const;
        DateTime operator+(const TimeDelta &offset) const;
        DateTime& operator+=(const TimeDelta &offset) const;

        TimeDelta operator-(const DateTime &rhs) const;
        DateTime operator-(const TimeDelta &offset) const;
        DateTime& operator-=(const TimeDelta &offset) const;

        int32_t Year() const;
        int32_t Month() const;
        int32_t Day() const;
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

    private:
        struct Implement;
        SharedPtr<Implement> m_pImpl;
    };
}

#endif // __MWL_DATETIME_H__
