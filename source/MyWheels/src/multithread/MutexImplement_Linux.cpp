#include "inc/BasicDefines.h"

#ifdef __MWL_LINUX__

#include "MutexImplement.h"
#include <pthread.h>

namespace mwl {

    Mutex::Implement::Implement(bool traceOwner) : _traceOwner(traceOwner) {
        int32_t ret = pthread_mutex_init(&m, nullptr);
        if (ret) {
            MWL_WARN_ERRNO("init mutex failed", ret);
        }
    }

    Mutex::Implement::~Implement() {
        int32_t ret = pthread_mutex_destroy(&m);
        if (ret) {
            MWL_WARN_ERRNO("destroy mutex failed", ret);
        }
    }

    int32_t Mutex::Implement::_Lock() {
        int32_t ret = pthread_mutex_lock(&m);
        if (ret) {
            MWL_WARN_ERRNO("lock mutex failed", ret);
        } else if (_traceOwner) {
            _owner = CurrentThreadID();
        }
        return -ret;
    }

    int32_t Mutex::Implement::_TryLock() {
        int32_t ret = pthread_mutex_trylock(&m);
        if (0 == ret && _traceOwner) {
            _owner = CurrentThreadID();
        }
        return -ret;
    }

    int32_t Mutex::Implement::_Unlock() {
        ThreadID owner;
        if (_traceOwner) {
            owner = _owner;
            _owner.pid = _owner.tid = -1;
        }
        int32_t ret = pthread_mutex_unlock(&m);
        if (ret) {
            MWL_WARN_ERRNO("unlock mutex failed", ret);
            if (_traceOwner) {
                _owner = owner;
            }
        }
        return -ret;
    }
}

#endif // __MWL_LINUX__
