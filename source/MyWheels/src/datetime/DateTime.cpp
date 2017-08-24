#include "inc/DateTime.h"
#include "DateTimeImplement.h"

namespace mwl {

    DateTime DateTime::Now() {
        return DateTime();
    }

    DateTime::DateTime() : m_pImpl(new Implement) {
    }

    DateTime::DateTime(const DateTime &/*src*/) 
    : m_pImpl(new Implement()) {
    }

    DateTime::~DateTime() {
        delete m_pImpl;
    }

    DateTime& DateTime::operator=(const DateTime &/*rhs*/) {
        return *this;
    }

    DateTime DateTime::operator+(const TimeSpan &offset) const {
        return DateTime(*this).operator+=(offset);
    }

    DateTime& DateTime::operator+=(const TimeSpan &/*offset*/) {
        return *this;
    }

    TimeSpan DateTime::operator-(const DateTime &/*rhs*/) const {
        return TimeSpan();
    }

    DateTime DateTime::operator-(const TimeSpan &offset) const {
        return DateTime(*this).operator-=(offset);
    }

    DateTime& DateTime::operator-=(const TimeSpan &/*offset*/) {
        return *this;
    }

    int32_t DateTime::Year() const {
        return 0;
    }

    int32_t DateTime::Month() const {
        return 0;
    }

    int32_t DateTime::MonthDay() const {
        return 0;
    }

    int32_t DateTime::WeekDay() const {
        return 0;
    }

    int32_t DateTime::Hour() const {
        return 0;
    }

    int32_t DateTime::Minute() const {
        return 0;
    }

    int32_t DateTime::Second() const {
        return 0;
    }


    bool DateTime::operator==(const DateTime &/*rhs*/) const {
        return true;
    }

    bool DateTime::operator!=(const DateTime &/*rhs*/) const {
        return true;
    }

    bool DateTime::operator<(const DateTime &/*rhs*/) const {
        return true;
    }

    bool DateTime::operator<=(const DateTime &/*rhs*/) const {
        return true;
    }

    bool DateTime::operator>(const DateTime &/*rhs*/) const {
        return true;
    }

    bool DateTime::operator>=(const DateTime &/*rhs*/) const {
        return true;
    }

    String DateTime::ToStr(const String &/*fmt*/) const {
        return String();
    }

    void DateTime::Swap(DateTime & other) {
        std::swap(m_pImpl, other.m_pImpl);
    }

}
