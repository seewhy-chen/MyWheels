#include "inc/BasicDefines.h"

#ifdef __MWL_LINUX__

#include "RWLockImplement.h"
#include <pthread.h>

namespace mwl {

    RWLock::Implement::Implement() {
        int32_t ret = pthread_rwlock_init(&_l, nullptr);
        if (ret) {
            MWL_WARN_ERRNO("init rwlock failed", ret);
        }
    }

    RWLock::Implement::~Implement() {
        int32_t ret = pthread_rwlock_destroy(&_l);
        if (ret) {
            MWL_WARN_ERRNO("destroy rwlock failed", ret);
        }
    }

    int32_t RWLock::Implement::_ReadLock() {
        int32_t ret = pthread_rwlock_rdlock(&_l);
        if (ret) {
            MWL_WARN_ERRNO("rwlock readlock failed", ret);
        }
        return -ret;
    }

    int32_t RWLock::Implement::_TryReadLock() {
        return -pthread_rwlock_tryrdlock(&_l);
    }

    int32_t RWLock::Implement::_ReadUnlock() {
        int32_t ret = pthread_rwlock_unlock(&_l);
        if (ret) {
            MWL_WARN_ERRNO("rwlock readunlock failed", ret);
        }
        return -ret;
    }

    int32_t RWLock::Implement::_WriteLock() {
        int32_t ret = pthread_rwlock_wrlock(&_l);
        if (ret) {
            MWL_WARN_ERRNO("rwlock writelock failed", ret);
        }
        return -ret;
    }

    int32_t RWLock::Implement::_TryWriteLock() {
        return -pthread_rwlock_trywrlock(&_l);
    }

    int32_t RWLock::Implement::_WriteUnlock() {
        int32_t ret = pthread_rwlock_unlock(&_l);
        if (ret) {
            MWL_WARN_ERRNO("rwlock writeunlock failed", ret);
        }
        return -ret;
    }
}

#endif // __MWL_LINUX__
