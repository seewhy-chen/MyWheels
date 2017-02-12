#include "inc/Condition.h"
#include "ConditionImplement.h"

namespace mwl {

    Condition::Condition() : m_pImpl(new Implement()) {}

    Condition::~Condition() {
        delete m_pImpl;
    }

    int32_t Condition::Wait(Mutex &mutex, const TimeSpec &timeout) {
        return m_pImpl->_Wait(mutex, timeout);
    }

    int32_t Condition::Signal() {
        return m_pImpl->_Signal();
    }

    int32_t Condition::Broadcast() {
        return m_pImpl->_Broadcast();
    }

}
