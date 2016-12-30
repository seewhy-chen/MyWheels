#include "inc/BasicDefines.h"

#ifdef __MWL_LINUX__

#include "MutexImplement.h"
#include <pthread.h>

namespace mwl {

    Mutex::Implement::Implement() {
        int32_t ret = pthread_mutex_init(&m, NULL);
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
            ret = -ret;
        }
        return -ret;
    }

    int32_t Mutex::Implement::_TryLock() {
        int32_t ret = pthread_mutex_trylock(&m);
        if (ret) {
            MWL_WARN_ERRNO("trylock mutex failed", ret);
            ret = -ret;
        }
        return -ret;
    }

    int32_t Mutex::Implement::_Unlock() {
        int32_t ret = pthread_mutex_unlock(&m);
        if (ret) {
            MWL_WARN_ERRNO("unlock mutex failed", ret);
            ret = -ret;
        }
        return -ret;
    }
    
}

#endif // __MWL_LINUX__
