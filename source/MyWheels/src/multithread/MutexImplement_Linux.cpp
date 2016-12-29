#ifdef __MWL_LINUX__

#include "MutexImplement.h"
#include <pthread.h>

namespace mwl {
    Mutex::Implement::Implement() {
        pthread_mutex_init(&_m, NULL);
    }

    Mutex::Implement::~Implement() {
        pthread_mutex_destroy(&_m);
    }

    int32_t Mutex::Implement::_Lock() {
        return pthread_mutex_lock(&_m);
    }

    int32_t Mutex::Implement::_TryLock() {
        return pthread_mutex_trylock(&_m);
    }

    int32_t Mutex::Implement::_Unlock() {
        return pthread_mutex_unlock(&_m);
    }
}

#endif // __MWL_LINUX__
