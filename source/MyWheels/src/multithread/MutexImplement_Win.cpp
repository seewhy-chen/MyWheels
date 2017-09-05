#include "inc/BasicDefines.h"

#ifdef __MWL_WIN__

#include "MutexImplement.h"

namespace mwl {

    Mutex::Implement::Implement() {
        InitializeSRWLock(&m);
    }

    Mutex::Implement::~Implement() {
        if (TryAcquireSRWLockExclusive(&m)) {
            _Unlock();
        } else {
            MWL_WARN_ERRNO("destroy mutex failed", EBUSY);
        }
    }

    int32_t Mutex::Implement::_Lock() {
        AcquireSRWLockExclusive(&m);
        return 0;
    }

    int32_t Mutex::Implement::_TryLock() {
        if (TryAcquireSRWLockExclusive(&m)) {
            return 0;
        } else {
            return ERR_BUSY;
        }
    }

    int32_t Mutex::Implement::_Unlock() {
        ReleaseSRWLockExclusive(&m);
        return 0;
    }

}

#endif // __MWL_WIN__
