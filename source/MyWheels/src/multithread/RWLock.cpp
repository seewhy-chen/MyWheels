#include "inc/RWLock.h"
#include "RWLockImplement.h"

namespace mwl {

    RWLock::RWLock() : m_pImpl(new Implement()) {
    }

    RWLock::~RWLock() {
        delete m_pImpl;
    }

    int32_t RWLock::ReadLock() {
        return m_pImpl->_ReadLock();
    }

    int32_t RWLock::TryReadLock() {
        return m_pImpl->_TryReadLock();
    }

    int32_t RWLock::ReadUnlock() {
        return m_pImpl->_ReadUnlock();
    }

    int32_t RWLock::WriteLock() {
        return m_pImpl->_WriteLock();
    }

    int32_t RWLock::TryWriteLock() {
        return m_pImpl->_TryWriteLock();
    }

    int32_t RWLock::WriteUnlock() {
        return m_pImpl->_WriteUnlock();
    }

    RWLock::AutoReadLock::AutoReadLock(RWLock &rwLock) : _rwLock(rwLock) {
        _rwLock.ReadLock();
    }

    RWLock::AutoReadLock::~AutoReadLock() {
        _rwLock.ReadUnlock();
    }

    RWLock::AutoWriteLock::AutoWriteLock(RWLock &rwLock) : _rwLock(rwLock) {
        _rwLock.WriteLock();
    }

    RWLock::AutoWriteLock::~AutoWriteLock() {
        _rwLock.WriteUnlock();
    }
}
