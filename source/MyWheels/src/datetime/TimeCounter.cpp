#include "inc/TimeCounter.h"
#include "TimeCounterImplement.h"

namespace mwl {

    TimeCounter::TimeCounter() : m_pImpl(new Implement) {}

    TimeCounter::~TimeCounter() {
        delete m_pImpl;
    }

    int32_t TimeCounter::Start(const String &tag) {
        return m_pImpl->_Start(tag);
    }

    uint64_t TimeCounter::TimeElapsed(TimeUnit unit) {
        return m_pImpl->_TimeElapsed(nullptr, unit);
    }

    uint64_t TimeCounter::TimeElapsed(const String &tag, TimeUnit unit) {
        return m_pImpl->_TimeElapsed(tag, unit);
    }

}
