#include "inc/BasicDefines.h"

#ifdef __MWL_WIN__

#include "MutexImplement.h"

namespace mwl {

    Mutex::Implement::Implement() {
        InitializeCriticalSection(&m);
    }

    Mutex::Implement::~Implement() {
        DeleteCriticalSection(&m);
    }

    int32_t Mutex::Implement::_Lock() {
        EnterCriticalSection(&m);
        return 0;
    }

    int32_t Mutex::Implement::_TryLock() {
        if (TryEnterCriticalSection(&m)) {
            return 0;
        } else {
            return ERR_BUSY;
        }
    }

    int32_t Mutex::Implement::_Unlock() {
        LeaveCriticalSection(&m);
        return 0;
    }

}

#endif // __MWL_WIN__
