#include "Mutex.h"
#include "MutexImplement.h"

namespace mwl {
    Mutex::Mutex() : m_pImpl(new Implement) {}

    Mutex::~Mutex() {
        delete m_pImpl;
    }

    int32_t Mutex::Lock() {
        return m_pImpl->_Lock();
    }

    int32_t Mutex::TryLock() {
        return m_pImpl->_TryLock();
    }

    int32_t Mutex::Unlock() {
        return m_pImpl->_Unlock();
    }

}
