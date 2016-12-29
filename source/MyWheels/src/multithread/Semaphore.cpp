#include "inc/Semaphore.h"
#include "SemaphoreImplement.h"

namespace mwl {

    Semaphore::Semaphore() : m_pImpl(new Implement()) {}

    Semaphore::~Semaphore() {
        delete m_pImpl;
    }

    int32_t Semaphore::Open(const char *name, int32_t initVal) {
        return m_pImpl->_Open(name, initVal);
    }

    int32_t Semaphore::Wait(int32_t timeoutInMs) {
        return m_pImpl->_Wait(timeoutInMs);
    }

    int32_t Semaphore::Post() {
        return m_pImpl->_Post();
    }

    int32_t Semaphore::Close() {
        return m_pImpl->_Close();
    }

}