#include "inc/DateTime.h"
#include "DateTimeImplement.h"

#include <time.h>

namespace mwl {

    DateTime DateTime::DateTime::Now(TimeZone tz) {
        DateTime now(tz);
        _GetTimeOfDay(&now.m_pImpl->_tv);
        _Tv2Tm(&now.m_pImpl->_tv, &now.m_pImpl->_tm, now.m_pImpl->_utc);
        return now;
    }

    DateTime::DateTime(TimeZone tz)
    : m_pImpl(new Implement(tz == TZ_UTC)) {}

    DateTime::DateTime(const DateTime &src) 
    : m_pImpl(new Implement(src.m_pImpl->_utc)) {
        m_pImpl->_tv = src.m_pImpl->_tv;
        m_pImpl->_tm = src.m_pImpl->_tm;
    }

    DateTime::~DateTime() {
        delete m_pImpl;
    }

    DateTime& DateTime::operator=(const DateTime &rhs) {
        if (this != &rhs) {
            m_pImpl->_tv = rhs.m_pImpl->_tv;
            m_pImpl->_tm = rhs.m_pImpl->_tm;
            m_pImpl->_utc = rhs.m_pImpl->_utc;
        }
        return *this;
    }

    DateTime DateTime::operator+(const TimeSpan &offset) const {
        return DateTime(*this).operator+=(offset);
    }

    DateTime& DateTime::operator+=(const TimeSpan &offset) {
        int64_t us = offset.ToI64(MICROSEC);
        if (us < 0) {
            return operator-=(TimeSpan(static_cast<double>(-us), MICROSEC));
        } else if (us > 0) {
            m_pImpl->_tv.tv_sec += static_cast<long>(us / 1000000);
            m_pImpl->_tv.tv_usec += static_cast<long>(us % 1000000);
            m_pImpl->_tv.tv_sec += m_pImpl->_tv.tv_usec / 1000000;
            m_pImpl->_tv.tv_usec %= 1000000;
            _Tv2Tm(&m_pImpl->_tv, &m_pImpl->_tm, m_pImpl->_utc);
        }
        return *this;
    }

    TimeSpan DateTime::operator-(const DateTime &rhs) const {
        double deltaUs = m_pImpl->_tv.tv_usec - rhs.m_pImpl->_tv.tv_usec;
        double deltaSec = m_pImpl->_tv.tv_sec - rhs.m_pImpl->_tv.tv_sec;
        return TimeSpan(deltaSec * 1000000 + deltaUs, MICROSEC);
    }

    DateTime DateTime::operator-(const TimeSpan &offset) const {
        return DateTime(*this).operator-=(offset);
    }

    DateTime& DateTime::operator-=(const TimeSpan &offset) {
        int64_t us = offset.ToI64(MICROSEC);
        if (us < 0) {
            return operator+=(TimeSpan(static_cast<double>(-us), MICROSEC));
        } else if (us > 0) {
            m_pImpl->_tv.tv_usec -= us % 1000000;
            if (m_pImpl->_tv.tv_usec < 0) {
                m_pImpl->_tv.tv_sec -= 1;
                m_pImpl->_tv.tv_usec += 1000000;
            }
            m_pImpl->_tv.tv_sec -= static_cast<long>(us / 1000000);
            if (m_pImpl->_tv.tv_sec >= 0) {
                _Tv2Tm(&m_pImpl->_tv, &m_pImpl->_tm, m_pImpl->_utc);
            }
        }
        return *this;
    }

    bool DateTime::IsUTC() const {
        return m_pImpl->_utc;
    }

    DateTime DateTime::AsUTC() const {
        return DateTime(*this).ToUTC();
    }

    DateTime& DateTime::ToUTC() {
        if (!m_pImpl->_utc) {
            m_pImpl->_utc = true;
            _Tv2Tm(&m_pImpl->_tv, &m_pImpl->_tm, m_pImpl->_utc);
        }
        return *this;
    }

    bool DateTime::IsLocal() const {
        return !m_pImpl->_utc;
    }
    DateTime DateTime::AsLocal() const {
        return DateTime(*this).ToLocal();
    }

    DateTime& DateTime::ToLocal() {
        if (m_pImpl->_utc) {
            m_pImpl->_utc = false;
            _Tv2Tm(&m_pImpl->_tv, &m_pImpl->_tm, m_pImpl->_utc);
        }
        return *this;
    }

    int32_t DateTime::Year() const {
        return m_pImpl->_tm.tm_year + 1900;
    }

    int32_t DateTime::Month() const {
        return m_pImpl->_tm.tm_mon;
    }

    int32_t DateTime::YearDay() const {
        return m_pImpl->_tm.tm_yday;
    }

    int32_t DateTime::MonthDay() const {
        return m_pImpl->_tm.tm_mday;
    }

    int32_t DateTime::WeekDay() const {
        return m_pImpl->_tm.tm_wday;
    }

    int32_t DateTime::Hour() const {
        return m_pImpl->_tm.tm_hour;
    }

    int32_t DateTime::Minute() const {
        return m_pImpl->_tm.tm_min;
    }

    int32_t DateTime::Second() const {
        return m_pImpl->_tm.tm_sec;
    }

    bool DateTime::operator==(const DateTime &rhs) const {
        return this == &rhs ||
            (m_pImpl->_tv.tv_sec == rhs.m_pImpl->_tv.tv_sec &&
                m_pImpl->_tv.tv_usec == rhs.m_pImpl->_tv.tv_usec);
    }

    bool DateTime::operator!=(const DateTime &rhs) const {
        return !operator==(rhs);
    }

    bool DateTime::operator<(const DateTime &rhs) const {
         return m_pImpl->_tv.tv_sec < rhs.m_pImpl->_tv.tv_sec ||
             (m_pImpl->_tv.tv_sec == rhs.m_pImpl->_tv.tv_sec && 
                m_pImpl->_tv.tv_usec < rhs.m_pImpl->_tv.tv_usec);
    }

    bool DateTime::operator<=(const DateTime &rhs) const {
        return operator==(rhs) || operator<(rhs);
    }

    bool DateTime::operator>(const DateTime &rhs) const {
        return !operator==(rhs) && !operator<(rhs);
    }

    bool DateTime::operator>=(const DateTime &rhs) const {
        return !operator<(rhs);
    }

    const String& DateTime::DefaultFmt() const {
        return m_pImpl->_defaultFmt;
    }

    void DateTime::SetDefaultFmt(const String &defaultFmt) {
        m_pImpl->_defaultFmt = defaultFmt.Empty() ? s_defaultFmt : defaultFmt;
    }

    String DateTime::ToStr(const String &fmt) const {
        if (m_pImpl->_tv.tv_sec <= 0) {
            return String();
        }
        const char *pFmt = fmt.Empty() ? "%x %X" : fmt.C_Str();
        ByteArray buf(64);
        size_t r = strftime(reinterpret_cast<char*>(buf.Data()), buf.Size(), pFmt, &m_pImpl->_tm);
        int32_t tryCnt = 0;
        while (r == 0 && tryCnt < 3) {
            ++tryCnt;
            buf.Increase(buf.Size());
            r = strftime(reinterpret_cast<char*>(buf.Data()), buf.Size(), pFmt, &m_pImpl->_tm);
        }
        return String(reinterpret_cast<char*>(buf.Data()), static_cast<int32_t>(r));
    }

    int32_t DateTime::FromStr(const String &timeString, const String &fmt) {
        return m_pImpl->_FromStr(timeString, fmt);
    }

    void DateTime::Swap(DateTime & other) {
        std::swap(m_pImpl, other.m_pImpl);
    }

}
