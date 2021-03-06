#include "inc/Mutex.h"
#include "MutexImplement.h"

namespace mwl {

    Mutex::Mutex(bool traceOwner) : m_pImpl(new Implement(traceOwner)) {}

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

    const ThreadID& Mutex::Owner() const {
        return m_pImpl->_owner;
    }

    Mutex::AutoLock::AutoLock(Mutex &mutex) : _mutex(mutex) {
        _mutex.Lock();
    }

    Mutex::AutoLock::~AutoLock() {
        _mutex.Unlock();
    }

    Mutex::Implement* Mutex::Impl() {
        return m_pImpl;
    }

}
