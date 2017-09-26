#include "inc/BasicDefines.h"

#ifdef __MWL_WIN__

#include "RWLockImplement.h"

namespace mwl {
    RWLock::Implement::Implement() {
        InitializeSRWLock(&_l);
    }

    RWLock::Implement::~Implement() {
        if (_TryWriteLock() >= 0) {
            _WriteUnlock();
        } else {
            MWL_WARN_ERRNO("destroy rwlock failed", EBUSY);
        }
    }

    int32_t RWLock::Implement::_ReadLock() {
        AcquireSRWLockShared(&_l);
        return 0;
    }

    int32_t RWLock::Implement::_TryReadLock() {
        if (TryAcquireSRWLockShared(&_l)) {
            return 0;
        } else {
            return -EBUSY;
        }
    }

    int32_t RWLock::Implement::_ReadUnlock() {
        ReleaseSRWLockShared(&_l);
        return 0;
    }

    int32_t RWLock::Implement::_WriteLock() {
        AcquireSRWLockExclusive(&_l);
        return 0;
    }

    int32_t RWLock::Implement::_TryWriteLock() {
        if (TryAcquireSRWLockExclusive(&_l)) {
            return 0;
        } else {
            return -EBUSY;
        }
    }

    int32_t RWLock::Implement::_WriteUnlock() {
        ReleaseSRWLockExclusive(&_l);
        return 0;
    }
}

#endif // __MWL_WIN__
