#ifdef __MWL_WIN__

#include "MutexImplement.h"

namespace mwl {
    Mutex::Implement::Implement() {
        InitializeCriticalSection(&_m);
    }

    Mutex::Implement::~Implement() {
        DeleteCriticalSection(&_m);
    }

    int32_t Mutex::Implement::_Lock() {
        EnterCriticalSection(&_m);
        return 0;
    }

    int32_t Mutex::Implement::_TryLock() {
        if (TryEnterCriticalSection(&_m)) {
            return 0;
        } else {
            return -1;
        }
    }

    int32_t Mutex::Implement::_Unlock() {
        LeaveCriticalSection(&_m);
        return 0;
    }
}

#endif // __MWL_WIN__
