#include "inc/Semaphore.h"
#include "SemaphoreImplement.h"

namespace mwl {

    Semaphore::Semaphore() : m_pImpl(new Implement()) {}

    Semaphore::~Semaphore() {
        delete m_pImpl;
    }

    int32_t Semaphore::Open(const String &name, int32_t initVal) {
        return m_pImpl->_Open(name, initVal);
    }

    int32_t Semaphore::Wait() {
        return m_pImpl->_Wait(nullptr);
    }

    int32_t Semaphore::Wait(const TimeSpan &timeout) {
        return m_pImpl->_Wait(&timeout);
    }

    int32_t Semaphore::Post(int32_t n) {
        return m_pImpl->_Post(n);
    }

    int32_t Semaphore::Close() {
        return m_pImpl->_Close();
    }

}
